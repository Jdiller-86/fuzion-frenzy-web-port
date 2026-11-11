// Join independently retained code, geometry, resource and runtime records.
// A filename or a shader address alone never confirms a UI asset.
import fs from 'node:fs/promises';
import path from 'node:path';
import { preparePrivateOutput } from './private-output.mjs';
const args = {};
for (let i = 2; i < process.argv.length; i += 2) {
  if (!['--inventory', '--overlays', '--references', '--components', '--out'].includes(process.argv[i]) || !process.argv[i + 1]) throw new Error('Pass --inventory --overlays --references --components --out paths');
  args[process.argv[i].slice(2)] = path.resolve(process.argv[i + 1]);
}
for (const key of ['inventory', 'overlays', 'references', 'components', 'out']) if (!args[key]) throw new Error(`Missing --${key}`);
const read = async file => JSON.parse(await fs.readFile(file, 'utf8'));
const [inventory, overlays, references, components] = await Promise.all([read(args.inventory), read(args.overlays), read(args.references), read(args.components)]);
if (inventory.sourceIdentity.executable.sha256 !== overlays.sourceIdentity.executableSha256 || inventory.sourceIdentity.executable.sha256 !== references.sourceIdentity.xbe.sha256) throw new Error('Evidence XBE identities disagree');
const sourceById = new Map(references.resources.map(row => [row.stableId, row]));
const byId = new Map(inventory.resources.map(resource => [resource.stableId, {
  stableId: resource.stableId, confidence: resource.relevance,
  classification: resource.classification, screens: [], atlasRegions: [],
  codeReferences: sourceById.get(resource.stableId)?.generatedReferenceHits || [],
  evidence: [], components: []
}]));
const screens = components.screens.map(screen => ({ ...screen, resources: [], confidence: screen.status, evidence: [] }));
for (const object of overlays.resources) {
  // Confirm a screen root only when both the curated screen binding and the
  // independently scanned instruction operand + following loader call agree.
  const screen = screens.find(screen => screen.assetRoots.some(id => object.inventoryStableIds.includes(id)));
  if (!screen) continue;
  const rootId = object.inventoryStableIds.find(id => screen.assetRoots.includes(id));
  const source = sourceById.get(rootId);
  const hit = source?.generatedReferenceHits.find(hit => hit.nearbyCalls.some(call => screen.loadSites.includes(call.guestInstructionAddress) && call.target === screen.loader));
  if (!hit) throw new Error(`No independently scanned load site for ${screen.id}`);
  const root = byId.get(rootId);
  root.confidence = 'confirmed'; root.classification = 'ui-layout-resource';
  root.screens.push({ id: screen.id, name: screen.name, confidence: 'confirmed-static-resource-request; screen observed separately' });
  screen.resources.push(rootId); screen.evidence.push({ source: 'source-references.json', resource: rootId, guestInstructionAddress: hit.guestInstructionAddress, calls: hit.nearbyCalls });
  for (const leaf of object.leafViews) for (const batch of leaf.geometry?.batches || []) {
    if (batch.textureMatchStatus !== 'unique-decoded-hash-match' || !batch.strictAxisAlignedUvQuad?.isQuad || batch.textureMatches.length !== 1) continue;
    const match = batch.textureMatches[0], resource = byId.get(match.stableId);
    if (!resource) throw new Error('Geometry join references an absent resource');
    resource.confidence = 'confirmed'; resource.classification = 'ui-texture';
    resource.screens.push({ id: screen.id, name: screen.name, confidence: 'confirmed-static-material-binding; no per-draw runtime texture identity claimed' });
    const uv = batch.textureRectangle.uvBounds, px = batch.textureRectangle.pixelBounds;
    const region = { label: `${screen.id} leaf ${leaf.nodeOffset}`, pixelRect: [px.left, px.top, px.right - px.left, px.bottom - px.top], uvRect: [uv.minU, uv.minV, uv.maxU - uv.minU, uv.maxV - uv.minV], type: 'geometry-texture-rectangle', semanticAtlasMembership: 'not established; rectangle may be entire texture', evidence: { object: rootId, nodeOffset: leaf.nodeOffset, hash: batch.textureHash, source: 'overlay-evidence.json', coordinateScope: leaf.boundsScope } };
    resource.atlasRegions.push(region); resource.evidence.push(region.evidence); screen.resources.push(resource.stableId);
  }
}
for (const component of components.components) for (const id of component.assets) {
  const row = byId.get(id);
  if (!row) throw new Error(`Component ${component.id} references missing asset ${id}`);
  row.components.push({ id: component.id, existence: component.existence, binding: component.assetBinding });
  if (!row.screens.length) for (const screenId of component.screens) {
    const screen = screens.find(s => s.id === screenId);
    if (screen) row.screens.push({ id: screenId, name: screen.name, confidence: 'probable-component-binding; runtime control existence alone does not confirm this texture' });
  }
  // A curated candidate binding is a probable use, never a confirmation.
  if (row.confidence === 'unclassified') row.confidence = 'probable';
}
for (const screen of screens) {
  for (const row of byId.values()) if (row.screens.some(s => s.id === screen.id)) screen.resources.push(row.stableId);
  screen.resources = [...new Set(screen.resources)];
}
const resources = [...byId.values()];
const result = { schemaVersion: 1, kind: 'private-ui-evidence-joins', generatedAt: new Date().toISOString(), executableSha256: inventory.sourceIdentity.executable.sha256,
  limits: ['Confirmed means a named UI layout request or an unambiguous material binding in that layout; it does not mean a captured runtime draw was matched to the package.', 'UV rectangles are exact geometry-derived bounds; font atlas membership, glyph semantics and timing remain unknown.', 'Other component bindings are probable and clearly labeled.'],
  summary: { resources: resources.length, confirmed: resources.filter(r => r.confidence === 'confirmed').length, probable: resources.filter(r => r.confidence === 'probable').length, unclassified: resources.filter(r => r.confidence === 'unclassified').length, knownRectangles: resources.reduce((sum, r) => sum + r.atlasRegions.length, 0) }, resources, screens };
await preparePrivateOutput(args.out, { inputs: [args.inventory, args.overlays, args.references, args.components, inventory.sourceIdentity.sourceTree] });
await fs.writeFile(args.out, JSON.stringify(result, null, 2) + '\n');
console.log(JSON.stringify({ output: args.out, ...result.summary }, null, 2));
