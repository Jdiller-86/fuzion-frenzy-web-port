/**
 * Shape and bound validation for a thread-startup proof report.
 *
 * The browser page receives a report from the worker and presents it as
 * evidence. A truncated or malformed report must never be shown as a result, so
 * this module checks the fields the report contract relies on: the stop record,
 * the bounded trace and device log, the counted arrays, and the absence of
 * unknown experimental claims. It validates shape only; it never reinterprets a
 * value or upgrades a run's meaning.
 */
import { validateSuppliedDeviceProfile } from './device-report.mjs';

export const TRACE_LIMIT = 8192;
export const DEVICE_EVENT_LIMIT = 32768;
export const DEVICE_ENTRY_LIMIT = 8192;
/* Must stay in sync with FF_DEVICE_STATUS_WAITS; each declaration supplies at most
 * 64 vertical-blank events in the display experiment. */
export const DEVICE_STATUS_WAITS = 4;
export const STOP_STATUSES = new Set([1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15]);

const integer = (value) => Number.isInteger(value) && value >= 0;
const dword = (value) => integer(value) && value <= 0xffffffff;
const bounded = (value, limit) => integer(value) && value <= limit;

export function validateProofReport(report) {
  if (!report || typeof report !== 'object') throw new Error('Proof report must be an object');
  if (typeof report.kind !== 'string' || !report.kind) throw new Error('Proof report kind is required');
  if (report.kind === 'error') return report; /* worker-level failure, no evidence */
  if (report.kind === 'startup-boundary') return validateStartupBoundary(report);
  if (report.kind !== 'thread-startup-boundary') throw new Error('Unknown proof report kind');
  if (report.verified !== false) throw new Error('A startup report must not claim verification');
  if (typeof report.phase !== 'string' || !report.phase) throw new Error('Proof report phase is required');
  if (!Array.isArray(report.trace) || report.trace.length > TRACE_LIMIT)
    throw new Error('Proof report trace exceeds the bounded trace capacity');
  for (const entry of report.trace) {
    if (!entry || typeof entry.kind !== 'string' || !integer(entry.address) || !integer(entry.stack))
      throw new Error('Malformed trace entry');
  }
  /* The reached-routine list is derived from the bounded trace; a hand-written
   * report must not smuggle an unbounded or malformed list past the contract.
   * Repeated addresses are expected: this is an entry sequence, not a set. */
  if (report.originalFunctionsReached !== undefined) {
    if (!Array.isArray(report.originalFunctionsReached) ||
        report.originalFunctionsReached.length > TRACE_LIMIT ||
        report.originalFunctionsReached.some(address => !integer(address)))
      throw new Error('The reached original-routine list is malformed or exceeds its bound');
    if (JSON.stringify(report.originalFunctionsReached) !==
        JSON.stringify(report.trace.filter(entry => entry.kind === 'enter').map(entry => entry.address)))
      throw new Error('The reached original-routine list does not match the entry trace');
  }
  const stop = report.stop;
  if (!stop || !STOP_STATUSES.has(stop.status) || !integer(stop.target) || !integer(stop.ordinal))
    throw new Error('Proof report stop record is incomplete or out of range');
  /* An unsupported-service stop must name the reached service and state that it
   * is not modeled; no other stop may carry a service descriptor. */
  if (stop.status === 1) {
    if (!stop.service || typeof stop.service !== 'object' || stop.service.ordinal !== stop.ordinal ||
        !('name' in stop.service) || (stop.service.name !== null && typeof stop.service.name !== 'string') ||
        stop.service.modeled !== false)
      throw new Error('An unsupported-service stop must name the service and state that it is not modeled');
    if (stop.service.name === 'AvSendTVEncoderOption') {
      if (stop.service.callingConvention !== 'stdcall' || stop.service.argumentBytes !== 16 ||
          stop.service.returns !== 'void')
        throw new Error('The AvSendTVEncoderOption descriptor must carry the evidenced stdcall/16-byte/void ABI');
      if (stop.service.resultBits !== undefined || stop.service.option !== undefined ||
          stop.service.output !== undefined)
        throw new Error('Observed option/result facts must not be stored in the static service descriptor');
    } else if (stop.service.callingConvention !== undefined || stop.service.argumentBytes !== undefined ||
               stop.service.returns !== undefined || stop.service.resultBits !== undefined ||
               stop.service.option !== undefined || stop.service.output !== undefined) {
      throw new Error('Only a known service may carry static ABI facts');
    }
  } else if (stop.service !== undefined) {
    throw new Error('Only an unsupported-service stop may carry a service descriptor');
  }
  const startup = report.originalStartup;
  if (!startup || !bounded(startup.totalInitializers, 4096) ||
      !bounded(startup.completedInitializers, startup.totalInitializers) ||
      !bounded(startup.distinctOriginalFunctions, 512) ||
      !bounded(startup.completedKernelOperations, 8192))
    throw new Error('Proof report startup counters are out of range');
  if (startup.completedInitializers !== startup.totalInitializers) {
    /* Partial progress is allowed but must be described, never presented as done. */
    if (typeof startup.stopReason !== 'string' || !startup.stopReason)
      throw new Error('An incomplete initializer run must state its stop reason');
  }
  /* The producer derives the distinct count from the reached-entry sequence.
   * When both are present they must agree; otherwise a report could inflate the
   * progress metric the comparison relies on without the corresponding list. */
  if (report.originalFunctionsReached !== undefined &&
      new Set(report.originalFunctionsReached).size !== startup.distinctOriginalFunctions)
    throw new Error('The reached original-routine list does not match the distinct function count');
  if (report.experiments !== undefined && !Array.isArray(report.experiments))
    throw new Error('Proof report experiments must be a list');
  for (const name of report.experiments ?? []) {
    if (typeof name !== 'string' || !/^(writeClearing|pmcZero|pcrtcVblanks:\d{1,2}|engineWait|pfifoReset|pfifoConsumer|displayMode|avUnknownOption|avSavedData|avNullResult|mmFreeContiguous|guestWatch|siteWatch|avQuery)$/.test(name))
      throw new Error('Unknown experimental input in report');
  }
  const device = report.deviceRegisters;
  if (device) {
    if (!bounded(device.entries?.length ?? -1, DEVICE_ENTRY_LIMIT) ||
        !bounded(device.log?.length ?? -1, DEVICE_EVENT_LIMIT) ||
        !bounded(device.count ?? -1, DEVICE_ENTRY_LIMIT) ||
        !bounded(device.events ?? -1, DEVICE_EVENT_LIMIT))
      throw new Error('Device report exceeds the bounded aperture capacity');
    /* Declared status inputs: address, polled mask, last masked value, read count. */
    if (device.statusWaits !== undefined) {
      if (!Array.isArray(device.statusWaits) || device.statusWaits.length > DEVICE_STATUS_WAITS)
        throw new Error('Device report exceeds the bounded status-wait capacity');
      for (const wait of device.statusWaits) {
        if (!wait || !integer(wait.address) || !integer(wait.mask) || !wait.mask ||
            !integer(wait.value) || !integer(wait.count))
          throw new Error('Malformed declared status wait');
      }
    }
    /* The supplied input profile, when present, must be the same bounded
     * window/register/zero-range shape the worker parsed. */
    if (device.supplied !== undefined && device.supplied !== null)
      validateSuppliedDeviceProfile(device.supplied);
  }
  /* Display-interrupt state, when the display path was exercised. */
  if (report.pcrtc !== undefined) {
    const pcrtc = report.pcrtc;
    if (!pcrtc || !integer(pcrtc.pending) || !integer(pcrtc.enabled) ||
        !bounded(pcrtc.events, DEVICE_STATUS_WAITS * 16))
      throw new Error('Malformed display-interrupt state');
  }
  /* The reset state and its experiment declaration are a pair: a report must not
   * show the proof without declaring the input, or claim the input without the
   * proof. */
  const experiments = report.experiments ?? [];
  const claimsReset = experiments.includes('pfifoReset');
  if (report.pfifoResetState !== undefined && !claimsReset)
    throw new Error('PFIFO reset state requires the pfifoReset experiment');
  if (claimsReset && report.pfifoResetState === undefined)
    throw new Error('The pfifoReset experiment requires PFIFO reset state');
  /* One explicit synthetic raw DWORD may bind the ordinal-2 query. It is a
   * labeled experiment, not a default or a claim about the original service. */
  const claimsAvQuery = experiments.includes('avQuery');
  if (report.avQueryWord !== undefined && !claimsAvQuery)
    throw new Error('The synthetic AV query word requires the avQuery experiment');
  if (claimsAvQuery && report.avQueryWord === undefined)
    throw new Error('The avQuery experiment requires an explicit raw word');
  /* The profile parser bounds the word to a raw DWORD; the report must apply the
   * same bound so a hand-written value cannot claim a wider domain. */
  if (report.avQueryWord !== undefined && !dword(report.avQueryWord))
    throw new Error('Malformed synthetic AV query word');
  /* PFIFO reference-reset state, when that experiment was selected. The report
   * must show one initialization, zero consumer updates and LOW_MARK on both
   * status registers; anything else defeats the proof the experiment exists for. */
  if (report.pfifoResetState !== undefined) {
    const pfifo = report.pfifoResetState;
    if (!pfifo || pfifo.scope !== 'seam' || !integer(pfifo.cache1) || !integer(pfifo.runout) || !integer(pfifo.dmaPush) ||
        !integer(pfifo.consumerUpdates) || !integer(pfifo.resets))
      throw new Error('Malformed PFIFO reset state');
    if (pfifo.resets !== 1) throw new Error('PFIFO reset must be applied exactly once');
    if (pfifo.consumerUpdates !== 0 && !experiments.includes('pfifoConsumer'))
      throw new Error('PFIFO reset must not make a consumer update');
    if (!(pfifo.cache1 & 0x10) || !(pfifo.runout & 0x10))
      throw new Error('PFIFO reset must set LOW_MARK on both status registers');
  }
  /* The consumer prototype and its experiment declaration are a pair. A refused
   * step must not advance GET; a completed one must consume at least one word
   * and update the seam twice (LOW_MARK clear then set). */
  const claimsConsumer = experiments.includes('pfifoConsumer');
  if (report.pfifoConsumerState !== undefined && !claimsConsumer)
    throw new Error('PFIFO consumer state requires the pfifoConsumer experiment');
  if (claimsConsumer && report.pfifoConsumerState === undefined)
    throw new Error('The pfifoConsumer experiment requires PFIFO consumer state');
  if (report.pfifoConsumerState !== undefined) {
    const consumer = report.pfifoConsumerState;
    if (!consumer || consumer.scope !== 'prototype' || !integer(consumer.classification) ||
        consumer.classification > 10 || !integer(consumer.get) || !integer(consumer.put) ||
        !integer(consumer.base) || !integer(consumer.end) || !integer(consumer.wordsConsumed) ||
        consumer.wordsConsumed > 4096 || !integer(consumer.consumerUpdates) ||
        !integer(consumer.lastHeader) || !integer(consumer.steps) || consumer.steps > 512 ||
        !integer(consumer.words) || consumer.words > 262144 || !integer(consumer.budgetHit) ||
        consumer.budgetHit > 1 || !integer(consumer.vmChecks) || consumer.vmChecks > 500000)
      throw new Error('Malformed PFIFO consumer state');
    if (consumer.classification === 0 &&
        (consumer.wordsConsumed < 1 || consumer.consumerUpdates < 2))
      throw new Error('A completed PFIFO pusher step must consume a word and update the seam');
    if (consumer.classification !== 0 && consumer.classification !== 1 && consumer.classification !== 8 &&
        consumer.wordsConsumed !== 0)
      throw new Error('A refused PFIFO pusher step must not consume a word');
    if (consumer.classification === 1 && consumer.wordsConsumed !== 0)
      throw new Error('An empty PFIFO pusher step must not consume a word');
    if (consumer.releaseAttempted !== undefined) {
      if (!integer(consumer.releaseAttempted) || consumer.releaseAttempted > 1 ||
          !integer(consumer.releaseFired) || consumer.releaseFired > 1 ||
          !integer(consumer.releaseReason) || consumer.releaseReason > 3 ||
          !integer(consumer.fenceVa) || !integer(consumer.releaseBefore) ||
          !integer(consumer.releaseWritten) || !integer(consumer.releaseSeq) ||
          !integer(consumer.drainedWords) || consumer.drainedWords > 4096)
        throw new Error('Malformed PFIFO consumer release state');
      if (consumer.releaseFired === 1 &&
          (consumer.releaseReason !== 0 || consumer.releaseWritten !== consumer.releaseSeq ||
           consumer.releaseBefore === consumer.releaseWritten))
        throw new Error('A fired PFIFO release must write the sequence over a behind value');
      if (consumer.releaseAttempted === 1 && consumer.releaseFired === 0 &&
          consumer.releaseReason === 0)
        throw new Error('A refused PFIFO release must report its reason');
    }    if (consumer.classification === 8 && consumer.budgetHit !== 1)
      throw new Error('A PFIFO consumer budget stop must report the budget hit');
  }
  /* Display-mode declaration and its experiment declaration are a pair; an
   * acceptance must carry the exact pinned tuple. */
  const claimsDisplay = experiments.includes('displayMode');
  if (report.displayModeState !== undefined && !claimsDisplay)
    throw new Error('Display-mode state requires the displayMode experiment');
  if (claimsDisplay && report.displayModeState === undefined)
    throw new Error('The displayMode experiment requires display-mode state');
  if (report.displayModeState !== undefined) {
    const d = report.displayModeState;
    if (!d || !integer(d.accepted) || d.accepted > 1 || !integer(d.addr) || !integer(d.step) ||
        !integer(d.mode) || !integer(d.format) || !integer(d.pitch) || !integer(d.fb) ||
        !integer(d.window) || !integer(d.decision) || d.decision > 3)
      throw new Error('Malformed display-mode state');
    if (d.accepted === 1 &&
        (d.addr !== 0x3C10CFC || d.step !== 0 || d.mode !== 0x60030307 || d.format !== 0 ||
         d.pitch !== 0xA00 || d.fb !== 0x03920000 || d.window !== 0x83920000 || d.decision !== 0))
      throw new Error('A display-mode acceptance must match the pinned declaration');
  }  /* Unknown-AV-option experiment and state pairing; acceptance requires the
   * exact observed tuple, one zero write, an untouched EAX and a 20-byte pop. */
  const claimsAvUnknown = experiments.includes('avUnknownOption');
  if (report.avUnknownOptionState !== undefined && !claimsAvUnknown)
    throw new Error('Unknown-AV-option state requires the avUnknownOption experiment');
  if (claimsAvUnknown && report.avUnknownOptionState === undefined)
    throw new Error('The avUnknownOption experiment requires its state');
  if (report.avUnknownOptionState !== undefined) {
    const u = report.avUnknownOptionState;
    if (!u || !integer(u.calls) || !integer(u.handle) || !integer(u.option) ||
        !integer(u.param) || !integer(u.result) || !integer(u.decision) ||
        !integer(u.written) || !integer(u.eaxBefore) || !integer(u.espBefore) ||
        !integer(u.eaxAfter) || !integer(u.espAfter))
      throw new Error('Malformed unknown-AV-option state');
    if (u.decision === 0 &&
        (u.handle !== 0xFD000000 || u.option !== 0x0F || u.param !== 0 || u.written !== 0 ||
         u.eaxAfter !== u.eaxBefore || u.espAfter !== ((u.espBefore + 20) >>> 0)))
      throw new Error('An accepted unknown AV option must match the pinned default');
  }  /* Saved-data-address experiment and state pairing; the pinned value is 0
   * with no arguments and no stack change. */
  const claimsAvSaved = experiments.includes('avSavedData');
  if (report.avSavedDataState !== undefined && !claimsAvSaved)
    throw new Error('Saved-data state requires the avSavedData experiment');
  if (claimsAvSaved && report.avSavedDataState === undefined)
    throw new Error('The avSavedData experiment requires its state');
  if (report.avSavedDataState !== undefined) {
    const s = report.avSavedDataState;
    if (!s || !integer(s.calls) || !integer(s.value) || !integer(s.decision) ||
        !integer(s.eaxBefore) || !integer(s.espBefore) || !integer(s.eaxAfter) ||
        !integer(s.espAfter) || !integer(s.priorSetter))
      throw new Error('Malformed saved-data state');
    if (s.decision === 0 &&
        (s.value !== 0 || s.eaxAfter !== 0 || s.espAfter !== s.espBefore || s.priorSetter !== 0))
      throw new Error('An accepted saved-data call must return the pinned 0 with no prior setter and no stack change');
  }

  /* Null-result AV query experiment and state pairing. The pinned early return
   * is unconditional on handle/option/param: any null Result is a no-op, while a
   * nonzero Result reaches the unmodeled option switch. The state keeps the last
   * accepted tuple; the two observed pairs stay pinned pair evidence. */
  const claimsAvNull = experiments.includes('avNullResult');
  const observedNullShapes = [[0x03920000, 9, 0], [0xFD000000, 9, 1]];
  const isObservedNullShape = (handle, option, param) =>
    observedNullShapes.some(([h, o, p]) => handle === h && option === o && param === p);
  if (report.avNullResultState !== undefined && !claimsAvNull)
    throw new Error('Null-result state requires the avNullResult experiment');
  if (claimsAvNull && report.avNullResultState === undefined)
    throw new Error('The avNullResult experiment requires its state');
  if (report.avNullResultState !== undefined) {
    const n = report.avNullResultState;
    if (!n || !integer(n.calls) || !integer(n.handle) || !integer(n.option) ||
        !integer(n.param) || !integer(n.result) || !integer(n.decision) ||
        !integer(n.writeBefore) || !integer(n.writeAfter) || !integer(n.checksumBefore) ||
        !integer(n.checksumAfter) || !integer(n.eaxBefore) || !integer(n.espBefore) ||
        !integer(n.eaxAfter) || !integer(n.espAfter))
      throw new Error('Malformed null-result state');
    if (n.decision === 0 &&
        (n.result !== 0 || !isObservedNullShape(n.handle, n.option, n.param) ||
         n.writeAfter !== n.writeBefore || n.checksumAfter !== n.checksumBefore ||
         n.eaxAfter !== n.eaxBefore || n.espAfter !== ((n.espBefore + 20) >>> 0)))
      throw new Error('An accepted null-result call must match the pinned no-op contract, an observed shape and the 20-byte pop');
  }

  /* Contiguous-free experiment and state pairing. The pinned bridge frees the
   * block and sets EAX 0; this harness owns no host allocator, so the accepted
   * effect is declaration-level only: no guest write, EAX 0, one-argument pop. */
  const claimsMmFree = experiments.includes('mmFreeContiguous');
  if (report.mmFreeContiguousState !== undefined && !claimsMmFree)
    throw new Error('Contiguous-free state requires the mmFreeContiguous experiment');
  if (claimsMmFree && report.mmFreeContiguousState === undefined)
    throw new Error('The mmFreeContiguous experiment requires its state');
  if (report.mmFreeContiguousState !== undefined) {
    const f = report.mmFreeContiguousState;
    if (!f || !integer(f.calls) || !integer(f.address) || !integer(f.decision) ||
        !integer(f.eaxBefore) || !integer(f.espBefore) || !integer(f.eaxAfter) ||
        !integer(f.espAfter) || !integer(f.writeBefore) || !integer(f.writeAfter))
      throw new Error('Malformed contiguous-free state');
    if (f.decision === 0 &&
        (!f.address || (f.address & 3) !== 0 || f.eaxAfter !== 0 ||
         f.espAfter !== ((f.espBefore + 8) >>> 0) || f.writeAfter !== f.writeBefore))
      throw new Error('An accepted contiguous-free call must match the pinned declaration-level effect');
  }

  /* Diagnostic guest-write watch. It is not a service model: it records the
   * first translated write site covering the selected device field. */
  const claimsWatch = experiments.includes('guestWatch');
  if (report.guestWatchState !== undefined && !claimsWatch)
    throw new Error('Guest-watch state requires the guestWatch experiment');
  if (claimsWatch && report.guestWatchState === undefined)
    throw new Error('The guestWatch experiment requires its state');
  if (report.guestWatchState !== undefined) {
    const g = report.guestWatchState;
    if (!g || !integer(g.address) || !integer(g.site) || !integer(g.count) ||
        !integer(g.readSite) || !integer(g.readCount))
      throw new Error('Malformed guest-watch state');
    if (!g.address)
      throw new Error('The guest-watch probe must target the selected device field');
  }

  /* Toolkit ABI-check diagnostic. It records the call target whose callee failed
   * to preserve ebx/esi/edi/esp; it is evidence, not a service model. */
  if (report.abiViolationState !== undefined) {
    const a = report.abiViolationState;
    if (!a || !integer(a.count) || !integer(a.first) || !integer(a.last) || !integer(a.esiCount) || !integer(a.esiFirst) || !integer(a.esiLast))
      throw new Error('Malformed ABI-violation state');
  }
  /* Diagnostic site watch: records the FUN_001FC010 entry pointer/ECX/return and
   * the +0x47C field address. Diagnostic only, never a service model. */
  const claimsSiteWatch = experiments.includes('siteWatch');
  if (report.siteWatchState !== undefined && !claimsSiteWatch)
    throw new Error('Site-watch state requires the siteWatch experiment');
  if (claimsSiteWatch && report.siteWatchState === undefined)
    throw new Error('The siteWatch experiment requires its state');
  if (report.siteWatchState !== undefined) {
    const s = report.siteWatchState;
    if (!s || !integer(s.entryAddr) || !integer(s.entryEcx) || !integer(s.entryReturn) ||
        !integer(s.fieldAddr) || !integer(s.fieldCount) || !Array.isArray(s.entries) || s.entries.length > 8 ||
        s.entries.some(e => !Array.isArray(e) || e.length !== 3 || e.some(v => !integer(v))))
      throw new Error('Malformed site-watch state');
  }
  /* Ordinal 2 remains unsupported by default: the observed call carries four
   * arguments and no output. Only the explicit synthetic avQuery word may make
   * the adapter complete the call, and the report must show that exact raw word. */
  let avQueryDenied = false;
  let sawFirstNullShape = false, sawSecondNullShape = false;
  if (report.calls !== undefined) {
    if (!Array.isArray(report.calls) || report.calls.length > 8192)
      throw new Error('Kernel call records must be a bounded list');
    for (const call of report.calls) {
      if (!call || typeof call !== 'object' || !integer(call.ordinal) || !integer(call.status) ||
          !Array.isArray(call.arguments) || call.arguments.length > 16 ||
          call.arguments.some(argument => !integer(argument)))
        throw new Error('Malformed kernel call record');
      if (call.ordinal !== 2) continue;
      if (call.arguments.length !== 4)
        throw new Error('An ordinal-2 call must carry four observed arguments');
      /* The unknown-option sibling has its own success shape; it is validated
       * by avUnknownOptionState, not by the startup avQuery adapter rules. */
      if (call.status === 0 && call.arguments[0] === 0xFD000000 &&
          call.arguments[1] === 0x0F && call.arguments[2] === 0) {
        if (!claimsAvUnknown)
          throw new Error('An unknown AV option call requires the avUnknownOption experiment');
        continue;
      }
      /* The null-result sibling is validated by avNullResultState; the two
       * observed pairs stay pinned and any other accepted tuple is flagged. */
      if (claimsAvNull && call.status === 0 && call.arguments[3] === 0) {
        if (!isObservedNullShape(call.arguments[0], call.arguments[1], call.arguments[2]))
          throw new Error('An accepted null-result call must use an observed shape');
        if (call.arguments[0] === 0x03920000) sawFirstNullShape = true;
        else sawSecondNullShape = true;
        continue;
      }
      if (report.avQueryWord === undefined) {
        if (call.status === 0)
          throw new Error('An unsupported ordinal-2 call must not report success');
        if (call.output !== undefined)
          throw new Error('Unsupported ordinal-2 calls must not carry an output word');
        /* The unconfigured record is the denial the unsupported stop names, so
         * it cannot carry an unrelated status. A non-denial record must not be
         * able to satisfy the ordinal-2 stop pairing below. */
        if (call.status !== 1)
          throw new Error('An unsupported ordinal-2 call must report the unsupported status');
        avQueryDenied = true;
      } else if (call.status === 0) {
        /* The adapter exists for the observed (0, 6, 0, Result) query, so a
         * successful call must carry exactly that shape and write through its
         * fourth argument. */
        if (call.arguments[0] !== 0 || call.arguments[1] !== 6 ||
            call.arguments[2] !== 0 || call.arguments[3] === 0)
          throw new Error('The synthetic avQuery call must use the observed (0, 6, 0, Result) query shape');
        /* The guest run must expose the adapter's VOID/EAX/stack effect, not just
         * the raw word, so the integration path is checked as well as the native
         * fixture. */
        if (!call.output || !dword(call.output.rawWord) || !dword(call.output.address) ||
            !dword(call.output.eaxBefore) || !dword(call.output.espBefore) ||
            !dword(call.output.eaxAfter) || !dword(call.output.espAfter))
          throw new Error('The synthetic avQuery call must carry its raw word, output address and CPU effect');
        if (call.output.rawWord !== report.avQueryWord)
          throw new Error('The synthetic avQuery output must match the declared raw word');
        if (call.output.address !== call.arguments[3])
          throw new Error('The synthetic avQuery output address must match the fourth call argument');
        if (call.output.eaxAfter !== call.output.eaxBefore)
          throw new Error('The synthetic avQuery call must leave EAX unchanged');
        if (call.output.espBefore > 0xffffffff - 20)
          throw new Error('The synthetic avQuery call must not report a wrapping stack frame');
        if (call.output.espAfter !== ((call.output.espBefore + 20) >>> 0))
          throw new Error('The synthetic avQuery call must pop the 20-byte stdcall frame');
      } else {
        /* A configured profile can still truthfully deny or reach the query
         * before it can be answered; a bridge-handled denial may carry a real
         * NTSTATUS, so only an output word or success is invalid here. */
        if (call.output !== undefined)
          throw new Error('A denied synthetic avQuery call must not carry an output word');
        avQueryDenied = true;
      }
    }
  }
  if (claimsAvNull && report.calls !== undefined &&
      (!sawFirstNullShape || !sawSecondNullShape))
    throw new Error('The avNullResult pair must reproduce both observed null-result shapes');
  /* An avQuery declaration may describe a prequery run in which ordinal 2 was
   * never reached; the report then has no ordinal-2 call and is still truthful.
   * That is only consistent when the run stopped before the query. A stop at
   * the unsupported ordinal-2 service must carry the denial record itself, with
   * or without the avQuery experiment; a completed call is not a denial and
   * cannot stand in for one. A denial, when present, is the unsupported-service
   * stop itself, even if an earlier ordinal-2 call completed. */
  if (stop.status === 1 && stop.ordinal === 2 && !avQueryDenied)
    throw new Error('An unsupported ordinal-2 stop requires the ordinal-2 denial record');
  if (avQueryDenied && (stop.status !== 1 || stop.ordinal !== 2))
    throw new Error('An ordinal-2 denial must stop at the unsupported service');
  return report;
}

