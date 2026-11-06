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
  { name: 'guest_mirror', sources: ['tests/guest_mirror_test.c', runtime('guest_mirror.c')] },
  { name: 'guest_write_map', sources: ['tests/guest_write_map_test.c', runtime('guest_write_map.c')] },
  { name: 'pfifo_channel', sources: ['tests/pfifo_channel_test.c', runtime('pfifo_channel.c')] },
  { name: 'pfifo_dma', sources: ['tests/pfifo_dma_test.c', runtime('pfifo_dma.c')] },
  { name: 'pfifo_pusher', sources: ['tests/pfifo_pusher_test.c', runtime('pfifo_pusher.c'), runtime('pfifo_status.c')] },
  { name: 'vblank_wait', sources: ['tests/vblank_wait_test.c', runtime('vblank_wait.c')] },
  { name: 'display_mode', sources: ['tests/display_mode_test.c', runtime('display_mode.c')] },
  { name: 'av_unknown_option', sources: ['tests/av_unknown_option_test.c', runtime('av_unknown_option.c')] },
  { name: 'av_saved_data', sources: ['tests/av_saved_data_test.c', runtime('av_saved_data.c')] },
  { name: 'av_null_result', sources: ['tests/av_null_result_test.c', runtime('av_null_result.c')] },
  { name: 'mm_free_contiguous', sources: ['tests/mm_free_contiguous_test.c', runtime('mm_free_contiguous.c')] },
  { name: 'tls_scope', sources: ['tests/tls_scope_test.c', runtime('tls_scope.c')] },
  { name: 'interrupt_vector', sources: ['tests/interrupt_vector_test.c', runtime('interrupt_vector.c'), runtime('virtual_memory.c')] },
  { name: 'reg_save_order', sources: ['tests/reg_save_order_test.c', runtime('reg_save_order.c')] },
  { name: 'reg_save_compiled_probe', sources: ['tests/reg_save_compiled_probe.c'] },
  { name: 'av_query_fixture', sources: ['tests/av_query_fixture_test.c', runtime('av_query.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')] },
  { name: 'av_query_adapter', sources: ['tests/av_query_adapter_test.c', runtime('av_query_adapter.c'), runtime('av_query.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')] },
  { name: 'fatx_volume', sources: ['tests/fatx_volume_test.c', runtime('fatx_volume.c'), runtime('local_disk.c'), runtime('file_namespace.c'), runtime('local_volume.c'), runtime('local_directories.c'), runtime('object_namespace.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')] },
  { name: 'local_files', sources: ['tests/local_files_test.c', runtime('local_volume.c'), runtime('local_directories.c'), runtime('local_files.c'), runtime('virtual_memory.c'), runtime('thread_bootstrap.c')] },
  { name: 'kernel_error_map', sources: ['tests/kernel_error_map_test.c', runtime('kernel_error_map.c')] },
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
  for (const fixture of fixtures) {
    for (const [mode, options] of [['', flags], [' (asan/ubsan)', sanitizer]]) {
      const binary = compilerPath(path.join(scratch,
        `${fixture.name}${mode ? '-san' : ''}${compiler.wsl ? '' : '.exe'}`), compiler.wsl);
      try {
        const plan = commandPlan(compiler, options, fixture.sources, binary);
        spawn(plan.compile.command, plan.compile.args);
        spawn(plan.run.command, plan.run.args);
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

if (process.argv[1] && import.meta.url === pathToFileURL(process.argv[1]).href) {
  process.exitCode = run();
}
