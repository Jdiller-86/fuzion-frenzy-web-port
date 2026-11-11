#!/usr/bin/env node
// Bounded metadata-only inspection of every census row whose resource name
// ends in .ttf. It verifies package and slice identities, checks standard
// SFNT/TTC signatures, and probes only recognized gzip/zlib wrappers.
import { createHash } from 'node:crypto';
import { createReadStream } from 'node:fs';
import { readFile, writeFile } from 'node:fs/promises';
import path from 'node:path';
import zlib from 'node:zlib';
import { preparePrivateOutput } from './private-output.mjs';

const MAX_PACKAGE_BYTES = 128 * 1024 * 1024;
const MAX_DECOMPRESSED_BYTES = 16 * 1024 * 1024;
const SFNT_SIGNATURES = new Map([
  ['\x00\x01\x00\x00', 'TrueType SFNT'], ['OTTO', 'OpenType CFF SFNT'],
  ['true', 'Apple TrueType SFNT'], ['typ1', 'PostScript Type 1 SFNT'],
]);
const SFNT_TAGS = new Set([
  'cmap','head','hhea','hmtx','maxp','loca','glyf','name','post','OS/2','cvt ','fpgm','prep','gasp','CFF ','GSUB','GPOS','GDEF','kern','vhea','vmtx','VORG','DSIG','BASE','JSTF','MATH','STAT','fvar','gvar','avar','HVAR','MVAR','CFF2','COLR','CPAL','CBDT','CBLC','sbix','SVG ','EBDT','EBLC',
]);

function usage() {
  console.error('Usage: node tools/ui/font-recovery.mjs --census PATH --source-references PATH --source-tree PATH --output PATH');
  process.exit(2);
}

function argsOf(argv) {
  const args = {};
  for (let i = 0; i < argv.length; i++) {
    const key = argv[i] === '--source-tree' ? 'sourceTree' : argv[i] === '--source-references' ? 'sourceReferences' : argv[i].slice(2);
    if (!['census','sourceReferences','sourceTree','output'].includes(key) || !argv[i + 1]) usage();
    args[key] = path.resolve(argv[++i]);
  }
  if (Object.keys(args).length !== 4) usage();
  return args;
}

function sha256(bytes) { return createHash('sha256').update(bytes).digest('hex'); }

async function fileSha256(file) {
  const hash = createHash('sha256');
  for await (const chunk of createReadStream(file)) hash.update(chunk);
  return hash.digest('hex');
}

function contained(root, file) {
  const relative = path.relative(root, file);
  return relative !== '' && relative !== '..' && !relative.startsWith(`..${path.sep}`) && !path.isAbsolute(relative);
}

function checkedRange(bytes, offset, size, label) {
  if (!Number.isSafeInteger(offset) || !Number.isSafeInteger(size) || offset < 0 || size < 0 || offset > bytes.length || size > bytes.length - offset) {
    throw new Error(`${label} exceeds its package range`);
  }
}

function parseSfnt(bytes, start) {
  if (start + 12 > bytes.length) return null;
  const family = SFNT_SIGNATURES.get(bytes.toString('latin1', start, start + 4));
  if (!family) return null;
  const count = bytes.readUInt16BE(start + 4);
  if (count < 1 || count > 64 || start + 12 + count * 16 > bytes.length) return null;
  const tables = [];
  for (let i = 0; i < count; i++) {
    const p = start + 12 + i * 16;
    const tag = bytes.toString('ascii', p, p + 4);
    const offset = bytes.readUInt32BE(p + 8), length = bytes.readUInt32BE(p + 12);
    if (!SFNT_TAGS.has(tag) || offset > bytes.length - start || length > bytes.length - start - offset) return null;
    tables.push(tag);
  }
  if (!['cmap','head','maxp'].every(tag => tables.includes(tag))) return null;
  return { byteOffset: start, family, tableCount: count, tables };
}

function findContainers(bytes) {
  const sfnt = [], ttc = [], compressed = [];
  for (let i = 0; i + 12 <= bytes.length; i++) {
    const candidate = parseSfnt(bytes, i);
    if (candidate) sfnt.push(candidate);
    if (bytes.toString('ascii', i, i + 4) === 'ttcf') {
      const count = bytes.readUInt32BE(i + 8);
      if (count > 0 && count <= 32 && i + 12 + count * 4 <= bytes.length) ttc.push({ byteOffset: i, faceCount: count });
    }
    if (bytes[i] === 0x1f && bytes[i + 1] === 0x8b) {
      try {
        const inflated = zlib.gunzipSync(bytes.subarray(i), { maxOutputLength: MAX_DECOMPRESSED_BYTES });
        compressed.push({ byteOffset: i, wrapper: 'gzip', decompressedBytes: inflated.length, sha256: sha256(inflated), sfnt: findRawSfnt(inflated) });
      } catch { /* Signature may occur in unrelated data or be truncated. */ }
    }
    if (bytes[i] === 0x78 && i + 1 < bytes.length && ((bytes[i] << 8) + bytes[i + 1]) % 31 === 0) {
      try {
        const inflated = zlib.inflateSync(bytes.subarray(i), { maxOutputLength: MAX_DECOMPRESSED_BYTES });
        compressed.push({ byteOffset: i, wrapper: 'zlib', decompressedBytes: inflated.length, sha256: sha256(inflated), sfnt: findRawSfnt(inflated) });
      } catch { /* Header shape alone does not establish a zlib stream. */ }
    }
  }
  return { sfnt, ttc, compressed };
}

