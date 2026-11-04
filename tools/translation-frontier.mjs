#!/usr/bin/env node
/**
 * Report the translation frontier of a prepared title translation.
 *
 * Reads one `title-functions.inc` (a private, player-local artifact) and counts
 * the admitted functions, the boundary placeholders the translator emitted for
 * known-untranslated routines, how many of those placeholders admitted code
 * already calls, and the distinct call targets overall. With `--run` it also
 * intersects that static frontier with one retained run report, so a batch can
 * be scoped to callers that actually executed instead of every static target.
 * It prints counts and addresses only and never reproduces original code.
 *
 * Usage: node tools/translation-frontier.mjs <path-to-translation-directory> [--run <report.json>] [--json]
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
  const callers = new Map();
  for (const [caller, set] of calls) {
    for (const address of set) {
      targets.add(address);
      callers.set(address, (callers.get(address) ?? new Set()).add(caller));
    }
  }
  const referenced = [...stubs].filter(address => targets.has(address)).sort((a, b) => a - b);
  /* Which admitted functions call each placeholder, so the next batch can be
   * selected by cluster instead of address order. */
  const referencedBy = Object.fromEntries(referenced.map(address => [
    address,
    [...(callers.get(address) ?? [])].sort((a, b) => a - b),
  ]));
  return {
    functions: functions.size,
    placeholders: stubs.size,
    referencedPlaceholders: referenced.length,
    callTargets: targets.size,
    referenced,
    referencedBy,
  };
}

/* A retained run report states which original functions executed and, when the
 * trace is present, the enter/exit sequence. Validate the shapes before using
 * them. The trace is not treated as a call stack: the retained reports contain
 * unmatched exits from tail calls and returns, so only the entered addresses are
 * used and the executed-function set is the authoritative filter. */
export function parseRunEvidence(report) {
  if (!report || typeof report !== 'object' || Array.isArray(report))
    throw new Error('Run evidence must be a report object');
  const addressOf = value => {
    if (!Number.isInteger(value) || value < 0 || value > 0xffffffff)
      throw new Error('Run evidence contains an invalid function address');
    return value >>> 0;
  };
  const reached = report.originalFunctionsReached;
  if (!Array.isArray(reached) && !Array.isArray(report.trace))
    throw new Error('Run evidence needs originalFunctionsReached or trace');
  const executed = new Set();
  const entered = new Set();
  if (reached !== undefined) {
    if (!Array.isArray(reached)) throw new Error('originalFunctionsReached must be an array');
    for (const address of reached) executed.add(addressOf(address));
  }
  /* A status-2 stop is the untranslated routine itself: it was entered, but it
   * is not an executed original function and does not appear in the reached set. */
  if (report.stop !== undefined) {
    if (!report.stop || typeof report.stop !== 'object' || Array.isArray(report.stop))
      throw new Error('stop must be an object');
    if (report.stop.status === 2) entered.add(addressOf(report.stop.target));
  }
  let truncated = false;
  if (report.trace !== undefined) {
    if (!Array.isArray(report.trace)) throw new Error('trace must be an array');
    for (const event of report.trace) {
      if (!event || typeof event !== 'object' || Array.isArray(event))
        throw new Error('trace events must be objects');
      if (event.kind !== 'enter' && event.kind !== 'exit')
        throw new Error('trace event kind must be enter or exit');
      const address = addressOf(event.address);
      if (event.kind === 'enter') executed.add(address);
    }
    truncated = Number.isInteger(report.traceCapacity) &&
      report.traceCapacity >= 0 && report.trace.length >= report.traceCapacity;
  }
  return { executed, entered, truncated };
}

/* Partition the static frontier by the retained run:
 * - entered: the placeholder itself executed (typically the boundary stop);
 * - called-by-executed-caller: a caller executed, but this call site is not
 *   proven taken (a static upper bound, not a dynamic edge);
 * - static-only: no caller executed.
 * Only the first group is proven to affect the current run. */
