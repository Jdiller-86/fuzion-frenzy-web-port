#!/usr/bin/env node
// Verify the exact XBP slices that the private inventory labels as TTF and
// determine whether they contain a recognizable standalone SFNT/TTC font.
// This records metadata only; it never writes or embeds resource bytes.

import { createHash } from 'node:crypto';
import { readFile, writeFile } from 'node:fs/promises';
import path from 'node:path';
import { preparePrivateOutput } from './private-output.mjs';

const tags = new Set(['cmap', 'head', 'hhea', 'hmtx', 'maxp', 'loca', 'glyf', 'name', 'post', 'OS/2', 'cvt ', 'fpgm', 'prep', 'gasp', 'CFF ', 'GSUB', 'GPOS', 'GDEF', 'kern', 'vhea', 'vmtx', 'VORG', 'DSIG', 'BASE', 'JSTF', 'MATH', 'STAT', 'fvar', 'gvar', 'avar', 'HVAR', 'MVAR', 'CFF2', 'COLR', 'CPAL', 'CBDT', 'CBLC', 'sbix', 'SVG ', 'EBDT', 'EBLC']);
const signatures = new Map([
  ['\u0000\u0001\u0000\u0000', 'TrueType SFNT'],
  ['OTTO', 'OpenType CFF SFNT'],
  ['true', 'Apple TrueType SFNT'],
  ['typ1', 'PostScript Type 1 SFNT'],
]);

function usage() {
  console.error('Usage: node tools/ui/font-evidence.mjs --inventory PATH --source-tree PATH --output PATH');
  process.exit(2);
}

function parseArgs(argv) {
  const out = {};
  for (let i = 0; i < argv.length; i++) {
    const option = argv[i];
    const field = option === '--source-tree' ? 'sourceTree' : option.slice(2);
    if (!['inventory', 'sourceTree', 'output'].includes(field) || !argv[i + 1]) usage();
    out[field] = path.resolve(argv[++i]);
  }
  if (Object.keys(out).length !== 3) usage();
  return out;
}

function checkedRange(buf, offset, size, label) {
  if (!Number.isSafeInteger(offset) || !Number.isSafeInteger(size) || offset < 0 || size < 0 || offset > buf.length || size > buf.length - offset) {
    throw new Error(`${label} exceeds the containing byte range`);
  }
}

function plausibleSfnt(buf, start) {
  const signature = buf.toString('latin1', start, start + 4);
  const family = signatures.get(signature);
  if (!family || start + 12 > buf.length) return null;
  const count = buf.readUInt16BE(start + 4);
  if (count < 1 || count > 64 || start + 12 + count * 16 > buf.length) return null;
  const observed = [];
  for (let i = 0; i < count; i++) {
    const p = start + 12 + i * 16;
    const tag = buf.toString('ascii', p, p + 4);
    const offset = buf.readUInt32BE(p + 8);
    const length = buf.readUInt32BE(p + 12);
    if (!tags.has(tag) || offset > buf.length - start || length > buf.length - start - offset) return null;
    observed.push(tag);
  }
  if (!['cmap', 'head', 'maxp'].every(required => observed.includes(required))) return null;
  return { byteOffset: start, family, tableCount: count, tables: observed };
}

function scanFontContainers(bytes) {
  const candidates = [];
  const maxStart = Math.max(0, bytes.length - 12);
  for (let offset = 0; offset <= maxStart; offset++) {
    const sfnt = plausibleSfnt(bytes, offset);
    if (sfnt) candidates.push(sfnt);
  }
  const ttcOffsets = [];
  for (let offset = 0; offset + 12 <= bytes.length; offset++) {
    if (bytes.toString('ascii', offset, offset + 4) !== 'ttcf') continue;
    const count = bytes.readUInt32BE(offset + 8);
    if (count > 0 && count <= 32 && offset + 12 + count * 4 <= bytes.length) ttcOffsets.push({ byteOffset: offset, faceCount: count });
  }
  return { sfntCandidates: candidates, ttcContainers: ttcOffsets };
}

function within(root, file) {
  const rel = path.relative(root, file);
  return rel !== '' && rel !== '..' && !rel.startsWith(`..${path.sep}`) && !path.isAbsolute(rel);
}

