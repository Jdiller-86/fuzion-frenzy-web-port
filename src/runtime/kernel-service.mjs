/* Source-owned static facts for reached Xbox kernel services.
 *
 * This is metadata only: it never implements a service or supplies a return
 * value. The pinned xboxrecomp-source reference declares ordinal 2 as
 * `VOID __stdcall AvSendTVEncoderOption(PVOID RegisterBase, ULONG Option,
 * ULONG Param, PULONG Result)` (kernel.h:1100) and defines the AV result layout
 * (kernel.h:1124-1145). The static descriptor carries only those service facts:
 * name, calling convention, argument bytes, return style and result bit layout.
 *
 * Observed call-site facts -- the selected option (6), the four arguments and
 * the output pointer/permissions/masks/table at 0x203720 -- are not asserted by
 * this descriptor. They are recorded in the run's call list and in
 * docs/AV_ENCODER_SERVICE_CHECKPOINT.md, so a static service fact cannot be
 * confused with an observation from one call site.
 */
const SERVICES = new Map([
  [2, {
    name: 'AvSendTVEncoderOption',
    callingConvention: 'stdcall',
    argumentBytes: 16,
    returns: 'void',
    resultBits: { pack: 0xff, standard: 0xff00, refresh: 0xc00000 },
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
    resultBits: { ...known.resultBits },
  };
}
