#!/usr/bin/env node
/**
 * Compile and run the owned C component tests with one command.
 *
 * The project's component fixtures are plain C with explicit sources; until now
 * they were compiled ad hoc, so a "tests pass" statement in a checkpoint could
 * not be reproduced with one command. This runner finds a C compiler (an
 * explicit FF_CC, a local cc/gcc/clang, or the WSL gcc on Windows), builds every
 * listed fixture into an ignored scratch directory and runs it.
 *
 * It never downloads anything, never touches original content, and never reports
 * success for a fixture it did not compile and execute.
 */
import { execFileSync } from 'node:child_process';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
export { nativeFixtures as fixtures } from './native-fixtures.mjs';
import { nativeFixtures as fixtures } from './native-fixtures.mjs';

/* Build outside the repository so the preparation scanner never sees a binary,
 * and translate the path for WSL when that compiler is used. */
function scratchDirectory() {
  const base = fs.mkdtempSync(path.join(os.tmpdir(), 'ff-native-'));
  return base;
}
function compilerPath(p, wsl) {
  if (!wsl) return p;
  return '/mnt/' + p[0].toLowerCase() + p.slice(2).replaceAll('\\', '/');
}

export function expandScratchPath(value, scratch, compiler) {
  return value.replaceAll('{scratch}', compilerPath(scratch, compiler.wsl));
}

function probe(command, prefix) {
  try {
    execFileSync(command, [...prefix, '--version'], { cwd: root, stdio: 'ignore', timeout: 20000 });
    return true;
  } catch {
    return false;
  }
}

export function findCompiler(platform = process.platform) {
  const explicit = process.env.FF_CC;
  if (explicit) return { command: explicit, prefix: [], wsl: false };
  for (const name of ['cc', 'gcc', 'clang']) if (probe(name, [])) return { command: name, prefix: [], wsl: false };
  if (platform === 'win32') {
    /* The project's Windows toolchain lives in WSL; `-e` runs the program
     * directly instead of through a shell, so argument quoting is preserved. */
    for (const prefix of [['-e'], ['-d', 'Ubuntu', '-e']]) {
      if (probe('wsl', [...prefix, 'gcc'])) return { command: 'gcc', prefix, wsl: true };
    }
  }
  return null;
}

function findPython(platform = process.platform) {
  const explicit = process.env.FF_PYTHON;
  if (explicit) return { command: explicit, prefix: [], wsl: false };
  if (platform === 'win32') {
    for (const prefix of [['-e'], ['-d', 'Ubuntu', '-e']]) {
      if (probe('wsl', [...prefix, 'python3'])) return { command: 'python3', prefix, wsl: true };
    }
  }
  for (const name of ['python3', 'python']) if (probe(name, [])) return { command: name, prefix: [], wsl: false };
  return null;
}

export function preparePlan(python, args) {
  if (python.wsl) return { command: 'wsl', args: [...python.prefix, python.command, ...args] };
  return { command: python.command, args };
}

/* Build the exact argv for one compile and one run. A non-WSL host compiles
 * with the detected driver and executes the product directly; WSL needs the
 * `gcc` program name only inside `wsl`, never as a native compiler argument. */
export function commandPlan(compiler, options, sources, binary) {
  const compileArgs = [...options, ...sources, '-o', binary];
  if (compiler.wsl) {
    return {
      compile: { command: 'wsl', args: [...compiler.prefix, 'gcc', ...compileArgs] },
      run: { command: 'wsl', args: [...compiler.prefix, binary] },
    };
  }
  return {
    compile: { command: compiler.command, args: compileArgs },
    run: { command: binary, args: [] },
  };
}

function run() {
  const compiler = findCompiler();
  if (!compiler) {
    console.error('No C compiler found. Set FF_CC or install cc/gcc/clang (WSL gcc is detected on Windows).');
    return 2;
  }
  const spawn = (command, args) => execFileSync(command, args,
    { cwd: root, stdio: 'pipe', timeout: 300000 });
  const scratch = scratchDirectory();
  const failures = [];
  try {
    for (const fixture of fixtures) {
      if (fixture.prepare) {
        try {
          const python = findPython();
          if (!python) throw new Error('No Python 3 interpreter found for fixture preparation. Set FF_PYTHON or install python3.');
          const outputRoot = compilerPath(scratch, python.wsl);
          const plan = preparePlan(python,
            fixture.prepare.args.map((arg) => arg.replaceAll('{scratch}', outputRoot)));
          spawn(plan.command, plan.args);
        } catch (error) {
          const output = `${error.stdout ?? ''}${error.stderr ?? ''}`.toString().trim();
          for (const mode of fixture.modes) {
            failures.push(`${fixture.name}${mode.name}`);
            console.error(`not ok ${fixture.name}${mode.name} (prepare failed)${output ? `\n${output}` : `\n${error.message}`}`);
          }
          continue;
        }
      }
      for (const mode of fixture.modes) {
        const binary = compilerPath(path.join(scratch,
          `${fixture.name}${mode.suffix ?? ''}${compiler.wsl ? '' : '.exe'}`), compiler.wsl);
        try {
          const expandedOptions = mode.flags.map((flag) => expandScratchPath(flag, scratch, compiler));
          const plan = commandPlan(compiler, expandedOptions, fixture.sources, binary);
          spawn(plan.compile.command, plan.compile.args);
          spawn(plan.run.command, plan.run.args);
          console.log(`ok ${fixture.name}${mode.name}`);
        } catch (error) {
          const output = `${error.stdout ?? ''}${error.stderr ?? ''}`.toString().trim();
          failures.push(`${fixture.name}${mode.name}`);
          console.error(`not ok ${fixture.name}${mode.name}${output ? `\n${output}` : ''}`);
        }
      }
    }
  } finally {
    fs.rmSync(scratch, { recursive: true, force: true });
  }
  const total = fixtures.reduce((sum, fixture) => sum + fixture.modes.length, 0);
  console.log(`${total - failures.length}/${total} native component runs passed`);
  return failures.length ? 1 : 0;
}

if (process.argv[1] && import.meta.url === pathToFileURL(process.argv[1]).href) {
  process.exitCode = run();
}
