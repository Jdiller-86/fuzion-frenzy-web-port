// Experimental flat stadium OBE profile, not a general OBE decoder.
// See docs/REFERENCE_INVENTORY.md for static provenance and coverage limits.
export function readStadiumRoot(buffer, {omitInvalidUV=false} = {}) {
  if (!(buffer instanceof ArrayBuffer) || buffer.byteLength < 0x100 || buffer.byteLength > 16 * 1024 * 1024) throw new Error('Invalid OBE size');
  const v = new DataView(buffer), size = v.byteLength;
  const range = (o, n) => { if (!Number.isSafeInteger(o) || !Number.isSafeInteger(n) || o < 0 || n < 0 || o > size || n > size - o) throw new Error('OBE range outside resource'); };
  const u = o => { range(o,4); return v.getUint32(o,true); };
  const f = o => { range(o,4); const x=v.getFloat32(o,true); if(!Number.isFinite(x) || Math.abs(x)>1e7) throw new Error('Invalid OBE numeric field'); return x; };
  const root = u(0x40), profile=v.getUint8(0x79); if (profile>1 || !root) throw new Error('Unsupported OBE profile'); const stride=profile===0?36:44;
  let node=root, positions=[], uvs=[], colors=[], triangleFlags=[], invalidUV=[], indices=[], batches=[], nodes=0; const seen=new Set();
  do {
    range(node,256); if(seen.has(node) || ++nodes>4096) throw new Error('Invalid OBE node ring'); seen.add(node);
    if (v.getUint8(node + 0xf0) !== 2 || u(node + 0xec)!==0) throw new Error('Unsupported OBE node');
    const translation=[f(node+8),f(node+12),f(node+16)];
    const scale=[f(node+0x28),f(node+0x2c),f(node+0x30)];
    for(let k=0;k<3;k++) if(f(node+0x14+k*4)!==0 || Math.abs(scale[k]-1)>0.00001) throw new Error('Unsupported OBE transform');
    const vertices = u(node + 0x70), vertexBuffer = u(node + 0x74), groupCount = u(node + 0x78), groups = u(node + 0x7c), primitives = u(node + 0x80);
    if (vertices===0 && groupCount===0) { range(vertexBuffer,16); range(groups,0); range(primitives,0); node=u(node+0xe0); continue; }
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
    for (let i = 0; i < vertices; i++) { for (let k = 0; k < 3; k++) positions.push(f(vertexBuffer + 16 + i * stride + k * 4) * scale[k] + translation[k]); let bad=false, pair=[];for(let k=0;k<2;k++){const value=v.getFloat32(vertexBuffer+16+i*stride+28+k*4,true); if(!Number.isFinite(value)||Math.abs(value)>1e7)bad=true; pair.push(value);} if(bad&&!omitInvalidUV)throw new Error('Invalid OBE texture coordinates');invalidUV.push(bad);uvs.push(...(bad?[0,0]:pair)); }
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
  } while(node!==root);
  const split=[];
  for(const batch of batches){let start=batch.start;while(start<batch.start+batch.count){const flags=triangleFlags[start/3];let end=start+3;while(end<batch.start+batch.count&&triangleFlags[end/3]===flags)end+=3;split.push({hash:batch.hash,pass:batch.pass,start,count:end-start,flags});start=end;}}
  batches=split;
  let omittedTriangles=0; if(omitInvalidUV){let filtered=[],newBatches=[];for(const batch of batches){let start=filtered.length;for(let i=batch.start;i<batch.start+batch.count;i+=3){let a=indices[i],b=indices[i+1],c=indices[i+2];if(invalidUV[a]||invalidUV[b]||invalidUV[c])omittedTriangles++;else filtered.push(a,b,c);}newBatches.push({...batch,start,count:filtered.length-start});}indices=filtered;batches=newBatches;}
  return {positions: new Float32Array(positions), indices: new Uint32Array(indices), uvs: new Float32Array(uvs), colors:new Uint8Array(colors), batches, nodes, omittedTriangles};
}
