import test from 'node:test';
import assert from 'node:assert/strict';
import { validateProofReport } from '../runtime/browser/proof-report.mjs';

const profile = () => ({ kind: 'synthetic-unnamed-unowned-mutants', synthetic: true, capacity: 4 });
const handleAt = created => 0x00080000 + created * 4;
const state = (count, status = 0, handles = Array.from({ length: count }, (_, i) => ({ handle: handleAt(i + 1), owned: false }))) => ({
  ...profile(), count, status, handles,
});
const successCall = (created, edits = {}) => ({
  ordinal: 192, status: 0, arguments: [0x1000, 0, 0],
  output: { backend: profile().kind, handle: handleAt(created), created,
    eaxBefore: 0xdeadbeef, espBefore: 0x03c00000, eaxAfter: 0, espAfter: 0x03c00010, ...edits },
});
const report = (options = {}) => {
  const { calls = [successCall(1)], ...edits } = options;
  const mutants = Object.hasOwn(options, 'mutants') ? options.mutants : profile();
  const mutantState = Object.hasOwn(options, 'mutantState') ? options.mutantState : state(1);
  return {
    kind: 'thread-startup-boundary', phase: 'original-initializer-bringup', verified: false,
    stop: { status: 10, target: 0xfd000100, ordinal: 1 },
    trace: [{ kind: 'enter', address: 0x1fdc30, stack: 0x03c00000 }],
    originalStartup: { totalInitializers: 83, completedInitializers: 83, distinctOriginalFunctions: 276,
      completedKernelOperations: 403 },
    consoleSettings: { mutants }, mutantState, calls, ...edits,
  };
};

test('mutant profile and state are an explicit pair, with strict profile selection', () => {
  assert.equal(validateProofReport(report({ mutantState: state(0), calls: [] })).mutantState.count, 0);
  assert.equal(validateProofReport(report()).calls[0].output.handle, handleAt(1));
  const missingState = report(); delete missingState.mutantState;
  assert.throws(() => validateProofReport(missingState), /explicit synthetic profile and state/);
  assert.throws(() => validateProofReport(report({ mutants: undefined })), /explicit synthetic profile and state/);
  for (const mutants of [null, [], {}, { ...profile(), capacity: 5 }, { ...profile(), extra: true },
    { ...profile(), synthetic: false }, { ...profile(), kind: 'other' }]) {
    assert.throws(() => validateProofReport(report({ mutants })), /synthetic unnamed mutant/);
  }
});

test('unselected legacy refusal with its single recorded argument remains valid', () => {
  const legacy = report({ mutants: undefined, mutantState: undefined, calls: [
    { ordinal: 192, status: 1, arguments: [0x1000] },
  ], stop: { status: 1, target: 0xfe000018, ordinal: 192,
    service: { ordinal: 192, name: 'NtCreateMutant', modeled: false } } });
  assert.equal(validateProofReport(legacy), legacy);
  assert.throws(() => validateProofReport({ ...legacy, calls: [successCall(1)] }), /explicit synthetic model/);
});

test('successful creation binds three arguments, checked guest output, NTSTATUS and stack effect', () => {
  assert.equal(validateProofReport(report()).calls[0].output.created, 1);
  const edits = [
    { arguments: [0x1000, 0, 0, 0] }, { arguments: [0x1000, 1, 0] },
    { arguments: [0x1000, 0, 1] }, { arguments: [0, 0, 0] },
    { arguments: [0x1002, 0, 0] }, { arguments: [0x04000000, 0, 0] },
    { output: { ...successCall(1).output, extra: true } },
    { output: { ...successCall(1).output, backend: 'native-mutant' } },
    { output: { ...successCall(1).output, handle: 0x1234 } },
    { output: { ...successCall(1).output, created: 2 } },
    { output: { ...successCall(1).output, eaxBefore: -1 } },
    { output: { ...successCall(1).output, espBefore: 0x04000000 } },
    { output: { ...successCall(1).output, eaxAfter: 1 } },
    { output: { ...successCall(1).output, espAfter: 0x03c00014 } },
  ];
  for (const change of edits) {
    const call = { ...successCall(1), ...change };
    assert.throws(() => validateProofReport(report({ calls: [call] })), /mutant/i);
  }
  for (const badState of [
    state(1, 0, [{ handle: handleAt(1), owned: true }]),
    state(1, 0, [{ handle: 0x1234, owned: false }]),
    { ...state(1), extra: true },
    { ...state(1), count: 2 },
  ]) assert.throws(() => validateProofReport(report({ mutantState: badState })), /mutant/i);
});

test('success count and creation indices bind retained state; zero-entry state is valid', () => {
  const empty = report({ mutantState: state(0), calls: [] });
  assert.equal(validateProofReport(empty), empty);
  const two = report({ mutantState: state(2), calls: [successCall(1), successCall(2)] });
  assert.equal(validateProofReport(two), two);
  assert.throws(() => validateProofReport(report({ mutantState: state(2), calls: [successCall(1)] })), /Mutant count/);
  assert.throws(() => validateProofReport(report({ mutantState: state(1), calls: [successCall(2)] })), /Mutant creation order/);
});

test('mutant refusal binds a nonzero state status to the final guarded ordinal-192 stop', () => {
  for (const status of [0xc000000d, 0xc00000bb, 0xc0000005, 0xc000009a]) {
    const count = status === 0xc000009a ? 4 : 0;
    const calls = [
      ...Array.from({ length: count }, (_, i) => successCall(i + 1)),
      { ordinal: 192, status: 1, arguments: [0x1000, 0, 0] },
    ];
    const refusal = report({ mutantState: state(count, status), calls,
      stop: { status: status === 0xc000009a ? 4 : 9, target: status, ordinal: 192 } });
    assert.equal(validateProofReport(refusal), refusal);
    for (const stop of [
      { ...refusal.stop, ordinal: 191 }, { ...refusal.stop, target: 0xc0000001 },
      { ...refusal.stop, status: refusal.stop.status === 4 ? 9 : 4 },
    ]) assert.throws(() => validateProofReport({ ...refusal, stop }), /Mutant refusal/);
    assert.throws(() => validateProofReport({ ...refusal, calls: [...refusal.calls.slice(0, -1),
      { ...refusal.calls.at(-1), output: successCall(1).output }] }), /Mutant refusal/);
  }
  const fullWithoutCapacity = report({ mutantState: state(3, 0xc000009a), calls: [successCall(1), successCall(2), successCall(3)],
    stop: { status: 4, target: 0xc000009a, ordinal: 192 } });
  assert.throws(() => validateProofReport(fullWithoutCapacity), /Mutant refusal/);
  const failed = { ordinal: 192, status: 1, arguments: [0x1000, 0, 0] };
  const invalidStatus = 0xc000000d;
  const matchingStop = { status: 9, target: invalidStatus, ordinal: 192 };
  assert.throws(() => validateProofReport(report({ mutantState: state(0, invalidStatus),
    calls: [failed, failed], stop: matchingStop })), /refusal.*last captured call/i);
  assert.throws(() => validateProofReport(report({ mutantState: state(1, invalidStatus),
    calls: [failed, successCall(1)], stop: matchingStop })), /refusal.*last captured call/i);
});
