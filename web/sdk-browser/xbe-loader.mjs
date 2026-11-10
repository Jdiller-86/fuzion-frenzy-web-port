import { readXbeProfile } from '../../host/xbe.mjs';

const MAX_IMAGE_BYTES = 128 * 1024 * 1024;
const HEADER_BYTES_MIN = 0x178;
// KERNEL_DATA declarations in the pinned SDK runtime/kernel/xboxkrnl.cpp.
export const SDK_KERNEL_DATA = Object.freeze({
  16: 'ExEventObjectType',
  22: 'ExMutantObjectType',
  30: 'ExSemaphoreObjectType',
  31: 'ExTimerObjectType',
  40: 'HalDiskCachePartitionCount',
  41: 'HalDiskModelNumber',
  42: 'HalDiskSerialNumber',
  64: 'IoCompletionObjectType',
  70: 'IoDeviceObjectType',
  71: 'IoFileObjectType',
  88: 'KdDebuggerEnabled',
  89: 'KdDebuggerNotPresent',
  120: 'KeInterruptTime',
  154: 'KeSystemTime',
  156: 'KeTickCount',
  157: 'KeTimeIncrement',
  164: 'LaunchDataPage',
  240: 'ObDirectoryObjectType',
  249: 'ObSymbolicLinkObjectType',
  259: 'PsThreadObjectType',
  321: 'XboxEEPROMKey',
  322: 'XboxHardwareInfo',
  323: 'XboxHDKey',
  324: 'XboxKrnlVersion',
  325: 'XboxSignatureKey',
  326: 'XeImageFileName',
  353: 'XboxLANKey',
  354: 'XboxAlternateSignatureKeys',
  355: 'XePublicKeyData',
  356: 'HalBootSMCVideoMode',
  357: 'IdexChannelObject',
});

function fail(message) {
  throw new Error(message);
}

function inputBytes(input) {
  if (input instanceof ArrayBuffer) return new Uint8Array(input);
  if (input instanceof Uint8Array) return input;
  fail('XBE input must be an ArrayBuffer or Uint8Array');
}

function memoryBytes(memory) {
  if (memory instanceof WebAssembly.Memory) return new Uint8Array(memory.buffer);
  if (memory instanceof Uint8Array) return memory;
  fail('Guest memory must be a WebAssembly.Memory or Uint8Array');
}

function validateExpected(profile, expected) {
  if (expected === undefined) return;
  if (!expected || typeof expected !== 'object') fail('Expected image identity must be an object');
  const checks = [
    ['base', 'imageBase'],
    ['entry', 'entryPoint'],
    ['kernelThunk', 'kernelThunk'],
  ];
  for (const [actualKey, expectedKey] of checks) {
    if (expected[expectedKey] !== undefined &&
        (!Number.isInteger(expected[expectedKey]) || expected[expectedKey] < 0 || expected[expectedKey] > 0xffffffff)) {
      fail(`Compiled ${expectedKey} must be a 32-bit unsigned integer`);
    }
    if (expected[expectedKey] !== undefined && profile[actualKey] !== expected[expectedKey]) {
      fail(`XBE ${actualKey} 0x${profile[actualKey].toString(16)} does not match compiled ${expectedKey} 0x${Number(expected[expectedKey]).toString(16)}`);
    }
  }
  if (expected.imageEnd !== undefined &&
      (!Number.isInteger(expected.imageEnd) || expected.imageEnd < 0 || expected.imageEnd > 0xffffffff)) {
    fail('Compiled imageEnd must be a 32-bit unsigned integer');
  }
  if (expected.imageEnd !== undefined && profile.base + profile.imageBytes !== expected.imageEnd) {
    fail(`XBE image end 0x${(profile.base + profile.imageBytes).toString(16)} does not match compiled imageEnd 0x${Number(expected.imageEnd).toString(16)}`);
  }
}

/**
 * Parse and map the header plus preload sections using the pinned SDK boot layout.
 * guestBase is the byte offset corresponding to guest virtual address zero, matching
 * the SDK's `HostPtr(va) == g_base + va` convention. No memory is grown implicitly.
 */
