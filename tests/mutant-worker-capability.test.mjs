import assert from 'node:assert/strict';
import { readFileSync } from 'node:fs';
import { fileURLToPath } from 'node:url';
import test from 'node:test';
import vm from 'node:vm';
import { createProofClock } from '../runtime/browser/browser-clock.mjs';
import { parseLocalConsoleSettings } from '../runtime/browser/local-title-proof.mjs';

const sourcePath = fileURLToPath(new URL('../runtime/browser/thread-proof-worker.mjs', import.meta.url));
const workerSource = readFileSync(sourcePath, 'utf8').replace(/^import .*?;\s*$/gm, '');
const mutantProfile = { kind: 'synthetic-unnamed-unowned-mutants', synthetic: true, capacity: 4 };
const settingsBytes = new TextEncoder().encode(JSON.stringify({
  kind: 'local-console-settings', synthetic: true, miscFlags: 0, mutants: mutantProfile,
})).buffer;

function workerAttempt(abi, capabilityNames) {
  const calls = [];
  const messages = [];
  const api = {
    ff_probe_abi() { return abi; },
    ff_probe_run() { calls.push('run'); },
    ff_probe_mutant_profile() { calls.push('mutant-profile'); return 0; },
    ff_probe_mutant_word() { calls.push('mutant-word'); return 0; },
  };
  for (const name of ['profile', 'word']) {
    if (!capabilityNames.includes(name)) delete api[`ff_probe_mutant_${name}`];
  }
  const self = { postMessage(message) { messages.push(message); } };
  let loadCount = 0;
  const context = vm.createContext({
    self, console, ArrayBuffer, Uint8Array, Uint32Array, DataView, TextDecoder, TextEncoder,
    WebAssembly, performance: { now: () => 0 },
    BUILD_REFERENCE: {},
    parseLocalConsoleSettings,
    createProofClock,
    loadLocalTitleProof: async () => {
      loadCount++;
      return { api, memory: new ArrayBuffer(64 * 1024 * 1024), range: () => 0 };
    },
  });
  vm.runInContext(workerSource, context, { filename: 'thread-proof-worker.mjs' });
  return context.self.onmessage({ data: { settingsBytes } }).then(() => ({ calls, messages, loadCount }));
}

test('selected mutant model rejects ABI 2 before any probe execution despite synthetic exports', async () => {
  const result = await workerAttempt(2, ['profile', 'word']);
  assert.equal(result.loadCount, 1);
  assert.deepEqual(result.calls, []);
  assert.equal(result.messages.length, 1);
  assert.equal(result.messages[0].kind, 'error');
  assert.match(result.messages[0].message, /Missing explicit synthetic mutant creation capability/);
});

test('selected mutant model rejects ABI 25 when either required profile or word export is missing', async () => {
  for (const exports of [[], ['profile'], ['word']]) {
    const result = await workerAttempt(25, exports);
    assert.equal(result.loadCount, 1);
    assert.deepEqual(result.calls, []);
    assert.equal(result.messages.length, 1);
    assert.equal(result.messages[0].kind, 'error');
    assert.match(result.messages[0].message, /Missing explicit synthetic mutant creation capability/);
  }
});
