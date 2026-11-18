const varyingNames = ["vD0", "vD1", "vB0", "vB1", "vTc0", "vTc1", "vTc2", "vTc3"];

function fail(message) {
  throw new Error(`Unsupported PixelHlsl: ${message}`);
}

function mainBody(source) {
  const signature = /\bfloat4\s+main\s*\(\s*VOut\s+i\s*,\s*bool\s+front\s*:\s*SV_IsFrontFace\s*\)\s*:\s*SV_Target\s*\{/g;
  const match = signature.exec(source);
  if (!match) fail("expected the pinned SDK pixel main signature");
  const start = signature.lastIndex;
  let depth = 1;
  for (let p = start; p < source.length; p++) {
    if (source[p] === "{") depth++;
    else if (source[p] === "}" && --depth === 0) return source.slice(start, p);
  }
  fail("unterminated main body");
}

function convertCasts(code) {
  // HLSL uses both C-style casts and constructor-style casts. Consume cast
  // operands with balanced delimiters so nested expressions stay intact.
  const cast = /\(\s*(float[234]|int[234]|bool[234]|vec[234]|ivec[234]|bvec[234]|float|int|bool)\s*\)\s*/g;
  let out = "";
  let last = 0;
  let match;
  while ((match = cast.exec(code))) {
    out += code.slice(last, match.index);
    const type = ({ float: "float", int: "int", bool: "bool" })[match[1]] ?? match[1].replace(/^float/, "vec").replace(/^int/, "ivec").replace(/^bool/, "bvec");
    let at = cast.lastIndex;
    let end = at;
    if (code[at] === "(") {
      let depth = 1;
      end = at + 1;
      while (end < code.length && depth) {
        if (code[end] === "(") depth++;
        else if (code[end] === ")") depth--;
        end++;
      }
      if (depth) fail("unbalanced cast operand");
    } else {
      const operand = /^(?:[A-Za-z_]\w*(?:\.[A-Za-z_]\w*)*|(?:\d+(?:\.\d*)?|\.\d+)(?:[fFuU])?)/.exec(code.slice(at));
      if (!operand) fail(`unsupported cast near ${code.slice(at, at + 24)}`);
      end = at + operand[0].length;
    }
    out += `${type}(${code.slice(at, end)})`;
    cast.lastIndex = end;
    last = cast.lastIndex;
  }
  out += code.slice(last);
  return out;
}

function vectorScalarInitializers(code) {
  // GLSL does not allow `vecN x = 0`; HLSL uses it extensively, including
  // later declarators in comma-separated declarations. Split only at commas
  // outside parentheses so constructor arguments remain untouched.
  const decl = /\b(vec[234]|ivec[234]|bvec[234])\s+([^;]+);/g;
  return code.replace(decl, (whole, type, tail) => {
    const parts = [];
    let start = 0, depth = 0;
    for (let i = 0; i < tail.length; i++) {
      if (tail[i] === "(") depth++;
      else if (tail[i] === ")") depth--;
      else if (tail[i] === "," && depth === 0) { parts.push(tail.slice(start, i)); start = i + 1; }
    }
    parts.push(tail.slice(start));
    const fixed = parts.map((part) => part.replace(/^(\s*[A-Za-z_]\w*\s*=\s*)([-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?)(\s*)$/, (_m, before, scalar, after) => `${before}${type}(${scalar})${after}`));
    return `${type} ${fixed.join(",")};`;
  });
}

function decimalizeFloatLiterals(code) {
  const types = new Map();
  const declarations = /\b(vec[234]|ivec[234]|bvec[234]|float|int|uint|bool)\s+([^;]+);/g;
  for (const [, type, tail] of code.matchAll(declarations)) {
    const parts = [];
    let start = 0, depth = 0;
    for (let i = 0; i < tail.length; i++) {
      if (tail[i] === "(") depth++;
      else if (tail[i] === ")") depth--;
      else if (tail[i] === "," && depth === 0) { parts.push(tail.slice(start, i)); start = i + 1; }
    }
    parts.push(tail.slice(start));
    for (const part of parts) {
      const name = /^\s*([A-Za-z_]\w*)/.exec(part)?.[1];
      if (name) types.set(name, type);
    }
  }
  for (const name of ["surf", "fogcolor", "misc", "tfactor", "pc", "texsize", "bumpmat", "bumplum", "uvrect", "vD0", "vD1", "vB0", "vB1", "vTc0", "vTc1", "vTc2", "vTc3"]) types.set(name, "vec4");
  types.set("vFog", "float");
  const floatType = (type) => type === "float" || /^vec[234]$/.test(type || "");
  const baseType = (expr) => {
    const m = /([A-Za-z_]\w*)\s*(?:\.[xyzwrgba]{1,4})?\s*$/.exec(expr);
    if (m) return types.get(m[1]);
    const call = /([A-Za-z_]\w*)\s*\(/.exec(expr);
    if (call && ["dot", "max", "min", "abs", "clamp", "round", "floor", "mod", "saturate", "mix"].includes(call[1])) return "float";
    return undefined;
  };
  const literal = /(?<![\w.])\d+(?![\w.])/g;
  const replacements = [];
  let scanned = 0, depth = 0;
  let m;
  while ((m = literal.exec(code))) {
    const at = m.index, value = m[0];
    for (; scanned < at; scanned++) {
      if (code[scanned] === "[") depth++;
      else if (code[scanned] === "]") depth--;
    }
    if (depth > 0) continue;
    const before = code.slice(Math.max(0, at - 120), at);
    const after = code.slice(at + value.length, Math.min(code.length, at + value.length + 120));
    if (/\b(?:uint|int|ivec[234])\s*\([^()]*$/.test(before) || /&\s*$/.test(before) || /^\s*&/.test(after)) continue;
    let inferred;
    const leftOp = /(?:==|!=|<=|>=|[+*/<>-])\s*$/.exec(before);
    if (leftOp) {
      const expr = before.slice(0, leftOp.index);
      inferred = baseType(expr);
      if (!inferred) {
        const names = [...expr.matchAll(/\b[A-Za-z_]\w*\b/g)].map((item) => types.get(item[0])).filter(Boolean);
        if (names.some(floatType) && !names.some((type) => type === "int" || type === "uint" || /^ivec/.test(type))) inferred = "float";
      }
    }
    if (!inferred) {
      const comma = /,\s*$/.exec(before);
      if (comma) {
        const prefix = before.slice(0, comma.index);
        const fn = /([A-Za-z_]\w*)\s*\([^()]*$/.exec(prefix);
        if (fn && ["max", "min", "clamp", "mod"].includes(fn[1])) inferred = baseType(prefix.slice(fn.index + fn[0].indexOf("(") + 1));
      }
    }
    if (!inferred) {
      const rightOp = /^\s*(?:==|!=|<=|>=|[+*/<>-])/.exec(after);
      if (rightOp) inferred = baseType(after.slice(rightOp[0].length));
    }
    if (floatType(inferred)) replacements.push([at, at + value.length, `${value}.0`]);
  }
  for (let i = replacements.length - 1; i >= 0; i--) {
    const [start, end, replacement] = replacements[i];
    code = code.slice(0, start) + replacement + code.slice(end);
  }
  return code;
}

function rewriteShadowCalls(code) {
  const re = /\bshadow\s*\(/g;
  let out = "", last = 0, m;
  while ((m = re.exec(code))) {
    const open = code.indexOf("(", m.index);
    let depth = 1, end = open + 1, argStart = end;
    const args = [];
    for (; end < code.length && depth; end++) {
      const c = code[end];
      if (c === "(") depth++;
      else if (c === ")") depth--;
      else if (c === "," && depth === 1) { args.push(code.slice(argStart, end).trim()); argStart = end + 1; }
    }
    if (depth) fail("unterminated shadow call");
    args.push(code.slice(argStart, end - 1).trim());
    const tex = /^tex([0-3])$/.exec(args[0] || "");
    if (!tex || args.length < 5 || args[1] !== `smp${tex[1]}`) fail("unsupported shadow call shape");
    out += code.slice(last, m.index) + `shadow${tex[1]}(${args[2]}, ${args[3]}, ${args[4] || "0"})`;
    last = end;
    re.lastIndex = end;
  }
  return out + code.slice(last);
}

function textureHelpers(texKinds, shadows) {
  const any2d = texKinds.some((kind) => kind === 1);
  if (!any2d) return "";
  let source = `int addressPixel(int p, int n, int mode, out bool borderHit) {
  borderHit = false;
  if (mode == 1) return ((p % n) + n) % n;
  if (mode == 2) {
    int period = 2 * n;
    int q = ((p % period) + period) % period;
    return q < n ? q : period - 1 - q;
  }
  if (mode == 3 || mode == 5) return clamp(p, 0, n - 1);
  if (mode == 4) { borderHit = p < 0 || p >= n; return clamp(p, 0, n - 1); }
  borderHit = true;
  return clamp(p, 0, n - 1);
}
bool depthCompare(float reference, float sampleValue) {
  int f = int(misc.z);
  if (f == 0) return false;
  if (f == 1) return sampleValue < reference;
  if (f == 2) return reference == sampleValue;
  if (f == 3) return sampleValue <= reference;
  if (f == 4) return sampleValue > reference;
  if (f == 5) return reference != sampleValue;
  if (f == 6) return sampleValue >= reference;
  return true;
}
`;
  for (let i = 0; i < 4; i++) {
    if (texKinds[i] !== 1) continue;
    const pos = `ivec2(floor(uv * vec2(sz)))`;
    if (shadows[i]) {
      source += `float shadowTap${i}(ivec2 p, ivec2 sz, float shadowRef) {
  bool bx, by;
  int x = addressPixel(p.x, sz.x, int(addresses[${i}].x), bx);
  int y = addressPixel(p.y, sz.y, int(addresses[${i}].y), by);
  float depthValue = (bx || by) ? border[${i}].r : texelFetch(tex${i}, ivec2(x, y), 0).r;
  return depthCompare(shadowRef, depthValue) ? 1.0 : 0.0;
}
vec4 shadow${i}(vec2 uv, float z, int fmt) {
  ivec2 sz = textureSize(tex${i}, 0);
  float mx = fmt == 2 ? 65535.0 : 16777215.0;
  float shadowRef = clamp(z, 0.0, mx) / mx;
  vec2 p = uv * vec2(sz) - 0.5;
  if (addresses[${i}].z == 1.0 && addresses[${i}].w == 1.0)
    return vec4(shadowTap${i}(${pos}, sz, shadowRef));
  ivec2 base = ivec2(floor(p)); vec2 f = fract(p);
  float a = shadowTap${i}(base, sz, shadowRef), b = shadowTap${i}(base + ivec2(1, 0), sz, shadowRef);
  float c = shadowTap${i}(base + ivec2(0, 1), sz, shadowRef), d = shadowTap${i}(base + ivec2(1, 1), sz, shadowRef);
  return vec4(mix(mix(a, b, f.x), mix(c, d, f.x), f.y));
}
`;
    } else {
      source += `vec4 sample${i}Tap(ivec2 p, ivec2 sz) {
  bool bx, by;
  int x = addressPixel(p.x, sz.x, int(addresses[${i}].x), bx);
  int y = addressPixel(p.y, sz.y, int(addresses[${i}].y), by);
  return (bx || by) ? border[${i}] : texelFetch(tex${i}, ivec2(x, y), 0);
}
vec4 sample${i}(vec2 uv) {
  if (int(addresses[${i}].x) != 4 && int(addresses[${i}].y) != 4) return texture(tex${i}, uv);
  ivec2 sz = textureSize(tex${i}, 0);
  vec2 dx = dFdx(uv) * vec2(sz), dy = dFdy(uv) * vec2(sz);
  bool magnifying = max(dot(dx, dx), dot(dy, dy)) <= 1.0;
  float selectedFilter = magnifying ? addresses[${i}].w : addresses[${i}].z;
  if (selectedFilter == 1.0)
    return sample${i}Tap(${pos}, sz);
  vec2 p = uv * vec2(sz) - 0.5;
  ivec2 base = ivec2(floor(p)); vec2 f = fract(p);
  vec4 a = sample${i}Tap(base, sz), b = sample${i}Tap(base + ivec2(1, 0), sz);
  vec4 c = sample${i}Tap(base + ivec2(0, 1), sz), d = sample${i}Tap(base + ivec2(1, 1), sz);
  return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}
`;
    }
  }
  return source;
}

function saturates(code) {
  const re = /\bsaturate\s*\(/g;
  let out = "", last = 0, m;
  while ((m = re.exec(code))) {
    let depth = 1, end = re.lastIndex;
    for (; end < code.length && depth; end++) {
      if (code[end] === "(") depth++;
      else if (code[end] === ")") depth--;
    }
    if (depth) fail("unterminated saturate call");
    out += code.slice(last, m.index) + `clamp(${code.slice(re.lastIndex, end - 1)}, 0.0, 1.0)`;
    last = end;
    re.lastIndex = end;
  }
  return out + code.slice(last);
}

function lowerBody(body, texKinds, shadows) {
  if (/\b(?:spreadtap|shadow|uvclamp|sbyte)\s*\(/.test(body)) {
    // These calls are translated below; hooks or other unfamiliar helpers are rejected.
  }
  const known = new Set(["spreadtap", "shadow", "uvclamp", "sbyte", "alphatest"]);
  const calls = [...body.matchAll(/\b([A-Za-z_]\w*)\s*\(/g)].map((m) => m[1]);
  const builtins = new Set(["float", "float2", "float3", "float4", "int", "int2", "int3", "int4", "uint", "bool", "saturate", "lerp", "min", "max", "abs", "dot", "clamp", "round", "texture", "Sample", "if"]);
  for (const call of calls) {
    if (/^tex[0-3]$/.test(call) || known.has(call) || builtins.has(call)) continue;
    fail(`unknown helper ${call}`);
  }
  if (/\b(?:ddx|ddy|asfloat|SampleCmp|SampleLevel)\b/.test(body)) fail("derivatives, bit reinterpretation, or comparison sampling are outside this translator");
  for (let i = 0; i < 4; i++) {
    if (new RegExp(`\\btex${i}\\.Sample\\s*\\(`).test(body) && !texKinds[i]) fail(`texture ${i} is sampled but has no declared kind`);
    if (new RegExp(`\\bshadow\\s*\\(\\s*tex${i}\\b`).test(body) && !shadows[i]) fail(`texture ${i} uses an unconfigured shadow sampler`);
    if (new RegExp(`\\btex${i}\\.Sample\\s*\\(`).test(body) && texKinds[i] !== 1) fail(`texture ${i} border sampler requires a 2D texture`);
  }
  if (/\bspreadtap\s*\(/.test(body)) fail("spreadtap requires SDK sampling hooks and is not enabled");

  let code = body
    .replace(/\bi\.(d0|d1|b0|b1|tc0|tc1|tc2|tc3|fogamt)\b/g, (_m, n) => ({ d0: "vD0", d1: "vD1", b0: "vB0", b1: "vB1", tc0: "vTc0", tc1: "vTc1", tc2: "vTc2", tc3: "vTc3", fogamt: "vFog" })[n])
    .replace(/\bi\.pos\b/g, "vec4(gl_FragCoord.xy, gl_FragCoord.z, 1.0)")
    .replace(/\bfront\b/g, "gl_FrontFacing")
    .replace(/\bfloat([234])\b/g, "vec$1")
    .replace(/\bint([234])\b/g, "ivec$1")
    .replace(/\blerp\s*\(/g, "mix(")
    .replace(/\btex([0-3])\.Sample\s*\(\s*smp\d+\s*,/g, "sample$1(")
    .replace(/\(?\s*uint\s*\(\s*r0\.a\s*\*\s*255\s*\)\s*&\s*1\s*\)?\s*==\s*1/g, "mod(floor(r0.a * 255.0), 2.0) == 1.0")
    .replace(/\breturn\s+outc\s*;/g, "fragColor = outc; return;");
  code = rewriteShadowCalls(code);
  code = convertCasts(code);
  code = vectorScalarInitializers(code);
  code = saturates(code);
  code = decimalizeFloatLiterals(code);
  // Generated declarations often initialize vectors from scalar zero.
  if (/\.Sample\s*\(|\bsmp\d\b/.test(code)) fail("unlowered HLSL sampler operation");
  if (/\b(?:float[234]|int[234]|saturate|lerp)\b/.test(code)) fail("unlowered HLSL syntax");
  return code;
}

export function pixelGlsl(hlsl, texKinds, shadows, flat = false) {
  if (typeof hlsl !== "string" || !Array.isArray(texKinds) || !Array.isArray(shadows) || texKinds.length !== 4 || shadows.length !== 4) {
    throw new TypeError("pixelGlsl expects HLSL and four texture-kind/shadow entries");
  }
  for (const kind of texKinds) if (![0, 1, 2, 3].includes(kind)) fail(`invalid texture kind ${kind}`);
  for (let i = 0; i < 4; i++) if (shadows[i] && texKinds[i] !== 1) fail(`shadow texture ${i} must be 2D`);
  const body = lowerBody(mainBody(hlsl), texKinds, shadows);
  return pixelShaderSource(body, texKinds, shadows, flat);
}

function pixelShaderSource(body, texKinds, shadows, flat, helpers = "") {
  const colorQualifier = flat ? "flat " : "";
  const samplers = texKinds.map((kind, i) => kind ? `uniform highp ${kind === 1 ? "sampler2D" : kind === 2 ? "sampler3D" : "samplerCube"} tex${i};` : "").filter(Boolean).join("\n");
  const inputs = varyingNames.map((name) => `${["vD0", "vD1", "vB0", "vB1"].includes(name) ? colorQualifier : ""}in vec4 ${name};`).join("\n");
  const sbyte = `vec3 sbyte(vec3 u, float lo, float hi, float d) {
  vec3 b = u * 255.0;
  vec3 offset = vec3(lo) + step(vec3(128.0), b) * (hi - lo);
  return (b - offset) / d;
}`;
  return `#version 300 es
precision highp float;
precision highp int;
precision highp sampler2D;
uniform vec4 surf, fogcolor, misc, pc[18], tfactor, texsize[4], bumpmat[4], bumplum[4], uvrect[4];
uniform vec4 addresses[4], border[4];
${samplers}
${inputs}
in float vFog;
out vec4 fragColor;
${sbyte}
vec2 uvclamp(vec2 uv, vec4 r) {
  return r.z > r.x && r.w > r.y ? clamp(uv, r.xy, r.zw) : uv;
}
${textureHelpers(texKinds, shadows)}
void alphatest(float a) {
  int f = int(misc.y);
  int v = int(round(a * 255.0)), r = int(misc.x);
  bool ok = f == 0 ? false : f == 1 ? v < r : f == 2 ? v == r : f == 3 ? v <= r : f == 4 ? v > r : f == 5 ? v != r : f == 6 ? v >= r : true;
  if (!ok) discard;
}
${helpers}void main() {${body}
}
`;
}

// A uniform-driven form of the pinned GPU generator's fixed texture stages.
// Admission of non-2D, shadow, wide and quad modes belongs to the caller.
export function fixedFallbackPixelGlsl(flat = false) {
  const helpers = `uniform highp uint fallbackPixelKey[41];
vec4 fallbackArgument(uint argument, vec4 v0, vec4 v1, vec4 cur, vec4 tmp, vec4 texel) {
  vec4 value;
  switch (argument & 15u) {
    case 0u: value = v0; break;
    case 2u: value = texel; break;
    case 3u: value = tfactor; break;
    case 4u: value = v1; break;
    case 5u: value = tmp; break;
    default: value = cur; break;
  }
  if ((argument & 32u) != 0u) value = vec4(value.a);
  if ((argument & 16u) != 0u) value = vec4(1.0) - value;
  return value;
}
vec4 fallbackOperation(uint operation, vec4 a1, vec4 a2, vec4 a0,
                       vec4 cur, vec4 v0, vec4 texel, bool alpha) {
  switch (operation) {
    case 2u: return a1;
    case 3u: return a2;
    case 4u: return a1 * a2;
    case 5u: return a1 * a2 * 2.0;
    case 6u: return a1 * a2 * 4.0;
    case 7u: return a1 + a2;
    case 8u: return a1 + a2 - 0.5;
    case 9u: return (a1 + a2 - 0.5) * 2.0;
    case 10u: return a1 - a2;
    case 11u: return a1 + a2 * (vec4(1.0) - a1);
    case 12u: return mix(a2, a1, v0.a);
    case 13u: return mix(a2, a1, cur.a);
    case 14u: return mix(a2, a1, texel.a);
    case 15u: return mix(a2, a1, tfactor.a);
    case 16u: return a1 + a2 * (1.0 - texel.a);
    case 18u: return alpha ? a1 : a1 + a2 * a1.a;
    case 19u: return alpha ? a1 : a1 * a2 + a1.a;
    case 20u: return alpha ? a1 : a1 + a2 * (1.0 - a1.a);
    case 21u: return alpha ? a1 : (vec4(1.0) - a1) * a2 + a1.a;
    case 22u: {
      vec3 x = alpha ? vec3(a1.a, 0.0, 0.0) : a1.rgb;
      vec3 y = alpha ? vec3(a2.a, 0.0, 0.0) : a2.rgb;
      return vec4(clamp(dot((x - 0.5) * 2.0, (y - 0.5) * 2.0), 0.0, 1.0));
    }
    case 23u: return a0 + a1 * a2;
    case 24u: return mix(a2, a1, a0);
    default: return cur;
  }
}
`;
  const stages = Array.from({length: 4}, (_, stage) => {
    const base = 5 + stage * 9, bit = 1 << stage;
    return `
  if (fallbackPixelKey[${base}] <= 1u) stagesEnabled = false;
  if (stagesEnabled) {
    vec4 texel = vec4(1.0);
    if ((fallbackPixelKey[4] & ${bit}u) != 0u) {
      vec4 coord = vTc${stage};
      vec2 uv = coord.xy;
      if ((fallbackPixelKey[2] & ${bit}u) != 0u) uv /= coord.w;
      if ((fallbackPixelKey[3] & ${bit}u) != 0u) uv /= texsize[${stage}].xy;
      uv += texsize[${stage}].zw;
      texel = sample${stage}(uvclamp(uv, uvrect[${stage}]));
    }
    if ((fallbackPixelKey[0] & ${bit}u) != 0u && texel.a == 0.0) discard;
    vec4 a1 = fallbackArgument(fallbackPixelKey[${base + 1}], v0, v1, cur, tmp, texel);
    vec4 a2 = fallbackArgument(fallbackPixelKey[${base + 2}], v0, v1, cur, tmp, texel);
    vec4 a0 = fallbackArgument(fallbackPixelKey[${base + 3}], v0, v1, cur, tmp, texel);
    vec3 color = clamp(fallbackOperation(fallbackPixelKey[${base}], a1, a2, a0,
                                       cur, v0, texel, false).rgb, vec3(0.0), vec3(1.0));
    float alpha = cur.a;
    if (fallbackPixelKey[${base + 4}] > 1u) {
      a1 = fallbackArgument(fallbackPixelKey[${base + 5}], v0, v1, cur, tmp, texel);
      a2 = fallbackArgument(fallbackPixelKey[${base + 6}], v0, v1, cur, tmp, texel);
      a0 = fallbackArgument(fallbackPixelKey[${base + 7}], v0, v1, cur, tmp, texel);
      alpha = fallbackOperation(fallbackPixelKey[${base + 4}], a1, a2, a0,
                                cur, v0, texel, true).a;
    }
    alpha = clamp(alpha, 0.0, 1.0);
    if (fallbackPixelKey[${base + 8}] == 5u) tmp = vec4(color, alpha);
    else cur = vec4(color, alpha);
  }
`;
  }).join("");
  const body = `
  vec4 v0 = fallbackPixelKey[1] != 0u && !gl_FrontFacing ? vB0 : vD0;
  vec4 v1 = fallbackPixelKey[1] != 0u && !gl_FrontFacing ? vB1 : vD1;
  if (misc.w == 0.0) v1 = vec4(0.0, 0.0, 0.0, 1.0);
  vec4 fogin = vec4(fogcolor.rgb, clamp(vFog, 0.0, 1.0));
  vec4 cur = v0, tmp = vec4(0.0);
  bool stagesEnabled = true;
${stages}
  cur.rgb = clamp(cur.rgb + v1.rgb, vec3(0.0), vec3(1.0));
  vec4 outc = vec4(mix(fogin.rgb, cur.rgb, fogin.a), cur.a);
  alphatest(outc.a);
  outc.rgb = clamp(outc.rgb, vec3(0.0), vec3(1.0));
  fragColor = outc;
  return;
`;
  return pixelShaderSource(body, [1, 1, 1, 1], [0, 0, 0, 0], flat, helpers);
}
