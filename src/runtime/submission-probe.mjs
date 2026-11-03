// Bounded, read-only snapshots at the reached PFIFO DMA submission cursor.
// This is not a pushbuffer decoder and never advances DMA_GET or executes a
// command. Bytes are labelled as memory snapshots; unavailable reasons stay
// explicit so a cursor or allocation base is not mistaken for a verified
// command start.
export const SUBMISSION_PROBE_BYTES = 256;

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
    note: 'checked guest-memory snapshots at the DMA submission cursor and observed allocation base; not decoded commands',
  };
}
