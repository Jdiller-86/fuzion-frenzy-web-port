/* Source-owned names and ABI facts for reached Xbox kernel services.
 *
 * This is metadata only: it never implements a service or supplies a return
 * value. The retained callers evidence names ordinal 2 as
 * AvSendTVEncoderOption. The call site at 0x1FD410 pushes four arguments and a
 * return address, and the following instruction is not an `add esp, imm`; the
 * translator therefore emits RECOMP_ICALL_SAFE_AT (callee-cleans), so the call
 * expects a stdcall/NTAPI callee that pops 16 bytes. The caller reads the
 * 4-byte output word at 0x203720, not an NTSTATUS in EAX, and the service is
 * VOID NTAPI-style. Adding these facts is not an implementation claim.
 */
const SERVICES = new Map([
  [2, {
    name: 'AvSendTVEncoderOption',
    callingConvention: 'stdcall',
    argumentBytes: 16,
    returns: 'void',
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
  };
}
