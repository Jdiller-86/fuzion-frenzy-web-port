import assert from 'node:assert/strict';
import {openXiso} from '../host/xdvdfs.mjs';
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
assert.throws(()=>fs.file({offset:NaN,size:1,directory:false}));
assert.throws(()=>fs.file({offset:0,size:Infinity,directory:false}));
await assert.rejects(fs.list({offset:-1,size:0,directory:true}));
const empty = fixture(), emptyView = new DataView(empty.buffer);
emptyView.setUint32(32*2048+24, 2048, true);
empty.fill(0xff, 33*2048, 34*2048);
const emptyFs = await openXiso(new Blob([empty]));
assert.deepEqual(await emptyFs.list(emptyFs.root), []);
await assert.rejects(emptyFs.find('missing.bin'), /path not found/);
await assert.rejects(emptyFs.list({offset:empty.length-1024,size:2048,directory:true}));
const mixed = empty.slice(); mixed[33*2048+14] = 0;
const mixedFs = await openXiso(new Blob([mixed]));
await assert.rejects(mixedFs.list(mixedFs.root));
const truncated = empty.slice();
new DataView(truncated.buffer).setUint32(32*2048+24, 2, true);
const truncatedFs = await openXiso(new Blob([truncated]));
await assert.rejects(truncatedFs.list(truncatedFs.root));
for (const mutate of [
 b => {b[32*2048] = 0;},
 b => {new DataView(b.buffer).setUint32(32*2048+20, 99999, true);},
 b => {new DataView(b.buffer).setUint16(33*2048, 1, true);},
 b => {new DataView(b.buffer).setUint32(33*2048+4, 99999, true);},
]) { const b = fixture(); mutate(b); await assert.rejects(async () => { const x = await openXiso(new Blob([b])); await x.list(x.root); }); }
console.log('XDVDFS synthetic boundary checks passed');
