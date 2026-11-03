import assert from 'node:assert/strict';
import test from 'node:test';
import { scanPushbuffer, PFIFO_SCAN_MAX_WORDS, PFIFO_SCAN_ABSOLUTE_WORDS } from '../src/runtime/pfifo-scan.mjs';

function words(values) {
  const buffer = new ArrayBuffer(Math.max(4, values.length * 4));
  const view = new DataView(buffer);
  values.forEach((value, index) => view.setUint32(index * 4, value >>> 0, true));
  return view;
}

test('the scanner inventories increasing, non-increasing and flow headers', () => {
  const view = words([
    0x00000000 | (1 << 13) | 0x100 | (2 << 18), // increasing, subch 1, method 0x100, 2 params
    0x11111111, 0x22222222,
    0x40000000 | (2 << 13) | 0x200 | (1 << 18), // non-increasing, subch 2, method 0x200, 1 param
    0x33333333,
    0x00000001, // jump
    0x00000002, // call
    0x00020000, // return
    0xFFFFFFFF, // unknown
  ]);
  const scan = scanPushbuffer(view, 0, view.byteLength);
  assert.equal(scan.available, true);
  assert.equal(scan.totalWords, 9);
  assert.equal(scan.scannedWords, 9);
  assert.equal(scan.truncated, false);
  assert.equal(scan.pendingParams, 0);
  assert.ok(scan.nonZeroWords >= 5);
  assert.equal(scan.firstNonZero, 0);
  assert.equal(scan.lastNonZero, 32);
  assert.equal(scan.unknown, 1);
  assert.equal(scan.jumps, 1);
  assert.equal(scan.calls, 1);
  assert.equal(scan.returns, 1);
  assert.equal(scan.methodKinds, 2);
  assert.deepEqual(scan.methods, [
    { subchannel: 1, method: 0x100, count: 1 },
    { subchannel: 2, method: 0x200, count: 1 },
  ]);
  assert.match(scan.note, /GET not advanced/);
});

test('the scanner stops at its word budget and reports truncation', () => {
  const values = Array.from({ length: PFIFO_SCAN_MAX_WORDS + 8 }, () => 0);
  const view = words(values);
  const scan = scanPushbuffer(view, 0, view.byteLength);
  assert.equal(scan.scannedWords, PFIFO_SCAN_MAX_WORDS);
  assert.equal(scan.totalWords, values.length);
  assert.equal(scan.truncated, true);
  assert.equal(scan.methods[0].method, 0);
});

test('parameter words are not decoded as headers', () => {
  const view = words([0x00000000 | (1 << 18), 0xFFFFFFFF, 0x00000001]);
  const scan = scanPushbuffer(view, 0, view.byteLength);
  assert.equal(scan.methodKinds, 1);
  assert.equal(scan.pendingParams, 0);
  assert.equal(scan.jumps, 1);
  assert.equal(scan.unknown, 0);
});

test('the scanner fails closed on invalid ranges and budgets', () => {
  const view = words([0, 0, 0, 0]);
  assert.deepEqual(scanPushbuffer(null, 0, 4), { available: false, reason: 'no checked memory view' });
  assert.deepEqual(scanPushbuffer(view, 2, 8), { available: false, reason: 'unaligned pushbuffer range' });
  assert.deepEqual(scanPushbuffer(view, 8, 4), { available: false, reason: 'invalid pushbuffer range' });
  assert.deepEqual(scanPushbuffer(view, 0, 0x04000004), { available: false, reason: 'invalid pushbuffer range' });
  assert.deepEqual(scanPushbuffer(view, 0, 0x100), { available: false, reason: 'range outside modeled memory' });
  assert.deepEqual(scanPushbuffer(view, 0, 4, { maxWords: 0 }), { available: false, reason: 'invalid scan budget' });
  assert.deepEqual(scanPushbuffer(view, 0, 4, { maxWords: PFIFO_SCAN_ABSOLUTE_WORDS + 1 }), { available: false, reason: 'invalid scan budget' });
});

test('an explicit budget above the default scans a larger range', () => {
  const values = Array.from({ length: 5000 }, () => 0);
  const view = words(values);
  assert.equal(scanPushbuffer(view, 0, view.byteLength).truncated, true);
  assert.equal(scanPushbuffer(view, 0, view.byteLength).scannedWords, PFIFO_SCAN_MAX_WORDS);
  const full = scanPushbuffer(view, 0, view.byteLength, { maxWords: 8192 });
  assert.equal(full.scannedWords, 5000);
  assert.equal(full.truncated, false);
});
