#!/usr/bin/env node
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { assertNoLinks, preparePrivateOutput } from './private-output.mjs';

const here = path.dirname(fileURLToPath(import.meta.url));
function args(argv) {
  const raw = {};
  for (let i = 0; i < argv.length; i++) {
    const key = argv[i];
    if (!['--inventory', '--out'].includes(key) || !argv[i + 1]) throw new Error('Usage: node catalog.mjs --inventory <absolute inventory.json> --out <absolute output directory>');
    raw[key.slice(2)] = argv[++i];
  }
  if (!raw.inventory || !raw.out || !path.isAbsolute(raw.inventory) || !path.isAbsolute(raw.out)) throw new Error('Both --inventory and --out absolute paths are required.');
  return { inventory: path.resolve(raw.inventory), out: path.resolve(raw.out) };
}
function safeRelative(value) {
  if (typeof value !== 'string' || path.isAbsolute(value) || value.split(/[\\/]/).some(p => !p || p === '.' || p === '..')) throw new Error(`Unsafe inventory image path: ${value}`);
  return value.replaceAll('\\', '/');
}
function escape(s) { return String(s).replace(/[&<>"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&apos;'}[c])); }
async function prepareOutputFile(file) {
  await assertNoLinks(file);
  fs.mkdirSync(path.dirname(file), { recursive: true });
  await assertNoLinks(file);
}
async function writeOutputFile(file, data) {
  await prepareOutputFile(file);
  fs.writeFileSync(file, data);
}
async function copyOutputFile(source, file) {
  await prepareOutputFile(file);
  fs.copyFileSync(source, file);
}
function svgSheet(title, entries, outputRoot, width = 1000) {
  const cellW = 180, cellH = 164, cols = 5, rows = Math.max(1, Math.ceil(entries.length / cols));
  const height = 72 + rows * cellH;
  const cells = entries.map((e, i) => {
    const x = (i % cols) * cellW + 8, y = Math.floor(i / cols) * cellH + 66;
    const data = fs.readFileSync(path.join(outputRoot, e.thumbnail)).toString('base64');
    return `<g><rect x="${x}" y="${y}" width="164" height="128" rx="4" fill="#eee" stroke="#bbb"/><image href="data:image/png;base64,${data}" x="${x + 4}" y="${y + 4}" width="156" height="100" preserveAspectRatio="xMidYMid meet"/><text x="${x + 4}" y="${y + 116}" font-size="10" font-family="sans-serif">${escape(e.resourceName || e.stableId)}</text><text x="${x + 4}" y="${y + 128}" font-size="9" fill="#555" font-family="sans-serif">${escape(e.stableId)} · ${escape(e.width || '?')}×${escape(e.height || '?')}</text></g>`;
  }).join('');
  return `<svg xmlns="http://www.w3.org/2000/svg" width="${width}" height="${height}" viewBox="0 0 ${width} ${height}"><rect width="100%" height="100%" fill="white"/><text x="14" y="30" font-size="18" font-family="sans-serif">${escape(title)}</text><text x="14" y="50" font-size="11" fill="#555" font-family="sans-serif">${entries.length} private decoded thumbnails · source identity ${escape('see catalog.json')}</text>${cells}</svg>`;
}
async function main() {
  const { inventory: invPath, out } = args(process.argv.slice(2));
  await assertNoLinks(invPath);
  const inv = JSON.parse(fs.readFileSync(invPath, 'utf8'));
  if (!Array.isArray(inv.resources) || !Array.isArray(inv.screens)) throw new Error('Inventory must contain resources and screens arrays.');
  const imageRoot = path.dirname(invPath);
  await preparePrivateOutput(out, { directory: true, inputs: [invPath, imageRoot, inv.sourceIdentity?.sourceTree] });
  const approved = new Set();
  const copyImage = async rel => {
      if (!rel) return null;
      rel = safeRelative(rel);
      const source = path.resolve(imageRoot, rel);
      if (!source.startsWith(path.resolve(imageRoot) + path.sep) || !fs.existsSync(source) || !fs.statSync(source).isFile()) return null;
      await assertNoLinks(source);
      const destRel = rel;
      if (!approved.has(destRel)) { await copyOutputFile(source, path.join(out, destRel)); approved.add(destRel); }
      return destRel;
  };
  const resources = [];
  for (const r of inv.resources) {
    const regions = Array.isArray(r.atlasRegions) ? r.atlasRegions : [];
    resources.push({ ...r, thumbnail: await copyImage(r.thumbnail), frames: await Promise.all((r.frames || []).map(async f => ({ ...f, thumbnail: await copyImage(f.thumbnail) }))), atlasRegions: regions,
      atlasRegionsStatus: regions.length ? 'known-evidence' : 'unknown-no-regions-provided' });
  }
  const resourceIds = new Set(resources.map(r => r.stableId));
  const candidateScreens = inv.screens.map(s => ({ ...s, id: `candidate-package:${s.id}`, name: `Candidate package: ${s.name}`, basis: 'package-name candidate; resource use unproved', resources: (s.resources || []).filter(id => resourceIds.has(id)) }));
  let screens = candidateScreens;
  const optional = {};
  for (const name of ['captures/observations.json', 'evidence-joins.json', 'package-census.json']) {
    const p = path.join(path.dirname(invPath), name);
    if (fs.existsSync(p) && fs.statSync(p).isFile()) {
      await assertNoLinks(p);
      try { optional[name] = JSON.parse(fs.readFileSync(p, 'utf8')); } catch { optional[name] = { status: 'present-but-invalid-json' }; }
    }
  }
  const observations = optional['captures/observations.json'];
  if (Array.isArray(observations)) {
    for (const observation of observations) {
      if (typeof observation.capture !== 'string' || !observation.capture.startsWith('captures/')) continue;
      const rel = safeRelative(observation.capture);
      const source = path.resolve(path.dirname(invPath), rel);
      if (!source.startsWith(path.resolve(path.dirname(invPath)) + path.sep) || !fs.existsSync(source) || !fs.statSync(source).isFile()) continue;
      await assertNoLinks(source);
      await copyOutputFile(source, path.join(out, rel)); observation.localCapture = rel;
    }
  }
  let joins = optional['evidence-joins.json'];
  if (joins?.resources) {
    const joinedResources = Array.isArray(joins.resources) ? joins.resources : Object.entries(joins.resources).map(([stableId, value]) => ({ stableId, ...value }));
    const byId = new Map(joinedResources.filter(x => x?.stableId).map(x => [x.stableId, x]));
    for (const r of resources) if (byId.has(r.stableId)) {
      r.joinedEvidence = byId.get(r.stableId);
      if (Array.isArray(r.joinedEvidence.atlasRegions) && r.joinedEvidence.atlasRegions.length) {
        r.atlasRegions = r.joinedEvidence.atlasRegions;
        r.atlasRegionsStatus = 'geometry-derived-texture-rectangles-atlas-membership-unknown';
      }
    }
    const joinedScreens = Array.isArray(joins.screens) ? joins.screens : Object.entries(joins.screens || {}).map(([id, value]) => ({ id, ...value }));
    const validJoinedScreens = joinedScreens.map(s => ({ ...s, evidenceKind: 'runtime/code-associated screen evidence', resources: (s.resources || []).filter(id => resourceIds.has(id)) }));
    for (const s of validJoinedScreens) {
      s.joinedEvidence = { confidence: s.confidence, status: s.status, evidence: s.evidence, captures: s.captures, codeReferences: s.codeReferences };
    }
    screens = [...candidateScreens, ...validJoinedScreens];
  }
  const catalog = { schemaVersion: 1, generatedAt: new Date().toISOString(), sourceIdentity: inv.sourceIdentity, scope: inv.scope, totals: inv.totals,
    packages: inv.packages, screens, resources, observations: optional['captures/observations.json'] || null, evidenceJoinsAvailable: Boolean(joins), packageCensusScope: optional['package-census.json']?.scope || null };
  await writeOutputFile(path.join(out, 'catalog.json'), JSON.stringify(catalog, null, 2));
  const byPackage = new Map();
  for (const r of resources.filter(r => r.thumbnail)) { const list = byPackage.get(r.package) || []; list.push(r); byPackage.set(r.package, list); }
  const sheetsDir = path.join(out, 'contact-sheets');
  const sheetList = [];
  for (const [pkg, list] of byPackage) {
    const file = `${pkg.replace(/[^a-z0-9._-]/gi, '_')}.svg`;
    for (const r of list) await assertNoLinks(path.join(out, r.thumbnail));
    await writeOutputFile(path.join(sheetsDir, file), svgSheet(pkg, list, out)); sheetList.push({ package: pkg, file: `contact-sheets/${file}`, count: list.length });
  }
  const links = sheetList.map(s => `<li><a href="${escape(path.basename(s.file))}"><img loading="lazy" src="${escape(path.basename(s.file))}" alt="Contact sheet for ${escape(s.package)}"><span>${escape(s.package)} · ${escape(s.count)} assets</span></a></li>`).join('');
  await writeOutputFile(path.join(sheetsDir, 'index.html'), `<!doctype html><html lang="en"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>Private Contact Sheets</title><style>body{font:16px system-ui;background:#111923;color:#e8edf4;margin:20px}ul{display:grid;grid-template-columns:repeat(auto-fill,minmax(340px,1fr));gap:16px;list-style:none;padding:0}a{color:#9dd3ff}img{display:block;width:100%;border:1px solid #627080;background:white}span{display:block;padding:6px}</style><h1>Printable Private Contact Sheets</h1><ul>${links}</ul></html>`);
  await writeOutputFile(path.join(out, 'contact-sheets.json'), JSON.stringify(sheetList, null, 2));
  const staticFiles = ['index.html', 'client.mjs', 'styles.css', 'catalog.json', 'contact-sheets.json'];
  if (optional['package-census.json']) {
    await writeOutputFile(path.join(out, 'package-census.json'), JSON.stringify(optional['package-census.json'], null, 2));
    staticFiles.push('package-census.json');
  }
  const allow = [...staticFiles, ...approved, ...sheetList.map(x => x.file), 'contact-sheets/index.html'];
  if (observations) {
    await writeOutputFile(path.join(out, 'captures', 'observations.json'), JSON.stringify(observations, null, 2)); allow.push('captures/observations.json');
    for (const observation of observations) if (observation.localCapture) allow.push(observation.localCapture);
  }
  await writeOutputFile(path.join(out, 'catalog-manifest.json'), JSON.stringify({ schemaVersion: 1, files: allow }, null, 2));
  for (const file of ['catalog-page.html', 'catalog-client.mjs', 'styles.css']) {
    const source = path.join(here, file); if (fs.existsSync(source)) await copyOutputFile(source, path.join(out, file === 'catalog-page.html' ? 'index.html' : file === 'catalog-client.mjs' ? 'client.mjs' : file));
  }
  console.log(JSON.stringify({ out, resources: resources.length, screens: screens.length, copiedImages: approved.size, packages: byPackage.size, observations: Boolean(catalog.observations), evidenceJoinsAvailable: catalog.evidenceJoinsAvailable }, null, 2));
}
main().catch(e => { console.error(e.message); process.exitCode = 1; });
