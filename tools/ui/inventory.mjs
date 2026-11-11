#!/usr/bin/env node
// Private, bounded XBP UI resource inventory. Uses the repository's XBP and
// texture readers; output is generated locally under artifacts/ui-inventory.
import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';
import zlib from 'node:zlib';
import { execFileSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';
import { readXbpIndex } from '../../host/xbp.mjs';
import { decodeTexture } from '../../host/texture.mjs';
import { assertNoLinks, preparePrivateOutput } from './private-output.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.resolve(HERE, '../..');
const DEFAULT_OUT = path.join(ROOT, 'artifacts/ui-inventory');
const LIMIT_PACKAGE_BYTES = 128 * 1024 * 1024;
const LIMIT_RESOURCES = 100_000;
const PACKAGE_LEAD = /^(?:charactersel|generic|local|scorescreen|gamewinner|minigameintro|demosplash|com_(?:mapscreen|scorescreen|teamscreen|winscreen|zonescorescreen|frenzyintro|ingame))\.xbp$/i;
const RESOURCE_UI_LEAD = /(?:menu|pause|result|score|screen|title|cursor|button|select|frenzy|zone|map|icon|logo|atlas|font|player|route)/i;
const sha256 = bytes => crypto.createHash('sha256').update(bytes).digest('hex');

const crcTable = (() => {
  const table = new Uint32Array(256);
  for (let n = 0; n < 256; n++) {
    let c = n;
    for (let k = 0; k < 8; k++) c = (c & 1) ? 0xedb88320 ^ (c >>> 1) : c >>> 1;
    table[n] = c >>> 0;
  }
  return table;
})();

function crc32(bytes) {
  let c = 0xffffffff;
  for (const byte of bytes) c = crcTable[(c ^ byte) & 255] ^ (c >>> 8);
  return (c ^ 0xffffffff) >>> 0;
}

function pngChunk(type, data) {
  const name = Buffer.from(type, 'ascii');
  const size = Buffer.alloc(4), crc = Buffer.alloc(4);
  size.writeUInt32BE(data.length);
  crc.writeUInt32BE(crc32(Buffer.concat([name, data])));
  return Buffer.concat([size, name, data, crc]);
}

function rgbaPng(width, height, rgba) {
  const ihdr = Buffer.alloc(13);
  ihdr.writeUInt32BE(width, 0); ihdr.writeUInt32BE(height, 4);
  ihdr[8] = 8; ihdr[9] = 6;
  const rowBytes = width * 4;
  const raw = Buffer.alloc((rowBytes + 1) * height);
  for (let y = 0; y < height; y++) {
    Buffer.from(rgba.buffer, rgba.byteOffset + y * rowBytes, rowBytes).copy(raw, y * (rowBytes + 1) + 1);
  }
  return Buffer.concat([
    Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]),
    pngChunk('IHDR', ihdr),
    pngChunk('IDAT', zlib.deflateSync(raw, { level: 6 })),
    pngChunk('IEND', Buffer.alloc(0)),
  ]);
}

async function walkPackages(dir) {
  const found = [];
  for (const entry of await fs.readdir(dir, { withFileTypes: true })) {
    const fullPath = path.join(dir, entry.name);
    if (entry.isDirectory()) found.push(...await walkPackages(fullPath));
    else if (entry.isFile() && PACKAGE_LEAD.test(entry.name)) found.push(fullPath);
  }
  return found.sort((a, b) => a.toLowerCase().localeCompare(b.toLowerCase()));
}

function parseArgs(argv) {
  let source, outDir = DEFAULT_OUT;
  for (let index = 0; index < argv.length; index++) {
    const arg = argv[index];
    if (arg === '--source') {
      source = argv[++index];
      if (!source) throw new Error('--source needs a directory path.');
      source = path.resolve(source);
    } else if (arg === '--out') {
      const value = argv[++index];
      if (!value) throw new Error('--out needs a directory path.');
      outDir = path.resolve(value);
    }
    else if (arg === '--help') {
      console.log('node tools/ui/inventory.mjs --source <loose game tree> [--out <private output dir>]');
      process.exit(0);
    } else throw new Error(`Unknown argument ${arg}`);
  }
  if (!source) throw new Error('Pass --source <loose game tree>; no machine-specific source default is used.');
  if (!outDir) throw new Error('Output path is required.');
  return { source: path.resolve(source), outDir };
}

async function readBoundedFile(filePath) {
  const info = await fs.stat(filePath);
  if (!info.isFile()) throw new Error(`Expected a regular file: ${path.basename(filePath)}`);
  if (info.size > LIMIT_PACKAGE_BYTES) throw new Error(`File exceeds ${LIMIT_PACKAGE_BYTES} byte limit: ${path.basename(filePath)}`);
  const bytes = await fs.readFile(filePath);
  if (bytes.length !== info.size) throw new Error(`File size changed during read: ${path.basename(filePath)}`);
  return bytes;
}

