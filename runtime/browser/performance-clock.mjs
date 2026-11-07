const HEADER_INDEX = 0xffffffff;
const CAPACITY = 64;
const TICKS_PER_SECOND = 10000000n;
const U64_MAX = (1n << 64n) - 1n;
const PROFILE_KEYS = ['kind', 'synthetic', 'frequencyHz', 'source', 'origin'];
const STATE_KEYS = [...PROFILE_KEYS, 'capacity', 'status', 'lastSite', 'frequencyQueries', 'admittedSites', 'samples'];

export function parsePerformanceClockProfile(value) {
  if (value === undefined) return undefined;
  if (value === null || typeof value !== 'object' || Array.isArray(value) ||
      Object.keys(value).length !== 5 ||
      !Object.hasOwn(value, 'kind') || !Object.hasOwn(value, 'synthetic') ||
      !Object.hasOwn(value, 'frequencyHz') || !Object.hasOwn(value, 'source') || !Object.hasOwn(value, 'origin') ||
      value.kind !== 'synthetic-performance-clock' || value.synthetic !== true ||
      !Number.isInteger(value.frequencyHz) || value.frequencyHz < 1 || value.frequencyHz > 1_000_000_000 ||
      value.source !== 'browser-monotonic-100ns' || value.origin !== 'proof-clock-zero') {
    throw new Error('Invalid synthetic performance clock profile');
  }
  return {
    kind: 'synthetic-performance-clock', synthetic: true, frequencyHz: value.frequencyHz,
    source: 'browser-monotonic-100ns', origin: 'proof-clock-zero'
  };
}

function readU32(getter, index, field, label) {
  const value = getter(index, field);
  if (!Number.isInteger(value) || value < 0 || value > 0xffffffff) {
    throw new Error(`Invalid performance clock ${label}`);
  }
  return value;
}

export function readPerformanceClockState(getter, profile) {
  if (typeof getter !== 'function') throw new Error('Invalid performance clock state getter');
  const parsed = parsePerformanceClockProfile(profile);
  if (parsed === undefined) throw new Error('Performance clock profile is required');

  const header = field => readU32(getter, HEADER_INDEX, field, `header field ${field}`);
  const ready = header(0), sampleCount = header(2), capacity = header(3), status = header(4);
  const lastSite = header(5), frequencyQueries = header(6), admittedSiteCount = header(7);
  if (ready !== 1 || header(1) !== parsed.frequencyHz || sampleCount > CAPACITY || capacity !== CAPACITY ||
      status > 4 || admittedSiteCount < 1 || admittedSiteCount > CAPACITY) {
    throw new Error('Invalid performance clock state header');
  }

  const admittedSites = [];
  const admitted = new Set();
  for (let index = 0; index < admittedSiteCount; index++) {
    const site = readU32(getter, index, 5, `admitted site ${index}`);
    if (site === 0 || (index > 0 && site <= admittedSites[index - 1])) {
      throw new Error('Invalid performance clock admitted-site order');
    }
    admittedSites.push(site);
    admitted.add(site);
  }
  if (lastSite !== 0 && !admitted.has(lastSite)) throw new Error('Invalid performance clock last site');

  const samples = [];
  let previousTicks = -1n;
  for (let index = 0; index < sampleCount; index++) {
    const site = readU32(getter, index, 0, `sample ${index} site`);
    const ticksLow = readU32(getter, index, 1, `sample ${index} tick low word`);
    const ticksHigh = readU32(getter, index, 2, `sample ${index} tick high word`);
    const counterLow = readU32(getter, index, 3, `sample ${index} counter low word`);
    const counterHigh = readU32(getter, index, 4, `sample ${index} counter high word`);
    if (!admitted.has(site)) throw new Error('Performance clock sample site is not admitted');
    const ticks = (BigInt(ticksHigh) << 32n) | BigInt(ticksLow);
    const counter = (BigInt(counterHigh) << 32n) | BigInt(counterLow);
    const expectedCounter = (ticks * BigInt(parsed.frequencyHz)) / TICKS_PER_SECOND;
    if (ticks < previousTicks || counter !== expectedCounter) {
      throw new Error('Invalid performance clock sample order or counter value');
    }
    previousTicks = ticks;
    samples.push({ site, interruptTicks: ticks.toString(), counter: counter.toString() });
  }

  return {
    ...parsed, capacity: CAPACITY, status, lastSite, frequencyQueries, admittedSites, samples
  };
}