export function loadXbeImage(input, memory, { guestBase = 0, expected } = {}) {
  const source = inputBytes(input);
  if (source.byteLength > 16 * 1024 * 1024) fail('XBE exceeds the 16 MiB input limit');
  if (!Number.isSafeInteger(guestBase) || guestBase < 0) fail('guestBase must be a nonnegative safe integer');

  // The metadata reader accepts an ArrayBuffer and validates header, sections,
  // entry point, kernel thunk, TLS, and import bounds before any guest memory write.
  const stableInput = source.slice();
  const profile = readXbeProfile(stableInput.buffer);
  if (profile.variant === 'beta') fail('Beta XBE profile is not supported by the pinned SDK loader');
  validateExpected(profile, expected);
  if (profile.imageBytes > MAX_IMAGE_BYTES) fail('XBE image exceeds the 128 MiB image limit');
  if (profile.headerBytes < HEADER_BYTES_MIN || profile.headerBytes > stableInput.byteLength) {
    fail('XBE header size is outside the supported input range');
  }

  const imageEnd = profile.base + profile.imageBytes;
  const reserveStart = profile.base & ~0xffff;
  const reserveEnd = Math.ceil(imageEnd / 0x10000) * 0x10000;
  const targetStart = guestBase + profile.base;
  const targetReserveStart = guestBase + reserveStart;
  const targetEnd = guestBase + reserveEnd;
  if (!Number.isSafeInteger(targetEnd) || targetEnd > 0x100000000) {
    fail('XBE image range exceeds the 32-bit guest address space');
  }
  const target = memoryBytes(memory);
  if (targetEnd > target.byteLength) {
    fail(`Guest memory too small: image requires ${targetEnd} bytes through offset 0x${targetEnd.toString(16)}, has ${target.byteLength} bytes`);
  }

  // Match kernel::Boot: ReserveImage commits a zero-filled image range, then
  // copies the header and only sections carrying kPreload (flag bit 1).
  target.fill(0, targetReserveStart, targetEnd);
  target.set(stableInput.subarray(0, profile.headerBytes), targetStart);
  const sourceView = new DataView(stableInput.buffer, stableInput.byteOffset, stableInput.byteLength);
  const sectionTableVA = sourceView.getUint32(0x120, true);
  const targetView = new DataView(target.buffer, target.byteOffset, target.byteLength);
  const loadedSections = [];
  let bytesCopied = profile.headerBytes;
  for (const section of profile.sections) {
    if (!(section.flags & 2)) continue;
    const count = Math.min(section.fileBytes, section.virtualBytes);
    const start = guestBase + section.address;
    target.set(stableInput.subarray(section.fileOffset, section.fileOffset + count), start);
    // The SDK marks preloaded section headers resident (header +0x18).
    targetView.setUint32(guestBase + sectionTableVA + section.index * 0x38 + 0x18, 1, true);
    loadedSections.push({ index: section.index, address: section.address, bytes: count });
    bytesCopied += count;
  }

  return {
    profile,
    guestBase,
    imageStart: targetStart,
    imageEnd: guestBase + imageEnd,
    reservedEnd: targetEnd,
    requiredMemoryBytes: targetEnd,
    bytesCopied,
    loadedSections,
  };
}

/** Validate an XBE and report the required memory extent without modifying memory. */
export function inspectXbeImage(input, { guestBase = 0, expected } = {}) {
  const source = inputBytes(input);
  if (source.byteLength > 16 * 1024 * 1024) fail('XBE exceeds the 16 MiB input limit');
  if (!Number.isSafeInteger(guestBase) || guestBase < 0) fail('guestBase must be a nonnegative safe integer');
  const profile = readXbeProfile(source.slice().buffer);
  if (profile.variant === 'beta') fail('Beta XBE profile is not supported by the pinned SDK loader');
  validateExpected(profile, expected);
  if (profile.imageBytes > MAX_IMAGE_BYTES) fail('XBE image exceeds the 128 MiB image limit');
  const imageEnd = guestBase + Math.ceil((profile.base + profile.imageBytes) / 0x10000) * 0x10000;
  if (!Number.isSafeInteger(imageEnd) || imageEnd > 0x100000000) fail('XBE image range exceeds the 32-bit guest address space');
  return { profile, guestBase, requiredMemoryBytes: imageEnd };
}

