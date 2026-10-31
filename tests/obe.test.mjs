import assert from 'node:assert/strict'; import {readStadiumRoot} from '../src/obe.mjs';
function fixture() {
 const b=new ArrayBuffer(1024),v=new DataView(b),u=(o,x)=>v.setUint32(o,x,true); u(0x40,0x80);u(0x160,0x80); for(let i=0;i<3;i++)v.setFloat32(0xa8+i*4,1,true);v.setUint8(0x79,1);v.setUint8(0x170,2);u(0xf0,3);u(0xf4,0x200);u(0xf8,1);u(0xfc,0x1c0);u(0x100,0x1d0);u(0x104,1);u(0x1c0,1);v.setUint16(0x1d0,5,true);v.setUint16(0x1d2,3,true);v.setUint16(0x1d4,1,true); for(let i=0;i<3;i++) v.setFloat32(0x210+i*44,i,true); return b;
}
const profile0=fixture();new DataView(profile0).setUint8(0x79,0);assert.equal(readStadiumRoot(profile0).positions.length,9);
const empty=fixture(),ev=new DataView(empty);ev.setUint32(0x160,0x300,true);ev.setUint32(0x3e0,0x80,true);ev.setUint8(0x3f0,2);for(let i=0;i<3;i++)ev.setFloat32(0x328+i*4,1,true);ev.setUint32(0x374,0x200,true);ev.setUint32(0x37c,0x200,true);ev.setUint32(0x380,0x200,true);assert.equal(readStadiumRoot(empty).nodes,2);
const invalidUv=fixture();new DataView(invalidUv).setFloat32(0x210+28,Infinity,true);assert.throws(()=>readStadiumRoot(invalidUv));const omitted=readStadiumRoot(invalidUv,{omitInvalidUV:true});assert.equal(omitted.indices.length,0);assert.equal(omitted.omittedTriangles,1);
const mesh=readStadiumRoot(fixture()); assert.equal(mesh.positions.length,9); assert.deepEqual([...mesh.indices],[0,1,2]);
const colored=fixture(),cv=new DataView(colored);cv.setUint32(0x210+24,0x54312d1d,true);cv.setUint8(0x1d1,1);const cm=readStadiumRoot(colored);assert.deepEqual([...cm.colors.slice(0,4)],[49,45,29,84]);assert.equal(cm.batches[0].flags,1);
const mixed=fixture(),mv=new DataView(mixed);mv.setUint32(0xf0,6,true);mv.setUint32(0x1c0,2,true);mv.setUint16(0x1d8,0x105,true);mv.setUint16(0x1da,3,true);mv.setUint16(0x1dc,1,true);const mm=readStadiumRoot(mixed);assert.deepEqual(mm.batches.map(b=>[b.start,b.count,b.flags]),[[0,3,0],[3,3,1]]);assert.deepEqual([...mm.indices],[0,1,2,3,4,5]);
assert.equal(cm.batches[0].pass,0); // A primitive flag does not select a draw pass.
const transparent=fixture(),tv=new DataView(transparent);tv.setUint32(0x104,0,true);tv.setUint32(0x10c,1,true);assert.equal(readStadiumRoot(transparent).batches[0].pass,2);
tv.setUint32(0x104,1,true);assert.throws(()=>readStadiumRoot(transparent),/draw-pass/);
for (const mutate of [v=>v.setUint32(0x40,1000,true),v=>v.setUint32(0xf0,999999,true),v=>v.setUint16(0x1d2,4,true),v=>v.setFloat32(0x210,NaN,true)]) {let b=fixture();mutate(new DataView(b));assert.throws(()=>readStadiumRoot(b));}
console.log('OBE root hypothesis synthetic checks passed');
