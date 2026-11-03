// Read-only NV2A pushbuffer header scan. It decodes the documented NV20/NV2A
// command encoding to inventory what a submitted range contains; it never
// executes a method, writes guest memory or advances DMA_GET.
export const PFIFO_SCAN_MAX_WORDS = 4096;

export function scanPushbuffer(view, start, end, { maxWords = PFIFO_SCAN_MAX_WORDS } = {}) {
  const unavailable = reason => ({ available: false, reason });
  if (!view || typeof view.getUint32 !== 'function' || !Number.isInteger(view.byteLength))
    return unavailable('no checked memory view');
  if (!Number.isInteger(start) || !Number.isInteger(end) || (start & 3) || (end & 3))
    return unavailable('unaligned pushbuffer range');
  if (end < start || end - start > 0x04000000) return unavailable('invalid pushbuffer range');
  if (start > view.byteLength - 4 || end > view.byteLength) return unavailable('range outside modeled memory');
  if (!Number.isInteger(maxWords) || maxWords < 1 || maxWords > PFIFO_SCAN_MAX_WORDS)
    return unavailable('invalid scan budget');
  const totalWords = (end - start) / 4;
  const methods = new Map();
  let address = start, scannedWords = 0, pending = 0, unknown = 0, jumps = 0, calls = 0, returns = 0;
  while (scannedWords < maxWords && address + 4 <= end) {
    const word = view.getUint32(address, true);
    scannedWords++;
    address += 4;
    if (pending) { pending--; continue; }
    if ((word & 3) === 1) { jumps++; continue; }
    if ((word & 3) === 2) { calls++; continue; }
    if ((word & 0xFFFF0003) === 0x00020000) { returns++; continue; }
    if ((word & 0xE0030003) === 0 || (word & 0xE0030003) === 0x40000000) {
      const subchannel = (word >>> 13) & 7;
      const method = word & 0x1FFC;
      const count = (word >>> 18) & 0x7FF;
      const key = `${subchannel}:${method}`;
      methods.set(key, (methods.get(key) || 0) + 1);
      pending = count;
      continue;
    }
    unknown++;
  }
  return {
    available: true,
    start,
    end,
    totalWords,
    scannedWords,
    truncated: scannedWords < totalWords,
    pendingParams: pending,
    unknown,
    jumps,
    calls,
    returns,
    methodKinds: methods.size,
    methods: [...methods.entries()].slice(0, 16).map(([key, count]) => {
      const [subchannel, method] = key.split(':').map(Number);
      return { subchannel, method, count };
    }),
    note: 'read-only header scan; no commands executed and GET not advanced',
  };
}
