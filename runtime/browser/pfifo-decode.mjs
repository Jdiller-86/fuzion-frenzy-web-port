// Read-only NV2A pushbuffer packet decoder. It walks a bounded, explicitly
// supplied range of little-endian words and reports each command header plus
// its arguments. It never executes a method, follows a jump/call/return,
// touches guest memory outside the supplied view or advances DMA_GET.
//
// Encoding follows the pinned bridge survey (host/kernel/nv2a_pb_scan.c):
//   (w & 0xE0030003) == 0x00000000  increasing methods
//   (w & 0xE0030003) == 0x40000000  non-increasing (same method, count params)
//   (w & 0x00000003) == 0x00000001  jump
//   (w & 0xE0000003) == 0x20000000  ring-jump variant used by the segment scan
//   (w & 0x00000003) == 0x00000002  call
//   (w & 0xFFFF0003) == 0x00020000  return
// Method header fields: count = (w >> 18) & 0x7FF, subchannel = (w >> 13) & 7,
// method = w & 0x1FFC. Targets are reported as encoded; this module does not
// apply the guest physical/virtual window.
export const PFIFO_DECODE_MAX_WORDS = 4096;
export const PFIFO_DECODE_ABSOLUTE_WORDS = 16 * 1024 * 1024;
export const PFIFO_DECODE_MAX_PACKETS = 4096;

const unavailable = reason => ({ available: false, reason });

export function decodePushbuffer(view, start, end, {
  maxWords = PFIFO_DECODE_MAX_WORDS,
  maxPackets = PFIFO_DECODE_MAX_PACKETS,
} = {}) {
  if (!view || typeof view.getUint32 !== 'function' || !Number.isInteger(view.byteLength))
    return unavailable('no checked memory view');
  if (!Number.isInteger(start) || !Number.isInteger(end) || (start & 3) || (end & 3))
    return unavailable('unaligned pushbuffer range');
  if (start < 0 || end < 0) return unavailable('invalid pushbuffer range');
  if (end < start || end - start > 0x04000000) return unavailable('invalid pushbuffer range');
  if (start > view.byteLength || end > view.byteLength) return unavailable('range outside modeled memory');
  if (!Number.isInteger(maxWords) || maxWords < 1 || maxWords > PFIFO_DECODE_ABSOLUTE_WORDS)
    return unavailable('invalid word budget');
  if (!Number.isInteger(maxPackets) || maxPackets < 1 || maxPackets > PFIFO_DECODE_MAX_PACKETS)
    return unavailable('invalid packet budget');

  const totalWords = (end - start) / 4;
  const packets = [];
  let address = start;
  let wordsDecoded = 0, unknown = 0;
  let stopReason = 'complete';

  const push = packet => { packets.push(packet); };

  while (address + 4 <= end && wordsDecoded < maxWords && packets.length < maxPackets) {
    const headerAddress = address;
    const word = view.getUint32(address, true);
    address += 4;
    wordsDecoded++;

    if ((word & 0x00000003) === 0x00000001) {
      push({ type: 'jump', address: headerAddress, header: word, target: word & 0xFFFFFFFC });
      continue;
    }
    if ((word & 0xE0000003) === 0x20000000) {
      push({ type: 'jump', variant: 'ring', address: headerAddress, header: word, target: word & 0x1FFFFFFC });
      continue;
    }
    if ((word & 0x00000003) === 0x00000002) {
      push({ type: 'call', address: headerAddress, header: word, target: word & 0xFFFFFFFC });
      continue;
    }
    if ((word & 0xFFFF0003) === 0x00020000) {
      push({ type: 'return', address: headerAddress, header: word });
      continue;
    }
    const increasing = (word & 0xE0030003) === 0;
    const nonIncreasing = (word & 0xE0030003) === 0x40000000;
    if (increasing || nonIncreasing) {
      const count = (word >>> 18) & 0x7FF;
      const subchannel = (word >>> 13) & 0x7;
      const method = word & 0x1FFC;
      const argumentsDecoded = [];
      let truncated = false;
      for (let i = 0; i < count; ++i) {
        if (address + 4 > end || wordsDecoded >= maxWords) {
          truncated = true;
          break;
        }
        argumentsDecoded.push(view.getUint32(address, true));
        address += 4;
        wordsDecoded++;
      }
      const packet = {
        type: 'method',
        address: headerAddress,
        header: word,
        subchannel,
        method,
        count,
        nonIncreasing,
        arguments: argumentsDecoded,
      };
      packets.push(packet);
      if (truncated) {
        stopReason = 'truncated-arguments';
        break;
      }
      continue;
    }
    unknown++;
    push({ type: 'unknown', address: headerAddress, header: word });
  }

  if (stopReason === 'complete') {
    if (address < end) stopReason = packets.length >= maxPackets ? 'packet-budget' : 'word-budget';
  }
  return {
    available: true,
    start,
    end,
    totalWords,
    wordsDecoded,
    packets,
    packetCount: packets.length,
    unknown,
    truncated: stopReason !== 'complete',
    stopReason,
    note: 'read-only packet decode; control flow not followed and GET not advanced',
  };
}
