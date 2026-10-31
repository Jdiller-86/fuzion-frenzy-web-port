import {renderGeometry} from './preview.mjs';
const input = document.querySelector('#image'), status = document.querySelector('#status'), result = document.querySelector('#result'), play = document.querySelector('#play');
let decoded, drawArena, drawCharacter, animating = false, pending = false;
function disposePreviews(){drawArena?.dispose();drawCharacter?.dispose();drawArena=undefined;drawCharacter=undefined;}
const memory = document.querySelector('#memory'), cancel = document.querySelector('#cancel'), repeat=document.querySelector('#repeat'), runs=document.querySelector('#runs'), profileOption=document.querySelector('#profile'), profileResult=document.querySelector('#profile-result');
let lastFile, timings=[];
let worker, revision=0, animationToken=0;
input.addEventListener('click', () => { input.value=''; });
input.addEventListener('change', () => { const file=input.files[0]; if(!file)return; lastFile=file; timings=[]; runs.textContent=''; startImport(file); });
repeat.addEventListener('click',()=>{if(lastFile)startImport(lastFile);});
function startImport(file) {
  worker?.terminate(); disposePreviews(); animationToken++; const current=++revision; repeat.disabled=true; const profiling=profileOption.checked; const profileSamples=[]; profileResult.textContent=profiling?'Memory stage sampling in progress.':''; cancel.disabled=false; memory.textContent='Browser memory measurement pending.'; animating=false; for(const id of ['preview','character']) { const canvas=document.getElementById(id); const replacement=canvas.cloneNode(false); canvas.replaceWith(replacement); } pending=false; result.textContent = ''; decoded = undefined; play.disabled = true; document.querySelector('#animate').disabled=true; document.querySelector('#animate').textContent='Play motion';
  worker = new Worker('/proof/worker.mjs', {type: 'module'});
  const start = performance.now(); status.textContent = 'Checking local file...';
  worker.onmessage = async ({data}) => {
    if (data.checkpoint) {
      let detail='unavailable';
      if(typeof performance.measureUserAgentSpecificMemory==='function') { try { const reading=await Promise.race([performance.measureUserAgentSpecificMemory(),new Promise((_,reject)=>setTimeout(()=>reject(new Error('timeout')),30000))]); detail=`${(reading.bytes/1048576).toFixed(1)} MiB`; } catch {} }
      if(current!==revision)return; profileSamples.push(`${data.checkpoint}: ${detail}`); profileResult.textContent=`Paused-stage page and worker snapshots: ${profileSamples.join('; ')}. These are samples, not a peak; GPU memory may be excluded.`; worker.postMessage({type:'resume'}); return;
    }
    if (data.frame) { drawCharacter(data.frame); pending=false; return; }
    if (data.error) { repeat.disabled=!lastFile; cancel.disabled=true; animating=false; memory.textContent='No import measured.'; status.textContent = `Unable to inspect this file: ${data.error}`; return; }
    try { drawArena=renderGeometry(document.querySelector('#preview'), data.positions, data.indices, data.textures, data.arenaUvs, data.batches); drawCharacter = renderGeometry(document.querySelector('#character'), data.characterPositions, data.characterIndices, data.texture, data.characterUvs); document.querySelector('#animate').disabled=false; decoded=data; play.disabled=false; } catch (e) { disposePreviews(); repeat.disabled=!lastFile; cancel.disabled=true; memory.textContent='No complete import measured.'; status.textContent = `Preview failed: ${e.message}`; return; }
    cancel.disabled=true; status.textContent = 'Local index, audio and geometry decoded; visual and animation fidelity remain unverified.';
    const elapsed=(performance.now()-start)/1000;
    if(!profiling) { timings.push(elapsed); if(timings.length>20)timings.shift(); runs.textContent=`In-page timings (first: cache unknown; repeats: likely warm): ${timings.map(x=>x.toFixed(3)).join(', ')} seconds`; }
    repeat.disabled=false;
    result.textContent = `Executable identity: ${data.identity}\nNamed package records: ${data.count}\nPreview geometry: ${data.positions.length / 3} vertices, ${data.indices.length / 3} triangles\nCharacter: ${data.bones} bones, ${data.characterPositions.length / 3} vertices, ${data.characterIndices.length / 3} triangles\nDecoded cue: ${data.pcm.length} samples at ${data.rate} Hz\n${profiling?'Diagnostic elapsed including measurement pauses':'Import and initial render time'}: ${elapsed.toFixed(2)} seconds`;
    if (typeof performance.measureUserAgentSpecificMemory === 'function') { try { const measurement=await performance.measureUserAgentSpecificMemory(); if(current===revision) memory.textContent=`Browser-reported page and worker memory: ${(measurement.bytes/1048576).toFixed(1)} MiB (snapshot; GPU allocation may be excluded).`; } catch { if(current===revision) memory.textContent='Browser memory measurement unavailable.'; } } else if(current===revision) memory.textContent='Browser memory measurement unavailable.';
  };
  worker.onerror = () => { repeat.disabled=!lastFile; cancel.disabled=true; memory.textContent='No import measured.'; status.textContent = 'Inspection failed.'; };
  worker.postMessage({file,profile:profiling});
}

cancel.addEventListener('click', () => {
  worker?.terminate(); disposePreviews(); worker=undefined; repeat.disabled=!lastFile; revision++; animationToken++; animating=false; pending=false; cancel.disabled=true; play.disabled=true; document.querySelector('#animate').disabled=true;
  for(const id of ['preview','character']) { const canvas=document.getElementById(id); canvas.replaceWith(canvas.cloneNode(false)); }
  decoded=undefined; result.textContent=''; profileResult.textContent=''; memory.textContent='No import measured.'; status.textContent='Import cancelled.';
});

play.addEventListener('click', async () => {
  if (!decoded) return;
  const context = new AudioContext();
  const clip = context.createBuffer(1, decoded.pcm.length, decoded.rate);
  const samples = clip.getChannelData(0);
  for (let i = 0; i < samples.length; i++) samples[i] = decoded.pcm[i] / 32768;
  const source = context.createBufferSource(); source.buffer = clip; source.connect(context.destination);
  source.onended = () => context.close();
  try { await context.resume(); source.start(); status.textContent = 'Playback started in the browser.'; } catch { status.textContent = 'Audio playback could not start.'; await context.close(); }
});

document.querySelector('#animate').addEventListener('click', () => {
  animating=!animating; const token=++animationToken; document.querySelector('#animate').textContent=animating?'Pause motion':'Play motion';
  if(!animating) return; const started=performance.now();
  function tick(now) { if(!animating || token!==animationToken)return; if(!pending){pending=true;worker.postMessage({type:'sample',time:((now-started)*0.03)%60});} requestAnimationFrame(tick); }
  requestAnimationFrame(tick);
});
