import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { execFileSync, spawnSync } from 'node:child_process';

import {
  parseDecisionIds,
  duplicateIds,
  missingDecisionIds,
  missingFiles,
} from '../tools/verify-integration.mjs';

const table = (rows) => [
  '| ID | Decision | Basis |',
  '| --- | --- | --- |',
  ...rows,
  '',
].join('\n');

test('decision IDs are read from table rows only', () => {
  const text = [
    'Intro mentions A001 in prose.',
    table(['| A001 | one | x |', '| D001 | two | y |', '| W001 | three | z |']),
    'A002 is also prose, not a row.',
  ].join('\n');
  assert.deepEqual(parseDecisionIds(text), ['A001', 'D001', 'W001']);
  assert.deepEqual(parseDecisionIds(''), []);
});

test('duplicate decision rows are reported once per repeat', () => {
  assert.deepEqual(duplicateIds(['A001', 'D001', 'D001', 'D001', 'A001']), ['D001', 'D001', 'A001']);
  assert.deepEqual(duplicateIds(['A001', 'D001']), []);
});

test('the union check catches a dropped parent row', () => {
  const base = parseDecisionIds(table(['| A001 | main | x |', '| A023 | main | y |']));
  const source = parseDecisionIds(table(['| D247 | lane | a |', '| D248 | lane | b |']));
  const good = parseDecisionIds(table([
    '| A001 | main | x |',
    '| A023 | main | y |',
    '| D247 | lane | a |',
    '| D248 | lane | b |',
  ]));
  assert.deepEqual(missingDecisionIds(base, source, good), { base: [], source: [], duplicates: [] });

  const lossy = parseDecisionIds(table(['| A023 | main | y |', '| D247 | lane | a |', '| D248 | lane | b |']));
  const result = missingDecisionIds(base, source, lossy);
  assert.deepEqual(result.base, ['A001']);
  assert.deepEqual(result.source, []);
  assert.deepEqual(result.duplicates, []);
});

test('a duplicated candidate row is rejected even when all parents are present', () => {
  const base = parseDecisionIds(table(['| A001 | main | x |']));
  const source = parseDecisionIds(table(['| D001 | lane | y |']));
  const candidate = parseDecisionIds(table(['| A001 | main | x |', '| D001 | lane | y |', '| A001 | main | x |']));
  const result = missingDecisionIds(base, source, candidate);
  assert.deepEqual(result.base, []);
  assert.deepEqual(result.source, []);
  assert.deepEqual(result.duplicates, ['A001']);
});

test('the file-set check reports only source paths the candidate lacks', () => {
  assert.deepEqual(missingFiles(['a.c', 'b.mjs'], ['b.mjs', 'a.c', 'extra.mjs']), []);
  assert.deepEqual(missingFiles(['a.c', 'b.mjs'], ['extra.mjs']), ['a.c', 'b.mjs']);
  assert.deepEqual(missingFiles([], ['a.c']), []);
});

test('the exact-head precheck runs from the command line and prints the marker', () => {
  const directory = fs.mkdtempSync(path.join(os.tmpdir(), 'exact-head-cli-'));
  const git = (args) => execFileSync('git', args, {
    cwd: directory, encoding: 'utf8', stdio: ['ignore', 'pipe', 'pipe'],
  });
  git(['init', '--quiet']);
  fs.writeFileSync(path.join(directory, 'a.txt'), 'one\n');
  git(['add', '-A']);
  git(['-c', 'user.email=lane@example.invalid', '-c', 'user.name=Lane Test',
    'commit', '--quiet', '-m', 'first']);
  const branch = git(['rev-parse', '--abbrev-ref', 'HEAD']).trim();
  git(['branch', 'architect']);
  fs.writeFileSync(path.join(directory, 'b.txt'), 'two\n');
  git(['add', '-A']);
  git(['-c', 'user.email=lane@example.invalid', '-c', 'user.name=Lane Test',
    'commit', '--quiet', '-m', 'second']);

  const run = () => spawnSync(process.execPath, [
    path.resolve('tools/verify-exact-head.mjs'), '--repo', directory, 'HEAD',
    '--architect-ref', 'architect', '--base-ref', branch, '--local-ref', branch,
  ], { encoding: 'utf8' });

  const stale = run();
  assert.equal(stale.status, 1, stale.stdout);
  assert.match(stale.stdout, /is not the architect head/);
  assert.match(stale.stdout, /exact-head marker: none/);
  assert.match(stale.stdout, /not asserted by this tool/);

  git(['branch', '-f', 'architect', 'HEAD']);
  const exact = run();
  assert.equal(exact.status, 0, exact.stdout + exact.stderr);
  assert.match(exact.stdout, /match/);
  assert.match(exact.stdout, /exact-head marker: [0-9a-f]{40}/);
});
