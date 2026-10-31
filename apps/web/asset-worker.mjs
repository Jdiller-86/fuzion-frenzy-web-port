import {openXiso} from '../../src/xdvdfs.mjs';
import {readXbpIndex,xbpResource} from '../../src/xbp.mjs';
import {requireBlobIdentity} from '../../src/identity.mjs';
import {readStadiumRoot} from '../../src/obe.mjs';
import {readCharacterBindPose} from '../../src/character.mjs';
import {readCharacterAnimation} from '../../src/animation.mjs';
import {readAnimationClips} from '../../src/clips.mjs';
import {decodeTexture} from '../../src/texture.mjs';
let animation;
onmessage=async({data})=>{
  try{
    if(data.type==='pose'){
      if(!animation)throw new Error('No local character');
      const poses=data.times.slice(0,4).map(t=>animation.sample(t));
      postMessage({type:'poses',poses},poses.map(p=>p.buffer));return;
    }
    const progress=text=>postMessage({type:'progress',text});
    progress('Checking compatible local build');
    const x=await openXiso(data.file);
    await requireBlobIdentity(x.file(await x.find('default.xbe')),2834432,'45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9');
    const pkg=x.file(await x.find('Packages/hop-v4.xbp'));
    await requireBlobIdentity(pkg,17053696,'c3875b31803a92172a06e889351d609246aa03dc4fa99355a1c9fe9a82b013d6',undefined,18*1024*1024);
    progress('Converting local reactor, rods and character');
    const entries=await readXbpIndex(pkg);
    const resource=async name=>{const e=entries.find(e=>e.name.toLowerCase()===name);if(!e)throw new Error('Required local resource missing');return xbpResource(pkg,e).arrayBuffer();};
    const meshes={};
    for(const [key,name] of Object.entries({drill:'v4/drill.obe',low:'v4/drillob01.obe',high:'v4/drillob02.obe',universe:'v4/universe.obe',world:'v4/worldadd.obe'}))meshes[key]=readStadiumRoot(await resource(name),{hierarchical:true});
    const hashes=new Set(Object.values(meshes).flatMap(m=>m.batches.filter(b=>b.count).map(b=>b.hash)));
    if(hashes.size>64)throw new Error('Texture count limit');
    const textures=[];let bytes=0;
    for(const hash of hashes){
      // Hash zero is explicitly rendered as untextured diffuse, never a missing-art fallback.
      if(hash===0)continue;
      const matches=entries.filter(e=>e.hash===hash);if(matches.length!==1)throw new Error('Unresolved reactor material');
      const texture=decodeTexture(await xbpResource(pkg,matches[0]).arrayBuffer());
      bytes+=texture.rgba.byteLength;if(bytes>32*1024*1024)throw new Error('Texture budget exceeded');textures.push({hash,...texture});
    }
    const characterData=await resource('characters/dub/dub.obe');
    const character=readCharacterBindPose(characterData);animation=readCharacterAnimation(characterData,character.positions);
    const clips=readAnimationClips(characterData),characterTexture=decodeTexture(await resource('characters/dub/tex/newdub.bmp'));
    progress('Local conversion complete');
    // No file handle, original package or executable is returned to the main thread.
    postMessage({type:'assets',meshes,textures,character,characterTexture,clips,textureBytes:bytes+characterTexture.rgba.byteLength});
  }catch(error){postMessage({type:'error',message:error instanceof Error?error.message:'Local conversion failed'});}
};
