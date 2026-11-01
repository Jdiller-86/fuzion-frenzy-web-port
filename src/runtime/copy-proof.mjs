// Owned diagnostic inputs. The selected original routine performs every copy;
// its private Wasm host independently checks memory, guards and the guest ABI.
export function runOriginalCopyProof(api, target) {
  if (typeof api.ff_probe_copy_case !== 'function' || typeof api.ff_probe_copy_recover !== 'function') {
    throw new Error('Missing original-copy diagnostic ABI');
  }
  let cases = 0;
  const categories = {}, failures = [];
  const run = (category, destination, source, bytes, expected = 0, entry = target) => {
    let result;
    try { result = api.ff_probe_copy_case(entry, destination, source, bytes, cases) >>> 0; }
    finally { api.ff_probe_copy_recover(); }
    ++cases; categories[category] = (categories[category] ?? 0) + 1;
    if (result !== expected && failures.length < 8) failures.push({ category, destination, source, bytes, result, expected });
  };
  // Every short length and both alignments, in either address order.
  for (let bytes = 0; bytes <= 128; ++bytes) for (let d = 0; d < 4; ++d) for (let s = 0; s < 4; ++s) {
    run('disjoint', 2048 + d, 512 + s, bytes);
    run('disjoint', 512 + d, 2048 + s, bytes);
  }
  const lengths = [0, 1, 2, 3, 4, 7, 8, 15, 16, 17, 31, 32, 33, 47, 48, 49,
    63, 64, 65, 127, 128, 129, 255, 256, 257, 1023, 1024, 1025, 4095, 4096];
  for (const bytes of lengths) for (let s = 0; s < 4; ++s) {
    for (let distance = 1; distance <= 16; ++distance) {
      run('overlap-forward', 512 + s - distance, 512 + s, bytes);
      run('overlap-backward', 512 + s + distance, 512 + s, bytes);
    }
    run('same-pointer', 512 + s, 512 + s, bytes);
  }
  for (const bytes of [255, 256, 257, 1023, 1024, 1025, 2047, 2048]) {
    for (let d = 0; d < 4; ++d) for (let s = 0; s < 4; ++s) {
      run('large-disjoint', 4096 + d, s, bytes);
      run('large-disjoint', d, 4096 + s, bytes);
    }
  }
  for (const bytes of lengths) {
    run('buffer-edge', 8192 - bytes, 0, bytes);
    run('buffer-edge', 0, 8192 - bytes, bytes);
  }
  for (const [destination, source, bytes, entry] of [[8193, 0, 0, target], [0, 8193, 0, target],
    [8192, 0, 1, target], [0, 8192, 1, target], [0, 0, 4097, target], [0, 0, 0, 0]]) {
    run('invalid-range', destination, source, bytes, 0x80000000, entry);
  }
  return { target, cases, categories, passed: failures.length === 0, failures,
    checked: ['full-buffer-snapshot', 'guards', 'cdecl-stack', 'return-pointer', 'callee-saved-registers', 'direction-flag', 'no-kernel-calls'],
    limitation: 'Bounded source-owned cases, not every possible input or proof of the complete game.' };
}
