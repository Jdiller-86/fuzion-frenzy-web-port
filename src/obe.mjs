import {matrix,multiply} from './transforms.mjs';
// Bounded stadium OBE profiles: flat meshes and opt-in initial-key hierarchy.
// See docs/REFERENCE_INVENTORY.md for static provenance and coverage limits.
export function readStadiumRoot(buffer, {omitInvalidUV=false,hierarchical=false} = {}) {
  if (!(buffer instanceof ArrayBuffer) || buffer.byteLength < 0x100 || buffer.byteLength > 16 * 1024 * 1024) throw new Error('Invalid OBE size');
  const v = new DataView(buffer), size = v.byteLength;
  const range = (o, n) => { if (!Number.isSafeInteger(o) || !Number.isSafeInteger(n) || o < 0 || n < 0 || o > size || n > size - o) throw new Error('OBE range outside resource'); };
  const u = o => { range(o,4); return v.getUint32(o,true); };
  const f = o => { range(o,4); const x=v.getFloat32(o,true); if(!Number.isFinite(x) || Math.abs(x)>1e7) throw new Error('Invalid OBE numeric field'); return x; };
  const root = u(0x40), profile=v.getUint8(0x79); if (profile>1 || !root) throw new Error('Unsupported OBE profile'); const stride=profile===0?36:44;
  let positions=[], uvs=[], colors=[], triangleFlags=[], invalidUV=[], indices=[], batches=[], nodes=0; const seen=new Set();
  function firstTrack(base,quaternion){
    const count=quaternion?u(base+4):v.getUint16(base+2,true),ptr=u(base+(quaternion?8:4)),stride=quaternion?10:8,width=quaternion?4:3,bias=quaternion?0x10:8,scale=quaternion?0x20:0x14;
    if(!count||count>65536)throw new Error('Invalid OBE transform track');range(ptr,count*stride);
    if(v.getUint16(ptr,true)!==0)throw new Error('Unsupported OBE initial key');
    return Array.from({length:width},(_,i)=>f(base+bias+i*4)+v.getUint16(ptr+2+i*2,true)*f(base+scale+i*4));
  }
  function walk(head,parent=null,parentScale=[1,1,1]){let node=head;do {
    range(node,256); if(seen.has(node) || ++nodes>4096) throw new Error('Invalid OBE node ring'); seen.add(node);
    if (v.getUint8(node + 0xf0) !== 2 || (!hierarchical&&u(node + 0xec)!==0)) throw new Error('Unsupported OBE node');
    const translation=hierarchical?firstTrack(node,false):[f(node+8),f(node+12),f(node+16)];
    const scale=hierarchical?firstTrack(node+0x20,false):[f(node+0x28),f(node+0x2c),f(node+0x30)];
    for(let k=0;k<3;k++) if(!hierarchical&&(f(node+0x14+k*4)!==0 || Math.abs(scale[k]-1)>0.00001)) throw new Error('Unsupported OBE transform');
    const vertices = u(node + 0x70), vertexBuffer = u(node + 0x74), groupCount = u(node + 0x78), groups = u(node + 0x7c), primitives = u(node + 0x80);
    if (vertices===0 && groupCount===0) { if(hierarchical&&u(node+0xec))throw new Error('Unsupported empty hierarchy container'); range(vertexBuffer,16); range(groups,0); range(primitives,0); node=u(node+0xe0); continue; }
    const quaternionOffset=u(node+0x48);
    if(!hierarchical&&u(node+0x44)!==1)throw new Error('Unsupported animated flat-node rotation');
    range(quaternionOffset,10);
    const quaternion=hierarchical?firstTrack(node+0x40,true):Array.from({length:4},(_,i)=>f(node+0x50+i*4)+v.getUint16(quaternionOffset+2+i*2,true)*f(node+0x60+i*4));
    const adjustedTranslation=translation.map((x,i)=>x*parentScale[i]);
    const local=matrix(adjustedTranslation,quaternion,scale),unscaled=matrix(adjustedTranslation,quaternion,[1,1,1]);
    const transform=parent?multiply(parent,local):local,childParent=parent?multiply(parent,unscaled):unscaled;
    if(u(node+0xec))walk(u(node+0xec),childParent,scale);
    if (!vertices || vertices > 200000 || positions.length/3+vertices>1000000 || !groupCount || groupCount > 10000) throw new Error('Unsupported OBE dimensions');
    const passCounts=[u(node+0x84),u(node+0x88),u(node+0x8c)];
    if(passCounts.reduce((n,x)=>n+x,0)!==groupCount)throw new Error('Invalid OBE draw-pass groups');
    const passForGroup=index=>index<passCounts[0]?0:index<passCounts[0]+passCounts[1]?1:2;
    range(vertexBuffer, 16 + vertices * stride); range(groups, groupCount * 16);
    let primitiveCount = 0; for (let i = 0; i < groupCount; i++) primitiveCount += u(groups + i * 16);
    if (primitiveCount > 200000) throw new Error('Too many OBE primitives');
    range(primitives, primitiveCount * 8);
    const base=positions.length/3;
    for(let i=0;i<vertices;i++){const color=u(vertexBuffer+16+i*stride+24);colors.push((color>>>16)&255,(color>>>8)&255,color&255,color>>>24);}
    for (let i = 0; i < vertices; i++) { const point=[0,1,2].map(k=>f(vertexBuffer+16+i*stride+k*4)); for(let k=0;k<3;k++)positions.push(transform[k*4]*point[0]+transform[k*4+1]*point[1]+transform[k*4+2]*point[2]+transform[k*4+3]); let bad=false, pair=[];for(let k=0;k<2;k++){const value=v.getFloat32(vertexBuffer+16+i*stride+28+k*4,true); if(!Number.isFinite(value)||Math.abs(value)>1e7)bad=true; pair.push(value);} if(bad&&!omitInvalidUV)throw new Error('Invalid OBE texture coordinates');invalidUV.push(bad);uvs.push(...(bad?[0,0]:pair)); }
    let cursor = 0, group = 0, groupEnd = u(groups), batchStart=indices.length;
    for (let i = 0; i < primitiveCount; i++) {
      while(i===groupEnd && group<groupCount) { batches.push({hash:u(groups+group*16+4), pass:passForGroup(group), start:batchStart, count:indices.length-batchStart}); batchStart=indices.length; group++; if(group<groupCount) groupEnd+=u(groups+group*16); }
      const p = primitives + i * 8, mode = v.getUint16(p, true) & 0xff, n = v.getUint16(p + 2, true), triangles = v.getUint16(p + 4, true);
      if (n < 3 || cursor + n > vertices) throw new Error('Invalid OBE primitive range');
      const before=indices.length;
      if (mode === 5 && n % 3 === 0 && triangles === n / 3) {
        for (let j = 0; j < n; j += 3) indices.push(base + cursor + j, base + cursor + j + 1, base + cursor + j + 2);
      } else if (mode === 6 && triangles === n - 2) {
        for (let j = 0; j < n - 2; j++) indices.push(base + cursor + j + (j % 2), base + cursor + j + 1 - (j % 2), base + cursor + j + 2);
      } else throw new Error('Unsupported OBE primitive');
      for(let j=before;j<indices.length;j+=3)triangleFlags.push(v.getUint8(p+1));
      cursor += n;
    }
    while(group<groupCount) { if(groupEnd!==primitiveCount) throw new Error('Invalid OBE material groups'); batches.push({hash:u(groups+group*16+4), pass:passForGroup(group), start:batchStart, count:indices.length-batchStart}); batchStart=indices.length; group++; if(group<groupCount) groupEnd+=u(groups+group*16); }
    if (cursor !== vertices) throw new Error('Unaccounted OBE vertices');
    node=u(node+0xe0);
  } while(node!==head);}
  walk(root);
  const split=[];
  for(const batch of batches){let start=batch.start;while(start<batch.start+batch.count){const flags=triangleFlags[start/3];let end=start+3;while(end<batch.start+batch.count&&triangleFlags[end/3]===flags)end+=3;split.push({hash:batch.hash,pass:batch.pass,start,count:end-start,flags});start=end;}}
  batches=split;
  let omittedTriangles=0; if(omitInvalidUV){let filtered=[],newBatches=[];for(const batch of batches){let start=filtered.length;for(let i=batch.start;i<batch.start+batch.count;i+=3){let a=indices[i],b=indices[i+1],c=indices[i+2];if(invalidUV[a]||invalidUV[b]||invalidUV[c])omittedTriangles++;else filtered.push(a,b,c);}newBatches.push({...batch,start,count:filtered.length-start});}indices=filtered;batches=newBatches;}
  return {positions: new Float32Array(positions), indices: new Uint32Array(indices), uvs: new Float32Array(uvs), colors:new Uint8Array(colors), batches, nodes, omittedTriangles};
}
