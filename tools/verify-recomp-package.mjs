#!/usr/bin/env node
/**
 * Verify one prepared recompilation package in a single command.
 *
 * The package is the existing, tested set of private research artifacts:
 *   - a prepared translation directory with generation.json/title-functions.inc;
 *   - the exact seed files recorded by that generation.json;
 *   - the control/experiment proof reports.
 *
 * The tool re-runs the owned checks rather than reinterpreting them:
 *   - verify-seeds.py: every recorded seed digest is present;
 *   - verify-translation.py: the translation is fully guarded;
 *   - compare-proof-profiles: the controlled pair is valid and progresses.
 *
 * It reads only local files, never original content into a result, and never
 * executes original code. A missing artifact or a failed check is an error.
 *
 * Usage:
 *   node tools/verify-recomp-package.mjs \
 *     --translation <dir> --control <control.json> --experiment <experiment.json> \
 *     [--seed-dir <dir>]... [--python <interpreter>] [--json]
 */
import crypto from 'node:crypto';
import fs from 'node:fs';
import path from 'node:path';
import { spawnSync } from 'node:child_process';
import { compareProofProfiles } from './compare-proof-profiles.mjs';

const SEED_MIN = 100;
const SEED_MAX = 2 * 1024 * 1024;

function sha256(bytes) {
  return crypto.createHash('sha256').update(bytes).digest('hex');
}

/* Find the exact seed files named by a generation record. Hashing every bounded
 * JSON under the requested roots is deliberate: the digest, not a file name,
 * proves the seed identity. */
export function findSeeds(directories, digests) {
  const wanted = new Set(digests);
  const found = new Map();
  const walk = directory => {
    for (const entry of fs.readdirSync(directory, { withFileTypes: true })) {
      const full = path.join(directory, entry.name);
      if (entry.isDirectory()) { walk(full); continue; }
      if (!entry.name.endsWith('.json')) continue;
      let bytes;
      try { bytes = fs.readFileSync(full); } catch { continue; }
      if (bytes.length < SEED_MIN || bytes.length > SEED_MAX) continue;
      const digest = sha256(bytes);
      if (wanted.has(digest) && !found.has(digest)) found.set(digest, full);
    }
  };
  for (const directory of directories) {
    const resolved = path.resolve(directory);
    if (!fs.statSync(resolved).isDirectory())
      throw new Error(`Seed root is not a directory: ${directory}`);
    walk(resolved);
  }
  return {
    seeds: [...found.values()],
    missing: digests.filter(digest => !found.has(digest)),
  };
}

function runPython(python, args) {
  const result = spawnSync(python, args, { encoding: 'utf8', timeout: 600000 });
  if (result.error) throw new Error(`Cannot run ${python}: ${result.error.message}`);
  return {
    status: result.status,
    stdout: (result.stdout ?? '').trim(),
    stderr: (result.stderr ?? '').trim(),
  };
}

function printAndCheck(label, result) {
  if (result.stdout) console.log(result.stdout);
  if (result.status !== 0) {
    if (result.stderr) console.error(result.stderr);
    throw new Error(`${label} failed`);
  }
}

function main(argv) {
  const options = { seedDirs: [], json: false };
  for (let index = 0; index < argv.length; ++index) {
    const argument = argv[index];
    if (argument === '--json') { options.json = true; continue; }
    const next = () => {
      if (index + 1 >= argv.length) throw new Error(`Missing value for ${argument}`);
      return argv[++index];
    };
    if (argument === '--translation') options.translation = next();
    else if (argument === '--control') options.control = next();
    else if (argument === '--experiment') options.experiment = next();
    else if (argument === '--seed-dir') options.seedDirs.push(next());
    else if (argument === '--python') options.python = next();
    else throw new Error(`Unknown argument: ${argument}`);
  }
  if (!options.translation || !options.control || !options.experiment) {
    console.error('Usage: node tools/verify-recomp-package.mjs --translation <dir> --control <json> --experiment <json> [--seed-dir <dir>]... [--python <interpreter>] [--json]');
    return 2;
  }
  const python = options.python ?? process.env.FF_PYTHON ?? 'python3';
  const seedDirs = options.seedDirs.length ? options.seedDirs : ['research/verification'];
  const translation = path.resolve(options.translation);

  try {
    const generation = JSON.parse(fs.readFileSync(path.join(translation, 'generation.json'), 'utf8'));
    if (!Array.isArray(generation.seedSha256s) || !generation.seedSha256s.length)
      throw new Error('The generation record has no seed digest list');
    const seeds = findSeeds(seedDirs, generation.seedSha256s);
    if (seeds.missing.length)
      throw new Error(`Missing ${seeds.missing.length} recorded seed(s): ${seeds.missing.join(', ')}`);

    const seedResult = runPython(python, [
      'tools/verify-seeds.py', '--translation', translation,
      ...seeds.seeds.flatMap(seed => ['--seed', seed]),
    ]);
    printAndCheck('verify-seeds', seedResult);

    const translationResult = runPython(python, ['tools/verify-translation.py', translation]);
    printAndCheck('verify-translation', translationResult);
    const generated = Number((translationResult.stdout.match(/functions:\s*(\d+)/) ?? [])[1]);
    const rewrites = Number((translationResult.stdout.match(/rewriting:\s*(\d+)/) ?? [])[1]);

    const control = JSON.parse(fs.readFileSync(path.resolve(options.control), 'utf8'));
    const experiment = JSON.parse(fs.readFileSync(path.resolve(options.experiment), 'utf8'));
    const pair = compareProofProfiles(control, experiment);

    const summary = {
      seeds: { recorded: generation.seedSha256s.length, offered: seeds.seeds.length, missing: 0 },
      translation: { admitted: generation.functions.length, generated, rewrites },
      pair: { ok: pair.ok, delta: pair.delta, nextStop: pair.nextStop },
    };
    if (options.json) console.log(JSON.stringify(summary, null, 2));
    else {
      console.log(`seeds: ${summary.seeds.recorded}/${summary.seeds.recorded} (0 missing)`);
      console.log(`translation: ${summary.translation.admitted} admitted functions; guard: ${generated} generated / ${rewrites} rewrites`);
      console.log(`controlled pair: ok; +${pair.delta.routines} routines / +${pair.delta.kernelOperations} operations; next stop 0x${(pair.nextStop.target >>> 0).toString(16).toUpperCase()}`);
      console.log('package verified');
    }
    return 0;
  } catch (error) {
    console.error(`Package verification failed: ${error.message}`);
    return 1;
  }
}

if (process.argv[1] && import.meta.url === new URL(`file://${path.resolve(process.argv[1])}`).href)
  process.exitCode = main(process.argv.slice(2));
