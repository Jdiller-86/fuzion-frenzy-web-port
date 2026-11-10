import createFuzionSdkRuntime from '/artifacts/sdk-browser-objects/fuzion-sdk-title.mjs';
import { loadXbeImage, resolveKernelImports } from './xbe-loader.mjs';

const GUEST_WINDOW_BYTES = 64 * 1024 * 1024;
const STACK_TOP_VA = GUEST_WINDOW_BYTES - 16;
const MAX_DATA_FILES = 16384;
const MAX_DATA_BYTES = 8 * 1024 * 1024 * 1024;
const EXPECTED = {
  imageBase: 0x10000,
  imageEnd: 0x326f20,
  entryPoint: 0x30f7f,
  kernelThunk: 0x253360,
  guestCapacity: 64 * 1024 * 1024,
};

function report(text) {
  postMessage({ type: 'progress', text });
}

function exported(module, name) {
  const value = module[name];
  if (typeof value === 'function') return value();
  if (typeof value === 'number') return value;
  throw new Error(`Compiled module is missing export ${name}`);
}

function verifyRuntimeIdentity(module) {
  const actual = {
    imageBase: exported(module, '_ff_sdk_image_base'),
    imageEnd: exported(module, '_ff_sdk_image_end'),
    entryPoint: exported(module, '_ff_sdk_entrypoint'),
    kernelThunk: exported(module, '_ff_sdk_kernel_thunk'),
    guestCapacity: exported(module, '_ff_sdk_guest_capacity'),
  };
  for (const [key, value] of Object.entries(EXPECTED)) {
    if (actual[key] !== value) throw new Error(`Built module ${key} 0x${Number(actual[key]).toString(16)} differs from this local candidate 0x${value.toString(16)}`);
  }
}

function hex(value) {
  return `0x${Number(value).toString(16).padStart(8, '0')}`;
}

function describeStop(module, status) {
  const details = `stop ${status}; code=${exported(module, '_ff_sdk_last_stop_code')}, address=${hex(exported(module, '_ff_sdk_last_stop_address'))}, ordinal=${exported(module, '_ff_sdk_last_stop_ordinal')}`;
  switch (status) {
    case 0: return `Generated SDK entry returned (${details}). This records guest execution, not a rendered frame.`;
    case 1: return `Guest execution stopped at a kernel/XDK service the current bridge does not implement (${details}). Implemented services ran through their real provider.`;
    case 2: return `Guest execution reached an unsupported translated instruction or trap (${details}).`;
    case 3: return `Guest execution reached an unknown indirect target (${details}).`;
    case 4: return `Guest execution ended in an unhandled SDK resume continuation (${details}).`;
    case 5: return `Guest execution hit the checked guest-memory bound (${details}).`;
    case 6: return `Guest execution reached an unsupported debug service (${details}).`;
    case 7: return `Guest execution stopped with an unexpected runtime exception (${details}).`;
    default: return `Guest execution stopped with an unrecognized runtime status (${details}).`;
  }
}

function validateRelativePath(input) {
  if (typeof input !== 'string' || !input || input.length > 1024 || input.startsWith('/') || input.includes('\\')) {
    throw new Error('Game data contains an invalid relative path.');
  }
  const parts = input.split('/');
  if (parts.length > 32 || parts.some(part => !part || part === '.' || part === '..' || part.includes('\0'))) {
    throw new Error(`Game data path is unsafe: ${input}`);
  }
  return input;
}

function checkedGamePath(input, pathIndex) {
  validateRelativePath(input);
  const parts = input.toLocaleLowerCase('en-US').split('/');
  let prefix = '';
  for (let index = 0; index < parts.length; index++) {
    prefix = prefix ? `${prefix}/${parts[index]}` : parts[index];
    const isFile = index === parts.length - 1;
    if (pathIndex.files.has(prefix) || (isFile && pathIndex.directories.has(prefix))) {
      throw new Error(`Game data contains duplicate or conflicting case-insensitive paths: ${input}`);
    }
    if (isFile) pathIndex.files.add(prefix);
    else pathIndex.directories.add(prefix);
  }
  return input;
}

