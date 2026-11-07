import assert from 'node:assert/strict';
import test from 'node:test';
import { validateProofReport, TRACE_LIMIT, DEVICE_EVENT_LIMIT } from '../runtime/browser/proof-report.mjs';
import { createProofClock } from '../runtime/browser/browser-clock.mjs';

const report = (edit = {}) => ({
  kind: 'thread-startup-boundary',
  phase: 'original-initializer-bringup',
  verified: false,
  stop: { status: 10, target: 0xFD000100, ordinal: 1 },
  trace: [{ kind: 'enter', address: 0x1FDC30, stack: 0x3C10C40 }],
  originalStartup: {
    totalInitializers: 83, completedInitializers: 83, distinctOriginalFunctions: 276,
    completedKernelOperations: 403,
  },
  ...edit,
});

/* The completed ordinal-2 call must expose the adapter's VOID/EAX/stack effect. */
const avOutput = (rawWord, edit = {}) => ({
  rawWord, address: 2111264, eaxBefore: 0xdeadbeef, espBefore: 0x3c10c40,
  eaxAfter: 0xdeadbeef, espAfter: 0x3c10c54, ...edit,
});

/* A status-1 ordinal-2 denial is the unsupported-service stop itself. */
const unsupportedOrdinal2 = () => ({
  status: 1, target: 0xFE000094, ordinal: 2,
  service: {
    ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false,
    callingConvention: 'stdcall', argumentBytes: 16, returns: 'void',
  },
});

test('a well-formed startup report validates and is returned unchanged', () => {
  const input = report();
  assert.equal(validateProofReport(input), input);
  const withExperiments = report({ experiments: ['writeClearing', 'pcrtcVblanks:8'] });
  assert.equal(validateProofReport(withExperiments), withExperiments);
  const device = report({
    deviceRegisters: { count: 2, events: 1, entries: [{}, {}], log: [{}] },
  });
  assert.equal(validateProofReport(device), device);
});

test('timestamp-counter boundary remains a stop-only one-attempt declaration', () => {
  const site = 0x00123456;
  const valid = report({ stop: { status: 9, target: site, ordinal: 0 },
    timestampBoundary: { kind: 'unmodeled-timestamp-counter', site, attempts: 1 } });
  assert.equal(validateProofReport(valid), valid);
  for (const boundary of [
    { kind: 'other', site, attempts: 1 }, { kind: 'unmodeled-timestamp-counter', site, attempts: 1, value: 123 },
    { kind: 'unmodeled-timestamp-counter', site, attempts: 0 }, { kind: 'unmodeled-timestamp-counter', site, attempts: 2 },
  ]) assert.throws(() => validateProofReport({ ...valid, timestampBoundary: boundary }), /Timestamp-counter boundary/);
  for (const stop of [{ status: 9, target: site + 4, ordinal: 0 }, { status: 9, target: site, ordinal: 1 },
    { status: 10, target: site, ordinal: 0 }])
    assert.throws(() => validateProofReport({ ...valid, stop }), /Timestamp-counter boundary/);
});

test('kernel capture reports bind calls, selected profile and captured suffix evidence', () => {
  const suffix = {
    kind: 'kernel-capture-window', version: 1, requestedStart: 512, capturedStart: 512,
    capturedCount: 1, totalCount: 513, completedOperations: 512, prefixCompletedOperations: 511,
    complete: false, sha256: 'a'.repeat(64), prefixSha256: 'b'.repeat(64),
  };
  const valid = report({
    consoleSettings: { kernelCaptureStart: 512 }, kernelCapture: suffix, kernelCapacity: 512,
    calls: [{ ordinal: 277, status: 0, arguments: [123] }],
    originalStartup: { totalInitializers: 83, completedInitializers: 83,
      distinctOriginalFunctions: 276, completedKernelOperations: 512 },
  });
  assert.equal(validateProofReport(valid), valid);
  assert.throws(() => validateProofReport({ ...valid, calls: undefined }), /requires calls/);
  assert.throws(() => validateProofReport({ ...valid, consoleSettings: { kernelCaptureStart: 0 } }), /declared profile/);
  for (const kernelCaptureStart of [0, 512])
    assert.throws(() => validateProofReport(report({ consoleSettings: { kernelCaptureStart } })),
      /Selected kernel capture profile requires capture metadata/);
  /* A suffix intentionally omits ordinal 2, so its captured rows are checked
   * while historical pair claims cannot be inferred from absent prefix rows. */
  const avNull = { ...valid, experiments: ['avNullResult'], avNullResultState: {
    calls: 1, handle: 0x03920000, option: 9, param: 0, result: 0, decision: 1,
    writeBefore: 0, writeAfter: 0, checksumBefore: 0, checksumAfter: 0,
    eaxBefore: 0, espBefore: 100, eaxAfter: 0, espAfter: 120,
  }, calls: [{ ordinal: 277, status: 0, arguments: [123] }] };
  assert.equal(validateProofReport(avNull), avNull);
  const badCapturedRow = { ...avNull, calls: [{ ordinal: 277, status: 0 }] };
  assert.throws(() => validateProofReport(badCapturedRow), /Malformed kernel call record/);
});

