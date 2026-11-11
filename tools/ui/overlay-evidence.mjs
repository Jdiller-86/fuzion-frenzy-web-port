#!/usr/bin/env node
// Private, bounded OBE overlay evidence extraction. Uses the existing stadium
// OBE reader and joins material hashes only to the supplied private inventory.
import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';
import { execFileSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';
import { readXbpIndex } from '../../host/xbp.mjs';
import { readStadiumRoot } from '../../host/obe.mjs';
import { assertNoLinks, preparePrivateOutput } from './private-output.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.resolve(HERE, '../..');
const DEFAULT_OUT = path.join(ROOT, 'artifacts/ui-inventory/overlay-evidence.json');
const MAX_FILE_BYTES = 128 * 1024 * 1024;
const MAX_RESOURCE_COUNT = 100_000;
const sha256 = bytes => crypto.createHash('sha256').update(bytes).digest('hex');

function parseArgs(argv) {
  let source, inventory, sourceReferences, output = DEFAULT_OUT;
  for (let i = 0; i < argv.length; i++) {
    if (argv[i] === '--source') {
      source = argv[++i];
      if (!source) throw new Error('--source needs a loose game tree path.');
      source = path.resolve(source);
    } else if (argv[i] === '--inventory') {
      inventory = argv[++i];
      if (!inventory) throw new Error('--inventory needs an inventory.json path.');
      inventory = path.resolve(inventory);
    } else if (argv[i] === '--source-references') {
      sourceReferences = argv[++i];
      if (!sourceReferences) throw new Error('--source-references needs a private source-references.json path.');
      sourceReferences = path.resolve(sourceReferences);
    } else if (argv[i] === '--out') {
      output = argv[++i];
      if (!output) throw new Error('--out needs an output path.');
      output = path.resolve(output);
    } else if (argv[i] === '--help') {
      console.log('node tools/ui/overlay-evidence.mjs --source <loose game tree> --inventory <private inventory.json> --source-references <private source-references.json> [--out <private output.json>]');
      process.exit(0);
    } else throw new Error(`Unknown argument ${argv[i]}`);
  }
  if (!source || !inventory || !sourceReferences) throw new Error('Pass --source, --inventory, and --source-references.');
  return { source, inventory, sourceReferences, output };
}

async function readBounded(filePath) {
  const stat = await fs.stat(filePath);
  if (!stat.isFile()) throw new Error(`Expected a regular file: ${path.basename(filePath)}`);
  if (stat.size > MAX_FILE_BYTES) throw new Error(`File exceeds ${MAX_FILE_BYTES} byte limit: ${path.basename(filePath)}`);
  const bytes = await fs.readFile(filePath);
  if (bytes.byteLength !== stat.size) throw new Error(`File changed while being read: ${path.basename(filePath)}`);
  return bytes;
}

function arrayBufferSlice(bytes, offset = 0, length = bytes.byteLength - offset) {
  return bytes.buffer.slice(bytes.byteOffset + offset, bytes.byteOffset + offset + length);
}

function indexArray(value) {
  return Array.from(value);
}

function bounds3(positions, vertexIds) {
  const bounds = { min: [Infinity, Infinity, Infinity], max: [-Infinity, -Infinity, -Infinity] };
  for (const id of vertexIds) for (let axis = 0; axis < 3; axis++) {
    const coordinate = positions[id * 3 + axis];
    bounds.min[axis] = Math.min(bounds.min[axis], coordinate);
    bounds.max[axis] = Math.max(bounds.max[axis], coordinate);
  }
  return vertexIds.length ? bounds : null;
}

function uvBounds(uvs, vertexIds) {
  if (!vertexIds.length) return null;
  const result = { minU: Infinity, minV: Infinity, maxU: -Infinity, maxV: -Infinity };
  for (const id of vertexIds) {
    result.minU = Math.min(result.minU, uvs[id * 2]);
    result.maxU = Math.max(result.maxU, uvs[id * 2]);
    result.minV = Math.min(result.minV, uvs[id * 2 + 1]);
    result.maxV = Math.max(result.maxV, uvs[id * 2 + 1]);
  }
  return result;
}

function equalWithin(a, b, tolerance = 1e-7) {
  return Math.abs(a - b) <= tolerance;
}

function strictUvQuad(indices, uvs) {
  if (indices.length !== 6) return { isQuad: false, reason: 'batch-does-not-contain-exactly-two-triangles' };
  const unique = [...new Set(indices)];
  if (unique.length !== 4 || [0, 3].some(start => new Set(indices.slice(start, start + 3)).size !== 3)) {
    return { isQuad: false, reason: 'triangle-index-topology-is-not-four-unique-corners' };
  }
  const us = [...new Set(unique.map(id => uvs[id * 2]))].sort((a, b) => a - b);
  const vs = [...new Set(unique.map(id => uvs[id * 2 + 1]))].sort((a, b) => a - b);
  if (us.length !== 2 || vs.length !== 2 || us[0] === us[1] || vs[0] === vs[1]) {
    return { isQuad: false, reason: 'uvs-do-not-form-two-by-two-axis-aligned-corners' };
  }
  const corners = new Set(unique.map(id => `${uvs[id * 2]},${uvs[id * 2 + 1]}`));
  if (corners.size !== 4) return { isQuad: false, reason: 'uv-corners-are-not-unique' };
  for (const u of us) for (const v of vs) {
    if (![...corners].some(pair => {
      const [pu, pv] = pair.split(',').map(Number);
      return equalWithin(pu, u) && equalWithin(pv, v);
    })) return { isQuad: false, reason: 'uv-rectangle-corner-is-missing' };
  }
  const first = new Set(indices.slice(0, 3));
  const second = new Set(indices.slice(3, 6));
  const shared = [...first].filter(id => second.has(id));
  const unshared = [...first].filter(id => !second.has(id)).concat([...second].filter(id => !first.has(id)));
  if (shared.length !== 2 || unshared.length !== 2) return { isQuad: false, reason: 'triangles-do-not-share-one-diagonal' };
  const a = unshared[0] * 2, b = unshared[1] * 2;
  if (equalWithin(uvs[a], uvs[b]) || equalWithin(uvs[a + 1], uvs[b + 1])) {
    return { isQuad: false, reason: 'unshared-corners-are-not-opposite' };
  }
  return {
    isQuad: true,
    uvBounds: { minU: us[0], minV: vs[0], maxU: us[1], maxV: vs[1] },
    cornerOrder: unique.map(id => ({ vertexIndex: id, u: uvs[id * 2], v: uvs[id * 2 + 1] })),
    triangleVertexOrder: indices,
  };
}

function pixelBounds(uv, width, height) {
  return {
    left: uv.minU * width,
    top: uv.minV * height,
    right: uv.maxU * width,
    bottom: uv.maxV * height,
    coordinateConvention: 'continuous pixel-edge coordinates; no floor/ceil or clamping applied',
  };
}

function inspectLeafNodes(bytes) {
  if (bytes.byteLength < 0x100) return { rootOffset: null, nodes: [], leaves: [], failures: [{ reason: 'Invalid OBE size' }] };
  const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
  const rootOffset = view.getUint32(0x40, true);
  const pending = [rootOffset];
  const visited = new Set();
  const nodes = [];
  const failures = [];
  while (pending.length) {
    const offset = pending.pop();
    if (!offset) continue;
    if (visited.has(offset)) continue;
    if (!Number.isSafeInteger(offset) || offset < 0 || offset + 0x100 > bytes.byteLength) {
      failures.push({ offset, reason: 'Node pointer outside resource bounds' });
      continue;
    }
    if (visited.size >= 4096) {
      failures.push({ offset, reason: 'Node traversal limit 4096 reached' });
      break;
    }
    visited.add(offset);
    const nodeType = view.getUint8(offset + 0xf0);
    const sibling = view.getUint32(offset + 0xe0, true);
    const child = view.getUint32(offset + 0xec, true);
    nodes.push({ offset, nodeType, sibling, child });
    if (sibling && !visited.has(sibling)) pending.push(sibling);
    if (child && !visited.has(child)) pending.push(child);
  }
  return {
    rootOffset,
    nodes,
    leaves: nodes.filter(node => node.nodeType === 2 && node.child === 0),
    failures,
  };
}

function parseLeafView(bytes, node, inventoryResources) {
  const source = Buffer.from(bytes);
  source.writeUInt32LE(node.offset, 0x40);
  source.writeUInt32LE(node.offset, node.offset + 0xe0);
  // The traverser admits only type-2 nodes with a null child pointer.
  if (node.nodeType !== 2 || node.child !== 0) {
    return { nodeOffset: node.offset, status: 'not-attempted', reason: 'Node is not a type-2 geometry leaf.' };
  }
  const profile = source.readUInt8(0x79);
  const profiles = profile === 1 ? [true, false] : [false, true];
  const attempts = [];
  for (const hierarchical of profiles) {
    try {
      const geometry = readStadiumRoot(arrayBufferSlice(source), { hierarchical });
      const evidence = geometryEvidence(geometry, inventoryResources);
      return {
        nodeOffset: node.offset,
        nodeType: node.nodeType,
        siblingReplacedWithSelf: true,
        childOffset: node.child,
        status: 'supported-leaf-view',
        selectedProfile: hierarchical ? 'initial-key-hierarchy' : 'flat-stadium',
        attempts: [...attempts, { profile: hierarchical ? 'initial-key-hierarchy' : 'flat-stadium', status: 'supported' }],
        boundsScope: 'isolated leaf parser coordinates; parent and sibling transforms omitted; not global overlay coordinates',
        geometry: evidence,
      };
    } catch (error) {
      attempts.push({ profile: hierarchical ? 'initial-key-hierarchy' : 'flat-stadium', status: 'unsupported', reason: String(error?.message ?? error) });
    }
  }
  return { nodeOffset: node.offset, nodeType: node.nodeType, siblingReplacedWithSelf: true, childOffset: node.child, status: 'unsupported-leaf-view', attempts };
}

function materialMatches(inventoryResources, hash) {
  return inventoryResources.filter(resource => Number(resource.xbpHash) >>> 0 === (hash >>> 0));
}

function resolveLoaderCodeEvidence(sourceReferences, inventoryResources) {
  const candidates = [
    { screen: 'main-menu', resourceName: 'over_main.obe' },
    { screen: 'options', resourceName: 'over_option.obe' },
  ];
  return candidates.map(candidate => {
    const references = sourceReferences.resources.filter(resource =>
      resource.package?.toLowerCase() === 'generic.xbp' && resource.resourceName?.toLowerCase() === candidate.resourceName,
    );
    const staticReferences = references.flatMap(reference => {
      const matches = inventoryResources.filter(resource =>
        resource.stableId === reference.stableId &&
        resource.packagePath === reference.packagePath &&
        resource.resourceName === reference.resourceName &&
        resource.offset === reference.resourceOffset &&
        resource.sha256 === reference.resourceSha256,
      );
      if (matches.length !== 1 || reference.status !== 'static-address-reference-hit') return [];
      return matches.map(resource => ({
        package: resource.package,
        packagePath: resource.packagePath,
        resourceName: resource.resourceName,
        stableId: resource.stableId,
        offset: resource.offset,
        xbpHash: resource.xbpHash,
        sha256: resource.sha256,
        xbeStringHits: reference.xbeStringHits,
        generatedReferenceHits: reference.generatedReferenceHits,
        staticReferenceLimits: reference.limits,
      }));
    });
    const functionLabels = [...new Set(staticReferences.flatMap(reference => reference.generatedReferenceHits.map(hit => hit.functionLabel)))];
    return {
      screen: candidate.screen,
      status: staticReferences.length === 1 ? 'confirmed-static-address-reference' : 'unresolved-static-address-reference',
      functionOwnership: functionLabels.length === 1 ? functionLabels[0] : functionLabels.length > 1 ? 'ambiguous-overlapping-generated-entries' : 'unknown',
      staticReferences,
      runtimeObservation: 'Main menu/options runtime observed per coordinator-provided evidence; not rerun by this extraction.',
    };
  });
}

function batchEvidence(geometry, batch, inventoryResources) {
  const triangleOrder = indexArray(geometry.indices.slice(batch.start, batch.start + batch.count));
  const firstUseOrder = [...new Set(triangleOrder)];
  const uv = uvBounds(geometry.uvs, firstUseOrder);
  const matches = materialMatches(inventoryResources, batch.hash);
  const decodedMatches = matches.filter(resource => resource.decodeStatus === 'decoded' && Number.isInteger(resource.width) && Number.isInteger(resource.height));
  const quad = strictUvQuad(triangleOrder, geometry.uvs);
  const matchStatus = matches.length === 0
    ? 'no-xbp-hash-match'
    : decodedMatches.length === 0
      ? (matches.length === 1 ? 'one-undecoded-hash-match' : 'ambiguous-undecoded-hash-matches')
      : matches.length === 1 && decodedMatches.length === 1
        ? 'unique-decoded-hash-match'
        : 'ambiguous-hash-matches';
  const record = {
    textureHash: batch.hash >>> 0,
    pass: batch.pass,
    flags: batch.flags,
    indexRange: { start: batch.start, count: batch.count },
    triangleCount: batch.count / 3,
    vertexIndexOrder: firstUseOrder,
    triangleVertexOrder: triangleOrder,
    vertexIndexBounds: firstUseOrder.length ? { min: Math.min(...firstUseOrder), max: Math.max(...firstUseOrder), count: firstUseOrder.length } : null,
    objectVertexBounds: bounds3(geometry.positions, firstUseOrder),
    batchUvBounds: uv,
    strictAxisAlignedUvQuad: quad,
    textureMatches: matches.map(resource => ({
      stableId: resource.stableId,
      package: resource.package,
      resourceName: resource.resourceName,
      xbpHash: resource.xbpHash,
      sha256: resource.sha256,
      decodeStatus: resource.decodeStatus,
      width: resource.width,
      height: resource.height,
      format: resource.format,
    })),
    textureMatchStatus: matchStatus,
    pixelBoundsByDecodedCandidate: quad.isQuad ? decodedMatches.map(resource => ({
      stableId: resource.stableId,
      package: resource.package,
      resourceName: resource.resourceName,
      dimensions: { width: resource.width, height: resource.height },
      pixelBounds: uv ? pixelBounds(uv, resource.width, resource.height) : null,
    })) : [],
    semanticAtlasRegion: null,
  };
  if (quad.isQuad && matchStatus === 'unique-decoded-hash-match' && uv) {
    record.textureRectangle = { uvBounds: uv, pixelBounds: pixelBounds(uv, decodedMatches[0].width, decodedMatches[0].height), semanticLabel: null };
  } else if (quad.isQuad) {
    record.textureRectangle = { uvBounds: uv, pixelBounds: null, semanticLabel: null };
  } else {
    record.textureRectangle = null;
  }
  return record;
}

function geometryEvidence(geometry, matches) {
  const allVertices = Array.from({ length: geometry.positions.length / 3 }, (_, index) => index);
  const allBatches = geometry.batches.map(batch => batchEvidence(geometry, batch, matches));
  return {
    nodes: geometry.nodes,
    vertexCount: allVertices.length,
    triangleCount: geometry.indices.length / 3,
    objectVertexBounds: bounds3(geometry.positions, allVertices),
    objectVertexOrder: 'positions and source index order are those returned by host/obe.mjs; per-batch vertexIndexOrder is first index-use order; triangleVertexOrder preserves each parsed batch index order',
    passCounts: Object.fromEntries([0, 1, 2].map(pass => [pass, allBatches.filter(batch => batch.pass === pass).length])),
    batches: allBatches,
  };
}

async function extractPackage(packageRow, sourceRoot, inventoryResources) {
  const packagePath = path.join(sourceRoot, ...packageRow.sourcePath.split('/'));
  const packageBytes = await readBounded(packagePath);
  const actualDigest = sha256(packageBytes);
  if (actualDigest !== packageRow.sha256) throw new Error(`Package identity changed: ${packageRow.sourcePath}`);
  const entries = await readXbpIndex(new Blob([packageBytes]));
  const obeEntries = entries.filter(entry => entry.name.toLowerCase().endsWith('.obe'));
  const results = [];
  for (const entry of obeEntries) {
    if (results.length >= MAX_RESOURCE_COUNT) throw new Error(`OBE count exceeds ${MAX_RESOURCE_COUNT}`);
    const bytes = packageBytes.subarray(entry.offset, entry.offset + entry.size);
    const matchingInventory = inventoryResources.filter(resource =>
      resource.packagePath === packageRow.sourcePath && resource.offset === entry.offset && resource.xbpHash === entry.hash && resource.resourceName === entry.name,
    );
    const row = {
      package: packageRow.name,
      packagePath: packageRow.sourcePath,
      resourceName: entry.name,
      offset: entry.offset,
      size: entry.size,
      xbpHash: entry.hash,
      sha256: sha256(bytes),
      inventoryStableIds: matchingInventory.map(resource => resource.stableId),
      candidateReason: /over_/i.test(entry.name) ? 'overlay-named-object' : null,
      wholeObject: { attempts: [], status: 'unsupported', geometry: null },
      leafTraversal: null,
      leafViews: [],
    };
    const profile = bytes.byteLength > 0x79 ? bytes[0x79] : 0;
    const profiles = profile === 1 ? [true, false] : [false, true];
    for (const hierarchical of profiles) {
      try {
        const parsed = readStadiumRoot(arrayBufferSlice(bytes), { hierarchical });
        row.wholeObject.attempts.push({ profile: hierarchical ? 'initial-key-hierarchy' : 'flat-stadium', status: 'supported' });
        row.wholeObject.geometry = geometryEvidence(parsed, inventoryResources);
        row.wholeObject.status = 'supported';
        break;
      } catch (error) {
        row.wholeObject.attempts.push({ profile: hierarchical ? 'initial-key-hierarchy' : 'flat-stadium', status: 'unsupported', reason: String(error?.message ?? error) });
      }
    }
    const inspected = inspectLeafNodes(bytes);
    row.leafTraversal = {
      rootOffset: inspected.rootOffset,
      visitedNodeCount: inspected.nodes.length,
      geometryLeafCount: inspected.leaves.length,
      failures: inspected.failures,
      basis: 'bounded pointers at retained reader node fields +0xE0 sibling, +0xEC child, +0xF0 type; isolated view uses documented +0x40 root field',
    };
    row.leafViews = inspected.leaves.map(node => parseLeafView(bytes, node, inventoryResources));
    results.push(row);
  }
  return results;
}

async function main() {
  const { source, inventory, sourceReferences: sourceReferencesPath, output } = parseArgs(process.argv.slice(2));
  const parsedInventory = JSON.parse(await fs.readFile(inventory, 'utf8'));
  if (parsedInventory.kind !== 'private-ui-package-inventory' || !Array.isArray(parsedInventory.packages) || !Array.isArray(parsedInventory.resources)) {
    throw new Error('Inventory JSON is not the expected private UI inventory schema.');
  }
  const executable = await readBounded(path.join(source, parsedInventory.sourceIdentity.executable.name));
  if (sha256(executable) !== parsedInventory.sourceIdentity.executable.sha256) throw new Error('Executable identity does not match the supplied inventory.');
  const sourceReferencesBytes = await fs.readFile(sourceReferencesPath);
  const parsedSourceReferences = JSON.parse(sourceReferencesBytes.toString('utf8'));
  const inventoryBytes = await fs.readFile(inventory);
  if (parsedSourceReferences.kind !== 'private-ui-source-reference-index' || parsedSourceReferences.sourceIdentity?.xbe?.sha256 !== sha256(executable)) {
    throw new Error('Source-reference index kind or XBE SHA-256 does not match the selected executable.');
  }
  if (parsedSourceReferences.sourceIdentity?.inventory?.sha256 !== sha256(inventoryBytes)) {
    throw new Error('Source-reference index was generated from a different inventory revision.');
  }
  const resources = parsedInventory.resources;
  const loaderCodeEvidence = resolveLoaderCodeEvidence(parsedSourceReferences, resources);
  const obeRows = [];
  for (const pkg of parsedInventory.packages) obeRows.push(...await extractPackage(pkg, source, resources));
  for (const row of obeRows) {
    row.loaderCodeEvidence = loaderCodeEvidence.filter(evidence =>
      evidence.staticReferences.some(reference => reference.packagePath === row.packagePath && reference.resourceName === row.resourceName),
    );
  }

  const result = {
    schemaVersion: 1,
    kind: 'private-overlay-evidence',
    generatedAt: new Date().toISOString(),
    sourceIdentity: {
      executableSha256: parsedInventory.sourceIdentity.executable.sha256,
      inventoryRepoHead: parsedInventory.evidence.repoHead,
      reader: 'host/obe.mjs readStadiumRoot',
      xbpReader: 'host/xbp.mjs readXbpIndex',
      sourceReferenceIndex: {
        path: sourceReferencesPath,
        sha256: sha256(sourceReferencesBytes),
        generatedDirectory: parsedSourceReferences.sourceIdentity.generatedDirectory,
        shardCount: parsedSourceReferences.sourceIdentity.generatedShards.length,
        generatedShardBytes: parsedSourceReferences.sourceIdentity.generatedShardBytes,
        generatedShards: parsedSourceReferences.sourceIdentity.generatedShards,
      },
    },
    screens: loaderCodeEvidence.map(evidence => ({
      id: evidence.screen,
      status: evidence.status,
      evidenceKind: 'latest source-reference index static address hit + XBE identity match + selected-package row identity join',
      runtimeObservation: evidence.runtimeObservation,
      functionOwnership: evidence.functionOwnership,
      staticReferences: evidence.staticReferences,
    })),
    scope: {
      packageCount: parsedInventory.packages.length,
      obeResourcesAttempted: obeRows.length,
      completeGameInventory: false,
      profilePolicy: 'Try the profile-indicated retained stadium view, then its alternate; also preserve separately labeled bounded leaf views. Do not extend parsers or infer unsupported semantics.',
      textureJoinPolicy: 'Join OBE batch hash to every inventory resource xbpHash match and preserve ambiguity; no single candidate is assumed without a unique decoded match.',
      limits: [
        'Only OBE resources inside the selected inventory package set were attempted.',
        'The retained reader supports bounded stadium-oriented OBE profiles and may reject UI overlay object layouts.',
        'An isolated leaf view is evidence for that leaf only; parent/sibling transforms and whole-object/global overlay coordinates are not included.',
        'Pixel bounds are UV bounds multiplied by each decoded candidate texture dimensions; no atlas semantics, clamping, glyph role, or animation timing is inferred.',
        'Batch pass numbers and flags are copied from the retained reader; their render meaning is not expanded here.',
      ],
    },
    totals: {
      wholeObjectSupported: obeRows.filter(row => row.wholeObject.status === 'supported').length,
      wholeObjectUnsupported: obeRows.filter(row => row.wholeObject.status === 'unsupported').length,
      leafViewsSupported: obeRows.reduce((sum, row) => sum + row.leafViews.filter(leaf => leaf.status === 'supported-leaf-view').length, 0),
      leafViewsUnsupported: obeRows.reduce((sum, row) => sum + row.leafViews.filter(leaf => leaf.status === 'unsupported-leaf-view').length, 0),
      overlayNamed: obeRows.filter(row => row.candidateReason === 'overlay-named-object').length,
      wholeObjectBatchViews: obeRows.reduce((sum, row) => sum + (row.wholeObject.geometry?.batches.length ?? 0), 0),
      isolatedLeafBatchViews: obeRows.reduce((sum, row) => sum + row.leafViews.reduce((leaves, leaf) => leaves + (leaf.geometry?.batches.length ?? 0), 0), 0),
      strictUvQuads: obeRows.reduce((sum, row) => sum + (row.wholeObject.geometry?.batches.filter(batch => batch.strictAxisAlignedUvQuad.isQuad).length ?? 0) + row.leafViews.reduce((leaves, leaf) => leaves + (leaf.geometry?.batches.filter(batch => batch.strictAxisAlignedUvQuad.isQuad).length ?? 0), 0), 0),
      batchHashMatches: obeRows.reduce((sum, row) => sum + (row.wholeObject.geometry?.batches.filter(batch => batch.textureMatches.length).length ?? 0) + row.leafViews.reduce((leaves, leaf) => leaves + (leaf.geometry?.batches.filter(batch => batch.textureMatches.length).length ?? 0), 0), 0),
      ambiguousHashBatches: obeRows.reduce((sum, row) => sum + (row.wholeObject.geometry?.batches.filter(batch => batch.textureMatchStatus === 'ambiguous-hash-matches' || batch.textureMatchStatus === 'ambiguous-undecoded-hash-matches').length ?? 0) + row.leafViews.reduce((leaves, leaf) => leaves + (leaf.geometry?.batches.filter(batch => batch.textureMatchStatus === 'ambiguous-hash-matches' || batch.textureMatchStatus === 'ambiguous-undecoded-hash-matches').length ?? 0), 0), 0),
    },
    resources: obeRows,
  };
  await preparePrivateOutput(output, { inputs: [source, inventory, sourceReferencesPath] });
  await assertNoLinks(output);
  await fs.writeFile(output, `${JSON.stringify(result, null, 2)}\n`);
  console.log(JSON.stringify({ output, ...result.totals, attempted: obeRows.length }, null, 2));
}

main().catch(error => { console.error(error.message); process.exitCode = 1; });
