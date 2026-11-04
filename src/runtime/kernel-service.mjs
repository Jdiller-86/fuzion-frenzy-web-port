/* Source-owned names for reached Xbox kernel services.
 *
 * This is metadata only: it never implements a service or supplies a return
 * value. The retained callers evidence names ordinal 2 as
 * AvSendTVEncoderOption; the harness has no encoder model, so the report must
 * say which service boundary was reached and that it is not modeled. Adding a
 * name here is not an implementation claim.
 */
const SERVICES = new Map([
  [2, 'AvSendTVEncoderOption'],
]);

export function describeKernelService(ordinal) {
  if (!Number.isInteger(ordinal) || ordinal < 0 || ordinal > 0xffff)
    return { ordinal, name: null, modeled: false };
  return { ordinal, name: SERVICES.get(ordinal) ?? null, modeled: false };
}
