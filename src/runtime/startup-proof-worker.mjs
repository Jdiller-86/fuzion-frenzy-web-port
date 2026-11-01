import { readXbeProfile } from '../xbe.mjs';
import { inspectProofWasm } from './wasm-proof.mjs';

// Curated addresses for the bounded startup proof, not original instructions.
const REFERENCE = {
  sha256: '45bb49e1685a5d86165b84c0f70250c19b80ecd89741f6d4541d685aa4eb00d9',
  entry: 0x30f7f, threadWrapper: 0x3439f, tlsSize: 0x3191b4,
  threadStart: 0x30f10, systemStart: 0x34307, kernelReturn: 0x343de, ordinal: 255
};
const RAM_BYTES = 64 * 1024 * 1024;
const exportsRequired = ['abi','ram','ram_bytes','stack_top','source_digest','entry','state','trace','trace_count',
  'binding_count','binding_slot','binding_ordinal','reset','bind_kernels','run'];
const sha256 = async buffer => [...new Uint8Array(await crypto.subtle.digest('SHA-256', buffer))]
  .map(n => n.toString(16).padStart(2, '0')).join('');

self.onmessage = async event => {
  try {
    const { moduleBytes, xbeBytes } = event.data ?? {};
    if (!(xbeBytes instanceof ArrayBuffer) || xbeBytes.byteLength > 16 * 1024 * 1024) throw new Error('Invalid local executable size');
    const limits = inspectProofWasm(moduleBytes);
    const sourceSha256 = await sha256(xbeBytes);
    if (sourceSha256 !== REFERENCE.sha256) throw new Error('This proof requires the verified local game build');
    const profile = readXbeProfile(xbeBytes);
    if (profile.entry !== REFERENCE.entry || !profile.tls || profile.nonKernelImportsAddress) throw new Error('Unsupported startup profile');
    const compiled = await WebAssembly.compile(moduleBytes);
    if (WebAssembly.Module.imports(compiled).length) throw new Error('Host imports are not admitted');
    const instance = await WebAssembly.instantiate(compiled, {}), api = instance.exports;
    if (!(api.memory instanceof WebAssembly.Memory) || exportsRequired.some(name => typeof api['ff_probe_' + name] !== 'function')) {
      throw new Error('Unexpected local proof ABI');
    }
    if (typeof api._initialize === 'function') api._initialize();
    if (api.ff_probe_abi() !== 1 || api.ff_probe_entry() !== profile.entry || api.ff_probe_ram_bytes() !== RAM_BYTES) {
      throw new Error('Module ABI/build does not match this proof');
    }
    const memory = api.memory.buffer;
    if (memory.byteLength !== limits.minimumPages * 65536) throw new Error('Module memory differs from admission limits');
    const range = (address, size, alignment = 1) => {
      address >>>= 0;
      if (address % alignment || size > memory.byteLength || address > memory.byteLength - size) throw new Error('Proof ABI range outside memory');
      return address;
    };
    const digestAddress = range(api.ff_probe_source_digest(), 64);
    const declaredDigest = new TextDecoder().decode(new Uint8Array(memory, digestAddress, 64));
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
    let trapped = false;
    try { api.ff_probe_run(); }
    catch (error) {
      if (!(error instanceof WebAssembly.RuntimeError)) throw error;
      trapped = true;
    }
    const state = Array.from(new Uint32Array(memory, range(api.ff_probe_state(), 64, 4), 16));
    const traceCount = api.ff_probe_trace_count();
    if (!Number.isInteger(traceCount) || traceCount > 16) throw new Error('Invalid proof trace count');
    const traceWords = new Uint32Array(memory, range(api.ff_probe_trace(), traceCount * 12, 4), traceCount * 3);
    const trace = Array.from({ length: traceCount }, (_, i) => ({
      kind: traceWords[i * 3] === 1 ? 'enter' : 'exit', address: traceWords[i * 3 + 1], stack: traceWords[i * 3 + 2]
    }));
    const tlsBytes = (((profile.tls.end - profile.tls.start + profile.tls.zeroFillBytes + 15) & 0xfffffff0) + 4) >>> 0;
    const tlsIndex = Math.trunc(tlsBytes / -4);
    const expectedArguments = [stackTop - 28, 0, profile.stackBytes, tlsBytes, 0, REFERENCE.threadStart, 0, 0, 0, REFERENCE.systemStart];
    const actualArguments = state.slice(6, 16);
    const entered = trace.filter(row => row.kind === 'enter');
    const checks = {
      explicitUnsupportedKernelStop: trapped && state[0] === 1 && state[2] === REFERENCE.ordinal,
      kernelReturnAddress: state[5] === REFERENCE.kernelReturn,
      kernelStackFrame: state[3] === stackTop - 84 && state[4] === 10 && actualArguments.every((n, i) => n === expectedArguments[i]),
      originalFunctionsReached: entered.length === 2 && entered[0].address === REFERENCE.entry && entered[0].stack === stackTop &&
        entered[1].address === REFERENCE.threadWrapper && entered[1].stack === stackTop - 36,
      tlsAllocation: view.getUint32(REFERENCE.tlsSize, true) === tlsBytes,
      tlsIndex: view.getInt32(profile.tls.index, true) === tlsIndex
    };
    self.postMessage({ kind: 'startup-boundary', verified: Object.values(checks).every(Boolean), checks,
      sourceSha256, moduleSha256: await sha256(moduleBytes), hostImports: 0, biosUsed: false,
      originalFunctionsReached: entered.map(row => row.address), trace,
      stop: { status: state[0], ordinal: state[2], target: state[1], stack: state[3], returnAddress: state[5], arguments: actualArguments },
      tls: { bytes: view.getUint32(REFERENCE.tlsSize, true), index: view.getInt32(profile.tls.index, true) },
      memoryBytes: memory.byteLength, gameBooted: false,
      limitation: 'Original startup only; thread creation is unsupported and deliberately stops. No gameplay, graphics or full-runtime acceptance.' });
  } catch (error) { self.postMessage({ kind: 'error', message: String(error) }); }
};
