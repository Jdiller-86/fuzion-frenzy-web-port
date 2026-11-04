#!/usr/bin/env node
/**
 * Run the Python evidence suites with one command.
 *
 * The memory-access guard tests, the title-seed checks and the generator-to-
 * compiled regression are cited in checkpoints but were only ever run by hand.
 * This runner locates a Python 3 interpreter (an explicit FF_PYTHON, a local
 * python/python3, or WSL python3 on Windows, which can also compile the C
 * harness) and executes each suite in order, reporting ok/not ok per suite.
 *
 * A missing interpreter is an error (exit 2), never a pass. The generator test
 * skips itself with a clear message when no C compiler is available.
 */
import { execFileSync } from 'node:child_process';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const suites = [
  'tests/recomp_memory_access_test.py',
  'tests/recomp_memory_access_codegen_test.py',
  'tests/title_seed_test.py',
];

function probe(command, prefix) {
  try {
    execFileSync(command, [...prefix, '--version'], { cwd: root, stdio: 'ignore', timeout: 20000 });
    return true;
  } catch {
    return false;
  }
}

export function findPython(platform = process.platform) {
  const explicit = process.env.FF_PYTHON;
  if (explicit) return { command: explicit, prefix: [], wsl: false };
  /* WSL python3 is preferred on Windows because it can also build the compiled
   * harness the codegen suite uses. */
  if (platform === 'win32') {
    for (const prefix of [['-e'], ['-d', 'Ubuntu', '-e']]) {
      if (probe('wsl', [...prefix, 'python3'])) return { command: 'python3', prefix, wsl: true };
    }
  }
  for (const name of ['python3', 'python']) if (probe(name, [])) return { command: name, prefix: [], wsl: false };
  return null;
}

function run() {
  const python = findPython();
  if (!python) {
    console.error('No Python 3 interpreter found. Set FF_PYTHON or install python3 (WSL python3 is detected on Windows).');
    return 2;
  }
  const invoke = (args) => execFileSync(python.wsl ? 'wsl' : python.command,
    python.wsl ? [...python.prefix, python.command, ...args] : args,
    { cwd: root, stdio: 'pipe', timeout: 600000 });

  const failures = [];
  for (const suite of suites) {
    if (!fs.existsSync(path.join(root, suite))) {
      failures.push(suite);
      console.error(`not ok ${suite} (missing)`);
      continue;
    }
    try {
      invoke([suite]);
      console.log(`ok ${suite}`);
    } catch (error) {
      const output = `${error.stdout ?? ''}${error.stderr ?? ''}`.toString().trim();
      failures.push(suite);
      console.error(`not ok ${suite}${output ? `\n${output}` : ''}`);
    }
  }
  console.log(`${suites.length - failures.length}/${suites.length} python evidence suites passed`);
  return failures.length ? 1 : 0;
}

process.exitCode = run();
