#!/usr/bin/env node
/**
 * Audit callee-saved push/pop order in one prepared translation.
 *
 * Reads a private translation directory's title-functions.inc and reports only
 * function addresses, register order and counts. For each function it takes the
 * contiguous prologue push sequence (before the first non-save statement) and
 * the contiguous epilogue pop sequence (immediately before the return) and
 * checks that the pops are the exact reverse of the pushes. Call-argument
 * pushes are excluded by construction.
 *
 * This is a textual audit of the generated code, not a runtime claim; it
 * distinguishes "the emitted text is wrong" from "the executed mapping differs
 * from correct text".
 *
 * Usage: node tools/reg-save-order-audit.mjs <translation-directory> [--json]
 */
import fs from 'node:fs';
import path from 'node:path';

const dir = process.argv[2];
const json = process.argv.includes('--json');
if (!dir) {
  console.error('usage: node tools/reg-save-order-audit.mjs <translation-directory> [--json]');
  process.exit(2);
}
const file = path.join(dir, 'title-functions.inc');
let text;
try {
  text = fs.readFileSync(file, 'utf8');
} catch {
  console.error(`cannot read ${file}`);
  process.exit(2);
}

const lines = text.split(/\r?\n/);
const fnRe = /^void ff_title_([0-9A-F]+)\(void\)/;
const starts = [];
for (let i = 0; i < lines.length; i++) {
  const m = lines[i].match(fnRe);
  if (m) starts.push({ addr: m[1], line: i });
}

function isNoise(line) {
  const t = line.trim();
  return !t || t === '{' || t === '}' || t.startsWith('loc_') || t.startsWith('RECOMP_TRACE_') || t.startsWith('/*') || t.startsWith('*');
}
function pushReg(line) {
  const m = line.match(/FF_MEMORY_PUSH\(esp,\s*([a-z0-9]+)\)/);
  return m ? m[1] : null;
}
function popReg(line) {
  const m = line.match(/FF_MEMORY_POP\(esp,\s*([a-z0-9]+)\)/);
  return m ? m[1] : null;
}

const rows = [];
let checked = 0;
for (let i = 0; i < starts.length; i++) {
  const from = starts[i].line + 1;
  const to = i + 1 < starts.length ? starts[i + 1].line : lines.length;
  let k = from;
  while (k < to && (isNoise(lines[k]) || lines[k].includes('uint32_t') || lines[k].includes('int32_t') ||
                    lines[k].includes('int _flags') || lines[k].includes('(void)') || lines[k].includes('= g_'))) k++;
  const pushes = [];
  while (k < to && (isNoise(lines[k]) || pushReg(lines[k]))) {
    const r = pushReg(lines[k]);
    if (r) pushes.push(r);
    k++;
  }
  let e = to - 1;
  while (e > from && !lines[e].includes('FF_RETURN_FRAME')) e--;
  if (e <= from) continue;
  const pops = [];
  let p = e - 1;
  while (p > from && (isNoise(lines[p]) || popReg(lines[p]))) {
    const r = popReg(lines[p]);
    if (r) pops.unshift(r);
    p--;
  }
  if (!pushes.length && !pops.length) continue;
  checked++;
  const expect = [...pushes].reverse();
  const ok = expect.length === pops.length && expect.every((r, j) => r === pops[j]);
  if (!ok) rows.push({ addr: starts[i].addr, pushes: pushes.join(','), pops: pops.join(',') });
}

if (json) {
  console.log(JSON.stringify({ functions: starts.length, checked, mismatched: rows.length, rows }));
} else {
  console.log(`functions=${starts.length} checked=${checked} mismatched=${rows.length}`);
  for (const r of rows) console.log(`  0x${r.addr} push=[${r.pushes}] pop=[${r.pops}]`);
}
