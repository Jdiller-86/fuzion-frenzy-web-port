#!/usr/bin/env node
/**
 * Compare two thread-startup proof reports that must describe the same module.
 *
 * The synthetic avQuery experiment changes exactly one declared input: the
 * explicit raw DWORD bound to ordinal 2. Everything else (executable and module
 * digests, the other experiments, the observed call frame) must match. This tool
 * enforces that pairing, reports both stops and the progress delta, and refuses
 * to treat a different module or an extra input as a controlled comparison.
 *
 * It reads only reports produced by the local proof worker. It does not read
 * original content, admit a translation boundary or reinterpret a value.
 *
 * Usage: node tools/compare-proof-profiles.mjs <control.json> <experiment.json> [--json]
 */
import fs from 'node:fs';
import path from 'node:path';
import { validateProofReport } from '../src/runtime/proof-report.mjs';
import { validateSuppliedDeviceProfile } from '../src/runtime/device-report.mjs';

const SHA256 = /^[0-9a-f]{64}$/;
const DWORD = value => Number.isInteger(value) && value >= 0 && value <= 0xffffffff;
const hex = value => `0x${value.toString(16).toUpperCase().padStart(8, '0')}`;

function digest(report, field) {
  const value = report[field];
  if (typeof value !== 'string' || !SHA256.test(value))
    throw new Error(`The ${field} digest is missing or malformed`);
  return value;
}

function experimentSet(report) {
  const names = report.experiments ?? [];
  if (!Array.isArray(names)) throw new Error('The experiments field must be a list');
  const set = new Set(names);
  /* A repeated name would collapse silently and could hide a second declared
   * input, so reject it instead of comparing the reduced set. */
  if (set.size !== names.length)
    throw new Error('The experiments field must not repeat an experiment');
  return set;
}

/* The supplied device input profile is the window, initial registers and
 * zero-ranges the run was configured with. Two runs with different supplied
 * inputs are not a controlled comparison even if their experiment names match,
 * so require the full profile and compare it deeply. Both reports must carry it:
 * an absent profile is not evidence that the inputs matched. */
function suppliedProfile(report) {
  const device = report.deviceRegisters;
  if (!device || typeof device !== 'object')
    throw new Error('Each profile must carry its supplied device input profile');
  const supplied = device.supplied;
  if (!supplied || typeof supplied !== 'object' || Array.isArray(supplied))
    throw new Error('Each profile must carry its supplied device input profile');
  return validateSuppliedDeviceProfile(supplied);
}

/* Compare the whole supplied profile, including fields this tool does not know
 * about, so a new or misspelled input cannot slip through as controlled. */
function canonical(value) {
  if (Array.isArray(value)) return value.map(canonical);
  if (value && typeof value === 'object')
    return Object.fromEntries(Object.keys(value).sort().map(key => [key, canonical(value[key])]));
  return value;
}

const sameCanonical = (left, right) => JSON.stringify(canonical(left)) === JSON.stringify(canonical(right));

/* Everything the run declared before it started, except the avQuery word under
 * test and the device aperture already checked above. Two runs that differ in
 * any of these are not a controlled comparison. Run state (local disk/volume,
 * FATX, clock window) is deliberately excluded. */
function declaredInputs(report) {
  const inputs = {
    consoleSettings: report.consoleSettings,
    volumeProfile: report.volumeProfile,
    directoryProfile: report.directoryProfile,
    fileStorageProfile: report.fileStorageProfile,
    diskProfile: report.diskProfile,
    pfifoProfile: report.pfifoProfile,
    memoryProfile: report.memoryAccounting?.profile,
  };
  /* Presence is part of the evidence: two reports that both omit a profile did
   * not demonstrate that the input matched. */
  for (const [name, value] of Object.entries(inputs)) {
    if (value === undefined || value === null)
      throw new Error(`Each profile must carry its declared ${name}`);
  }
  return inputs;
}

function ordinalTwoCalls(report) {
  if (!Array.isArray(report.calls))
    throw new Error('Each profile must carry its kernel call list');
  return report.calls.filter(call => call.ordinal === 2);
}

/* Validate both reports, require the same executable/module, require the only
 * declared input difference to be avQuery, and require the control to stop on
 * the unsupported ordinal-2 call while the experiment progresses past it. */
