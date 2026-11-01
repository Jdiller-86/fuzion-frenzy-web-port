import { readXbeProfile } from '../xbe.mjs';
import { inspectProofWasm } from './wasm-proof.mjs';

// Curated build/ABI observations; no original instructions or file content.
export const BUILD_REFERENCE = {
  sha256: '45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9',
  entry: 0x30f7f, threadWrapper: 0x3439f, tlsSize: 0x3191b4,
  threadStart: 0x30f10, systemStart: 0x34307, kernelReturn: 0x343de, ordinal: 255
};
const RAM_BYTES = 64 * 1024 * 1024;
const required = ['abi','ram','ram_bytes','stack_top','source_digest','entry','state','trace','trace_count',
  'binding_count','binding_slot','binding_ordinal','reset','bind_kernels','run'];
const sha256 = async buffer => [...new Uint8Array(await crypto.subtle.digest('SHA-256', buffer))]
  .map(n => n.toString(16).padStart(2, '0')).join('');

export function parseLocalConsoleSettings(buffer) {
  if (!(buffer instanceof ArrayBuffer) || buffer.byteLength > 4096) throw new Error('Invalid local settings profile size');
  const profile = JSON.parse(new TextDecoder('utf-8', { fatal: true }).decode(buffer));
  if (profile?.kind !== 'local-console-settings' || profile.synthetic !== true ||
      !Number.isInteger(profile.miscFlags) || profile.miscFlags < 0 || profile.miscFlags > 0xffffffff) {
    throw new Error('Use an explicit synthetic misc-flags test profile');
  }
  return { kind: profile.kind, synthetic: true, miscFlags: profile.miscFlags };
}

export function proofTraceCapacity(api) {
  const capacity=typeof api.ff_probe_trace_capacity==='function' ? api.ff_probe_trace_capacity() : 64;
  if (![64,128].includes(capacity)) throw new Error('Unsupported bounded trace capacity');
  return capacity;
}
export function proofKernelCapacity(api) {
  const capacity=typeof api.ff_probe_kernel_capacity==='function'?api.ff_probe_kernel_capacity():32;
  if(![32,128].includes(capacity)) throw new Error('Unsupported bounded kernel-record capacity');
  return capacity;
}

export function parseLocalVolumeProfile(buffer) {
  parseLocalConsoleSettings(buffer);
  const volume = JSON.parse(new TextDecoder('utf-8', { fatal: true }).decode(buffer)).volume;
  const count = value => typeof value === 'string' && /^(0|[1-9][0-9]{0,18})$/.test(value) ? BigInt(value) : -1n;
  const total = count(volume?.totalAllocationUnits), available = count(volume?.availableAllocationUnits);
  const sectors = volume?.sectorsPerAllocationUnit, bytes = volume?.bytesPerSector;
  if (volume?.kind !== 'local-volume-profile' || volume.synthetic !== true || volume.device !== 'harddisk0-partition1' ||
      total <= 0n || total > 0x7fffffffffffffffn || available < 0n || available > total ||
      !Number.isInteger(sectors) || sectors < 1 || sectors > 128 || (sectors & (sectors - 1)) ||
      ![512,1024,2048,4096].includes(bytes) || total * BigInt(sectors) * BigInt(bytes) > 0xffffffffffffffffn) {
    throw new Error('Select explicit synthetic volume capacity and geometry; no original disk default is assumed');
  }
  return { kind: volume.kind, synthetic: true, device: volume.device, totalAllocationUnits: total.toString(),
    availableAllocationUnits: available.toString(), sectorsPerAllocationUnit: sectors, bytesPerSector: bytes };
}

export function parseLocalDirectoryProfile(buffer) {
  parseLocalVolumeProfile(buffer);
  const state=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).volume.directoryState;
  if(state!=='fresh') throw new Error('Select an explicit fresh synthetic directory state; no original save state is assumed');
  return {synthetic:true,state:'fresh',persistent:false};
}
export function parseLocalFileProfile(buffer) {
  parseLocalDirectoryProfile(buffer);
  const state=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(buffer)).volume.fileState;
  if(state!=='fresh') throw new Error('Select an explicit fresh synthetic file state; no original saved files are assumed');
  return {synthetic:true,state:'fresh',persistent:false,maxFiles:16,maxFileBytes:65536};
}