test('console OS settings profile requires coherent captured ordinal-24 evidence', () => {
  const osSettings = { kind: 'synthetic-console-os-settings', synthetic: true,
    layout: 'cxbx-user-settings-96-v1', timeZone: 'utc-no-dst', zeroOtherFields: true,
    checksum: 0x1234, miscFlags: 7, parentalControlGames: 9 };
  const call = (output = { backend: 'synthetic-console-os-settings', type: 3,
    returnedBytes: 96, capacity: 256, providerSnapshotMatches: true,
    eaxBefore: 0xdeadbeef, eaxAfter: 0, espBefore: 100, espAfter: 124 }, args = [255, 0x1000, 0x2000, 256, 0]) =>
    ({ ordinal: 24, status: 0, arguments: args, output });
  const accepted = report({
    consoleSettings: { miscFlags: 7, parentalControlGames: 9, osSettings },
    calls: [call()],
  });
  assert.equal(validateProofReport(accepted), accepted);
  const adjacent = { ...accepted, calls: [call(undefined, [255, 0x2100, 0x2000, 256, 0])] };
  assert.equal(validateProofReport(adjacent), adjacent);
  assert.throws(() => validateProofReport(report({ consoleSettings: { miscFlags: 7, parentalControlGames: 9, osSettings } })), /captured calls/);
  assert.throws(() => validateProofReport(report({ consoleSettings: { miscFlags: 7, parentalControlGames: 10, osSettings }, calls: [call()] })), /consistent declared DWORDs/);
  for (const output of [
    { ...call().output, type: 2 }, { ...call().output, returnedBytes: 95 },
    { ...call().output, capacity: 255 }, { ...call().output, providerSnapshotMatches: false },
    { ...call().output, eaxAfter: 1 }, { ...call().output, espAfter: 123 },
    { ...call().output, rawPayload: 'unbounded' },
  ]) assert.throws(() => validateProofReport(report({
    consoleSettings: { miscFlags: 7, parentalControlGames: 9, osSettings }, calls: [call(output)],
  })), /Accepted console OS settings call/);
  for (const args of [[255, 0, 0x2000, 256, 0], [255, 0x1000, 0, 256, 0],
    [255, 0x1000, 0x2000, 255, 0], [255, 0x1000, 0x2000, 256, 1],
    [255, 0x2000, 0x1f10, 256, 0]])
    assert.throws(() => validateProofReport(report({
      consoleSettings: { miscFlags: 7, parentalControlGames: 9, osSettings }, calls: [call(undefined, args)],
    })), /Accepted console OS settings call/);
  /* Legacy profiles without this opt-in still retain the old unsupported-service stop. */
  const refused = report({ stop: { status: 9, target: 255, ordinal: 24 },
    calls: [{ ordinal: 24, status: 0xc00000bb, arguments: [255, 0x1000, 0x2000, 256, 0] }] });
  assert.equal(validateProofReport(refused), refused);
});

test('fixed system clock reports bind selected profile, declaration and captured query', () => {
  const fixed = { kind: 'fixed-system-clock', synthetic: true, unixMillis: 1234 };
  const clock = createProofClock({ monotonic: () => 0, fixedSystemProfile: fixed }).describe();
  const make = (settings = fixed, description = clock, calls) => report({
    consoleSettings: { systemClock: settings }, clock: description, calls,
  });
  const ticks = ((1234n + 11644473600000n) * 10000n).toString();
  const accepted = make(fixed, clock, [{ ordinal: 128, status: 0, arguments: [0x1000], output: { systemTicks: ticks } }]);
  assert.equal(validateProofReport(accepted), accepted);
  assert.throws(() => validateProofReport(make(fixed, clock)));
  for (const bad of [null, { ...clock, fixedSystemProfile: undefined },
    { ...clock, fixedSystemProfile: { ...fixed, extra: true } },
    { ...clock, fixedSystemProfile: { ...fixed, synthetic: false } },
    { ...clock, kind: 'other' }, { ...clock, system: 'Date.now; UTC epoch 1601' },
    { ...clock, monotonic: 'Date.now' }])
    assert.throws(() => validateProofReport(make(fixed, bad, accepted.calls)));
  assert.throws(() => validateProofReport(make({ ...fixed, unixMillis: 1235 }, clock, accepted.calls)), /match the explicitly selected/);
  assert.throws(() => validateProofReport(make(fixed, clock, [{ ordinal: 128, status: 0, arguments: [0x1000], output: { systemTicks: '1' } }])), /match the selected fixed clock/);
  assert.throws(() => validateProofReport(make(fixed, clock, [{ ...accepted.calls[0], arguments: [] }])), /match the selected fixed clock/);
  assert.throws(() => validateProofReport(report({clock:{...clock,fixedSystemProfile:undefined},calls:[]})), /match the explicitly selected/);
  const liveLegacy = report();
  assert.equal(validateProofReport(liveLegacy), liveLegacy);
});

