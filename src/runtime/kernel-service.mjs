/* Source-owned static facts for reached Xbox kernel services.
 *
 * This is metadata only: it never implements a service or supplies a return
 * value. The pinned xboxrecomp-source reference declares ordinal 2 as
 * `VOID __stdcall AvSendTVEncoderOption(PVOID RegisterBase, ULONG Option,
 * ULONG Param, PULONG Result)` (kernel.h:1100), and the call site evidences the
 * stdcall/16-byte/void convention.
 *
 * The descriptor deliberately carries only the facts that are not specific to
 * one observed option: name and ABI. The option-6/result-bit semantics and the
 * output pointer/permissions/masks/table are observed call facts; they live in
 * the run's call list and docs/RESEARCH.md#av-services-and-provenance, with the
 * unresolved Cxbx provenance. A static service fact cannot be confused with an
 * observation, and option 6 is not asserted as a service-wide property.
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
