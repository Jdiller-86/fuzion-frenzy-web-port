/* Explicit AV-pack input for the reached ordinal-2 service.
 *
 * The pinned xboxrecomp-source reference defines the result layout:
 * pack type in bits 0-7, video standard in bits 8-15 and refresh in bits
 * 22-23 (AV_PACK_*, AV_STANDARD_*, AV_REFRESH_* in kernel.h:1124-1157).
 * The reference's HDTV/NTSC-M/60 Hz value is a reference default, not the
 * player's console, so this module has no default: the profile must declare
 * all three components explicitly, and any unknown or partial value fails
 * closed.
 */
export const AV_PACK = Object.freeze({
  none: 0x00, standard: 0x01, rfu: 0x02, scart: 0x03,
  hdtv: 0x04, vga: 0x05, svideo: 0x06,
});
export const AV_STANDARD = Object.freeze({ 'ntsc-m': 0x01, 'ntsc-j': 0x02, 'pal-i': 0x03 });
export const AV_REFRESH = Object.freeze({ 60: 0x00400000, 50: 0x00800000 });
/* The option-6 naming comes from the pinned xboxrecomp-source reference. A
 * provisional review cites a Cxbx-Reloaded pin 585c49a conflict, but that
 * source is not retained locally, so the conflict is unverified. Every modeled
 * run must state this provenance; the value is not a promoted default. */
export const AV_PACK_PROVENANCE = Object.freeze({
  primary: 'xboxrecomp-source',
  conflict: 'cxbx-585c49a',
  status: 'unverified',
});

export function composeAvPackWord(input) {
  if (!input || typeof input !== 'object' || Array.isArray(input))
    throw new Error('AV pack input must be an object');
  const { pack, standard, refresh } = input;
  if (typeof pack !== 'string' || !(pack in AV_PACK))
    throw new Error('AV pack input requires a known pack name');
  if (typeof standard !== 'string' || !(standard in AV_STANDARD))
    throw new Error('AV pack input requires a known video standard');
  if (!Number.isInteger(refresh) || !(refresh in AV_REFRESH))
    throw new Error('AV pack input requires a 50 or 60 Hz refresh');
  return (AV_PACK[pack] | (AV_STANDARD[standard] << 8) | AV_REFRESH[refresh]) >>> 0;
}
