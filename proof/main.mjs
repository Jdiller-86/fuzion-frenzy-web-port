import {renderGeometry} from './preview.mjs';
import {selectionIdleId,clipPreviewRate,clipPreviewTime} from '../src/clips.mjs';
const input = document.querySelector('#image'), status = document.querySelector('#status'), result = document.querySelector('#result'), play = document.querySelector('#play');
let decoded, drawArena, drawCharacter, activeAudio, animating = false, pending = false;
function stopAudio(){const context=activeAudio;activeAudio=undefined;if(context&&context.state!=='closed')void context.close();}
function disposePreviews(){drawArena?.dispose();drawCharacter?.dispose();drawArena=undefined;drawCharacter=undefined;}
const memory = document.querySelector('#memory'), cancel = document.querySelector('#cancel'), repeat=document.querySelector('#repeat'), runs=document.querySelector('#runs'), profileOption=document.querySelector('#profile'), profileResult=document.querySelector('#profile-result');
let lastFile, timings=[];
let worker, revision=0, animationToken=0;
const clip=document.querySelector('#clip'),clear=document.querySelector('#clear');
const clipPosition=document.querySelector('#clip-position'),clipKey=document.querySelector('#clip-key');
function pauseMotion(){animating=false;animationToken++;document.querySelector('#animate').textContent='Play motion';}
function samplePosition(){
  const range=decoded?.clips[clip.selectedIndex];if(!range||!worker)return;
  const time=range.start+(range.end-range.start)*Number(clipPosition.value)/1000;
  clipKey.textContent=`Imported key ${time.toFixed(3)}`;
  worker.postMessage({type:'sample',time});
}
clipPosition.addEventListener('input',()=>{pauseMotion();samplePosition();});
function describeClip(){const range=decoded?.clips[clip.selectedIndex];document.querySelector('#clip-rate').textContent=range?`${clipPreviewRate(range)} key units per second (${range.id===selectionIdleId?'static character-selection idle profile; runtime comparison pending':'diagnostic rate; original speed unknown'}).`:'';}
clip.addEventListener('change',()=>{pauseMotion();clipPosition.value='0';describeClip();samplePosition();});
input.addEventListener('click', () => { input.value=''; });
input.addEventListener('change', () => { const file=input.files[0]; if(!file)return; lastFile=file; timings=[]; runs.textContent=''; startImport(file); });
repeat.addEventListener('click',()=>{if(lastFile)startImport(lastFile);});
function startImport(file) {
  clear.disabled=false;stopAudio();
  clip.disabled=true;clip.replaceChildren();clipPosition.disabled=true;clipPosition.value='0';clipKey.textContent='';document.querySelector('#clip-rate').textContent='';
  worker?.terminate(); disposePreviews(); animationToken++; const current=++revision; repeat.disabled=true; const profiling=profileOption.checked, surroundings=document.querySelector('#surroundings').checked, cull=document.querySelector('#cull').value, reverseDepth=document.querySelector('#reverse-depth').checked, levelDirection=document.querySelector('#level-direction').checked, materialProfile=document.querySelector('#material-profile').checked; const profileSamples=[]; profileResult.textContent=profiling?'Memory stage sampling in progress.':''; cancel.disabled=false; memory.textContent='Browser memory measurement pending.'; animating=false; for(const id of ['preview','character']) { const canvas=document.getElementById(id); const replacement=canvas.cloneNode(false); canvas.replaceWith(replacement); } pending=false; result.textContent = ''; decoded = undefined; play.disabled = true; document.querySelector('#animate').disabled=true; document.querySelector('#animate').textContent='Play motion';
  worker = new Worker('/proof/worker.mjs', {type: 'module'});
  const start = performance.now(); status.textContent = 'Checking local file...';
  worker.onmessage = async ({data}) => {
    if(current!==revision)return;
    if (data.checkpoint) {
      let detail='unavailable';
      if(typeof performance.measureUserAgentSpecificMemory==='function') { try { const reading=await Promise.race([performance.measureUserAgentSpecificMemory(),new Promise((_,reject)=>setTimeout(()=>reject(new Error('timeout')),30000))]); detail=`${(reading.bytes/1048576).toFixed(1)} MiB`; } catch {} }
      if(current!==revision)return; profileSamples.push(`${data.checkpoint}: ${detail}`); profileResult.textContent=`Paused-stage page and worker snapshots: ${profileSamples.join('; ')}. These are samples, not a peak; GPU memory may be excluded.`; worker.postMessage({type:'resume'}); return;
    }
    if (data.frame) { drawCharacter(data.frame); pending=false; return; }
    if (data.error) { repeat.disabled=!lastFile; cancel.disabled=true; animating=false; memory.textContent='No import measured.'; status.textContent = `Unable to inspect this file: ${data.error}`; return; }
    data.clips.forEach((range,i)=>{const option=document.createElement('option');option.value=String(i);option.textContent=`${i+1}: keys ${range.start}–${range.end}`;clip.append(option);});
    const idleIndex=data.clips.findIndex(range=>range.id===selectionIdleId);
    clip.selectedIndex=idleIndex>=0?idleIndex:data.clips.reduce((best,range,i)=>range.start<data.clips[best].start?i:best,0);
    clip.disabled=false;
    try { drawArena=renderGeometry(document.querySelector('#preview'), data.positions, data.indices, data.textures, data.arenaUvs, data.batches, {frameVertices:levelDirection?data.landscapeVertices:data.frameVertices,cull,reverseDepth:reverseDepth||levelDirection,levelDirection,colors:materialProfile?data.arenaColors:null}); drawCharacter = renderGeometry(document.querySelector('#character'), data.characterPositions, data.characterIndices, data.texture, data.characterUvs); document.querySelector('#animate').disabled=false; decoded=data; play.disabled=false; } catch (e) { disposePreviews(); repeat.disabled=!lastFile; cancel.disabled=true; memory.textContent='No complete import measured.'; status.textContent = `Preview failed: ${e.message}`; return; }
    cancel.disabled=true; status.textContent = 'Local index, audio and geometry decoded; visual and animation fidelity remain unverified.';
    describeClip();
    clipPosition.disabled=false;samplePosition();
    const elapsed=(performance.now()-start)/1000;
    if(!profiling&&!surroundings&&cull==='none'&&!reverseDepth&&!levelDirection&&!materialProfile) { timings.push(elapsed); if(timings.length>20)timings.shift(); runs.textContent=`In-page timings (first: cache unknown; repeats: likely warm): ${timings.map(x=>x.toFixed(3)).join(', ')} seconds`; }
    repeat.disabled=false;
    result.textContent = `Executable identity: ${data.identity}\nNamed package records: ${data.count}\nPreview geometry: ${data.positions.length / 3} vertices, ${data.indices.length / 3} triangles${surroundings?` (diagnostic surroundings; ${data.omittedTriangles} triangles omitted)`:''}\nCharacter: ${data.bones} bones, ${data.characterPositions.length / 3} vertices, ${data.characterIndices.length / 3} triangles\nDecoded cue: ${data.pcm.length} samples at ${data.rate} Hz\n${profiling?'Diagnostic elapsed including measurement pauses':surroundings||cull!=='none'||reverseDepth||levelDirection||materialProfile?'Diagnostic import and initial render time':'Import and initial render time'}: ${elapsed.toFixed(2)} seconds`;
    if (typeof performance.measureUserAgentSpecificMemory === 'function') { try { const measurement=await performance.measureUserAgentSpecificMemory(); if(current===revision) memory.textContent=`Browser-reported page and worker memory: ${(measurement.bytes/1048576).toFixed(1)} MiB (snapshot; GPU allocation may be excluded).`; } catch { if(current===revision) memory.textContent='Browser memory measurement unavailable.'; } } else if(current===revision) memory.textContent='Browser memory measurement unavailable.';
  };
  worker.onerror = () => { repeat.disabled=!lastFile; cancel.disabled=true; memory.textContent='No import measured.'; status.textContent = 'Inspection failed.'; };
  worker.postMessage({file,profile:profiling,surroundings});
}

