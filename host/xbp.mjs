// Read-only, bounded experimental XBP table reader. It does not decode resources.
// Format lead: https://reshax.com/topic/18058-ripping-sounds-from-fuzion-frenzy-xbox/
const MAX_TABLE = 8 * 1024 * 1024;
const MAX_STRINGS = 8 * 1024 * 1024;
const MAX_ENTRIES = 100000;
const MAX_NAME = 1024, MAX_NAME_BYTES = 8 * 1024 * 1024;
const decoder = new TextDecoder('utf-8', { fatal: true });

async function read(blob, offset, length) {
  if (!Number.isSafeInteger(offset) || !Number.isSafeInteger(length) || offset < 0 || length < 0 || offset > blob.size || length > blob.size - offset) throw new Error('XBP range outside input');
  const buffer = await blob.slice(offset, offset + length).arrayBuffer();
  if (buffer.byteLength !== length) throw new Error('XBP truncated read');
  return buffer;
}

export async function readXbpIndex(blob) {
  const h = new DataView(await read(blob, 0, 64));
  const unit = h.getUint32(4, true), count = h.getUint32(12, true);
  const table = h.getUint32(16, true) * unit, names = h.getUint32(40, true) * unit;
  const nameSize = h.getUint32(44, true), tableSize = h.getUint32(48, true);
  if (!unit || count > MAX_ENTRIES || tableSize !== count * 32 || tableSize > MAX_TABLE || nameSize > MAX_STRINGS) throw new Error('Unsupported XBP table dimensions');
  if (!count) {
    if (nameSize !== 0) throw new Error('Unexpected empty XBP table');
    return [];
  }
  const entries = new DataView(await read(blob, table, tableSize));
  const strings = new Uint8Array(await read(blob, names, nameSize));
  const result = []; let totalNameBytes=0;
  for (let i = 0; i < count; i++) {
    const row = i * 32;
    const offset = entries.getUint32(row, true) * unit;
    const size = entries.getUint32(row + 8, true);
    const nameOffset = entries.getUint32(row + 12, true);
    if (offset > blob.size || size > blob.size - offset) throw new Error('XBP resource range outside input');
    // In this build a table row refers to the complete string table itself.
    if (nameOffset === nameSize && offset === names && size === nameSize) continue;
    if (nameOffset >= nameSize) throw new Error('XBP name offset outside string table');
    let end = nameOffset;
    while (end < nameSize && strings[end] !== 0 && end-nameOffset<MAX_NAME) end++;
    if(end-nameOffset===MAX_NAME && end<nameSize && strings[end]!==0) throw new Error('XBP resource name too long');
    totalNameBytes += end-nameOffset;
    if(totalNameBytes>MAX_NAME_BYTES) throw new Error('XBP name budget exceeded');
    if (end === nameSize) throw new Error('Unterminated XBP resource name');
    let name;
    try { name = decoder.decode(strings.subarray(nameOffset, end)).replaceAll('\\', '/'); }
    catch { throw new Error('Invalid XBP resource name'); }
    const parts = name.split('/');
    if (!name || name.startsWith('/') || parts.some(part => !part || part === '.' || part === '..' || part.includes(':'))) throw new Error('Unsafe XBP resource name');
    result.push({ name, offset, size, hash: entries.getUint32(row + 4, true) });
  }
  return result;
}

export function xbpResource(blob, entry) {
  if (!Number.isSafeInteger(entry.offset) || !Number.isSafeInteger(entry.size) || entry.offset < 0 || entry.size < 0 || entry.offset > blob.size || entry.size > blob.size - entry.offset) throw new Error('XBP resource range outside input');
  return blob.slice(entry.offset, entry.offset + entry.size);
}
