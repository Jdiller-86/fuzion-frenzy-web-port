/* Source-owned names and ABI/output facts for reached Xbox kernel services.
 *
 * This is metadata only: it never implements a service or supplies a return
 * value. The retained callers evidence names ordinal 2 as
 * AvSendTVEncoderOption. The call site at 0x1FD410 pushes four arguments and a
 * return address, and the following instruction is not an `add esp, imm`; the
 * translator therefore emits RECOMP_ICALL_SAFE_AT (callee-cleans), so the call
 * expects a stdcall/NTAPI callee that pops 16 bytes. The return is VOID
 * NTAPI-style. The caller reads a 4-byte output word at 0x203720, masks only
 * 0xFF and 0xFF00, and scans a 0xBF-entry table at 0x202C38 with stride 0xC;
 * bits 16-31 are not consumed on this path. These are observed facts, not an
 * implementation claim or a capability result.
 */
const SERVICES = new Map([
  [2, {
    name: 'AvSendTVEncoderOption',
    callingConvention: 'stdcall',
    argumentBytes: 16,
    returns: 'void',
    output: {
      address: 0x203720,
      bytes: 4,
      writable: true,
      masks: [0xff, 0xff00],
      table: { base: 0x202c38, entries: 0xbf, stride: 0xc },
    },
  }],
]);

export function describeKernelService(ordinal) {
  if (!Number.isInteger(ordinal) || ordinal < 0 || ordinal > 0xffff)
    return { ordinal, name: null, modeled: false };
  const known = SERVICES.get(ordinal);
  if (!known) return { ordinal, name: null, modeled: false };
  return {
    ordinal,
    name: known.name,
    modeled: false,
    callingConvention: known.callingConvention,
    argumentBytes: known.argumentBytes,
    returns: known.returns,
    output: { ...known.output, masks: [...known.output.masks], table: { ...known.output.table } },
  };
}
