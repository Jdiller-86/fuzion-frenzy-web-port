import assert from 'node:assert/strict';
import test from 'node:test';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { spawnSync } from 'node:child_process';
import { analyzeReachability, analyzeTranslation, parseRunEvidence } from '../translator/translation-frontier.mjs';

const translation = [
  '/**',
  ' * ff_title_00100000',
  ' */',
  'void ff_title_00100000(void)',
  '{',
  'loc_00100000: ;',
  '    FF_MEMORY_PUSH(esp, 0x00100010u); RECOMP_ABI_CALL(0x00100100u, ff_title_00100100); /* call */',
  '    FF_MEMORY_PUSH(esp, 0x0010001Cu); RECOMP_ABI_CALL(0x00100200u, ff_title_00100200); /* call */',
  '}',
  '',
  'void ff_title_00100100(void)',
  '{',
  'loc_00100100: ;',
  '    eax = 0;',
  '}',
  '',
  'void ff_title_00100200(void) { ff_probe_stop_direct(0x00100200u); }',
  'void ff_title_00100300(void) { ff_probe_stop_direct(0x00100300u); }',
  '',
].join('\n');

test('the frontier report counts functions, placeholders and called placeholders', () => {
  const report = analyzeTranslation(translation);
  assert.equal(report.functions, 4);
  assert.equal(report.placeholders, 2);
  assert.equal(report.referencedPlaceholders, 1);
  assert.equal(report.callTargets, 2);
  assert.deepEqual(report.referenced, [0x00100200]);
});

test('the frontier report attributes placeholders to their callers', () => {
  const report = analyzeTranslation(translation);
  assert.deepEqual(report.referencedBy[0x00100200], [0x00100000]);
  assert.equal(report.referencedBy[0x00100300], undefined);
});

test('a translation without placesholders reports zero placeholders', () => {
  const report = analyzeTranslation('void ff_title_00000010(void)\n{\nloc_00000010: ;\n}\n');
  assert.equal(report.functions, 1);
  assert.equal(report.placeholders, 0);
  assert.equal(report.referencedPlaceholders, 0);
  assert.equal(report.callTargets, 0);
  assert.deepEqual(report.referenced, []);
});

test('calls are attributed to the enclosing function only', () => {
  const text = [
    'void ff_title_00000010(void)',
    '{',
    '  RECOMP_ABI_CALL(0x00000020u, ff_title_00000020);',
    '}',
    'void ff_title_00000020(void)',
    '{',
    '  /* no calls */',
    '}',
    '',
  ].join('\n');
  const report = analyzeTranslation(text);
  assert.equal(report.callTargets, 1);
  assert.equal(report.referencedPlaceholders, 0);
});

const reachTranslation = [
  'void ff_title_00100000(void)',
  '{',
  '  RECOMP_ABI_CALL(0x00100200u, ff_title_00100200);',
  '  RECOMP_ABI_CALL(0x00100500u, ff_title_00100500);',
  '}',
  'void ff_title_00100400(void)',
  '{',
  '  RECOMP_ABI_CALL(0x00100300u, ff_title_00100300);',
  '}',
  'void ff_title_00100200(void) { ff_probe_stop_direct(0x00100200u); }',
  'void ff_title_00100300(void) { ff_probe_stop_direct(0x00100300u); }',
  'void ff_title_00100500(void) { ff_probe_stop_direct(0x00100500u); }',
  '',
].join('\n');

const reachReport = {
  originalFunctionsReached: [0x00100000],
  trace: [
    { kind: 'enter', address: 0x00100000 },
    { kind: 'enter', address: 0x00100500 },
    { kind: 'exit', address: 0x00100500 },
    { kind: 'exit', address: 0x00100000 },
  ],
  traceCapacity: 8192,
};

test('run evidence partitions entered, executed-caller and static placeholders', () => {
  const report = analyzeTranslation(reachTranslation);
  const reach = analyzeReachability(report, reachReport);
  assert.deepEqual(reach.entered, [{ placeholder: 0x00100500, callers: [0x00100000] }]);
  assert.deepEqual(reach.calledByExecutedCaller, [{ placeholder: 0x00100200, callers: [0x00100000] }]);
  assert.deepEqual(reach.staticOnly, [0x00100300]);
  assert.equal(reach.executedFunctions, 2);
  assert.equal(reach.truncated, false);
});

test('run evidence rejects malformed addresses and event kinds', () => {
  assert.throws(() => parseRunEvidence({}), /originalFunctionsReached or trace/);
  assert.throws(() => parseRunEvidence({ originalFunctionsReached: [-1] }), /invalid function address/);
  assert.throws(() => parseRunEvidence({ originalFunctionsReached: [1], trace: [{ kind: 'jump', address: 1 }] }),
    /enter or exit/);
  assert.throws(() => parseRunEvidence({ originalFunctionsReached: [1], trace: [{ kind: 'enter', address: 0x100000000 }] }),
    /invalid function address/);
});

test('an unmatched exit is tolerated because tail calls leave the stack unbalanced', () => {
  const evidence = parseRunEvidence({
    originalFunctionsReached: [1],
    trace: [{ kind: 'enter', address: 1 }, { kind: 'exit', address: 2 }],
  });
  assert.deepEqual([...evidence.executed], [1]);
  assert.equal(evidence.truncated, false);
});

test('run evidence accepts a trace-only report and reports truncation', () => {
  const evidence = parseRunEvidence({
    trace: [{ kind: 'enter', address: 0x10 }, { kind: 'exit', address: 0x10 }],
    traceCapacity: 2,
  });
  assert.deepEqual([...evidence.executed], [0x10]);
  assert.equal(evidence.truncated, true);
});

test('a status-2 stop target is an entered placeholder even when absent from the reached set', () => {
  const report = analyzeTranslation(reachTranslation);
  const reach = analyzeReachability(report, {
    originalFunctionsReached: [0x00100000],
    stop: { status: 2, target: 0x00100200 },
  });
  assert.deepEqual(reach.entered, [{ placeholder: 0x00100200, callers: [0x00100000] }]);
  assert.equal(reach.executedFunctions, 1);
  assert.throws(() => parseRunEvidence({ originalFunctionsReached: [1], stop: { status: 2 } }),
    /invalid function address/);
});

test('the command line reads a run report without mistaking it for the translation', () => {
  const directory = fs.mkdtempSync(path.join(os.tmpdir(), 'frontier-'));
  fs.writeFileSync(path.join(directory, 'title-functions.inc'), reachTranslation);
  const run = path.join(directory, 'run.json');
  fs.writeFileSync(run, JSON.stringify(reachReport));
  const result = spawnSync(process.execPath, [
    path.resolve('translator/translation-frontier.mjs'), directory, '--run', run, '--json',
  ], { encoding: 'utf8' });
  assert.equal(result.status, 0, result.stderr);
  const parsed = JSON.parse(result.stdout);
  assert.equal(parsed.functions, 5);
  assert.deepEqual(parsed.reachability.entered, [{ placeholder: 0x00100500, callers: [0x00100000] }]);
});
