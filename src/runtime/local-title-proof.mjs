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

export async function loadLocalTitleProof({ moduleBytes, xbeBytes } = {}, abi, extraExports = []) {
  if (typeof WorkerGlobalScope === 'undefined' || !(self instanceof WorkerGlobalScope)) {
    throw new Error('Original title proof execution requires a browser worker');
  }
  if (!(xbeBytes instanceof ArrayBuffer) || xbeBytes.byteLength > 16 * 1024 * 1024) throw new Error('Invalid local executable size');
  const limits = inspectProofWasm(moduleBytes), sourceSha256 = await sha256(xbeBytes);
  if (sourceSha256 !== BUILD_REFERENCE.sha256) throw new Error('This proof requires the verified local game build');
  const profile = readXbeProfile(xbeBytes);
  if (profile.entry !== BUILD_REFERENCE.entry || !profile.tls || profile.nonKernelImportsAddress) throw new Error('Unsupported startup profile');
  const compiled = await WebAssembly.compile(moduleBytes);
  if (WebAssembly.Module.imports(compiled).length) throw new Error('Host imports are not admitted');
  const api = (await WebAssembly.instantiate(compiled, {})).exports;
  if (!(api.memory instanceof WebAssembly.Memory) || [...required, ...extraExports].some(name => typeof api['ff_probe_' + name] !== 'function')) {
    throw new Error('Unexpected local proof ABI');
  }
  if (typeof api._initialize === 'function') api._initialize();
  if (api.ff_probe_abi() !== abi || api.ff_probe_entry() !== profile.entry || api.ff_probe_ram_bytes() !== RAM_BYTES) {
    throw new Error('Module ABI/build does not match this proof');
  }
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
