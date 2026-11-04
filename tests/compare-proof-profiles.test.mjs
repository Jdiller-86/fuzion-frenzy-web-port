import assert from 'node:assert/strict';
import test from 'node:test';
import { compareProofProfiles } from '../tools/compare-proof-profiles.mjs';

const digest = character => character.repeat(64);
const service = {
  ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false,
  callingConvention: 'stdcall', argumentBytes: 16, returns: 'void',
};
const startup = (routines, operations) => ({
  totalInitializers: 83, completedInitializers: 83,
  distinctOriginalFunctions: routines, completedKernelOperations: operations,
});
const supplied = () => ({
  window: { base: 0xFD000000, size: 0x01000000 },
  registers: [{ address: 0xFD001804, initial: 0 }, { address: 0xFD600140, initial: 0 }],
  zeroRanges: [{ address: 0xFD001000, bytes: 16 }],
});
const deviceRegisters = supplied => ({
  entries: [], log: [], aliases: [], statusWaits: [], count: 0, events: 0, supplied,
});
const control = () => ({
  kind: 'thread-startup-boundary',
  phase: 'original-initializer-bringup',
  verified: false,
  trace: [],
  stop: { status: 1, target: 0xFE000094, ordinal: 2, service },
  originalStartup: startup(279, 404),
  sourceSha256: digest('a'),
  moduleSha256: digest('b'),
  experiments: ['writeClearing'],
  deviceRegisters: deviceRegisters(supplied()),
  calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }],
});
const experiment = () => ({
  ...control(),
  stop: { status: 2, target: 0x1F79A0, ordinal: 0 },
  originalStartup: startup(280, 405),
  experiments: ['writeClearing', 'avQuery'],
  avQueryWord: 0x00800303,
  calls: [{
    ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264],
    output: {
      rawWord: 0x00800303, address: 2111264, eaxBefore: 0xdeadbeef,
      espBefore: 0x3c10c40, eaxAfter: 0xdeadbeef, espAfter: 0x3c10c54,
    },
  }],
});

test('the controlled avQuery comparison reports the delta and next stop', () => {
  const result = compareProofProfiles(control(), experiment());
  assert.equal(result.ok, true);
  assert.equal(result.addedExperiment, 'avQuery');
  assert.equal(result.avQueryWord, 0x00800303);
  assert.equal(result.delta.routines, 1);
  assert.equal(result.delta.kernelOperations, 1);
  assert.equal(result.nextStop.target, 0x1F79A0);
  assert.equal(result.control.ordinal2[0].status, 1);
  assert.equal(result.experiment.ordinal2[0].status, 0);
  assert.deepEqual(result.deviceInputs, supplied());
});

test('the comparison rejects a different module or executable', () => {
  assert.throws(() => compareProofProfiles(control(), { ...experiment(), moduleSha256: digest('c') }),
    /different modules/);
  assert.throws(() => compareProofProfiles(control(), { ...experiment(), sourceSha256: digest('c') }),
    /different executables/);
  assert.throws(() => compareProofProfiles({ ...control(), moduleSha256: undefined }, experiment()),
    /digest is missing or malformed/);
});

test('the comparison rejects a control whose ordinal-2 denial does not stop there', () => {
  assert.throws(() => compareProofProfiles(
    { ...control(), stop: { status: 10, target: 0xFD000100, ordinal: 1 } }, experiment()),
    /must stop at the unsupported service/);
});

test('the comparison rejects an experiment that does not progress past ordinal 2', () => {
  assert.throws(() => compareProofProfiles(control(), {
    ...experiment(),
    stop: { status: 1, target: 0xFE000094, ordinal: 2, service },
  }), /experiment must progress past the ordinal-2 call/);
});

test('the comparison requires the avQuery experiment to be the only added input', () => {
  const missing = {
    ...experiment(), stop: control().stop, experiments: control().experiments,
    avQueryWord: undefined, calls: control().calls,
  };
  assert.throws(() => compareProofProfiles(control(), missing),
    /must declare the avQuery experiment/);
  assert.throws(() => compareProofProfiles(control(), {
    ...experiment(), experiments: [...experiment().experiments, 'engineWait'],
  }), /differ only by the avQuery experiment/);
});

test('the comparison requires the full supplied device input profile to match', () => {
  const other = edit => ({
    ...experiment(), deviceRegisters: deviceRegisters({ ...supplied(), ...edit }),
  });
  assert.throws(() => compareProofProfiles(control(), other({
    window: { base: 0xFD000000, size: 0x00F00000 },
  })), /different device inputs/);
  assert.throws(() => compareProofProfiles(control(), other({
    registers: [{ address: 0xFD001804, initial: 1 }, { address: 0xFD600140, initial: 0 }],
  })), /different device inputs/);
  assert.throws(() => compareProofProfiles(control(), other({ zeroRanges: [] })),
    /different device inputs/);
  /* Fields this tool does not know about are still part of the profile. */
  assert.throws(() => compareProofProfiles(
    { ...control(), deviceRegisters: deviceRegisters({ ...supplied(), note: 'a' }) },
    { ...experiment(), deviceRegisters: deviceRegisters({ ...supplied(), note: 'b' }) }),
    /different device inputs/);
  /* An absent profile is not evidence that the inputs matched. */
  const empty = { ...control() };
  delete empty.deviceRegisters;
  assert.throws(() => compareProofProfiles(empty, experiment()),
    /must carry its supplied device input profile/);
  assert.throws(() => compareProofProfiles({
    ...control(), deviceRegisters: deviceRegisters(null),
  }, experiment()), /must carry its supplied device input profile/);
});

test('the comparison requires the same ordinal-2 call arguments', () => {
  const base = experiment();
  const shifted = {
    ...base,
    calls: [{
      ...base.calls[0], arguments: [0, 6, 0, 2111272],
      output: { ...base.calls[0].output, address: 2111272 },
    }],
  };
  assert.throws(() => compareProofProfiles(control(), shifted),
    /different ordinal-2 call arguments/);
});

test('a truthful configured denial validates but is not a progress comparison', () => {
  const denied = {
    ...experiment(),
    stop: { status: 1, target: 0xFE000094, ordinal: 2, service },
    calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }],
  };
  assert.throws(() => compareProofProfiles(control(), denied),
    /must complete ordinal 2 with its declared raw word/);
});

test('the comparison rejects repeated experiment names', () => {
  assert.throws(() => compareProofProfiles(
    { ...control(), experiments: ['writeClearing', 'writeClearing'] }, experiment()),
    /must not repeat an experiment/);
  assert.throws(() => compareProofProfiles(control(), {
    ...experiment(), experiments: ['writeClearing', 'avQuery', 'avQuery'],
  }), /must not repeat an experiment/);
});

test('the comparison requires both call lists and a non-decreasing run', () => {
  assert.throws(() => compareProofProfiles({ ...control(), calls: undefined }, experiment()),
    /must carry its kernel call list/);
  assert.throws(() => compareProofProfiles(control(), {
    ...experiment(), originalStartup: startup(278, 405),
  }), /must not report fewer functions or kernel operations/);
});

test('an unsupported control call and a non-DWORD word stay rejected by the report contract', () => {
  assert.throws(() => compareProofProfiles({
    ...control(),
    calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264], output: { rawWord: 1 } }],
  }, experiment()), /must not report success/);
  assert.throws(() => compareProofProfiles(control(), {
    ...experiment(), avQueryWord: 0x100000000,
    calls: [{
      ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264],
      output: { rawWord: 0x100000000 },
    }],
  }), /Malformed synthetic AV query word/);
});
