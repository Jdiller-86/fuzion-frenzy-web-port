import createFuzionSdkRuntime from '/artifacts/sdk-browser-full/fuzion-sdk-full.mjs';
import { openXiso } from '/host/xdvdfs.mjs';
import { WebGLRenderer } from './webgl-renderer.mjs';

const PINNED_XBE = {
  bytes: 2_834_432,
  sha256: '45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9',
};
const SDK_WASM = '/artifacts/sdk-browser-full/fuzion-sdk-full.wasm';
const BUILD_ETAG = /^"[0-9a-f]{64}"$/;
// These are the bounded candidate partition bases used by the retained native
// XISO extractor and the SDK's own disc reader. `openXiso` checks the volume
// descriptor and both directory ranges after each slice, so this reads only a
// few KiB per candidate and leaves the original full image mounted for the SDK.
const XISO_PARTITION_BASES = [0, 0x0fd90000, 0x18300000, 0x02080000, 0x1fb20000];
const BUTTON = { dpadUp: 1, dpadDown: 2, dpadLeft: 4, dpadRight: 8, start: 0x10, back: 0x20, leftThumb: 0x40, rightThumb: 0x80, a: 1 << 16, b: 1 << 17, x: 1 << 18, y: 1 << 19, black: 1 << 20, white: 1 << 21 };
let moduleInstance;
let lastInput = { buttons: 0, lx: 0, ly: 0, rx: 0, ry: 0, lt: 0, rt: 0 };
let cancelled = false;
let saveTimer;
let persistentSaves = false;
let pacingTiming;

async function flushSaves() {
  if (persistentSaves && moduleInstance) await new Promise((resolve, reject) => moduleInstance.FS.syncfs(false, error => error ? reject(error) : resolve()));
}

