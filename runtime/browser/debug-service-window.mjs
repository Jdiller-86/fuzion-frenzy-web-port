// Explicit finite admission for the checked debug-length observer. No payloads.
export function parseDebugServiceStart(value) {
  if (value === undefined) return undefined;
  if (value !== 0 && value !== 8) throw new Error('Select debug service start 0 or 8');
  return value;
}

export function selectDebugServiceWindow(api, start) {
  parseDebugServiceStart(start);
  if (start === undefined || typeof api.ff_probe_debug_service_profile !== 'function' ||
      typeof api.ff_probe_debug_service_word !== 'function' ||
      typeof api.ff_probe_debug_service_window_word !== 'function')
    throw new Error('Missing selected debug service window capability');
  if (api.ff_probe_debug_service_profile(start) !== 0)
    throw new Error('Module refused selected debug service window');
}

const fields = ['requestedStart', 'capacity', 'limit', 'prefixCompleted', 'capturedCompleted', 'totalCompleted'];
export function validateDebugServiceWindow(window, start, completed) {
  parseDebugServiceStart(start);
  if (start === undefined || !window || Object.keys(window).length !== 8 ||
      window.kind !== 'bounded-debug-service-window' || window.version !== 1 ||
      fields.some(name => !Number.isInteger(window[name]) || window[name] < 0 || window[name] > 0xffffffff) ||
      window.requestedStart !== start || window.capacity !== 8 || window.limit !== start + 8 ||
      window.totalCompleted !== completed || completed > window.limit ||
      window.prefixCompleted !== Math.min(completed, start) ||
      window.capturedCompleted !== completed - window.prefixCompleted)
    throw new Error('Malformed or mismatched debug service window');
  return window;
}

export function readDebugServiceWindow(api, start, completed) {
  if (typeof api.ff_probe_debug_service_window_word !== 'function')
    throw new Error('Missing selected debug service window capability');
  const window = { kind: 'bounded-debug-service-window', version: 1,
    ...Object.fromEntries(fields.map((field, index) => [field, api.ff_probe_debug_service_window_word(index)])) };
  return validateDebugServiceWindow(window, start, completed);
}
