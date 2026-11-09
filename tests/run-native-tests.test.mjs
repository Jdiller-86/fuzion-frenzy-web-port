import test from 'node:test';
import assert from 'node:assert/strict';

import { commandPlan, expandScratchPath, fixtures, preparePlan } from '../tools/run-native-tests.mjs';

test('a non-WSL host compiles with the driver and runs the product directly', () => {
  const plan = commandPlan(
    { command: 'cc', prefix: [], wsl: false },
    ['-std=c11', '-O2'],
    ['tests/x_test.c', 'runtime/core/x.c'],
    '/tmp/ff-native/x'
  );
  assert.equal(plan.compile.command, 'cc');
  assert.deepEqual(plan.compile.args,
    ['-std=c11', '-O2', 'tests/x_test.c', 'runtime/core/x.c', '-o', '/tmp/ff-native/x']);
  assert.ok(!plan.compile.args.includes('gcc'),
    'a native driver must not receive a bare gcc token as an input file');
  assert.equal(plan.run.command, '/tmp/ff-native/x');
  assert.deepEqual(plan.run.args, []);
});

test('the consolidated manifest retains distinct CI compile contracts', () => {
  const byName = new Map(fixtures.map((fixture) => [fixture.name, fixture]));
  assert.equal(fixtures.length, 64);
  assert.deepEqual(byName.get('nv2a_triangle').modes[0].flags,
    ['-std=c11', '-O2', '-Wall', '-Wextra', '-Werror', '-ffp-contract=off']);
  assert.ok(byName.get('fatx_root').sources.includes('runtime/core/local_disk.c'));
  assert.ok(byName.get('memory_access').prepare.args.includes('{scratch}/owned-memory.inc'));
  assert.ok(byName.get('memory_access').modes[0].flags.includes('{scratch}'));
  assert.ok(byName.get('av_query_ci').modes[1].flags.includes('-fno-omit-frame-pointer'));
  assert.ok(byName.get('recomp_memory_access_xmm').modes.some((mode) => mode.flags.includes('-O2')));
  for (const duplicate of ['interrupt_vector_ci', 'thread_bootstrap_ci', 'kernel_timers_ci', 'memory_statistics_ci', 'port_io_ci']) {
    assert.ok(!byName.has(duplicate), `${duplicate} has the same source and flags as an existing fixture`);
  }
});

test('fixture preparation uses native host paths and WSL compiler paths', () => {
  const hostPython = preparePlan({ command: 'python3', prefix: [], wsl: false },
    ['translator/recomp_memory_access.py', 'C:\\Temp\\ff-native\\owned-memory.inc']);
  assert.deepEqual(hostPython, {
    command: 'python3',
    args: ['translator/recomp_memory_access.py', 'C:\\Temp\\ff-native\\owned-memory.inc'],
  });
  const wslPython = preparePlan({ command: 'python3', prefix: ['-e'], wsl: true },
    ['translator/recomp_memory_access.py', '/mnt/c/Temp/ff-native/owned-memory.inc']);
  assert.deepEqual(wslPython, {
    command: 'wsl',
    args: ['-e', 'python3', 'translator/recomp_memory_access.py', '/mnt/c/Temp/ff-native/owned-memory.inc'],
  });
  assert.equal(expandScratchPath('-I{scratch}', 'C:\\Temp\\ff-native', { wsl: true }),
    '-I/mnt/c/Temp/ff-native');
  assert.equal(expandScratchPath('-I{scratch}', '/tmp/ff-native', { wsl: false }),
    '-I/tmp/ff-native');
});

test('a WSL host names the gcc program only inside the wsl command', () => {
  const plan = commandPlan(
    { command: 'gcc', prefix: ['-e'], wsl: true },
    ['-std=c11'],
    ['tests/x_test.c'],
    '/mnt/c/tmp/x'
  );
  assert.equal(plan.compile.command, 'wsl');
  assert.deepEqual(plan.compile.args,
    ['-e', 'gcc', '-std=c11', 'tests/x_test.c', '-o', '/mnt/c/tmp/x']);
  assert.equal(plan.run.command, 'wsl');
  assert.deepEqual(plan.run.args, ['-e', '/mnt/c/tmp/x']);
});