function resetPreview(message) {
  stopAudio();
  worker?.terminate(); disposePreviews(); worker=undefined; repeat.disabled=!lastFile; revision++; animationToken++; animating=false; pending=false; cancel.disabled=true; play.disabled=true; document.querySelector('#animate').disabled=true;
  for(const id of ['preview','character']) { const canvas=document.getElementById(id); canvas.replaceWith(canvas.cloneNode(false)); }
  decoded=undefined; result.textContent=''; profileResult.textContent=''; memory.textContent='No import measured.'; status.textContent=message;
  clip.disabled=true;clip.replaceChildren();clipPosition.disabled=true;clipPosition.value='0';clipKey.textContent='';document.querySelector('#clip-rate').textContent='';document.querySelector('#animate').textContent='Play motion';
}
cancel.addEventListener('click',()=>resetPreview('Import cancelled.'));
clear.addEventListener('click',()=>{resetPreview('Local preview cleared. Original files remain on disk.');lastFile=undefined;input.value='';timings=[];runs.textContent='';repeat.disabled=true;clear.disabled=true;});

play.addEventListener('click', async () => {
  if (!decoded) return;
  stopAudio();
  const context = new AudioContext();activeAudio=context;
  const clip = context.createBuffer(1, decoded.pcm.length, decoded.rate);
  const samples = clip.getChannelData(0);
  for (let i = 0; i < samples.length; i++) samples[i] = decoded.pcm[i] / 32768;
  const source = context.createBufferSource(); source.buffer = clip; source.connect(context.destination);
  const audioRevision=revision;
  source.onended = () => {if(audioRevision===revision&&activeAudio===context)status.textContent=`Decoded cue playback completed: ${clip.duration.toFixed(3)} seconds; audio context ${context.state}. Audible quality is not established by this diagnostic.`;if(context.state!=='closed')void context.close();if(activeAudio===context)activeAudio=undefined;};
  try { await context.resume(); if(audioRevision!==revision||activeAudio!==context)return; source.start(); status.textContent = 'Playback started in the browser.'; } catch { if(audioRevision===revision)status.textContent = 'Audio playback could not start.'; if(context.state!=='closed')await context.close(); }
});

document.querySelector('#animate').addEventListener('click', () => {
  animating=!animating; const token=++animationToken; document.querySelector('#animate').textContent=animating?'Pause motion':'Play motion';
  if(!animating) return; const started=performance.now(),range=decoded.clips[clip.selectedIndex],offsetMs=(range.end-range.start)*Number(clipPosition.value)/1000/clipPreviewRate(range)*1000;
  // The current frame's rAF timestamp can predate this event handler.
  function tick(now) { if(!animating || token!==animationToken)return; if(!pending){const time=clipPreviewTime(range,offsetMs+Math.max(0,now-started));clipPosition.value=String(Math.min(1000,(time-range.start)/(range.end-range.start)*1000));clipKey.textContent=`Imported key ${time.toFixed(3)}`;pending=true;worker.postMessage({type:'sample',time});} requestAnimationFrame(tick); }
  requestAnimationFrame(tick);
});
