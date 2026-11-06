// Read-only NV2A pushbuffer ring pointer model. The caller supplies explicit
// base, end, put and get values; this module validates and describes them. It
// never writes guest memory, advances DMA_GET, follows a jump or decides that
// the ring is empty/full. Empty/wrap semantics are not inferred from pointers.
//
// Observed from the title's D3D device code (private read-only analysis):
// - CDevice_MakeSpace writes a wrap jump as (base & 0x0FFFFFFF) + 1.
// - CDevice_KickOff writes DMA_PUT as put & 0x03FFFFFF.
// - the context reset starts the push pointer at the ring base and an empty
//   buffer therefore has put at the base; GET is separately initialized to 0.
export const PFIFO_RING_MAX_BYTES = 64 * 1024 * 1024;
export const PFIFO_RING_ADDRESS_MASK = 0x03FFFFFF;
export const PFIFO_RING_JUMP_MASK = 0x0FFFFFFF;

const unavailable = reason => ({ available: false, reason });

export function encodeRingJump(base) {
  if (!Number.isInteger(base) || base < 0 || base > 0xFFFFFFFF)
    throw new Error('ring base must be a 32-bit unsigned integer');
  return ((base & PFIFO_RING_JUMP_MASK) | 1) >>> 0;
}

export function describePfifoRing({ base, end, put, get, ramBytes } = {}) {
  if (![base, end, put, get, ramBytes].every(Number.isInteger))
    return unavailable('ring values must be integers');
  if (ramBytes < 4 || ramBytes > PFIFO_RING_MAX_BYTES) return unavailable('invalid ring memory size');
  if ((base & 3) || (end & 3) || (put & 3) || (get & 3)) return unavailable('unaligned ring pointer');
  if (base < 0 || end <= base || end > ramBytes) return unavailable('invalid ring bounds');
  if (end - base > PFIFO_RING_MAX_BYTES) return unavailable('ring exceeds the bounded size');
  if (put < 0 || put > ramBytes) return unavailable('put outside ring memory');
  if (get < 0 || get > ramBytes) return unavailable('get outside ring memory');
  return {
    available: true,
    base: base >>> 0,
    end: end >>> 0,
    put: put >>> 0,
    get: get >>> 0,
    ringBytes: end - base,
    jumpWord: encodeRingJump(base),
    putPhysical: put & PFIFO_RING_ADDRESS_MASK,
    getPhysical: get & PFIFO_RING_ADDRESS_MASK,
    putInsideRing: put >= base && put <= end,
    getInsideRing: get >= base && get <= end,
    zeroGet: get === 0,
    samePointer: put === get,
    note: 'explicit pointer model only; empty/wrap semantics are not inferred and GET is not advanced',
  };
}
