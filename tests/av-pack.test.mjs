import assert from 'node:assert/strict';
import test from 'node:test';
import { composeAvPackWord } from '../src/runtime/av-pack.mjs';

test('composes the pinned AV pack word from explicit components', () => {
  assert.equal(composeAvPackWord({ pack: 'hdtv', standard: 'ntsc-m', refresh: 60 }), 0x00400104);
  assert.equal(composeAvPackWord({ pack: 'standard', standard: 'pal-i', refresh: 50 }), 0x00800301);
});

test('rejects missing, unknown or out-of-range AV pack components', () => {
  const bad = [
    undefined,
    null,
    {},
    { pack: 'hdtv' },
    { pack: 'hdmi', standard: 'ntsc-m', refresh: 60 },
    { pack: 'hdtv', standard: 'pal', refresh: 60 },
    { pack: 'hdtv', standard: 'ntsc-m', refresh: 59 },
    { pack: 4, standard: 'ntsc-m', refresh: 60 },
  ];
  for (const input of bad) assert.throws(() => composeAvPackWord(input));
});
