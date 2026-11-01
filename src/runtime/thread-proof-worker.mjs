import { BUILD_REFERENCE as REF, loadLocalTitleProof, parseLocalConsoleSettings } from './local-title-proof.mjs';

const fields = ['id','handle','handleOpen','state','object','fs','stackLow','stackTop','tls','tlsBytes',
  'routine','context','system','initialSp'];
self.onmessage = async event => {
  try {
    const { api, memory, range, view, ram, profile, sourceSha256, moduleSha256, stackTop } =
      await loadLocalTitleProof(event.data, [2, 3], ['thread_count','parent_returned','kernel_count','kernel_word','thread_word']);
    let consoleSettings;
    if (api.ff_probe_abi() === 3) {
      if (typeof api.ff_probe_misc_flags !== 'function') throw new Error('Missing local settings ABI');
      if (!event.data.settingsBytes) throw new Error('Select an explicit local test console-settings profile');
      consoleSettings = parseLocalConsoleSettings(event.data.settingsBytes);
      api.ff_probe_misc_flags(consoleSettings.miscFlags);
    } else if (event.data.settingsBytes) throw new Error('The earlier proof ABI does not consume a settings profile');
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
      arguments: Array.from({ length: ({ 255: 10, 107: 3, 113: 2, 24: 5 })[api.ff_probe_kernel_word(i, 0)] ?? 1 }, (_, n) => api.ff_probe_kernel_word(i, n + 2) >>> 0) }));
    calls.forEach((call, i) => {
      if (call.ordinal === 24) call.output = { type: api.ff_probe_kernel_word(i, 7),
        value: api.ff_probe_kernel_word(i, 8) >>> 0, requiredBytes: api.ff_probe_kernel_word(i, 9) };
    });
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
    const reachedTimerRequest = trapped && state[0] === 1 && state[2] === 149 && !!consoleSettings;
    const beyondThreadEntry = reachedThreadEntry || reachedTimerRequest;
    const checks = {
      realThreadCreation: calls.length === (reachedTimerRequest ? 8 : reachedThreadEntry ? 4 : 2) && calls[0].ordinal === 255 && calls[0].status === 0 &&
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
      explicitNextUnsupportedStop: (trapped && state[0] === 2 && (state[1] === 0x34198 || reachedThreadEntry)) || reachedTimerRequest
    };
    if (beyondThreadEntry) {
      checks.originalThreadStartEntered = entered.includes(0x34198) && entered.includes(REF.threadStart);
      checks.notificationCriticalSectionBalanced = calls[2]?.ordinal === 277 && calls[3]?.ordinal === 294 &&
        calls[2].status === 0 && calls[3].status === 0 && calls[2].arguments[0] === 0x2845b0 && calls[3].arguments[0] === 0x2845b0 &&
        view.getInt32(0x2845c0, true) === -1 && view.getUint32(0x2845c4, true) === 0 && view.getUint32(0x2845c8, true) === 0;
    }
    let objectInitialization;
    if (reachedTimerRequest) {
      const dpc = calls[4], timer = calls[5], setting = calls[6], conversion = calls[7];
      const valid = (address, size) => Number.isInteger(address) && address > 0 && address % 4 === 0 && address <= ram.length - size;
      const da = dpc?.arguments[0], ta = timer?.arguments[0];
      checks.originalCrtAndHelpersReached = [0x33ccc, 0x346a7, 0x309b2, 0x34720, 0x3467a].every(address => entered.includes(address));
      checks.deferredCallbackObjectInitialized = dpc?.ordinal === 107 && dpc.status === 0 && valid(da, 28) &&
        view.getUint16(da, true) === 0x13 && view.getUint8(da + 2) === 0 &&
        view.getUint32(da + 12, true) === dpc.arguments[1] && view.getUint32(da + 16, true) === dpc.arguments[2];
      checks.timerObjectInitialized = timer?.ordinal === 113 && timer.status === 0 && valid(ta, 40) &&
        view.getUint8(ta) === 8 + timer.arguments[1] && view.getUint8(ta + 2) === 10 && view.getUint8(ta + 3) === 0 &&
        view.getInt32(ta + 4, true) === 0 && view.getUint32(ta + 8, true) === ta + 8 && view.getUint32(ta + 12, true) === ta + 8 &&
        view.getBigUint64(ta + 16, true) === 0n && view.getUint32(ta + 24, true) === 0 && view.getUint32(ta + 28, true) === 0 &&
        view.getInt32(ta + 36, true) === 0;
      checks.explicitSyntheticSettingRead = setting?.ordinal === 24 && setting.status === 0 && setting.arguments[0] === 17 &&
        setting.arguments[3] === 4 && setting.output.type === 4 && setting.output.requiredBytes === 4 && setting.output.value === consoleSettings.miscFlags;
      checks.exactSuccessStatusConversion = conversion?.ordinal === 301 && conversion.arguments[0] === 0 && conversion.status === 0;
      const due = BigInt.asIntN(64, (BigInt(state[8]) << 32n) | BigInt(state[7]));
      checks.originalSixHourTimerRequest = state[6] === ta && state[9] === da && due === -216000000000n && state[5] === 0x34694;
      objectInitialization = { dpc: da, deferredRoutine: dpc?.arguments[1], deferredContext: dpc?.arguments[2], timer: ta,
        timerType: timer?.arguments[1], requestedDueTicks: due.toString(), requestedDelaySeconds: Number(-due) / 10000000 };
    }
    self.postMessage({ kind: 'thread-startup-boundary', phase: reachedTimerRequest ? 'crt-timer-request' : reachedThreadEntry ? 'original-thread-entry' : 'original-child-tls',
      verified: Object.values(checks).every(Boolean), checks,
      sourceSha256, moduleSha256, consoleSettings, biosUsed: false, hostImports: 0, originalFunctionsReached: entered,
      trace, calls, threads, objectInitialization, tlsSelfReference: view.getUint32(thread.tls, true),
      stop: { status: state[0], target: state[1], ordinal: state[2], stack: state[3], returnAddress: state[5], stackArguments: state.slice(6, 16) },
      gameBooted: false, memoryBytes: memory.byteLength,
      limitation: 'Boot-thread dispatch only; no preemption, waits, complete kernel, renderer or playable game. Unsupported original calls stop.' });
  } catch (error) { self.postMessage({ kind: 'error', message: String(error) }); }
};
