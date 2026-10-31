// Narrow experimental mono Xbox IMA ADPCM resource decoder.
// Layout hypothesis: https://reshax.com/topic/18058-ripping-sounds-from-fuzion-frenzy-xbox/
// Block behavior cross-check: https://ffmpeg.org/doxygen/trunk/adpcm_8c_source.html
const steps = [7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,34,37,41,45,50,55,60,66,73,80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,337,371,408,449,494,544,598,658,724,796,876,963,1060,1166,1282,1411,1552,1707,1878,2066,2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,5894,6484,7132,7845,8630,9493,10442,11487,12635,13899,15289,16818,18500,20350,22385,24623,27086,29794,32767];
const adjustments = [-1,-1,-1,-1,2,4,6,8];
const clamp = (x, a, b) => Math.max(a, Math.min(b, x));
export function decodeMonoCue(buffer) {
  if (!(buffer instanceof ArrayBuffer) || buffer.byteLength < 104 || buffer.byteLength > 8 * 1024 * 1024) throw new Error('Invalid audio resource size');
  const v = new DataView(buffer), channels = v.getUint16(74, true), rate = v.getUint32(76, true), alignment = v.getUint16(84, true), bits = v.getUint16(86, true), offset = v.getUint32(92, true), size = v.getUint32(100, true);
  if (v.getUint16(72, true) !== 0x69 || channels !== 1 || rate < 8000 || rate > 96000 || alignment !== 36 || bits !== 4 || offset !== 104 || size > buffer.byteLength - offset || size === 0 || size % alignment) throw new Error('Unsupported or malformed audio resource');
  const pcm = new Int16Array(size / alignment * 64);
  for (let block = 0; block < size / alignment; block++) {
    const start = offset + block * alignment;
    let predictor = v.getInt16(start, true), index = v.getUint16(start + 2, true);
    if (index > 88) throw new Error('Invalid ADPCM step index');
    let output = block * 64;
    pcm[output++] = predictor;
    for (let n = 0; n < 63; n++) {
      const packed = v.getUint8(start + 4 + (n >> 1));
      const nibble = (n & 1) ? packed >> 4 : packed & 15;
      const magnitude = ((2 * (nibble & 7) + 1) * steps[index]) >> 3;
      predictor = clamp(predictor + ((nibble & 8) ? -magnitude : magnitude), -32768, 32767);
      index = clamp(index + adjustments[nibble & 7], 0, 88);
      pcm[output++] = predictor;
    }
  }
  return { sampleRate: rate, pcm };
}