export function analyzeReachability(frontier, report) {
  const { executed, entered: enteredByStop, truncated } = parseRunEvidence(report);
  const entered = [], calledByExecutedCaller = [], staticOnly = [];
  for (const placeholder of frontier.referenced) {
    if (enteredByStop.has(placeholder) || executed.has(placeholder)) {
      entered.push({ placeholder, callers: frontier.referencedBy[placeholder] ?? [] });
      continue;
    }
    const callers = (frontier.referencedBy[placeholder] ?? [])
      .filter(caller => executed.has(caller)).sort((a, b) => a - b);
    if (callers.length) calledByExecutedCaller.push({ placeholder, callers });
    else staticOnly.push(placeholder);
  }
  return {
    executedFunctions: executed.size,
    truncated,
    entered,
    calledByExecutedCaller,
    staticOnly,
  };
}

const hex = (address) => `0x${address.toString(16).toUpperCase().padStart(8, '0')}`;

function main(argv) {
  let directory, runPath, json = false;
  for (let i = 0; i < argv.length; ++i) {
    const argument = argv[i];
    if (argument === '--json') json = true;
    else if (argument === '--run') {
      runPath = argv[++i];
      if (!runPath) {
        console.error('--run needs a report path');
        return 2;
      }
    } else if (!argument.startsWith('--') && !directory) directory = argument;
    else {
      console.error(`Unknown argument: ${argument}`);
      return 2;
    }
  }
  if (!directory) {
    console.error('Usage: node tools/translation-frontier.mjs <translation-directory> [--run <report.json>] [--json]');
    return 2;
  }
  const file = path.join(directory, 'title-functions.inc');
  if (!fs.existsSync(file)) {
    console.error(`No translation found at ${file}. The translation is a private, player-local artifact.`);
    return 2;
  }
  const report = analyzeTranslation(fs.readFileSync(file, 'utf8'));
  let reachability;
  if (runPath) {
    if (!fs.existsSync(runPath)) {
      console.error(`No run report found at ${runPath}. The report is a private, player-local artifact.`);
      return 2;
    }
    try {
      reachability = analyzeReachability(report, JSON.parse(fs.readFileSync(runPath, 'utf8')));
    } catch (error) {
      console.error(`Invalid run evidence: ${error.message}`);
      return 2;
    }
  }
  if (json) {
    console.log(JSON.stringify(reachability ? { ...report, reachability } : report, null, 2));
    return 0;
  }
  console.log(`functions: ${report.functions}`);
  console.log(`boundary placeholders: ${report.placeholders}`);
  console.log(`placeholders called by admitted code: ${report.referencedPlaceholders}`);
  console.log(`distinct call targets: ${report.callTargets}`);
  for (const address of report.referenced) {
    const callersOfPlaceholder = report.referencedBy[address] ?? [];
    console.log(`  ${hex(address)} called by ${callersOfPlaceholder.map(hex).join(', ') || 'none'}`);
  }
  if (reachability) {
    console.log(`executed functions: ${reachability.executedFunctions}`);
    console.log(`placeholders entered: ${reachability.entered.length}`);
    console.log(`placeholders called by an executed caller: ${reachability.calledByExecutedCaller.length}`);
    console.log(`placeholders with only unexecuted callers: ${reachability.staticOnly.length}`);
    if (reachability.truncated)
      console.log('warning: the trace reached its declared capacity; the executed set may be incomplete');
    for (const item of reachability.entered)
      console.log(`  ENTERED ${hex(item.placeholder)} called by ${item.callers.map(hex).join(', ') || 'none'}`);
    for (const item of reachability.calledByExecutedCaller)
      console.log(`  EXECUTED-CALLER ${hex(item.placeholder)} statically called by ${item.callers.map(hex).join(', ')}`);
  }
  return 0;
}

if (process.argv[1] && import.meta.url === new URL(`file://${path.resolve(process.argv[1])}`).href) {
  process.exitCode = main(process.argv.slice(2));
}
