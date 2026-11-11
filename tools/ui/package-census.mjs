#!/usr/bin/env node
// Private metadata-only census of local XBP package indexes. Package bytes are
// streamed only for SHA-256 identity; host/xbp.mjs reads the index and strings.
import { createReadStream, openAsBlob } from 'node:fs';
import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';
import { fileURLToPath } from 'node:url';
import { readXbpIndex } from '../../host/xbp.mjs';
import { assertNoLinks, preparePrivateOutput } from './private-output.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.resolve(HERE, '../..');
const DEFAULT_OUT = path.join(ROOT, 'artifacts/ui-inventory/package-census.json');
const MAX_PACKAGE_BYTES = 128 * 1024 * 1024;
const MAX_RESOURCES = 100_000;
const PACKAGE_NAME_LEAD = /(?:bg(?:effects|fx)|boot(?:seq|sequence)|map|meta|demo|generic|local|character.?sel|score.?screen|game.?winner|mini.?game.?intro|screen|menu|font)/i;
const RESOURCE_NAME_LEAD = /(?:^|[_.\/ -])(?:menu|pause|result|score|screen|title|cursor|button|select|frenzy|zone|map|icon|atlas|font|background|backdrop|overlay|arrow|option|loading|text|meter|panel|prompt)(?=$|[_.\/ -])|logo/i;
const FONT_RESOURCE = /\.(?:ttf|otf|xpr)$/i;
const sha256 = bytes => crypto.createHash('sha256').update(bytes).digest('hex');

function parseArgs(argv) {
  let source, inventory, output = DEFAULT_OUT;
  for (let i = 0; i < argv.length; i++) {
    if (argv[i] === '--source') {
      if (!argv[i + 1]) throw new Error('--source needs a loose game tree path.');
      source = path.resolve(argv[++i]);
    } else if (argv[i] === '--inventory') {
      if (!argv[i + 1]) throw new Error('--inventory needs an inventory.json path.');
      inventory = path.resolve(argv[++i]);
    } else if (argv[i] === '--out') {
      if (!argv[i + 1]) throw new Error('--out needs a private output path.');
      output = path.resolve(argv[++i]);
    } else if (argv[i] === '--help') {
      console.log('node tools/ui/package-census.mjs --source <loose game tree> --inventory <private inventory.json> [--out <private output.json>]');
      process.exit(0);
    } else throw new Error(`Unknown argument ${argv[i]}`);
  }
  if (!source || !inventory) throw new Error('Pass both --source and --inventory.');
  return { source, inventory, output };
}

async function walkXbp(dir) {
  const paths = [];
  for (const entry of await fs.readdir(dir, { withFileTypes: true })) {
    const full = path.join(dir, entry.name);
    if (entry.isDirectory()) paths.push(...await walkXbp(full));
    else if (entry.isFile() && entry.name.toLowerCase().endsWith('.xbp')) paths.push(full);
  }
  return paths.sort((a, b) => a.toLowerCase().localeCompare(b.toLowerCase()));
}

async function streamedSha256(filePath) {
  const digest = crypto.createHash('sha256');
  for await (const chunk of createReadStream(filePath)) digest.update(chunk);
  return digest.digest('hex');
}

function inventoryKey(resource) {
  return `${resource.packagePath.toLowerCase()}\0${resource.resourceName.toLowerCase()}\0${resource.offset}\0${resource.size}\0${Number(resource.xbpHash) >>> 0}`;
}

