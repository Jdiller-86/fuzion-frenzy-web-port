import assert from 'node:assert/strict';
import test from 'node:test';
import { parseLocalConsoleSettings } from '../runtime/browser/local-title-proof.mjs';
import { validateProofReport } from '../runtime/browser/proof-report.mjs';
import { createProofClock } from '../runtime/browser/browser-clock.mjs';

const deterministic = (edit = {}) => ({ kind: 'deterministic-monotonic-clock', synthetic: true,
  stepTicks: 7, jumpBeforeSample: 2, jumpTicks: 11, ...edit });
const fixed = { kind: 'fixed-system-clock', synthetic: true, unixMillis: 1234 };
const settings = (edit = {}) => ({ kind: 'local-console-settings', synthetic: true, miscFlags: 0,
  systemClock: fixed, deterministicClock: deterministic(), ...edit });
const encode = value => new TextEncoder().encode(JSON.stringify(value)).buffer;
const baseReport = edit => ({ kind: 'thread-startup-boundary', phase: 'original-initializer-bringup',
  verified: false, stop: { status: 10, target: 0xfd000100, ordinal: 1 }, trace: [],
  originalStartup: { totalInitializers: 1, completedInitializers: 1,
    distinctOriginalFunctions: 1, completedKernelOperations: 0 }, ...edit });
const clockDescription = (samples = 1) => {
  const clock = createProofClock({ fixedSystemProfile: fixed, deterministicProfile: deterministic() });
  for (let i = 0; i < samples; i++) clock.sample();
  return clock.describe();
};
const performanceProfile = (source = 'synthetic-deterministic-100ns') => ({
  kind: 'synthetic-performance-clock', synthetic: true, frequencyHz: 1_000_000,
  source, origin: 'proof-clock-zero'
});

test('deterministic settings require fixed system time and reject pauses', () => {
  const parsed = parseLocalConsoleSettings(encode(settings()));
  assert.deepEqual(parsed.deterministicClock, deterministic());
  assert.notEqual(parsed.deterministicClock, settings().deterministicClock);
  for (const invalid of [
    settings({ systemClock: undefined }),
    settings({ clockPause: { kind: 'one-shot-browser-clock-pause', synthetic: true, beforeSample: 1, milliseconds: 1 } }),
    settings({ deterministicClock: deterministic({ stepTicks: 0 }) }),
  ]) assert.throws(() => parseLocalConsoleSettings(encode(invalid)));
});

test('report requires exact selected and declared deterministic profile plus bounded state', () => {
  const valid = baseReport({ consoleSettings: { systemClock: fixed, deterministicClock: deterministic() },
    clock: clockDescription(3), calls: [] });
  assert.equal(validateProofReport(valid), valid);
  for (const edit of [
    { consoleSettings: { systemClock: fixed } },
    { clock: { ...valid.clock, deterministicProfile: deterministic({ stepTicks: 8 }) } },
    { clock: { ...valid.clock, deterministicProfile: undefined } },
    { clock: { ...valid.clock, deterministicState: undefined } },
    { clock: { ...valid.clock, deterministicState: { ...valid.clock.deterministicState, capacity: 16383 } } },
    { clock: { ...valid.clock, samples: 16385 } },
    { clock: { ...valid.clock, minimumObservedPositiveTicks: '8' } },
  ]) assert.throws(() => validateProofReport({ ...valid, ...edit }));
  for (const ticks of ['22', '022', '-1', '999999999999999999999']) {
    const bad = { ...valid, clock: { ...valid.clock,
      deterministicState: { ...valid.clock.deterministicState, lastInterruptTicks: ticks } } };
    assert.throws(() => validateProofReport(bad));
  }
  const empty = baseReport({ consoleSettings: { systemClock: fixed, deterministicClock: deterministic() },
    clock: clockDescription(0), calls: [] });
  assert.equal(validateProofReport(empty), empty);
  assert.throws(() => validateProofReport({ ...empty, clock: { ...empty.clock,
    deterministicState: { ...empty.clock.deterministicState, lastInterruptTicks: '0' } } }));
});