function report(text) { postMessage({ type: 'progress', text }); }
function digestHex(bytes) {
  return crypto.subtle.digest('SHA-256', bytes).then(hash => [...new Uint8Array(hash)].map(value => value.toString(16).padStart(2, '0')).join(''));
}
function boundedInt16(value) { return Math.max(-32768, Math.min(32767, Math.round(value * 32767))); }
function stopThreads(module) {
  const gpu = module?.onFuzionGpu;
  if (gpu) {
    gpu.deferGpuErrors = false;
    gpu.cancelShaderPromotion();
    try { gpu.cancelFrameErrorCheck(); } catch {}
    try { gpu.disposeGpuTiming(); } catch {}
  }
  try { module?._ff_browser_stop?.(); } catch {}
  try { module?.PThread?.terminateAllThreads?.(); } catch {}
}
function onFrame(ptr, width, height, pitch, format) {
  if (!moduleInstance || cancelled || !Number.isInteger(width) || !Number.isInteger(height) || width < 1 || height < 1 || width > 4096 || height > 4096) return;
  const rowBytes = width * 4;
  if (moduleInstance.onFuzionGpu) { moduleInstance.onFuzionGpu.pixels(ptr,width,height,pitch); return; }
  pitch = Number(pitch) || rowBytes;
  if (pitch < rowBytes || pitch > rowBytes + 65536) return;
  const length = pitch * height;
  const start = Number(ptr);
  if (!Number.isSafeInteger(start) || start < 0 || start + length > moduleInstance.HEAPU8.byteLength) return;
  const rgba = new Uint8Array(rowBytes * height);
  for (let y = 0; y < height; y++) rgba.set(moduleInstance.HEAPU8.subarray(start + y * pitch, start + y * pitch + rowBytes), y * rowBytes);
  const rgbaBuffer = rgba.buffer;
  postMessage({ type: 'frame', width, height, rgbaBuffer }, [rgbaBuffer]);
}
let metricStart = 0, metricFrames = 0, metricDraw = 0, metricClear = 0, metricCopy = 0, metricCalls = 0;
let metricPrevious = 0, metricIntervals = [];
let completionPreviousMs = null, completionPreviousSequence = null, completionIntervals = [];
let blankStartCount=null,blankStartMs=null;
function intervalSummary(intervals) {
  if (!intervals.length) return null;
  const ordered = intervals.slice().sort((a, b) => a - b);
  const percentile = p => ordered[Math.min(ordered.length - 1, Math.floor((ordered.length - 1) * p))];
  return { p50: percentile(.5), p95: percentile(.95), p99: percentile(.99), max: ordered.at(-1), samples: ordered.length };
}
function onMetrics(drawMs, clearMs, copyMs, drawCalls, completionMs, frameSequence, blankCount, rawDrawReuses) {
  if (cancelled || ![drawMs, clearMs, copyMs, drawCalls].every(v => Number.isFinite(v) && v >= 0)) return;
  const now = performance.now();
  // These timestamps are captured at BrowserBackend::EndFrame after Present
  // returns. Count only adjacent, strictly monotonic consumer sequences.
  const validBlank=Number.isInteger(blankCount)&&blankCount>=0&&blankCount<=0xffffffff&&Number.isFinite(completionMs);
  if(validBlank&&blankStartCount===null){blankStartCount=blankCount;blankStartMs=completionMs;}
  if (Number.isFinite(completionMs) && completionMs >= 0 && Number.isSafeInteger(frameSequence) && frameSequence > 0) {
    if (completionPreviousMs !== null && completionPreviousSequence !== null &&
        completionMs > completionPreviousMs && frameSequence > completionPreviousSequence) {
      if (frameSequence === completionPreviousSequence + 1) completionIntervals.push(completionMs - completionPreviousMs);
      completionPreviousMs = completionMs;
      completionPreviousSequence = frameSequence;
    } else {
      completionPreviousMs = completionMs;
      completionPreviousSequence = frameSequence;
    }
  } else {
    completionPreviousMs = completionPreviousSequence = null;
  }
  if (!metricStart) { metricStart = metricPrevious = now; return; }
  if (now > metricPrevious) metricIntervals.push(now-metricPrevious);
  metricPrevious=now;
  metricFrames++;
  metricDraw += drawMs; metricClear += clearMs; metricCopy += copyMs; metricCalls += drawCalls;
  const elapsed = now - metricStart;
  if (elapsed < 1000) return;
  const metricsCallbackIntervalMs = intervalSummary(metricIntervals);
  const prepareCompletionIntervalMs = intervalSummary(completionIntervals);
  const vblankHz=validBlank&&blankStartCount!==null&&completionMs>blankStartMs?
    ((blankCount-blankStartCount)>>>0)*1000/(completionMs-blankStartMs):null;
  postMessage({type: 'performance', fps: metricFrames * 1000 / elapsed,
    drawMs: metricDraw / metricFrames, clearMs: metricClear / metricFrames,
    copyMs: metricCopy / metricFrames, drawCalls: metricCalls / metricFrames,
    submissionMs: moduleInstance?.onFuzionGpu?.lastSubmitMs ?? 0,
    gpuBatches: moduleInstance?.onFuzionGpu?.lastIssuedDraws ?? 0,
    vertexPrograms: moduleInstance?.onFuzionGpu?.vertexPrograms?.size ?? 0,
    pipeline:moduleInstance?.onFuzionGpu?.pipelined===true,
    geometry: moduleInstance?.onFuzionGpu?.lastFrameStats,
    shaders: moduleInstance?.onFuzionGpu?.shaderStats,
    imageLookups: moduleInstance?.onFuzionGpu?.imageLookups,
    constantPacking: moduleInstance?.onFuzionGpu?.constantPacking,
    textureMetadataLookups: moduleInstance?.onFuzionGpu?.textureMetadataLookups,
    rawDrawReuses,
    gpuTiming: moduleInstance?.onFuzionGpu?.gpuTimingStats,
    pacingTiming,
    metricsCallbackIntervalMs,
    frameMs: metricsCallbackIntervalMs,
    prepareCompletionIntervalMs,vblankHz});
  // The SDK virtual-blank counter is independent of rendered/presented frames.
  metricStart = now; metricFrames = metricDraw = metricClear = metricCopy = metricCalls = 0;
  metricIntervals=[];completionIntervals=[];
  if (pacingTiming) {
    for (const samples of Object.values(pacingTiming)) samples.length = 0;
  }
  blankStartCount=validBlank?blankCount:null;blankStartMs=validBlank?completionMs:null;
}
function onAudio(ptr, frames, rate) {
  if (!moduleInstance || cancelled || !Number.isInteger(frames) || frames < 1 || frames > 65536 || !Number.isFinite(rate) || rate < 8000 || rate > 192000) return;
  const start = Number(ptr) >>> 2;
  const count = frames * 2;
  if (!Number.isSafeInteger(start) || start + count > moduleInstance.HEAPF32.length) return;
  const samplesBuffer = moduleInstance.HEAPF32.slice(start, start + count).buffer;
  postMessage({ type: 'audio', frames, rate, samplesBuffer }, [samplesBuffer]);
}
function pushInput() {
  if (!moduleInstance || cancelled) return;
  const setPad = moduleInstance._ff_browser_set_pad;
  if (typeof setPad !== 'function') return;
  const p = lastInput;
  setPad(0, p.buttons, p.lx, p.ly, p.rx, p.ry, p.lt, p.rt);
}
function parseInput(message) {
  const keySet = new Set(message.keys || []);
  let buttons = 0;
  const keyMap = { ArrowUp: BUTTON.dpadUp, ArrowDown: BUTTON.dpadDown, ArrowLeft: BUTTON.dpadLeft, ArrowRight: BUTTON.dpadRight, Enter: BUTTON.start, Tab: BUTTON.back, Space: BUTTON.a, Escape: BUTTON.b, Backspace: BUTTON.b, KeyX: BUTTON.x, KeyY: BUTTON.y, Digit1: BUTTON.black, Digit2: BUTTON.white };
  for (const [key, bit] of Object.entries(keyMap)) if (keySet.has(key)) buttons |= bit;
  const padButtons = message.buttons || [];
  const mapping = [[0,BUTTON.a],[1,BUTTON.b],[2,BUTTON.x],[3,BUTTON.y],[4,BUTTON.black],[5,BUTTON.white],[8,BUTTON.back],[9,BUTTON.start],[10,BUTTON.leftThumb],[11,BUTTON.rightThumb],[12,BUTTON.dpadUp],[13,BUTTON.dpadDown],[14,BUTTON.dpadLeft],[15,BUTTON.dpadRight]];
  for (const [index, bit] of mapping) if ((padButtons[index] || 0) > 0.2) buttons |= bit;
  const axes = message.axes || [];
  lastInput = { buttons, lx: boundedInt16((axes[0] || 0) + Number(keySet.has('KeyD')) - Number(keySet.has('KeyA'))), ly: boundedInt16(-(axes[1] || 0) + Number(keySet.has('KeyW')) - Number(keySet.has('KeyS'))), rx: boundedInt16(axes[2] || 0), ry: boundedInt16(-(axes[3] || 0)), lt: keySet.has('KeyZ') ? 255 : Math.round(Math.max(0, Math.min(1, padButtons[6] || 0)) * 255), rt: keySet.has('KeyC') ? 255 : Math.round(Math.max(0, Math.min(1, padButtons[7] || 0)) * 255) };
}

