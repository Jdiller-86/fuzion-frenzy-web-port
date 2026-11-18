#!/usr/bin/env python3
"""Loopback-only, allowlisted host for the local-ISO browser build."""
import argparse
import errno
import hashlib
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import os
from pathlib import Path
import threading
import webbrowser

ROOT = Path(__file__).resolve().parents[1]
ROUTES = {
    '/sdk-browser/': ('web/sdk-browser/index.html', 'text/html; charset=utf-8'),
    '/sdk-browser/index.html': ('web/sdk-browser/index.html', 'text/html; charset=utf-8'),
    '/sdk-browser/app.mjs': ('web/sdk-browser/app.mjs', 'text/javascript; charset=utf-8'),
    '/sdk-browser/worker.mjs': ('web/sdk-browser/worker.mjs', 'text/javascript; charset=utf-8'),
    '/sdk-browser/presentation.mjs': ('web/sdk-browser/presentation.mjs', 'text/javascript; charset=utf-8'),
    '/sdk-browser/webgl-renderer.mjs': ('web/sdk-browser/webgl-renderer.mjs', 'text/javascript; charset=utf-8'),
    '/sdk-browser/pixel-glsl.mjs': ('web/sdk-browser/pixel-glsl.mjs', 'text/javascript; charset=utf-8'),
    '/sdk-browser/vertex-glsl.mjs': ('web/sdk-browser/vertex-glsl.mjs', 'text/javascript; charset=utf-8'),
    '/sdk-browser/fixed-vertex-fallback.mjs': ('web/sdk-browser/fixed-vertex-fallback.mjs', 'text/javascript; charset=utf-8'),
    '/sdk-browser/programmable-vertex-fallback.mjs': ('web/sdk-browser/programmable-vertex-fallback.mjs', 'text/javascript; charset=utf-8'),
    '/sdk-browser/shader-warmup.mjs': ('web/sdk-browser/shader-warmup.mjs', 'text/javascript; charset=utf-8'),
    '/host/xdvdfs.mjs': ('host/xdvdfs.mjs', 'text/javascript; charset=utf-8'),
    '/artifacts/sdk-browser-full/fuzion-sdk-full.mjs': ('.tools/artifacts/sdk-browser-full/fuzion-sdk-full.mjs', 'text/javascript; charset=utf-8'),
    '/artifacts/sdk-browser-full/fuzion-sdk-full.wasm': ('.tools/artifacts/sdk-browser-full/fuzion-sdk-full.wasm', 'application/wasm'),
    '/artifacts/sdk-browser-full/fuzion-sdk-full.worker.js': ('.tools/artifacts/sdk-browser-full/fuzion-sdk-full.worker.js', 'text/javascript; charset=utf-8'),
}


class Handler(BaseHTTPRequestHandler):
    _wasm_validator = None

    @classmethod
    def wasm_etag(cls, stream, stat):
        key = (stat.st_dev, stat.st_ino, stat.st_size, stat.st_mtime_ns, stat.st_ctime_ns)
        cached = cls._wasm_validator
        if cached and cached[0] == key:
            return cached[1]
        digest = hashlib.sha256()
        while block := stream.read(1024 * 1024):
            digest.update(block)
        stream.seek(0)
        etag = f'"{digest.hexdigest()}"'
        cls._wasm_validator = (key, etag)
        return etag

    def do_HEAD(self):
        self.do_GET()

    def do_GET(self):
        route = ROUTES.get(self.path.split('?', 1)[0])
        if not route:
            self.send_error(404)
            return
        source = ROOT / route[0]
        if source.is_symlink() or not source.is_file():
            self.send_error(503, 'Build artifact missing; compile the full SDK browser module first')
            return
        try:
            with source.open('rb') as stream:
                stat = os.fstat(stream.fileno())
                # Tag the exact module so the page can reuse compiled code
                # when it starts another game worker.
                etag = self.wasm_etag(stream, stat) if route[1] == 'application/wasm' else None
                self.send_response(200)
                self.send_header('Content-Type', route[1])
                self.send_header('Content-Length', str(stat.st_size))
                self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
                self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
                self.send_header('Content-Security-Policy', "default-src 'none'; script-src 'self' 'wasm-unsafe-eval'; style-src 'unsafe-inline'; worker-src 'self'; connect-src 'self'; base-uri 'none'; form-action 'none'")
                self.send_header('Cache-Control', 'no-store')
                if etag:
                    self.send_header('ETag', etag)
                self.send_header('X-Content-Type-Options', 'nosniff')
                self.send_header('Referrer-Policy', 'no-referrer')
                self.end_headers()
                if self.command == 'HEAD':
                    return
                while block := stream.read(1024 * 1024):
                    self.wfile.write(block)
        except (BrokenPipeError, ConnectionResetError):
            pass


def bind_server(port=None):
    # Bind the actual server directly: probing then reopening would race another app.
    ports = [port] if port is not None else [4182, 8765, 0]
    for candidate in ports:
        try:
            return ThreadingHTTPServer(('127.0.0.1', candidate), Handler)
        except OSError as error:
            retryable = (error.errno in (errno.EACCES, errno.EADDRINUSE)
                         or getattr(error, 'winerror', None) in (10013, 10048))
            if not retryable or candidate == ports[-1]:
                raise
            print(f'Local port {candidate} is unavailable; trying another port.', flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--port', type=int, default=None,
                        help='Use this port (0 lets Windows/the OS choose). Default: try 4182, then 8765, then an available port.')
    parser.add_argument('--open', action='store_true')
    args = parser.parse_args()
    if args.port is not None and args.port != 0 and not 1024 <= args.port <= 65535:
        parser.error('Port must be 0 or between 1024 and 65535')
    try:
        server = bind_server(args.port)
    except OSError as error:
        parser.exit(1, f'Cannot start the local browser host: {error}\n'
                    'Try: py -3.12 tools\\serve-sdk-browser.py --open --port 8765\n'
                    'If other ports also fail, check security software or Windows policy blocking local servers.\n')
    url = f'http://127.0.0.1:{server.server_address[1]}/sdk-browser/'
    print(f'Fuzion Frenzy browser host: {url}\nKeep this window open. Ctrl+C stops the host.', flush=True)
    if args.open:
        threading.Timer(0.5, lambda: webbrowser.open(url)).start()
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