test('bounded debug service accepts print metadata and binds refusal to the exact stop', () => {
  const printed = report({ debugServiceState: { completed: 1, service: 1, bytes: 24, status: 0 } });
  assert.equal(validateProofReport(printed), printed);
  const refused = report({
    stop: { status: 9, ordinal: 0, target: 0xc00000bb },
    debugServiceState: { completed: 0, service: 2, bytes: 0, status: 0xc00000bb },
  });
  assert.equal(validateProofReport(refused), refused);
  assert.throws(() => validateProofReport({ ...refused, stop: printed.stop }), /guarded result/);
  assert.throws(() => validateProofReport({
    ...printed, debugServiceState: { ...printed.debugServiceState, bytes: 1025 },
  }), /Malformed bounded debug-service state/);
  assert.throws(() => validateProofReport({
    ...printed, debugServiceState: { ...printed.debugServiceState, payload: 'synthetic' },
  }), /Malformed bounded debug-service state/);
});

test('PFIFO reset state requires one initialization, zero consumer updates and LOW_MARK', () => {
  const valid = report({
    experiments: ['pfifoReset'],
    pfifoResetState: { scope: 'seam', cache1: 0x10, runout: 0x10, dmaPush: 0, consumerUpdates: 0, resets: 1 },
  });
  assert.equal(validateProofReport(valid), valid);
  assert.throws(() => validateProofReport(report({
    pfifoResetState: { scope: 'seam', cache1: 0x10, runout: 0x10, dmaPush: 0, consumerUpdates: 0, resets: 1 },
  })), /requires the pfifoReset experiment/);
  assert.throws(() => validateProofReport(report({ experiments: ['pfifoReset'] })),
    /requires PFIFO reset state/);
  assert.throws(() => validateProofReport(report({
    experiments: ['pfifoReset'],
    pfifoResetState: { scope: 'seam', cache1: 0x10, runout: 0x10, dmaPush: 0, consumerUpdates: 1, resets: 1 },
  })), /consumer update/);
  assert.throws(() => validateProofReport(report({
    experiments: ['pfifoReset'],
    pfifoResetState: { scope: 'seam', cache1: 0x10, runout: 0x10, dmaPush: 0, consumerUpdates: 0, resets: 2 },
  })), /exactly once/);
  assert.throws(() => validateProofReport(report({
    experiments: ['pfifoReset'],
    pfifoResetState: { scope: 'seam', cache1: 0, runout: 0x10, dmaPush: 0, consumerUpdates: 0, resets: 1 },
  })), /LOW_MARK/);
  assert.throws(() => validateProofReport(report({
    experiments: ['pfifoReset'], pfifoResetState: { scope: 'seam', cache1: 0x10 },
  })), /Malformed/);
  assert.throws(() => validateProofReport(report({
    experiments: ['pfifoReset'],
    pfifoResetState: { scope: 'device', cache1: 0x10, runout: 0x10, dmaPush: 0, consumerUpdates: 0, resets: 1 },
  })), /Malformed/);
});

test('display-mode state requires the experiment and the pinned declaration', () => {
  const state = { accepted: 1, addr: 0x3C10CFC, step: 0, mode: 0x60030307, format: 0,
    pitch: 0xA00, fb: 0x03920000, window: 0x83920000, decision: 0 };
  const valid = report({ experiments: ['displayMode'], displayModeState: state });
  assert.equal(validateProofReport(valid), valid);
  assert.throws(() => validateProofReport(report({ displayModeState: state })),
    /requires the displayMode experiment/);
  assert.throws(() => validateProofReport(report({ experiments: ['displayMode'] })),
    /requires display-mode state/);
  assert.throws(() => validateProofReport(report({
    experiments: ['displayMode'], displayModeState: { ...state, fb: 0x04000000 },
  })), /pinned declaration/);
});

