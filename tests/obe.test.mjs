import assert from 'node:assert/strict'; import {readStadiumRoot} from '../src/obe.mjs';
function fixture() {
 const b=new ArrayBuffer(1024),v=new DataView(b),u=(o,x)=>v.setUint32(o,x,true); u(0x40,0x80);u(0x160,0x80); for(let i=0;i<3;i++)v.setFloat32(0xa8+i*4,1,true);v.setUint8(0x79,1);v.setUint8(0x170,2);u(0xf0,3);u(0xf4,0x200);u(0xf8,1);u(0xfc,0x1c0);u(0x100,0x1d0);u(0x1c0,1);v.setUint16(0x1d0,5,true);v.setUint16(0x1d2,3,true);v.setUint16(0x1d4,1,true); for(let i=0;i<3;i++) v.setFloat32(0x210+i*44,i,true); return b;
}
const profile0=fixture();new DataView(profile0).setUint8(0x79,0);assert.equal(readStadiumRoot(profile0).positions.length,9);
const empty=fixture(),ev=new DataView(empty);ev.setUint32(0x160,0x300,true);ev.setUint32(0x3e0,0x80,true);ev.setUint8(0x3f0,2);for(let i=0;i<3;i++)ev.setFloat32(0x328+i*4,1,true);ev.setUint32(0x374,0x200,true);ev.setUint32(0x37c,0x200,true);ev.setUint32(0x380,0x200,true);assert.equal(readStadiumRoot(empty).nodes,2);
const invalidUv=fixture();new DataView(invalidUv).setFloat32(0x210+28,Infinity,true);assert.throws(()=>readStadiumRoot(invalidUv));const omitted=readStadiumRoot(invalidUv,{omitInvalidUV:true});assert.equal(omitted.indices.length,0);assert.equal(omitted.omittedTriangles,1);
const mesh=readStadiumRoot(fixture()); assert.equal(mesh.positions.length,9); assert.deepEqual([...mesh.indices],[0,1,2]);
for (const mutate of [v=>v.setUint32(0x40,1000,true),v=>v.setUint32(0xf0,999999,true),v=>v.setUint16(0x1d2,4,true),v=>v.setFloat32(0x210,NaN,true)]) {let b=fixture();mutate(new DataView(b));assert.throws(()=>readStadiumRoot(b));}
console.log('OBE root hypothesis synthetic checks passed');
