const isoPicker = document.querySelector('#iso');
const runButton = document.querySelector('#run');
const stopButton = document.querySelector('#stop');
const status = document.querySelector('#status');
const error = document.querySelector('#error');
const log = document.querySelector('#log');
let canvas = document.querySelector('#screen');
const performanceReadout = document.querySelector('#performance');
let context;
let worker;
let compiledWasm;
let stopping = false;
let inputTimer;
let sentInput;
let audioContext;
let audioCursor;
let hasFrame = false;
function showFrame() {
  if (!hasFrame) {
    status.textContent = 'Rendering original game frames.';
    runButton.textContent = 'Game running';
    window.dispatchEvent(new CustomEvent('fuzion-busy',{detail:{busy:false}}));
    hasFrame = true;
    window.dispatchEvent(new CustomEvent('fuzion-state',{detail:{running:true}}));
    canvas.focus({preventScroll:true});
  }
}

function setBusy(busy) {
  runButton.disabled = busy || stopping;
  stopButton.disabled = !busy;
  isoPicker.disabled = busy;
  document.querySelector('#renderer').disabled = busy;
  runButton.textContent = busy ? 'Starting…' : stopping ? 'Stopping…' : 'Start game';
  window.dispatchEvent(new CustomEvent('fuzion-busy',{detail:{busy}}));
}
function stop() {
  hasFrame = false;
  globalThis.fuzionPerformance = undefined;
  window.dispatchEvent(new CustomEvent('fuzion-state',{detail:{running:false}}));
  if (audioContext) { audioContext.close().catch(() => {}); audioContext = undefined; }
  audioCursor = undefined;
  clearInterval(inputTimer);
  inputTimer = undefined;
  sentInput = undefined;
  if (worker) {
    const previous = worker;
    worker = undefined;
    stopping = true;
    const finish = () => {
      clearTimeout(timeout);
      previous.terminate();
      stopping = false;
      setBusy(false);
    };
    const timeout = setTimeout(finish, 3000);
    previous.addEventListener('message', ({data}) => {
      if (data.type === 'stopped') finish();
    });
    previous.postMessage({ type: 'stop' });
  }
  setBusy(false);
}
function sameInputValues(previous, next) {
  return previous.length === next.length && next.every((value, index) => value === previous[index]);
}
function sendInput(current, keys, buttons, axes) {
  if (!current || current !== worker) return;
  if (sentInput && sameInputValues(sentInput.keys, keys)
      && sameInputValues(sentInput.buttons, buttons)
      && sameInputValues(sentInput.axes, axes)) return;
  const snapshot = { type: 'input', keys, buttons, axes };
  current.postMessage(snapshot);
  sentInput = snapshot;
}
function keyboardBits() {
  const keys = new Set(['ArrowUp','ArrowDown','ArrowLeft','ArrowRight','Enter','Escape','Space','Tab','Backspace','KeyW','KeyA','KeyS','KeyD','KeyX','KeyY','KeyZ','KeyC','Digit1','Digit2']);
  const held = new Set();
  window.addEventListener('keydown', e => { if (worker && e.target === canvas && keys.has(e.code)) { held.add(e.code); e.preventDefault(); } });
  window.addEventListener('keyup', e => held.delete(e.code));
  const release = () => { held.clear(); sendInput(worker, [], [], []); };
  window.addEventListener('blur', release);
  document.addEventListener('visibilitychange', () => { if (document.hidden) release(); });
  document.addEventListener('focusin', e => { if(e.target !== canvas) release(); });
  return held;
}
const heldKeys = keyboardBits();