async function censusPackage(packagePath, sourceRoot, selectedResources, resourceRows) {
  const stat = await fs.stat(packagePath);
  const relativePath = path.relative(sourceRoot, packagePath).replaceAll('\\', '/');
  const packageName = path.posix.basename(relativePath);
  const row = {
    name: packageName,
    path: relativePath,
    size: stat.size,
    sha256: null,
    tableRowsIncludingMetadata: null,
    metadataRowsOmittedByReader: null,
    indexedEntries: 0,
    status: 'unsupported',
    error: null,
    selectedInventoryPackage: false,
    nameLead: PACKAGE_NAME_LEAD.test(packageName),
    resourceNameLeadCount: 0,
  };
  if (stat.size > MAX_PACKAGE_BYTES) {
    row.error = `Package exceeds ${MAX_PACKAGE_BYTES} byte index-read limit`;
    return row;
  }
  try {
    // Blob slices used by readXbpIndex issue bounded random reads for the XBP
    // header, entry table, and string table. No resource payload is requested.
    const blob = await openAsBlob(packagePath);
    const header = new DataView(await blob.slice(0, 64).arrayBuffer());
    row.tableRowsIncludingMetadata = header.getUint32(12, true);
    const entries = await readXbpIndex(blob);
    row.indexedEntries = entries.length;
    row.metadataRowsOmittedByReader = row.tableRowsIncludingMetadata - entries.length;
    row.status = 'indexed-not-decoded';
    row.sha256 = await streamedSha256(packagePath);
    if (entries.length > MAX_RESOURCES - resourceRows.length) throw new Error(`Resource limit ${MAX_RESOURCES} would be exceeded`);
    for (const entry of entries) {
      if (resourceRows.length >= MAX_RESOURCES) throw new Error(`Resource limit ${MAX_RESOURCES} reached`);
      const key = `${relativePath.toLowerCase()}\0${entry.name.toLowerCase()}\0${entry.offset}\0${entry.size}\0${Number(entry.hash) >>> 0}`;
      const matches = selectedResources.get(key) ?? [];
      const resourceLead = RESOURCE_NAME_LEAD.test(entry.name) || FONT_RESOURCE.test(entry.name);
      const stableId = sha256(Buffer.from(`${row.sha256}\0${entry.name}\0${entry.offset}\0${entry.size}\0${entry.hash}`)).slice(0, 24);
      const resource = {
        stableId,
        package: packageName,
        packagePath: relativePath,
        resourceName: entry.name,
        offset: entry.offset,
        size: entry.size,
        xbpHash: entry.hash,
        selectedInventoryStableIds: matches.map(match => match.stableId),
        selectedInventoryMatch: matches.length > 0,
        status: 'indexed-not-decoded',
        classification: resourceLead ? 'probable-name-lead' : 'unclassified',
        evidence: [
          ...(row.nameLead ? [{ kind: 'package-name-candidate', confidence: 'probable', value: packageName }] : []),
          ...(resourceLead ? [{ kind: 'resource-name', confidence: 'probable', value: entry.name }] : []),
        ],
      };
      if (resourceLead) row.resourceNameLeadCount++;
      resourceRows.push(resource);
    }
  } catch (error) {
    row.status = 'unsupported';
    row.error = String(error?.message ?? error).slice(0, 300);
  }
  return row;
}

