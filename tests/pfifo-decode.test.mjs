import assert from 'node:assert/strict';
import test from 'node:test';
import {
  decodePushbuffer,
  PFIFO_DECODE_MAX_WORDS,
  PFIFO_DECODE_ABSOLUTE_WORDS,
  PFIFO_DECODE_MAX_PACKETS,
} from '../runtime/browser/pfifo-decode.mjs';

const words = (...values) => {
  const view = new DataView(new ArrayBuffer(values.length * 4));
  values.forEach((value, index) => view.setUint32(index * 4, value >>> 0, true));
  return view;
};

const method = (subchannel, count, firstMethod, nonIncreasing = false) => (
  (nonIncreasing ? 0x40000000 : 0) | ((count & 0x7FF) << 18) |
  ((subchannel & 7) << 13) | (firstMethod & 0x1FFC)
) >>> 0;

test('an increasing method reports its header and ordered arguments', () => {
  const view = words(method(2, 3, 0x100), 0xAAAA0001, 0xAAAA0002, 0xAAAA0003);
  const result = decodePushbuffer(view, 0, view.byteLength);
  assert.equal(result.available, true);
  assert.equal(result.packetCount, 1);
  assert.equal(result.wordsDecoded, 4);
  assert.equal(result.unknown, 0);
  assert.equal(result.truncated, false);
  assert.equal(result.stopReason, 'complete');
  const [packet] = result.packets;
  assert.equal(packet.type, 'method');
  assert.equal(packet.address, 0);
  assert.equal(packet.subchannel, 2);
  assert.equal(packet.method, 0x100);
  assert.equal(packet.count, 3);
  assert.equal(packet.nonIncreasing, false);
  assert.deepEqual(packet.arguments, [0xAAAA0001, 0xAAAA0002, 0xAAAA0003]);
});

test('a non-increasing method keeps one method for every parameter', () => {
  const view = words(method(1, 2, 0x200, true), 7, 8);
  const [packet] = decodePushbuffer(view, 0, view.byteLength).packets;
  assert.equal(packet.type, 'method');
  assert.equal(packet.nonIncreasing, true);
  assert.equal(packet.method, 0x200);
  assert.equal(packet.count, 2);
  assert.deepEqual(packet.arguments, [7, 8]);
});

test('jump, ring-jump, call, return and unknown headers stay separate and are not followed', () => {
  const view = words(
    0x12345679, // jump, target 0x12345678
    0x20000104, // ring-jump variant, target 0x104
    0x30000002, // call, target 0x30000000
    0x00020000, // return
    0xFFFFFFFF, // unknown
  );
  const result = decodePushbuffer(view, 0, view.byteLength);
  assert.deepEqual(result.packets.map(packet => packet.type), ['jump', 'jump', 'call', 'return', 'unknown']);
  assert.equal(result.packets[0].target, 0x12345678);
  assert.equal(result.packets[0].variant, undefined);
  assert.equal(result.packets[1].variant, 'ring');
  assert.equal(result.packets[1].target, 0x104);
  assert.equal(result.packets[2].target, 0x30000000);
  assert.equal(result.unknown, 1);
  assert.equal(result.packets[4].address, 16);
});

test('a method with a truncated argument list stops without inventing words', () => {
  const view = words(method(0, 4, 0x100), 1, 2);
  const result = decodePushbuffer(view, 0, view.byteLength);
  assert.equal(result.truncated, true);
  assert.equal(result.stopReason, 'truncated-arguments');
  assert.equal(result.wordsDecoded, 3);
  assert.equal(result.packets[0].count, 4);
  assert.deepEqual(result.packets[0].arguments, [1, 2]);
});

test('word and packet budgets stop deterministically', () => {
  const view = words(method(0, 1, 0x100), 1, method(0, 1, 0x200), 2);
  const byWords = decodePushbuffer(view, 0, view.byteLength, { maxWords: 2 });
  assert.equal(byWords.stopReason, 'word-budget');
  assert.equal(byWords.wordsDecoded, 2);
  assert.equal(byWords.packetCount, 1);

  const byPackets = decodePushbuffer(view, 0, view.byteLength, { maxPackets: 1 });
  assert.equal(byPackets.stopReason, 'packet-budget');
  assert.equal(byPackets.packetCount, 1);
});

test('a zero-count method consumes only its header', () => {
  const view = words(method(3, 0, 0x180), 0x00000001);
  const result = decodePushbuffer(view, 0, view.byteLength);
  assert.equal(result.packets.length, 2);
  assert.equal(result.packets[0].type, 'method');
  assert.equal(result.packets[0].count, 0);
  assert.deepEqual(result.packets[0].arguments, []);
  assert.equal(result.packets[1].type, 'jump');
});

test('an empty range succeeds without reading memory', () => {
  const view = words(0);
  const result = decodePushbuffer(view, 4, 4);
  assert.equal(result.available, true);
  assert.equal(result.packetCount, 0);
  assert.equal(result.wordsDecoded, 0);
  assert.equal(result.truncated, false);
  assert.equal(result.stopReason, 'complete');
});

test('the decoder never mutates the supplied words', () => {
  const view = words(method(0, 2, 0x100), 0x11111111, 0x22222222);
  const before = new Uint8Array(view.buffer.slice(0));
  decodePushbuffer(view, 0, view.byteLength);
  assert.deepEqual(new Uint8Array(view.buffer), before);
});

test('invalid views, ranges and budgets fail closed', () => {
  const view = words(0, 0);
  assert.deepEqual(decodePushbuffer(null, 0, 8), { available: false, reason: 'no checked memory view' });
  assert.deepEqual(decodePushbuffer({}, 0, 8), { available: false, reason: 'no checked memory view' });
  assert.deepEqual(decodePushbuffer(view, 2, 8), { available: false, reason: 'unaligned pushbuffer range' });
  assert.deepEqual(decodePushbuffer(view, 0, 7), { available: false, reason: 'unaligned pushbuffer range' });
  assert.deepEqual(decodePushbuffer(view, -4, 4), { available: false, reason: 'invalid pushbuffer range' });
  assert.deepEqual(decodePushbuffer(view, 0, -4), { available: false, reason: 'invalid pushbuffer range' });
  assert.deepEqual(decodePushbuffer(view, 8, 4), { available: false, reason: 'invalid pushbuffer range' });
  assert.deepEqual(decodePushbuffer(view, 0, 0x04000004), { available: false, reason: 'invalid pushbuffer range' });
  assert.deepEqual(decodePushbuffer(view, 0, 12), { available: false, reason: 'range outside modeled memory' });
  assert.deepEqual(decodePushbuffer(view, 0, 8, { maxWords: 0 }), { available: false, reason: 'invalid word budget' });
  assert.deepEqual(decodePushbuffer(view, 0, 8, { maxWords: PFIFO_DECODE_ABSOLUTE_WORDS + 1 }), { available: false, reason: 'invalid word budget' });
  assert.deepEqual(decodePushbuffer(view, 0, 8, { maxPackets: 0 }), { available: false, reason: 'invalid packet budget' });
  assert.deepEqual(decodePushbuffer(view, 0, 8, { maxPackets: PFIFO_DECODE_MAX_PACKETS + 1 }), { available: false, reason: 'invalid packet budget' });
  assert.equal(PFIFO_DECODE_MAX_WORDS, 4096);
  assert.equal(PFIFO_DECODE_ABSOLUTE_WORDS, 16 * 1024 * 1024);
});