test('deterministic report kind cannot masquerade as live and live kind cannot claim deterministic state', () => {
  const valid = baseReport({ consoleSettings: { systemClock: fixed, deterministicClock: deterministic() },
    clock: clockDescription(), calls: [] });
  assert.throws(() => validateProofReport({ ...valid, clock: { ...valid.clock,
    kind: 'browser-clock', monotonic: 'performance.now' } }));
  const live = baseReport({ clock: { kind: 'browser-clock', monotonic: 'performance.now', samples: 0,
    system: 'Date.now; UTC epoch 1601', limitation: 'Observed sample deltas are not timer resolution or original-console/suspend fidelity.' } });
  assert.equal(validateProofReport(live), live);
  assert.throws(() => validateProofReport({ ...live, clock: { ...live.clock,
    deterministicState: { capacity: 16384, lastInterruptTicks: null } } }));
});

test('deterministic performance source is explicit and samples must belong to its sequence', () => {
  const profile = performanceProfile();
  const clock = clockDescription();
  const state = { ...profile, capacity: 64, status: 0, lastSite: 0x100, frequencyQueries: 0,
    admittedSites: [0x100], samples: [{ site: 0x100, interruptTicks: '7', counter: '0' }] };
  const accepted = baseReport({ consoleSettings: { systemClock: fixed, deterministicClock: deterministic(),
    performanceClock: profile }, clock, performanceClockState: state, calls: [] });
  assert.equal(validateProofReport(accepted), accepted);
  assert.throws(() => parseLocalConsoleSettings(encode(settings({
    performanceClock: performanceProfile('browser-monotonic-100ns') }))));
  assert.throws(() => parseLocalConsoleSettings(encode({ ...settings(),
    deterministicClock: undefined, performanceClock: performanceProfile() })));
  const outsideSequence = { ...accepted, performanceClockState: { ...state,
    samples: [{ ...state.samples[0], interruptTicks: '8' }] } };
  assert.throws(() => validateProofReport(outsideSequence));
  const wrongSource = { ...accepted, consoleSettings: { ...accepted.consoleSettings,
    performanceClock: performanceProfile('browser-monotonic-100ns') } };
  assert.throws(() => validateProofReport(wrongSource));
});

test('counter raw and polled QPC observations must both be generated deterministic ticks', () => {
  const performance = performanceProfile();
  const counter = { kind: 'synthetic-performance-counter', synthetic: true, source: 'paired-performance-clock' };
  const report = baseReport({ consoleSettings: { systemClock: fixed, deterministicClock: deterministic(),
    performanceClock: performance, performanceCounter: counter }, clock: clockDescription(3),
    performanceClockState: { ...performance, capacity: 64, status: 0, lastSite: 0x100,
      frequencyQueries: 0, admittedSites: [0x100],
      samples: [{ site: 0x100, interruptTicks: '25', counter: '2' }] },
    performanceCounterState: { ...counter, frequencyHz: 1_000_000, count: 1, capacity: 64,
      status: 0, lastTicks: '25', lastCounter: '2' },
    calls: [{ ordinal: 126, status: 0, arguments: [], output: { backend: counter.kind,
      frequencyHz: 1_000_000, interruptTicks: '25', polledInterruptTicks: '7', counter: '2',
      low: 2, high: 0, eaxBefore: 8, edxBefore: 9, espBefore: 0x3c10000,
      eaxAfter: 2, edxAfter: 0, espAfter: 0x3c10004 } }] });
  assert.equal(validateProofReport(report), report);

  const offSequenceRaw = structuredClone(report);
  offSequenceRaw.performanceCounterState.lastTicks = '26';
  offSequenceRaw.calls[0].output.interruptTicks = '26';
  assert.throws(() => validateProofReport(offSequenceRaw));

  const offSequencePoll = structuredClone(report);
  offSequencePoll.calls[0].output.polledInterruptTicks = '8';
  assert.throws(() => validateProofReport(offSequencePoll));
});
