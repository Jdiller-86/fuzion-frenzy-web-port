import assert from 'node:assert/strict';
import test from 'node:test';
import {
  describePfifoRing,
  encodeRingJump,
  PFIFO_RING_MAX_BYTES,
  PFIFO_RING_ADDRESS_MASK,
  PFIFO_RING_JUMP_MASK,
} from '../runtime/browser/pfifo-ring.mjs';

test('the observed pushbuffer example reports the jump word, physical masks and zero get', () => {
  const ring = describePfifoRing({ base: 0x03B7D000, end: 0x03C00000, put: 0x03B7D000, get: 0, ramBytes: 0x04000000 });
  assert.equal(ring.available, true);
  assert.equal(ring.ringBytes, 0x83000);
  assert.equal(ring.jumpWord, 0x03B7D001);
  assert.equal(ring.putPhysical, 0x03B7D000);
  assert.equal(ring.getPhysical, 0);
  assert.equal(ring.putInsideRing, true);
  assert.equal(ring.getInsideRing, false);
  assert.equal(ring.zeroGet, true);
  assert.equal(ring.samePointer, false);
  assert.match(ring.note, /not inferred/);
});

test('the jump word keeps only the low 28 bits and always sets bit zero', () => {
  assert.equal(encodeRingJump(0x87654320), 0x07654321);
  assert.equal(encodeRingJump(0x00001000), 0x00001001);
  assert.equal(PFIFO_RING_JUMP_MASK, 0x0FFFFFFF);
  assert.equal(PFIFO_RING_ADDRESS_MASK, 0x03FFFFFF);
});

test('a ring with both pointers inside is described without claiming emptiness', () => {
  const ring = describePfifoRing({ base: 0x1000, end: 0x2000, put: 0x1800, get: 0x1400, ramBytes: 0x4000 });
  assert.equal(ring.putInsideRing, true);
  assert.equal(ring.getInsideRing, true);
  assert.equal(ring.samePointer, false);
  assert.equal(ring.zeroGet, false);
});

test('a put at the base is reported as put-inside without inferring an empty ring', () => {
  const ring = describePfifoRing({ base: 0x1000, end: 0x2000, put: 0x1000, get: 0x1000, ramBytes: 0x4000 });
  assert.equal(ring.putInsideRing, true);
  assert.equal(ring.samePointer, true);
  assert.equal(ring.ringBytes, 0x1000);
});

test('invalid sizes, alignment, bounds and pointers fail closed', () => {
  assert.deepEqual(describePfifoRing({}), { available: false, reason: 'ring values must be integers' });
  assert.deepEqual(describePfifoRing({ base: 0x1000, end: 0x2000, put: 0x1000, get: 0, ramBytes: 0 }), { available: false, reason: 'invalid ring memory size' });
  assert.deepEqual(describePfifoRing({ base: 0x1000, end: 0x2000, put: 0x1000, get: 0, ramBytes: PFIFO_RING_MAX_BYTES + 4 }), { available: false, reason: 'invalid ring memory size' });
  assert.deepEqual(describePfifoRing({ base: 0x1002, end: 0x2000, put: 0x1000, get: 0, ramBytes: 0x4000 }), { available: false, reason: 'unaligned ring pointer' });
  assert.deepEqual(describePfifoRing({ base: 0x2000, end: 0x2000, put: 0x2000, get: 0, ramBytes: 0x4000 }), { available: false, reason: 'invalid ring bounds' });
  assert.deepEqual(describePfifoRing({ base: -4, end: 0x2000, put: 0x1000, get: 0, ramBytes: 0x4000 }), { available: false, reason: 'invalid ring bounds' });
  assert.deepEqual(describePfifoRing({ base: 0x1000, end: 0x2000, put: 0x4004, get: 0, ramBytes: 0x4000 }), { available: false, reason: 'put outside ring memory' });
  assert.deepEqual(describePfifoRing({ base: 0x1000, end: 0x2000, put: 0x1000, get: 0x4004, ramBytes: 0x4000 }), { available: false, reason: 'get outside ring memory' });
  assert.throws(() => encodeRingJump(-4), /32-bit/);
  assert.throws(() => encodeRingJump(0x100000000), /32-bit/);
});
