import { parsePerformanceClockProfile } from './performance-clock.mjs';

const HEADER_INDEX = 0xffffffff;
const CAPACITY = 64;
const TICKS_PER_SECOND = 10_000_000n;
const U64_MAX = (1n << 64n) - 1n;
const PROFILE_KEYS = ['kind', 'synthetic', 'source'];
const STATE_KEYS = [...PROFILE_KEYS, 'frequencyHz', 'count', 'capacity', 'status', 'lastTicks', 'lastCounter'];

export function parsePerformanceCounterProfile(value) {
  if (value === undefined) return undefined;
  if (value === null || typeof value !== 'object' || Array.isArray(value) ||
      Object.keys(value).length !== 3 || PROFILE_KEYS.some(key => !Object.hasOwn(value, key)) ||
      value.kind !== 'synthetic-performance-counter' || value.synthetic !== true ||
      value.source !== 'paired-performance-clock') {
    throw new Error('Invalid synthetic performance counter profile');
  }
  return { kind: 'synthetic-performance-counter', synthetic: true, source: 'paired-performance-clock' };
}

function readU32(getter, index, field, label) {
  const value = getter(index, field);
  if (!Number.isInteger(value) || value < 0 || value > 0xffffffff) {
    throw new Error(`Invalid performance counter ${label}`);
  }
  return value;
}

function wordsToU64(low, high) {
  return (BigInt(high) << 32n) | BigInt(low);
}

export function readPerformanceCounterState(getter, profile, performanceClockProfile) {
  if (typeof getter !== 'function') throw new Error('Invalid performance counter state getter');
  const parsed = parsePerformanceCounterProfile(profile);
  const clock = parsePerformanceClockProfile(performanceClockProfile);
  if (parsed === undefined || clock === undefined) throw new Error('Performance counter and clock profiles are required');

  const header = field => readU32(getter, HEADER_INDEX, field, `header field ${field}`);
  const enabled = header(0), frequencyHz = header(1), count = header(2), capacity = header(3), status = header(4);
  const ticks = wordsToU64(header(5), header(6));
  const counter = wordsToU64(header(7), header(8));
  if (enabled !== 1 || frequencyHz !== clock.frequencyHz || count > CAPACITY || capacity !== CAPACITY || status > 4) {
    throw new Error('Invalid performance counter state header');
  }
  if (count === 0 && (ticks !== 0n || counter !== 0n)) throw new Error('Invalid empty performance counter state');
  if (counter !== (ticks * BigInt(frequencyHz)) / TICKS_PER_SECOND) {
    throw new Error('Invalid performance counter scaling');
  }
  return {
    ...parsed, frequencyHz, count, capacity: CAPACITY, status,
    lastTicks: ticks.toString(), lastCounter: counter.toString()
  };
}

function exactKeys(value, keys, label) {
  if (value === null || typeof value !== 'object' || Array.isArray(value)) throw new Error(`Invalid serialized performance counter ${label}`);
  const actual = Object.keys(value);
  if (actual.length !== keys.length || keys.some(key => !Object.hasOwn(value, key)) || actual.some(key => !keys.includes(key))) {
    throw new Error(`Invalid serialized performance counter ${label} shape`);
  }
}

function parseU64Decimal(value, label) {
  if (typeof value !== 'string' || value.length > 20 || !/^(0|[1-9][0-9]*)$/.test(value)) {
    throw new Error(`Invalid serialized performance counter ${label}`);
  }
  const parsed = BigInt(value);
  if (parsed > U64_MAX) throw new Error(`Serialized performance counter ${label} exceeds u64`);
  return parsed;
}

export function validatePerformanceCounterState(state, profile, performanceClockProfile) {
  const selected = parsePerformanceCounterProfile(profile);
  const clock = parsePerformanceClockProfile(performanceClockProfile);
  if (selected === undefined || clock === undefined) throw new Error('Performance counter and clock profiles are required');
  exactKeys(state, STATE_KEYS, 'state');
  if (PROFILE_KEYS.some(key => state[key] !== selected[key])) throw new Error('Serialized performance counter profile does not match selected profile');
  if (!Number.isInteger(state.frequencyHz) || state.frequencyHz < 0 || state.frequencyHz > 0xffffffff ||
      !Number.isInteger(state.count) || state.count < 0 || state.count > 0xffffffff ||
      !Number.isInteger(state.capacity) || state.capacity < 0 || state.capacity > 0xffffffff ||
      !Number.isInteger(state.status) || state.status < 0 || state.status > 0xffffffff) {
    throw new Error('Invalid serialized performance counter header fields');
  }
  const ticks = parseU64Decimal(state.lastTicks, 'last ticks');
  const counter = parseU64Decimal(state.lastCounter, 'last counter');
  const header = [1, state.frequencyHz, state.count, state.capacity, state.status,
    Number(ticks & 0xffffffffn), Number((ticks >> 32n) & 0xffffffffn),
    Number(counter & 0xffffffffn), Number((counter >> 32n) & 0xffffffffn)];
  const canonical = readPerformanceCounterState((index, field) => index === HEADER_INDEX ? header[field] : undefined, selected, clock);
  if (STATE_KEYS.some(key => state[key] !== canonical[key])) throw new Error('Serialized performance counter state is inconsistent');
  return canonical;
}
