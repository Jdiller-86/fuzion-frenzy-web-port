import http from 'node:http';
import fs from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const files = new Map([
  ['/', ['tools/fixtures/local_startup_check.html', 'text/html; charset=utf-8']],
  ['/thread', ['tools/fixtures/thread_startup_check.html', 'text/html; charset=utf-8']],
  ['/host/xbe.mjs', ['host/xbe.mjs', 'text/javascript']],
  ['/runtime/browser/local-title-proof.mjs', ['runtime/browser/local-title-proof.mjs', 'text/javascript']],
  ['/runtime/browser/browser-clock.mjs', ['runtime/browser/browser-clock.mjs', 'text/javascript']],
  ['/runtime/browser/performance-clock.mjs', ['runtime/browser/performance-clock.mjs', 'text/javascript']],
  ['/runtime/browser/performance-counter.mjs', ['runtime/browser/performance-counter.mjs', 'text/javascript']],
  ['/runtime/browser/unnamed-mutants.mjs', ['runtime/browser/unnamed-mutants.mjs', 'text/javascript']],
  ['/runtime/browser/local-mutants.mjs', ['runtime/browser/local-mutants.mjs', 'text/javascript']],
  ['/runtime/browser/named-events.mjs', ['runtime/browser/named-events.mjs', 'text/javascript']],
  ['/runtime/browser/copy-proof.mjs', ['runtime/browser/copy-proof.mjs', 'text/javascript']],
  ['/runtime/browser/file-request.mjs', ['runtime/browser/file-request.mjs', 'text/javascript']],
  ['/runtime/browser/device-report.mjs', ['runtime/browser/device-report.mjs', 'text/javascript']],
  ['/runtime/browser/submission-probe.mjs', ['runtime/browser/submission-probe.mjs', 'text/javascript']],
  ['/runtime/browser/pfifo-scan.mjs', ['runtime/browser/pfifo-scan.mjs', 'text/javascript']],
  ['/runtime/browser/proof-report.mjs', ['runtime/browser/proof-report.mjs', 'text/javascript']],
  ['/runtime/browser/kernel-service.mjs', ['runtime/browser/kernel-service.mjs', 'text/javascript']],
  ['/runtime/browser/kernel-capture.mjs', ['runtime/browser/kernel-capture.mjs', 'text/javascript']],
  ['/runtime/browser/console-os-settings.mjs', ['runtime/browser/console-os-settings.mjs', 'text/javascript']],
  ['/runtime/browser/thread-proof-worker.mjs', ['runtime/browser/thread-proof-worker.mjs', 'text/javascript']],
  ['/runtime/browser/wasm-proof.mjs', ['runtime/browser/wasm-proof.mjs', 'text/javascript']],
  ['/runtime/browser/startup-proof-worker.mjs', ['runtime/browser/startup-proof-worker.mjs', 'text/javascript']]
]);

// Fixed source-only routes. Original XBE/Wasm/code is always selected locally.
export function createStartupProofServer() {
  return http.createServer(async (request, response) => {
    response.setHeader('Cache-Control', 'no-store');
    response.setHeader('Cross-Origin-Opener-Policy', 'same-origin');
    response.setHeader('Cross-Origin-Embedder-Policy', 'require-corp');
    response.setHeader('Content-Security-Policy', "default-src 'none'; script-src 'self' 'unsafe-inline' 'wasm-unsafe-eval'; style-src 'unsafe-inline'; worker-src 'self'; connect-src 'none'; base-uri 'none'; form-action 'none'");
    const file = files.get(request.url);
    if (!['GET', 'HEAD'].includes(request.method) || !file) return response.writeHead(404).end();
    try {
      const filename = path.join(root, file[0]);
      const stat = await fs.lstat(filename);
      if (!stat.isFile() || stat.size > 1024 * 1024) throw new Error('Invalid source file');
      const bytes = await fs.readFile(filename);
      response.writeHead(200, { 'Content-Type': file[1], 'Content-Length': bytes.length });
      response.end(request.method === 'HEAD' ? undefined : bytes);
    } catch { response.writeHead(404).end(); }
  });
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  const [portArg = '4180', ...extra] = process.argv.slice(2), port = Number(portArg);
  if (extra.length || !Number.isInteger(port) || port < 1024 || port > 65535) throw new Error('Supply one valid loopback port');
  createStartupProofServer().listen(port, '127.0.0.1', () => {
    console.log(`Source-only local startup host: http://127.0.0.1:${port}/ (PID ${process.pid})`);
  });
}
