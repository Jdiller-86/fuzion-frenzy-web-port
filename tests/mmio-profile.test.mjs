import assert from 'node:assert/strict';
import test from 'node:test';
import { parseLocalMmioProfile, describeMmioExperiments } from '../src/runtime/local-title-proof.mjs';

const reached = [
  { address: 0xFD001804, initial: 0x10 },
  { address: 0xFD600140, initial: 0x20 },
];
const bytes = (registers, edit = {}) => new TextEncoder().encode(JSON.stringify({
  kind: 'local-console-settings', synthetic: true, miscFlags: 0,
  device: {
    kind: 'synthetic-register-storage', synthetic: true,
    window: { base: 0xFD000000, size: 0x00800000 }, registers, ...edit,
  },
})).buffer;

test('an explicit register profile accepts 2-64 distinct entries', () => {
  assert.deepEqual(parseLocalMmioProfile(bytes(reached)).registers, reached);
  const three = [...reached, { address: 0xFD009140, initial: 0x30 }];
  assert.deepEqual(parseLocalMmioProfile(bytes(three)).registers, three);
  const extras = Array.from({ length: 62 }, (_, i) => ({ address: 0xFD000000 + i * 4, initial: i }));
  const sixtyFour = [...reached, ...extras];
  assert.equal(parseLocalMmioProfile(bytes(sixtyFour)).registers.length, 64);
});

test('explicit zero ranges are preserved and bounded', () => {
  const profile = parseLocalMmioProfile(bytes(reached, { zeroRanges: [{ address: 0xFD100200, bytes: 0x1000 }] }));
  assert.deepEqual(profile.zeroRanges, [{ address: 0xFD100200, bytes: 0x1000 }]);
  assert.deepEqual(parseLocalMmioProfile(bytes(reached)).zeroRanges, []);
  const five = Array.from({ length: 5 }, (_, i) => ({ address: 0xFD100000 + i * 0x1000, bytes: 0x100 }));
  assert.equal(parseLocalMmioProfile(bytes(reached, { zeroRanges: five })).zeroRanges.length, 5);
  const bad = [
    [{ address: 0xFD100202, bytes: 0x100 }],
    [{ address: 0xFD100200, bytes: 2 }],
    [{ address: 0xFC000000, bytes: 0x100 }],
    [{ address: 0xFD7FFFF0, bytes: 0x20 }],
    Array.from({ length: 9 }, (_, i) => ({ address: 0xFD100000 + i * 0x1000, bytes: 0x100 })),
    [{ address: 0xFD100200, bytes: 0x100 }, { address: 0xFD100200, bytes: 0x200 }],
  ];
  for (const zeroRanges of bad) assert.throws(() => parseLocalMmioProfile(bytes(reached, { zeroRanges })));
});

test('an explicit register profile admits the 16 MiB NV2A aperture window', () => {
  const wide = bytes(reached, { window: { base: 0xFD000000, size: 0x1000000 } });
  assert.equal(parseLocalMmioProfile(wide).window.size, 0x1000000);
  const user = [...reached, { address: 0xFD800040, initial: 0x10 }];
  const profile = parseLocalMmioProfile(bytes(user, {
    window: { base: 0xFD000000, size: 0x1000000 },
    zeroRanges: [{ address: 0xFD800000, bytes: 0x1000 }],
  }));
  assert.deepEqual(profile.zeroRanges, [{ address: 0xFD800000, bytes: 0x1000 }]);
  assert.throws(() => parseLocalMmioProfile(bytes(reached, { window: { base: 0xFD000000, size: 0x1000004 } })));
});

test('experimental register controls default off and require an explicit true', () => {
  assert.equal(parseLocalMmioProfile(bytes(reached)).writeClearing, false);
  assert.equal(parseLocalMmioProfile(bytes(reached)).pmcZero, false);
  assert.equal(parseLocalMmioProfile(bytes(reached)).pcrtcVblanks, 0);
  assert.equal(parseLocalMmioProfile(bytes(reached, { writeClearing: true })).writeClearing, true);
  assert.equal(parseLocalMmioProfile(bytes(reached, { pmcZero: true })).pmcZero, true);
  for (const value of [1, 'true', {}, []]) {
    assert.equal(parseLocalMmioProfile(bytes(reached, { writeClearing: value })).writeClearing, false);
    assert.equal(parseLocalMmioProfile(bytes(reached, { pmcZero: value })).pmcZero, false);
  }
  /* Declared display events are an explicit, bounded count. */
  assert.equal(parseLocalMmioProfile(bytes(reached, { pcrtcVblanks: 8 })).pcrtcVblanks, 8);
  for (const value of [0.5, -1, 65, '8', true, {}]) {
    assert.equal(parseLocalMmioProfile(bytes(reached, { pcrtcVblanks: value })).pcrtcVblanks, 0);
  }
  /* The engine-wait diagnostic is a literal opt-in as well. */
  assert.equal(parseLocalMmioProfile(bytes(reached, { engineWait: true })).engineWait, true);
  for (const value of [1, 'true', {}, []]) {
    assert.equal(parseLocalMmioProfile(bytes(reached, { engineWait: value })).engineWait, false);
  }
  /* The PFIFO reference reset state is a separate literal opt-in. */
  assert.equal(parseLocalMmioProfile(bytes(reached)).pfifoReset, false);
  assert.equal(parseLocalMmioProfile(bytes(reached, { pfifoReset: true })).pfifoReset, true);
  for (const value of [1, 'true', {}, []]) {
    assert.equal(parseLocalMmioProfile(bytes(reached, { pfifoReset: value })).pfifoReset, false);
  }
});