test('PFIFO reset state allows consumer updates when the consumer is declared', () => {
  const valid = report({
    experiments: ['pfifoReset', 'pfifoConsumer'],
    pfifoResetState: { scope: 'seam', cache1: 0x10, runout: 0x10, dmaPush: 0, consumerUpdates: 2, resets: 1 },
    pfifoConsumerState: { scope: 'prototype', classification: 0, get: 0x1004, put: 0x1008,
      base: 0x1000, end: 0x2000, wordsConsumed: 1, consumerUpdates: 2, lastHeader: 0, steps: 1,
      words: 1, budgetHit: 0, vmChecks: 0 },
  });
  assert.equal(validateProofReport(valid), valid);
  assert.throws(() => validateProofReport(report({
    experiments: ['pfifoReset'],
    pfifoResetState: { scope: 'seam', cache1: 0x10, runout: 0x10, dmaPush: 0, consumerUpdates: 2, resets: 1 },
  })), /consumer update/);
});
test('PFIFO consumer state requires the experiment and consistent accounting', () => {
  const advance = report({
    experiments: ['pfifoConsumer'],
    pfifoConsumerState: { scope: 'prototype', classification: 0, get: 0x1004, put: 0x1008,
      base: 0x1000, end: 0x2000, wordsConsumed: 1, consumerUpdates: 2, lastHeader: 0, steps: 1, words: 1, budgetHit: 0, vmChecks: 0 },
  });
  assert.equal(validateProofReport(advance), advance);
  const refusal = report({
    experiments: ['pfifoConsumer'],
    pfifoConsumerState: { scope: 'prototype', classification: 3, get: 0, put: 0x1008,
      base: 0x1000, end: 0x2000, wordsConsumed: 0, consumerUpdates: 0, lastHeader: 0, steps: 1, words: 0, budgetHit: 0, vmChecks: 0 },
  });
  assert.equal(validateProofReport(refusal), refusal);
  const budget = report({
    experiments: ['pfifoConsumer'],
    pfifoConsumerState: { scope: 'prototype', classification: 8, get: 0x1004, put: 0x1008,
      base: 0x1000, end: 0x2000, wordsConsumed: 1, consumerUpdates: 2, lastHeader: 0, steps: 1,
      words: 1, budgetHit: 1, vmChecks: 500000 },
  });
  assert.equal(validateProofReport(budget), budget);
  assert.throws(() => validateProofReport(report({
    experiments: ['pfifoConsumer'],
    pfifoConsumerState: { scope: 'prototype', classification: 8, get: 0x1004, put: 0x1008,
      base: 0x1000, end: 0x2000, wordsConsumed: 1, consumerUpdates: 2, lastHeader: 0, steps: 1,
      words: 1, budgetHit: 0, vmChecks: 500000 },
  })), /budget hit/);
  assert.throws(() => validateProofReport(report({
    pfifoConsumerState: { scope: 'prototype', classification: 0, get: 0, put: 0, base: 0, end: 0,
      wordsConsumed: 0, consumerUpdates: 0, lastHeader: 0, steps: 0, words: 0, budgetHit: 0, vmChecks: 0 },
  })), /requires the pfifoConsumer experiment/);
  assert.throws(() => validateProofReport(report({ experiments: ['pfifoConsumer'] })),
    /requires PFIFO consumer state/);
  assert.throws(() => validateProofReport(report({
    experiments: ['pfifoConsumer'],
    pfifoConsumerState: { scope: 'prototype', classification: 0, get: 0, put: 0, base: 0, end: 0,
      wordsConsumed: 0, consumerUpdates: 2, lastHeader: 0, steps: 1, words: 0, budgetHit: 0, vmChecks: 0 },
  })), /must consume a word/);
  assert.throws(() => validateProofReport(report({
    experiments: ['pfifoConsumer'],
    pfifoConsumerState: { scope: 'prototype', classification: 3, get: 0, put: 0, base: 0, end: 0,
      wordsConsumed: 1, consumerUpdates: 0, lastHeader: 0, steps: 1, words: 1, budgetHit: 0, vmChecks: 0 },
  })), /must not consume a word/);
});

test('an unsupported ordinal-2 call carries four arguments and no output', () => {
  const valid = report({
    stop: unsupportedOrdinal2(),
    calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }],
  });
  assert.equal(validateProofReport(valid), valid);
  assert.throws(() => validateProofReport(report({
    calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0] }],
  })), /four observed arguments/);
  assert.throws(() => validateProofReport(report({
    calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264] }],
  })), /must not report success/);
  assert.throws(() => validateProofReport(report({
    calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264], output: { packWord: 0 } }],
  })), /must not carry an output/);
});

test('an unconfigured ordinal-2 call must report completed or denied status', () => {
  for (const status of [2, 9, 10]) {
    assert.throws(() => validateProofReport(report({
      calls: [{ ordinal: 2, status, arguments: [0, 6, 0, 2111264] }],
    })), /must report the unsupported status/);
    /* A non-denial record cannot satisfy the unsupported ordinal-2 stop. */
    assert.throws(() => validateProofReport(report({
      stop: unsupportedOrdinal2(),
      calls: [{ ordinal: 2, status, arguments: [0, 6, 0, 2111264] }],
    })), /must report the unsupported status/);
  }
  /* The actual denial still pairs with the stop. */
  const denied = report({
    stop: unsupportedOrdinal2(),
    calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }],
  });
  assert.equal(validateProofReport(denied), denied);
});