export function compareProofProfiles(control, experiment) {
  validateProofReport(control);
  validateProofReport(experiment);
  if (control.kind !== 'thread-startup-boundary' || experiment.kind !== 'thread-startup-boundary')
    throw new Error('Both profiles must be thread-startup-boundary reports');

  const sourceSha256 = digest(control, 'sourceSha256');
  if (digest(experiment, 'sourceSha256') !== sourceSha256)
    throw new Error('The profiles were prepared for different executables');
  const moduleSha256 = digest(control, 'moduleSha256');
  if (digest(experiment, 'moduleSha256') !== moduleSha256)
    throw new Error('The profiles ran different modules, so the comparison is not controlled');

  const controlExperiments = experimentSet(control);
  const experimentExperiments = experimentSet(experiment);
  const controlSupplied = suppliedProfile(control);
  const experimentSupplied = suppliedProfile(experiment);
  if (!sameCanonical(controlSupplied, experimentSupplied))
    throw new Error('The profiles supplied different device inputs, so the comparison is not controlled');
  if (!sameCanonical(declaredInputs(control), declaredInputs(experiment)))
    throw new Error('The profiles declared different console/volume/directory/file-storage/disk/PFIFO/memory inputs, so the comparison is not controlled');
  if (controlExperiments.has('avQuery'))
    throw new Error('The control profile must not declare the avQuery experiment');
  if (!experimentExperiments.has('avQuery'))
    throw new Error('The experiment profile must declare the avQuery experiment');
  if (experimentExperiments.size !== controlExperiments.size + 1 ||
      [...controlExperiments].some(name => !experimentExperiments.has(name)))
    throw new Error('The profiles may differ only by the avQuery experiment');
  if (control.avQueryWord !== undefined)
    throw new Error('The control profile must not carry a synthetic AV query word');
  if (!DWORD(experiment.avQueryWord))
    throw new Error('The experiment profile must carry one explicit 32-bit AV query word');

  const controlCalls = ordinalTwoCalls(control);
  const experimentCalls = ordinalTwoCalls(experiment);
  if (!controlCalls.length)
    throw new Error('The control profile must record the attempted ordinal-2 call');
  if (!experimentCalls.length)
    throw new Error('The experiment profile must record the completed ordinal-2 call');
  /* A controlled comparison must observe the same guest call. Differing
   * arguments would mean a different query reached the binding, not just a
   * different supplied input. */
  if (JSON.stringify(controlCalls.map(call => call.arguments)) !==
      JSON.stringify(experimentCalls.map(call => call.arguments)))
    throw new Error('The profiles recorded different ordinal-2 call arguments');
  /* validateProofReport already enforces the per-call status/output pairing for
   * both branches; these checks name the comparison invariant explicitly. */
  if (controlCalls.some(call => call.status === 0 || call.output !== undefined))
    throw new Error('The control ordinal-2 call must stay unsupported');
  if (!experimentCalls.some(call => call.status === 0 &&
      call.output?.rawWord === experiment.avQueryWord))
    throw new Error('The experiment must complete ordinal 2 with its declared raw word');

  if (control.stop.ordinal !== 2 || control.stop.status !== 1)
    throw new Error('The control must stop on the unsupported ordinal-2 call');
  if (experiment.stop.ordinal === 2)
    throw new Error('The experiment must progress past the ordinal-2 call');

  const before = control.originalStartup;
  const after = experiment.originalStartup;
  if (after.distinctOriginalFunctions < before.distinctOriginalFunctions ||
      after.completedKernelOperations < before.completedKernelOperations)
    throw new Error('The experiment must not report fewer functions or kernel operations');

  return {
    ok: true,
    sourceSha256,
    moduleSha256,
    addedExperiment: 'avQuery',
    avQueryWord: experiment.avQueryWord,
    deviceInputs: controlSupplied,
    control: {
      stop: control.stop,
      routines: before.distinctOriginalFunctions,
      kernelOperations: before.completedKernelOperations,
      ordinal2: controlCalls,
    },
    experiment: {
      stop: experiment.stop,
      routines: after.distinctOriginalFunctions,
      kernelOperations: after.completedKernelOperations,
      ordinal2: experimentCalls,
    },
    delta: {
      routines: after.distinctOriginalFunctions - before.distinctOriginalFunctions,
      kernelOperations: after.completedKernelOperations - before.completedKernelOperations,
    },
    nextStop: experiment.stop,
  };
}

function main(argv) {
  const paths = [];
  let json = false;
  for (const argument of argv) {
    if (argument === '--json') json = true;
    else if (!argument.startsWith('--') && paths.length < 2) paths.push(argument);
    else {
      console.error(`Unknown argument: ${argument}`);
      return 2;
    }
  }
  if (paths.length !== 2) {
    console.error('Usage: node tools/compare-proof-profiles.mjs <control.json> <experiment.json> [--json]');
    return 2;
  }
  let control, experiment;
  try {
    control = JSON.parse(fs.readFileSync(paths[0], 'utf8'));
    experiment = JSON.parse(fs.readFileSync(paths[1], 'utf8'));
  } catch (error) {
    console.error(`Cannot read a proof report: ${error.message}`);
    return 2;
  }
  let result;
  try {
    result = compareProofProfiles(control, experiment);
  } catch (error) {
    console.error(`Comparison rejected: ${error.message}`);
    return 1;
  }
  if (json) {
    console.log(JSON.stringify(result, null, 2));
    return 0;
  }
  console.log(`module: ${result.moduleSha256}`);
  console.log(`source: ${result.sourceSha256}`);
  console.log(`added input: ${result.addedExperiment} = ${hex(result.avQueryWord)}`);
  console.log(`control: status ${result.control.stop.status} at ${hex(result.control.stop.target)}, ` +
    `${result.control.routines} routines / ${result.control.kernelOperations} kernel operations`);
  console.log(`experiment: status ${result.experiment.stop.status} at ${hex(result.experiment.stop.target)}, ` +
    `${result.experiment.routines} routines / ${result.experiment.kernelOperations} kernel operations`);
  console.log(`delta: +${result.delta.routines} routines, +${result.delta.kernelOperations} kernel operations`);
  console.log(`next stop: ${hex(result.nextStop.target)} (status ${result.nextStop.status})`);
  return 0;
}

if (process.argv[1] && import.meta.url === new URL(`file://${path.resolve(process.argv[1])}`).href) {
  process.exitCode = main(process.argv.slice(2));
}
