/* Source-owned names and contract facts for reached Xbox kernel services.
 *
 * This is metadata only: it never implements a service or supplies a return
 * value. The retained callers evidence and the pinned xboxrecomp-source kernel
 * reference ground the ordinal-2 contract:
 *
 * - `kernel.h:1100` declares
 *   `VOID __stdcall AvSendTVEncoderOption(PVOID RegisterBase, ULONG Option,
 *   ULONG Param, PULONG Result)`; the call site pushes four arguments and the
 *   following instruction is not `add esp, imm`, so the stdcall/16-byte
 *   callee-cleanup convention is consistent.
 * - `kernel.h:1147-1157` defines option 6 as `AV_OPTION_QUERY_AVPACK`.
 * - `kernel.h:1124-1145` defines the result fields: pack type in `0xFF`,
 *   video standard in `0xFF00`, refresh in `0xC00000`.
 * - `kernel_hal.c:1463-1479` returns a pack/standard/refresh word; its
 *   HDTV/NTSC-M/60 Hz value is a reference default, not the player's console,
 *   so no value is adopted here.
 *
 * The caller at 0x203720 masks 0xFF and 0xFF00 and scans a 0xBF-entry table at
 * 0x202C38 with stride 0xC. The service remains unmodeled.
 */
const SERVICES = new Map([
  [2, {
    name: 'AvSendTVEncoderOption',
    callingConvention: 'stdcall',
    argumentBytes: 16,
    returns: 'void',
    option: { code: 6, name: 'AV_OPTION_QUERY_AVPACK' },
    resultBits: { pack: 0xff, standard: 0xff00, refresh: 0xc00000 },
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
    option: { ...known.option },
    resultBits: { ...known.resultBits },
    output: { ...known.output, masks: [...known.output.masks], table: { ...known.output.table } },
  };
}
