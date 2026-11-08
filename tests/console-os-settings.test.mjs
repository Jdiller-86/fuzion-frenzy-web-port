import test from 'node:test';
import assert from 'node:assert/strict';
import { CONSOLE_OS_BYTES, encodeConsoleOsSettings, parseConsoleOsSettings } from '../runtime/browser/console-os-settings.mjs';

const profile = () => ({
  kind: 'synthetic-console-os-settings', synthetic: true,
  layout: 'cxbx-user-settings-96-v1', timeZone: 'utc-no-dst',
  zeroOtherFields: true, checksum: 0x12345678,
});
const context = { miscFlags: 0x89abcdef, parentalControlGames: 0x10203040 };

test('requires explicit opt-in and keeps profile fields with derived values', () => {
  assert.equal(parseConsoleOsSettings(undefined, context), undefined);
  assert.deepEqual(parseConsoleOsSettings(profile(), context), { ...profile(), ...context });
});

test('rejects wrong kind, nonsynthetic and unsupported profile declarations', () => {
  for (const change of [
    { kind: 'console-eeprom' }, { synthetic: false }, { layout: 'unknown' },
    { timeZone: 'local' }, { zeroOtherFields: false }, { zeroOtherFields: undefined },
    { checksum: -1 }, { inventedHardware: 'x' },
  ]) assert.throws(() => parseConsoleOsSettings({ ...profile(), ...change }, context));
});

test('requires explicit unsigned DWORD context values', () => {
  for (const value of [undefined, -1, 0x1_0000_0000, 1.5, '1']) {
    assert.throws(() => parseConsoleOsSettings(profile(), { ...context, miscFlags: value }));
    assert.throws(() => parseConsoleOsSettings(profile(), { ...context, parentalControlGames: value }));
  }
});

test('encodes only the declared DWORD offsets and leaves all other bytes zero', () => {
  const bytes = encodeConsoleOsSettings(parseConsoleOsSettings(profile(), context));
  assert.equal(bytes.byteLength, CONSOLE_OS_BYTES);
  const view = new DataView(bytes.buffer);
  assert.equal(view.getUint32(0, true), profile().checksum);
  assert.equal(view.getUint32(60, true), context.parentalControlGames);
  assert.equal(view.getUint32(88, true), context.miscFlags);
  for (let i = 0; i < bytes.length; i++) {
    if ((i < 4) || (i >= 60 && i < 64) || (i >= 88 && i < 92)) continue;
    assert.equal(bytes[i], 0, `byte ${i} should be zero`);
  }
});

test('encoding revalidates DTOs and rejects mutation, missing derived fields, and contradictory raw fields', () => {
  const dto = parseConsoleOsSettings(profile(), context);
  dto.miscFlags = -1;
  assert.throws(() => encodeConsoleOsSettings(dto));
  const missing = parseConsoleOsSettings(profile(), context);
  delete missing.parentalControlGames;
  assert.throws(() => encodeConsoleOsSettings(missing));
  assert.throws(() => parseConsoleOsSettings({ ...profile(), miscFlags: 1 }, context));
  assert.throws(() => parseConsoleOsSettings({ ...profile(), parentalControlGames: 1 }, context));
});
