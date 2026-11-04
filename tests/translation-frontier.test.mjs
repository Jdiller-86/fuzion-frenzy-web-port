import assert from 'node:assert/strict';
import test from 'node:test';
import { analyzeTranslation } from '../tools/translation-frontier.mjs';

const translation = [
  '/**',
  ' * ff_title_00100000',
  ' */',
  'void ff_title_00100000(void)',
  '{',
  'loc_00100000: ;',
  '    FF_MEMORY_PUSH(esp, 0x00100010u); RECOMP_ABI_CALL(0x00100100u, ff_title_00100100); /* call */',
  '    FF_MEMORY_PUSH(esp, 0x0010001Cu); RECOMP_ABI_CALL(0x00100200u, ff_title_00100200); /* call */',
  '}',
  '',
  'void ff_title_00100100(void)',
  '{',
  'loc_00100100: ;',
  '    eax = 0;',
  '}',
  '',
  'void ff_title_00100200(void) { ff_probe_stop_direct(0x00100200u); }',
  'void ff_title_00100300(void) { ff_probe_stop_direct(0x00100300u); }',
  '',
].join('\n');

test('the frontier report counts functions, placeholders and called placeholders', () => {
  const report = analyzeTranslation(translation);
  assert.equal(report.functions, 4);
  assert.equal(report.placeholders, 2);
  assert.equal(report.referencedPlaceholders, 1);
  assert.equal(report.callTargets, 2);
  assert.deepEqual(report.referenced, [0x00100200]);
});

test('the frontier report attributes placeholders to their callers', () => {
  const report = analyzeTranslation(translation);
  assert.deepEqual(report.referencedBy[0x00100200], [0x00100000]);
  assert.equal(report.referencedBy[0x00100300], undefined);
});

test('a translation without placesholders reports zero placeholders', () => {
  const report = analyzeTranslation('void ff_title_00000010(void)\n{\nloc_00000010: ;\n}\n');
  assert.equal(report.functions, 1);
  assert.equal(report.placeholders, 0);
  assert.equal(report.referencedPlaceholders, 0);
  assert.equal(report.callTargets, 0);
  assert.deepEqual(report.referenced, []);
});

test('calls are attributed to the enclosing function only', () => {
  const text = [
    'void ff_title_00000010(void)',
    '{',
    '  RECOMP_ABI_CALL(0x00000020u, ff_title_00000020);',
    '}',
    'void ff_title_00000020(void)',
    '{',
    '  /* no calls */',
    '}',
    '',
  ].join('\n');
  const report = analyzeTranslation(text);
  assert.equal(report.callTargets, 1);
  assert.equal(report.referencedPlaceholders, 0);
});
