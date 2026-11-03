/* Bounded device-aperture report mapping. The guest module owns the actual
 * capacities; these constants must stay in sync with FF_DEVICE_ENTRIES and
 * FF_DEVICE_EVENTS in device_registers.h. Reporting every recorded entry/event
 * avoids silently hiding the next original device sequence behind an arbitrary
 * eight- or sixteen-item cap. */
export const DEVICE_ENTRIES = 8192;
export const DEVICE_EVENTS = 32768;

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
  return {
    scope: 'explicit synthetic aperture; unknown reads fail; one modeled self-clearing flush bit; no general NV2A or device behaviour',
    base: api.ff_probe_mmio_word(1) >>> 0,
    size: api.ff_probe_mmio_word(2) >>> 0,
    count,
    events,
    entries,
    log,
    supplied: profile ? {
      window: { base: profile.window.base, size: profile.window.size },
      registers: profile.registers.map(entry => ({ address: entry.address, initial: entry.initial })),
      zeroRanges: (profile.zeroRanges ?? []).map(range => ({ address: range.address, bytes: range.bytes })),
    } : null,
  };
}