async function main() {
  const { source, inventory, output } = parseArgs(process.argv.slice(2));
  const sourceRoot = path.resolve(source);
  const parsedInventory = JSON.parse(await fs.readFile(inventory, 'utf8'));
  if (parsedInventory.kind !== 'private-ui-package-inventory' || !Array.isArray(parsedInventory.resources)) {
    throw new Error('Inventory JSON is not the expected private UI inventory schema.');
  }
  if (path.resolve(parsedInventory.sourceIdentity.sourceTree).toLowerCase() !== sourceRoot.toLowerCase()) {
    throw new Error('Selected source tree does not match the inventory source identity.');
  }
  const executablePath = path.join(sourceRoot, parsedInventory.sourceIdentity.executable.name);
  const executableStat = await fs.stat(executablePath);
  if (executableStat.size !== parsedInventory.sourceIdentity.executable.size || await streamedSha256(executablePath) !== parsedInventory.sourceIdentity.executable.sha256) {
    throw new Error('Selected executable identity does not match the inventory.');
  }
  await preparePrivateOutput(output, { inputs: [sourceRoot, inventory] });
  const packagePaths = await walkXbp(sourceRoot);
  const selectedResources = new Map();
  for (const resource of parsedInventory.resources) {
    const key = inventoryKey(resource);
    selectedResources.set(key, [...(selectedResources.get(key) ?? []), resource]);
  }
  const packages = [];
  const resources = [];
  for (const packagePath of packagePaths) packages.push(await censusPackage(packagePath, sourceRoot, selectedResources, resources));

  const selectedPackages = new Set(parsedInventory.packages.map(pkg => pkg.sourcePath.toLowerCase()));
  for (const pkg of packages) pkg.selectedInventoryPackage = selectedPackages.has(pkg.path.toLowerCase());
  const additionalCandidates = packages
    .filter(pkg => !pkg.selectedInventoryPackage && (pkg.nameLead || pkg.resourceNameLeadCount > 0))
    .map(pkg => ({
      package: pkg.name,
      packagePath: pkg.path,
      packageSha256: pkg.sha256,
      packageNameLead: pkg.nameLead,
      indexedEntries: pkg.indexedEntries,
      resourceNameLeadCount: pkg.resourceNameLeadCount,
      resourceLeads: resources.filter(resource => resource.packagePath === pkg.path && resource.evidence.some(evidence => evidence.kind === 'resource-name')).map(resource => ({
        resourceName: resource.resourceName,
        offset: resource.offset,
        size: resource.size,
        xbpHash: resource.xbpHash,
        stableId: resource.stableId,
        classification: resource.classification,
      })),
      interpretation: 'probable name lead only; resource use and UI role remain unconfirmed',
    }));
  const tableRowsIncludingMetadata = packages.reduce((sum, pkg) => sum + (pkg.tableRowsIncludingMetadata ?? 0), 0);
  const result = {
    schemaVersion: 1,
    kind: 'private-xbp-package-census',
    generatedAt: new Date().toISOString(),
    sourceIdentity: {
      sourceTree: sourceRoot,
      executable: parsedInventory.sourceIdentity.executable,
      inventorySha256: sha256(await fs.readFile(inventory)),
      packageHashes: 'SHA-256 streamed for each package after bounded index parsing; no resource payloads decoded or copied.',
    },
    scope: {
      xbpFilesEncountered: packagePaths.length,
      packagesIndexed: packages.filter(pkg => pkg.status === 'indexed-not-decoded').length,
      packagesUnsupported: packages.filter(pkg => pkg.status === 'unsupported').length,
      tableRowsIncludingMetadata,
      metadataRowsOmittedByReader: packages.reduce((sum, pkg) => sum + (pkg.metadataRowsOmittedByReader ?? 0), 0),
      returnedResourceEntries: resources.length,
      payloadsDecoded: 0,
      selectedInventoryPackages: selectedPackages.size,
      additionalProbablePackages: additionalCandidates.length,
      additionalResourceNameLeads: additionalCandidates.reduce((sum, pkg) => sum + pkg.resourceLeads.length, 0),
      limits: [
        'The existing XBP reader reads only header/table/string-table ranges; resource payloads are never decoded or embedded.',
        'Package SHA-256 requires streaming each package for identity only.',
        'Filename/resource-name matches are leads, not confirmed UI use, screen joins, or game completeness.',
        'Symlinked directories and files are not followed by the traversal.',
      ],
    },
    totals: {
      selectedInventoryResourceMatches: resources.filter(resource => resource.selectedInventoryMatch).length,
      packageNameLeadPackages: packages.filter(pkg => pkg.nameLead).length,
      resourcesInsidePackageNameLeads: resources.filter(resource => packages.some(pkg => pkg.path === resource.packagePath && pkg.nameLead)).length,
      resourceNameLeadEntries: resources.filter(resource => resource.evidence.some(evidence => evidence.kind === 'resource-name')).length,
      probableNameLeadEntries: resources.filter(resource => resource.classification === 'probable-name-lead').length,
      unclassifiedEntries: resources.filter(resource => resource.classification === 'unclassified').length,
      unsupportedPackageCount: packages.filter(pkg => pkg.status === 'unsupported').length,
    },
    packages,
    additionalProbablePackages: additionalCandidates,
    resources,
  };
  await preparePrivateOutput(output, { inputs: [sourceRoot, inventory] });
  await assertNoLinks(output);
  await fs.writeFile(output, `${JSON.stringify(result, null, 2)}\n`);
  console.log(JSON.stringify({ output, ...result.scope, ...result.totals }, null, 2));
}

main().catch(error => { console.error(error.message); process.exitCode = 1; });
