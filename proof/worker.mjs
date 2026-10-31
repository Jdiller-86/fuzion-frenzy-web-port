import {openXiso} from '../src/xdvdfs.mjs';
import {readXbpIndex, xbpResource} from '../src/xbp.mjs';
import {decodeMonoCue} from '../src/audio.mjs';
import {readStadiumRoot} from '../src/obe.mjs';
import {readCharacterBindPose} from '../src/character.mjs';
import {readCharacterAnimation} from '../src/animation.mjs';
import {decodeTexture} from '../src/texture.mjs';
import {sha256Blob,requireBlobIdentity} from '../src/identity.mjs';
let animation, profile=false, resumeCheckpoint;
async function checkpoint(stage) { if(!profile)return; postMessage({checkpoint:stage}); await new Promise(resolve=>{resumeCheckpoint=resolve;}); }
const supportedPackages = new Map([
  ['Packages/charactersel.xbp',[6164480,'c808846e407f86e777f8f3752e8c06915a158c11f4adf0608e5de3d8de4f43db']],
  ['Packages/English/Dub.xbp',[491520,'0afc216d1c156150effeb52ca6dcf784fae3a392da83b5d60260a4d78fd6e98d']],
  ['Packages/MetaStad.xbp',[4204544,'0412bff78d89298de2ed6f2d0fc9fd1ae1e63899f55a4204f368e6aed37ff737']],
  ['Packages/carryon_gen.xbp',[7778304,'f9e4f0b49ed1794c5d5268114af042d104f89441d6d25ca2568e814707767b13']]
]);
async function verifiedPackage(x,path) { const blob=x.file(await x.find(path)), expected=supportedPackages.get(path); if(!expected)throw new Error('Unsupported content package'); await requireBlobIdentity(blob,expected[0],expected[1],path==='Packages/carryon_gen.xbp'?async()=>checkpoint('Character package held for hashing'):undefined); return blob; }
const supported = '45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9';
onmessage = async ({data: image}) => {
  try {
    if (image && image.type === 'resume') { if(resumeCheckpoint){const resume=resumeCheckpoint;resumeCheckpoint=undefined;resume();} return; }
    if (image && image.type === 'sample') { if (!animation) throw new Error('No animation loaded'); const frame=animation.sample(image.time); postMessage({frame},[frame.buffer]); return; }
    profile=!!image.profile; image=image.file;
    const x = await openXiso(image), exe = x.file(await x.find('default.xbe'));
    if (exe.size !== 2834432) throw new Error('Unsupported executable');
    const hash = await sha256Blob(exe,2834432);
    if (hash !== supported) throw new Error('Unsupported game build');
    const pkg = await verifiedPackage(x,'Packages/charactersel.xbp');
    const entries = await readXbpIndex(pkg);
    const audioPackage = await verifiedPackage(x,'Packages/English/Dub.xbp');
    const cue = (await readXbpIndex(audioPackage)).find(e => e.name.toLowerCase() === 'dub/selected00.wav');
    if (!cue) throw new Error('Required cue not found');
    const {pcm, sampleRate} = decodeMonoCue(await xbpResource(audioPackage, cue).arrayBuffer());
    const arenaPackage = await verifiedPackage(x,'Packages/MetaStad.xbp');
    const arenaEntries=await readXbpIndex(arenaPackage);
    const arena = arenaEntries.find(e => e.name.toLowerCase() === 'stadiummesh.obe');
    if (!arena) throw new Error('Stadium candidate not found');
    const landscape=readStadiumRoot(await xbpResource(arenaPackage, arena).arrayBuffer());
    const wallEntry=arenaEntries.find(e=>e.name.toLowerCase()==='stadium_wall.obe'); if(!wallEntry)throw new Error('Stadium wall not found');
    const wall=readStadiumRoot(await xbpResource(arenaPackage,wallEntry).arrayBuffer());
    const meshes=[landscape,wall];
    const positions=new Float32Array(meshes.reduce((n,m)=>n+m.positions.length,0));
    const arenaUvs=new Float32Array(meshes.reduce((n,m)=>n+m.uvs.length,0));
    const indices=new Uint32Array(meshes.reduce((n,m)=>n+m.indices.length,0));
    const batches=[]; let po=0,uo=0,io=0;
    for(const mesh of meshes){positions.set(mesh.positions,po);arenaUvs.set(mesh.uvs,uo);for(let i=0;i<mesh.indices.length;i++)indices[io+i]=mesh.indices[i]+po/3;for(const batch of mesh.batches)batches.push({...batch,start:batch.start+io});po+=mesh.positions.length;uo+=mesh.uvs.length;io+=mesh.indices.length;}
    const textures=[], hashes=new Set(batches.map(x=>x.hash)); let textureBytes=0;
    if(hashes.size>64)throw new Error('Too many arena textures');
    for(const hash of hashes) { const matches=arenaEntries.filter(e=>e.hash===hash); if(matches.length!==1)throw new Error('Ambiguous arena texture'); const tex=decodeTexture(await xbpResource(arenaPackage,matches[0]).arrayBuffer()); textureBytes+=tex.rgba.byteLength; if(textureBytes>32*1024*1024)throw new Error('Arena texture budget exceeded'); textures.push({hash,...tex}); }
    const characterPackage = await verifiedPackage(x,'Packages/carryon_gen.xbp');
    const characterEntries = await readXbpIndex(characterPackage);
    const character = characterEntries.find(e => e.name.toLowerCase() === 'characters/dub.obe');
    if (!character) throw new Error('Character candidate not found');
    const characterData = await xbpResource(characterPackage, character).arrayBuffer();
    const bind = readCharacterBindPose(characterData);
    animation = readCharacterAnimation(characterData, bind.positions.slice());
    const texEntry=characterEntries.find(e=>e.name.toLowerCase()==='characters/newdub.bmp');
    if(!texEntry)throw new Error('Character texture candidate not found');
    const texture=decodeTexture(await xbpResource(characterPackage,texEntry).arrayBuffer());
    await checkpoint('Decoded assets held before rendering');
    postMessage({identity: hash, count: entries.length, pcm, rate: sampleRate, positions, indices, arenaUvs, batches, textures, characterPositions: bind.positions, characterIndices: bind.indices, characterUvs:bind.uvs, texture, bones: animation.bones}, [pcm.buffer, positions.buffer, indices.buffer, arenaUvs.buffer, ...textures.map(t=>t.rgba.buffer), bind.positions.buffer, bind.indices.buffer, bind.uvs.buffer, texture.rgba.buffer]);
  } catch (e) { postMessage({error: e instanceof Error ? e.message : 'Unknown error'}); }
};
