/* Source-owned static facts for reached Xbox kernel services.
 *
 * This is metadata only: it never implements a service or supplies a return
 * value. Ordinals 126/127 have zero-argument, 64-bit performance-clock ABIs;
 * its descriptor supplies no frequency or timing model. The pinned
 * xboxrecomp-source reference declares ordinal 2 as
 * `VOID __stdcall AvSendTVEncoderOption(PVOID RegisterBase, ULONG Option,
 * ULONG Param, PULONG Result)` (kernel.h:1100), and the call site evidences the
 * stdcall/16-byte/void convention.
 *
 * The descriptor deliberately carries only the facts that are not specific to
 * one observed option: name and ABI. The option-6/result-bit semantics and the
 * output pointer/permissions/masks/table are observed call facts; they live in
 * the run's call list and docs/research.md#av-services-and-provenance, with the
 * unresolved Cxbx provenance. A static service fact cannot be confused with an
 * observation, and option 6 is not asserted as a service-wide property.
 */
const SERVICES = new Map([
  [221, {
    // Original two-word frame and pinned kernel header; ABI facts only.
    name: 'NtReleaseMutant', callingConvention: 'stdcall', argumentBytes: 8, returns: 'ntstatus',
  }],
  [234, {
    // Pinned kernel.h and the observed four pushed arguments; ABI facts only.
    name: 'NtWaitForSingleObjectEx', callingConvention: 'stdcall', argumentBytes: 16, returns: 'ntstatus',
  }],
  [126, {
    name: 'KeQueryPerformanceCounter',
    callingConvention: 'stdcall',
    argumentBytes: 0,
    returns: 'uint64',
  }],
  [2, {
    name: 'AvSendTVEncoderOption',
    callingConvention: 'stdcall',
    argumentBytes: 16,
    returns: 'void',
  }],
  [127, {
    name: 'KeQueryPerformanceFrequency',
    callingConvention: 'stdcall',
    argumentBytes: 0,
    returns: 'uint64',
  }],
  [189, {
    /* Pinned xboxrecomp-source kernel.h:838 identifies the service; the four
     * observed DWORD arguments and stdcall frame establish ABI only. No event
     * behavior or parameter meanings are modeled here. */
    name: 'NtCreateEvent',
    callingConvention: 'stdcall',
    argumentBytes: 16,
    returns: 'ntstatus',
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
