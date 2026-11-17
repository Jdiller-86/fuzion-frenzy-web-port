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
let inputTimer;
let audioContext;
let audioCursor = 0;
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
  runButton.disabled = busy;
  stopButton.disabled = !busy;
  isoPicker.disabled = busy;
  document.querySelector('#renderer').disabled = busy;
  runButton.textContent = busy ? 'Starting…' : 'Start game';
  window.dispatchEvent(new CustomEvent('fuzion-busy',{detail:{busy}}));
}
function stop() {
  hasFrame = false;
  globalThis.fuzionPerformance = undefined;
  window.dispatchEvent(new CustomEvent('fuzion-state',{detail:{running:false}}));
  if (audioContext) { audioContext.close().catch(() => {}); audioContext = undefined; }
  audioCursor = 0;
  clearInterval(inputTimer);
  inputTimer = undefined;
  if (worker) {
    const previous = worker;
    previous.postMessage({ type: 'stop' });
    setTimeout(() => previous.terminate(), 3000);
    worker = undefined;
  }
  setBusy(false);
}
function keyboardBits() {
  const keys = new Set(['ArrowUp','ArrowDown','ArrowLeft','ArrowRight','Enter','Escape','Space','Tab','Backspace','KeyW','KeyA','KeyS','KeyD','KeyX','KeyY','KeyZ','KeyC','Digit1','Digit2']);
  const held = new Set();
  window.addEventListener('keydown', e => { if (worker && e.target === canvas && keys.has(e.code)) { held.add(e.code); e.preventDefault(); } });
  window.addEventListener('keyup', e => held.delete(e.code));
  const release = () => { held.clear(); worker?.postMessage({ type: 'input', keys: [], buttons: [], axes: [] }); };
  window.addEventListener('blur', release);
  document.addEventListener('visibilitychange', () => { if (document.hidden) release(); });
  document.addEventListener('focusin', e => { if(e.target !== canvas) release(); });
  return held;
}
const heldKeys = keyboardBits();

runButton.addEventListener('click', () => {
  error.textContent = '';
  if (!globalThis.crossOriginIsolated || typeof SharedArrayBuffer === 'undefined') {
    error.textContent = 'This build needs browser thread support. Use a current 64-bit Chrome or Edge and start the included local server; do not open the HTML file directly.';
    return;
  }
  const iso = isoPicker.files?.[0];
  if (!iso) { error.textContent = 'Select your local Fuzion Frenzy disc image.'; return; }
  stop();
  heldKeys.clear();
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
    audioCursor = audioContext.currentTime;
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
        const left = new Float32Array(data.frames);
        const right = new Float32Array(data.frames);
        for (let i = 0; i < data.frames; i++) { left[i] = samples[i * 2]; right[i] = samples[i * 2 + 1]; }
        buffer.copyToChannel(left, 0);
        buffer.copyToChannel(right, 1);
        const source = audioContext.createBufferSource();
        source.buffer = buffer;
        source.connect(audioContext.destination);
        if (audioCursor > audioContext.currentTime + 0.5) return;
        audioCursor = Math.max(audioCursor, audioContext.currentTime);
        source.start(audioCursor);
        audioCursor += buffer.duration;
      }
    } else if (data.type === 'stopped') {
      current.terminate();
      if (worker === current) worker = undefined;
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
  current.postMessage({ type: 'run', iso, renderer, canvas: offscreen, gpuVertices: new URLSearchParams(location.search).get('cpuVertices') !== '1', threadedGpu: new URLSearchParams(location.search).get('threadedGpu') !== '0', pipelinedGpu: new URLSearchParams(location.search).get('pipelinedGpu') !== '0', rawVertices: new URLSearchParams(location.search).get('rawVertices') !== '0', precisePacing: new URLSearchParams(location.search).has('precisePacing') ? new URLSearchParams(location.search).get('precisePacing') === '1' : /Windows/.test(navigator.userAgent) }, offscreen ? [offscreen] : []);
  inputTimer = setInterval(() => {
    const pad = document.activeElement !== canvas || document.hidden ? undefined : [...navigator.getGamepads()].find(item => item?.connected && item.mapping === 'standard');
    current.postMessage({ type: 'input', keys: document.activeElement === canvas ? [...heldKeys] : [], buttons: pad ? pad.buttons.map(b => b.value) : [], axes: pad ? [...pad.axes] : [] });
  }, 1000 / 60);
});

stopButton.addEventListener('click', () => {
  stop();
  status.textContent = 'Game stopped.';
});
