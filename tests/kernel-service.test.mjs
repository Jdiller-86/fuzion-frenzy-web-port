import assert from 'node:assert/strict';
import test from 'node:test';
import { describeKernelService } from '../src/runtime/kernel-service.mjs';

test('ordinal 2 is named AvSendTVEncoderOption with the pinned option/result facts and is not modeled', () => {
  assert.deepEqual(describeKernelService(2), {
    ordinal: 2,
    name: 'AvSendTVEncoderOption',
    modeled: false,
    callingConvention: 'stdcall',
    argumentBytes: 16,
    returns: 'void',
    option: { code: 6, name: 'AV_OPTION_QUERY_AVPACK' },
    resultBits: { pack: 0xff, standard: 0xff00, refresh: 0xc00000 },
    output: {
      address: 0x203720,
      bytes: 4,
      writable: true,
      masks: [0xff, 0xff00],
      table: { base: 0x202c38, entries: 0xbf, stride: 0xc },
    },
  });
});

test('unknown and invalid ordinals are unnamed and not modeled', () => {
  assert.deepEqual(describeKernelService(3), { ordinal: 3, name: null, modeled: false });
  assert.deepEqual(describeKernelService(-1), { ordinal: -1, name: null, modeled: false });
  assert.deepEqual(describeKernelService(0x10000), { ordinal: 0x10000, name: null, modeled: false });
  assert.deepEqual(describeKernelService(2.5), { ordinal: 2.5, name: null, modeled: false });
});
