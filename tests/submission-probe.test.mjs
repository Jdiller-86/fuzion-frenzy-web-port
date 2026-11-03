import assert from 'node:assert/strict';
import test from 'node:test';
import { createSubmissionProbe, SUBMISSION_PROBE_BYTES } from '../src/runtime/submission-probe.mjs';

function fixture({ put = 0x800, get = 0, status = 13, includePut = true, includeGet = false } = {}) {
  const buffer = new ArrayBuffer(0x1000);
  const view = new DataView(buffer);
  for (let offset = 0; offset < buffer.byteLength; offset += 4) view.setUint32(offset, 0x10000000 + offset, true);
  const entries = [];
  if (includePut) entries.push({ address: 0xFD800040, value: put });
  if (includeGet) entries.push({ address: 0xFD800044, value: get });
  return { view, ramBytes: buffer.byteLength, entries, status };
}

test('a status-13 snapshot returns the bounded window before the put cursor', () => {
  const { view, ramBytes, entries, status } = fixture({ get: 0x1234, includeGet: true });
  const probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.available, true);
  assert.equal(probe.channel, 0);
  assert.equal(probe.put, 0x800);
  assert.equal(probe.get, 0x1234);
  assert.equal(probe.base, 0x800 - SUBMISSION_PROBE_BYTES);
  assert.equal(probe.bytes, SUBMISSION_PROBE_BYTES);
  assert.equal(probe.words.length, SUBMISSION_PROBE_BYTES / 4);
  assert.equal(probe.words[0], 0x10000000 + probe.base);
  assert.equal(probe.words.at(-1), 0x10000000 + probe.base + SUBMISSION_PROBE_BYTES - 4);
  assert.match(probe.note, /not decoded commands/);
  assert.equal(view.getUint32(probe.base, true), probe.words[0]);
  assert.equal(probe.allocation.available, false);
  assert.equal(probe.allocation.reason, 'no observed GPU-instance allocation');
});

test('an observed allocation base adds a bounded read-only second window', () => {
  const { view, ramBytes, entries, status } = fixture();
  const before = new Uint8Array(view.buffer).slice();
  const probe = createSubmissionProbe({ status, entries, view, ramBytes, allocation: { base: 0x400, bytes: 256 } });
  assert.equal(probe.allocation.available, true);
  assert.equal(probe.allocation.base, 0x400);
  assert.equal(probe.allocation.bytes, 256);
  assert.equal(probe.allocation.words[0], 0x10000000 + 0x400);
  assert.equal(probe.allocation.words.at(-1), 0x10000000 + 0x400 + 252);
  assert.deepEqual(new Uint8Array(view.buffer), before);
  const short = createSubmissionProbe({ status, entries, view, ramBytes, allocation: { base: 0x400, bytes: 4 } });
  assert.equal(short.allocation.available, true);
  assert.equal(short.allocation.bytes, 4);
  const outside = createSubmissionProbe({ status, entries, view, ramBytes, allocation: { base: 0x1000, bytes: 256 } });
  assert.equal(outside.allocation.available, false);
  assert.equal(outside.allocation.reason, 'window outside aligned guest memory');
});

test('the snapshot is read-only and reports missing get as zero', () => {
  const { view, ramBytes, entries, status } = fixture();
  const before = new Uint8Array(view.buffer).slice();
  const probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.get, 0);
  assert.deepEqual(new Uint8Array(view.buffer), before);
});

test('the snapshot fails closed outside the submission boundary or memory', () => {
  const base = fixture();
  assert.deepEqual(createSubmissionProbe({ ...base, status: 12 }), { available: false, reason: 'not stopped at the PFIFO DMA submission boundary' });
  assert.deepEqual(createSubmissionProbe({ ...fixture({ includePut: false }) }), { available: false, reason: 'no observed DMA_PUT write' });
  assert.deepEqual(createSubmissionProbe({ ...fixture({ put: 0x802 }) }), { available: false, reason: 'DMA_PUT outside aligned guest memory' });
  assert.deepEqual(createSubmissionProbe({ ...fixture({ put: 0x1000 }) }), { available: false, reason: 'DMA_PUT outside aligned guest memory' });
  assert.deepEqual(createSubmissionProbe({ ...base, view: null }), { available: false, reason: 'no checked guest memory view' });
  assert.deepEqual(createSubmissionProbe({ ...base, windowBytes: 260 }), { available: false, reason: 'invalid snapshot size' });
  assert.deepEqual(createSubmissionProbe({ ...base, windowBytes: 512 }), { available: false, reason: 'invalid snapshot size' });
});

test('a low put cursor clamps the snapshot base to zero', () => {
  const { view, ramBytes, entries, status } = fixture({ put: 0x80 });
  const probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.available, true);
  assert.equal(probe.base, 0);
  assert.equal(probe.words.length, SUBMISSION_PROBE_BYTES / 4);
});
