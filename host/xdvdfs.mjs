// Bounded reader for a bare XDVDFS volume at image offset zero.
// Format reference: https://multimedia.cx/xdvdfs.html
const SECTOR = 2048, MAX_DIRECTORY = 16 * 1024 * 1024;
const MAGIC = 'MICROSOFT*XBOX*MEDIA';
const text = new TextDecoder('windows-1252');
async function range(blob, offset, size) {
  if (!Number.isSafeInteger(offset) || !Number.isSafeInteger(size) || offset < 0 || size < 0 || offset > blob.size || size > blob.size - offset) throw new Error('XDVDFS range outside image');
  const b = await blob.slice(offset, offset + size).arrayBuffer();
  if (b.byteLength !== size) throw new Error('Truncated XDVDFS read');
  return b;
}
export async function openXiso(blob) {
  const bytes = new Uint8Array(await range(blob, 32 * SECTOR, SECTOR));
  if (text.decode(bytes.subarray(0, 20)) !== MAGIC || text.decode(bytes.subarray(SECTOR - 20)) !== MAGIC) throw new Error('Unsupported XDVDFS volume');
  const v = new DataView(bytes.buffer);
  const root = {offset: v.getUint32(20, true) * SECTOR, size: v.getUint32(24, true), directory: true};
  async function list(dir) {
    if (!dir || !dir.directory || !Number.isSafeInteger(dir.offset) || !Number.isSafeInteger(dir.size) || dir.offset < 0 || dir.size < 0 || dir.offset > blob.size || dir.size > blob.size-dir.offset || dir.size > MAX_DIRECTORY) throw new Error('Unsupported XDVDFS directory');
    if (!dir.size) return [];
    const data = new Uint8Array(await range(blob, dir.offset, dir.size));
    const view = new DataView(data.buffer), stack = [0], seen = new Set(), result = [], names = new Set(), extents=[];
    while (stack.length) {
      const p = stack.pop();
      if (seen.has(p) || p + 14 > data.length) throw new Error('Invalid XDVDFS directory tree');
      seen.add(p);
      const left = view.getUint16(p, true) * 4, right = view.getUint16(p + 2, true) * 4;
      const length = data[p + 13];
      if (!length || p + 14 + length > data.length) throw new Error('Invalid XDVDFS name');
      extents.push([p,p+14+length]);
      const name = text.decode(data.subarray(p + 14, p + 14 + length));
      if (/[\\/:\x00-\x1f]/.test(name) || name === '.' || name === '..') throw new Error('Unsafe XDVDFS name');
      const key = name.toLowerCase();
      if (names.has(key)) throw new Error('Duplicate XDVDFS name');
      names.add(key);
      const entry = {name, offset: view.getUint32(p + 4, true) * SECTOR, size: view.getUint32(p + 8, true), directory: !!(data[p + 12] & 0x10)};
      if (entry.offset > blob.size || entry.size > blob.size - entry.offset) throw new Error('XDVDFS entry outside image');
      result.push(entry);
      if (left) stack.push(left);
      if (right) stack.push(right);
    }
    extents.sort((a,b)=>a[0]-b[0]);
    for(let i=1;i<extents.length;i++) if(extents[i][0]<extents[i-1][1]) throw new Error('Overlapping XDVDFS directory entries');
    return result;
  }
  async function find(path) {
    const parts = path.split('/');
    if (!parts.length || parts.some(x => !x || x === '.' || x === '..' || x.includes('\\'))) throw new Error('Invalid XDVDFS path');
    let entry = root;
    for (const part of parts) {
      entry = (await list(entry)).find(x => x.name.toLowerCase() === part.toLowerCase());
      if (!entry) throw new Error('XDVDFS path not found');
    }
    return entry;
  }
  function file(entry) {
    if (!entry || entry.directory || !Number.isSafeInteger(entry.offset) || !Number.isSafeInteger(entry.size) || entry.offset < 0 || entry.size < 0 || entry.offset > blob.size || entry.size > blob.size - entry.offset) throw new Error('Invalid XDVDFS file');
    return blob.slice(entry.offset, entry.offset + entry.size);
  }
  return {root, list, find, file};
}
