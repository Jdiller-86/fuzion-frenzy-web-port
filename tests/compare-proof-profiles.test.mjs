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
  originalStartup: startup(279, 0),
  sourceSha256: digest('a'),
  moduleSha256: digest('b'),
  experiments: ['writeClearing'],
  consoleSettings: { kind: 'local-console-settings', synthetic: true, miscFlags: 0, parentalControlGames: 0 },
  volumeProfile: {
    kind: 'local-volume-profile', synthetic: true, device: 'harddisk0-partition1',
    totalAllocationUnits: '65536', availableAllocationUnits: '65536',
    sectorsPerAllocationUnit: 32, bytesPerSector: 512,
  },
  directoryProfile: { synthetic: true, state: 'fresh', persistent: false },
  fileStorageProfile: { synthetic: true, state: 'fresh', persistent: false, maxFiles: 16, maxFileBytes: 65536 },
  diskProfile: {
    kind: 'local-disk-profile', synthetic: true, device: 'harddisk0', state: 'fresh',
    bytes: '8589934592', sectorBytes: 512, cachePartitions: 3, persistent: false, maxBackingPages: 128,
  },
  pfifoProfile: { kind: 'local-pfifo-profile', synthetic: true, base: 0x03B80000, end: 0x03BF0000, put: 0x03B80000 },
  memoryAccounting: {
    profile: {
      kind: 'local-memory-profile', synthetic: true, physicalPages: 16384,
      baselinePoolPages: 0, fixedArena: 'committed-runtime-pool',
    },
  },
  deviceRegisters: deviceRegisters(supplied()),
  calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }],
});
const experiment = () => ({
  ...control(),
  stop: { status: 2, target: 0x1F79A0, ordinal: 0 },
  originalStartup: startup(280, 1),
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
  /* An experiment that stops on the unsupported ordinal-2 service without a
   * denial record is already invalid at the report layer. */
  assert.throws(() => compareProofProfiles(control(), {
    ...experiment(),
    stop: { status: 1, target: 0xFE000094, ordinal: 2, service },
  }), /requires the ordinal-2 denial record/);
  /* A valid report can still stop on ordinal 2: a completed call plus a denial
   * pairs with the stop, but it is not a progress comparison. The control has
   * two matching denial arguments so the paired-argument check passes first. */
  const argumentsPair = [0, 6, 0, 2111264];
  const pairedControl = {
    ...control(),
    calls: [
      { ordinal: 2, status: 1, arguments: argumentsPair },
      { ordinal: 2, status: 1, arguments: argumentsPair },
    ],
  };
  const pairedExperiment = {
    ...experiment(),
    stop: { status: 1, target: 0xFE000094, ordinal: 2, service },
    calls: [
      experiment().calls[0],
      { ordinal: 2, status: 1, arguments: argumentsPair },
    ],
  };
  assert.throws(() => compareProofProfiles(pairedControl, pairedExperiment),
    /experiment must progress past the ordinal-2 call/);
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

test('the comparison requires the other declared inputs to match', () => {
  const edits = [
    { consoleSettings: { ...control().consoleSettings, miscFlags: 1 } },
    { volumeProfile: { ...control().volumeProfile, availableAllocationUnits: '65535' } },
    { directoryProfile: { ...control().directoryProfile, state: 'reused' } },
    { fileStorageProfile: { ...control().fileStorageProfile, maxFiles: 8 } },
    { diskProfile: { ...control().diskProfile, state: 'reused' } },
    { pfifoProfile: { ...control().pfifoProfile, put: 0x03B80004 } },
    { memoryAccounting: { profile: { ...control().memoryAccounting.profile, baselinePoolPages: 1 } } },
  ];
  for (const edit of edits) {
    assert.throws(() => compareProofProfiles(control(), { ...experiment(), ...edit }),
      /declared different console\/volume\/directory\/file-storage\/disk\/PFIFO\/memory inputs/);
  }
});

test('the comparison requires every declared input profile to be present', () => {
  for (const field of ['consoleSettings', 'volumeProfile', 'directoryProfile',
    'fileStorageProfile', 'diskProfile', 'pfifoProfile', 'memoryAccounting']) {
    const missing = { ...experiment() };
    delete missing[field];
    assert.throws(() => compareProofProfiles(control(), missing),
      /must carry its declared/);
  }
  const noControl = { ...control() }, noExperiment = { ...experiment() };
  delete noControl.pfifoProfile; delete noExperiment.pfifoProfile;
  assert.throws(() => compareProofProfiles(noControl, noExperiment),
    /must carry its declared/);
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

test('the comparison preserves every routine the control reached', () => {
  const reached = count => Array.from({ length: count }, (_, index) => index + 1);
  const withLists = {
    control: { ...control(), originalFunctionsReached: reached(279) },
    experiment: { ...experiment(), originalFunctionsReached: [...reached(279), 5000] },
  };
  assert.equal(compareProofProfiles(withLists.control, withLists.experiment).ok, true);
  /* The experiment keeps 280 unique routines but drops one control routine and
   * adds two others, so the count alone would still pass. */
  const dropped = {
    ...experiment(),
    originalFunctionsReached: [...reached(280).slice(1), 5000, 5001].slice(1),
  };
  assert.throws(() => compareProofProfiles(withLists.control, dropped),
    /dropped routines the control reached/);
  /* One list cannot stand in for both reports' evidence. */
  assert.throws(() => compareProofProfiles(withLists.control, experiment()),
    /Both profiles must carry their reached-routine list/);
  assert.throws(() => compareProofProfiles(control(), withLists.experiment),
    /Both profiles must carry their reached-routine list/);
});

test('the comparison requires the operation count to match successful calls', () => {
  assert.throws(() => compareProofProfiles(control(), {
    ...experiment(), originalStartup: startup(280, 2),
  }), /completed kernel-operation count/);
  assert.throws(() => compareProofProfiles({
    ...control(), originalStartup: startup(279, 1),
  }, experiment()), /completed kernel-operation count/);
});

test('a truthful configured denial validates but is not a progress comparison', () => {
  const denied = {
    ...experiment(),
    stop: { status: 1, target: 0xFE000094, ordinal: 2, service },
    calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }],
    originalStartup: startup(280, 0),
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

test('the comparison requires the control denial record and a non-decreasing run', () => {
  assert.throws(() => compareProofProfiles({ ...control(), calls: undefined }, experiment()),
    /requires the ordinal-2 denial record/);
  assert.throws(() => compareProofProfiles(control(), {
    ...experiment(), originalStartup: startup(278, 1),
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
