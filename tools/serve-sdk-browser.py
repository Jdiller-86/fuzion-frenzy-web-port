#!/usr/bin/env python3
"""Loopback-only, allowlisted host for the local-ISO browser build."""
import argparse
import errno
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
