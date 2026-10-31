import assert from 'node:assert/strict';
import {decodeMonoCue} from '../src/audio.mjs';
function fixture() {
 const a = new ArrayBuffer(140), v = new DataView(a);
 v.setUint16(72, 0x69, true); v.setUint16(74, 1, true); v.setUint32(76, 22050, true);
 v.setUint16(84, 36, true); v.setUint16(86, 4, true); v.setUint32(92, 104, true); v.setUint32(100, 36, true);
 v.setInt16(104, 100, true); v.setUint8(108, 0x07); return a;
}
let a=fixture(); let result=decodeMonoCue(a); assert.equal(result.sampleRate,22050); assert.equal(result.pcm.length,64); assert.equal(result.pcm[0],100); assert.equal(result.pcm[1],113); assert.equal(result.pcm[2],115);
for (const mutate of [v=>v.setUint16(74,2,true), v=>v.setUint32(100,37,true), v=>v.setUint16(106,89,true), v=>v.setUint32(92,0,true)]) {a=fixture(); mutate(new DataView(a)); assert.throws(()=>decodeMonoCue(a));}
console.log('Audio synthetic boundary checks passed');
