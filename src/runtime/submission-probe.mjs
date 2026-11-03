// Bounded, read-only snapshots at the reached PFIFO DMA submission cursor.
// This is not a pushbuffer decoder and never advances DMA_GET or executes a
// command. Bytes are labelled as memory snapshots; unavailable reasons stay
// explicit so a cursor or allocation base is not mistaken for a verified
// command start.
export const SUBMISSION_PROBE_BYTES = 256;
const PFIFO_INSTANCE = 0xFD00322C;
const RAMIN_BASE = 0xFD700000;
const DMA_CLASS = 0x00000FFF;
const DMA_TARGET = 0x00030000;
const DMA_ADJUST = 0xFFF00000;
const DMA_ADDRESS = 0xFFFFF000;
const targetName = target =>
  target === 0x00000000 ? 'NVM/VRAM' : target === 0x00010000 ? 'NVM tiled'
  : target === 0x00020000 ? 'PCI' : target === 0x00030000 ? 'AGP' : 'unknown';

// The reached PFIFO DMA object: a RAMIN object selected by the instance
// register. PUT/GET are offsets into this object, not guest addresses.
function dmaObject(entries) {
  const value = address => entries.find(entry => entry.address === address)?.value;
  const instanceRegister = value(PFIFO_INSTANCE);
  if (!Number.isInteger(instanceRegister)) return { available: false, reason: 'no observed PFIFO DMA instance register' };
  const objectAddress = ((instanceRegister & 0xFFFF) << 4) >>> 0;
  const flags = value(RAMIN_BASE + objectAddress);
  const limit = value(RAMIN_BASE + objectAddress + 4);
  const frame = value(RAMIN_BASE + objectAddress + 8);
  if (![flags, limit, frame].every(Number.isInteger))
    return { available: false, reason: 'no complete DMA object in the observed RAMIN entries' };
  const target = (flags & DMA_TARGET) >>> 0;
  const adjust = (flags & DMA_ADJUST) >>> 0;
  const base = (((frame & DMA_ADDRESS) | adjust) & 0x07FFFFFF) >>> 0;
  return {
    available: true,
    instanceRegister: instanceRegister >>> 0,
    objectAddress,
    flags: flags >>> 0,
    dmaClass: (flags & DMA_CLASS) >>> 0,
    target,
    targetName: targetName(target),
    adjust,
    base,
    limit: limit >>> 0,
    note: 'PUT/GET are offsets into the DMA object; the mapped target buffer is not modeled',
  };
}

function snapshot(view, ramBytes, base, bytes) {
  if (!view || !Number.isInteger(ramBytes) || ramBytes < 4)
    return { available: false, reason: 'no checked guest memory view' };
  if (!Number.isInteger(base) || (base & 3) || base > ramBytes - 4)
    return { available: false, reason: 'window outside aligned guest memory' };
  if (!Number.isInteger(bytes) || bytes < 4 || bytes > SUBMISSION_PROBE_BYTES || (bytes & 3))
    return { available: false, reason: 'invalid snapshot size' };
  const words = [];
  for (let offset = 0; offset < bytes; offset += 4) words.push(view.getUint32(base + offset, true));
  return { available: true, base, bytes, words };
}

export function createSubmissionProbe({ status, entries, view, ramBytes, windowBytes = SUBMISSION_PROBE_BYTES, allocation } = {}) {
  const unavailable = reason => ({ available: false, reason });
  if (status !== 13) return unavailable('not stopped at the PFIFO DMA submission boundary');
  const putEntry = entries?.find(entry => entry.address === 0xFD800040);
  if (!putEntry) return unavailable('no observed DMA_PUT write');
  if (!view || !Number.isInteger(ramBytes) || ramBytes < 4) return unavailable('no checked guest memory view');
  if (!Number.isInteger(windowBytes) || windowBytes < 4 || windowBytes > SUBMISSION_PROBE_BYTES || (windowBytes & 3))
    return unavailable('invalid snapshot size');
  const put = putEntry.value >>> 0;
  if ((put & 3) || put > ramBytes - 4) return unavailable('DMA_PUT outside aligned guest memory');
  const getEntry = entries.find(entry => entry.address === 0xFD800044);
  const get = getEntry ? getEntry.value >>> 0 : 0;
  const base = put >= windowBytes ? put - windowBytes : 0;
  const before = snapshot(view, ramBytes, base, windowBytes);
  if (!before.available) return unavailable(before.reason);
  const allocationWindow = allocation && Number.isInteger(allocation.base)
    ? snapshot(view, ramBytes, allocation.base, Math.min(windowBytes, allocation.bytes ?? windowBytes))
    : { available: false, reason: 'no observed GPU-instance allocation' };
  return {
    available: true,
    channel: 0,
    put,
    get,
    base: before.base,
    bytes: before.bytes,
    words: before.words,
    allocation: allocationWindow,
    dmaObject: dmaObject(entries),
    note: 'checked guest-memory snapshots at the raw cursor and allocation addresses; PUT/GET are DMA-object offsets and the mapped buffer is not modeled',
  };
}
