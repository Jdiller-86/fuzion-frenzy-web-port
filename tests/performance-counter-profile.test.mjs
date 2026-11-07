import test from 'node:test';
import assert from 'node:assert/strict';
import {
  parsePerformanceCounterProfile, readPerformanceCounterState, validatePerformanceCounterState
} from '../runtime/browser/performance-counter.mjs';

const profile = () => ({ kind: 'synthetic-performance-counter', synthetic: true, source: 'paired-performance-clock' });
const clock = frequencyHz => ({ kind: 'synthetic-performance-clock', synthetic: true, frequencyHz,
  source: 'browser-monotonic-100ns', origin: 'proof-clock-zero' });
const HEADER = 0xffffffff;
const words = value => [Number(value & 0xffffffffn), Number((value >> 32n) & 0xffffffffn)];
const headerFor = ({ frequencyHz = 1_000_000, count = 1, status = 2, ticks = 12_345_678n } = {}) => {
  const counter = ticks * BigInt(frequencyHz) / 10_000_000n;
  return [1, frequencyHz, count, 64, status, ...words(ticks), ...words(counter)];
};
const getterFor = header => (index, field) => index === HEADER ? header[field] : undefined;

test('counter profile is opt-in, exact, and returned as a fresh canonical value', () => {
  assert.equal(parsePerformanceCounterProfile(undefined), undefined);
  const input = profile();
  const canonical = parsePerformanceCounterProfile(input);
  assert.deepEqual(canonical, input);
  assert.notEqual(canonical, input);
  for (const bad of [null, [], {}, { ...input, extra: 1 }, { ...input, synthetic: false },
    { ...input, kind: 'other' }, { ...input, source: 'wall-clock' }, { kind: input.kind, synthetic: true }]) {
    assert.throws(() => parsePerformanceCounterProfile(bad), /synthetic performance counter/);
  }
});

test('reads bounded u32 header and checked exact scaling with shared clock frequency', () => {
  const header = headerFor();
  assert.deepEqual(readPerformanceCounterState(getterFor(header), profile(), clock(1_000_000)), {
    ...profile(), frequencyHz: 1_000_000, count: 1, capacity: 64, status: 2,
    lastTicks: '12345678', lastCounter: '1234567'
  });
  assert.deepEqual(readPerformanceCounterState(getterFor(headerFor({ count: 64, status: 4 })), profile(), clock(1_000_000)).count, 64);
  assert.throws(() => readPerformanceCounterState(getterFor(header), profile(), clock(2_000_000)), /header/);
  assert.throws(() => readPerformanceCounterState(getterFor(header), undefined, clock(1_000_000)), /required/);
  assert.throws(() => readPerformanceCounterState(getterFor(header), profile(), undefined), /required/);
});

test('rejects invalid u32 words, flags, capacity, status and count bounds', () => {
  for (const [field, value] of [[0, 0], [0, 2], [1, -1], [1, 0x1_0000_0000], [2, 65], [3, 63], [4, 5], [5, -1], [6, NaN], [7, Infinity], [8, '0']]) {
    const header = headerFor(); header[field] = value;
    assert.throws(() => readPerformanceCounterState(getterFor(header), profile(), clock(1_000_000)));
  }
});

test('checks zero-count consistency and permits a nonzero count whose scaled counter is zero', () => {
  const zeroCounter = headerFor({ count: 1, ticks: 1n, frequencyHz: 1 });
  assert.equal(readPerformanceCounterState(getterFor(zeroCounter), profile(), clock(1)).lastCounter, '0');
  const empty = headerFor({ count: 0, ticks: 0n });
  assert.equal(readPerformanceCounterState(getterFor(empty), profile(), clock(1_000_000)).count, 0);
  const inconsistent = headerFor({ count: 0, ticks: 1n });
  assert.throws(() => readPerformanceCounterState(getterFor(inconsistent), profile(), clock(1_000_000)), /empty/);
});

test('validates exact serialized shape, canonical u64 decimals and recomputed scaling', () => {
  const state = readPerformanceCounterState(getterFor(headerFor()), profile(), clock(1_000_000));
  const reordered = Object.fromEntries(Object.entries(state).reverse());
  const canonical = validatePerformanceCounterState(reordered, profile(), clock(1_000_000));
  assert.deepEqual(canonical, state);
  assert.notEqual(canonical, state);
  assert.throws(() => validatePerformanceCounterState(state, undefined, clock(1_000_000)), /required/);
  assert.throws(() => validatePerformanceCounterState(state, profile(), undefined), /required/);
  for (const changed of [
    { ...state, extra: 1 }, { ...state, kind: 'other' }, { ...state, frequencyHz: 2_000_000 },
    { ...state, count: 65 }, { ...state, capacity: 63 }, { ...state, status: 5 },
    { ...state, lastTicks: '00' }, { ...state, lastTicks: '-1' }, { ...state, lastTicks: '1'.repeat(21) },
    { ...state, lastTicks: '18446744073709551616' }, { ...state, lastCounter: '999' }
  ]) assert.throws(() => validatePerformanceCounterState(changed, profile(), clock(1_000_000)));
});

test('accepts maximum u64 words when their exact frequency result fits u64', () => {
  const ticks = (1n << 64n) - 1n;
  const header = headerFor({ frequencyHz: 1, ticks });
  const state = readPerformanceCounterState(getterFor(header), profile(), clock(1));
  assert.equal(state.lastTicks, ticks.toString());
  assert.equal(validatePerformanceCounterState(state, profile(), clock(1)).lastTicks, ticks.toString());
});