export async function loadLocalTitleProof({ moduleBytes, xbeBytes } = {}, abi, extraExports = [], { clock } = {}) {
  if (typeof WorkerGlobalScope === 'undefined' || !(self instanceof WorkerGlobalScope)) {
    throw new Error('Original title proof execution requires a browser worker');
  }
  if (!(xbeBytes instanceof ArrayBuffer) || xbeBytes.byteLength > 16 * 1024 * 1024) throw new Error('Invalid local executable size');
  const limits = inspectProofWasm(moduleBytes, { allowClockImport: !!clock }), sourceSha256 = await sha256(xbeBytes);
  if (sourceSha256 !== BUILD_REFERENCE.sha256) throw new Error('This proof requires the verified local game build');
  const profile = readXbeProfile(xbeBytes);
  if (profile.entry !== BUILD_REFERENCE.entry || !profile.tls || profile.nonKernelImportsAddress) throw new Error('Unsupported startup profile');
  const compiled = await WebAssembly.compile(moduleBytes);
  const imports = WebAssembly.Module.imports(compiled);
  if (imports.length !== (limits.clockImport ? 1 : 0) || imports.some(item =>
    item.module !== 'env' || item.name !== 'ff_host_clock' || item.kind !== 'function')) throw new Error('Host imports are not admitted');
  let admittedMemory;
  const api = (await WebAssembly.instantiate(compiled, limits.clockImport ? {
    env: { ff_host_clock: address => clock.write(admittedMemory?.buffer, address) }
  } : {})).exports;
  if (!(api.memory instanceof WebAssembly.Memory) || [...required, ...extraExports].some(name => typeof api['ff_probe_' + name] !== 'function')) {
    throw new Error('Unexpected local proof ABI');
  }
  admittedMemory = api.memory;
  if (typeof api._initialize === 'function') api._initialize();
  if (!(Array.isArray(abi) ? abi : [abi]).includes(api.ff_probe_abi()) || api.ff_probe_entry() !== profile.entry || api.ff_probe_ram_bytes() !== RAM_BYTES) {
    throw new Error('Module ABI/build does not match this proof');
  }
  const proofAbi = api.ff_probe_abi();
  if ([4,5,6,7,8,9,10].includes(proofAbi) !== !!limits.clockImport) throw new Error('Only clock proof ABIs use the required clock import');
  if ([5,6,7,8,9,10].includes(proofAbi) && ['vm_image','vm_map_image','vm_region_count','vm_region_word','vm_page_word','vm_access_count','vm_fault_word','vm_access_probe'].some(name =>
    typeof api['ff_probe_' + name] !== 'function')) throw new Error('Missing checked virtual-memory ABI');
  const memory = api.memory.buffer;
  if (memory.byteLength !== limits.minimumPages * 65536) throw new Error('Module memory differs from admission limits');
  const range = (address, size, alignment = 1) => {
    address >>>= 0;
    if (address % alignment || size > memory.byteLength || address > memory.byteLength - size) throw new Error('Proof ABI range outside memory');
    return address;
  };
  const declaredDigest = new TextDecoder().decode(new Uint8Array(memory, range(api.ff_probe_source_digest(), 64), 64));
  if (declaredDigest !== sourceSha256) throw new Error('Local module was prepared for a different executable');
  const ramAddress = range(api.ff_probe_ram(), RAM_BYTES, 4);
  const ram = new Uint8Array(memory, ramAddress, RAM_BYTES), view = new DataView(memory, ramAddress, RAM_BYTES);
  ram.fill(0); api.ff_probe_reset();
  if (profile.base + profile.headerBytes > RAM_BYTES) throw new Error('Headers exceed the proof RAM');
  ram.set(new Uint8Array(xbeBytes, 0, profile.headerBytes), profile.base);
  for (const section of profile.sections) {
    if (section.address + section.virtualBytes > RAM_BYTES) throw new Error('Section exceeds proof RAM');
    ram.set(new Uint8Array(xbeBytes, section.fileOffset, section.fileBytes), section.address);
  }
  if ([5,6,7,8,9,10].includes(proofAbi)) {
    if (api.ff_probe_vm_image(profile.base, profile.imageBytes) || api.ff_probe_vm_map_image(profile.base, profile.headerBytes, 2)) {
      throw new Error('Cannot admit image and private runtime ranges');
    }
    for (const section of profile.sections) {
      const protection = section.executable ? section.flags & 1 ? 0x40 : 0x20 : section.flags & 1 ? 4 : 2;
      if (api.ff_probe_vm_map_image(section.address, section.virtualBytes, protection)) throw new Error('Cannot admit section pages');
    }
  }
  const bindingCount = api.ff_probe_binding_count();
  if (!Number.isInteger(bindingCount) || bindingCount < 1 || bindingCount > 64) throw new Error('Invalid proof binding count');
  for (let i = 0; i < bindingCount; i++) {
    const slot = api.ff_probe_binding_slot(i), ordinal = api.ff_probe_binding_ordinal(i);
    if (!profile.imports.some(entry => entry.address === slot && entry.ordinal === ordinal) ||
        slot % 4 || slot > RAM_BYTES - 4 || view.getUint32(slot, true) !== (0x80000000 | ordinal) >>> 0) {
      throw new Error('Kernel-call binding does not match the original import declaration');
    }
  }
  api.ff_probe_bind_kernels();
  const stackTop = api.ff_probe_stack_top();
  if (stackTop !== RAM_BYTES - 4096) throw new Error('Unsupported proof stack');
  return { api, memory, range, view, ram, profile, sourceSha256, moduleSha256: await sha256(moduleBytes), stackTop };
}
