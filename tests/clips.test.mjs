import assert from 'node:assert/strict';
import {readAnimationClips,selectionIdleId,clipPreviewRate,clipPreviewTime} from '../src/clips.mjs';
function fixture(){const b=new ArrayBuffer(192),v=new DataView(b);v.setUint32(0x54,128,true);v.setUint32(0x58,2,true);for(let i=0;i<2;i++){v.setUint32(128+i*32,i+1,true);v.setInt32(132+i*32,(1+i*20)*1024,true);v.setInt32(136+i*32,(11+i*20)*1024,true);v.setInt32(140+i*32,17,true);}return b;}
assert.deepEqual(readAnimationClips(fixture()),[{id:1,start:1,end:11,rawStep:17},{id:2,start:21,end:31,rawStep:17}]);
for(const mutate of [v=>v.setUint32(0x58,513,true),v=>v.setUint32(0x54,180,true),v=>v.setUint32(0x54,0,true),v=>v.setInt32(132,-1,true),v=>v.setInt32(136,1024,true),v=>v.setInt32(136,65536*1024,true)]){const b=fixture();mutate(new DataView(b));assert.throws(()=>readAnimationClips(b));}
assert.throws(()=>readAnimationClips(fixture().slice(0,191)));
const idle={id:selectionIdleId,start:667,end:727};assert.equal(clipPreviewRate(idle),60);assert.equal(clipPreviewTime(idle,0),667);assert.equal(clipPreviewTime(idle,500),697);const period=(60+1/1024)/60*1000;assert.ok(Math.abs(clipPreviewTime(idle,period)-667)<1e-8);assert.equal(clipPreviewRate({id:1}),30);assert.throws(()=>clipPreviewTime(idle,-1));assert.throws(()=>clipPreviewTime(idle,NaN));
console.log('Animation clip metadata boundary checks passed');
