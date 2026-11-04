#!/usr/bin/env node
/**
 * Report the translation frontier of a prepared title translation.
 *
 * Reads one `title-functions.inc` (a private, player-local artifact) and counts
 * the admitted functions, the boundary placeholders the translator emitted for
 * known-untranslated routines, how many of those placeholders admitted code
 * already calls, and the distinct call targets overall. This turns a one-off
 * analysis into a repeatable measurement; it prints counts and addresses only and
 * never reproduces original code.
 *
 * Usage: node tools/translation-frontier.mjs <path-to-translation-directory> [--json]
 */
import fs from 'node:fs';
import path from 'node:path';

const FUNCTION = /^void ff_title_([0-9A-F]{8})\(void\)/;
const STUB = /^void ff_title_([0-9A-F]{8})\(void\) \{ ff_probe_stop_direct\(0x[0-9A-F]{8}u\); \}/;
const CALL = /RECOMP_ABI_CALL\(0x([0-9A-F]{8})u,/g;

export function analyzeTranslation(text) {
  const functions = new Set();
  const stubs = new Set();
  const calls = new Map();
  let current = null;
  for (const line of text.split('\n')) {
    const stub = STUB.exec(line);
    if (stub) {
      const address = parseInt(stub[1], 16);
      functions.add(address);
      stubs.add(address);
      current = address;
      continue;
    }
    const header = FUNCTION.exec(line);
    if (header) {
      current = parseInt(header[1], 16);
      functions.add(current);
      continue;
    }
    if (current === null) continue;
    for (const match of line.matchAll(CALL)) {
      if (!calls.has(current)) calls.set(current, new Set());
      calls.get(current).add(parseInt(match[1], 16));
    }
  }
  const targets = new Set();
  for (const set of calls.values()) for (const address of set) targets.add(address);
  const referenced = [...stubs].filter(address => targets.has(address)).sort((a, b) => a - b);
  return {
    functions: functions.size,
    placeholders: stubs.size,
    referencedPlaceholders: referenced.length,
    callTargets: targets.size,
    referenced,
  };
}

const hex = (address) => `0x${address.toString(16).toUpperCase().padStart(8, '0')}`;

function main(argv) {
  const directory = argv.find(argument => !argument.startsWith('--'));
  const json = argv.includes('--json');
  if (!directory) {
    console.error('Usage: node tools/translation-frontier.mjs <translation-directory> [--json]');
    return 2;
  }
  const file = path.join(directory, 'title-functions.inc');
  if (!fs.existsSync(file)) {
    console.error(`No translation found at ${file}. The translation is a private, player-local artifact.`);
    return 2;
  }
  const report = analyzeTranslation(fs.readFileSync(file, 'utf8'));
  if (json) {
    console.log(JSON.stringify(report, null, 2));
    return 0;
  }
  console.log(`functions: ${report.functions}`);
  console.log(`boundary placeholders: ${report.placeholders}`);
  console.log(`placeholders called by admitted code: ${report.referencedPlaceholders}`);
  console.log(`distinct call targets: ${report.callTargets}`);
  for (const address of report.referenced) console.log(`  ${hex(address)}`);
  return 0;
}

if (process.argv[1] && import.meta.url === new URL(`file://${path.resolve(process.argv[1])}`).href) {
  process.exitCode = main(process.argv.slice(2));
}
