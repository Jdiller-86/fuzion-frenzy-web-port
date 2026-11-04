import test from 'node:test';
import assert from 'node:assert/strict';
import { validateAvObservation } from '../src/runtime/av-observation.mjs';

const before = ['0x00', '0x00', '0x00', '0x00'];
const after = ['0x04', '0x01', '0x40', '0x00'];
const record = (edit = {}) => ({
  schema: 'av-observation/1',
  source: 'original-hardware',
  game: { sha256: 'a'.repeat(64), name: 'Fuzion Frenzy (USA)' },
  kernel: { version: '1.0.5838.1' },
  console: { videoConfiguration: 'NTSC-M, HDTV, 60 Hz', avPack: 'HDTV', cable: 'component' },
  call: { registerBase: 0, option: 6, param: 0, resultAddress: '0x203720' },
  before,
  after,
  returnedNormally: true,
  collection: 'instrumented original console',
  instrumentation: 'capture probe attached before the call',
  confirmations: [{ before, after, returnedNormally: true }],
  ...edit,
});

test('a complete original-hardware observation is accepted and reports the measured word', () => {
  const result = validateAvObservation(record());
  assert.equal(result.word, 0x00400104);
  assert.equal(result.beforeWord, 0);
  assert.equal(result.changed, true);
  assert.equal(result.consumerDomainMatch, true);
  assert.equal(result.confirmations.length, 1);
  assert.deepEqual(result.notes, []);
});

test('a measured word outside the consumer domain is retained as evidence, not rejected', () => {
  const zero = ['0x00', '0x00', '0x00', '0x00'];
  const result = validateAvObservation(record({
    after: zero,
    confirmations: [{ before, after: zero, returnedNormally: true }],
  }));
  assert.equal(result.word, 0);
  assert.equal(result.changed, false);
  assert.equal(result.consumerDomainMatch, false);
  assert.ok(result.notes.some(note => /zero/.test(note)));
  assert.ok(result.notes.some(note => /outside/.test(note)));
  assert.ok(result.notes.some(note => /did not change/.test(note)));
});

test('only the exact original-hardware call shape is accepted', () => {
  assert.throws(() => validateAvObservation(record({ schema: 'av-observation/2' })), /schema/);
  assert.throws(() => validateAvObservation(record({ source: 'emulator' })), /original-hardware/);
  assert.throws(() => validateAvObservation(record({
    call: { registerBase: 1, option: 6, param: 0, resultAddress: '0x203720' },
  })), /\(0, 6, 0, Result\)/);
  assert.throws(() => validateAvObservation(record({
    call: { registerBase: 0, option: 6, param: 0, resultAddress: '0x203722' },
  })), /aligned/);
  assert.throws(() => validateAvObservation(record({
    call: { registerBase: 0, option: 6, param: 0, resultAddress: '0x0' },
  })), /non-null/);
});

test('missing identity, configuration, bytes and normal return fail closed', () => {
  assert.throws(() => validateAvObservation(record({ game: { name: 'Fuzion Frenzy' } })), /SHA-256/);
  assert.throws(() => validateAvObservation(record({ kernel: { version: '' } })), /kernel version/);
  assert.throws(() => validateAvObservation(record({
    console: { videoConfiguration: '', avPack: 'HDTV', cable: 'component' },
  })), /video configuration/);
  assert.throws(() => validateAvObservation(record({ before: ['0x00'] })), /exactly four/);
  assert.throws(() => validateAvObservation(record({ after: ['0x00', '0x00', '0x00', '0xGG'] })), /0xNN/);
  assert.throws(() => validateAvObservation(record({ returnedNormally: false })), /returned normally/);
  assert.throws(() => validateAvObservation(record({ confirmations: [] })), /one to four/);
});

test('a differing confirmation requires and records its variation note', () => {
  const differing = ['0x03', '0x01', '0x40', '0x00'];
  assert.throws(() => validateAvObservation(record({
    confirmations: [{ before, after: differing, returnedNormally: true }],
  })), /variation note/);
  const result = validateAvObservation(record({
    confirmations: [{
      before, after: differing, returnedNormally: true,
      variationNote: 'Second capture selected a SCART entry; configuration recorded as changed.',
    }],
  }));
  assert.equal(result.word, 0x00400104);
  assert.equal(result.confirmations[0].word, 0x00400103);
  assert.equal(result.confirmations[0].variation, true);
  assert.ok(result.notes.some(note => /confirmation differs/.test(note)));
});
