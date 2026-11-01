import { BUILD_REFERENCE as REFERENCE, loadLocalTitleProof } from './local-title-proof.mjs';

self.onmessage = async event => {
  try {
    const { api, memory, range, view, profile, sourceSha256, moduleSha256, stackTop } = await loadLocalTitleProof(event.data, 1);
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
      sourceSha256, moduleSha256, hostImports: 0, biosUsed: false,
      originalFunctionsReached: entered.map(row => row.address), trace,
      stop: { status: state[0], ordinal: state[2], target: state[1], stack: state[3], returnAddress: state[5], arguments: actualArguments },
      tls: { bytes: view.getUint32(REFERENCE.tlsSize, true), index: view.getInt32(profile.tls.index, true) },
      memoryBytes: memory.byteLength, gameBooted: false,
      limitation: 'Original startup only; thread creation is unsupported and deliberately stops. No gameplay, graphics or full-runtime acceptance.' });
  } catch (error) { self.postMessage({ kind: 'error', message: String(error) }); }
};
