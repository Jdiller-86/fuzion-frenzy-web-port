import { DiscSession } from '/src/runtime/disc-session.mjs';

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
async function sha256(bytes) {
  const digest = await crypto.subtle.digest('SHA-256', bytes);
  return [...new Uint8Array(digest)].map(x => x.toString(16).padStart(2, '0')).join('');
}
async function run() {
  const { b, xbeBytes } = fixture();
  const expectedIdentity = { size: xbeBytes.length, sha256: await sha256(xbeBytes) };
  const file = new File([b], 'fixture.iso', { type: 'application/octet-stream' });
  const session = await DiscSession.open(file, { expectedIdentity });
  const inventory = await session.inventory();
  const data = await session.read('data.bin', 1, 2);
  const executable = await session.read('default.xbe', 0, 16);
  const oversized = await session.read('data.bin', 0, 65537).then(() => false, error => error.code === 'range');
  const unsafe = await session.read('../data.bin', 0, 1).then(() => false, error => error.code === 'path');
  const first = session.read('default.xbe', 0, 16);
  const busy = await session.read('data.bin', 0, 1).then(() => false, error => error.code === 'busy');
  await first;
  session.clear();
  const cleared = await session.read('data.bin', 0, 1).then(() => false, error => error.code === 'state');
  const reopened = await DiscSession.open(file, { expectedIdentity });
  const again = await reopened.read('data.bin', 0, 4);
  reopened.clear();
  return {
    ok: true,
    inventory: inventory.map(entry => entry.name),
    data: [...data.bytes],
    executable: [...executable.bytes],
    oversized,
    unsafe,
    busy,
    cleared,
    reopened: [...again.bytes],
  };
}
run().then(result => {
  window.__discSessionResult = result;
  document.querySelector('#result').textContent = JSON.stringify(result);
}).catch(error => {
  const result = { ok: false, error: error.message, code: error.code };
  window.__discSessionResult = result;
  document.querySelector('#result').textContent = JSON.stringify(result);
});
