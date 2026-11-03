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
  assert.equal(probe.base, Math.max(0, 0x800 - SUBMISSION_PROBE_BYTES));
  assert.equal(probe.bytes, SUBMISSION_PROBE_BYTES);
  assert.equal(probe.words.length, SUBMISSION_PROBE_BYTES / 4);
  assert.equal(probe.words[0], 0x10000000 + probe.base);
  assert.equal(probe.words.at(-1), 0x10000000 + probe.base + SUBMISSION_PROBE_BYTES - 4);
  assert.match(probe.note, /UMA identity mapping/);
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

test('the PFIFO DMA object decodes from the instance register and RAMIN entries', () => {
  const { view, ramBytes, entries, status } = fixture();
  entries.push({ address: 0xFD00322C, value: 0x11C });
  entries.push({ address: 0xFD7011C0, value: 0xB002 });
  entries.push({ address: 0xFD7011C4, value: 0x3FFAFFF });
  entries.push({ address: 0xFD7011C8, value: 3 });
  const probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.dmaObject.available, true);
  assert.equal(probe.dmaObject.instanceRegister, 0x11C);
  assert.equal(probe.dmaObject.objectAddress, 0x11C0);
  assert.equal(probe.dmaObject.flags, 0xB002);
  assert.equal(probe.dmaObject.dmaClass, 2);
  assert.equal(probe.dmaObject.target, 0);
  assert.equal(probe.dmaObject.targetName, 'NVM/VRAM');
  assert.equal(probe.dmaObject.adjust, 0);
  assert.equal(probe.dmaObject.base, 0);
  assert.equal(probe.dmaObject.limit, 0x3FFAFFF);
  assert.match(probe.dmaObject.note, /offsets into the DMA object/);
});

test('the DMA object decoder reports missing or incomplete state', () => {
  const { view, ramBytes, entries, status } = fixture();
  assert.deepEqual(createSubmissionProbe({ status, entries, view, ramBytes }).dmaObject,
    { available: false, reason: 'no observed PFIFO DMA instance register' });
  entries.push({ address: 0xFD00322C, value: 0x11C });
  assert.deepEqual(createSubmissionProbe({ status, entries, view, ramBytes }).dmaObject,
    { available: false, reason: 'no complete DMA object in the observed RAMIN entries' });
});

test('the DMA object decoder applies target and adjust masks', () => {
  const { view, ramBytes, entries, status } = fixture();
  entries.push({ address: 0xFD00322C, value: 0x10 });
  entries.push({ address: 0xFD700100, value: 0x00330205 });
  entries.push({ address: 0xFD700104, value: 0x00100000 });
  entries.push({ address: 0xFD700108, value: 0x00200000 });
  const probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.dmaObject.available, true);
  assert.equal(probe.dmaObject.objectAddress, 0x100);
  assert.equal(probe.dmaObject.dmaClass, 0x205);
  assert.equal(probe.dmaObject.targetName, 'AGP');
  assert.equal(probe.dmaObject.adjust, 0x00300000);
  assert.equal(probe.dmaObject.base, 0x00300000);
});

test('the DMA resolver reports a candidate guest address without claiming backing', () => {
  const { view, ramBytes, entries, status } = fixture({ put: 0x800, get: 0 });
  entries.push({ address: 0xFD00322C, value: 0x11C });
  entries.push({ address: 0xFD7011C0, value: 0xB002 });
  entries.push({ address: 0xFD7011C4, value: 0x3FFAFFF });
  entries.push({ address: 0xFD7011C8, value: 3 });
  const probe = createSubmissionProbe({ status, entries, view, ramBytes, allocation: { base: 0x400, bytes: 256 } });
  assert.deepEqual(probe.putResolved, {
    resolved: true, candidate: false, verified: true, address: 0x800,
    backing: 'xemu UMA: VRAM is system RAM (identity)', reason: 'target backed by guest physical RAM',
  });
  assert.equal(probe.getResolved.address, 0);
  const noObject = createSubmissionProbe({ status, entries: [], view, ramBytes });
  assert.equal(noObject.available, false);
});

test('the DMA resolver fails closed on target, limit and RAM bounds', () => {
  const { view, ramBytes, entries, status } = fixture({ put: 0x800 });
  const object = (flags, limit, frame) => {
    entries.length = 1;
    entries.push({ address: 0xFD00322C, value: 0x10 });
    entries.push({ address: 0xFD700100, value: flags });
    entries.push({ address: 0xFD700104, value: limit });
    entries.push({ address: 0xFD700108, value: frame });
  };
  object(0x00030002, 0x3FFAFFF, 0);
  let probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.putResolved.resolved, false);
  assert.match(probe.putResolved.reason, /AGP is not modeled/);
  object(0x00000002, 0x100, 0);
  probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.putResolved.reason, 'DMA offset beyond object limit');
  object(0x00000002, 0x3FFAFFF, 0x1000);
  probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.putResolved.reason, 'resolved address outside modeled guest RAM');
});

test('the allocation page summary counts nonzero dwords without storing bytes', () => {
  const { view, ramBytes, entries, status } = fixture();
  const probe = createSubmissionProbe({ status, entries, view, ramBytes, allocation: { base: 0, bytes: 4096 } });
  assert.equal(probe.allocationPages.available, true);
  assert.deepEqual(probe.allocationPages.pages, [{ offset: 0, nonzero: 1024 }]);
  new Uint8Array(view.buffer).fill(0);
  const zero = createSubmissionProbe({ status, entries, view, ramBytes, allocation: { base: 0, bytes: 4096 } });
  assert.deepEqual(zero.allocationPages.pages, [{ offset: 0, nonzero: 0 }]);
});

test('the PFIFO state reports DMA mode and the USER/CACHE1 alias gap', () => {
  const { view, ramBytes, entries, status } = fixture({ put: 0x800, get: 0 });
  entries.push({ address: 0xFD003204, value: 0x100 });
  entries.push({ address: 0xFD00322C, value: 0x11C });
  entries.push({ address: 0xFD003240, value: 0 });
  entries.push({ address: 0xFD003244, value: 0 });
  let probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.pfifoState.available, true);
  assert.equal(probe.pfifoState.mode, 'DMA');
  assert.equal(probe.pfifoState.channel, 0);
  assert.equal(probe.pfifoState.instanceRegister, 0x11C);
  assert.equal(probe.pfifoState.cachePut, 0);
  assert.equal(probe.pfifoState.userPut, 0x800);
  assert.equal(probe.pfifoState.aliasConsistent, false);
  entries.find(entry => entry.address === 0xFD003240).value = 0x800;
  probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.pfifoState.aliasConsistent, true);
  const noPush = createSubmissionProbe({ status, entries: [], view, ramBytes });
  assert.equal(noPush.available, false);
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
  assert.deepEqual(createSubmissionProbe({ ...base, windowBytes: 0 }), { available: false, reason: 'invalid snapshot size' });
  assert.deepEqual(createSubmissionProbe({ ...base, windowBytes: 4100 }), { available: false, reason: 'invalid snapshot size' });
});

test('a low put cursor clamps the snapshot base to zero', () => {
  const { view, ramBytes, entries, status } = fixture({ put: 0x80 });
  const probe = createSubmissionProbe({ status, entries, view, ramBytes });
  assert.equal(probe.available, true);
  assert.equal(probe.base, 0);
  assert.equal(probe.words.length, SUBMISSION_PROBE_BYTES / 4);
});