function findRawSfnt(bytes) {
  const found = [];
  for (let i = 0; i + 12 <= bytes.length; i++) {
    const result = parseSfnt(bytes, i);
    if (result) found.push(result);
    if (bytes.toString('ascii', i, i + 4) === 'ttcf') {
      const count = bytes.readUInt32BE(i + 8);
      if (count > 0 && count <= 32 && i + 12 + count * 4 <= bytes.length) found.push({ byteOffset: i, family: 'TrueType Collection', faceCount: count });
    }
  }
  return found;
}

async function main() {
  const args = argsOf(process.argv.slice(2));
  const census = JSON.parse(await readFile(args.census, 'utf8'));
  const sourceReferences = JSON.parse(await readFile(args.sourceReferences, 'utf8'));
  await preparePrivateOutput(args.output, { inputs: [args.census, args.sourceReferences, args.sourceTree] });
  if (census.kind !== 'private-xbp-package-census' || !Array.isArray(census.resources) || !Array.isArray(census.packages)) throw new Error('Expected the private XBP package census');
  const xbePath = path.join(args.sourceTree, 'default.xbe');
  const xbeHash = await fileSha256(xbePath);
  const expectedXbeHash = census.sourceIdentity?.executable?.sha256;
  if (!expectedXbeHash || xbeHash !== expectedXbeHash) throw new Error('Source-tree XBE identity does not match the census');
  if (sourceReferences.kind !== 'private-ui-source-reference-index' || sourceReferences.sourceIdentity?.xbe?.sha256 !== xbeHash) throw new Error('Source-reference index XBE identity does not match the supplied tree');
  const packageByPath = new Map(census.packages.map(pkg => [pkg.path, pkg]));
  const referenceByKey = new Map((sourceReferences.resources ?? []).map(resource => [`${resource.packagePath}\0${resource.resourceName}`, resource]));
  const resources = census.resources.filter(resource => typeof resource.resourceName === 'string' && /\.ttf$/i.test(resource.resourceName));
  if (!resources.length) throw new Error('No case-insensitive .ttf resource suffixes occur in the census');
  const packageHashes = new Map(), results = [];
  for (const resource of resources) {
    const rel = resource.packagePath;
    if (typeof rel !== 'string' || rel.startsWith('/') || rel.includes('\\') || rel.split('/').some(part => !part || part === '.' || part === '..')) throw new Error(`Unsafe census package path: ${rel}`);
    const packageFile = path.resolve(args.sourceTree, ...rel.split('/'));
    if (!contained(args.sourceTree, packageFile)) throw new Error(`Package path escapes source tree: ${rel}`);
    const packageMeta = packageByPath.get(rel);
    if (!packageMeta) throw new Error(`Census lacks package identity for ${rel}`);
    if (!packageHashes.has(rel)) {
      if (packageMeta.size > MAX_PACKAGE_BYTES) throw new Error(`Package exceeds bounded hash limit: ${rel}`);
      packageHashes.set(rel, await fileSha256(packageFile));
    }
    if (packageHashes.get(rel) !== packageMeta.sha256) throw new Error(`Package digest mismatch: ${rel}`);
    const packageBytes = await readFile(packageFile);
    checkedRange(packageBytes, resource.offset, resource.size, `Resource ${resource.stableId}`);
    const bytes = packageBytes.subarray(resource.offset, resource.offset + resource.size);
    let firstNonzero = 0;
    while (firstNonzero < bytes.length && bytes[firstNonzero] === 0) firstNonzero++;
    if (firstNonzero === bytes.length) firstNonzero = null;
    let lastNonzero = bytes.length - 1;
    while (lastNonzero >= 0 && bytes[lastNonzero] === 0) lastNonzero--;
    const containers = findContainers(bytes);
    const textureProbe = bytes.length >= 0x780 + 0x38 ? {
      resourceOffset: 0x780,
      nativeConsumer: 'FUN_00041890',
      fieldsReadByConsumer: {
        plus0x28_u32: bytes.readUInt32LE(0x780 + 0x28),
        plus0x2c_u16: bytes.readUInt16LE(0x780 + 0x2c),
        plus0x2e_u16: bytes.readUInt16LE(0x780 + 0x2e),
        plus0x30_u16: bytes.readUInt16LE(0x780 + 0x30),
        plus0x36_u8: bytes[0x780 + 0x36],
        plus0x37_u8: bytes[0x780 + 0x37],
      },
      switchCase5Behavior: 'For +0x2c == 5, the decompiled consumer passes format 0x0b and 8 bits per pixel to XGSetTextureHeader.',
    } : null;
    const selected = resource.selectedInventoryMatch === true;
    const refs = referenceByKey.get(`${rel}\0${resource.resourceName}`);
    results.push({
      stableId: resource.stableId, package: resource.package, packagePath: rel,
      resourceName: resource.resourceName, packageOffset: resource.offset, byteLength: bytes.length,
      xbpHash: resource.xbpHash, censusClassification: resource.classification,
      censusStatus: resource.status, selectedInventoryMatch: selected,
      packageSha256: packageHashes.get(rel), resourceSha256: sha256(bytes),
      identityChecks: { xbeMatchesCensus: true, packageMatchesCensus: true, rangeWithinPackage: true },
      byteShape: { firstNonzeroByteOffset: firstNonzero, lastNonzeroByteOffset: lastNonzero, nonzeroBytes: bytes.reduce((n, value) => n + (value !== 0), 0) },
      privateLayoutProbe: {
        firstNonzeroWindowHex: firstNonzero === null ? '' : bytes.subarray(firstNonzero, Math.min(bytes.length, firstNonzero + 32)).toString('hex'),
        bytesAfterCommonPrefix42Hex: bytes.length > 42 ? bytes.subarray(42, Math.min(bytes.length, 42 + 64)).toString('hex') : null,
        bytesAtOffset0x780Hex: bytes.length > 0x780 ? bytes.subarray(0x780, Math.min(bytes.length, 0x780 + 64)).toString('hex') : null,
        nativeTextureConsumerFieldProbe: textureProbe,
        note: 'Raw bytes are recorded only in this ignored private report. The named offsets mirror reads in the retained FUN_00041890 decompilation; they do not identify glyph semantics.',
      },
      recognizedContainers: containers,
      staticSourceReferences: refs ? {
        indexed: true, status: refs.status,
        xbeStringHits: refs.xbeStringHits,
        generatedReferenceHits: refs.generatedReferenceHits.map(hit => ({
          guestInstructionAddress: hit.guestInstructionAddress,
          functionLabel: hit.functionLabel,
          resourceVirtualAddress: hit.resourceVirtualAddress,
          nearbyCallTargets: hit.nearbyCalls.map(call => ({ guestInstructionAddress: call.guestInstructionAddress, target: call.target })),
        })),
        limit: 'Static pointers and calls to the common resource loader do not establish execution, returned-resource interpretation, or text drawing.',
      } : { indexed: false, reason: 'The selected-package source-reference index does not cover this census-only resource row.' },
      result: containers.sfnt.length || containers.ttc.length || containers.compressed.some(item => item.sfnt.length)
        ? 'standard font container or compressed standard-font container found; glyph metrics and runtime use not examined'
        : 'no validated standalone SFNT/TTC or gzip/zlib-wrapped SFNT/TTC found in the exact indexed slice',
    });
  }
  const packages = [...new Set(results.map(item => item.packagePath))].sort();
  const selectedCount = results.filter(item => item.selectedInventoryMatch).length;
  const output = {
    schemaVersion: 1,
    kind: 'private-xbp-font-recovery-probe',
    source: { censusPath: args.census, sourceTree: args.sourceTree, xbeSha256: xbeHash, censusSelectedPackageCount: census.scope?.selectedInventoryPackages ?? null },
    scope: 'Every census resource name ending case-insensitively in .ttf was range checked against a full-package hash. Only standard SFNT/TTC and gzip/zlib wrappers were probed; the scan does not identify a proprietary bitmap-font format.',
    totals: { resourceRows: results.length, distinctPackages: packages.length, selectedInventoryRows: selectedCount, additionalCensusRows: results.length - selectedCount, recognizedStandaloneFonts: results.filter(item => item.recognizedContainers.sfnt.length || item.recognizedContainers.ttc.length).length, recognizedWrappedFonts: results.filter(item => item.recognizedContainers.compressed.some(stream => stream.sfnt.length)).length },
    limits: [
      'Census names are leads and do not prove UI use or a renderer load path.',
      'No XBP payload decoder exists in the retained host XBP reader; package and byte-range verification do not identify the internal payload format.',
      'The scan recognizes only standard SFNT/TTC and gzip/zlib wrappers. A failed probe is not evidence that the resource is not a font.',
      'No resource is rendered, copied into output, or interpreted as a glyph atlas; text IDs, glyph mapping, metrics and per-draw UVs remain unknown.',
    ],
    resources: results,
  };
  await writeFile(args.output, `${JSON.stringify(output, null, 2)}\n`, 'utf8');
  process.stdout.write(`Wrote ${results.length} font-name lead records across ${packages.length} packages to ${args.output}\n`);
}

main().catch(error => { console.error(error instanceof Error ? error.message : String(error)); process.exitCode = 1; });