async function main() {
  const args = parseArgs(process.argv.slice(2));
  const inventory = JSON.parse(await readFile(args.inventory, 'utf8'));
  await preparePrivateOutput(args.output, { inputs: [args.inventory, args.sourceTree] });
  if (inventory.kind !== 'private-ui-package-inventory' || !Array.isArray(inventory.resources)) throw new Error('Expected the private UI package inventory');
  const resources = inventory.resources.filter(resource => resource.format === 'ttf');
  if (!resources.length) throw new Error('No TTF-labeled resources occur in the inventory');
  const fonts = [];
  for (const resource of resources) {
    const packagePath = resource.packagePath;
    if (typeof packagePath !== 'string' || packagePath.startsWith('/') || packagePath.includes('\\') || packagePath.split('/').some(part => !part || part === '.' || part === '..')) {
      throw new Error(`Unsafe package path for ${resource.stableId}`);
    }
    const packageFile = path.resolve(args.sourceTree, ...packagePath.split('/'));
    if (!within(args.sourceTree, packageFile)) throw new Error(`Package path escapes source tree: ${packagePath}`);
    const packageBytes = await readFile(packageFile);
    checkedRange(packageBytes, resource.offset, resource.size, `XBP resource ${resource.stableId}`);
    const bytes = packageBytes.subarray(resource.offset, resource.offset + resource.size);
    const sha256 = createHash('sha256').update(bytes).digest('hex');
    if (sha256 !== resource.sha256) throw new Error(`XBP resource ${resource.stableId} digest mismatch`);
    const containers = scanFontContainers(bytes);
    fonts.push({
      stableId: resource.stableId,
      package: resource.package,
      packagePath,
      resourceName: resource.resourceName,
      packageOffset: resource.offset,
      byteLength: resource.size,
      sha256,
      resourceDigestMatchesInventory: true,
      inventoryXbpHash: resource.xbpHash,
      inventoryClassification: resource.classification,
      inventoryDecodeStatus: resource.decodeStatus,
      containerScan: containers,
      atlasEvidence: 'No atlas rectangle or UV data is exposed by the inventory; no glyph metadata is derived from the filename.',
      result: containers.sfntCandidates.length || containers.ttcContainers.length
        ? 'recognized font container is present; glyph metrics and render-path use remain unparsed'
        : 'inventory labels this resource ttf, but its exact resource slice contains no validated standalone SFNT/TTC header',
    });
  }
  const result = {
    schemaVersion: 1,
    kind: 'private-font-resource-evidence',
    source: {
      inventoryPath: args.inventory,
      selectedResourceCount: resources.length,
      corpusLimit: 'The inventory contains 14 selected candidate packages, not the full game.',
      sourceIdentity: inventory.sourceIdentity,
    },
    scope: 'Resource boundaries and hashes were checked against the inventory. A font filename alone does not establish a parseable TTF, an atlas format, or screen use.',
    retainedSdkEvidence: {
      path: 'research/windows-render-20261110/source/XboxXBERecomp/runtime/gpu/d3d12.cpp',
      functions: ['TexcoordRect', 'FillUvRectConstants'],
      sourceObservation: 'The retained renderer comment identifies a 512x512 font atlas as the menu case. For eligible draws the renderer reads float texture coordinates from the assembled vertex data, computes per-stage minima/maxima, insets by half a logical texel, and writes a normalized UV clamp rectangle.',
      observedTextureCapture: {
        path: 'research/windows-render-20261110/captures/201908/frame.txt',
        guestTextureAddress: '0x81BDC500',
        guestFormat: '0x0B',
        dimensions: { width: 512, height: 512 },
        firstRecordedDraws: [2104, 2105, 2106, 2107, 2108, 2109],
        captureScope: 'The draw records identify a stage-0 512x512 guest texture. They do not retain usable vertex UV samples for these draws, and the guest address is not joined to any selected XBP resource.',
      },
      eligibleDrawConditions: [
        'quad-list primitive, count four, four assembled vertices',
        'single-level 2D non-render-target, non-shadow, non-wide texture stage',
        'unambiguous texture-coordinate input register; float2/float4 coordinates',
        'finite, readable, non-degenerate coordinate bounds in the accepted coordinate domain',
      ],
      rectFormula: {
        texelUnits: 'uvMin=(lo+0.5)/guestSize; uvMax=(hi-0.5)/guestSize',
        normalized01: 'uvMin=lo+0.5/guestSize; uvMax=hi-0.5/guestSize',
        caveat: 'These are dynamic per-draw formulas; no retained record supplies the actual lo/hi or resulting per-glyph pixel rectangle.',
      },
      sourceMapping: 'No character-to-glyph mapping, glyph metrics, atlas cell table, XBP-resource-to-guest-address join, or original title draw-function connection is established by these retained inputs.',
    },
    limits: [
      'No complete glyph character map, advance/kerning data, glyph rectangles, atlas UVs, or original text draw function was established.',
      'No static font atlas was identified in the selected XBP resource inventory.',
      'A resource slice without a standard SFNT/TTC header may use a game-specific wrapper or encoding; this tool does not guess a decoder.',
      'The SDK host renderer can extract and clamp per-draw texture-coordinate rectangles for atlas sprites, but that does not establish original glyph-to-UV mapping.',
    ],
    fonts,
  };
  await writeFile(args.output, `${JSON.stringify(result, null, 2)}\n`, 'utf8');
  process.stdout.write(`Wrote ${fonts.length} checked font-resource records to ${args.output}\n`);
}

main().catch(error => { console.error(error instanceof Error ? error.message : String(error)); process.exitCode = 1; });
