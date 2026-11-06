// Pure fixed-step accumulator for the browser host loop. The caller supplies
// elapsed time in milliseconds; this module decides how many whole steps to
// run and how much time carries into the next frame. It does not read a clock,
// execute guest code, sample input or render, and it does not claim original
// console timing or cross-runtime determinism.
//
// Time is accumulated in units of 1/hz microseconds so a whole step is exactly
// 1,000,000 of those units regardless of whether 1e6 is divisible by hz. Input
// deltas are rounded to whole microseconds; sub-microsecond differences are
// discarded explicitly rather than drifting through a floating accumulator.
export const HOST_STEP_DEFAULT_HZ = 60;
export const HOST_STEP_DEFAULT_MAX_CATCH_UP = 5;
export const HOST_STEP_MAX_HZ = 1000;
export const HOST_STEP_MAX_CATCH_UP = 240;

const STEP_UNITS = 1000000;
const MICROSECONDS_PER_MILLISECOND = 1000;

export function createHostStep({
  hz = HOST_STEP_DEFAULT_HZ,
  maxCatchUp = HOST_STEP_DEFAULT_MAX_CATCH_UP,
} = {}) {
  if (!Number.isInteger(hz) || hz < 1 || hz > HOST_STEP_MAX_HZ)
    throw new Error(`hz must be an integer from 1 to ${HOST_STEP_MAX_HZ}`);
  if (!Number.isInteger(maxCatchUp) || maxCatchUp < 1 || maxCatchUp > HOST_STEP_MAX_CATCH_UP)
    throw new Error(`maxCatchUp must be an integer from 1 to ${HOST_STEP_MAX_CATCH_UP}`);

  let pending = 0;
  let totalSteps = 0;
  let droppedSteps = 0;
  let paused = false;

  const pausedResult = () => ({
    steps: 0,
    pendingMs: 0,
    droppedSteps: 0,
    paused: true,
  });

  function advance(deltaMs) {
    if (!Number.isFinite(deltaMs) || deltaMs < 0)
      throw new Error('deltaMs must be finite and non-negative');
    if (paused) return pausedResult();
    const deltaUs = Math.round(deltaMs * MICROSECONDS_PER_MILLISECOND);
    if (!Number.isSafeInteger(deltaUs)) throw new Error('deltaMs exceeds the safe range');
    const scaled = deltaUs * hz;
    if (!Number.isSafeInteger(pending + scaled)) throw new Error('accumulator exceeds the safe range');
    pending += scaled;

    let whole = Math.floor(pending / STEP_UNITS);
    pending -= whole * STEP_UNITS;
    let dropped = 0;
    if (whole > maxCatchUp) {
      dropped = whole - maxCatchUp;
      whole = maxCatchUp;
    }
    totalSteps += whole;
    droppedSteps += dropped;
    return {
      steps: whole,
      pendingMs: pending / (hz * MICROSECONDS_PER_MILLISECOND),
      droppedSteps: dropped,
      paused,
    };
  }

  return {
    advance,
    pause() { paused = true; pending = 0; },
    resume() { paused = false; pending = 0; },
    reset() { pending = 0; totalSteps = 0; droppedSteps = 0; paused = false; },
    describe() {
      return {
        kind: 'host-step',
        hz,
        stepMs: 1000 / hz,
        maxCatchUp,
        totalSteps,
        droppedSteps,
        paused,
        limitation: 'Host-loop pacing only; this is not original-console timing, input latency or determinism evidence.',
      };
    },
  };
}
