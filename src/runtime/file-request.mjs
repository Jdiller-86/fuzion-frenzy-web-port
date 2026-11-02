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

export function inspectCreateFileRequest(ram, args, checkAccess) {
  if (!Array.isArray(args) || args.length !== 9 || args.some(n => !Number.isInteger(n) || n < 0 || n > 0xffffffff)) {
    throw new Error('Invalid create-file diagnostic input');
  }
  const request = inspectOpenFileRequest(ram, [...args.slice(0, 4), args[6], args[8]], checkAccess);
  const allocationSizePointer = args[4];
  let allocationBytes;
  if (allocationSizePointer) {
    admittedRange(ram, checkAccess, allocationSizePointer, 8, 4, 1);
    allocationBytes = new DataView(ram.buffer, ram.byteOffset, ram.byteLength).getBigInt64(allocationSizePointer, true).toString();
  }
  return { ...request, allocationSizePointer, allocationBytes, fileAttributes:args[5], createDisposition:args[7] };
}

export function inspectWriteFileRequest(ram, args, checkAccess) {
  if (!(ram instanceof Uint8Array) || !Array.isArray(args) || args.length !== 8 ||
      args.some(n => !Number.isInteger(n) || n < 0 || n > 0xffffffff) || typeof checkAccess !== 'function') {
    throw new Error('Invalid write-file diagnostic input');
  }
  const [fileHandle,eventHandle,apcRoutine,apcContext,ioStatusBlockPointer,buffer,bytes,byteOffsetPointer]=args;
  if (bytes > 65536) throw new Error('Write-file payload exceeds diagnostic bounds');
  admittedRange(ram,checkAccess,ioStatusBlockPointer,8,4,2);
  if (bytes) admittedRange(ram,checkAccess,buffer,bytes,1,1);
  let byteOffset;
  if (byteOffsetPointer) {
    admittedRange(ram,checkAccess,byteOffsetPointer,8,4,1);
    byteOffset=new DataView(ram.buffer,ram.byteOffset,ram.byteLength).getBigInt64(byteOffsetPointer,true).toString();
  }
  return {fileHandle,eventHandle,apcRoutine,apcContext,ioStatusBlockPointer,buffer,bytes,byteOffsetPointer,byteOffset,
    payload:Array.from(ram.subarray(buffer,buffer+bytes)),serviceImplemented:false};
}

// Compare one bounded wide-format shape. This observes an already returned
// original result; it is never an implementation of the guest formatter.
export function compareWideMetadataFormat({format,arguments:args,output,returnedUnits,maximumUnits}={}) {
  if ([format,output].some(text=>typeof text!=='string' || text.includes('\0')) || format.length>=128 || output.length>=260 ||
      !Array.isArray(args) || args.length<1 || args.length>8 || args.some(arg=>!arg || (arg.kind==='character' ?
        !Number.isInteger(arg.value) || arg.value<0 || arg.value>65535 : arg.kind!=='string' || typeof arg.value!=='string' || arg.value.length>=128 || arg.value.includes('\0'))) ||
      !Number.isInteger(returnedUnits) || returnedUnits<0 || returnedUnits>=260 ||
      !Number.isInteger(maximumUnits) || maximumUnits<1 || maximumUnits>260) throw new Error('Invalid bounded wide-format observation');
  let expected='', argument=0;
  for(let i=0;i<format.length;i++) {
    if(format[i]!=='%') {expected+=format[i];continue;}
    if(format[++i]==='%') {expected+='%';continue;}
    const token=format.slice(i,i+2), arg=args[argument++]; i++;
    if(token==='lc' && arg?.kind==='character') expected+=String.fromCharCode(arg.value);
    else if(token==='ls' && arg?.kind==='string') expected+=arg.value;
    else throw new Error('Unsupported observed wide-format shape');
  }
  if(argument!==args.length) throw new Error('Unsupported observed wide-format arguments');
  const expectedPayload=[];
  for(let i=0;i<expected.length;i++) {const unit=expected.charCodeAt(i);expectedPayload.push(unit&255,unit>>>8);}
  return {matches:expected.length<maximumUnits && output===expected && returnedUnits===expected.length,
    expectedUnits:expected.length,expectedBytes:expectedPayload.length,expectedPayload};
}
