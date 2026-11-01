// Xbox 32-bit declarations: nxdk f8c52e4, xboxkrnl.h / xboxdef.h.
// Diagnostic only: no path resolution, host file access, guest writes or success result.
function admittedRange(ram, checkAccess, address, bytes, alignment, mode) {
  if (!Number.isInteger(address) || address <= 0 || address > 0xffffffff || address % alignment || bytes > ram.length ||
      address > ram.length - bytes || !checkAccess(address, bytes, mode)) throw new Error('File-request range is not admitted');
}
export function inspectCountedAnsiString(ram, address, checkAccess) {
  if (!(ram instanceof Uint8Array) || typeof checkAccess !== 'function') throw new Error('Invalid counted-string diagnostic input');
  admittedRange(ram, checkAccess, address, 8, 4, 1);
  const view = new DataView(ram.buffer, ram.byteOffset, ram.byteLength);
  const nameBytes = view.getUint16(address, true), nameMaximumBytes = view.getUint16(address + 2, true);
  const nameBuffer = view.getUint32(address + 4, true);
  if (nameBytes > nameMaximumBytes || nameBytes > 1024) throw new Error('File-request name exceeds diagnostic bounds');
  if (nameBytes) admittedRange(ram, checkAccess, nameBuffer, nameBytes, 1, 1);
  const bytes = nameBytes ? ram.subarray(nameBuffer, nameBuffer + nameBytes) : [];
  if (bytes.includes(0)) throw new Error('File-request name contains an embedded NUL');
  // Preserve byte values; do not interpret the name as UTF-8, a URL or a host path.
  return { nameDescriptorPointer:address, nameBuffer, nameBytes, nameMaximumBytes,
    name:Array.from(bytes, byte => String.fromCharCode(byte)).join('') };
}
export function inspectOpenFileRequest(ram, args, checkAccess) {
  if (!(ram instanceof Uint8Array) || !Array.isArray(args) || args.length !== 6 ||
      args.some(n => !Number.isInteger(n) || n < 0 || n > 0xffffffff) || typeof checkAccess !== 'function') {
    throw new Error('Invalid file-request diagnostic input');
  }
  const view = new DataView(ram.buffer, ram.byteOffset, ram.byteLength);
  const range = (address, bytes, alignment, mode) => admittedRange(ram, checkAccess, address, bytes, alignment, mode);
  const [fileHandlePointer, desiredAccess, objectAttributesPointer, ioStatusBlockPointer, shareAccess, openOptions] = args;
  range(fileHandlePointer, 4, 4, 2);
  range(ioStatusBlockPointer, 8, 4, 2);
  range(objectAttributesPointer, 12, 4, 1);
  const rootDirectory = view.getUint32(objectAttributesPointer, true);
  const nameDescriptorPointer = view.getUint32(objectAttributesPointer + 4, true);
  const attributes = view.getUint32(objectAttributesPointer + 8, true);
  const name = inspectCountedAnsiString(ram, nameDescriptorPointer, checkAccess);
  return { fileHandlePointer, desiredAccess, objectAttributesPointer, rootDirectory, attributes,
    ...name,
    ioStatusBlockPointer, shareAccess, openOptions, serviceImplemented: false };
}
