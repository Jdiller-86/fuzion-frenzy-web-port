#!/usr/bin/env node
// Private, read-only index connecting exact resource names in an XBE with
// address literals in its generated recompilation source. A hit is a static
// reference, not proof that a screen loads or displays that resource.
import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';
import { readXbeProfile } from '../../host/xbe.mjs';
import { preparePrivateOutput, assertNoLinks } from './private-output.mjs';

const MAX_XBE_BYTES = 64 * 1024 * 1024;
const MAX_INVENTORY_BYTES = 32 * 1024 * 1024;
const MAX_GENERATED_FILES = 128;
const MAX_GENERATED_FILE_BYTES = 128 * 1024 * 1024;
const MAX_GENERATED_TOTAL_BYTES = 1024 * 1024 * 1024;
const sha256 = bytes => crypto.createHash('sha256').update(bytes).digest('hex');
const hex = value => `0x${value.toString(16).toUpperCase().padStart(8, '0')}`;

function usage() {
  return 'node tools/ui/source-evidence.mjs --xbe <path> --generated-dir <dir> --inventory <json> --out <json> [--vtables <json>]';
}

function parseArgs(argv) {
  const values = {};
  for (let i = 0; i < argv.length; i++) {
    const arg = argv[i];
    if (arg === '--help') {
      console.log(usage());
      process.exit(0);
    }
    if (!['--xbe', '--generated-dir', '--inventory', '--out', '--vtables'].includes(arg)) throw new Error(`Unknown argument ${arg}`);
    const value = argv[++i];
    if (!value) throw new Error(`${arg} needs a path`);
    values[arg.slice(2)] = path.resolve(value);
  }
  for (const key of ['xbe', 'generated-dir', 'inventory', 'out']) {
    if (!values[key]) throw new Error(`Missing --${key}`);
  }
  return values;
}

async function boundedRead(filePath, limit) {
  const stat = await fs.stat(filePath);
  if (!stat.isFile()) throw new Error(`Expected regular file: ${filePath}`);
  if (stat.size > limit) throw new Error(`Input exceeds ${limit} byte cap: ${filePath}`);
  const bytes = await fs.readFile(filePath);
  if (bytes.length !== stat.size) throw new Error(`Input changed while reading: ${filePath}`);
  return bytes;
}

function mapXbe(bytes) {
  const buffer = bytes.buffer.slice(bytes.byteOffset, bytes.byteOffset + bytes.byteLength);
  const profile = readXbeProfile(buffer);
  const vaToOffset = (address, size = 1) => {
    if (address >= profile.base && address - profile.base < profile.headerBytes && size <= profile.headerBytes - (address - profile.base)) {
      return address - profile.base;
    }
    const section = profile.sections.find(item => address >= item.address && address - item.address <= item.fileBytes && size <= item.fileBytes - (address - item.address));
    return section ? section.fileOffset + address - section.address : null;
  };
  const offsetToLocation = offset => {
    if (offset >= 0 && offset < profile.headerBytes) return { virtualAddress: hex(profile.base + offset), sectionIndex: null, ambiguousSectionIndices: [] };
    const matches = profile.sections.filter(section => offset >= section.fileOffset && offset < section.fileOffset + section.fileBytes);
    if (!matches.length) return null;
    const section = matches[0];
    return {
      virtualAddress: hex(section.address + offset - section.fileOffset),
      sectionIndex: section.index,
      ambiguousSectionIndices: matches.length > 1 ? matches.map(item => item.index) : [],
    };
  };
  return { ...profile, vaToOffset, offsetToLocation };
}

function findAll(bytes, needle) {
  const hits = [];
  let offset = 0;
  while (true) {
    offset = bytes.indexOf(needle, offset);
    if (offset < 0) return hits;
    hits.push(offset);
    offset += Math.max(1, needle.length);
  }
}

