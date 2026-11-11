#!/usr/bin/env python3
"""Loopback-only, allowlisted host for the local-ISO browser build."""
import argparse
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
import threading
import webbrowser

ROOT = Path(__file__).resolve().parents[1]
ROUTES = {
    '/sdk-browser/': ('web/sdk-browser/index.html', 'text/html; charset=utf-8'),
    '/sdk-browser/index.html': ('web/sdk-browser/index.html', 'text/html; charset=utf-8'),
    '/sdk-browser/app.mjs': ('web/sdk-browser/app.mjs', 'text/javascript; charset=utf-8'),
    '/sdk-browser/worker.mjs': ('web/sdk-browser/worker.mjs', 'text/javascript; charset=utf-8'),
    '/host/xdvdfs.mjs': ('host/xdvdfs.mjs', 'text/javascript; charset=utf-8'),
    '/artifacts/sdk-browser-full/fuzion-sdk-full.mjs': ('.tools/artifacts/sdk-browser-full/fuzion-sdk-full.mjs', 'text/javascript; charset=utf-8'),
    '/artifacts/sdk-browser-full/fuzion-sdk-full.wasm': ('.tools/artifacts/sdk-browser-full/fuzion-sdk-full.wasm', 'application/wasm'),
}


class Handler(BaseHTTPRequestHandler):
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
        self.send_response(200)
        self.send_header('Content-Type', route[1])
        self.send_header('Content-Length', str(source.stat().st_size))
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        self.send_header('Content-Security-Policy', "default-src 'none'; script-src 'self' 'wasm-unsafe-eval'; style-src 'unsafe-inline'; worker-src 'self'; connect-src 'self'; base-uri 'none'; form-action 'none'")
        self.send_header('Cache-Control', 'no-store')
        self.send_header('X-Content-Type-Options', 'nosniff')
        self.send_header('Referrer-Policy', 'no-referrer')
        self.end_headers()
        if self.command == 'HEAD':
            return
        try:
            with source.open('rb') as stream:
                while block := stream.read(1024 * 1024):
                    self.wfile.write(block)
        except (BrokenPipeError, ConnectionResetError):
            pass


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--port', type=int, default=4182)
    parser.add_argument('--open', action='store_true')
    args = parser.parse_args()
    if not 1024 <= args.port <= 65535:
        parser.error('Port must be between 1024 and 65535')
    server = ThreadingHTTPServer(('127.0.0.1', args.port), Handler)
    url = f'http://127.0.0.1:{args.port}/sdk-browser/'
    print(f'Fuzion Frenzy browser host: {url}\nKeep this window open. Ctrl+C stops the host.', flush=True)
    if args.open:
        threading.Timer(0.5, lambda: webbrowser.open(url)).start()
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
