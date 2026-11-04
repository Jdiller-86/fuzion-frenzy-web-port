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
export const STOP_STATUSES = new Set([1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15]);

const integer = (value) => Number.isInteger(value) && value >= 0;
const bounded = (value, limit) => integer(value) && value <= limit;

export function validateProofReport(report) {
  if (!report || typeof report !== 'object') throw new Error('Proof report must be an object');
  if (typeof report.kind !== 'string' || !report.kind) throw new Error('Proof report kind is required');
  if (report.kind === 'error') return report; /* worker-level failure, no evidence */
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
    if (typeof name !== 'string' || !/^(writeClearing|pmcZero|pcrtcVblanks:\d{1,2}|engineWait)$/.test(name))
      throw new Error('Unknown experimental input in report');
  }
  const device = report.deviceRegisters;
  if (device) {
    if (!bounded(device.entries?.length ?? -1, DEVICE_ENTRY_LIMIT) ||
        !bounded(device.log?.length ?? -1, DEVICE_EVENT_LIMIT) ||
        !bounded(device.count ?? -1, DEVICE_ENTRY_LIMIT) ||
        !bounded(device.events ?? -1, DEVICE_EVENT_LIMIT))
      throw new Error('Device report exceeds the bounded aperture capacity');
  }
  return report;
}
