import test from 'node:test';
import assert from 'node:assert/strict';
import { DiscSessionCore, DiscSessionError } from '../src/runtime/disc-session-core.mjs';
import { DiscSession } from '../src/runtime/disc-session.mjs';

const SECTOR = 2048;
function fixture() {
  const b = new Uint8Array(36 * SECTOR), v = new DataView(b.buffer), enc = new TextEncoder();
  const d = 32 * SECTOR, root = 33 * SECTOR, xbe = 34 * SECTOR, data = 35 * SECTOR;
  b.set(enc.encode('MICROSOFT*XBOX*MEDIA'), d);
  b.set(enc.encode('MICROSOFT*XBOX*MEDIA'), d + SECTOR - 20);
  v.setUint32(d + 20, 33, true);
  v.setUint32(d + 24, 128, true);
  v.setUint16(root + 0, 0, true);
  v.setUint16(root + 2, 16, true);
  v.setUint32(root + 4, 34, true);
  v.setUint32(root + 8, 16, true);
  b[root + 12] = 0x20;
  b[root + 13] = 11;
  b.set(enc.encode('default.xbe'), root + 14);
  const second = root + 64;
  v.setUint16(second + 0, 0, true);
  v.setUint16(second + 2, 0, true);
  v.setUint32(second + 4, 35, true);
  v.setUint32(second + 8, 4, true);
  b[second + 12] = 0x20;
  b[second + 13] = 8;
  b.set(enc.encode('data.bin'), second + 14);
  const xbeBytes = new Uint8Array(16);
  for (let i = 0; i < xbeBytes.length; ++i) xbeBytes[i] = i + 1;
  b.set(xbeBytes, xbe);
  b.set([0x10, 0x20, 0x30, 0x40], data);
  return { b, xbeBytes };
}
async function expected(xbeBytes) {
  const digest = await crypto.subtle.digest('SHA-256', xbeBytes);
  return { size: xbeBytes.length, sha256: [...new Uint8Array(digest)].map(x => x.toString(16).padStart(2, '0')).join('') };
}
const blob = b => new Blob([b]);

test('a supported XISO opens and returns only requested byte ranges', async () => {
  const { b, xbeBytes } = fixture();
  const core = new DiscSessionCore();
  const info = await core.open(blob(b), await expected(xbeBytes));
  assert.deepEqual(info, { imageBytes: b.length, xbeBytes: 16 });
  const data = await core.read('data.bin', 1, 2);
  assert.deepEqual([...data.bytes], [0x20, 0x30]);
  assert.equal(data.fileSize, 4);
  assert.equal(core.dataBytesRead, 2);
  const executable = await core.read('default.xbe', 0, 16);
  assert.deepEqual([...executable.bytes], [...xbeBytes]);
  assert.ok(core.dataBytesRead < b.length);
});

test('unsupported identity, malformed image, unsafe path and invalid range fail closed', async () => {
  const { b, xbeBytes } = fixture();
  const id = await expected(xbeBytes);
  const core = new DiscSessionCore();
  await assert.rejects(core.open(blob(b), { ...id, sha256: 'a'.repeat(64) }), /Unsupported or changed content/);
  await assert.rejects(core.read('data.bin', 0, 1), /not open/);
  assert.equal(core.opened, false);
  await assert.rejects(new DiscSessionCore().open(new Blob([]), id), /non-empty/);
  await assert.rejects(new DiscSessionCore().open(blob(b.subarray(0, 32 * SECTOR)), id), /XDVDFS|range/);
  const opened = new DiscSessionCore();
  await opened.open(blob(b), id);
  await assert.rejects(opened.read('../data.bin', 0, 1), /Invalid XDVDFS path|path/);
  await assert.rejects(opened.read('missing.bin', 0, 1), /not found|path/);
  await assert.rejects(opened.read('data.bin', -1, 1), /range/);
  await assert.rejects(opened.read('data.bin', 0, 0), /range/);
  await assert.rejects(opened.read('data.bin', 0, 65537), /range/);
  await assert.rejects(opened.read('data.bin', 4, 1), /range/);
  await assert.rejects(opened.read('data.bin', 3, 2), /range/);
});

test('only one read may be outstanding and clear rejects a late result', async () => {
  const { b, xbeBytes } = fixture();
  const core = new DiscSessionCore();
  await core.open(blob(b), await expected(xbeBytes));
  const first = core.read('default.xbe', 0, 16);
  await assert.rejects(core.read('data.bin', 0, 1), error => error instanceof DiscSessionError && error.code === 'busy');
  await first;
  const late = core.read('default.xbe', 0, 16);
  core.close();
  await assert.rejects(late, error => error instanceof DiscSessionError && error.code === 'state');
  await assert.rejects(core.read('data.bin', 0, 1), /not open/);
});

test('a new core opens an independent session after clear', async () => {
  const { b, xbeBytes } = fixture();
  const id = await expected(xbeBytes);
  const first = new DiscSessionCore();
  await first.open(blob(b), id);
  first.close();
  const second = new DiscSessionCore();
  await second.open(blob(b), id);
  assert.deepEqual([...(await second.read('data.bin', 0, 4)).bytes], [0x10, 0x20, 0x30, 0x40]);
  second.close();
});

class FakeWorker {
  constructor() { this.sent = []; this.terminated = false; this.onmessage = null; this.onerror = null; }
  postMessage(message) { this.sent.push(message); }
  terminate() { this.terminated = true; }
  reply(message) { this.onmessage?.({ data: message }); }
}
const tick = () => new Promise(resolve => setImmediate(resolve));

test('the client enforces one request, late-result isolation and clear termination', async () => {
  const worker = new FakeWorker();
  const opening = DiscSession.open(new Blob([1, 2, 3]), {
    expectedIdentity: { size: 1, sha256: 'a'.repeat(64) },
    workerFactory: () => worker,
  });
  await tick();
  assert.equal(worker.sent[0].type, 'open');
  worker.reply({ type: 'opened', id: worker.sent[0].id, result: { imageBytes: 3, xbeBytes: 1 } });
  const session = await opening;
  const read = session.read('data.bin', 0, 1);
  await tick();
  assert.equal(worker.sent[1].type, 'read');
  await assert.rejects(session.read('data.bin', 1, 1), /Only one request/);
  worker.reply({ type: 'read-result', id: worker.sent[1].id, result: { bytes: new Uint8Array([1]) } });
  await read;
  session.clear();
  assert.equal(worker.terminated, true);
  await assert.rejects(session.read('data.bin', 0, 1), /not open/);
  worker.reply({ type: 'read-result', id: worker.sent[1].id, result: { bytes: new Uint8Array([9]) } });
});

test('worker errors reject the pending request and terminate the session', async () => {
  const worker = new FakeWorker();
  const opening = DiscSession.open(new Blob([1]), {
    expectedIdentity: { size: 1, sha256: 'a'.repeat(64) },
    workerFactory: () => worker,
  });
  await tick();
  worker.reply({ type: 'error', id: worker.sent[0].id, code: 'identity', message: 'unsupported' });
  await assert.rejects(opening, /unsupported/);
  assert.equal(worker.terminated, true);
});
