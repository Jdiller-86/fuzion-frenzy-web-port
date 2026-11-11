const isoPicker = document.querySelector('#iso');
const runButton = document.querySelector('#run');
const stopButton = document.querySelector('#stop');
const status = document.querySelector('#status');
const error = document.querySelector('#error');
const log = document.querySelector('#log');
const canvas = document.querySelector('#screen');
const context = canvas.getContext('2d', { alpha: false });
let worker;
let inputTimer;
let audioContext;
let audioCursor = 0;

function setBusy(busy) {
  runButton.disabled = busy;
  stopButton.disabled = !busy;
}
function stop() {
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
  window.addEventListener('keydown', e => { if (keys.has(e.code)) { held.add(e.code); e.preventDefault(); } });
  window.addEventListener('keyup', e => held.delete(e.code));
  const release = () => { held.clear(); worker?.postMessage({ type: 'input', keys: [], buttons: [], axes: [] }); };
  window.addEventListener('blur', release);
  document.addEventListener('visibilitychange', () => { if (document.hidden) release(); });
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
  if (typeof AudioContext !== 'undefined') {
    audioContext ??= new AudioContext();
    audioContext.resume().catch(() => {});
    audioCursor = audioContext.currentTime;
  }
  worker = new Worker('./worker.mjs', { type: 'module' });
  const current = worker;
  setBusy(true);
  status.textContent = 'Checking the local disc image…';
  current.addEventListener('message', ({data}) => {
    if (worker !== current) return;
    if (data.type === 'log') {
      const lines = (log.textContent + '\n' + data.text).split('\n');
      log.textContent = lines.slice(-200).join('\n');
    }
    else if (data.type === 'progress') status.textContent = data.text;
    else if (data.type === 'frame') {
      const pixels = new Uint8ClampedArray(data.rgbaBuffer);
      if (canvas.width !== data.width || canvas.height !== data.height) { canvas.width = data.width; canvas.height = data.height; }
      context.putImageData(new ImageData(pixels, data.width, data.height), 0, 0);
      status.textContent = 'Rendering original game frames.';
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
  current.postMessage({ type: 'run', iso });
  inputTimer = setInterval(() => {
    const pad = !document.hasFocus() || document.hidden ? undefined : [...navigator.getGamepads()].find(item => item?.connected && item.mapping === 'standard');
    current.postMessage({ type: 'input', keys: [...heldKeys], buttons: pad ? pad.buttons.map(b => b.value) : [], axes: pad ? [...pad.axes] : [] });
  }, 1000 / 60);
});

stopButton.addEventListener('click', () => {
  stop();
  status.textContent = 'Game stopped.';
});
