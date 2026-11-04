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
  if (report.experiments !== undefined && !Array.isArray(report.experiments))
    throw new Error('Proof report experiments must be a list');
  for (const name of report.experiments ?? []) {
    if (typeof name !== 'string' || !/^(writeClearing|pmcZero|pcrtcVblanks:\d{1,2}|engineWait|pfifoReset|avQuery)$/.test(name))
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
    if (pfifo.consumerUpdates !== 0) throw new Error('PFIFO reset must not make a consumer update');
    if (!(pfifo.cache1 & 0x10) || !(pfifo.runout & 0x10))
      throw new Error('PFIFO reset must set LOW_MARK on both status registers');
  }
  /* Ordinal 2 remains unsupported by default: the observed call carries four
   * arguments and no output. Only the explicit synthetic avQuery word may make
   * the adapter complete the call, and the report must show that exact raw word. */
  let avQueryCall = false;
  let avQueryAttempt = false;
  let avQueryDenied = false;
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
      if (report.avQueryWord === undefined) {
        if (call.status === 0)
          throw new Error('An unsupported ordinal-2 call must not report success');
        if (call.output !== undefined)
          throw new Error('Unsupported ordinal-2 calls must not carry an output word');
        if (call.status === 1) avQueryDenied = true;
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
        avQueryCall = true;
      } else {
        /* A configured profile can still truthfully deny or reach the query
         * before it can be answered; preserve that report instead of presenting
         * a denial as success. */
        if (call.status !== 1)
          throw new Error('A denied synthetic avQuery call must report the unsupported status');
        if (call.output !== undefined)
          throw new Error('A denied synthetic avQuery call must not carry an output word');
        avQueryAttempt = true;
        avQueryDenied = true;
      }
    }
  }
  /* The declaration is a claim about a recorded guest call, so it must hold
   * even when the report omits the call list entirely. A truthful denial counts
   * as the recorded attempt; a success must satisfy the exact shape above. */
  if (claimsAvQuery && !avQueryCall && !avQueryAttempt)
    throw new Error('The avQuery experiment requires a recorded ordinal-2 call');
  /* An ordinal-2 denial is the unsupported-service stop itself. A report that
   * records the denial but claims a later stop is not a truthful observation. */
  if (avQueryDenied && !avQueryCall && (stop.status !== 1 || stop.ordinal !== 2))
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
