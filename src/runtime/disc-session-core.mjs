import { openXiso } from '../xdvdfs.mjs';
import { requireBlobIdentity } from '../identity.mjs';

/* Bounded, read-only XISO session core. It identifies the supported executable
 * and exposes byte ranges only; it never returns the whole image, lists the
 * filesystem, converts assets or uploads content. A Blob is passed through
 * without a whole-image arrayBuffer read. */
export const DISC_MAX_READ = 65536;
const MAX_XBE_BYTES = 16 * 1024 * 1024;
const HASH = /^[a-f0-9]{64}$/;

export class DiscSessionError extends Error {
  constructor(code, message) {
    super(message || code);
    this.name = 'DiscSessionError';
    this.code = code;
  }
}
function fail(code, message) {
  throw new DiscSessionError(code, message);
}
function identity(value) {
  if (!value || typeof value !== 'object' || Array.isArray(value) ||
      !Number.isSafeInteger(value.size) || value.size < 1 || value.size > MAX_XBE_BYTES ||
      typeof value.sha256 !== 'string' || !HASH.test(value.sha256))
    fail('identity', 'An explicit supported executable size and SHA-256 are required');
  return value;
}

export class DiscSessionCore {
  constructor({ maxReadBytes = DISC_MAX_READ, maxInventoryEntries = 256 } = {}) {
    if (!Number.isSafeInteger(maxReadBytes) || maxReadBytes < 1 || maxReadBytes > DISC_MAX_READ)
      fail('limit', 'The read limit must be an integer from 1 to 65536');
    if (!Number.isSafeInteger(maxInventoryEntries) || maxInventoryEntries < 1 || maxInventoryEntries > 1024)
      fail('limit', 'The inventory limit must be an integer from 1 to 1024');
    this.maxReadBytes = maxReadBytes;
    this.maxInventoryEntries = maxInventoryEntries;
    this.image = null;
    this.fs = null;
    this.opened = false;
    this.closed = false;
    this.busy = false;
    this.dataBytesRead = 0;
  }

  async open(image, expectedIdentity) {
    if (this.opened || this.closed) fail('state', 'The session is already open or cleared');
    if (!(image instanceof Blob) || !image.size) fail('content', 'A non-empty local XISO blob is required');
    const expected = identity(expectedIdentity);
    let fs;
    try {
      fs = await openXiso(image);
    } catch (error) {
      fail('volume', error.message);
    }
    let entry;
    try {
      entry = await fs.find('default.xbe');
    } catch (error) {
      fail('identity', error.message);
    }
    if (!entry || entry.directory) fail('identity', 'The selected image has no default.xbe file');
    try {
      await requireBlobIdentity(fs.file(entry), expected.size, expected.sha256);
    } catch (error) {
      fail('identity', error.message);
    }
    this.image = image;
    this.fs = fs;
    this.opened = true;
    this.closed = false;
    this.busy = false;
    this.dataBytesRead = 0;
    return { imageBytes: image.size, xbeBytes: expected.size };
  }

  async read(path, offset, length) {
    if (!this.opened || this.closed) fail('state', 'The session is not open');
    if (this.busy) fail('busy', 'Only one read may be outstanding');
    if (typeof path !== 'string' || !path || path.length > 256 ||
        path.includes('\\') || path.includes('\0'))
      fail('path', 'Invalid XDVDFS path');
    if (!Number.isSafeInteger(offset) || offset < 0 ||
        !Number.isSafeInteger(length) || length < 1 || length > this.maxReadBytes)
      fail('range', 'The read range is outside the bounded response');
    this.busy = true;
    try {
      if (this.closed) fail('state', 'The session was cleared');
      let entry;
      try {
        entry = await this.fs.find(path);
      } catch (error) {
        fail('path', error.message);
      }
      if (this.closed) fail('state', 'The session was cleared');
      if (!entry || entry.directory) fail('path', 'The path is not a file');
      if (offset > entry.size || length > entry.size - offset)
        fail('range', 'The read range is outside the file');
      const bytes = new Uint8Array(await this.image.slice(entry.offset + offset,
        entry.offset + offset + length).arrayBuffer());
      if (this.closed) fail('state', 'The session was cleared');
      if (bytes.byteLength !== length) fail('range', 'Truncated XISO read');
      this.dataBytesRead += length;
      return { path, offset, length, fileSize: entry.size, bytes };
    } finally {
      this.busy = false;
    }
  }

  async inventory(path = '') {
    if (!this.opened || this.closed) fail('state', 'The session is not open');
    if (this.busy) fail('busy', 'Only one request may be outstanding');
    if (typeof path !== 'string' || path.length > 256 || path.includes('\\') || path.includes('\0'))
      fail('path', 'Invalid inventory path');
    this.busy = true;
    try {
      if (this.closed) fail('state', 'The session was cleared');
      let directory = this.fs.root;
      if (path) {
        try {
          directory = await this.fs.find(path);
        } catch (error) {
          fail('path', error.message);
        }
      }
      if (this.closed) fail('state', 'The session was cleared');
      if (!directory || !directory.directory) fail('path', 'The inventory path is not a directory');
      const entries = await this.fs.list(directory);
      if (entries.length > this.maxInventoryEntries)
        fail('limit', 'The directory inventory exceeds the bounded entry limit');
      if (this.closed) fail('state', 'The session was cleared');
      return entries.map(entry => ({ name: entry.name, size: entry.size, directory: entry.directory }));
    } finally {
      this.busy = false;
    }
  }

  close() {
    this.opened = false;
    this.closed = true;
    this.busy = false;
    this.image = null;
    this.fs = null;
  }
}
