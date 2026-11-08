import test from 'node:test';
import assert from 'node:assert/strict';
import { parseMutantProfile, readMutantState, validateMutantState } from '../runtime/browser/unnamed-mutants.mjs';

const profile = () => ({ kind: 'synthetic-unnamed-unowned-mutants', synthetic: true, capacity: 4 });
const HEADER = 0xffffffff;
const statusValues = [0, 0xc000000d, 0xc00000bb, 0xc0000005, 0xc000009a];
const getterFor = (count = 2, status = 0) => (index, field) => {
  if (index === HEADER) return [1, count, 4, status][field];
  if (index >= 0 && index < count) return [[0x00080004 + index * 4, 0][field]]?.[0];
  return undefined;
};

test('profile is opt-in, exact and canonical', () => {
  assert.equal(parseMutantProfile(undefined), undefined);
  const input = profile();
  const canonical = parseMutantProfile(input);
  assert.deepEqual(canonical, input);
  assert.notEqual(canonical, input);
  for (const bad of [null, [], {}, { ...input, extra: 1 }, { ...input, synthetic: false },
    { ...input, kind: 'other' }, { ...input, capacity: 5 }, { kind: input.kind, synthetic: true }]) {
    assert.throws(() => parseMutantProfile(bad), /synthetic unnamed mutant/);
  }
});

test('reads bounded ready header, status and exact unowned synthetic handles', () => {
  for (const status of statusValues) {
    const state = readMutantState(getterFor(2, status), profile());
    assert.deepEqual(state, { ...profile(), count: 2, status, handles: [
      { handle: 0x80004, owned: false }, { handle: 0x80008, owned: false }
    ] });
  }
  assert.equal(readMutantState(getterFor(0), profile()).count, 0);
  assert.equal(readMutantState(getterFor(4), profile()).handles.length, 4);
  assert.throws(() => readMutantState(getterFor(), undefined), /required/);
});

test('rejects malformed getter words and row ownership or handles', () => {
  for (const [field, value] of [[0, 0], [1, 5], [2, 3], [3, 1], [0, -1], [0, 0x1_0000_0000], [0, NaN], [0, '1']]) {
    const base = getterFor(1);
    const getter = (index, current) => index === HEADER && current === field ? value : base(index, current);
    assert.throws(() => readMutantState(getter, profile()));
  }
  for (const badRow of [[0x80008, 0], [0x80004, 1], [0x80004, 2]]) {
    assert.throws(() => readMutantState((index, field) => index === HEADER ? [1, 1, 4, 0][field] : badRow[field], profile()));
  }
});

test('validates serialized exact shape, status, count and DTO copies', () => {
  const state = readMutantState(getterFor(), profile());
  const copy = validateMutantState(JSON.parse(JSON.stringify(state)), profile());
  assert.deepEqual(copy, state);
  assert.notEqual(copy, state);
  assert.notEqual(copy.handles, state.handles);
  assert.notEqual(copy.handles[0], state.handles[0]);
  assert.throws(() => validateMutantState(state, undefined), /required/);
  for (const changed of [
    { ...state, extra: 1 }, { ...state, kind: 'other' }, { ...state, synthetic: false },
    { ...state, capacity: 5 }, { ...state, count: 3 }, { ...state, status: 1 },
    { ...state, handles: [{ ...state.handles[0], owned: true }, state.handles[1]] },
    { ...state, handles: [{ ...state.handles[0], handle: 0x1234 }, state.handles[1]] },
    { ...state, handles: [{ ...state.handles[0], extra: true }, state.handles[1]] }
  ]) assert.throws(() => validateMutantState(changed, profile()));
  const forbiddenRows = new Proxy([], { get() { throw new Error('oversized handles inspected'); } });
  assert.throws(() => validateMutantState({ ...state, count: 5, handles: forbiddenRows }, profile()), /exceeds capacity/);
});
