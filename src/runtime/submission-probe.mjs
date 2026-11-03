// Bounded, read-only snapshots at the reached PFIFO DMA submission cursor.
// This is not a pushbuffer decoder and never advances DMA_GET or executes a
// command. Bytes are labelled as memory snapshots; unavailable reasons stay
// explicit so a cursor or allocation base is not mistaken for a verified
// command start.
export const SUBMISSION_PROBE_BYTES = 4096;
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

// Bounded nonzero-dword summary per 4 KiB page; it stores counts, not bytes.
function pageCounts(view, ramBytes, base, bytes) {
  if (!view || !Number.isInteger(ramBytes) || ramBytes < 4)
    return { available: false, reason: 'no checked guest memory view' };
  if (!Number.isInteger(base) || (base & 3) || !Number.isInteger(bytes) || bytes < 4 || base > ramBytes - 4)
    return { available: false, reason: 'page range outside aligned guest memory' };
  const pages = Math.min(16, Math.ceil(bytes / 0x1000));
  const rows = [];
  for (let page = 0; page < pages; ++page) {
    const start = base + page * 0x1000;
    const end = Math.min(start + 0x1000, base + bytes, ramBytes);
    let nonzero = 0;
    for (let address = start; address + 4 <= end; address += 4)
      if (view.getUint32(address, true)) ++nonzero;
    rows.push({ offset: page * 0x1000, nonzero });
  }
  return { available: true, pageBytes: 0x1000, pages: rows };
}

// Candidate resolution only: the proof has no established VRAM target backing.
// This never returns bytes and never advances GET.
function resolveDmaOffset(object, offset, ramBytes) {
  if (!object?.available) return { resolved: false, reason: 'DMA object unavailable' };
  if (object.target !== 0) return { resolved: false, reason: `target ${object.targetName} is not modeled` };
  if (!Number.isInteger(offset) || (offset & 3)) return { resolved: false, reason: 'unaligned DMA offset' };
  if (offset > object.limit) return { resolved: false, reason: 'DMA offset beyond object limit' };
  if ((object.base & 3) || offset > 0xFFFFFFFF - object.base)
    return { resolved: false, reason: 'DMA base/offset overflow' };
  const address = (object.base + offset) >>> 0;
  if (!Number.isInteger(ramBytes) || ramBytes < 4 || address > ramBytes - 4)
    return { resolved: false, reason: 'resolved address outside modeled guest RAM' };
  return {
    resolved: true,
    candidate: false,
    verified: true,
    address,
    backing: 'xemu UMA: VRAM is system RAM (identity)',
    reason: 'target backed by guest physical RAM',
  };
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
  const allocationPages = allocation && Number.isInteger(allocation.base) && Number.isInteger(allocation.bytes)
    ? pageCounts(view, ramBytes, allocation.base, allocation.bytes)
    : { available: false, reason: 'no observed GPU-instance allocation' };
  const object = dmaObject(entries);
  return {
    available: true,
    channel: 0,
    put,
    get,
    base: before.base,
    bytes: before.bytes,
    words: before.words,
    allocation: allocationWindow,
    allocationPages,
    dmaObject: object,
    putResolved: resolveDmaOffset(object, put, ramBytes),
    getResolved: resolveDmaOffset(object, get, ramBytes),
    note: 'checked guest-memory snapshots at DMA-object offsets resolved through the xemu UMA identity mapping; not decoded commands',
  };
}
