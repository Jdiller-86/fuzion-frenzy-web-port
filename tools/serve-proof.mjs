// Development-only static server: only these source files can be requested.
import { createServer } from 'node:http';
import { readFile } from 'node:fs/promises';
import { fileURLToPath } from 'node:url';
const root = new URL('../', import.meta.url);
const allowed = new Map([['/host/transforms.mjs',['host/transforms.mjs','text/javascript; charset=utf-8']],['/host/clips.mjs',['host/clips.mjs','text/javascript; charset=utf-8']],['/', ['web/proof/index.html','text/html; charset=utf-8']], ['/web/proof/main.mjs',['web/proof/main.mjs','text/javascript; charset=utf-8']], ['/web/proof/worker.mjs',['web/proof/worker.mjs','text/javascript; charset=utf-8']], ['/host/xdvdfs.mjs',['host/xdvdfs.mjs','text/javascript; charset=utf-8']], ['/host/xbp.mjs',['host/xbp.mjs','text/javascript; charset=utf-8']], ['/host/audio.mjs',['host/audio.mjs','text/javascript; charset=utf-8']], ['/host/obe.mjs',['host/obe.mjs','text/javascript; charset=utf-8']], ['/host/character.mjs',['host/character.mjs','text/javascript; charset=utf-8']], ['/host/animation.mjs',['host/animation.mjs','text/javascript; charset=utf-8']], ['/host/texture.mjs',['host/texture.mjs','text/javascript; charset=utf-8']], ['/host/identity.mjs',['host/identity.mjs','text/javascript; charset=utf-8']], ['/web/proof/preview.mjs',['web/proof/preview.mjs','text/javascript; charset=utf-8']], ['/disc-session',['web/proof/disc-session.html','text/html; charset=utf-8']], ['/web/proof/disc-session.mjs',['web/proof/disc-session.mjs','text/javascript; charset=utf-8']], ['/runtime/browser/disc-session.mjs',['runtime/browser/disc-session.mjs','text/javascript; charset=utf-8']], ['/runtime/browser/disc-session-core.mjs',['runtime/browser/disc-session-core.mjs','text/javascript; charset=utf-8']], ['/runtime/browser/disc-session-worker.mjs',['runtime/browser/disc-session-worker.mjs','text/javascript; charset=utf-8']]]);
createServer(async (req,res) => {
  const item = req.method === 'GET' ? allowed.get(req.url) : undefined;
  if (!item) { res.writeHead(404); res.end(); return; }
  try {
    const data = await readFile(fileURLToPath(new URL(item[0],root)));
    res.writeHead(200, {'Content-Type': item[1], 'Cache-Control':'no-store', 'Content-Security-Policy': "default-src 'none'; script-src 'self'; worker-src 'self'; connect-src 'none'; base-uri 'none'; form-action 'none'", 'X-Content-Type-Options':'nosniff', 'Cross-Origin-Opener-Policy':'same-origin', 'Cross-Origin-Embedder-Policy':'require-corp'}); res.end(data);
  } catch { res.writeHead(500); res.end(); }
}).listen(4173, '127.0.0.1', () => console.log('Local research server at http://127.0.0.1:4173'));
