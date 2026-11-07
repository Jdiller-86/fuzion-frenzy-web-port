import test from 'node:test';
import assert from 'node:assert/strict';
import { createProofClock, parseDeterministicClockProfile } from '../runtime/browser/browser-clock.mjs';

const profile = (stepTicks = 7, jumpBeforeSample = 3, jumpTicks = 100) => ({
  kind: 'deterministic-monotonic-clock', synthetic: true, stepTicks, jumpBeforeSample, jumpTicks
});
const fixed = { kind: 'fixed-system-clock', synthetic: true, unixMillis: 1234 };
const pause = { kind: 'one-shot-browser-clock-pause', synthetic: true, beforeSample: 1, milliseconds: 1 };

test('deterministic profile parser accepts only the exact bounded shape and copies it', () => {
  assert.equal(parseDeterministicClockProfile(undefined), undefined);
  const input = profile(), parsed = parseDeterministicClockProfile(input);
  assert.deepEqual(parsed, input);
  assert.notEqual(parsed, input);
  for (const bad of [null, [], {}, { ...input, extra: true }, { ...input, synthetic: false },
    { ...input, kind: 'other' }, ...[[0, 1, 1], [10001, 1, 1], [1, 0, 1], [1, 16385, 1],
      [1, 1.5, 1], [1, 1, 0], [1, 1, 10000001]].map(([stepTicks, jumpBeforeSample, jumpTicks]) => profile(stepTicks, jumpBeforeSample, jumpTicks))]) {
    assert.throws(() => parseDeterministicClockProfile(bad), /deterministic monotonic clock/);
  }
});

test('deterministic sequence jumps exactly before its selected 1-based sample', () => {
  const input = profile();
  const clock = createProofClock({ fixedSystemProfile: fixed, deterministicProfile: input,
    monotonic: () => assert.fail('monotonic provider must not be called'),
    wall: () => assert.fail('wall provider must not be called'), pauseWait: () => assert.fail('pause wait must not be called') });
  input.stepTicks = 10000;
  assert.deepEqual([clock.sample().interrupt, clock.sample().interrupt, clock.sample().interrupt,
    clock.sample().interrupt].map(String), ['7', '14', '121', '128']);
  assert.equal(clock.sample().system, (1234n + 11644473600000n) * 10000n);
  const description = clock.describe();
  assert.equal(description.kind, 'synthetic-clock');
  assert.equal(description.monotonic, 'configured deterministic 100ns ticks');
  assert.equal(description.samples, 5);
  assert.equal(description.minimumObservedPositiveTicks, '7');
  assert.deepEqual(description.deterministicProfile, profile());
  assert.deepEqual(description.deterministicState, { capacity: 16384, lastInterruptTicks: '135' });
  description.deterministicProfile.stepTicks = 1;
  assert.deepEqual(clock.describe().deterministicProfile, profile());
});

test('deterministic profile requires fixed system time and excludes pauses', () => {
  assert.throws(() => createProofClock({ deterministicProfile: profile() }), /fixed system profile/);
  assert.throws(() => createProofClock({ fixedSystemProfile: fixed, deterministicProfile: profile(), pauseProfile: pause,
    pauseWait: () => 'timed-out' }), /cannot use a pause profile/);
});

test('sample cap refuses before state or output mutation', () => {
  const clock = createProofClock({ fixedSystemProfile: fixed, deterministicProfile: profile(1, 16384, 1) });
  for (let i = 0; i < 16384; i++) clock.sample();
  const before = clock.describe();
  assert.throws(() => clock.sample(), /capacity exceeded/);
  assert.deepEqual(clock.describe(), before);
  const output = new ArrayBuffer(16), bytes = new Uint8Array(output);
  bytes.fill(0xA5);
  assert.throws(() => clock.write(output, 0), /capacity exceeded/);
  assert.deepEqual([...bytes], Array(16).fill(0xA5));
  assert.deepEqual(clock.describe(), before);
});
