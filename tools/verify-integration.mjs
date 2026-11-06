#!/usr/bin/env node
/**
 * Verify that an integration candidate preserves both lanes.
 *
 * The report/helper integration lost main-only decision rows twice: a
 * `-X theirs` merge reduced docs/decisions.md to 245 rows with only A023, and a
 * 20-commit rebase dropped 28 pre-slice source files. This check makes the
 * invariant explicit and repeatable:
 *
 *   - every decision row present on either parent is present on the candidate;
 *   - the candidate has no duplicate decision IDs;
 *   - every path tracked by the source lane is tracked by the candidate.
 *
 * It reads only decision IDs, file paths and refs. It does not read original
 * content, change a ref or decide whether the integration is correct.
 *
 * Usage: node tools/verify-integration.mjs <base-ref> <source-ref> <candidate-ref>
 */
import { execFileSync } from 'node:child_process';
import { pathToFileURL } from 'node:url';

export function parseDecisionIds(text) {
  const ids = [];
  for (const line of text.split('\n')) {
    const match = line.match(/^\| ([A-Z]\d{3}) \|/);
    if (match) ids.push(match[1]);
  }
  return ids;
}

export function duplicateIds(ids) {
  const seen = new Set();
  const duplicates = [];
  for (const id of ids) {
    if (seen.has(id)) duplicates.push(id);
    seen.add(id);
  }
  return duplicates;
}

export function missingDecisionIds(baseIds, sourceIds, candidateIds) {
  const present = new Set(candidateIds);
  return {
    base: baseIds.filter((id) => !present.has(id)),
    source: sourceIds.filter((id) => !present.has(id)),
    duplicates: duplicateIds(candidateIds),
  };
}

export function missingFiles(sourceFiles, candidateFiles) {
  const present = new Set(candidateFiles);
  return sourceFiles.filter((file) => !present.has(file));
}

function git(args) {
  return execFileSync('git', args, { encoding: 'utf8', timeout: 60000 });
}

function decisionText(ref) {
  return git(['show', `${ref}:docs/decisions.md`]);
}

function trackedFiles(ref) {
  return git(['ls-tree', '-r', '--name-only', ref]).split('\n').filter(Boolean);
}

function main(argv) {
  const [base, source, candidate] = argv;
  if (!base || !source || !candidate) {
    console.error('usage: node tools/verify-integration.mjs <base-ref> <source-ref> <candidate-ref>');
    return 2;
  }
  let baseText;
  let sourceText;
  let candidateText;
  try {
    baseText = decisionText(base);
    sourceText = decisionText(source);
    candidateText = decisionText(candidate);
  } catch (error) {
    console.error(`Cannot read docs/decisions.md from one of the refs: ${error.message}`);
    return 2;
  }
  const baseIds = parseDecisionIds(baseText);
  const sourceIds = parseDecisionIds(sourceText);
  const candidateIds = parseDecisionIds(candidateText);
  const decisions = missingDecisionIds(baseIds, sourceIds, candidateIds);
  const files = missingFiles(trackedFiles(source), trackedFiles(candidate));

  console.log(`decision rows: base ${baseIds.length}, source ${sourceIds.length}, candidate ${candidateIds.length}`);
  console.log(`missing base rows: ${decisions.base.length}`);
  console.log(`missing source rows: ${decisions.source.length}`);
  console.log(`duplicate candidate rows: ${decisions.duplicates.length}`);
  console.log(`missing source files: ${files.length}`);
  for (const id of decisions.base.slice(0, 10)) console.log(`  missing base decision ${id}`);
  for (const id of decisions.source.slice(0, 10)) console.log(`  missing source decision ${id}`);
  for (const id of decisions.duplicates.slice(0, 10)) console.log(`  duplicate decision ${id}`);
  for (const file of files.slice(0, 10)) console.log(`  missing source file ${file}`);

  const failed = decisions.base.length > 0
    || decisions.source.length > 0
    || decisions.duplicates.length > 0
    || files.length > 0;
  if (failed) {
    console.error('integration candidate does not preserve both lanes');
    return 1;
  }
  console.log('integration candidate preserves both decision logs and the source file set');
  return 0;
}

if (process.argv[1] && import.meta.url === pathToFileURL(process.argv[1]).href) {
  process.exitCode = main(process.argv.slice(2));
}