test('an ordinal-2 denial must pair with the unsupported-service stop', () => {
  const call = { ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] };
  /* Any report claiming the unsupported ordinal-2 stop must record the denial,
   * including the unconfigured control path. */
  assert.throws(() => validateProofReport(report({ stop: unsupportedOrdinal2() })),
    /requires the ordinal-2 denial record/);
  /* A completed call is not a denial and cannot satisfy the stop pairing. */
  assert.throws(() => validateProofReport(report({
    stop: unsupportedOrdinal2(),
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264], output: avOutput(0x00800303) }],
  })), /requires the ordinal-2 denial record/);
  assert.throws(() => validateProofReport(report({ calls: [call] })),
    /must stop at the unsupported service/);
  assert.throws(() => validateProofReport(report({
    stop: { status: 1, target: 0xFE000094, ordinal: 3, service: { ordinal: 3, name: null, modeled: false } },
    calls: [call],
  })), /must stop at the unsupported service/);
  assert.throws(() => validateProofReport(report({
    experiments: ['avQuery'], avQueryWord: 0x00800303, calls: [call],
  })), /must stop at the unsupported service/);
  /* A completed call is not a denial and does not require the unsupported stop. */
  const completed = report({
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264], output: avOutput(0x00800303) }],
  });
  assert.equal(validateProofReport(completed), completed);
});

test('a denial after a completed avQuery call still requires the unsupported stop', () => {
  const success = { ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264], output: avOutput(0x00800303) };
  const denial = { ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] };
  assert.throws(() => validateProofReport(report({
    stop: { status: 2, target: 0x1F79A0, ordinal: 0 },
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [success, denial],
  })), /must stop at the unsupported service/);
  const paired = report({
    stop: unsupportedOrdinal2(),
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [success, denial],
  });
  assert.equal(validateProofReport(paired), paired);
});

test('an explicit synthetic avQuery word completes ordinal 2 with its raw word', () => {
  const valid = report({
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264], output: avOutput(0x00800303) }],
  });
  assert.equal(validateProofReport(valid), valid);
  assert.throws(() => validateProofReport(report({
    avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264], output: avOutput(0x00800303) }],
  })), /requires the avQuery experiment/);
  assert.throws(() => validateProofReport(report({
    experiments: ['avQuery'],
    calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264], output: avOutput(0x00800303) }],
  })), /requires an explicit raw word/);
  /* A configured profile may be a truthful prequery: ordinal 2 was never
   * reached, so the report has no ordinal-2 call. */
  const prequery = report({ experiments: ['avQuery'], avQueryWord: 0x00800303, calls: [] });
  assert.equal(validateProofReport(prequery), prequery);
  const prequeryWithoutCalls = report({ experiments: ['avQuery'], avQueryWord: 0x00800303 });
  assert.equal(validateProofReport(prequeryWithoutCalls), prequeryWithoutCalls);
  /* A prequery may not claim the unsupported ordinal-2 stop without the denial
   * record. */
  assert.throws(() => validateProofReport(report({
    stop: unsupportedOrdinal2(),
    experiments: ['avQuery'], avQueryWord: 0x00800303,
  })), /requires the ordinal-2 denial record/);
  /* A configured profile may truthfully record a denial; the report must
   * preserve that instead of presenting it as success. */
  const denied = report({
    stop: unsupportedOrdinal2(),
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }],
  });
  assert.equal(validateProofReport(denied), denied);
  assert.throws(() => validateProofReport(report({
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264], output: { rawWord: 1 } }],
  })), /must not carry an output word/);
  const configuredDenial = report({
    stop: unsupportedOrdinal2(),
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 4, arguments: [0, 6, 0, 2111264] }],
  });
  assert.equal(validateProofReport(configuredDenial), configuredDenial);
  assert.throws(() => validateProofReport(report({
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264] }],
  })), /must carry its raw word, output address/);
  assert.throws(() => validateProofReport(report({
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264], output: avOutput(0x00800304) }],
  })), /must match the declared raw word/);
});

test('a successful avQuery call must use the observed query shape and output address', () => {
  const call = (arguments_, output = avOutput(0x00800303)) => report({
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 0, arguments: arguments_, output }],
  });
  for (const arguments_ of [[1, 6, 0, 2111264], [0, 5, 0, 2111264], [0, 6, 1, 2111264], [0, 6, 0, 0]])
    assert.throws(() => validateProofReport(call(arguments_)),
      /observed \(0, 6, 0, Result\) query shape/);
  assert.throws(() => validateProofReport(call([0, 6, 0, 2111264],
    avOutput(0x00800303, { address: 2111272 }))), /output address must match/);
  assert.throws(() => validateProofReport(call([0, 6, 0, 2111264],
    avOutput(0x00800303, { address: undefined }))), /raw word, output address/);
});

test('the completed avQuery call must expose an unchanged EAX and a 20-byte pop', () => {
  const call = (output) => report({
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264], output }],
  });
  assert.throws(() => validateProofReport(call(avOutput(0x00800303, { eaxAfter: 1 }))),
    /leave EAX unchanged/);
  assert.throws(() => validateProofReport(call(avOutput(0x00800303, { espAfter: 0x3c10c55 }))),
    /pop the 20-byte stdcall frame/);
  assert.throws(() => validateProofReport(call(avOutput(0x00800303,
    { espBefore: 0xfffffff0, espAfter: 0x00000004 }))), /wrapping stack frame/);
  assert.throws(() => validateProofReport(call(avOutput(0x00800303, { eaxBefore: undefined }))),
    /must carry its raw word, output address/);
  assert.equal(validateProofReport(call(avOutput(0x00800303))).calls[0].output.eaxBefore, 0xdeadbeef);
});

