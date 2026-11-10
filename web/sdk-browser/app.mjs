const PINNED_XBE = {
  bytes: 2_834_432,
  sha256: '45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9',
};
const MAX_DATA_FILES = 16384;
const MAX_MOUNT_BYTES = 8 * 1024 * 1024 * 1024;
const MAX_DATA_BYTES = MAX_MOUNT_BYTES - PINNED_XBE.bytes;

const xbePicker = document.querySelector('#xbe');
const dataPicker = document.querySelector('#data');
const runButton = document.querySelector('#run');
const stopButton = document.querySelector('#stop');
const status = document.querySelector('#status');
const error = document.querySelector('#error');
const dataSummary = document.querySelector('#data-summary');
let worker;

function setBusy(busy) {
  runButton.disabled = busy;
  stopButton.disabled = !busy;
}

function dataInventory(files) {
  if (files.length > MAX_DATA_FILES) throw new Error(`Data folder has ${files.length} files; limit is ${MAX_DATA_FILES}`);
  let totalBytes = 0;
  const entries = [];
  for (const file of files) {
    totalBytes += file.size;
    if (!Number.isSafeInteger(totalBytes) || totalBytes > MAX_DATA_BYTES) {
      throw new Error('Selected game data plus the pinned XBE exceeds the 8 GiB mount limit');
    }
    const relativePath = file.webkitRelativePath || file.name;
    const parts = relativePath.split(/[\\/]/);
    if (relativePath.startsWith('/') || parts.length < 2 || parts.some(part => !part || part === '.' || part === '..' || part.includes('\0'))) {
      throw new Error('Selected data folder contains an unsafe relative path');
    }
    const guestPath = parts.slice(1).join('/');
    if (guestPath.length > 1024 || parts.length > 32) throw new Error('Selected game data path exceeds the path depth or length limit');
    entries.push({ path: guestPath, bytes: file.size, file });
  }
  return { count: entries.length, totalBytes, entries };
}

dataPicker.addEventListener('change', () => {
  try {
    const inventory = dataInventory([...dataPicker.files]);
    dataSummary.textContent = inventory.count
      ? `${inventory.count} local files selected (${inventory.totalBytes.toLocaleString()} bytes); the worker will mount them read-only on demand.`
      : 'No data folder selected.';
    error.textContent = '';
  } catch (cause) {
    dataSummary.textContent = 'Data folder selection rejected.';
    error.textContent = cause instanceof Error ? cause.message : String(cause);
  }
});

function failRun(message) {
  error.textContent = message;
  status.textContent = 'Run stopped.';
  setBusy(false);
  worker?.terminate();
  worker = undefined;
}

runButton.addEventListener('click', async () => {
  error.textContent = '';
  if (!xbePicker.files[0]) {
    error.textContent = 'Choose the matching original XBE first.';
    return;
  }
  let inventory;
  try {
    inventory = dataInventory([...dataPicker.files]);
  } catch (cause) {
    error.textContent = cause instanceof Error ? cause.message : String(cause);
    return;
  }
  if (worker) worker.terminate();
  worker = new Worker('./worker.mjs', { type: 'module' });
  const currentWorker = worker;
  setBusy(true);
  status.textContent = 'Reading the selected XBE locally…';
  currentWorker.addEventListener('message', event => {
    if (worker !== currentWorker) return;
    const message = event.data;
    if (message.type === 'progress') {
      status.textContent = message.text;
    } else if (message.type === 'result') {
      status.textContent = message.text;
      error.textContent = message.error || '';
      setBusy(false);
      currentWorker.terminate();
      worker = undefined;
    }
  });
  currentWorker.addEventListener('error', event => {
    if (worker === currentWorker) failRun(event.message || 'Browser worker failed.');
  });
  currentWorker.postMessage({
    type: 'run',
    xbe: xbePicker.files[0],
    data: { count: inventory.count, totalBytes: inventory.totalBytes, entries: inventory.entries },
    pinned: PINNED_XBE,
  });
});

stopButton.addEventListener('click', () => {
  worker?.terminate();
  worker = undefined;
  status.textContent = 'Worker stopped by the user.';
  setBusy(false);
});
