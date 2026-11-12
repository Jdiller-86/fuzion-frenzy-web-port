import createFuzionSdkRuntime from '/artifacts/sdk-browser-full/fuzion-sdk-full.mjs';
import { openXiso } from '/host/xdvdfs.mjs';

const PINNED_XBE = {
  bytes: 2_834_432,
  sha256: '45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9',
};
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

async function flushSaves() {
  if (persistentSaves && moduleInstance) await new Promise((resolve, reject) => moduleInstance.FS.syncfs(false, error => error ? reject(error) : resolve()));
}

function report(text) { postMessage({ type: 'progress', text }); }
function digestHex(bytes) {
  return crypto.subtle.digest('SHA-256', bytes).then(hash => [...new Uint8Array(hash)].map(value => value.toString(16).padStart(2, '0')).join(''));
}
function boundedInt16(value) { return Math.max(-32768, Math.min(32767, Math.round(value * 32767))); }
function stopThreads(module) {
  try { module?._ff_browser_stop?.(); } catch {}
  try { module?.PThread?.terminateAllThreads?.(); } catch {}
}
function onFrame(ptr, width, height, pitch, format) {
  if (!moduleInstance || cancelled || !Number.isInteger(width) || !Number.isInteger(height) || width < 1 || height < 1 || width > 4096 || height > 4096) return;
  const rowBytes = width * 4;
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
function onMetrics(drawMs, clearMs, copyMs, drawCalls) {
  if (cancelled || ![drawMs, clearMs, copyMs, drawCalls].every(v => Number.isFinite(v) && v >= 0)) return;
  const now = performance.now();
  if (!metricStart) { metricStart = now; return; }
  metricFrames++;
  metricDraw += drawMs; metricClear += clearMs; metricCopy += copyMs; metricCalls += drawCalls;
  const elapsed = now - metricStart;
  if (elapsed < 1000) return;
  postMessage({type: 'performance', fps: metricFrames * 1000 / elapsed,
    drawMs: metricDraw / metricFrames, clearMs: metricClear / metricFrames,
    copyMs: metricCopy / metricFrames, drawCalls: metricCalls / metricFrames});
  metricStart = now; metricFrames = metricDraw = metricClear = metricCopy = metricCalls = 0;
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

async function boot(iso) {
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
  report('Starting the full SDK runtime from the local disc image…');
  const gameXbe = new Blob([xbe], { type: 'application/octet-stream' });
  const factory = createFuzionSdkRuntime;
  const module = await factory({
    locateFile: name => name.endsWith('.wasm') ? '/artifacts/sdk-browser-full/fuzion-sdk-full.wasm' : name.endsWith('.worker.js') ? '/artifacts/sdk-browser-full/fuzion-sdk-full.worker.js' : name,
    print: line => report(`SDK: ${line}`),
    printErr: line => postMessage({ type: 'log', text: String(line) }),
    onAbort: reason => postMessage({ type: 'result', text: 'SDK runtime stopped.', error: String(reason), done: true }),
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
  if (cancelled) { stopThreads(module); return; }
  if (module.IDBFS) {
    try {
      module.FS.mount(module.IDBFS, {}, '/hdd');
      await new Promise((resolve, reject) => module.FS.syncfs(true, error => error ? reject(error) : resolve()));
      persistentSaves = true;
      saveTimer = setInterval(() => flushSaves().catch(error => postMessage({ type: 'log', text: `Save persistence: ${error.message}` })), 30000);
    } catch (error) { report(`Browser save storage is unavailable: ${error.message}. Saves will last only for this run.`); }
  }
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
    try { await flushSaves(); } catch (error) { postMessage({ type: 'log', text: `Save flush failed: ${error.message}` }); }
    postMessage({ type: 'stopped' });
    return;
  }
  if (data?.type !== 'run') return;
  try { await boot(data.iso); }
  catch (cause) {
    stopThreads(moduleInstance);
    postMessage({ type: 'result', text: 'The original game did not reach a running frame.', error: cause instanceof Error ? cause.message : String(cause), done: true });
  }
};