function extractFunctionLabel(line) {
  const match = line.match(/^\s*X86_FUNC\(__imp__(sub_[0-9A-Fa-f]+)\)/);
  return match ? match[1] : null;
}

function instructionFromLine(line) {
  const match = line.match(/^\s*\/\/\s*([0-9A-Fa-f]{8})\s+(.+?)\s*$/);
  return match ? { address: Number.parseInt(match[1], 16), text: match[2] } : null;
}

function directCallTarget(text) {
  const match = text.match(/\bcall\s+(?:dword ptr\s+)?(?:\[)?0x([0-9A-Fa-f]{1,8})(?:\])?/i);
  return match ? hex(Number.parseInt(match[1], 16)) : null;
}

async function scanShard(filePath, root, addressHexes, sourceFileRows, allHits, functionEntries) {
  const bytes = await boundedRead(filePath, MAX_GENERATED_FILE_BYTES);
  const rel = path.relative(root, filePath).replaceAll('\\', '/');
  sourceFileRows.push({ path: rel, bytes: bytes.length, sha256: sha256(bytes) });
  const lines = bytes.toString('utf8').split(/\r?\n/);
  let functionLabel = null;
  let functionEntryLine = null;
  const instructions = [];
  const localHits = [];
  for (let index = 0; index < lines.length; index++) {
    const entryLabel = extractFunctionLabel(lines[index]);
    if (entryLabel) {
      functionLabel = entryLabel;
      functionEntryLine = index + 1;
      const address = hex(Number.parseInt(entryLabel.slice('sub_'.length), 16));
      const matches = functionEntries.get(address) ?? [];
      matches.push({ file: rel, fileLine: index + 1, label: entryLabel });
      functionEntries.set(address, matches);
    }
    const inst = instructionFromLine(lines[index]);
    if (!inst) continue;
    const calls = directCallTarget(inst.text);
    const record = { fileLine: index + 1, guestInstructionAddress: hex(inst.address), text: inst.text, directCallTarget: calls, functionLabel, functionEntryLine };
    instructions.push(record);
    const operands = [...inst.text.matchAll(/\b0x([0-9A-Fa-f]{1,8})\b/g)].map(match => hex(Number.parseInt(match[1], 16)));
    for (const resourceVA of addressHexes) {
      if (!operands.includes(resourceVA)) continue;
      localHits.push({ ...record, resourceVirtualAddress: resourceVA });
    }
  }
  for (const hit of localHits) {
    const sameFunction = instructions.filter(inst => inst.functionEntryLine === hit.functionEntryLine);
    const pos = sameFunction.findIndex(inst => inst.fileLine === hit.fileLine);
    const near = sameFunction.filter((inst, at) => inst.directCallTarget && Math.abs(at - pos) <= 8 && inst.fileLine !== hit.fileLine);
    hit.nearbyCalls = near.map(inst => ({ guestInstructionAddress: inst.guestInstructionAddress, target: inst.directCallTarget, fileLine: inst.fileLine }));
    allHits.push(hit);
  }
}