runButton.addEventListener('click', () => {
  if (stopping) return;
  error.textContent = '';
  if (!globalThis.crossOriginIsolated || typeof SharedArrayBuffer === 'undefined') {
    error.textContent = 'This build needs browser thread support. Use a current 64-bit Chrome or Edge and start the included local server; do not open the HTML file directly.';
    return;
  }
  const iso = isoPicker.files?.[0];
  if (!iso) { error.textContent = 'Select your local Fuzion Frenzy disc image.'; return; }
  stop();
  heldKeys.clear();
  const options = new URLSearchParams(location.search);
  const deadlineAudio = options.get('audioClock') !== 'relative';
  const audioLead = deadlineAudio ? 0.03 : 0;
  const renderer = document.querySelector('#renderer').value;
  if (renderer === 'webgl' && !canvas.transferControlToOffscreen) {
    error.textContent = 'This browser needs OffscreenCanvas for GPU rendering. Choose Software reference or use current Chrome/Edge.';
    return;
  }
  // A transferred canvas belongs to one worker. A fresh element makes restart
  // and backend switching deterministic after its worker has been terminated.
  const nextCanvas = canvas.cloneNode(false);
  canvas.replaceWith(nextCanvas); canvas = nextCanvas;
  canvas.addEventListener('click', () => canvas.focus());
  context = renderer === 'software' ? canvas.getContext('2d', {alpha:false}) : undefined;
  const offscreen = renderer === 'webgl' ? canvas.transferControlToOffscreen() : undefined;
  if (typeof AudioContext !== 'undefined') {
    audioContext ??= new AudioContext();
    audioContext.resume().catch(() => {});
  }
  worker = new Worker('./worker.mjs', { type: 'module' });
  const current = worker;
  setBusy(true);
  status.textContent = 'Checking the local disc image…';
  performanceReadout.textContent = `${renderer === 'webgl' ? 'WebGL 2 GPU' : 'Software reference'} renderer. Waiting for frame measurements…`;
  current.addEventListener('message', ({data}) => {
    if (worker !== current) return;
    if (data.type === 'log') {
      const lines = (log.textContent + '\n' + data.text).split('\n');
      log.textContent = lines.slice(-200).join('\n');
    }
    else if (data.type === 'progress') status.textContent = data.text;
    else if (data.type === 'compiled-wasm') compiledWasm = data.build;
    else if (data.type === 'gpu-error') {
      error.textContent = data.text; status.textContent = 'GPU renderer stopped.'; stop();
    }
    else if (data.type === 'vertex-diagnostic') {
      // Player-local diagnostic only. Never sent to the HTTP host or telemetry.
      globalThis.fuzionVertexDiagnostic = {hlsl:data.hlsl,regs:data.regs,flat:data.flat};
    }
    else if (data.type === 'gpu-frame') {
      showFrame();
    }
    else if (data.type === 'frame') {
      const pixels = new Uint8ClampedArray(data.rgbaBuffer);
      if (canvas.width !== data.width || canvas.height !== data.height) { canvas.width = data.width; canvas.height = data.height; }
      context.putImageData(new ImageData(pixels, data.width, data.height), 0, 0);
      showFrame();
    } else if (data.type === 'performance') {
        globalThis.fuzionPerformance = data; // Local profiling; never uploaded.
      if ([data.fps, data.drawMs, data.clearMs, data.copyMs, data.drawCalls].every(v => Number.isFinite(v) && v >= 0)) {
          performanceReadout.textContent = `${renderer === 'webgl' ? 'WebGL 2' : 'Software'} · ${data.fps.toFixed(1)} FPS · CPU ${data.pipeline?'prepare/wait':renderer === 'webgl'?'frame/submit':'draw/submit'} ${data.drawMs.toFixed(1)} ms · clear ${data.clearMs.toFixed(1)} ms · copy ${data.copyMs.toFixed(1)} ms · ${Math.round(data.drawCalls)} draws/frame`;
        if (renderer === 'webgl') performanceReadout.textContent += ` · JS submit ${Number(data.submissionMs||0).toFixed(1)} ms · ${Math.round(data.gpuBatches||0)} GPU batches · ${Math.round(data.vertexPrograms||0)} vertex programs`;
      }
    } else if (data.type === 'audio') {
      if (audioContext && data.frames > 0 && data.frames <= 65536) {
        const samples = new Float32Array(data.samplesBuffer);
        const buffer = audioContext.createBuffer(2, data.frames, data.rate);
        const left = buffer.getChannelData(0);
        const right = buffer.getChannelData(1);
        for (let i = 0; i < data.frames; i++) { left[i] = samples[i * 2]; right[i] = samples[i * 2 + 1]; }
        const source = audioContext.createBufferSource();
        source.buffer = buffer;
        source.connect(audioContext.destination);
        const now = audioContext.currentTime;
        if (audioCursor > now + 0.5) return;
        if (audioCursor === undefined || audioCursor < now) audioCursor = now + audioLead;
        source.start(audioCursor);
        audioCursor += buffer.duration;
      }
    } else if (data.type === 'result') {
      status.textContent = data.text;
      error.textContent = data.error || '';
      if (data.done) {
        stop();
      }
    }
  });
  current.addEventListener('error', e => {
    if (worker !== current) return;
    error.textContent = e.message || 'Browser worker failed.';
    status.textContent = 'Game stopped.';
    stop();
  });
  const windowsPacing = /Windows/.test(navigator.userAgent);
  current.postMessage({
    type: 'run', iso, renderer, canvas: offscreen, compiledWasm,
    gpuVertices: options.get('cpuVertices') !== '1',
    threadedGpu: options.get('threadedGpu') !== '0',
    pipelinedGpu: options.get('pipelinedGpu') !== '0',
    paceBeforePresent: options.has('presentPacing')
      ? options.get('presentPacing') === 'before'
      : windowsPacing,
    deferGpuErrors: options.get('gpuErrorChecks') !== 'sync',
    rawVertices: options.get('rawVertices') !== '0',
    shaderPrewarm: options.get('shaderPrewarm') !== '0',
    shaderWarmup: options.get('shaderWarmup') !== '0',
    shaderFallback: options.get('shaderFallback') !== '0',
    shaderPromotion: options.get('shaderPromotion') !== '0',
    guestCodeWarmup: options.get('guestCodeWarmup') !== '0',
    gpuTiming: options.get('gpuTiming') === '1',
    directTextureUploads: options.get('textureUpload') !== 'copy',
    pacingTiming: options.get('pacingTiming') === '1',
    resourceFences: options.get('resourceFences'),
    gridContours: options.get('gridContours') === '1',
    fastInteger: options.get('fastInteger') !== '0',
    decimalProductMemo: options.get('decimalProductMemo') !== '0',
    asyncAudio: options.get('audioHandoff') !== 'sync',
    deadlineAudio,
    precisePacing: options.has('precisePacing')
      ? options.get('precisePacing') === '1'
      : windowsPacing,
    frameCapPrecision: options.has('frameCapPrecision')
      ? options.get('frameCapPrecision') === '1'
      : windowsPacing,
  }, offscreen ? [offscreen] : []);
  inputTimer = setInterval(() => {
    const focused = document.activeElement === canvas && !document.hidden;
    const pad = focused ? [...navigator.getGamepads()].find(item => item?.connected && item.mapping === 'standard') : undefined;
    sendInput(current, focused ? [...heldKeys] : [], pad ? pad.buttons.map(b => b.value) : [], pad ? [...pad.axes] : []);
  }, 1000 / 60);
});

stopButton.addEventListener('click', () => {
  stop();
  status.textContent = 'Game stopped.';
});