test('a configured avQuery report may be a prequery, but the word must be a DWORD', () => {
  /* The declaration is an input claim; a prequery run that never reaches
   * ordinal 2 is truthful and must still validate. */
  const prequery = report({ experiments: ['avQuery'], avQueryWord: 0x00800303 });
  assert.equal(validateProofReport(prequery), prequery);
  const otherCalls = report({
    experiments: ['avQuery'], avQueryWord: 0x00800303,
    calls: [{ ordinal: 187, status: 0, arguments: [1] }],
  });
  assert.equal(validateProofReport(otherCalls), otherCalls);
  /* A report without the experiment may still omit the call list. */
  const plain = report();
  assert.equal(validateProofReport(plain), plain);
  for (const avQueryWord of [-1, 0x100000000, 1.5, '1']) {
    assert.throws(() => validateProofReport(report({
      experiments: ['avQuery'], avQueryWord,
      calls: [{ ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264], output: avOutput(avQueryWord) }],
    })), /Malformed synthetic AV query word/);
  }
});

test('an unsupported-service stop must carry a non-modeled service descriptor', () => {
  const valid = report({
    stop: {
      status: 1, target: 0xFE000094, ordinal: 2,
      service: {
        ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false,
        callingConvention: 'stdcall', argumentBytes: 16, returns: 'void',
      },
    },
    calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }],
  });
  assert.equal(validateProofReport(valid), valid);
  assert.throws(() => validateProofReport(report({
    stop: { status: 1, target: 0xFE000094, ordinal: 2 },
  })), /must name the service/);
  assert.throws(() => validateProofReport(report({
    stop: {
      status: 1, target: 0xFE000094, ordinal: 2,
      service: { ordinal: 3, name: 'AvSendTVEncoderOption', modeled: false },
    },
  })), /must name the service/);
  assert.throws(() => validateProofReport(report({
    stop: {
      status: 1, target: 0xFE000094, ordinal: 2,
      service: { ordinal: 2, name: 'AvSendTVEncoderOption', modeled: true },
    },
  })), /must name the service/);
  assert.throws(() => validateProofReport(report({
    stop: {
      status: 1, target: 0xFE000094, ordinal: 2,
      service: {
        ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false,
        callingConvention: 'cdecl', argumentBytes: 16, returns: 'void',
      },
    },
  })), /evidenced stdcall/);
  assert.throws(() => validateProofReport(report({
    stop: {
      status: 1, target: 0xFE000094, ordinal: 2,
      service: {
        ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false,
        callingConvention: 'stdcall', argumentBytes: 16, returns: 'void',
        resultBits: { pack: 0xff, standard: 0xff00, refresh: 0xc00000 },
        option: { code: 6, name: 'AV_OPTION_QUERY_AVPACK' },
      },
    },
  })), /Observed option\/result facts/);
  assert.throws(() => validateProofReport(report({
    stop: {
      status: 1, target: 0xFE000094, ordinal: 2,
      service: {
        ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false,
        callingConvention: 'stdcall', argumentBytes: 16, returns: 'void',
        resultBits: { pack: 0xff, standard: 0xff00, refresh: 0xc00000 },
        output: {
          address: 0x203720, bytes: 4, writable: true, masks: [0xff, 0xff00],
          table: { base: 0x202c38, entries: 0xbf, stride: 0xc },
        },
      },
    },
  })), /Observed option\/result facts/);
  assert.throws(() => validateProofReport(report({
    stop: {
      status: 10, target: 0xFD000100, ordinal: 1,
      service: { ordinal: 1, name: null, modeled: false },
    },
  })), /Only an unsupported-service/);
});

test('ordinal 127 stop carries only its known zero-argument frequency ABI', () => {
  const service = { ordinal: 127, name: 'KeQueryPerformanceFrequency', modeled: false,
    callingConvention: 'stdcall', argumentBytes: 0, returns: 'uint64' };
  const valid = report({ stop: { status: 1, target: 0xFE000000, ordinal: 127, service },
    calls: [{ ordinal: 127, status: 1, arguments: [] }] });
  assert.equal(validateProofReport(valid), valid);
  assert.throws(() => validateProofReport(report({ stop: { ...valid.stop,
    service: { ...service, argumentBytes: 4 } }, calls: valid.calls })), /zero-argument 64-bit ABI/);
  assert.throws(() => validateProofReport(report({ stop: valid.stop,
    calls: [{ ordinal: 127, status: 1, arguments: [1] }] })), /has no arguments/);
});

test('a worker-level error report is accepted without evidence fields', () => {
  const failure = { kind: 'error', message: 'Missing bounded device-aperture accessors' };
  assert.equal(validateProofReport(failure), failure);
});

