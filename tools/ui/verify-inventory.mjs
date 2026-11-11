// Verify resource coverage and match the loose extraction corpus to the disc
// used by the native reference. This reads only the selected package extents.
import fs from 'node:fs/promises';
import { openAsBlob } from 'node:fs';
import crypto from 'node:crypto';
import path from 'node:path';
import { openXiso } from '../../host/xdvdfs.mjs';
import { preparePrivateOutput, assertNoLinks } from './private-output.mjs';
const args = {};
for (let i = 2; i < process.argv.length; i += 2) {
  const key = process.argv[i];
  if (!['--inventory', '--disc', '--joins', '--overlays', '--references', '--out'].includes(key) || !process.argv[i + 1]) throw new Error('Pass --inventory --disc --joins --overlays --references --out paths');
  args[key.slice(2)] = path.resolve(process.argv[i + 1]);
}
for (const key of ['inventory', 'disc', 'joins', 'overlays', 'references', 'out']) if (!args[key]) throw new Error(`Missing --${key}`);
const hash = bytes => crypto.createHash('sha256').update(bytes).digest('hex');
const load = async key => JSON.parse(await fs.readFile(args[key], 'utf8'));
const [inventory, joins, overlays, references] = await Promise.all(['inventory', 'joins', 'overlays', 'references'].map(load));
const expectedXbe = inventory.sourceIdentity.executable.sha256;
if ([joins.executableSha256, overlays.sourceIdentity.executableSha256, references.sourceIdentity.xbe.sha256].some(value => value !== expectedXbe)) throw new Error('XBE identities differ');
if (overlays.sourceIdentity.sourceReferenceIndex.sha256 !== hash(await fs.readFile(args.references))) throw new Error('Overlay source-reference index is stale');
if (references.sourceIdentity.inventory.sha256 !== hash(await fs.readFile(args.inventory))) throw new Error('Source-reference inventory is stale');
const counts = { decoded: 0, unsupported: 0, duplicate: 0 };
const ids = new Set(), uniqueImages = new Set();
for (const row of inventory.resources) {
  if (!Object.hasOwn(counts, row.decodeStatus) || ids.has(row.stableId)) throw new Error('Unknown status or duplicate stable ID');
  counts[row.decodeStatus]++; ids.add(row.stableId);
  for (const frame of row.frames) if (frame.thumbnail) {
    const image = path.resolve(path.dirname(args.inventory), frame.thumbnail);
    if (!image.startsWith(path.dirname(args.inventory) + path.sep)) throw new Error('Image escapes inventory');
    await assertNoLinks(image);
    const png = await fs.readFile(image);
    if (png.subarray(0, 8).toString('hex') !== '89504e470d0a1a0a' || png.readUInt32BE(16) !== row.width || png.readUInt32BE(20) !== row.height) throw new Error('PNG dimensions differ');
    uniqueImages.add(frame.thumbnail);
  }
}
for (const row of inventory.resources) if (row.duplicateOf && !ids.has(row.duplicateOf)) throw new Error('Missing duplicate target');
if (Object.values(counts).reduce((a, b) => a + b, 0) !== inventory.scope.resourceCount) throw new Error('Coverage does not balance');
for (const row of joins.resources) if (!ids.has(row.stableId)) throw new Error('Join references unknown asset');
const disc = await openAsBlob(args.disc), volume = await openXiso(disc);
async function discHash(entry) {
  const digest = crypto.createHash('sha256');
  for await (const bytes of disc.slice(entry.offset, entry.offset + entry.size).stream()) digest.update(bytes);
  return digest.digest('hex');
}
const compared = [];
for (const item of [{ sourcePath: 'default.xbe', size: inventory.sourceIdentity.executable.size, sha256: expectedXbe }, ...inventory.sourceIdentity.packages]) {
  const entry = await volume.find(item.sourcePath);
  if (entry.directory || entry.size !== item.size || await discHash(entry) !== item.sha256) throw new Error(`Runtime disc differs from inventoried input: ${item.sourcePath}`);
  const loose = path.resolve(inventory.sourceIdentity.sourceTree, item.sourcePath);
  if (hash(await fs.readFile(loose)) !== item.sha256) throw new Error('Loose source changed since extraction');
  compared.push({ path: item.sourcePath, offset: entry.offset, size: entry.size, sha256: item.sha256, matchesLooseInput: true });
}
await preparePrivateOutput(args.out, { inputs: [args.inventory, args.disc, args.joins, args.overlays, args.references, inventory.sourceIdentity.sourceTree] });
const result = { schemaVersion: 1, kind: 'private-ui-inventory-verification', checkedAt: new Date().toISOString(), passed: true,
  executableSha256: expectedXbe, resourceStatusCounts: counts, uniqueImages: uniqueImages.size, classification: joins.summary,
  runtimeDisc: { path: args.disc, selectedEntriesMatched: compared.length, entries: compared, wholeDiscHash: 'see run-identities.json' },
  limits: ['Checks package-byte identity, accounting, PNG dimensions and index freshness; not complete original UI fidelity.', 'Original inputs were read only. Exact disc offsets are private output.'] };
await fs.writeFile(args.out, JSON.stringify(result, null, 2) + '\n');
console.log(JSON.stringify({ output: args.out, passed: true, ...counts, uniqueImages: uniqueImages.size, runtimeDiscMatches: compared.length }, null, 2));
