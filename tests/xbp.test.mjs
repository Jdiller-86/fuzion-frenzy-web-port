import assert from 'node:assert/strict';
import { readXbpIndex, xbpResource } from '../src/xbp.mjs';
function fixture() {
  const bytes = new Uint8Array(512), d = new DataView(bytes.buffer);
  const put = (o, v) => d.setUint32(o, v, true);
  put(4, 16); put(12, 2); put(16, 8); put(40, 6); put(44, 6); put(48, 64);
  bytes.set(new TextEncoder().encode('a.bin\0'), 96);
  put(128, 16); put(136, 4); put(140, 0);
  put(160, 6); put(168, 6); put(172, 6);
  bytes.set([1, 2, 3, 4], 256);
  return bytes;
}
const source = fixture();
const index = await readXbpIndex(new Blob([source]));
assert.deepEqual(index, [{name: 'a.bin', offset: 256, size: 4, hash: 0}]);
assert.deepEqual([...new Uint8Array(await xbpResource(new Blob([source]), index[0]).arrayBuffer())], [1, 2, 3, 4]);
for (const mutate of [
  b => new DataView(b.buffer).setUint32(128, 1000, true),
  b => new DataView(b.buffer).setUint32(140, 100, true),
  b => { b[101] = 65; },
  b => { b.set(new TextEncoder().encode('../x\0'), 96); },
  b => new DataView(b.buffer).setUint32(48, 65, true),
]) { const b = fixture(); mutate(b); await assert.rejects(readXbpIndex(new Blob([b]))); }
await assert.rejects(readXbpIndex(new Blob([new Uint8Array(12)])));
// Shared name offsets must not turn a bounded string table into unbounded work/output.
const count=8193, big=new Uint8Array(8192+count*32), bd=new DataView(big.buffer);
bd.setUint32(4,1,true);bd.setUint32(12,count,true);bd.setUint32(16,8192,true);bd.setUint32(40,4096,true);bd.setUint32(44,1025,true);bd.setUint32(48,count*32,true);big.fill(97,4096,5120);big[5120]=0;
for(let i=0;i<count;i++){bd.setUint32(8192+i*32,64,true);bd.setUint32(8200+i*32,1,true);}
await assert.rejects(readXbpIndex(new Blob([big])),/name budget/);
const tooLong=big.slice();new DataView(tooLong.buffer).setUint32(12,1,true);new DataView(tooLong.buffer).setUint32(48,32,true);new DataView(tooLong.buffer).setUint32(44,1026,true);tooLong[5120]=97;tooLong[5121]=0;
await assert.rejects(readXbpIndex(new Blob([tooLong])),/name too long/);
console.log('XBP synthetic boundary checks passed');
