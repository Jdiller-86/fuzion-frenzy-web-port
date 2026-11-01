import test from 'node:test';
import assert from 'node:assert/strict';
import http from 'node:http';
import { inspectProofWasm, PROOF_MEMORY_PAGES, PROOF_WASM_LIMIT } from '../src/runtime/wasm-proof.mjs';
import { createStartupProofServer } from '../tools/serve-startup-proof.mjs';
import { loadLocalTitleProof } from '../src/runtime/local-title-proof.mjs';

const header = [0, 97, 115, 109, 1, 0, 0, 0];
const uint = value => {
  const bytes = [];
  do { const low = value % 128; value = Math.floor(value / 128); bytes.push(low | (value ? 128 : 0)); } while (value);
  return bytes;
};
const section = (id, bytes) => [id, ...uint(bytes.length), ...bytes];
const memory = (minimum = 1, maximum = PROOF_MEMORY_PAGES, flags = 1, count = 1) =>
  section(5, [count, flags, ...uint(minimum), ...uint(maximum)]);
const module = (...sections) => Uint8Array.from([...header, ...sections.flat()]).buffer;

test('title execution loader rejects non-browser-worker execution before compiling or instantiating a module', async () => {
  await assert.rejects(loadLocalTitleProof({}, 2), /requires a browser worker/);
});

test('proof admission accepts bounded memory and leaves full Wasm validation to the engine', () => {
  const bytes = module(memory());
  assert.equal(WebAssembly.validate(bytes), true);
  assert.deepEqual(inspectProofWasm(bytes), { minimumPages: 1, maximumPages: PROOF_MEMORY_PAGES });
  assert.equal(WebAssembly.validate(module(section(5, [1, 1, 1, 1]), section(7, [1]))), false);
});

test('proof admission rejects uncapped, shared, multiple and excessive memories', () => {
  for (const bytes of [module(memory(1, 1, 0)), module(memory(1, 1, 3)), module(memory(1, 1, 5)),
    module(memory(1, 1, 1, 2)), module(memory(1, PROOF_MEMORY_PAGES + 1)), module(memory(2, 1)),
    module(memory(), memory())]) {
    assert.throws(() => inspectProofWasm(bytes));
  }
});

test('proof admission rejects valid host-import and automatic-start modules before instantiation', () => {
  const type = section(1, [1, 0x60, 0, 0]);
  const imported = module(type, section(2, [1, 3, 101, 110, 118, 1, 102, 0, 0]), memory());
  const started = module(type, section(3, [1, 0]), memory(), section(8, [0]), section(10, [1, 2, 0, 11]));
  assert.equal(WebAssembly.validate(imported), true);
  assert.equal(WebAssembly.validate(started), true);
  assert.throws(() => inspectProofWasm(imported), /no host imports/);
  assert.throws(() => inspectProofWasm(started), /Implicit module startup/);
});

test('proof admission bounds file sizes, section spans and unsigned integer decoding', () => {
  const badMagic = new Uint8Array(module(memory())); badMagic[0] = 1;
  for (const bytes of [badMagic.buffer, module(), new ArrayBuffer(PROOF_WASM_LIMIT + 1),
    Uint8Array.from([...header, 5, 127, 1]).buffer,
    Uint8Array.from([...header, 5, 0x80]).buffer,
    Uint8Array.from([...header, 5, 0xff, 0xff, 0xff, 0xff, 0x10]).buffer]) {
    assert.throws(() => inspectProofWasm(bytes));
  }
  assert.throws(() => inspectProofWasm(new Uint8Array(module(memory()))));
});

test('startup host serves only owned source and rejects original modules, files, queries and uploads', async t => {
  const server = createStartupProofServer();
  await new Promise((resolve, reject) => { server.once('error', reject); server.listen(0, '127.0.0.1', resolve); });
  t.after(() => new Promise(resolve => server.close(resolve)));
  const request = (url, method = 'GET', body) => new Promise((resolve, reject) => {
    const req = http.request({ host: '127.0.0.1', port: server.address().port, path: url, method, headers: { Connection: 'close' } }, res => {
      const chunks = []; res.on('data', bytes => chunks.push(bytes));
      res.on('end', () => resolve({ status: res.statusCode, headers: res.headers, body: Buffer.concat(chunks).toString('utf8') }));
    });
    req.on('error', reject); req.end(body);
  });
  for (const url of ['/', '/thread', '/src/xbe.mjs', '/src/runtime/wasm-proof.mjs', '/src/runtime/startup-proof-worker.mjs',
    '/src/runtime/local-title-proof.mjs', '/src/runtime/thread-proof-worker.mjs']) {
    const response = await request(url);
    assert.equal(response.status, 200);
    assert.ok(response.body.length > 100);
    assert.match(response.headers['content-security-policy'], /connect-src 'none'/);
    assert.match(response.headers['content-security-policy'], /form-action 'none'/);
    assert.equal(response.headers['cross-origin-embedder-policy'], 'require-corp');
    const head = await request(url, 'HEAD');
    assert.equal(head.status, 200); assert.equal(head.body, '');
  }
  for (const url of ['/startup.wasm', '/default.xbe', '/Fuzion%20Frenzy%20(USA)/default.xbe',
    '/research/verification/recomp-title-build-02/startup.wasm', '/.tools/emulation/emsdk/.emscripten',
    '/src/xbe.mjs?content=game', '/src/../../research/reference/default.xbe', '/%2e%2e/default.xbe']) {
    assert.equal((await request(url)).status, 404);
  }
  assert.equal((await request('/', 'POST', 'source-owned payload')).status, 404);
  assert.equal((await request('/src/xbe.mjs', 'PUT', 'source-owned payload')).status, 404);
});
