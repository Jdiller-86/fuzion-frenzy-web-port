import test from 'node:test';
import assert from 'node:assert/strict';
import { validateKernelCapture, readKernelCapture, absoluteKernelCalls } from '../runtime/browser/kernel-capture.mjs';

const calls = [{ ordinal: 277, status: 0, arguments: [123] }];
const capture = {
  kind: 'kernel-capture-window', version: 1, requestedStart: 512, capturedStart: 512,
  capturedCount: 1, totalCount: 513, completedOperations: 512, prefixCompletedOperations: 511,
  complete: false, sha256: 'a'.repeat(64), prefixSha256: 'b'.repeat(64),
};
test('capture windows retain explicit omission, absolute indexes and completion totals', () => {
  assert.equal(validateKernelCapture(capture, calls), capture);
  const indexed = absoluteKernelCalls(capture, calls);
  assert.equal(indexed.length, 513);
  assert.equal(0 in indexed, false);
  assert.equal(indexed[512], calls[0]);
  assert.deepEqual(indexed.filter(call => call.status === 0), calls);
});
test('capture window validation rejects hidden omissions, malformed digests and inflated counts', () => {
  for (const change of [
    { complete: true }, { totalCount: 514 }, { capturedCount: 2 }, { requestedStart: 1024 },
    { completedOperations: 513 }, { prefixCompletedOperations: 513, completedOperations: 514 },
    { sha256: 'no' }, { prefixSha256: undefined }, { capturedStart: 0 },
  ]) assert.throws(() => validateKernelCapture({ ...capture, ...change }, calls));
});
test('a requested suffix cannot be empty', () => {
  const emptySuffix = { ...capture, capturedCount: 0, totalCount: 512, completedOperations: 511 };
  assert.throws(() => validateKernelCapture(emptySuffix, []), /Malformed bounded kernel capture/);
});
test('requested second window may stop early with the full first prefix still captured', () => {
  const early = { ...capture, capturedStart: 0, capturedCount: 1, totalCount: 1,
    completedOperations: 1, prefixCompletedOperations: 0, complete: true };
  delete early.prefixSha256;
  assert.equal(validateKernelCapture(early, calls), early);
  assert.equal(absoluteKernelCalls(early, calls), calls);
});
test('capture ABI decodes fixed digest words and leaves legacy modules without a window', () => {
  const words = new Map([[0,1],[1,512],[2,512],[3,513],[4,512],[5,511]]);
  for (let i = 8; i < 16; i++) words.set(i, 0xaaaaaaaa);
  for (let i = 16; i < 24; i++) words.set(i, 0xbbbbbbbb);
  const api = { ff_probe_abi: () => 25, ff_probe_kernel_capture_word: field => words.get(field) ?? 0 };
  assert.deepEqual(readKernelCapture(api, calls), capture);
  assert.equal(readKernelCapture({ ff_probe_abi: () => 24 }, calls), undefined);
});
