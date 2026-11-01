// Resource admission for a browser-local engineering module, not a full Wasm validator.
// WebAssembly.compile performs the complete format/type validation afterward.
export const PROOF_WASM_LIMIT = 4 * 1024 * 1024;
export const PROOF_MEMORY_PAGES = 2048; // 128 MiB, private proof ceiling

export function inspectProofWasm(buffer) {
  if (!(buffer instanceof ArrayBuffer) || buffer.byteLength < 8 || buffer.byteLength > PROOF_WASM_LIMIT) {
    throw new Error('Invalid local module size');
  }
  const bytes = new Uint8Array(buffer);
  if ([0, 97, 115, 109, 1, 0, 0, 0].some((value, i) => bytes[i] !== value)) throw new Error('Invalid WebAssembly header');
  let cursor = 8, sectionEnd = bytes.length, memory;
  const seen = new Set();
  function uint() {
    let value = 0;
    for (let i = 0; i < 5; i++) {
      if (cursor >= sectionEnd) throw new Error('Truncated module integer');
      const byte = bytes[cursor++];
      if (i === 4 && (byte & 0xf0)) throw new Error('Invalid module integer');
      value += (byte & 127) * 2 ** (7 * i);
      if (!(byte & 128)) return value;
    }
    throw new Error('Invalid module integer');
  }
  while (cursor < bytes.length) {
    sectionEnd = bytes.length;
    const id = bytes[cursor++], length = uint(), start = cursor;
    if (length > bytes.length - start) throw new Error('Module section outside input');
    sectionEnd = start + length;
    if ([2, 5, 8].includes(id)) {
      if (seen.has(id)) throw new Error('Duplicate proof boundary section');
      seen.add(id);
    }
    if (id === 2 && uint() !== 0) throw new Error('Local proof modules must have no host imports');
    if (id === 8) throw new Error('Implicit module startup is not admitted');
    if (id === 5) {
      if (uint() !== 1 || uint() !== 1) throw new Error('One bounded unshared wasm32 memory is required');
      const minimum = uint(), maximum = uint();
      if (minimum < 1 || maximum < minimum || maximum > PROOF_MEMORY_PAGES || cursor !== sectionEnd) {
        throw new Error('Module memory exceeds the proof boundary');
      }
      memory = { minimumPages: minimum, maximumPages: maximum };
    }
    cursor = sectionEnd;
  }
  if (!memory) throw new Error('A bounded module memory declaration is required');
  return memory;
}
