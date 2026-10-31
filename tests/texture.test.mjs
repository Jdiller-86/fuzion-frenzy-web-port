import assert from 'node:assert/strict';import {decodeTexture} from '../src/texture.mjs';
function fixture(){let b=new ArrayBuffer(264),v=new DataView(b);v.setUint16(0x2c,3,true);v.setUint16(0x2e,2,true);v.setUint16(0x30,2,true);v.setUint8(0x37,1);v.setUint16(256,0xfc00,true);v.setUint16(258,0x83e0,true);v.setUint16(260,0x801f,true);return b;}
let a=decodeTexture(fixture());assert.deepEqual([...a.rgba.slice(0,12)],[255,0,0,255,0,255,0,255,0,0,255,255]);let b=fixture();new DataView(b).setUint16(0x2e,4,true);assert.throws(()=>decodeTexture(b));
// 2x4 rectangle checks y's extra swizzle bit and palette ARGB conversion.
b=new ArrayBuffer(1288);let v=new DataView(b);v.setUint16(0x2c,5,true);v.setUint16(0x2e,2,true);v.setUint16(0x30,4,true);v.setUint8(0x37,1);for(let i=0;i<8;i++){v.setUint32(256+i*4,0xff000000|(i<<16),true);v.setUint8(1280+i,i);}a=decodeTexture(b);assert.deepEqual(Array.from({length:8},(_,i)=>a.rgba[4*i]),[0,1,2,3,4,5,6,7]);
b=new ArrayBuffer(272);v=new DataView(b);v.setUint16(0x2c,1,true);v.setUint16(0x2e,2,true);v.setUint16(0x30,2,true);v.setUint8(0x37,1);v.setUint32(256,0x80402010,true);a=decodeTexture(b);assert.deepEqual([...a.rgba.slice(0,4)],[64,32,16,128]);
console.log('Texture synthetic checks passed');