test('the PFIFO reset experiment is a default-off flag that leaves the register list unchanged', () => {
  const off = parseLocalMmioProfile(bytes(reached));
  assert.equal(off.pfifoReset, false);
  const on = parseLocalMmioProfile(bytes(reached, { pfifoReset: true }));
  assert.equal(on.pfifoReset, true);
  assert.deepEqual(on.registers, reached);
});

test('the AV pack input is explicit and validated', () => {
  assert.equal(parseLocalMmioProfile(bytes(reached)).avPack, undefined);
  assert.equal(parseLocalMmioProfile(bytes(reached, {
    avPack: { pack: 'hdtv', standard: 'ntsc-m', refresh: 60 },
  })).avPack, 0x00400104);
  const bad = [{}, { pack: 'hdtv' }, { pack: 'hdmi', standard: 'ntsc-m', refresh: 60 },
    { pack: 'hdtv', standard: 'ntsc-m', refresh: 59 }];
  for (const avPack of bad) assert.throws(() => parseLocalMmioProfile(bytes(reached, { avPack })));
});

test('unknown device profile keys fail closed', () => {
  assert.throws(() => parseLocalMmioProfile(bytes(reached, { pmcZer0: true })));
  assert.throws(() => parseLocalMmioProfile(bytes(reached, { writeClearing: true, engineWait: true, pmcZer0: true })));
  assert.throws(() => parseLocalMmioProfile(bytes(reached, { extra: 1 })));
  /* Every supported key is accepted together, except the two mutually exclusive diagnostics. */
  const all = parseLocalMmioProfile(bytes(reached, {
    zeroRanges: [{ address: 0xFD100200, bytes: 0x1000 }],
    writeClearing: true, pmcZero: true, pcrtcVblanks: 4, pfifoReset: true,
    avPack: { pack: 'hdtv', standard: 'ntsc-m', refresh: 60 },
  }));
  assert.equal(all.writeClearing, true);
  assert.equal(all.pmcZero, true);
  assert.equal(all.pcrtcVblanks, 4);
  assert.equal(all.engineWait, false);
  assert.equal(all.pfifoReset, true);
  assert.equal(all.avPack, 0x00400104);
});

test('pfifoReset and engineWait are mutually exclusive', () => {
  assert.throws(() => parseLocalMmioProfile(bytes(reached, { pfifoReset: true, engineWait: true })),
    /mutually exclusive/);
  assert.equal(parseLocalMmioProfile(bytes(reached, { pfifoReset: true })).engineWait, false);
  assert.equal(parseLocalMmioProfile(bytes(reached, { engineWait: true })).pfifoReset, false);
});

test('experimental device inputs are summarised for the report', () => {
  const none = parseLocalMmioProfile(bytes(reached));
  assert.deepEqual(describeMmioExperiments(none), []);
  assert.deepEqual(describeMmioExperiments(null), []);
  const all = parseLocalMmioProfile(bytes(reached, {
    writeClearing: true, pmcZero: true, pcrtcVblanks: 4, pfifoReset: true,
    avPack: { pack: 'hdtv', standard: 'ntsc-m', refresh: 60 },
  }));
  assert.deepEqual(describeMmioExperiments(all), ['writeClearing', 'pmcZero', 'pcrtcVblanks:4', 'pfifoReset', 'avPack']);
  const one = parseLocalMmioProfile(bytes(reached, { pcrtcVblanks: 2 }));
  assert.deepEqual(describeMmioExperiments(one), ['pcrtcVblanks:2']);
});

test('register profiles reject missing, duplicate, unaligned, out-of-window or excessive entries', () => {
  const bad = [
    [reached[0]],
    [...reached, ...Array.from({ length: 63 }, (_, i) => ({ address: 0xFD000000 + i * 4, initial: i }))],
    [...reached, reached[0]],
    [reached[0], { address: 0xFD000000, initial: 0 }],
    [...reached, { address: 0xFD000002, initial: 0 }],
    [...reached, { address: 0xFC000000, initial: 0 }],
    [...reached, { address: 0xFD000004, initial: -1 }],
    [...reached, { address: 0xFD000004, initial: 0x100000000 }],
  ];
  for (const registers of bad) assert.throws(() => parseLocalMmioProfile(bytes(registers)));
  assert.throws(() => parseLocalMmioProfile(bytes(reached, { synthetic: false })));
  assert.throws(() => parseLocalMmioProfile(bytes(reached, { kind: 'other' })));
});
