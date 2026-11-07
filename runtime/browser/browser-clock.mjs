const TICKS_PER_MS = 10000n;
const EPOCH_OFFSET_MS = 11644473600000n;
const U64_MAX = (1n << 64n) - 1n;
const SYSTEM_CLOCK_MAX_UNIX_MILLIS = 253402300799999;

export function parseClockPauseProfile(value) {
  if (value === undefined) return undefined;
  if (value === null || typeof value !== 'object' || Array.isArray(value) || Object.keys(value).length !== 4 ||
      !Object.hasOwn(value, 'kind') || !Object.hasOwn(value, 'synthetic') || !Object.hasOwn(value, 'beforeSample') || !Object.hasOwn(value, 'milliseconds') ||
      value.kind !== 'one-shot-browser-clock-pause' || value.synthetic !== true ||
      !Number.isInteger(value.beforeSample) || value.beforeSample < 1 || value.beforeSample > 16384 ||
      !Number.isInteger(value.milliseconds) || value.milliseconds < 1 || value.milliseconds > 1000) {
    throw new Error('Invalid synthetic one-shot browser clock pause profile');
  }
  return { kind: 'one-shot-browser-clock-pause', synthetic: true, beforeSample: value.beforeSample, milliseconds: value.milliseconds };
}

export function parseSystemClockProfile(value) {
  if (value === undefined) return undefined;
  if (value === null || typeof value !== 'object' || Array.isArray(value) ||
      Object.keys(value).length !== 3 ||
      !Object.hasOwn(value, 'kind') || !Object.hasOwn(value, 'synthetic') || !Object.hasOwn(value, 'unixMillis') ||
      value.kind !== 'fixed-system-clock' || value.synthetic !== true ||
      !Number.isSafeInteger(value.unixMillis) || value.unixMillis < 0 || value.unixMillis > SYSTEM_CLOCK_MAX_UNIX_MILLIS) {
    throw new Error('Invalid explicit synthetic fixed system clock profile');
  }
  return { kind: 'fixed-system-clock', synthetic: true, unixMillis: value.unixMillis };
}

// Clock-only host capability. The caller supplies the already admitted Wasm
// memory; a guest request can write exactly four bounded little-endian words.
export function createProofClock({ monotonic = () => performance.now(), wall = () => Date.now(), fixedSystemProfile, pauseProfile, pauseWait } = {}) {
  const fixedSystem = parseSystemClockProfile(fixedSystemProfile);
  const pause = parseClockPauseProfile(pauseProfile);
  if(pause!==undefined && pauseWait!==undefined && typeof pauseWait!=='function')throw new Error('Invalid browser clock pause wait capability');
  if (pause !== undefined && pauseWait === undefined) {
    if (typeof WorkerGlobalScope === 'undefined' || !(globalThis instanceof WorkerGlobalScope) || typeof SharedArrayBuffer !== 'function' ||
        typeof Atomics !== 'object' || typeof Atomics.wait !== 'function') throw new Error('Browser clock pause requires worker SharedArrayBuffer Atomics.wait support');
    const probe = new Int32Array(new SharedArrayBuffer(4));
    if (Atomics.wait(probe, 0, 0, 0) !== 'timed-out') throw new Error('Browser clock pause capability probe failed');
  }
  const wait = pauseWait ?? (milliseconds => {
    const cell = new Int32Array(new SharedArrayBuffer(4));
    return Atomics.wait(cell, 0, 0, milliseconds);
  });
  const pauseState = { performed: false, beforeSample: pause?.beforeSample ?? null, milliseconds: pause?.milliseconds ?? null, beforeTicks: null, afterTicks: null };
  const origin = monotonic();
  if (!Number.isFinite(origin) || origin < 0) throw new Error('Invalid monotonic clock origin');
  let previous = origin, previousTicks = 0n, minimumDelta, samples = 0;
  function sample() {
    if (pause !== undefined && !pauseState.performed && samples + 1 === pause.beforeSample) {
      const before = monotonic();
      if (!Number.isFinite(before) || before < previous) throw new Error('Invalid browser clock pause start');
      const result = wait(pause.milliseconds);
      const after = monotonic();
      if (result !== 'timed-out') throw new Error('Browser clock pause did not time out');
      if (!Number.isFinite(after) || after < before || !Number.isSafeInteger(Math.floor((before - origin) * Number(TICKS_PER_MS))) ||
          !Number.isSafeInteger(Math.floor((after - origin) * Number(TICKS_PER_MS)))) throw new Error('Invalid browser clock pause measurement');
      const beforeTicks = Math.floor((before - origin) * Number(TICKS_PER_MS));
      const afterTicks = Math.floor((after - origin) * Number(TICKS_PER_MS));
      if (beforeTicks < 0 || afterTicks < beforeTicks || afterTicks - beforeTicks < pause.milliseconds * Number(TICKS_PER_MS)) throw new Error('Browser clock pause undershot requested duration');
      pauseState.beforeTicks = String(beforeTicks); pauseState.afterTicks = String(afterTicks); pauseState.performed = true;
      previous=after;
    }
    const now = monotonic(), date = fixedSystem === undefined ? wall() : fixedSystem.unixMillis;
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
      const description = { kind: 'browser-clock', monotonic: 'performance.now',
        system: fixedSystem === undefined ? 'Date.now; UTC epoch 1601' : 'configured synthetic fixed Unix time; UTC epoch 1601',
        samples, minimumObservedPositiveTicks: minimumDelta?.toString() ?? null,
        limitation: 'Observed sample deltas are not timer resolution or original-console/suspend fidelity.' };
      if (fixedSystem !== undefined) description.fixedSystemProfile = { ...fixedSystem };
      if (pause !== undefined) { description.pauseProfile = { ...pause }; description.pauseState = { ...pauseState }; }
      return description;
    }
  };
}
