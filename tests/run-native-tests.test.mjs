import test from 'node:test';
import assert from 'node:assert/strict';

import { commandPlan } from '../tools/run-native-tests.mjs';

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
