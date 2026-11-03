// Bounded, read-only snapshot at the reached PFIFO DMA submission cursor.
// This is not a pushbuffer decoder and never advances DMA_GET or executes a
// command. Bytes are labelled as a memory snapshot; unavailable reasons stay
// explicit so a candidate cursor is not mistaken for a verified command start.
export const SUBMISSION_PROBE_BYTES = 256;

export function createSubmissionProbe({ status, entries, view, ramBytes, windowBytes = SUBMISSION_PROBE_BYTES } = {}) {
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
  const words = [];
  for (let offset = 0; offset < windowBytes; offset += 4) words.push(view.getUint32(base + offset, true));
  return {
    available: true,
    channel: 0,
    put,
    get,
    base,
    bytes: windowBytes,
    words,
    note: 'checked guest-memory snapshot at the DMA submission cursor; not decoded commands',
  };
}