async function mountLocalFiles(module, xbeFile, xbeSha256, entries) {
  const FS = module.FS;
  if (!FS || typeof FS.mkdir !== 'function' || typeof FS.mount !== 'function' || !module.WORKERFS) {
    throw new Error('Linked SDK module has no exported Emscripten WorkerFS; rebuild with the workerfs library and WORKERFS export.');
  }
  FS.mkdir('/game');
  const blobs = [{ name: 'DEFAULT.XBE', data: xbeFile }];
  const pathIndex = { files: new Set(['default.xbe']), directories: new Set() };
  let total = xbeFile.size;
  let count = 1;
  let sawFolderXbe = false;
  for (const entry of entries) {
    if (!entry || !Number.isSafeInteger(entry.bytes) || entry.bytes < 0 || !entry.file || typeof entry.file.arrayBuffer !== 'function') {
      throw new Error('Selected local data file metadata is invalid.');
    }
    const relativePath = validateRelativePath(entry.path);
    if (relativePath.toLocaleLowerCase('en-US') === 'default.xbe') {
      if (sawFolderXbe) throw new Error('Game data contains duplicate DEFAULT.XBE paths.');
      sawFolderXbe = true;
      if (entry.bytes !== xbeFile.size) throw new Error('Game folder DEFAULT.XBE differs from the separately selected XBE.');
      const digest = new Uint8Array(await crypto.subtle.digest('SHA-256', await entry.file.arrayBuffer()));
      const fileHash = [...digest].map(byte => byte.toString(16).padStart(2, '0')).join('');
      if (fileHash !== xbeSha256) throw new Error('Game folder DEFAULT.XBE differs from the separately selected XBE.');
      continue;
    }
    checkedGamePath(relativePath, pathIndex);
    total += entry.bytes;
    if (!Number.isSafeInteger(total) || total > MAX_DATA_BYTES || ++count > MAX_DATA_FILES) {
      throw new Error('Selected game data exceeds the 8 GiB or 16,384 file limit.');
    }
    blobs.push({ name: relativePath, data: entry.file });
  }
  FS.mount(module.WORKERFS, { blobs }, '/game');
  return { count, total };
}

async function run({ xbe, data, pinned }) {
  if (!(xbe instanceof File)) throw new Error('The worker did not receive a local XBE file.');
  if (xbe.size !== pinned.bytes) throw new Error(`XBE size must be ${pinned.bytes} bytes; selected ${xbe.size}.`);
  if (!data || !Array.isArray(data.entries) || data.count !== data.entries.length || data.count > MAX_DATA_FILES || data.totalBytes > MAX_DATA_BYTES) {
    throw new Error('Selected data inventory exceeds the local bound or is malformed.');
  }

  report('Reading and hashing the selected XBE locally…');
  const xbeBytes = new Uint8Array(await xbe.arrayBuffer());
  const digest = new Uint8Array(await crypto.subtle.digest('SHA-256', xbeBytes));
  const sha256 = [...digest].map(byte => byte.toString(16).padStart(2, '0')).join('');
  if (sha256 !== pinned.sha256) throw new Error(`XBE SHA-256 does not match the pinned original (${sha256}).`);

  report('Loading the linked SDK WebAssembly module…');
  const module = await createFuzionSdkRuntime({
    print: line => report(`SDK: ${line}`),
    printErr: line => report(`SDK diagnostic: ${line}`),
  });
  verifyRuntimeIdentity(module);

  report('Allocating the bounded 64 MiB guest window…');
  const guestBase = module._malloc(GUEST_WINDOW_BYTES);
  if (!guestBase) throw new Error(`SDK module could not allocate the ${GUEST_WINDOW_BYTES} byte guest window.`);
  try {
    if (guestBase + GUEST_WINDOW_BYTES > module.HEAPU8.byteLength) {
      throw new Error('SDK WebAssembly memory does not contain the complete guest allocation.');
    }
    const loaded = loadXbeImage(xbeBytes, module.HEAPU8, { guestBase, expected: EXPECTED });
    const imports = resolveKernelImports(loaded.profile, module.HEAPU8, {
      guestBase,
      resolver: ordinal => 0xfe000000 + ordinal * 4,
    });
    const pendingText = imports.pendingData.length
      ? ` Pending kernel data: ${imports.pendingData.map(item => `${item.name} (#${item.ordinal})`).join(', ')}.`
      : '';
    report(`Mapped XBE ${sha256}; ${loaded.loadedSections.length} preload sections, ${loaded.bytesCopied} bytes copied; resolved ${imports.resolvedFunctions.length} function imports.${pendingText} Mounting ${data.count} local data files…`);
    const mounted = await mountLocalFiles(module, xbe, sha256, data.entries);

    const status = module._ff_sdk_call_guest(guestBase, STACK_TOP_VA, EXPECTED.entryPoint);
    const result = describeStop(module, status);
    const dataImports = imports.pendingData.length
      ? `\nKERNEL_DATA imports remain their original ordinal markers: ${imports.pendingData.map(item => `${item.name} (#${item.ordinal})`).join(', ')}.`
      : '\nNo KERNEL_DATA imports were present.';
    postMessage({ type: 'result', text: `${result}\nResolved ${imports.resolvedFunctions.length} function imports. Mounted ${mounted.count} local game files (${mounted.total} bytes) read-only on demand at /game in this worker; SDK kernel filesystem providers are not connected to the mount yet.${dataImports}` });
  } finally {
    module._free(guestBase);
  }
}

onmessage = async event => {
  if (event.data?.type !== 'run') return;
  try {
    await run(event.data);
  } catch (cause) {
    postMessage({
      type: 'result',
      text: 'SDK browser run did not reach the generated entry.',
      error: cause instanceof Error ? cause.message : String(cause),
    });
  }
};
