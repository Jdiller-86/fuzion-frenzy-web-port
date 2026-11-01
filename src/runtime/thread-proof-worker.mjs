import { BUILD_REFERENCE as REF, loadLocalTitleProof } from './local-title-proof.mjs';

const fields = ['id','handle','handleOpen','state','object','fs','stackLow','stackTop','tls','tlsBytes',
  'routine','context','system','initialSp'];
self.onmessage = async event => {
  try {
    const { api, memory, range, view, ram, profile, sourceSha256, moduleSha256, stackTop } =
      await loadLocalTitleProof(event.data, 2, ['thread_count','parent_returned','kernel_count','kernel_word','thread_word']);
    if (profile.sections.some(s => s.address + s.virtualBytes > 0x03c00000)) throw new Error('Image overlaps boot-thread arena');
    let trapped = false;
    try { api.ff_probe_run(); } catch (error) {
      if (!(error instanceof WebAssembly.RuntimeError)) throw error;
      trapped = true;
    }
    const count = api.ff_probe_trace_count(), threadCount = api.ff_probe_thread_count(), callCount = api.ff_probe_kernel_count();
    if (count > 64 || threadCount > 4 || callCount > 16) throw new Error('Invalid boot-proof counts');
    const words = new Uint32Array(memory, range(api.ff_probe_trace(), count * 12, 4), count * 3);
    const trace = Array.from({ length: count }, (_, i) => ({ kind: words[i * 3] === 1 ? 'enter' : 'exit',
      address: words[i * 3 + 1], stack: words[i * 3 + 2] }));
    const state = Array.from(new Uint32Array(memory, range(api.ff_probe_state(), 64, 4), 16));
    const threads = Array.from({ length: threadCount }, (_, i) => Object.fromEntries(fields.map((key, n) => [key, api.ff_probe_thread_word(i, n) >>> 0])));
    const calls = Array.from({ length: callCount }, (_, i) => ({ ordinal: api.ff_probe_kernel_word(i, 0),
      status: api.ff_probe_kernel_word(i, 1) >>> 0,
      arguments: Array.from({ length: api.ff_probe_kernel_word(i, 0) === 255 ? 10 : 1 }, (_, n) => api.ff_probe_kernel_word(i, n + 2) >>> 0) }));
    const thread = threads[0];
    if (!thread || thread.object < 0x03c00000 || thread.fs < thread.object || thread.fs + 64 > thread.stackLow ||
        thread.stackTop > 0x03e00000 || thread.stackLow >= thread.initialSp || thread.initialSp + 12 > thread.tls ||
        thread.tlsBytes > 65536 || thread.tls + thread.tlsBytes !== thread.stackTop) throw new Error('Invalid boot-thread ranges');
    const tlsBytes = (((profile.tls.end - profile.tls.start + profile.tls.zeroFillBytes + 15) & 0xfffffff0) + 4) >>> 0;
    const expectedCreate = [stackTop - 28, 0, profile.stackBytes, tlsBytes, 0, REF.threadStart, 0, 0, 0, REF.systemStart];
    const entered = trace.filter(row => row.kind === 'enter').map(row => row.address);
    const loaderExit = trace.findIndex(row => row.kind === 'exit' && row.address === REF.entry);
    const childEnter = trace.findIndex(row => row.kind === 'enter' && row.address === REF.systemStart);
    const reachedThreadEntry = state[0] === 2 && state[1] === 0x33ccc;
    const checks = {
      realThreadCreation: calls.length === (reachedThreadEntry ? 4 : 2) && calls[0].ordinal === 255 && calls[0].status === 0 &&
        calls[0].arguments.every((n, i) => n === expectedCreate[i]) && threadCount === 1,
      closedHandleKeepsRunnableThread: calls[1]?.ordinal === 187 && calls[1].status === 0 &&
        calls[1].arguments[0] === thread.handle && thread.handleOpen === 0 && thread.state === 3,
      loaderReturnedBeforeChild: api.ff_probe_parent_returned() === 1 && loaderExit >= 0 && childEnter > loaderExit,
      childSystemAndSehEntered: entered.includes(REF.systemStart) && entered.includes(0x1e04cc),
      fsAndThreadTlsAgree: view.getUint32(thread.fs + 0x28, true) === thread.object &&
        view.getUint32(thread.object + 0x28, true) === thread.tls && view.getUint32(thread.fs + 4, true) === thread.stackTop &&
        view.getUint32(profile.tls.index, true) === (-tlsBytes / 4) >>> 0 && thread.tlsBytes === tlsBytes,
      originalTlsSelfReference: view.getUint32(thread.tls, true) === thread.tls + 4 && !((thread.tls + 4) & 15) &&
        ram.slice(thread.tls + 4, thread.tls + 4 + profile.tls.zeroFillBytes).every(n => n === 0),
      twoArgumentSystemFrame: view.getUint32(thread.initialSp + 4, true) === REF.threadStart &&
        view.getUint32(thread.initialSp + 8, true) === 0 && thread.routine === REF.threadStart && thread.system === REF.systemStart,
      explicitNextUnsupportedStop: trapped && state[0] === 2 && (state[1] === 0x34198 || reachedThreadEntry)
    };
    if (reachedThreadEntry) {
      checks.originalThreadStartEntered = entered.includes(0x34198) && entered.includes(REF.threadStart);
      checks.notificationCriticalSectionBalanced = calls[2]?.ordinal === 277 && calls[3]?.ordinal === 294 &&
        calls[2].status === 0 && calls[3].status === 0 && calls[2].arguments[0] === 0x2845b0 && calls[3].arguments[0] === 0x2845b0 &&
        view.getInt32(0x2845c0, true) === -1 && view.getUint32(0x2845c4, true) === 0 && view.getUint32(0x2845c8, true) === 0;
    }
    self.postMessage({ kind: 'thread-startup-boundary', phase: reachedThreadEntry ? 'original-thread-entry' : 'original-child-tls',
      verified: Object.values(checks).every(Boolean), checks,
      sourceSha256, moduleSha256, biosUsed: false, hostImports: 0, originalFunctionsReached: entered,
      trace, calls, threads, tlsSelfReference: view.getUint32(thread.tls, true),
      stop: { status: state[0], target: state[1], ordinal: state[2], stack: state[3], returnAddress: state[5] },
      gameBooted: false, memoryBytes: memory.byteLength,
      limitation: 'Boot-thread dispatch only; no preemption, waits, complete kernel, renderer or playable game. Unsupported original calls stop.' });
  } catch (error) { self.postMessage({ kind: 'error', message: String(error) }); }
};
