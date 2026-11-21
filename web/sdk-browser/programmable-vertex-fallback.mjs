import { vertexGlsl } from './vertex-glsl.mjs';

const MAX_INSTRUCTIONS = 136;
export const FALLBACK_VERTEX_CODE_BYTES = MAX_INSTRUCTIONS * 16;

function operand(common, destination, negated, swizzle, register, mux) {
  let source = 0;
  let index = register;
  if (mux === 1) {
    if (index > 12) return null;
  } else if (mux === 2) {
    source = 1;
    index = (common >>> 9) & 15;
  } else {
    source = destination & 2 ? 3 : 2;
    index = (common >>> 13) & 255;
    if (source === 2 && index >= 192) return null;
  }
  return index | (swizzle << 8) | (source << 16) | (negated << 18);
}

// Read the owned instruction words once and retain a compact operand stream.
export function decodeFallbackVertexCode(hlsl, registers, flat) {
  if (registers !== 0x3f || flat) return null;
  const match = /^\/\/ ff_browser_vertex_words=(\d+(?:,\d+)*)$/m.exec(hlsl);
  if (!match) return null;
  const words = match[1].split(',').map(Number);
  if (words.length % 4 || words.length > MAX_INSTRUCTIONS * 4 ||
      words.some(word => !Number.isInteger(word) || word < 0 || word > 0xffffffff)) return null;

  const packed = new Uint32Array(words.length);
  let count = 0;
  for (let i = 0; i < words.length; i += 4) {
    const common = words[i + 1], sources = words[i + 2], destination = words[i + 3];
    const mac = (common >>> 21) & 15, ilu = (common >>> 25) & 7;
    if (mac > 13) return null;
    if (mac || ilu) {
      const a = mac ? operand(common, destination, (common >>> 8) & 1,
                              common & 255, sources >>> 28, (sources >>> 26) & 3) : 0;
      const usesB = mac === 2 || (mac >= 4 && mac <= 12);
      const b = usesB ? operand(common, destination, (sources >>> 25) & 1,
                                (sources >>> 17) & 255, (sources >>> 13) & 15, (sources >>> 11) & 3) : 0;
      let swizzleC = (sources >>> 2) & 255;
      if (ilu >= 2 && ilu <= 6) swizzleC = (swizzleC >>> 6) * 0x55;
      const c = ilu || mac === 3 || mac === 4
        ? operand(common, destination, (sources >>> 10) & 1,
                  swizzleC, ((sources & 3) << 2) | (destination >>> 30), (destination >>> 28) & 3)
        : 0;
      if (a === null || b === null || c === null) return null;

      const temporary = (destination >>> 20) & 15;
      const macMask = mac && mac !== 13 && temporary <= 12 && !(ilu && temporary === 1)
        ? (destination >>> 24) & 15 : 0;
      const iluMask = ilu && (mac || temporary <= 12) ? (destination >>> 16) & 15 : 0;
      const outRegister = (destination >>> 3) & 15;
      const outWithIlu = (destination >>> 2) & 1;
      const writesOutput = (destination & 0x800) && (outWithIlu ? ilu : mac && mac !== 13) &&
        (outRegister === 0 || (outRegister >= 3 && outRegister <= 12));
      let outMask = writesOutput ? (destination >>> 12) & 15 : 0;
      if (outRegister === 5) {
        const lanes = (outMask & 1) + ((outMask >>> 1) & 1) + ((outMask >>> 2) & 1) + (outMask >>> 3);
        outMask = 15 ^ (15 >>> lanes);
      }
      packed[count++] = mac | (ilu << 4) | (temporary << 7) | (macMask << 11) |
        (iluMask << 15) | (outRegister << 19) | (outMask << 23) | (outWithIlu << 27);
      packed[count++] = a;
      packed[count++] = b;
      packed[count++] = c;
    }
    if (destination & 1) break;
  }
  return count ? packed.subarray(0, count) : null;
}

