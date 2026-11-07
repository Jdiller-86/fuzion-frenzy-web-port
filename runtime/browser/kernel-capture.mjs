const MAX_ROWS = 512;
const hashWord = value => (value >>> 0).toString(16).padStart(8, '0');

export function validateKernelCapture(capture, calls) {
  if (!capture || capture.kind !== 'kernel-capture-window' || capture.version !== 1 ||
      ![0, MAX_ROWS].includes(capture.requestedStart) ||
      ![0, MAX_ROWS].includes(capture.capturedStart) ||
      !Number.isInteger(capture.capturedCount) || capture.capturedCount < 0 || capture.capturedCount > MAX_ROWS ||
      capture.capturedCount !== calls.length ||
      (capture.capturedStart !== 0 && capture.capturedCount === 0) ||
      capture.totalCount !== capture.capturedStart + capture.capturedCount ||
      capture.totalCount > capture.requestedStart + MAX_ROWS ||
      capture.capturedStart > capture.requestedStart ||
      capture.complete !== (capture.capturedStart === 0) ||
      !/^[0-9a-f]{64}$/.test(capture.sha256 ?? '') ||
      !Number.isInteger(capture.completedOperations) || capture.completedOperations < 0 ||
      capture.completedOperations > capture.totalCount ||
      !Number.isInteger(capture.prefixCompletedOperations) || capture.prefixCompletedOperations < 0 ||
      capture.prefixCompletedOperations > capture.capturedStart ||
      capture.completedOperations !== capture.prefixCompletedOperations + calls.filter(call => call.status === 0).length ||
      (capture.capturedStart === 0 ? capture.prefixCompletedOperations !== 0 || capture.prefixSha256 !== undefined :
        !/^[0-9a-f]{64}$/.test(capture.prefixSha256 ?? ''))) {
    throw new Error('Malformed bounded kernel capture window');
  }
  return capture;
}

export function readKernelCapture(api, calls) {
  if (api.ff_probe_abi() < 25) return undefined;
  const word = field => api.ff_probe_kernel_capture_word(field) >>> 0;
  if (word(0) !== 1) throw new Error('Unsupported kernel capture version');
  const capturedStart = word(2);
  const capture = {
    kind: 'kernel-capture-window', version: 1,
    requestedStart: word(1), capturedStart, capturedCount: calls.length,
    totalCount: word(3), completedOperations: word(4), prefixCompletedOperations: word(5),
    complete: capturedStart === 0,
    sha256: Array.from({ length: 8 }, (_, i) => hashWord(word(i + 8))).join(''),
  };
  if (capturedStart) capture.prefixSha256 = Array.from({ length: 8 }, (_, i) => hashWord(word(i + 16))).join('');
  return validateKernelCapture(capture, calls);
}

/* Historical proof checks use absolute call indexes. Omitted rows stay holes
 * internally, and the published report carries only the captured window. */
export function absoluteKernelCalls(capture, calls) {
  if (!capture?.capturedStart) return calls;
  const indexed = new Array(capture.totalCount);
  calls.forEach((call, i) => { indexed[capture.capturedStart + i] = call; });
  return indexed;
}