function exactKeys(value, keys, label) {
  if (value === null || typeof value !== 'object' || Array.isArray(value)) throw new Error(`Invalid serialized performance clock ${label}`);
  const actual = Object.keys(value);
  if (actual.length !== keys.length || keys.some(key => !Object.hasOwn(value, key)) || actual.some(key => !keys.includes(key))) {
    throw new Error(`Invalid serialized performance clock ${label} shape`);
  }
}

function boundedArray(value, maximum, minimum, label) {
  if (!Array.isArray(value) || value.length < minimum || value.length > maximum || Object.keys(value).length !== value.length) {
    throw new Error(`Invalid serialized performance clock ${label}`);
  }
  return value;
}

function parseU64Decimal(value, label) {
  if (typeof value !== 'string' || value.length>20 || !/^(0|[1-9][0-9]*)$/.test(value)) throw new Error(`Invalid serialized performance clock ${label}`);
  const parsed = BigInt(value);
  if (parsed > U64_MAX) throw new Error(`Serialized performance clock ${label} exceeds u64`);
  return parsed;
}

function sameFields(actual, expected, keys) {
  return keys.every(key => actual[key] === expected[key]);
}

export function validatePerformanceClockState(state, profile) {
  const selected = parsePerformanceClockProfile(profile);
  if (selected === undefined) throw new Error('Performance clock profile is required');
  exactKeys(state, STATE_KEYS, 'state');
  if (!sameFields(state, selected, PROFILE_KEYS)) throw new Error('Serialized performance clock profile does not match selected profile');
  if (state.capacity !== CAPACITY || !Number.isInteger(state.status) || state.status < 0 || state.status > 4 ||
      !Number.isInteger(state.lastSite) || state.lastSite < 0 || state.lastSite > 0xffffffff ||
      !Number.isInteger(state.frequencyQueries) || state.frequencyQueries < 0 || state.frequencyQueries > 0xffffffff) {
    throw new Error('Invalid serialized performance clock header fields');
  }
  const admittedSites = boundedArray(state.admittedSites, CAPACITY, 1, 'admitted sites');
  const samples = boundedArray(state.samples, CAPACITY, 0, 'samples');
  const normalizedSamples = samples.map((sample, index) => {
    exactKeys(sample, ['site', 'interruptTicks', 'counter'], `sample ${index}`);
    return {
      site: sample.site,
      ticks: parseU64Decimal(sample.interruptTicks, `sample ${index} ticks`),
      counter: parseU64Decimal(sample.counter, `sample ${index} counter`)
    };
  });
  const header = [1, selected.frequencyHz, samples.length, CAPACITY, state.status, state.lastSite, state.frequencyQueries, admittedSites.length];
  const getter = (index, field) => {
    if (index === HEADER_INDEX) return header[field];
    if (field === 5) return admittedSites[index];
    const sample = normalizedSamples[index];
    if (!sample) return undefined;
    if (field === 0) return sample.site;
    if (field === 1) return Number(sample.ticks & 0xffffffffn);
    if (field === 2) return Number((sample.ticks >> 32n) & 0xffffffffn);
    if (field === 3) return Number(sample.counter & 0xffffffffn);
    if (field === 4) return Number((sample.counter >> 32n) & 0xffffffffn);
    return undefined;
  };
  const canonical = readPerformanceClockState(getter, selected);
  if (!sameFields(state, canonical, STATE_KEYS.filter(key => key !== 'admittedSites' && key !== 'samples'))) {
    throw new Error('Serialized performance clock state is inconsistent');
  }
  if (state.admittedSites.length !== canonical.admittedSites.length ||
      state.admittedSites.some((site, index) => site !== canonical.admittedSites[index]) ||
      state.samples.length !== canonical.samples.length ||
      state.samples.some((sample, index) => !sameFields(sample, canonical.samples[index], ['site', 'interruptTicks', 'counter']))) {
    throw new Error('Serialized performance clock state is inconsistent');
  }
  return canonical;
}
