import assert from 'node:assert/strict';
import {openXiso} from '../src/xdvdfs.mjs';
function fixture() {
  const b = new Uint8Array(35 * 2048), v = new DataView(b.buffer), enc = new TextEncoder();
  const d = 32 * 2048, root = 33 * 2048;
  b.set(enc.encode('MICROSOFT*XBOX*MEDIA'), d); b.set(enc.encode('MICROSOFT*XBOX*MEDIA'), d + 2028);
  v.setUint32(d + 20, 33, true); v.setUint32(d + 24, 64, true);
  v.setUint32(root + 4, 34, true); v.setUint32(root + 8, 3, true);
  b[root + 12] = 0x20; b[root + 13] = 5; b.set(enc.encode('a.bin'), root + 14);
  b.set([3, 2, 1], 34 * 2048); return b;
}
const b = fixture(), fs = await openXiso(new Blob([b]));
assert.deepEqual([...new Uint8Array(await fs.file(await fs.find('A.BIN')).arrayBuffer())], [3, 2, 1]);
await assert.rejects(fs.find('../a.bin'));
for (const mutate of [
 b => {b[32*2048] = 0;},
 b => {new DataView(b.buffer).setUint32(32*2048+20, 99999, true);},
 b => {new DataView(b.buffer).setUint16(33*2048, 1, true);},
 b => {new DataView(b.buffer).setUint32(33*2048+4, 99999, true);},
]) { const b = fixture(); mutate(b); await assert.rejects(async () => { const x = await openXiso(new Blob([b])); await x.list(x.root); }); }
console.log('XDVDFS synthetic boundary checks passed');
