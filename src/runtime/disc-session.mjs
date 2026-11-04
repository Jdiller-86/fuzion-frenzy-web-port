import { DISC_MAX_READ, DiscSessionError } from './disc-session-core.mjs';

/* Main-thread handle for one bounded read-only disc session. It enforces one
 * outstanding request, ignores late results after clear, and terminates the
 * worker on clear. It never receives the whole image: only the worker holds the
 * selected local Blob and returns requested ranges. */
const browserWorker = () => new Worker(new URL('./disc-session-worker.mjs', import.meta.url), { type: 'module' });

export class DiscSession {
  static async open(image, { expectedIdentity, workerFactory = browserWorker, timeoutMs = 10000 } = {}) {
    const worker = workerFactory();
    const session = new DiscSession(worker, timeoutMs);
    try {
      await session._request({ type: 'open', image, expectedIdentity }, 'opened');
      session.opened = true;
      return session;
    } catch (error) {
      session.clear();
      throw error;
    }
  }

  constructor(worker, timeoutMs) {
    this.worker = worker;
    this.timeoutMs = timeoutMs;
    this.pending = null;
    this.nextId = 0;
    this.opened = false;
    this.closed = false;
    worker.onmessage = event => this._onMessage(event.data);
    worker.onerror = event => this._fail(new DiscSessionError('worker', event.message || 'disc worker failed'));
  }

  _fail(error) {
    this.closed = true;
    this.opened = false;
    if (this.pending) {
      clearTimeout(this.pending.timer);
      const pending = this.pending;
      this.pending = null;
      pending.reject(error);
    }
    try { this.worker.terminate(); } catch { /* ignore */ }
  }

  _onMessage(message) {
    if (!message || !this.pending || message.id !== this.pending.id) return;
    const pending = this.pending;
    clearTimeout(pending.timer);
    this.pending = null;
    if (message.type === 'error') {
      pending.reject(new DiscSessionError(message.code, message.message));
    } else if (message.type === pending.expected) {
      pending.resolve(message.result);
    } else {
      pending.reject(new DiscSessionError('protocol', 'Unexpected disc-session response'));
    }
  }

  _request(message, expected) {
    if (this.closed) return Promise.reject(new DiscSessionError('state', 'The session was cleared'));
    if (this.pending) return Promise.reject(new DiscSessionError('busy', 'Only one request may be outstanding'));
    const id = ++this.nextId;
    return new Promise((resolve, reject) => {
      const timer = setTimeout(() => {
        if (this.pending && this.pending.id === id) {
          this.pending = null;
          this.closed = true;
          this.opened = false;
          reject(new DiscSessionError('timeout', 'The disc session timed out'));
          try { this.worker.terminate(); } catch { /* ignore */ }
        }
      }, this.timeoutMs);
      this.pending = { id, expected, resolve, reject, timer };
      try {
        this.worker.postMessage({ ...message, id });
      } catch (error) {
        clearTimeout(timer);
        this.pending = null;
        reject(new DiscSessionError('worker', error.message));
      }
    });
  }

  read(path, offset, length) {
    if (!this.opened || this.closed)
      return Promise.reject(new DiscSessionError('state', 'The session is not open'));
    if (length > DISC_MAX_READ)
      return Promise.reject(new DiscSessionError('range', 'A data read may not exceed 65536 bytes'));
    return this._request({ type: 'read', path, offset, length }, 'read-result');
  }

  inventory() {
    if (!this.opened || this.closed)
      return Promise.reject(new DiscSessionError('state', 'The session is not open'));
    return this._request({ type: 'inventory' }, 'inventory-result');
  }

  clear() {
    if (this.closed) return;
    this.closed = true;
    this.opened = false;
    if (this.pending) {
      clearTimeout(this.pending.timer);
      const pending = this.pending;
      this.pending = null;
      pending.reject(new DiscSessionError('cancelled', 'The session was cleared'));
    }
    try { this.worker.terminate(); } catch { /* ignore */ }
  }
}
