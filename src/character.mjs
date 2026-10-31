// Provisional bind-position reader for one soft-skin profile; no bone deformation yet.
export function readCharacterBindPose(buffer) {
  if (!(buffer instanceof ArrayBuffer) || buffer.byteLength < 128 || buffer.byteLength > 16*1024*1024) throw new Error('Invalid character resource');
  const v=new DataView(buffer), size=v.byteLength;
  const range=(o,n)=>{if(!Number.isSafeInteger(o)||!Number.isSafeInteger(n)||o<0||n<0||o>size||n>size-o)throw new Error('Character range outside resource');};
  const u=o=>{range(o,4);return v.getUint32(o,true);};
  const count=v.getUint16(0x20,true), groupsCount=v.getUint16(0x22,true), vertexOffset=u(0x24),groups=u(0x28),primitives=u(0x2c);
  if (!(u(0x44)&1) || v.getUint8(0x79)!==0 || !count || count>60000 || !groupsCount || groupsCount>1000) throw new Error('Unsupported character profile');
  range(vertexOffset,count*56);range(groups,groupsCount*16);
  let primitiveCount=0;for(let i=0;i<groupsCount;i++) primitiveCount+=u(groups+i*16);
  if(primitiveCount>100000)throw new Error('Too many character primitives');range(primitives,primitiveCount*56);
  const uvs=new Float32Array(count*2);for(let i=0;i<count;i++)for(let k=0;k<2;k++){let uv=v.getFloat32(vertexOffset+i*56+48+k*4,true);if(!Number.isFinite(uv)||Math.abs(uv)>1000)throw new Error('Invalid character UV');uvs[i*2+k]=uv;}
  const positions=new Float32Array(count*3);for(let i=0;i<count;i++)for(let k=0;k<3;k++){let x=v.getFloat32(vertexOffset+i*56+k*4,true);if(!Number.isFinite(x)||Math.abs(x)>1e7)throw new Error('Invalid character position');positions[i*3+k]=x;}
  let cursor=0,indices=[];for(let i=0;i<primitiveCount;i++){
    const p=primitives+i*56, mode=v.getUint16(p,true)&255,n=v.getUint16(p+2,true),triangles=v.getUint16(p+6,true);
    if(n<3||cursor+n>count)throw new Error('Invalid character primitive range');
    if(mode===5&&n%3===0&&triangles===n/3){for(let j=0;j<n;j+=3)indices.push(cursor+j,cursor+j+1,cursor+j+2);}
    else if(mode===6&&triangles===n-2){for(let j=0;j<n-2;j++)indices.push(cursor+j+(j%2),cursor+j+1-(j%2),cursor+j+2);}
    else throw new Error('Unsupported character primitive');cursor+=n;
  }
  if(cursor!==count)throw new Error('Unaccounted character vertices');
  return {positions,indices:new Uint32Array(indices),uvs};
}