async function main() {
  const args = parseArgs(process.argv.slice(2));
  const [xbeBytes, inventoryBytes] = await Promise.all([
    boundedRead(args.xbe, MAX_XBE_BYTES),
    boundedRead(args.inventory, MAX_INVENTORY_BYTES),
  ]);
  const inventory = JSON.parse(inventoryBytes.toString('utf8'));
  if (inventory.kind !== 'private-ui-package-inventory' || !Array.isArray(inventory.resources)) throw new Error('Inventory schema/kind is not supported');
  const xbe = mapXbe(xbeBytes);
  const declaredXbeHash = inventory.sourceIdentity?.executable?.sha256;
  if (declaredXbeHash && declaredXbeHash.toLowerCase() !== sha256(xbeBytes)) {
    throw new Error(`XBE SHA-256 does not match inventory identity: expected ${declaredXbeHash}, got ${sha256(xbeBytes)}`);
  }
  const shardNames = (await fs.readdir(args['generated-dir'], { withFileTypes: true }))
    .filter(entry => entry.isFile() && /^x86_recomp(?:\.\d+)?\.cpp$/i.test(entry.name))
    .map(entry => entry.name)
    .sort((a, b) => a.localeCompare(b, undefined, { numeric: true }));
  if (!shardNames.length || shardNames.length > MAX_GENERATED_FILES) throw new Error(`Generated shard count out of bounds: ${shardNames.length}`);

  const names = [...new Set(inventory.resources.map(row => row.resourceName).filter(name => typeof name === 'string' && name.length > 0))].sort();
  const binaryHits = new Map();
  const nonAsciiNames = new Set();
  const codeHits = [];
  const sourceFiles = [];
  let totalGeneratedBytes = 0;
  for (const name of names) {
    if (!/^[\x00-\x7F]+$/.test(name)) {
      nonAsciiNames.add(name);
      binaryHits.set(name, []);
      continue;
    }
    const nameBytes = Buffer.from(name, 'ascii');
    const rawHits = findAll(xbeBytes, nameBytes);
    binaryHits.set(name, rawHits.map(fileOffset => ({ fileOffset, ...xbe.offsetToLocation(fileOffset) })));
  }
  const addressHexes = new Set();
  for (const [name, hits] of binaryHits) {
    for (const hit of hits) if (hit.virtualAddress) addressHexes.add(hit.virtualAddress);
  }
  const functionEntries = new Map();

  for (const shardName of shardNames) {
    const fullPath = path.join(args['generated-dir'], shardName);
    const stat = await fs.stat(fullPath);
    totalGeneratedBytes += stat.size;
    if (totalGeneratedBytes > MAX_GENERATED_TOTAL_BYTES) throw new Error(`Generated sources exceed ${MAX_GENERATED_TOTAL_BYTES} byte cap`);
    await scanShard(fullPath, args['generated-dir'], addressHexes, sourceFiles, codeHits, functionEntries);
  }

  let vtableResults = [];
  if (args.vtables) {
    const configBytes = await boundedRead(args.vtables, 1024 * 1024);
    const config = JSON.parse(configBytes.toString('utf8'));
    const tables = config.kind === 'bounded-original-ui-component-specification' ? config.vtableProfiles : config.kind === 'private-xbe-vtable-profiles' ? config.vtables : null;
    if (!Array.isArray(tables)) throw new Error('Vtable profile schema/kind is not supported');
    vtableResults = tables.map(table => {
      if (typeof table.label !== 'string' || !/^0x[0-9a-f]{1,8}$/i.test(table.virtualAddress) || !Number.isInteger(table.slotCount) || table.slotCount < 1 || table.slotCount > 64) {
        throw new Error('Invalid vtable profile row');
      }
      const tableVA = Number.parseInt(table.virtualAddress.slice(2), 16);
      const tableOffset = xbe.vaToOffset(tableVA);
      if (tableOffset === null || tableOffset + table.slotCount * 4 > xbeBytes.length) throw new Error(`Vtable is outside mapped XBE bytes: ${table.label}`);
      const slots = [];
      for (let slotIndex = 0; slotIndex < table.slotCount; slotIndex++) {
        const slotVA = tableVA + slotIndex * 4;
        const rawOffset = tableOffset + slotIndex * 4;
        const target = hex(xbeBytes.readUInt32LE(rawOffset));
        slots.push({
          slotIndex,
          slotVirtualAddress: hex(slotVA),
          rawFileOffset: rawOffset,
          targetGuestAddress: target,
          generatedFunctionEntries: functionEntries.get(target) ?? [],
        });
      }
      return { label: table.label, virtualAddress: hex(tableVA), rawFileOffset: tableOffset, slotCount: table.slotCount, slots };
    });
  }

  const resources = inventory.resources.map(row => {
    const xbeStringHits = binaryHits.get(row.resourceName) ?? [];
    const vaddrs = new Set(xbeStringHits.map(hit => hit.virtualAddress).filter(Boolean));
    const generatedReferenceHits = codeHits.filter(hit => vaddrs.has(hit.resourceVirtualAddress));
    return {
      stableId: row.stableId,
      packagePath: row.packagePath,
      package: row.package,
      resourceName: row.resourceName,
      resourceOffset: row.offset,
      resourceSize: row.size,
      resourceSha256: row.sha256,
      xbeStringHits,
      generatedReferenceHits,
      status: nonAsciiNames.has(row.resourceName) ? 'non-ascii-resource-name-skipped' : !xbeStringHits.length ? 'no-exact-xbe-ascii-name-hit' : !generatedReferenceHits.length ? 'xbe-name-no-generated-instruction-literal-hit' : 'static-address-reference-hit',
      limits: [
        'A static address reference does not prove runtime execution or resource use.',
        'This scan matches exact ASCII resource names and generated instruction-comment address literals only.',
        'Indirect, computed, hashed, or table-mediated references can be missed.',
      ],
    };
  });
  const statusCounts = Object.fromEntries(['no-exact-xbe-ascii-name-hit', 'xbe-name-no-generated-instruction-literal-hit', 'static-address-reference-hit', 'non-ascii-resource-name-skipped'].map(status => [status, resources.filter(row => row.status === status).length]));
  const result = {
    schemaVersion: 1,
    kind: 'private-ui-source-reference-index',
    sourceIdentity: {
      xbe: { path: args.xbe, bytes: xbeBytes.length, sha256: sha256(xbeBytes) },
      xbeProfile: { variant: xbe.variant, base: hex(xbe.base), headerBytes: xbe.headerBytes, sectionCount: xbe.sections.length },
      generatedDirectory: args['generated-dir'],
      generatedShards: sourceFiles,
      generatedShardBytes: totalGeneratedBytes,
      inventory: { path: args.inventory, bytes: inventoryBytes.length, sha256: sha256(inventoryBytes) },
      vtableProfiles: args.vtables ? { path: args.vtables, bytes: (await fs.stat(args.vtables)).size, sha256: sha256(await boundedRead(args.vtables, 1024 * 1024)) } : null,
    },
    method: {
      xbeMagic: 'XBEH',
      addressMapping: 'host/xbe.mjs readXbeProfile section address/fileOffset/fileBytes, with header mapping limited to validated headerBytes',
      stringMatching: 'exact ASCII resource name bytes anywhere in the XBE; every hit returned',
      codeMatching: 'instruction comment operands in generated x86_recomp shards matched against XBE virtual addresses',
      nonAsciiNames: 'resource names containing non-ASCII characters are reported as skipped and are never ASCII-encoded lossily',
      functionLabels: 'nearest preceding X86_FUNC sub_ label; analyst labels only',
      hitMeaning: 'static address reference only; not dynamic execution or resource-use proof',
      noHitMeaning: 'no hit under this scan method; not proof of absence',
    },
    summary: {
      inventoryResourceRows: resources.length,
      distinctResourceNames: names.length,
      codeReferenceHitCount: codeHits.length,
      ...statusCounts,
      vtableCount: vtableResults.length,
      vtableSlotCount: vtableResults.reduce((sum, table) => sum + table.slotCount, 0),
    },
    vtables: vtableResults,
    resources,
  };
  await preparePrivateOutput(args.out, {
    directory: false,
    inputs: [args.xbe, args.inventory, args['generated-dir'], inventory.sourceIdentity?.sourceTree, args.vtables],
  });
  await assertNoLinks(args.out);
  await fs.writeFile(args.out, `${JSON.stringify(result, null, 2)}\n`);
  await assertNoLinks(args.out);
  console.log(JSON.stringify({ output: args.out, ...result.summary, xbeSha256: result.sourceIdentity.xbe.sha256 }, null, 2));
}

main().catch(error => { console.error(error.message); process.exitCode = 1; });
