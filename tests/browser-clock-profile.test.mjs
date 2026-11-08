import test from 'node:test';
import assert from 'node:assert/strict';
import { createProofClock, parseSystemClockProfile } from '../runtime/browser/browser-clock.mjs';

const UNIX_EPOCH_AS_1601_TICKS = 116444736000000000n;
const profile = unixMillis => ({ kind: 'fixed-system-clock', synthetic: true, unixMillis });

test('system clock profile is opt-in, exact, bounded and returned as a fresh DTO', () => {
  assert.equal(parseSystemClockProfile(undefined), undefined);
  const input = profile(0);
  const parsed = parseSystemClockProfile(input);
  assert.deepEqual(parsed, input);
  assert.notEqual(parsed, input);
  for (const bad of [null, [], {}, { ...input, extra: 1 }, { ...input, synthetic: false },
    { ...input, kind: 'other' }, { kind: input.kind, synthetic: true },
    ...[-1, 0.5, NaN, Infinity, Number.MAX_SAFE_INTEGER, 253402300800000, null].map(unixMillis => profile(unixMillis))]) {
    assert.throws(() => parseSystemClockProfile(bad), /synthetic fixed system clock/);
  }
});

test('fixed system time maps Unix epoch to 1601 ticks while monotonic time advances', () => {
  let now = 100;
  const fixed = profile(0);
  const clock = createProofClock({ monotonic: () => now, wall: () => { throw new Error('wall clock must not be sampled'); }, fixedSystemProfile: fixed });
  fixed.unixMillis = 1000;
  assert.deepEqual(clock.sample(), { interrupt: 0n, system: UNIX_EPOCH_AS_1601_TICKS });
  now = 100.125;
  assert.deepEqual(clock.sample(), { interrupt: 1250n, system: UNIX_EPOCH_AS_1601_TICKS });
  const described = clock.describe();
  assert.equal(described.system, 'configured synthetic fixed Unix time; UTC epoch 1601');
  assert.deepEqual(described.fixedSystemProfile, profile(0));
  assert.notEqual(described.fixedSystemProfile, fixed);
  described.fixedSystemProfile.unixMillis = 9;
  assert.deepEqual(clock.describe().fixedSystemProfile, profile(0));
});

test('default system clock retains live Date.now behavior and description shape', () => {
  let now = 10, date = 1234;
  const clock = createProofClock({ monotonic: () => now, wall: () => date });
  const first = clock.sample();
  assert.equal(first.system, (BigInt(date) + 11644473600000n) * 10000n);
  now = 10.25;
  date = 2000;
  const second = clock.sample();
  assert.equal(second.interrupt, 2500n);
  assert.equal(second.system, (BigInt(date) + 11644473600000n) * 10000n);
  assert.deepEqual(clock.describe(), {
    kind: 'browser-clock', monotonic: 'performance.now', system: 'Date.now; UTC epoch 1601', samples: 2,
    minimumObservedPositiveTicks: '2500',
    limitation: 'Observed sample deltas are not timer resolution or original-console/suspend fidelity.'
  });
});
