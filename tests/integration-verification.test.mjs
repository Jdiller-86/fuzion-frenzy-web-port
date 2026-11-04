import test from 'node:test';
import assert from 'node:assert/strict';

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
