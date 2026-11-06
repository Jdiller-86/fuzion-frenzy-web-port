/* Bounded device-aperture report mapping. The guest module owns the actual
 * capacities; these constants must stay in sync with FF_DEVICE_ENTRIES and
 * FF_DEVICE_EVENTS in device_registers.h. Reporting every recorded entry/event
 * avoids silently hiding the next original device sequence behind an arbitrary
 * eight- or sixteen-item cap. */
export const DEVICE_ENTRIES = 8192;
export const DEVICE_EVENTS = 32768;
/* Must stay in sync with FF_DEVICE_STATUS_WAITS in device_registers.h. */
export const DEVICE_STATUS_WAITS = 4;

const DWORD = value => Number.isInteger(value) && value >= 0 && value <= 0xffffffff;

/* Validate the supplied device-input profile carried in a proof report. It is
 * the window/register/zero-range set the worker parsed, so a hand-written or
 * truncated report cannot claim an input profile the runtime did not use. The
 * bounds mirror parseLocalMmioProfile; the report contract and the controlled
 * comparison share this one check so they cannot drift apart. */
export function validateSuppliedDeviceProfile(supplied) {
  if (!supplied || typeof supplied !== 'object' || Array.isArray(supplied))
    throw new Error('Malformed supplied device input profile');
  const window = supplied.window;
  if (!window || !DWORD(window.base) || !DWORD(window.size) ||
      (window.base & 3) || (window.size & 3) ||
      window.size < 4096 || window.size > 0x1000000)
    throw new Error('Malformed supplied device window');
  const registers = supplied.registers ?? [];
  const zeroRanges = supplied.zeroRanges ?? [];
  if (!Array.isArray(registers) || registers.length < 2 || registers.length > 64 ||
      !Array.isArray(zeroRanges) || zeroRanges.length > 8)
    throw new Error('Malformed supplied device input lists');
  const seen = new Set();
  for (const entry of registers) {
    if (!entry || !DWORD(entry.address) || !DWORD(entry.initial) ||
        (entry.address & 3) || entry.address < window.base ||
        entry.address - window.base >= window.size || seen.has(entry.address))
      throw new Error('Malformed supplied device register');
    seen.add(entry.address);
  }
  /* The explicit profile requires the two reached device addresses. */
  if (!seen.has(0xfd001804) || !seen.has(0xfd600140))
    throw new Error('Malformed supplied device register');
  const zeroSeen = new Set();
  for (const range of zeroRanges) {
    if (!range || !DWORD(range.address) || !DWORD(range.bytes) ||
        (range.address & 3) || (range.bytes & 3) || range.bytes < 4 ||
        range.address < window.base ||
        range.bytes > window.size - (range.address - window.base) ||
        zeroSeen.has(range.address))
      throw new Error('Malformed supplied device zero range');
    zeroSeen.add(range.address);
  }
  return supplied;
}

export function createDeviceReport(api, profile = null) {
  if (!api || typeof api.ff_probe_mmio_word !== 'function' ||
      typeof api.ff_probe_mmio_event !== 'function') {
    throw new Error('Missing bounded device-aperture accessors');
  }
  const count = api.ff_probe_mmio_word(3) >>> 0;
  const events = api.ff_probe_mmio_word(4) >>> 0;
  if (count > DEVICE_ENTRIES || events > DEVICE_EVENTS) {
    throw new Error('Device report exceeds the bounded aperture capacity');
  }
  const entries = [];
  for (let i = 0; i < count; ++i) {
    entries.push({
      address: api.ff_probe_mmio_word(5 + i * 4) >>> 0,
      value: api.ff_probe_mmio_word(6 + i * 4) >>> 0,
      reads: api.ff_probe_mmio_word(7 + i * 4) >>> 0,
      writes: api.ff_probe_mmio_word(8 + i * 4) >>> 0,
    });
  }
  const log = [];
  for (let i = 0; i < events; ++i) {
    const site = api.ff_probe_mmio_event(i, 4) >>> 0;
    log.push({
      address: api.ff_probe_mmio_event(i, 0) >>> 0,
      direction: api.ff_probe_mmio_event(i, 1) >>> 0,
      value: api.ff_probe_mmio_event(i, 2) >>> 0,
      beforeKnown: api.ff_probe_mmio_event(i, 3) >>> 0,
      site: site === 0xFFFFFFFF ? null : site,
    });
  }
  const aliases = [];
  if (typeof api.ff_probe_mmio_alias_count === 'function' &&
      typeof api.ff_probe_mmio_alias_word === 'function') {
    const count = api.ff_probe_mmio_alias_count() >>> 0;
    if (count > 4) throw new Error('Device report exceeds the bounded alias capacity');
    for (let i = 0; i < count; ++i) aliases.push({
      first: api.ff_probe_mmio_alias_word(i, 0) >>> 0,
      second: api.ff_probe_mmio_alias_word(i, 1) >>> 0,
    });
  }
  const statusWaits = [];
  if (typeof api.ff_probe_device_status_wait_count === 'function' &&
      typeof api.ff_probe_device_status_wait_word === 'function') {
    const count = api.ff_probe_device_status_wait_count() >>> 0;
    if (count > DEVICE_STATUS_WAITS) throw new Error('Device report exceeds the bounded status-wait capacity');
    for (let i = 0; i < count; ++i) statusWaits.push({
      address: api.ff_probe_device_status_wait_word(i, 0) >>> 0,
      mask: api.ff_probe_device_status_wait_word(i, 1) >>> 0,
      value: api.ff_probe_device_status_wait_word(i, 2) >>> 0,
      count: api.ff_probe_device_status_wait_word(i, 3) >>> 0,
    });
  }
  return {
    scope: 'explicit synthetic aperture; unknown reads fail; one modeled self-clearing flush bit; no general NV2A or device behaviour',
    base: api.ff_probe_mmio_word(1) >>> 0,
    size: api.ff_probe_mmio_word(2) >>> 0,
    count,
    events,
    entries,
    log,
    aliases,
    statusWaits,
    supplied: profile ? {
      window: { base: profile.window.base, size: profile.window.size },
      registers: profile.registers.map(entry => ({ address: entry.address, initial: entry.initial })),
      zeroRanges: (profile.zeroRanges ?? []).map(range => ({ address: range.address, bytes: range.bytes })),
    } : null,
  };
}
