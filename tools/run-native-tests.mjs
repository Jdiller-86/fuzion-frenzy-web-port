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
import { fileURLToPath } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const runtime = (name) => `src/runtime/${name}`;

/* Each fixture lists its own translation units; a fixture that needs a new
 * source must say so here, which keeps the link set reviewable. */
const fixtures = [
  { name: 'device_registers', sources: ['tests/device_registers_test.c', runtime('device_registers.c'), runtime('thread_bootstrap.c')] },
  { name: 'pcrtc_state', sources: ['tests/pcrtc_state_test.c', runtime('pcrtc_state.c')] },
  { name: 'kernel_timers', sources: ['tests/kernel_timers_test.c', runtime('kernel_timers.c'), runtime('kernel_objects.c')] },
  { name: 'port_io', sources: ['tests/port_io_test.c', runtime('port_io.c')] },
  { name: 'controller_device', sources: ['tests/controller_device_test.c', runtime('controller_device.c'), runtime('controller_port.c'), runtime('controller_packet.c')] },
  { name: 'controller_packet', sources: ['tests/controller_packet_test.c', runtime('controller_packet.c')] },
  { name: 'controller_port', sources: ['tests/controller_port_test.c', runtime('controller_port.c'), runtime('controller_packet.c')] },
  { name: 'pfifo_status', sources: ['tests/pfifo_status_test.c', runtime('pfifo_status.c')] },
  { name: 'av_query_fixture', sources: ['tests/av_query_fixture_test.c', runtime('av_query.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')] },
  { name: 'av_query_adapter', sources: ['tests/av_query_adapter_test.c', runtime('av_query_adapter.c'), runtime('av_query.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')] },
  { name: 'fatx_volume', sources: ['tests/fatx_volume_test.c', runtime('fatx_volume.c'), runtime('local_disk.c'), runtime('file_namespace.c'), runtime('local_volume.c'), runtime('local_directories.c'), runtime('object_namespace.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')] },
  { name: 'recomp_memory_access_xmm', sources: ['tests/recomp_memory_access_xmm_test.c'] },
  { name: 'recomp_memory_access_vm', sources: ['tests/recomp_memory_access_vm_test.c', runtime('virtual_memory.c'), runtime('thread_bootstrap.c')] },
];
const flags = ['-std=c11', '-O2', '-Wall', '-Wextra', '-Werror', '-I', 'src/runtime'];
/* The checkpoints quote sanitizer runs, so the runner reproduces them too. */
const sanitizer = ['-std=c11', '-O1', '-g', '-Wall', '-Wextra', '-Werror', '-I', 'src/runtime',
  '-fsanitize=address,undefined', '-fno-sanitize-recover=all'];

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

function run() {
  const compiler = findCompiler();
  if (!compiler) {
    console.error('No C compiler found. Set FF_CC or install cc/gcc/clang (WSL gcc is detected on Windows).');
    return 2;
  }
  const invoke = (args) => execFileSync(compiler.wsl ? 'wsl' : compiler.command,
    compiler.wsl ? [...compiler.prefix, args[0], ...args.slice(1)] : args,
    { cwd: root, stdio: 'pipe', timeout: 300000 });
  const scratch = scratchDirectory();
  const failures = [];
  for (const fixture of fixtures) {
    for (const [mode, options] of [['', flags], [' (asan/ubsan)', sanitizer]]) {
      const binary = compilerPath(path.join(scratch,
        `${fixture.name}${mode ? '-san' : ''}${compiler.wsl ? '' : '.exe'}`), compiler.wsl);
      try {
        invoke(['gcc', ...options, ...fixture.sources, '-o', binary]);
        invoke([binary]);
        console.log(`ok ${fixture.name}${mode}`);
      } catch (error) {
        const output = `${error.stdout ?? ''}${error.stderr ?? ''}`.toString().trim();
        failures.push(`${fixture.name}${mode}`);
        console.error(`not ok ${fixture.name}${mode}${output ? `\n${output}` : ''}`);
      }
    }
  }
  fs.rmSync(scratch, { recursive: true, force: true });
  const total = fixtures.length * 2;
  console.log(`${total - failures.length}/${total} native component runs passed`);
  return failures.length ? 1 : 0;
}

process.exitCode = run();
