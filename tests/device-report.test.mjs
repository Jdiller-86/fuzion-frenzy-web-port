import assert from 'node:assert/strict';
import test from 'node:test';
import { createDeviceReport, DEVICE_ENTRIES, DEVICE_EVENTS } from '../src/runtime/device-report.mjs';

function fake(count, events, site = 0x00100020) {
  return {
    ff_probe_mmio_word(field) {
      if (field === 1) return 0xFD000000;
      if (field === 2) return 0x00800000;
      if (field === 3) return count;
      if (field === 4) return events;
      const entry = Math.floor((field - 5) / 4), part = (field - 5) % 4;
      return [0xFD000000 + entry * 4, 0x10 + entry, entry + 1, entry + 2][part];
    },
    ff_probe_mmio_event(index, field) {
      return [0xFD000000 + index * 4, index % 2 ? 2 : 1, 0x20 + index, index % 2, site][field];
    },
  };
}

test('device reports retain every bounded entry and event', () => {
  const report = createDeviceReport(fake(17, 9));
  assert.equal(report.base, 0xFD000000);
  assert.equal(report.size, 0x00800000);
  assert.equal(report.count, 17);
  assert.equal(report.events, 9);
  assert.equal(report.entries.length, 17);
  assert.deepEqual(report.entries[16], { address: 0xFD000040, value: 0x20, reads: 17, writes: 18 });
  assert.equal(report.log.length, 9);
  assert.deepEqual(report.log[0], { address: 0xFD000000, direction: 1, value: 0x20, beforeKnown: 0, site: 0x00100020 });
  assert.deepEqual(report.log[1], { address: 0xFD000004, direction: 2, value: 0x21, beforeKnown: 1, site: 0x00100020 });
});

test('an older module without event site provenance reports null', () => {
  const report = createDeviceReport(fake(1, 1, 0xFFFFFFFF));
  assert.equal(report.log[0].site, null);
});

test('reports reject counts beyond the aperture capacities', () => {
  assert.throws(() => createDeviceReport(fake(DEVICE_ENTRIES + 1, 0)), /bounded aperture capacity/);
  assert.throws(() => createDeviceReport(fake(0, DEVICE_EVENTS + 1)), /bounded aperture capacity/);
});

test('reports require the bounded accessors', () => {
  assert.throws(() => createDeviceReport(null), /Missing bounded device-aperture accessors/);
  assert.throws(() => createDeviceReport({}), /Missing bounded device-aperture accessors/);
});
