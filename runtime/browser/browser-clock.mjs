const TICKS_PER_MS = 10000n;
const EPOCH_OFFSET_MS = 11644473600000n;
const U64_MAX = (1n << 64n) - 1n;

// Clock-only host capability. The caller supplies the already admitted Wasm
// memory; a guest request can write exactly four bounded little-endian words.
export function createProofClock({ monotonic = () => performance.now(), wall = () => Date.now() } = {}) {
  const origin = monotonic();
  if (!Number.isFinite(origin) || origin < 0) throw new Error('Invalid monotonic clock origin');
  let previous = origin, previousTicks = 0n, minimumDelta, samples = 0;
  function sample() {
    const now = monotonic(), date = wall();
    const elapsedTicks = Math.floor((now - origin) * Number(TICKS_PER_MS));
    if (!Number.isFinite(now) || now < previous || !Number.isSafeInteger(elapsedTicks) || elapsedTicks < 0 ||
        !Number.isSafeInteger(date)) throw new Error('Invalid browser clock sample');
    const interrupt = BigInt(elapsedTicks), system = (BigInt(date) + EPOCH_OFFSET_MS) * TICKS_PER_MS;
    if (interrupt > U64_MAX || system < 0n || system > U64_MAX) throw new Error('Clock exceeds 64-bit range');
    const delta = interrupt - previousTicks;
    if (delta > 0n && (minimumDelta === undefined || delta < minimumDelta)) minimumDelta = delta;
    previous = now; previousTicks = interrupt; samples++;
    return { interrupt, system };
  }
  return {
    sample,
    write(buffer, address) {
      address >>>= 0;
      if (!(buffer instanceof ArrayBuffer) || address % 4 || buffer.byteLength < 16 || address > buffer.byteLength - 16) {
        throw new Error('Clock output lies outside admitted memory');
      }
      const { interrupt, system } = sample(), view = new DataView(buffer, address, 16);
      view.setBigUint64(0, interrupt, true); view.setBigUint64(8, system, true);
    },
    describe() {
      return { kind: 'browser-clock', monotonic: 'performance.now', system: 'Date.now; UTC epoch 1601',
        samples, minimumObservedPositiveTicks: minimumDelta?.toString() ?? null,
        limitation: 'Observed sample deltas are not timer resolution or original-console/suspend fidelity.' };
    }
  };
}
