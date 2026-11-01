import { BUILD_REFERENCE as REF, loadLocalTitleProof, parseLocalConsoleSettings, parseLocalVolumeProfile } from './local-title-proof.mjs';
import { createProofClock } from './browser-clock.mjs';
import { runOriginalCopyProof } from './copy-proof.mjs';
import { inspectOpenFileRequest, inspectCountedAnsiString } from './file-request.mjs';

const fields = ['id','handle','handleOpen','state','object','fs','stackLow','stackTop','tls','tlsBytes',
  'routine','context','system','initialSp'];
self.onmessage = async event => {
  try {
    const clock = createProofClock();
    const { api, memory, range, view, ram, profile, sourceSha256, moduleSha256, stackTop } =
      await loadLocalTitleProof(event.data, [2, 3, 4, 5, 6], ['thread_count','parent_returned','kernel_count','kernel_word','thread_word'], { clock });
    const timerAbi = api.ff_probe_abi() >= 4, memoryAbi = api.ff_probe_abi() >= 5, volumeAbi = api.ff_probe_abi() === 6;
    if (timerAbi && ['timer_count','timer_word','dpc_count','dpc_delivered','poll','dpc_recover'].some(name =>
      typeof api['ff_probe_' + name] !== 'function')) throw new Error('Missing timer proof ABI');
    let consoleSettings, volumeProfile;
    if (api.ff_probe_abi() >= 3) {
      if (typeof api.ff_probe_misc_flags !== 'function') throw new Error('Missing local settings ABI');
      if (!event.data.settingsBytes) throw new Error('Select an explicit local test console-settings profile');
      consoleSettings = parseLocalConsoleSettings(event.data.settingsBytes);
      api.ff_probe_misc_flags(consoleSettings.miscFlags);
    } else if (event.data.settingsBytes) throw new Error('The earlier proof ABI does not consume a settings profile');
    if (volumeAbi) {
      if (['volume_profile','volume_word','volume_handle_word'].some(name => typeof api['ff_probe_' + name] !== 'function')) throw new Error('Missing local volume ABI');
      volumeProfile = parseLocalVolumeProfile(event.data.settingsBytes);
      const total = BigInt(volumeProfile.totalAllocationUnits), available = BigInt(volumeProfile.availableAllocationUnits);
      if (api.ff_probe_volume_profile(Number(total & 0xffffffffn), Number(total >> 32n), Number(available & 0xffffffffn),
          Number(available >> 32n), volumeProfile.sectorsPerAllocationUnit, volumeProfile.bytesPerSector)) throw new Error('Cannot admit explicit local volume profile');
    }
    if (profile.sections.some(s => s.address + s.virtualBytes > 0x03c00000)) throw new Error('Image overlaps boot-thread arena');
    let trapped = false;
    try { api.ff_probe_run(); } catch (error) {
      if (!(error instanceof WebAssembly.RuntimeError)) throw error;
      trapped = true;
    } finally { if (timerAbi) api.ff_probe_dpc_recover(); }
    const count = api.ff_probe_trace_count(), threadCount = api.ff_probe_thread_count(), callCount = api.ff_probe_kernel_count();
    if (count > 64 || threadCount > 4 || callCount > 32) throw new Error('Invalid boot-proof counts');
    const words = new Uint32Array(memory, range(api.ff_probe_trace(), count * 12, 4), count * 3);
    const hasCpuTrace = typeof api.ff_probe_trace_cpu === 'function';
    const trace = Array.from({ length: count }, (_, i) => ({ kind: words[i * 3] === 1 ? 'enter' : 'exit',
      address: words[i * 3 + 1], stack: words[i * 3 + 2], ...(hasCpuTrace ? { cpu: {
        eax: api.ff_probe_trace_cpu(i, 0) >>> 0, fs: api.ff_probe_trace_cpu(i, 1) >>> 0,
        exceptionHead: api.ff_probe_trace_cpu(i, 2) >>> 0 } } : {}) }));
    const state = Array.from(new Uint32Array(memory, range(api.ff_probe_state(), 64, 4), 16));
    const threads = Array.from({ length: threadCount }, (_, i) => Object.fromEntries(fields.map((key, n) => [key, api.ff_probe_thread_word(i, n) >>> 0])));
    const calls = Array.from({ length: callCount }, (_, i) => ({ ordinal: api.ff_probe_kernel_word(i, 0),
      status: api.ff_probe_kernel_word(i, 1) >>> 0,
      arguments: Array.from({ length: ({ 255: 10, 107: 3, 113: 2, 24: 5, 149: 4, 184: 5, 202: 6, 218: 5 })[api.ff_probe_kernel_word(i, 0)] ?? 1 }, (_, n) => api.ff_probe_kernel_word(i, n + 2) >>> 0) }));
    calls.forEach((call, i) => {
      if (call.ordinal === 24) call.output = { type: api.ff_probe_kernel_word(i, 7),
        value: api.ff_probe_kernel_word(i, 8) >>> 0, requiredBytes: api.ff_probe_kernel_word(i, 9) };
      if (call.ordinal === 149) call.output = { previousInsertion: api.ff_probe_kernel_word(i, 6),
        sampledInterruptTicks: ((BigInt(api.ff_probe_kernel_word(i, 8) >>> 0) << 32n) | BigInt(api.ff_probe_kernel_word(i, 7) >>> 0)).toString(),
        sampledSystemTicks: ((BigInt(api.ff_probe_kernel_word(i, 10) >>> 0) << 32n) | BigInt(api.ff_probe_kernel_word(i, 9) >>> 0)).toString() };
      if (call.ordinal === 184 && !call.status) call.output = { baseAddress: api.ff_probe_kernel_word(i, 7) >>> 0, regionBytes: api.ff_probe_kernel_word(i, 8) >>> 0 };
      if (call.ordinal === 202 && !call.status) call.output = { handle: api.ff_probe_kernel_word(i, 8) >>> 0,
        ioStatus: api.ff_probe_kernel_word(i, 9) >>> 0, information: api.ff_probe_kernel_word(i, 10) >>> 0 };
      if (call.ordinal === 218 && !call.status) call.output = { ioStatus: api.ff_probe_kernel_word(i, 7) >>> 0, information: api.ff_probe_kernel_word(i, 8) >>> 0,
        sizeWords:Array.from({length:6}, (_, n) => api.ff_probe_kernel_word(i, n + 9) >>> 0) };
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
    const reachedTimerArmed = timerAbi && calls[8]?.ordinal === 149 && calls[8].status === 0;
    const reachedNextHelper = reachedTimerArmed && trapped && state[0] === 2 && state[1] === 0x1df9c0 && state[5] === 0x32327;
    const copyReturned = trace.some(row => row.kind === 'exit' && row.address === 0x1df9c0);
    const bindingIndex = (state[1] - 0xfe000000) / 4;
    const reachedAllocation = reachedTimerArmed && copyReturned && trapped && state[0] === 1 && state[2] === 184 &&
      Number.isInteger(bindingIndex) && bindingIndex >= 0 && bindingIndex < api.ff_probe_binding_count() &&
      api.ff_probe_binding_ordinal(bindingIndex) === 184 && state[5] === 0x32530 &&
      state.slice(6, 11).every((n, i) => n === [62983944, 0, 62983988, 8192, 4][i]);
    const reachedHeapDependency = memoryAbi && reachedTimerArmed && copyReturned && trapped && state[0] === 2 &&
      state[1] === 0x31da0 && state[5] === 0x32696 && calls.length === 12;
    const reachedFileRequest = memoryAbi && reachedTimerArmed && copyReturned && trapped && state[0] === 1 && state[2] === 202 &&
      Number.isInteger(bindingIndex) && bindingIndex >= 0 && bindingIndex < api.ff_probe_binding_count() &&
      api.ff_probe_binding_ordinal(bindingIndex) === 202 && state[5] === 0x337ca && calls.length === 12 && entered.includes(0x33794);
    const reachedVolumeLifecycle = volumeAbi && reachedTimerArmed && copyReturned && trapped && state[0] === 1 && state[2] === 67 &&
      Number.isInteger(bindingIndex) && bindingIndex >= 0 && bindingIndex < api.ff_probe_binding_count() &&
      api.ff_probe_binding_ordinal(bindingIndex) === 67 && state[5] === 0x33d41 && calls.length === 15;
    const beyondThreadEntry = reachedThreadEntry || reachedTimerRequest || reachedTimerArmed;
    const checks = {
      realThreadCreation: calls.length === (reachedVolumeLifecycle ? 15 : reachedHeapDependency || reachedFileRequest ? 12 : reachedTimerArmed ? 9 : reachedTimerRequest ? 8 : reachedThreadEntry ? 4 : 2) && calls[0].ordinal === 255 && calls[0].status === 0 &&
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
      explicitNextUnsupportedStop: (trapped && state[0] === 2 && (state[1] === 0x34198 || reachedThreadEntry)) || reachedTimerRequest || reachedNextHelper || reachedAllocation || reachedHeapDependency || reachedFileRequest || reachedVolumeLifecycle
    };
    if (beyondThreadEntry) {
      checks.originalThreadStartEntered = entered.includes(0x34198) && entered.includes(REF.threadStart);
      checks.notificationCriticalSectionBalanced = calls[2]?.ordinal === 277 && calls[3]?.ordinal === 294 &&
        calls[2].status === 0 && calls[3].status === 0 && calls[2].arguments[0] === 0x2845b0 && calls[3].arguments[0] === 0x2845b0 &&
        view.getInt32(0x2845c0, true) === -1 && view.getUint32(0x2845c4, true) === 0 && view.getUint32(0x2845c8, true) === 0;
    }
    let objectInitialization, timerQueue;
    if (reachedTimerRequest || reachedTimerArmed) {
      const dpc = calls[4], timer = calls[5], setting = calls[6], conversion = calls[7];
      const valid = (address, size) => Number.isInteger(address) && address > 0 && address % 4 === 0 && address <= ram.length - size;
      const da = dpc?.arguments[0], ta = timer?.arguments[0];
      checks.originalCrtAndHelpersReached = [0x33ccc, 0x346a7, 0x309b2, 0x34720, 0x3467a].every(address => entered.includes(address));
      checks.deferredCallbackObjectInitialized = dpc?.ordinal === 107 && dpc.status === 0 && valid(da, 28) &&
        view.getUint16(da, true) === 0x13 && view.getUint8(da + 2) === 0 &&
        view.getUint32(da + 12, true) === dpc.arguments[1] && view.getUint32(da + 16, true) === dpc.arguments[2];
      checks.timerObjectInitialized = timer?.ordinal === 113 && timer.status === 0 && valid(ta, 40) &&
        view.getUint8(ta) === 8 + timer.arguments[1] && view.getUint8(ta + 2) === 10 && view.getUint8(ta + 3) === (reachedTimerArmed ? 1 : 0) &&
        view.getInt32(ta + 4, true) === 0 && view.getUint32(ta + 8, true) === ta + 8 && view.getUint32(ta + 12, true) === ta + 8 &&
        (reachedTimerArmed ? view.getBigUint64(ta + 16, true) > 0n : view.getBigUint64(ta + 16, true) === 0n &&
        view.getUint32(ta + 24, true) === 0 && view.getUint32(ta + 28, true) === 0) &&
        view.getInt32(ta + 36, true) === 0;
      checks.explicitSyntheticSettingRead = setting?.ordinal === 24 && setting.status === 0 && setting.arguments[0] === 17 &&
        setting.arguments[3] === 4 && setting.output.type === 4 && setting.output.requiredBytes === 4 && setting.output.value === consoleSettings.miscFlags;
      checks.exactSuccessStatusConversion = conversion?.ordinal === 301 && conversion.arguments[0] === 0 && conversion.status === 0;
      const arming = reachedTimerArmed ? calls[8] : undefined;
      const due = arming ? BigInt.asIntN(64, (BigInt(arming.arguments[2]) << 32n) | BigInt(arming.arguments[1]))
        : BigInt.asIntN(64, (BigInt(state[8]) << 32n) | BigInt(state[7]));
      checks.originalSixHourTimerRequest = due === -216000000000n && (arming
        ? arming.arguments[0] === ta && arming.arguments[3] === da : state[6] === ta && state[9] === da && state[5] === 0x34694);
      objectInitialization = { dpc: da, deferredRoutine: dpc?.arguments[1], deferredContext: dpc?.arguments[2], timer: ta,
        timerType: timer?.arguments[1], requestedDueTicks: due.toString(), requestedDelaySeconds: Number(-due) / 10000000,
        callbackStateWord: view.getUint32(0x3191a8, true) };
      if (arming) {
        const timerCount = api.ff_probe_timer_count();
        if (timerCount > 32 || api.ff_probe_dpc_count() > 32) throw new Error('Invalid timer queue count');
        timerQueue = { active: timerCount, pendingCallbacks: api.ff_probe_dpc_count(), deliveredCallbacks: api.ff_probe_dpc_delivered(),
          entries: Array.from({ length: timerCount }, (_, i) => ({ timer: api.ff_probe_timer_word(i, 0), dpc: api.ff_probe_timer_word(i, 1),
            absolute: api.ff_probe_timer_word(i, 2), deadlineTicks: ((BigInt(api.ff_probe_timer_word(i, 4) >>> 0) << 32n) |
              BigInt(api.ff_probe_timer_word(i, 3) >>> 0)).toString() })) };
        checks.realClockBackedTimerQueued = clock.describe().samples > 0 && arming.output.previousInsertion === 0 && timerCount === 1 && timerQueue.entries[0].timer === ta &&
          timerQueue.entries[0].dpc === da && timerQueue.entries[0].absolute === 0 &&
          BigInt(timerQueue.entries[0].deadlineTicks) === BigInt(arming.output.sampledInterruptTicks) - due &&
          view.getBigUint64(ta + 16, true).toString() === timerQueue.entries[0].deadlineTicks && view.getUint32(ta + 32, true) === da;
        checks.timerLinksAndNoEarlyCallback = view.getUint32(ta + 24, true) === 0x03e00000 && view.getUint32(ta + 28, true) === 0x03e00000 &&
          view.getUint32(0x03e00000, true) === ta + 24 && timerQueue.pendingCallbacks === 0 && timerQueue.deliveredCallbacks === 0 &&
          view.getUint8(thread.fs + 0x24) === 0 && view.getUint32(thread.fs + 0x58, true) === 0;
        checks.originalArmingWrapperReturned = trace.some(row => row.kind === 'exit' && row.address === 0x3467a) &&
          trace.some(row => row.kind === 'exit' && row.address === 0x346a7);
        checks.originalNextDependencyReached = (reachedNextHelper || reachedAllocation || reachedHeapDependency || reachedFileRequest || reachedVolumeLifecycle) && entered.includes(0x322ef);
      }
    }
    let allocationRequest, copyProof, virtualMemory, heapProof, openFileRequest, localVolume, symbolicLinkRequest;
    const admittedGuestRange = (address, bytes, access) => {
      for (let page = Math.floor(address / 4096); page <= Math.floor((address + bytes - 1) / 4096); ++page) {
        if (!api.ff_probe_vm_page_word(page * 4096, 1) || (api.ff_probe_vm_page_word(page * 4096, 2) & access) !== access) return false;
      }
      return true;
    };
    if (volumeAbi && trapped && state[0] === 1 && state[2] === 67) {
      symbolicLinkRequest = { link:inspectCountedAnsiString(ram,state[6],admittedGuestRange),
        device:inspectCountedAnsiString(ram,state[7],admittedGuestRange), serviceImplemented:false };
    }
    if (volumeAbi) {
      const total = (BigInt(api.ff_probe_volume_word(5) >>> 0) << 32n) | BigInt(api.ff_probe_volume_word(4) >>> 0);
      const available = (BigInt(api.ff_probe_volume_word(7) >>> 0) << 32n) | BigInt(api.ff_probe_volume_word(6) >>> 0);
      localVolume = { ready:api.ff_probe_volume_word(0), active:api.ff_probe_volume_word(1), opened:api.ff_probe_volume_word(2), closed:api.ff_probe_volume_word(3),
        totalAllocationUnits:total.toString(), availableAllocationUnits:available.toString(), sectorsPerAllocationUnit:api.ff_probe_volume_word(8),
        bytesPerSector:api.ff_probe_volume_word(9), handles:Array.from({length:16}, (_, i) => ({handle:api.ff_probe_volume_handle_word(i,0),
          access:api.ff_probe_volume_handle_word(i,1), share:api.ff_probe_volume_handle_word(i,2), options:api.ff_probe_volume_handle_word(i,3)})).filter(row => row.handle) };
      if (localVolume.active > 16 || localVolume.opened > 32 || localVolume.closed > localVolume.opened) throw new Error('Invalid local volume counters');
      const open = calls[12], query = calls[13], close = calls[14], handle = open?.output?.handle;
      const entry = trace.find(row => row.address === 0x33794 && row.kind === 'enter');
      const returned = trace.find(row => row.address === 0x33794 && row.kind === 'exit');
      const expectedSize = [Number(total & 0xffffffffn), Number(total >> 32n), Number(available & 0xffffffffn),
        Number(available >> 32n), volumeProfile.sectorsPerAllocationUnit, volumeProfile.bytesPerSector];
      checks.explicitSyntheticVolumeProfileUsed = localVolume.ready === 1 && localVolume.totalAllocationUnits === volumeProfile.totalAllocationUnits &&
        localVolume.availableAllocationUnits === volumeProfile.availableAllocationUnits && localVolume.sectorsPerAllocationUnit === volumeProfile.sectorsPerAllocationUnit &&
        localVolume.bytesPerSector === volumeProfile.bytesPerSector;
      checks.realVolumeRootOpened = open?.ordinal === 202 && open.status === 0 && open.output.ioStatus === 0 && open.output.information === 1 &&
        handle >= 0x10000 && handle < 0x20000 && !(handle & 3) && handle !== thread.handle &&
        open.arguments[1] === 0x100001 && open.arguments[4] === 3 && open.arguments[5] === 0x800021;
      checks.realVolumeSizeQueryCompleted = query?.ordinal === 218 && query.status === 0 && query.arguments[0] === handle &&
        query.arguments[1] === open.arguments[3] && query.arguments[3] === 24 && query.arguments[4] === 3 &&
        query.output.ioStatus === 0 && query.output.information === 24 && query.output.sizeWords.every((n,i) => n === expectedSize[i]);
      checks.volumeRootHandleClosed = close?.ordinal === 187 && close.status === 0 && close.arguments[0] === handle &&
        localVolume.opened === 1 && localVolume.closed === 1 && localVolume.active === 0 && localVolume.handles.length === 0;
      checks.originalVolumeInitializerReturned = !!entry?.cpu && !!returned?.cpu && entry.stack === returned.stack &&
        entry.cpu.fs === thread.fs && returned.cpu.fs === entry.cpu.fs && returned.cpu.exceptionHead === entry.cpu.exceptionHead;
      checks.nextOriginalDriveLinkReached = reachedVolumeLifecycle && symbolicLinkRequest?.link.name === '\\??\\D:' &&
        symbolicLinkRequest.device.name === '\\Device\\CdRom0' && symbolicLinkRequest.serviceImplemented === false;
    }
    if (memoryAbi) {
      const regionCount = api.ff_probe_vm_region_count();
      if (regionCount > 128) throw new Error('Invalid virtual-memory region count');
      virtualMemory = { checkedAccesses: api.ff_probe_vm_access_count(), fault: Array.from({length:4}, (_, i) => api.ff_probe_vm_fault_word(i)),
        regions: Array.from({length:regionCount}, (_, i) => ({base:api.ff_probe_vm_region_word(i,0), bytes:api.ff_probe_vm_region_word(i,1),
          kind:api.ff_probe_vm_region_word(i,2), protection:api.ff_probe_vm_region_word(i,3)})) };
      for (const region of virtualMemory.regions) {
        if (region.base % 4096 || region.bytes % 4096 || region.base > ram.length - region.bytes) throw new Error('Invalid virtual-memory region');
        if (region.kind === 2) region.pages = Array.from({length:region.bytes / 4096}, (_, i) => ({
          committed:api.ff_probe_vm_page_word(region.base + i*4096,1), permission:api.ff_probe_vm_page_word(region.base + i*4096,2)}));
      }
      const reserve = calls[9], commit = calls[10], critical = calls[11], base = reserve?.output?.baseAddress;
      const region = virtualMemory.regions.find(row => row.kind === 2 && row.base === base);
      checks.realGuestReservation = reserve?.ordinal === 184 && reserve.status === 0 && reserve.arguments[1] === 0 &&
        reserve.arguments[3] === 0x2000 && reserve.arguments[4] === 4 && base % 65536 === 0 &&
        base >= profile.base + profile.imageBytes && base + 1048576 <= 0x03c00000 && reserve.output.regionBytes === 1048576 &&
        region?.bytes === 1048576 && virtualMemory.regions.filter(row => row.kind === 2).length === 1;
      checks.onlyRequestedPageCommitted = !!region && commit?.ordinal === 184 && commit.status === 0 && commit.arguments[1] === 0 &&
        commit.arguments[3] === 0x1000 && commit.arguments[4] === 4 && commit.output.baseAddress === base && commit.output.regionBytes === 4096 &&
        region.pages[0].committed === 1 && region.pages[0].permission === 3 && region.pages.slice(1).every(p => !p.committed && !p.permission);
      const ca = critical?.arguments[0];
      checks.heapCriticalSectionInitialized = !!region && critical?.ordinal === 291 && critical.status === 0 && ca === base + 0x610 &&
        view.getUint8(ca) === 1 && view.getUint8(ca + 2) === 4 && view.getUint32(ca + 4, true) === 0 &&
        view.getUint32(ca + 8, true) === ca + 8 && view.getUint32(ca + 12, true) === ca + 8 &&
        view.getInt32(ca + 16, true) === -1 && view.getUint32(ca + 20, true) === 0 && view.getUint32(ca + 24, true) === 0;
      checks.checkedAccessesHadNoBootFault = virtualMemory.checkedAccesses > 0 && virtualMemory.fault.every(word => word === 0);
      if (reachedFileRequest || volumeAbi) {
        const heapEnter = trace.find(row => row.address === 0x322ef && row.kind === 'enter');
        const heapExit = trace.find(row => row.address === 0x322ef && row.kind === 'exit');
        heapProof = { entry: heapEnter, returned: heapExit };
        checks.originalHeapConstructorReturned = !!heapExit?.cpu && heapExit.cpu.eax === base;
        checks.originalHeapStackAndSehRestored = !!heapEnter?.cpu && !!heapExit?.cpu && heapEnter.cpu.fs === thread.fs &&
          heapExit.cpu.fs === heapEnter.cpu.fs && heapExit.cpu.exceptionHead === heapEnter.cpu.exceptionHead && heapExit.stack === heapEnter.stack;
        checks.originalHeapSupportReturned = [0x31da0, 0x3139a, 0x31259, 0x3185d, 0x1e0505].every(address =>
          trace.some(row => row.kind === 'exit' && row.address === address));
        if (reachedFileRequest) openFileRequest = inspectOpenFileRequest(ram, state.slice(6, 12), admittedGuestRange);
        if (reachedFileRequest) checks.boundedFileRequestDecoded = true;
      }
    }
    if (reachedAllocation) {
      for (const pointer of [state[6], state[8]]) {
        if (pointer % 4 || pointer > ram.length - 4) throw new Error('Invalid original allocation output pointer');
      }
      allocationRequest = { baseAddressPointer: state[6], baseAddress: view.getUint32(state[6], true), zeroBits: state[7],
        regionSizePointer: state[8], regionBytes: view.getUint32(state[8], true), allocationType: state[9], protection: state[10] };
    }
    if (entered.includes(0x1df9c0)) {
      copyProof = runOriginalCopyProof(api, 0x1df9c0);
      checks.originalCopyReturned = copyReturned;
      checks.originalCopyMemoryAndAbi = copyProof.passed && api.ff_probe_trace_count() === count && api.ff_probe_kernel_count() === callCount;
    }
    if (memoryAbi && checks.realGuestReservation && checks.onlyRequestedPageCommitted) {
      const denied = (address, access) => {
        let trapped = false;
        try { api.ff_probe_vm_access_probe(address, access); } catch (error) {
          if (!(error instanceof WebAssembly.RuntimeError)) throw error;
          trapped = true;
        }
        const fault = Array.from({length:4}, (_, i) => api.ff_probe_vm_fault_word(i));
        return trapped && fault.every((word,i) => word === [address,1,0,access][i]) &&
          new Uint32Array(memory, range(api.ff_probe_state(),64,4),16)[0] === 10;
      };
      virtualMemory.denialDiagnostics = { reservedRead:denied(calls[9].output.baseAddress + 4096,1), readOnlyImageWrite:denied(profile.base,2) };
      checks.reservedAndReadOnlyAccessDenied = Object.values(virtualMemory.denialDiagnostics).every(Boolean);
    }
    self.postMessage({ kind: 'thread-startup-boundary', phase: reachedVolumeLifecycle ? 'original-volume-drive-stop' : reachedFileRequest ? 'original-heap-file-stop' : reachedHeapDependency ? 'heap-first-page-next-stop' : reachedAllocation ? 'original-copy-allocation-stop' : reachedTimerArmed ? 'timer-armed-next-stop' : reachedTimerRequest ? 'crt-timer-request' : reachedThreadEntry ? 'original-thread-entry' : 'original-child-tls',
      verified: Object.values(checks).every(Boolean), checks,
      sourceSha256, moduleSha256, consoleSettings, volumeProfile, localVolume, biosUsed: false, hostImports: timerAbi ? 1 : 0, clock: timerAbi ? clock.describe() : undefined, originalFunctionsReached: entered,
      trace, calls, threads, objectInitialization, timerQueue, allocationRequest, copyProof, virtualMemory, heapProof, openFileRequest, symbolicLinkRequest, tlsSelfReference: view.getUint32(thread.tls, true),
      stop: { status: state[0], target: state[1], ordinal: state[2], stack: state[3], returnAddress: state[5], stackArguments: state.slice(6, 16) },
      gameBooted: false, memoryBytes: memory.byteLength,
      limitation: 'Boot-thread dispatch only; no preemption, waits, complete kernel, renderer or playable game. Unsupported original calls stop.' });
  } catch (error) { self.postMessage({ kind: 'error', message: String(error) }); }
};