test('reports that claim verification or omit the stop record are rejected', () => {
  assert.throws(() => validateProofReport(report({ verified: true })));
  assert.throws(() => validateProofReport(report({ stop: null })));
  assert.throws(() => validateProofReport(report({ stop: { status: 99, target: 0, ordinal: 0 } })));
  assert.throws(() => validateProofReport(report({ stop: { status: 10, target: -1, ordinal: 0 } })));
  assert.throws(() => validateProofReport(report({ phase: '' })));
  assert.throws(() => validateProofReport(report({ kind: '' })));
  assert.throws(() => validateProofReport(null));
});

test('trace entries and capacities are bounded', () => {
  assert.throws(() => validateProofReport(report({ trace: [{ kind: 'enter', address: 1 }] })));
  assert.throws(() => validateProofReport(report({ trace: new Array(TRACE_LIMIT + 1).fill({ kind: 'enter', address: 1, stack: 2 }) })));
  const atLimit = report({
    trace: new Array(TRACE_LIMIT).fill({ kind: 'enter', address: 1, stack: 2 }),
  });
  assert.equal(validateProofReport(atLimit), atLimit);
});

test('the reached original-routine list is bounded and type-checked when present', () => {
  const entered = values => values.map(address => ({ kind: 'enter', address, stack: 1 }));
  const counters = distinct => ({
    totalInitializers: 83, completedInitializers: 83,
    distinctOriginalFunctions: distinct, completedKernelOperations: 403,
  });
  assert.throws(() => validateProofReport(report({ originalFunctionsReached: 'nope' })),
    /reached original-routine list/);
  assert.throws(() => validateProofReport(report({ originalFunctionsReached: [1, 'x'] })),
    /reached original-routine list/);
  assert.throws(() => validateProofReport(report({
    originalFunctionsReached: new Array(TRACE_LIMIT + 1).fill(1),
  })), /reached original-routine list/);
  const repeated = report({ originalFunctionsReached: [1, 1, 2], originalStartup: counters(2),
    trace: entered([1, 1, 2]) });
  assert.equal(validateProofReport(repeated), repeated);
  const absent = report();
  assert.equal(validateProofReport(absent), absent);
  /* The distinct count is derived from the same entry sequence. */
  const matching = report({ originalFunctionsReached: [1, 1, 2], originalStartup: counters(2),
    trace: entered([1, 1, 2]) });
  assert.equal(validateProofReport(matching), matching);
  assert.throws(() => validateProofReport(report({
    originalFunctionsReached: [1, 1, 2], originalStartup: counters(3),
    trace: entered([1, 1, 2]),
  })), /does not match the distinct function count/);
  /* The list must be the trace's enter sequence, not a separate claim. */
  assert.throws(() => validateProofReport(report({
    originalFunctionsReached: [1, 1, 2], originalStartup: counters(2),
    trace: entered([1, 2, 3]),
  })), /does not match the entry trace/);
});

test('startup counters must stay in range and partial progress must be explained', () => {
  assert.throws(() => validateProofReport(report({
    originalStartup: { totalInitializers: 83, completedInitializers: 84, distinctOriginalFunctions: 1, completedKernelOperations: 1 },
  })));
  assert.throws(() => validateProofReport(report({
    originalStartup: { totalInitializers: 83, completedInitializers: 82, distinctOriginalFunctions: 1, completedKernelOperations: 1 },
  })));
  const partial = report({
    originalStartup: {
      totalInitializers: 83, completedInitializers: 82, distinctOriginalFunctions: 1,
      completedKernelOperations: 1, stopReason: 'unsupported Xbox service',
    },
  });
  assert.equal(validateProofReport(partial), partial);
  assert.throws(() => validateProofReport(report({
    originalStartup: { totalInitializers: 83, completedInitializers: 83, distinctOriginalFunctions: 513, completedKernelOperations: 1 },
  })));
});

test('startup-boundary reports require consistent checks and honest flags', () => {
  const sha = 'a'.repeat(64);
  const base = () => ({
    kind: 'startup-boundary',
    checks: { kernelReturnAddress: true, originalFunctionsReached: true },
    verified: true,
    sourceSha256: sha, moduleSha256: sha, hostImports: 0, biosUsed: false, gameBooted: false,
    originalFunctionsReached: [0x30F7F, 0x30FC0],
    trace: [{ kind: 'enter', address: 0x30F7F, stack: 0x3C30000 }],
    stop: { status: 1, ordinal: 3, target: 0xC0000000, stack: 0x3C2FFFC },
    limitation: 'Original startup only; no gameplay.',
  });
  const good = base();
  assert.equal(validateProofReport(good), good);
  const failing = { ...base(), checks: { kernelReturnAddress: true, originalFunctionsReached: false }, verified: false };
  assert.equal(validateProofReport(failing), failing);
  /* Verification must equal the checks, and the honesty flags may not be flipped. */
  assert.throws(() => validateProofReport({ ...base(), verified: false }));
  assert.throws(() => validateProofReport({ ...base(), checks: { a: 1 }, verified: true }));
  assert.throws(() => validateProofReport({ ...base(), checks: {} }));
  assert.throws(() => validateProofReport({ ...base(), gameBooted: true }));
  assert.throws(() => validateProofReport({ ...base(), hostImports: 1 }));
  assert.throws(() => validateProofReport({ ...base(), biosUsed: true }));
  assert.throws(() => validateProofReport({ ...base(), moduleSha256: 'short' }));
  assert.throws(() => validateProofReport({ ...base(), trace: new Array(17).fill({ kind: 'enter', address: 1, stack: 2 }) }));
  assert.throws(() => validateProofReport({ ...base(), originalFunctionsReached: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17] }));
  assert.throws(() => validateProofReport({ ...base(), limitation: '' }));
  assert.throws(() => validateProofReport({ ...base(), stop: null }));
  assert.throws(() => validateProofReport({ kind: 'unknown-boundary' }));
});

