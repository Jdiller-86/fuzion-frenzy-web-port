// Developer-only differential check of compiled source-owned geometry against
// synthetic pinned-emulator shader results. Never pass game files to this tool.
import fs from 'node:fs/promises';
import path from 'node:path';
import { createRequire } from 'node:module';
import { fileURLToPath } from 'node:url';
import { createHash } from 'node:crypto';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const [moduleArg, referenceArg, reportArg, arithmeticArg, ...extra] = process.argv.slice(2);
if (!moduleArg || !referenceArg || !reportArg || !['fused', 'separate'].includes(arithmeticArg) || extra.length) {
  throw new Error('Usage: node tools/compare-nv2a-geometry.mjs <compiled.cjs> <reference.json> <new-research-report.json> <fused|separate>');
}
const arithmetic = arithmeticArg === 'fused' ? 0 : 1;
const inside = (base, target) => {
  const relative = path.relative(base, target);
  return relative !== '' && !relative.startsWith(`..${path.sep}`) && relative !== '..' && !path.isAbsolute(relative);
};
const toolRoot = await fs.realpath(path.join(root, '.tools/emulation'));
const research = await fs.realpath(path.join(root, 'research'));
const modulePath = await fs.realpath(path.resolve(moduleArg));
const referencePath = await fs.realpath(path.resolve(referenceArg));
const reportPath = path.resolve(reportArg);
const reportDirectory = await fs.realpath(path.dirname(reportPath));
if (!inside(toolRoot, modulePath) || path.extname(modulePath) !== '.cjs' ||
    !inside(toolRoot, referencePath) || path.extname(referencePath) !== '.json' ||
    !(reportDirectory === research || inside(research, reportDirectory)) ||
    path.extname(reportPath) !== '.json') throw new Error('Use private tools inputs and a new ignored research JSON output');
// Reserve before loading executable tool code or doing any substantial work.
const reportFile = await fs.open(reportPath, 'wx');
const sha = bytes => createHash('sha256').update(bytes).digest('hex');
try {
  const stat = await fs.stat(referencePath);
  if (stat.size > 16 * 1024 * 1024) throw new Error('Reference exceeds the owned fixture limit');
  const referenceBytes = await fs.readFile(referencePath);
  const reference = JSON.parse(referenceBytes);
  const manifest = JSON.parse(await fs.readFile(path.join(root, 'tools/original-execution-toolchain.json'), 'utf8'));
  if (reference.kind !== 'pinned-xemu-triangle-geometry-transform-feedback' ||
      reference.xemuCommit !== manifest.sources.xemu.commit || reference.inputStride !== 42 ||
      reference.outputStride !== 51 || !Array.isArray(reference.records) ||
      !reference.records.length || reference.records.length > 4096) throw new Error('Unsupported reference schema/pin');
  const wasmPath = await fs.realpath(modulePath.slice(0, -4) + '.wasm');
  if (!inside(toolRoot, wasmPath)) throw new Error('Wasm must remain in the private tool prefix');
  const wasmBytes = await fs.readFile(wasmPath);
  const factory = createRequire(import.meta.url)(modulePath);
  const module = await factory({ wasmBinary: wasmBytes });
  const input = module._malloc(126 * 4), state = module._malloc(4 * 4), output = module._malloc(153 * 4);
  if (!input || !state || !output) throw new Error('Wasm fixture allocation failed');
  let forwardedMismatches = 0, slopeMismatches = 0, maxSlopeUlps = 0;
  const examples = [];
  const view = new DataView(new ArrayBuffer(4));
  const number = bits => { view.setUint32(0, bits, true); return view.getFloat32(0, true); };
  const ordered = bits => bits & 0x80000000 ? 0x80000000 - (bits & 0x7fffffff) : 0x80000000 + bits;
  try {
    for (const record of reference.records) {
      const words = array => Array.isArray(array) && array.every(n => Number.isInteger(n) && n >= 0 && n <= 0xffffffff);
      if (!words(record.input) || record.input.length !== 126 || !words(record.expected) ||
          record.expected.length !== 153 || !words(record.state) || record.state.length !== 4 ||
          record.state.slice(0, 3).some(n => n > 1) || record.state[3] > 2) throw new Error('Malformed owned reference record');
      module.HEAPU32.set(record.input, input >>> 2);
      module.HEAPU32.set(record.state, state >>> 2);
      if (module._ff_nv2a_expand_triangle(input, state, output, arithmetic) !== 0) throw new Error('Prepass rejected valid reference state');
      const actual = module.HEAPU32.subarray(output >>> 2, (output >>> 2) + 153);
      for (let i = 0; i < 153; ++i) {
        if (actual[i] === record.expected[i]) continue;
        const slope = i % 51 === 50;
        if (slope) {
          slopeMismatches++;
          const a = number(actual[i]), b = number(record.expected[i]);
          const ulps = Number.isFinite(a) && Number.isFinite(b) ? Math.abs(ordered(actual[i]) - ordered(record.expected[i])) : Infinity;
          maxSlopeUlps = Math.max(maxSlopeUlps, ulps);
        } else forwardedMismatches++;
        if (examples.length < 12) examples.push({ case: record.case, state: record.state, component: i,
          expectedBits: record.expected[i], actualBits: actual[i], expected: number(record.expected[i]), actual: number(actual[i]) });
      }
    }
  } finally {
    module._free(input); module._free(state); module._free(output);
  }
  const report = {
    kind: 'wasm-triangle-versus-pinned-shader', xemuCommit: reference.xemuCommit, arithmetic: arithmeticArg,
    referenceSha256: sha(referenceBytes), moduleSha256: sha(await fs.readFile(modulePath)), wasmSha256: sha(wasmBytes),
    records: reference.records.length, comparedScalars: reference.records.length * 153,
    forwardedMismatches, slopeMismatches, maxSlopeUlps, examples,
    status: forwardedMismatches || slopeMismatches ? 'differences-found' : 'exact-match',
    limitation: 'Synthetic filled-triangle component comparison only; no game execution or original-hardware fidelity proof.'
  };
  await reportFile.writeFile(JSON.stringify(report, null, 2) + '\n');
  console.log(JSON.stringify({ status: report.status, records: report.records, forwardedMismatches, slopeMismatches, maxSlopeUlps }));
  if (report.status !== 'exact-match') process.exitCode = 1;
} finally {
  await reportFile.close();
}
