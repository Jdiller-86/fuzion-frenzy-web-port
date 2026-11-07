import test from 'node:test';
import assert from 'node:assert/strict';
import { parsePerformanceClockProfile, readPerformanceClockState, validatePerformanceClockState } from '../runtime/browser/performance-clock.mjs';

const profile = frequencyHz => ({
  kind: 'synthetic-performance-clock', synthetic: true, frequencyHz,
  source: 'browser-monotonic-100ns', origin: 'proof-clock-zero'
});
const validHeader = frequencyHz => [1, frequencyHz, 2, 64, 1, 0x100, 3, 2];
const words = value => [Number(value & 0xffffffffn), Number((value >> 32n) & 0xffffffffn)];

function stateGetter({ header = validHeader(1000000), sites = [0x100, 0x200], samples } = {}) {
  const rows = samples ?? [
    [0x100, ...words(10000000n), ...words(1000000n)],
    [0x200, ...words(20000000n), ...words(2000000n)]
  ];
  return (index, field) => {
    if (index === 0xffffffff) return header[field];
    if (field === 5) return sites[index];
    return rows[index]?.[field];
  };
}

test('performance profile is opt-in, exact, bounded and copied', () => {
  assert.equal(parsePerformanceClockProfile(undefined), undefined);
  const input = profile(1_000_000);
  const parsed = parsePerformanceClockProfile(input);
  assert.deepEqual(parsed, input);
  assert.notEqual(parsed, input);
  for (const bad of [null, [], {}, { ...input, extra: 1 }, { ...input, synthetic: false },
    { ...input, kind: 'other' }, { kind: input.kind, synthetic: true },
    ...[0, -1, 1.5, NaN, Infinity, 1_000_000_001, null].map(frequencyHz => profile(frequencyHz)),
    { ...input, source: 'wall-clock' }, { ...input, origin: 'unknown' }]) {
    assert.throws(() => parsePerformanceClockProfile(bad), /synthetic performance clock/);
  }
});

test('reads validated profile, header, sorted whitelist and exact 64-bit samples', () => {
  const result = readPerformanceClockState(stateGetter(), profile(1_000_000));
  assert.deepEqual(result, {
    ...profile(1_000_000), capacity: 64, status: 1, lastSite: 0x100, frequencyQueries: 3,
    admittedSites: [0x100, 0x200],
    samples: [
      { site: 0x100, interruptTicks: '10000000', counter: '1000000' },
      { site: 0x200, interruptTicks: '20000000', counter: '2000000' }
    ]
  });
});

test('rejects malformed header values, readiness, profile mismatch and oversized counts', () => {
  for (const [field, value] of [[0, 0], [1, 999], [2, 65], [3, 63], [4, 5], [5, -1], [6, 0x1_0000_0000], [7, 0]]) {
    const header = validHeader(1_000_000);
    header[field] = value;
    assert.throws(() => readPerformanceClockState(stateGetter({ header }), profile(1_000_000)));
  }
  assert.throws(() => readPerformanceClockState(stateGetter(), profile(2_000_000)), /header/);
  assert.throws(() => readPerformanceClockState(stateGetter(), undefined), /required/);
});

test('rejects unsorted, duplicate, zero and non-whitelisted sites', () => {
  for (const sites of [[0, 0x200], [0x200, 0x100], [0x100, 0x100]]) {
    assert.throws(() => readPerformanceClockState(stateGetter({ sites }), profile(1_000_000)), /admitted-site/);
  }
  const badSample = [0x300, ...words(10000000n), ...words(1000000n)];
  assert.throws(() => readPerformanceClockState(stateGetter({ samples: [badSample, [0x200, ...words(20000000n), ...words(2000000n)]] }), profile(1_000_000)), /not admitted/);
  const header = validHeader(1_000_000);
  header[5] = 0x300;
  assert.throws(() => readPerformanceClockState(stateGetter({ header }), profile(1_000_000)), /last site/);
});

test('rejects decreasing tick samples and incorrect frequency arithmetic', () => {
  const secondEarlier = [0x200, ...words(9999999n), ...words(999999n)];
  assert.throws(() => readPerformanceClockState(stateGetter({ samples: [
    [0x100, ...words(10000000n), ...words(1000000n)], secondEarlier
  ] }), profile(1_000_000)), /order or counter/);
  const wrongCounter = [0x100, ...words(10000000n), ...words(999999n)];
  assert.throws(() => readPerformanceClockState(stateGetter({ samples: [wrongCounter] }), profile(1_000_000)), /order or counter/);
});

test('serialized state validator returns a canonical state and requires its selected profile', () => {
  const state = readPerformanceClockState(stateGetter(), profile(1_000_000));
  const canonical = validatePerformanceClockState(state, profile(1_000_000));
  assert.deepEqual(canonical, state);
  assert.notEqual(canonical, state);
  assert.throws(() => validatePerformanceClockState(state, undefined), /required/);
});

test('serialized state rejects extra keys, mismatched profiles, malformed bounds and decimals', () => {
  const valid = readPerformanceClockState(stateGetter(), profile(1_000_000));
  const cases = [
    { ...valid, extra: true },
    { ...valid, frequencyHz: 2_000_000 },
    { ...valid, capacity: 63 },
    { ...valid, status: 5 },
    { ...valid, lastSite: 0x300 },
    { ...valid, frequencyQueries: -1 },
    { ...valid, admittedSites: Array(65).fill(1) },
    { ...valid, samples: Array(65).fill(valid.samples[0]) },
    { ...valid, admittedSites: [0x100, 0x100, 0x300] },
    { ...valid, samples: [{ ...valid.samples[0], extra: 1 }, valid.samples[1]] },
    { ...valid, samples: [{ ...valid.samples[0], interruptTicks: '01' }, valid.samples[1]] },
    { ...valid, samples: [{ ...valid.samples[0], counter: '18446744073709551616' }, valid.samples[1]] },
    { ...valid, samples: [{ ...valid.samples[0], counter: '1000001' }, valid.samples[1]] }
  ];
  for (const state of cases) assert.throws(() => validatePerformanceClockState(state, profile(1_000_000)));
});
