// Bounded one-weight sampling using serialized inverse binds and bone palette IDs.
// Static analysis and validation context: docs/REFERENCE_INVENTORY.md.
import {multiply,matrix} from './transforms.mjs';
export function readCharacterAnimation(buffer, positions){
 if(!(buffer instanceof ArrayBuffer)||buffer.byteLength<128||buffer.byteLength>16*1024*1024||!(positions instanceof Float32Array))throw new Error('Invalid animation resource');
 const v=new DataView(buffer),size=v.byteLength;const range=(o,n)=>{if(!Number.isSafeInteger(o)||!Number.isSafeInteger(n)||o<0||n<0||o>size||n>size-o)throw new Error('Animation range outside resource');};
 const u=o=>{range(o,4);return v.getUint32(o,true);},f=o=>{range(o,4);let x=v.getFloat32(o,true);if(!Number.isFinite(x)||Math.abs(x)>1e7)throw new Error('Invalid animation value');return x;};
 let root=u(0x40),vertices=v.getUint16(0x20,true),vertexOffset=u(0x24),expected=v.getUint8(0x78);if(vertices*3!==positions.length||!expected||expected>128)throw new Error('Unsupported skeleton');range(vertexOffset,vertices*56);
 const nodes=[],seen=new Set();function walk(head,parent){let node=head;do{range(node,256);if(seen.has(node)||nodes.length>=128||v.getUint8(node+0xf0)!==1)throw new Error('Invalid skeleton hierarchy');seen.add(node);let index=nodes.length;nodes.push({offset:node,parent,bone:u(node+0xb0)});let child=u(node+0xec);if(child)walk(child,index);node=u(node+0xe0);}while(node!==head);}
 walk(root,-1);if(nodes.length!==expected)throw new Error('Skeleton count mismatch');
 function track(base,quaternion,time){let count=quaternion?u(base+4):v.getUint16(base+2,true),ptr=u(base+(quaternion?8:4)),width=quaternion?4:3,stride=quaternion?10:8,offs=quaternion?0x10:8,scales=quaternion?0x20:0x14;if(!count||count>65536)throw new Error('Invalid animation key count');range(ptr,count*stride);let lo=0,hi=count-1;while(lo<hi){let mid=Math.ceil((lo+hi)/2);if(v.getUint16(ptr+mid*stride,true)<=time)lo=mid;else hi=mid-1;}let a=lo,b=Math.min(lo+1,count-1),at=v.getUint16(ptr+a*stride,true),bt=v.getUint16(ptr+b*stride,true);if(bt<at)throw new Error('Unsorted animation keys');let mix=bt===at?0:Math.max(0,Math.min(1,(time-at)/(bt-at))),values=[];for(let i=0;i<width;i++){let bias=f(base+offs+4*i),scale=f(base+scales+4*i),x=v.getUint16(ptr+a*stride+2+2*i,true)*scale+bias,y=v.getUint16(ptr+b*stride+2+2*i,true)*scale+bias;values.push([x,y]);}if(quaternion){let dot=values.reduce((sum,p)=>sum+p[0]*p[1],0);if(dot<0){for(let p of values)p[1]=-p[1];dot=-dot;} dot=Math.min(1,dot); if(dot<0.9995){let angle=Math.acos(dot),den=Math.sin(angle),a=Math.sin((1-mix)*angle)/den,b=Math.sin(mix*angle)/den;return values.map(([x,y])=>x*a+y*b);}}return values.map(([x,y])=>x+(y-x)*mix);}
 // The runtime propagates rotation/translation separately from each node's scale.
 const identityScale=[1,1,1],paletteSeen=new Set();
 const binds=nodes.map(n=>{
   if(n.bone>=nodes.length||paletteSeen.has(n.bone))throw new Error('Invalid bone palette index');paletteSeen.add(n.bone);
   const bind=Float64Array.from({length:16},(_,i)=>f(n.offset+0x70+(i%4*4+Math.floor(i/4))*4));
   if(Math.abs(bind[12])+Math.abs(bind[13])+Math.abs(bind[14])+Math.abs(bind[15]-1)>1e-5)throw new Error('Invalid inverse bind matrix');
   return bind;
 });
 function globals(time){const out=[],parents=[],scales=[];for(const n of nodes){let o=n.offset,t=track(o,false,time),q=track(o+0x40,true,time),s=track(o+0x20,false,time);if(n.parent>=0)t=t.map((x,i)=>x*scales[n.parent][i]);let local=matrix(t,q,s),parent=matrix(t,q,identityScale);out.push(n.parent<0?local:multiply(parents[n.parent],local));parents.push(n.parent<0?parent:multiply(parents[n.parent],parent));scales.push(s);}return out;}
 const bones=new Uint16Array(vertices);for(let i=0;i<vertices;i++){let p=vertexOffset+i*56;let w0=f(p+12),w1=f(p+16),w2=f(p+20),code=v.getInt16(p+24,true),index=(code+96)/3-(code>=-36?1:0);if(Math.abs(w0-1)>1e-5||w1!==0||w2!==0||!Number.isInteger(index)||(code>-42&&code<-36)||!paletteSeen.has(index))throw new Error('Unsupported vertex weights');bones[i]=index;}
 return {bones:nodes.length, sample(time){if(!Number.isFinite(time)||time<0||time>65535)throw new Error('Invalid animation time');let curr=globals(time),delta=[];curr.forEach((m,i)=>{delta[nodes[i].bone]=multiply(m,binds[i]);});const out=new Float32Array(positions.length);for(let i=0;i<vertices;i++){let m=delta[bones[i]],x=positions[i*3],y=positions[i*3+1],z=positions[i*3+2];out[i*3]=m[0]*x+m[1]*y+m[2]*z+m[3];out[i*3+1]=m[4]*x+m[5]*y+m[6]*z+m[7];out[i*3+2]=m[8]*x+m[9]*y+m[10]*z+m[11];}return out;}};
}
