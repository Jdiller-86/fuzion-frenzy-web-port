import assert from 'node:assert/strict';
import test from 'node:test';
import { parseLocalMmioProfile } from '../src/runtime/local-title-proof.mjs';

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