/**
 * Resolve XBE kernel-import slots after image mapping, like SDK kernel::Boot.
 * Function imports use a caller-owned guest-thunk resolver. Known KERNEL_DATA
 * imports are intentionally left at their original encoded value until a real
 * data provider supplies an address.
 */
export function resolveKernelImports(profile, memory, { guestBase = 0, resolver, dataResolver } = {}) {
  if (!profile || !Array.isArray(profile.imports) || profile.imports.length > 512) {
    fail('XBE kernel import profile is missing or exceeds the 512-slot bound');
  }
  if (!Number.isSafeInteger(guestBase) || guestBase < 0) fail('guestBase must be a nonnegative safe integer');
  if (typeof resolver !== 'function' && profile.imports.some(item => !SDK_KERNEL_DATA[item?.ordinal])) {
    fail('A kernel function-import resolver is required');
  }
  if (!Number.isSafeInteger(profile.base) || profile.base < 0 ||
      !Number.isSafeInteger(profile.imageBytes) || profile.imageBytes < 0 ||
      profile.base + profile.imageBytes > 0x100000000) {
    fail('XBE kernel import profile has an invalid image range');
  }

  const target = memoryBytes(memory);
  const targetView = new DataView(target.buffer, target.byteOffset, target.byteLength);
  const imageEnd = profile.base + profile.imageBytes;
  const seenSlots = new Set();
  const writes = [];
  const pendingData = [];
  const resolvedData = [];
  for (const item of profile.imports) {
    if (!item || !Number.isInteger(item.ordinal) || item.ordinal < 1 || item.ordinal > 0xffff ||
        !Number.isInteger(item.address) || item.address < profile.base || item.address > imageEnd - 4 || item.address % 4) {
      fail('XBE kernel import contains an invalid ordinal or unaligned slot address');
    }
    const slotKey = item.address;
    if (seenSlots.has(slotKey)) fail('XBE kernel import profile repeats a slot address');
    seenSlots.add(slotKey);
    const offset = guestBase + item.address;
    if (!Number.isSafeInteger(offset) || offset < 0 || offset > target.byteLength - 4) {
      fail(`Kernel import slot 0x${item.address.toString(16)} exceeds guest memory`);
    }
    const original = targetView.getUint32(offset, true);
    const expectedOriginal = (0x80000000 | item.ordinal) >>> 0;
    if (original !== expectedOriginal) {
      fail(`Mapped kernel import slot 0x${item.address.toString(16)} does not contain its original ordinal marker`);
    }
    const dataName = SDK_KERNEL_DATA[item.ordinal];
    if (dataName) {
      if (typeof dataResolver !== 'function') {
        pendingData.push({ ordinal: item.ordinal, name: dataName, address: item.address });
        continue;
      }
      const guestVA = dataResolver(item.ordinal, item);
      if (guestVA === undefined || guestVA === null) {
        pendingData.push({ ordinal: item.ordinal, name: dataName, address: item.address });
        continue;
      }
      if (!Number.isInteger(guestVA) || guestVA <= 0 || guestVA > 0xffffffff) {
        fail(`Kernel data resolver returned no valid guest VA for ordinal ${item.ordinal}`);
      }
      writes.push({ offset, address: item.address, ordinal: item.ordinal, guestVA, kind: 'data' });
      resolvedData.push({ address: item.address, ordinal: item.ordinal, name: dataName, guestVA });
      continue;
    }
    const guestVA = resolver(item.ordinal, item);
    if (!Number.isInteger(guestVA) || guestVA <= 0 || guestVA > 0xffffffff) {
      fail(`Kernel function resolver returned no valid guest VA for ordinal ${item.ordinal}`);
    }
    writes.push({ offset, address: item.address, ordinal: item.ordinal, guestVA, kind: 'function' });
  }

  for (const write of writes) targetView.setUint32(write.offset, write.guestVA >>> 0, true);
  return {
    resolvedFunctions: writes.filter(write => write.kind === 'function').map(({ address, ordinal, guestVA }) => ({ address, ordinal, guestVA })),
    resolvedData,
    pendingData,
  };
}
