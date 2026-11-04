import { DiscSessionCore } from './disc-session-core.mjs';

/* Browser worker boundary for the bounded read-only disc session. The selected
 * File/Blob is cloned into the worker; the core reads only the XDVDFS metadata,
 * the executable identity span and requested byte ranges. No content is
 * uploaded or forwarded to the page except the requested range. */
const session = new DiscSessionCore();

self.onmessage = async event => {
  const message = event.data;
  if (!message || typeof message !== 'object') return;
  const id = message.id;
  try {
    if (message.type === 'open') {
      const result = await session.open(message.image, message.expectedIdentity);
      self.postMessage({ type: 'opened', id, result });
    } else if (message.type === 'read') {
      const result = await session.read(message.path, message.offset, message.length);
      self.postMessage({ type: 'read-result', id, result }, [result.bytes.buffer]);
    } else if (message.type === 'inventory') {
      const result = await session.inventory();
      self.postMessage({ type: 'inventory-result', id, result });
    } else if (message.type === 'close') {
      session.close();
      self.postMessage({ type: 'closed', id });
    } else {
      throw new Error('Unknown disc-session message');
    }
  } catch (error) {
    self.postMessage({ type: 'error', id, code: error.code || 'error', message: error.message });
  }
};