test('the supplied device input profile is validated when present', () => {
  const supplied = () => ({
    window: { base: 0xFD000000, size: 0x01000000 },
    registers: [{ address: 0xFD001804, initial: 0 }, { address: 0xFD600140, initial: 0 }],
    zeroRanges: [{ address: 0xFD001000, bytes: 16 }],
  });
  const withSupplied = value => report({
    deviceRegisters: { count: 0, events: 0, entries: [], log: [], supplied: value },
  });
  const valid = withSupplied(supplied());
  assert.equal(validateProofReport(valid), valid);
  assert.equal(validateProofReport(withSupplied(null)).deviceRegisters.supplied, null);
  const bad = [
    [],
    { ...supplied(), window: { base: 0xFD000001, size: 0x01000000 } },
    { ...supplied(), window: { base: 0xFD000000, size: 0x100 } },
    { ...supplied(), registers: [{ address: 0xFD001804, initial: 0 }] },
    { ...supplied(), registers: [{ address: 0xFD001802, initial: 0 }, { address: 0xFD600140, initial: 0 }] },
    { ...supplied(), registers: [{ address: 0xFD001804, initial: 0 }, { address: 0xFD001804, initial: 1 }] },
    { ...supplied(), registers: [{ address: 0xFD001804, initial: 0 }, { address: 0xFD101800, initial: 0 }] },
    { ...supplied(), registers: [{ address: 0xFD001804, initial: 0 }, { address: 0xFE000000, initial: 0 }] },
    { ...supplied(), zeroRanges: [{ address: 0xFD001002, bytes: 16 }] },
    { ...supplied(), zeroRanges: [{ address: 0xFD001000, bytes: 0x1000000 }] },
    { ...supplied(), zeroRanges: [{ address: 0xFD001000, bytes: 16 }, { address: 0xFD001000, bytes: 16 }] },
  ];
  for (const value of bad)
    assert.throws(() => validateProofReport(withSupplied(value)), /supplied device/);
});

test('declared status waits and display state are validated when present', () => {
  const waits = report({
    deviceRegisters: {
      count: 1, events: 1, entries: [{}], log: [{}],
      statusWaits: [{ address: 0xFD000100, mask: 0x01000000, value: 0, count: 65 }],
    },
    pcrtc: { pending: 0, enabled: 1, events: 8 },
  });
  assert.equal(validateProofReport(waits), waits);
  const empty = report({ deviceRegisters: { count: 0, events: 0, entries: [], log: [], statusWaits: [] } });
  assert.equal(validateProofReport(empty), empty);
  const bad = [
    [{ address: 0xFD000100, mask: 0, value: 0, count: 1 }],
    [{ address: 0xFD000100, mask: 0x10, value: 0, count: -1 }],
    [{ address: 'FD000100', mask: 0x10, value: 0, count: 1 }],
    [{ mask: 0x10, value: 0, count: 1 }],
    new Array(5).fill({ address: 1, mask: 1, value: 0, count: 1 }),
  ];
  for (const statusWaits of bad) {
    assert.throws(() => validateProofReport(report({
      deviceRegisters: { count: 1, events: 1, entries: [{}], log: [{}], statusWaits },
    })));
  }
  for (const pcrtc of [null, {}, { pending: -1, enabled: 1, events: 1 }, { pending: 0, enabled: 1, events: 99999 }]) {
    assert.throws(() => validateProofReport(report({ pcrtc })));
  }
});

test('experimental inputs and device capacities are validated', () => {
  assert.throws(() => validateProofReport(report({ experiments: 'writeClearing' })));
  assert.throws(() => validateProofReport(report({ experiments: ['pmcZer0'] })));
  assert.throws(() => validateProofReport(report({
    deviceRegisters: { count: 1, events: DEVICE_EVENT_LIMIT + 1, entries: [{}], log: [] },
  })));
  assert.throws(() => validateProofReport(report({
    deviceRegisters: { count: 1, events: 1, entries: [{}], log: new Array(DEVICE_EVENT_LIMIT + 1).fill({}) },
  })));
});