const interpreter = `
layout(std140) uniform FallbackVertexCode {
  uvec4 fallbackInstructions[${MAX_INSTRUCTIONS}];
};
uniform int fallbackInstructionCount;

vec4 fallbackOperand(uint operand, vec4 inputs[6], vec4 registers[13], int addressRegister) {
  uint source = (operand >> 16u) & 3u;
  int index = int(operand & 255u);
  vec4 v;
  if (source == 0u) v = registers[index];
  else if (source == 1u) v = index < 6 ? inputs[index] : vec4(0.0, 0.0, 0.0, 1.0);
  else v = c[source == 3u ? clamp(index + addressRegister, 0, 191) : index];
  vec4 swizzled = vec4(v[int((operand >> 14u) & 3u)], v[int((operand >> 12u) & 3u)],
                       v[int((operand >> 10u) & 3u)], v[int((operand >> 8u) & 3u)]);
  return (operand & 0x40000u) != 0u ? -swizzled : swizzled;
}

vec4 fallbackMac(uint op, vec4 a, vec4 b, vec4 value) {
  switch (op) {
    case 1u: return a;
    case 2u: return mac_mul(a, b);
    case 3u: return a + value;
    case 4u: return mac_mul(a, b) + value;
    case 5u: return vec4(dot(a.xyz, b.xyz));
    case 6u: return vec4(dot(vec4(a.xyz, 1.0), b));
    case 7u: return vec4(dot(a, b));
    case 8u: return vec4(1.0, a.y * b.y, a.z, b.w);
    case 9u: return min(a, b);
    case 10u: return max(a, b);
    case 11u: return vec4(lessThan(a, b));
    case 12u: return vec4(greaterThanEqual(a, b));
    case 13u: return a;
    default: return vec4(0.0);
  }
}

vec4 fallbackIlu(uint op, vec4 v) {
  switch (op) {
    case 1u: return v;
    case 2u: return ilu_rcp(v.x);
    case 3u: return ilu_rcc(v.x);
    case 4u: return ilu_rsq(v.x);
    case 5u: return ilu_exp(v.x);
    case 6u: return ilu_log(v.x);
    case 7u: return ilu_lit(v);
    default: return vec4(0.0);
  }
}

vec4 fallbackMasked(vec4 current, vec4 value, uint mask) {
  return mix(current, value, notEqual(uvec4(mask) & uvec4(8u, 4u, 2u, 1u), uvec4(0u)));
}
`;

const evaluation = `
  vec4 fallbackInputs[6] = vec4[6](aV0, aV1, aV2, aV3, aV4, aV5);
  vec4 fallbackRegisters[13];
  vec4 fallbackOutputs[16];
  for (int i = 0; i < 13; ++i) fallbackRegisters[i] = vec4(0.0);
  for (int i = 0; i < 16; ++i) fallbackOutputs[i] = vec4(0.0);
  fallbackOutputs[3] = oD0;
  fallbackOutputs[7] = oB0;
  fallbackOutputs[9] = oT0;
  fallbackOutputs[10] = oT1;
  fallbackOutputs[11] = oT2;
  fallbackOutputs[12] = oT3;
  for (int i = 0; i < min(fallbackInstructionCount, ${MAX_INSTRUCTIONS}); ++i) {
    uvec4 instruction = fallbackInstructions[i];
    uint mac = instruction.x & 15u;
    uint ilu = (instruction.x >> 4u) & 7u;
    vec4 a = fallbackOperand(instruction.y, fallbackInputs, fallbackRegisters, a0);
    vec4 b = fallbackOperand(instruction.z, fallbackInputs, fallbackRegisters, a0);
    vec4 value = fallbackOperand(instruction.w, fallbackInputs, fallbackRegisters, a0);
    vec4 mr = fallbackMac(mac, a, b, value);
    vec4 ir = fallbackIlu(ilu, value);
    int dest = int((instruction.x >> 7u) & 15u);
    uint macMask = (instruction.x >> 11u) & 15u;
    uint iluMask = (instruction.x >> 15u) & 15u;
    uint outMask = (instruction.x >> 23u) & 15u;
    if (mac == 13u) a0 = mac_arl(mr.x);
    if (macMask != 0u) fallbackRegisters[dest] = fallbackMasked(fallbackRegisters[dest], mr, macMask);
    if (iluMask != 0u) {
      int iluDest = mac != 0u ? 1 : dest;
      fallbackRegisters[iluDest] = fallbackMasked(fallbackRegisters[iluDest], ir, iluMask);
    }
    if (outMask != 0u) {
      int outReg = int((instruction.x >> 19u) & 15u);
      vec4 result = (instruction.x & 0x8000000u) != 0u ? ir : mr;
      if (outReg == 0) fallbackRegisters[12] = fallbackMasked(fallbackRegisters[12], result, outMask);
      else fallbackOutputs[outReg] = fallbackMasked(fallbackOutputs[outReg], result, outMask);
    }
  }
  oPos = fallbackRegisters[12];
  oD0 = fallbackOutputs[3];
  oD1 = fallbackOutputs[4];
  oFog = fallbackOutputs[5];
  oB0 = fallbackOutputs[7];
  oB1 = fallbackOutputs[8];
  oT0 = fallbackOutputs[9];
  oT1 = fallbackOutputs[10];
  oT2 = fallbackOutputs[11];
  oT3 = fallbackOutputs[12];
`;

export function programmableFallbackVertexGlsl(hlsl) {
  const vertex = vertexGlsl(hlsl, 0x3f, false);
  const main = '\nvoid main() {';
  const epilogue = 'vec4 p = oPos;';
  if (!vertex.includes(main) || !vertex.includes(epilogue)) {
    throw new Error('Unsupported SDK template for the programmable vertex fallback');
  }
  return vertex.replace(main, interpreter + main).replace(epilogue, evaluation + epilogue);
}
