import assert from 'node:assert/strict';
import test from 'node:test';
import { validateProofReport, TRACE_LIMIT, DEVICE_EVENT_LIMIT } from '../src/runtime/proof-report.mjs';

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

test('an unsupported-service stop must carry a non-modeled service descriptor', () => {
  const valid = report({
    stop: {
      status: 1, target: 0xFE000094, ordinal: 2,
      service: {
        ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false,
        callingConvention: 'stdcall', argumentBytes: 16, returns: 'void',
        option: { code: 6, name: 'AV_OPTION_QUERY_AVPACK' },
        resultBits: { pack: 0xff, standard: 0xff00, refresh: 0xc00000 },
        output: {
          address: 0x203720, bytes: 4, writable: true,
          masks: [0xff, 0xff00],
          table: { base: 0x202c38, entries: 0xbf, stride: 0xc },
        },
      },
    },
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
        option: { code: 6, name: 'AV_OPTION_QUERY_AVPACK' },
        resultBits: { pack: 0xff, standard: 0xff00, refresh: 0xc00000 },
        output: {
          address: 0x203720, bytes: 4, writable: true, masks: [0xff],
          table: { base: 0x202c38, entries: 0xbf, stride: 0xc },
        },
      },
    },
  })), /output facts/);
  assert.throws(() => validateProofReport(report({
    stop: {
      status: 1, target: 0xFE000094, ordinal: 2,
      service: {
        ordinal: 2, name: 'AvSendTVEncoderOption', modeled: false,
        callingConvention: 'stdcall', argumentBytes: 16, returns: 'void',
        option: { code: 6, name: 'AV_OPTION_WRONG' },
        resultBits: { pack: 0xff, standard: 0xff00, refresh: 0xc00000 },
        output: {
          address: 0x203720, bytes: 4, writable: true, masks: [0xff, 0xff00],
          table: { base: 0x202c38, entries: 0xbf, stride: 0xc },
        },
      },
    },
  })), /option\/result-bit/);
  assert.throws(() => validateProofReport(report({
    stop: {
      status: 10, target: 0xFD000100, ordinal: 1,
      service: { ordinal: 1, name: null, modeled: false },
    },
  })), /Only an unsupported-service/);
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
