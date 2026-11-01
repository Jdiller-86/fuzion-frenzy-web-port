import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { test } from 'node:test';
import { createSdlCheckServer } from '../tools/serve-sdl-check.mjs';

test('synthetic runtime proof serves isolated outputs with thread headers and rejects other files/uploads', async () => {
  const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '../.tools/emulation');
  await fs.mkdir(root, { recursive: true });
  const directory = await fs.mkdtemp(path.join(root, 'sdl-browser-test-'));
  const attempt = path.basename(directory).slice('sdl-browser-'.length);
  for (const name of ['index.html', 'check.js', 'check.worker.js', 'check.wasm', 'private.xbe']) {
    await fs.writeFile(path.join(directory, name), `synthetic:${name}`, { flag: 'wx' });
  }
  assert.throws(() => createSdlCheckServer('../reference'), /Invalid fixture/);
  const server = createSdlCheckServer(attempt);
  await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
  const base = `http://127.0.0.1:${server.address().port}`;
  try {
    for (const route of ['/', '/check.js', '/check.worker.js', '/check.wasm']) {
      const response = await fetch(base + route);
      assert.equal(response.status, 200);
      assert.equal(response.headers.get('cross-origin-opener-policy'), 'same-origin');
      assert.equal(response.headers.get('cross-origin-embedder-policy'), 'require-corp');
      assert.match(await response.text(), /^synthetic:/);
    }
    assert.equal((await fetch(base + '/check.wasm')).headers.get('content-type'), 'application/wasm');
    const head = await fetch(base + '/check.js', { method: 'HEAD' });
    assert.equal(head.status, 200);
    assert.equal(await head.text(), '');
    for (const route of ['/private.xbe', '/index.html', '/%2e%2e%2fprivate.xbe', '/check.js?file=private.xbe']) {
      assert.equal((await fetch(base + route)).status, 404);
    }
    assert.equal((await fetch(base + '/', { method: 'POST', body: 'synthetic-upload' })).status, 404);
    assert.equal(await fs.readFile(path.join(directory, 'private.xbe'), 'utf8'), 'synthetic:private.xbe');
  } finally {
    await new Promise(resolve => server.close(resolve));
  }
});
