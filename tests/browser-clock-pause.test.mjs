import test from 'node:test';
import assert from 'node:assert/strict';
import { createProofClock, parseClockPauseProfile } from '../runtime/browser/browser-clock.mjs';

const profile = (beforeSample = 2, milliseconds = 10) => ({ kind: 'one-shot-browser-clock-pause', synthetic: true, beforeSample, milliseconds });

test('pause profile is exact, bounded and canonical', () => {
  assert.equal(parseClockPauseProfile(undefined), undefined);
  const input = profile(), parsed = parseClockPauseProfile(input);
  assert.deepEqual(parsed, input); assert.notEqual(parsed, input);
  for (const bad of [null, [], {}, { ...input, extra: true }, { ...input, synthetic: false }, { ...input, kind: 'other' },
    { ...input, beforeSample: 0 }, { ...input, beforeSample: 16385 }, { ...input, beforeSample: 1.5 },
    { ...input, milliseconds: 0 }, { ...input, milliseconds: 1001 }, { ...input, milliseconds: 1.5 }]) {
    assert.throws(() => parseClockPauseProfile(bad), /synthetic one-shot browser clock pause/);
  }
});

test('default clock description shape is unchanged and opted-in description is copied', () => {
  const clock = createProofClock({ monotonic: () => 10, wall: () => 20 });
  assert.deepEqual(clock.describe(), { kind: 'browser-clock', monotonic: 'performance.now', system: 'Date.now; UTC epoch 1601', samples: 0,
    minimumObservedPositiveTicks: null, limitation: 'Observed sample deltas are not timer resolution or original-console/suspend fidelity.' });
  const p = profile(); let now = 0;
  const opted = createProofClock({ monotonic: () => now, wall: () => 0, pauseProfile: p, pauseWait: () => 'timed-out' });
  p.milliseconds = 900;
  const d = opted.describe();
  assert.deepEqual(d.pauseProfile, profile());
  assert.deepEqual(d.pauseState, { performed: false, beforeSample: 2, milliseconds: 10, beforeTicks: null, afterTicks: null });
  d.pauseProfile.milliseconds = 1; d.pauseState.performed = true;
  assert.deepEqual(opted.describe().pauseProfile, profile());
  assert.equal(opted.describe().pauseState.performed, false);
});

test('unreached pause stays unperformed with null measurements', () => {
  let now = 0;
  const clock = createProofClock({ monotonic: () => now, wall: () => 0, pauseProfile: profile(3), pauseWait: () => assert.fail('pause must not run') });
  clock.sample(); clock.sample();
  assert.deepEqual(clock.describe().pauseState, { performed: false, beforeSample: 3, milliseconds: 10, beforeTicks: null, afterTicks: null });
});

test('pause triggers once before its selected sample, measures same-origin elapsed ticks and samples after wait', () => {
  let now = 100, calls = 0;
  const clock = createProofClock({ monotonic: () => now, wall: () => 0, pauseProfile: profile(2, 10), pauseWait(ms) {
    calls++; assert.equal(ms, 10); now += 12; return 'timed-out';
  } });
  assert.equal(clock.sample().interrupt, 0n);
  assert.equal(clock.sample().interrupt, 120000n);
  assert.equal(clock.sample().interrupt, 120000n);
  assert.equal(calls, 1);
  assert.deepEqual(clock.describe().pauseState, { performed: true, beforeSample: 2, milliseconds: 10, beforeTicks: '0', afterTicks: '120000' });
});

test('pause fails closed on wait result, undershoot or backwards time', () => {
  for (const waitResult of ['ok', undefined]) {
    let now = 0;
    const clock = createProofClock({ monotonic: () => now, pauseProfile: profile(1, 10), pauseWait: () => waitResult });
    assert.throws(() => clock.sample(), /did not time out/);
  }
  let now = 0;
  const undershot = createProofClock({ monotonic: () => now, pauseProfile: profile(1, 10), pauseWait: () => { now = 9.999; return 'timed-out'; } });
  assert.throws(() => undershot.sample(), /undershot/);
  let values = [0, 0, -1];
  const backwards = createProofClock({ monotonic: () => values.shift(), pauseProfile: profile(1, 1), pauseWait: () => 'timed-out' });
  assert.throws(() => backwards.sample(), /measurement/);
  values=[0,0,2,1];
  const afterWait=createProofClock({monotonic:()=>values.shift(),pauseProfile:profile(1,1),pauseWait:()=>'timed-out'});
  assert.throws(()=>afterWait.sample(),/Invalid browser clock sample/);
  assert.throws(()=>createProofClock({pauseProfile:profile(),pauseWait:3}),/wait capability/);
});