function textureFormat(code) {
  return ({ 1: 'ARGB8888', 3: 'ARGB1555', 5: 'P8-palette-ARGB8888' })[code] ?? `xbox-swizzled-format-${code}`;
}

function alphaCounts(rgba) {
  let transparentPixels = 0;
  for (let i = 3; i < rgba.length; i += 4) if (rgba[i] !== 255) transparentPixels++;
  return { transparentPixels, opaquePixels: rgba.length / 4 - transparentPixels };
}

async function decodeEntry({ bytes, row, outDir }) {
  const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
  const textureCode = view.getUint16(0x2c, true);
  const frameCount = view.getUint8(0x37);
  const firstFrame = decodeTexture(bytes.buffer.slice(bytes.byteOffset, bytes.byteOffset + bytes.byteLength), { frame: 0 });
  row.format = textureFormat(textureCode);
  row.width = firstFrame.width;
  row.height = firstFrame.height;
  row.frameCount = frameCount;
  row.headerMipLevels = view.getUint8(0x36);
  row.headerMipEvidence = { source: 'retained texture-loader.txt', function: 'XGSetTextureHeader input', caller: '0x41890', byteOffset: '0x36' };
  row.mipChainDecoded = false;
  row.mips = { headerMipLevels: row.headerMipLevels, chainDecoded: false };
  row.compression = 'none-detected-by-supported-decoder';
  row.alpha = { meaning: 'decoded RGBA alpha channel', frames: [] };
  row.frames = [];
  row.decodeStatus = 'decoded';
  row.classification = 'texture';

  for (let frame = 0; frame < frameCount; frame++) {
    const decoded = frame === 0
      ? firstFrame
      : decodeTexture(bytes.buffer.slice(bytes.byteOffset, bytes.byteOffset + bytes.byteLength), { frame });
    const imagePath = `images/${row.stableId}-f${String(frame).padStart(2, '0')}.png`;
    const destination = path.join(outDir, imagePath);
    await assertNoLinks(destination);
    await fs.writeFile(destination, rgbaPng(decoded.width, decoded.height, decoded.rgba));
    const alpha = alphaCounts(decoded.rgba);
    row.alpha.frames.push({ index: frame, ...alpha });
    row.frames.push({ index: frame, timing: 'unknown', decoded: true, thumbnail: imagePath });
  }
  row.thumbnail = row.frames[0].thumbnail;
}

async function scanPackage({ packagePath, sourceRoot, outDir, duplicateIndex, resourceRows }) {
  const packageBytes = await readBoundedFile(packagePath);
  const blob = new Blob([packageBytes]);
  const entries = await readXbpIndex(blob);
  const rel = path.relative(sourceRoot, packagePath).replaceAll('\\', '/');
  const packageHash = sha256(packageBytes);
  const header = new DataView(packageBytes.buffer, packageBytes.byteOffset, 64);
  const tableRowCount = header.getUint32(12, true);
  const packageName = path.posix.basename(rel);
  const packageRow = {
    name: packageName, sourcePath: rel, offset: null, size: packageBytes.length, sha256: packageHash,
    tableRowCount, metadataRowsExcludedByParser: tableRowCount - entries.length,
    resourceEntriesReturnedByParser: entries.length, selectionBasis: 'filename-lead',
    classification: 'probable-ui-package',
  };

  for (const entry of entries) {
    if (resourceRows.length >= LIMIT_RESOURCES) throw new Error(`Resource limit ${LIMIT_RESOURCES} reached`);
    const bytes = packageBytes.subarray(entry.offset, entry.offset + entry.size);
    const digest = sha256(bytes);
    const duplicateKey = `${digest}:${entry.size}`;
    const stableId = sha256(Buffer.from(`${packageHash}\0${entry.name}\0${entry.offset}\0${entry.size}\0${entry.hash}`)).slice(0, 24);
    const extension = path.posix.extname(entry.name).slice(1).toLowerCase();
    const matchedPackageLead = PACKAGE_LEAD.test(packageName);
    const matchedResourceLead = RESOURCE_UI_LEAD.test(entry.name);
    const row = {
      stableId, package: packageName, packagePath: rel, resourceName: entry.name,
      offset: entry.offset, size: entry.size, xbpHash: entry.hash, sha256: digest,
      duplicateOf: duplicateIndex.get(duplicateKey) ?? null,
      classification: 'unclassified', relevance: 'unclassified',
      format: extension || 'unknown', compression: 'unknown', alpha: 'unknown',
      headerMipLevels: null, headerMipEvidence: null, mipChainDecoded: null,
      mips: 'unknown', width: null, height: null, frameCount: null,
      atlasRegions: [], frames: [], timing: 'unknown', decodeStatus: 'unsupported',
      decodeError: null, thumbnail: null, evidence: [],
    };

    if (matchedPackageLead && matchedResourceLead) {
      row.relevance = 'probable';
      row.evidence.push({ kind: 'package-and-resource-name-candidate', package: packageName, resourceName: entry.name, confidence: 'probable' });
    }
    if (row.duplicateOf) {
      row.decodeStatus = 'duplicate';
      row.classification = 'duplicate';
      row.duplicateMetadata = `reuse-resource:${row.duplicateOf}`;
      const original = duplicateIndex.rows.get(row.duplicateOf);
      if (original) {
        for (const key of ['classification', 'format', 'compression', 'alpha', 'headerMipLevels', 'headerMipEvidence', 'mipChainDecoded', 'mips', 'width', 'height', 'frameCount', 'frames', 'thumbnail', 'decodeError']) row[key] = original[key];
      }
    } else {
      duplicateIndex.set(duplicateKey, stableId);
      try {
        await decodeEntry({ bytes, row, outDir });
      } catch (error) {
        row.decodeError = String(error?.message ?? error).slice(0, 240);
      }
      duplicateIndex.rows.set(stableId, row);
    }
    resourceRows.push(row);
  }
  return packageRow;
}

