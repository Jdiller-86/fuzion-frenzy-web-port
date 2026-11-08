export const CONSOLE_OS_BYTES = 96;

const PROFILE_FIELDS = new Set([
  'kind', 'synthetic', 'layout', 'timeZone', 'zeroOtherFields', 'checksum',
]);
const DTO_FIELDS = new Set([...PROFILE_FIELDS, 'miscFlags', 'parentalControlGames']);
const U32_MAX = 0xffff_ffff;

function requireU32(value, name) {
  if (!Number.isInteger(value) || value < 0 || value > U32_MAX) {
    throw new TypeError(`${name} must be an explicit unsigned 32-bit integer`);
  }
}

function validateProfile(profile) {
  if (!profile || typeof profile !== 'object' || Array.isArray(profile)) {
    throw new TypeError('Console OS settings profile must be an object');
  }
  for (const key of Object.keys(profile)) {
    if (!PROFILE_FIELDS.has(key)) throw new TypeError(`Unsupported profile field: ${key}`);
  }
  if (profile.kind !== 'synthetic-console-os-settings') throw new TypeError('Unsupported console OS settings profile kind');
  if (profile.synthetic !== true) throw new TypeError('Console OS settings profile must be explicitly synthetic');
  if (profile.layout !== 'cxbx-user-settings-96-v1') throw new TypeError('Unsupported console OS settings layout');
  if (profile.timeZone !== 'utc-no-dst') throw new TypeError('Unsupported console OS settings timezone');
  if (profile.zeroOtherFields !== true) throw new TypeError('zeroOtherFields must be explicitly true');
  requireU32(profile.checksum, 'checksum');
}

function validateDto(settings) {
  if (!settings || typeof settings !== 'object' || Array.isArray(settings)) {
    throw new TypeError('Console OS settings DTO must be an object');
  }
  for (const key of Object.keys(settings)) {
    if (!DTO_FIELDS.has(key)) throw new TypeError(`Unsupported settings field: ${key}`);
  }
  for (const key of PROFILE_FIELDS) {
    if (!Object.hasOwn(settings, key)) throw new TypeError(`Missing profile field: ${key}`);
  }
  const profile = Object.fromEntries([...PROFILE_FIELDS].map(key => [key, settings[key]]));
  validateProfile(profile);
  requireU32(settings.miscFlags, 'miscFlags');
  requireU32(settings.parentalControlGames, 'parentalControlGames');
}

export function parseConsoleOsSettings(profile, { miscFlags, parentalControlGames } = {}) {
  if (profile === undefined) return undefined;
  validateProfile(profile);
  requireU32(miscFlags, 'miscFlags');
  requireU32(parentalControlGames, 'parentalControlGames');
  return { ...profile, miscFlags, parentalControlGames };
}

export function encodeConsoleOsSettings(settings) {
  validateDto(settings);
  const bytes = new Uint8Array(CONSOLE_OS_BYTES);
  const view = new DataView(bytes.buffer);
  view.setUint32(0, settings.checksum, true);
  view.setUint32(60, settings.parentalControlGames, true);
  view.setUint32(88, settings.miscFlags, true);
  return bytes;
}