async function compileWasm(previous) {
  if (typeof WebAssembly.compileStreaming !== 'function') return undefined;
  try {
    if (previous?.module instanceof WebAssembly.Module && BUILD_ETAG.test(previous.etag)) {
      const head = await fetch(SDK_WASM, {method:'HEAD', cache:'no-store'});
      if (head.ok && head.headers.get('ETag') === previous.etag) return previous;
    }
    if (cancelled) return undefined;
    const response = await fetch(SDK_WASM, {cache:'no-store'});
    if (!response.ok) return undefined;
    const etag = response.headers.get('ETag');
    return {etag, module:await WebAssembly.compileStreaming(response)};
  } catch {
    return undefined;
  }
}

async function boot({iso,canvas,renderer,gpuVertices,threadedGpu,pipelinedGpu,paceBeforePresent,deferGpuErrors,rawVertices,precisePacing,frameCapPrecision,compiledWasm,
                     shaderPrewarm,shaderWarmup,shaderFallback,shaderPromotion,guestCodeWarmup,gpuTiming,pacingTiming:measurePacing,resourceFences}) {
  if (!(iso instanceof File)) throw new Error('Select your local Fuzion Frenzy disc image.');
  report('Checking the local disc image and locating DEFAULT.XBE…');
  // Partition probing reads only the descriptor and root-directory ranges for
  // each known base; it never reads or hashes the full ISO.
  let disc;
  let partitionError;
  for (const base of XISO_PARTITION_BASES) {
    if (base >= iso.size) continue;
    try {
      disc = await openXiso(iso.slice(base));
      break;
    } catch (cause) { partitionError = cause; }
  }
  if (!disc) throw new Error(`Could not locate a supported Xbox game partition in this ISO.${partitionError ? ` ${partitionError.message}` : ''}`);
  const xbe = disc.file(await disc.find('default.xbe'));
  if (xbe.size !== PINNED_XBE.bytes) throw new Error(`DEFAULT.XBE size does not match this local build (${xbe.size} bytes).`);
  const xbeHash = await digestHex(await xbe.arrayBuffer());
  if (xbeHash !== PINNED_XBE.sha256) throw new Error(`DEFAULT.XBE does not match the pinned Fuzion Frenzy build (SHA-256 ${xbeHash}).`);
  if (cancelled) return;
  const build = compileWasm(compiledWasm);
  const createGpu=()=>new WebGLRenderer(canvas,()=>moduleInstance,text=>postMessage({type:'log',text}),gpuVertices,shaderPrewarm!==false,gpuTiming===true,shaderFallback===true,shaderPromotion!==false);
  const prepareShaders = renderer === 'webgl' && shaderPrewarm !== false;
  const gpu = prepareShaders && (shaderWarmup === true || shaderFallback === true) ? createGpu() : undefined;
  const warmup = gpu?.parallelShaderCompile && shaderWarmup === true
    ? import('./shader-warmup.mjs')
        .then(({openShaderWarmup}) => openShaderWarmup(xbeHash))
        .then(store => { if (!cancelled) gpu.prepareWarmup(store); })
        .catch(() => postMessage({type:'log',text:'Local shader warmup storage is unavailable.'}))
    : undefined;
  if(cancelled)return;
  report('Starting the full SDK runtime from the local disc image…');
  const gameXbe = new Blob([xbe], { type: 'application/octet-stream' });
  const compiled = await build;
  if (cancelled) return;
  const factory = createFuzionSdkRuntime;
  const module = await factory({
    locateFile: name => name.endsWith('.wasm') ? SDK_WASM : name.endsWith('.worker.js') ? '/artifacts/sdk-browser-full/fuzion-sdk-full.worker.js' : name,
    ...(compiled && {
      instantiateWasm(imports, receiveInstance) {
        const instance = new WebAssembly.Instance(compiled.module, imports);
        receiveInstance(instance, compiled.module);
        return instance.exports;
      },
    }),
    print: line => report(`SDK: ${line}`),
    printErr: line => postMessage({ type: 'log', text: String(line) }),
    onAbort: reason => postMessage({ type: 'result', text: 'SDK runtime stopped.', error: String(reason || 'Runtime aborted; the local browser console contains the stack.'), done: true }),
    onFuzionFrame: onFrame,
    onFuzionMetrics: onMetrics,
    onFuzionAudio: onAudio,
    preRun: [runtime => {
      const FS = runtime.FS;
      if (!FS || !runtime.WORKERFS) throw new Error('Full SDK build does not expose WorkerFS for local game files.');
      FS.mkdir('/game');
      FS.mount(runtime.WORKERFS, { blobs: [{ name: 'game.iso', data: iso }, { name: 'default.xbe', data: gameXbe }] }, '/game');
      try { FS.mkdir('/hdd'); } catch {}
    }],
  });
  moduleInstance = module;
  if (compiled && compiled !== compiledWasm && BUILD_ETAG.test(compiled.etag)) {
    // Only the compiled module is shared. Each worker creates its own instance,
    // guest memory and threads, and first verifies the local disc identity.
    try { postMessage({type:'compiled-wasm', build:compiled}); } catch { /* Sharing is optional. */ }
  }
  if (measurePacing === true) {
    const waits = [[], [], []];
    const presents = [];
    const resourceWaits = [];
    const scenePhases = [];
    pacingTiming = { frameCap: waits[0], verticalBlank: waits[1], eventHold: waits[2], presents, resourceWaits, scenePhases };
    module.onFuzionPacingWait = (kind, ...sample) => {
      const list = waits[kind];
      if (list && list.length < 120) list.push(sample);
    };
    module.onFuzionPacingPresent = (...sample) => {
      if (presents.length < 120) presents.push(sample);
    };
    module.onFuzionPacingResourceWait = (...sample) => {
      if (resourceWaits.length < 120) resourceWaits.push(sample);
    };
    module.onFuzionSceneTiming = (...sample) => {
      if (scenePhases.length < 128) scenePhases.push(sample);
    };
  }
  if (renderer === 'webgl') {
    module.onFuzionGpu = gpu ?? createGpu();
    module.onFuzionGpu.threaded=threadedGpu===true;
    module.onFuzionGpu.pipelined=pipelinedGpu===true;
    module.onFuzionGpu.paceBeforePresent=paceBeforePresent===true;
    module.onFuzionGpu.deferGpuErrors=deferGpuErrors===true;
    module.onFuzionGpu.rawVertices=rawVertices===true;
    if (shaderFallback && rawVertices && module._ff_browser_fallback_vertex_template)
      module.onFuzionGpu.beginProgrammableFallback(module._ff_browser_fallback_vertex_template());
    module.onFuzionGpu.measurePacing=measurePacing===true;
    module.onFuzionGpu.resourceFences=resourceFences;
    module.onFuzionGpu.precisePacing=precisePacing===true;
    module.onFuzionGpu.frameCapPrecision=frameCapPrecision===true;
    postMessage({type:'log',text:`Render preparation: ${module.onFuzionGpu.threaded?'threaded':'serial'}`});
  }
  if (cancelled) { stopThreads(module); return; }
  if (module.IDBFS) {
    try {
      module.FS.mount(module.IDBFS, {}, '/hdd');
      await new Promise((resolve, reject) => module.FS.syncfs(true, error => error ? reject(error) : resolve()));
      persistentSaves = true;
      saveTimer = setInterval(() => flushSaves().catch(error => postMessage({ type: 'log', text: `Save persistence: ${error.message}` })), 30000);
    } catch (error) { report(`Browser save storage is unavailable: ${error.message}. Saves will last only for this run.`); }
  }
  if (warmup) await warmup;
  if (cancelled) { stopThreads(module); return; }
  const webgl = module.onFuzionGpu;
  if (webgl?.pendingFixedFallbackPrograms) {
    report('Preparing resident WebGL shaders…');
    webgl.finishFixedFallbacks();
  }
  if (webgl?.warmPrograms?.size) {
    report('Preparing previously seen WebGL shaders…');
    await webgl.finishWarmup(() => cancelled);
    if (cancelled) return;
  }
  if (guestCodeWarmup !== false && typeof module._ff_browser_warm_guest_code === 'function') {
    report('Preparing original game code…');
    let count = 0;
    let elapsed = 0;
    for (const warmup of [module._ff_browser_warm_guest_code, module._ff_browser_warm_guest_code_extra]) {
      if (typeof warmup !== 'function') continue;
      const start = performance.now();
      count += warmup();
      elapsed += performance.now() - start;
      await new Promise(resolve => setTimeout(resolve, 0));
      if (cancelled) { stopThreads(module); return; }
      if (webgl?.contextLost || webgl?.gl.isContextLost())
        throw new Error('The graphics context was lost. Start the game again to recreate its resources.');
    }
    postMessage({type:'log',text:`Prepared ${count} guest functions in ${elapsed.toFixed(2)} ms.`});
  }
  if (webgl?.contextLost || webgl?.gl.isContextLost())
    throw new Error('The graphics context was lost. Start the game again to recreate its resources.');
  pushInput();
  const bootFn = module._ff_browser_boot;
  if (typeof bootFn !== 'function') throw new Error('Full SDK build is missing ff_browser_boot().');
  report('Booting the original game; waiting for the renderer’s first frame…');
  const result = bootFn();
  if (result !== 0) throw new Error(`Original game boot returned ${result} before completing startup.`);
  postMessage({ type: 'result', text: 'The SDK boot loop returned after starting the game runtime.' });
}

onmessage = async ({ data }) => {
  if (data?.type === 'input') { parseInput(data); pushInput(); return; }
  if (data?.type === 'stop') {
    cancelled = true;
    clearInterval(saveTimer);
    stopThreads(moduleInstance);
    const saveShaders=moduleInstance?.onFuzionGpu?.shaderWarmup?.save().catch(()=>{
      postMessage({type:'log',text:'Local shader warmup could not be saved.'});
    });
    try { await flushSaves(); } catch (error) { postMessage({ type: 'log', text: `Save flush failed: ${error.message}` }); }
    await saveShaders;
    postMessage({ type: 'stopped' });
    return;
  }
  if (data?.type !== 'run') return;
  try { await boot(data); }
  catch (cause) {
    stopThreads(moduleInstance);
    postMessage({ type: 'result', text: 'The original game did not reach a running frame.', error: cause instanceof Error ? cause.message : String(cause), done: true });
  }
};