/* The earlier single-thread startup report: its `verified` flag is meaningful, but
 * it may only be true when every individual check is true, and it must never
 * claim a booted game, a host import or a BIOS. */
const SHA256 = /^[0-9a-f]{64}$/;
export function validateStartupBoundary(report) {
  if (report.gameBooted !== false) throw new Error('A startup boundary must not claim a booted game');
  if (report.hostImports !== 0) throw new Error('A startup boundary must report no host imports');
  if (report.biosUsed !== false) throw new Error('A startup boundary must report no BIOS use');
  if (typeof report.sourceSha256 !== 'string' || !SHA256.test(report.sourceSha256) ||
      typeof report.moduleSha256 !== 'string' || !SHA256.test(report.moduleSha256))
    throw new Error('A startup boundary must carry both source and module digests');
  const checks = report.checks;
  if (!checks || typeof checks !== 'object' || !Object.keys(checks).length ||
      Object.values(checks).some(value => typeof value !== 'boolean'))
    throw new Error('A startup boundary must list boolean checks');
  const allPass = Object.values(checks).every(Boolean);
  if (report.verified !== allPass) throw new Error('A startup boundary verification must match its checks');
  if (!Array.isArray(report.trace) || report.trace.length > 16 ||
      report.trace.some(entry => !entry || typeof entry.kind !== 'string' ||
        !integer(entry.address) || !integer(entry.stack)))
    throw new Error('A startup boundary trace is malformed or exceeds its bound');
  if (!Array.isArray(report.originalFunctionsReached) || report.originalFunctionsReached.length > 16 ||
      report.originalFunctionsReached.some(address => !integer(address)))
    throw new Error('A startup boundary reached-routine list is malformed');
  const stop = report.stop;
  if (!stop || !STOP_STATUSES.has(stop.status) || !integer(stop.target) || !integer(stop.ordinal) ||
      !integer(stop.stack))
    throw new Error('A startup boundary stop record is incomplete or out of range');
  if (typeof report.limitation !== 'string' || !report.limitation)
    throw new Error('A startup boundary must state its limitation');
  return report;
}