async function main() {
  const { source: sourceRoot, outDir } = parseArgs(process.argv.slice(2));
  const packagePaths = await walkPackages(sourceRoot);
  if (!packagePaths.length) throw new Error(`No selected XBP packages found below ${sourceRoot}`);
  await preparePrivateOutput(outDir, { directory: true, inputs: [sourceRoot] });
  const imageDirectory = path.join(outDir, 'images');
  await assertNoLinks(imageDirectory);
  await fs.mkdir(imageDirectory, { recursive: true });

  const xbe = await readBoundedFile(path.join(sourceRoot, 'default.xbe'));
  const packages = [];
  const resources = [];
  const duplicateIndex = new Map();
  duplicateIndex.rows = new Map();
  for (const packagePath of packagePaths) {
    packages.push(await scanPackage({ packagePath, sourceRoot, outDir, duplicateIndex, resourceRows: resources }));
  }

  const imageFiles = new Set(resources.flatMap(resource => resource.frames.map(frame => frame.thumbnail).filter(Boolean))).size;
  const uniqueDecoded = resources.filter(resource => resource.decodeStatus === 'decoded').length;
  const totals = {
    decodedResources: uniqueDecoded,
    duplicateResources: resources.filter(resource => resource.decodeStatus === 'duplicate').length,
    unsupportedResources: resources.filter(resource => resource.decodeStatus === 'unsupported').length,
    unclassifiedResources: resources.filter(resource => resource.classification === 'unclassified').length,
    imageFiles,
  };
  const result = {
    schemaVersion: 1,
    kind: 'private-ui-package-inventory',
    generatedAt: new Date().toISOString(),
    sourceIdentity: {
      executable: { name: 'default.xbe', size: xbe.length, sha256: sha256(xbe) },
      packages: packages.map(({ sourcePath, size, sha256: digest }) => ({ sourcePath, size, sha256: digest })),
      sourceTree: sourceRoot,
    },
    scope: {
      packageSelection: 'filename-led bounded candidate set; selected XBP packages only',
      packageCount: packages.length,
      resourceCount: resources.length,
      completeGameInventory: false,
      screenClaims: 'No screen is confirmed by package naming. Package/name evidence remains probable or unclassified until joined to runtime/code evidence.',
      limits: [
        'Selected packages only; this is not whole-game coverage.',
        'The supported texture decoder covers only repository-documented swizzled formats 1/3/5.',
        'Frame timing, atlas regions, decoded mip chains, and font glyph metadata remain unknown unless separately evidenced.',
        'The XBP parser omits its string-table metadata row from returned resources; tableRowCount and metadataRowsExcludedByParser preserve that distinction.',
        'No original bytes are embedded in JSON; decoded PNGs are private derivatives.',
      ],
    },
    evidence: {
      repoHead: execFileSync('git', ['rev-parse', 'HEAD'], { cwd: ROOT, encoding: 'utf8' }).trim(),
      readers: ['host/xbp.mjs', 'host/texture.mjs'],
      textureHeader: 'retained texture-loader.txt: XGSetTextureHeader receives mip level byte at resource offset 0x36 from caller 0x41890',
      codeEvidence: [],
    },
    screens: packages.map(pkg => ({
      id: path.posix.basename(pkg.name, '.xbp').toLowerCase(),
      name: pkg.name,
      status: 'probable',
      relation: 'candidate-package-containment; resource-use-unproved',
      evidence: [{ kind: 'package-name', value: pkg.name, confidence: 'probable' }],
      resources: resources.filter(resource => resource.packagePath === pkg.sourcePath).map(resource => resource.stableId),
    })),
    packages,
    resources,
    totals,
  };
  const inventoryPath = path.join(outDir, 'inventory.json');
  await assertNoLinks(inventoryPath);
  await fs.writeFile(inventoryPath, `${JSON.stringify(result, null, 2)}\n`);
  console.log(JSON.stringify({ output: path.join(outDir, 'inventory.json'), packages: packages.length, resources: resources.length, ...totals, executableSha256: result.sourceIdentity.executable.sha256 }, null, 2));
}

main().catch(error => { console.error(error.message); process.exitCode = 1; });
