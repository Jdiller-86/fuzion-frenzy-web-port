import assert from 'node:assert/strict';
import test from 'node:test';
import { describeKernelService } from '../src/runtime/kernel-service.mjs';

test('ordinal 2 is named AvSendTVEncoderOption with static facts only', () => {
  assert.deepEqual(describeKernelService(2), {
    ordinal: 2,
    name: 'AvSendTVEncoderOption',
    modeled: false,
    callingConvention: 'stdcall',
    argumentBytes: 16,
    returns: 'void',
  });
});

test('unknown and invalid ordinals are unnamed and not modeled', () => {
  assert.deepEqual(describeKernelService(3), { ordinal: 3, name: null, modeled: false });
  assert.deepEqual(describeKernelService(-1), { ordinal: -1, name: null, modeled: false });
  assert.deepEqual(describeKernelService(0x10000), { ordinal: 0x10000, name: null, modeled: false });
  assert.deepEqual(describeKernelService(2.5), { ordinal: 2.5, name: null, modeled: false });
});
