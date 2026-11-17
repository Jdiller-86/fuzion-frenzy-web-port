const outputs = ["vD0", "vD1", "vB0", "vB1", "vTc0", "vTc1", "vTc2", "vTc3"];

function fail(message) { throw new Error(`Unsupported VertexHlsl: ${message}`); }

function block(source, signature) {
  const re = new RegExp(signature, "g");
  const m = re.exec(source);
  if (!m) fail(`expected generated ${signature}`);
  const start = re.lastIndex;
  let depth = 1;
  for (let p = start; p < source.length; p++) {
    if (source[p] === "{") depth++;
    else if (source[p] === "}" && --depth === 0) return source.slice(start, p);
  }
  fail("unterminated generated function");
}

function balancedEnd(code, start) {
  if (code[start] === "(") {
    let depth = 1;
    for (let i = start + 1; i < code.length; i++) {
      if (code[i] === "(") depth++;
      else if (code[i] === ")" && --depth === 0) return i + 1;
    }
    fail("unbalanced cast operand");
  }
  let parens = 0, brackets = 0;
  for (let i = start; i < code.length; i++) {
    if (code[i] === "(") parens++;
    else if (code[i] === ")" && --parens < 0) return i;
    else if (code[i] === "[") brackets++;
    else if (code[i] === "]") brackets--;
    if (parens === 0 && brackets === 0 && /[;,{}\n]/.test(code[i])) return i;
  }
  return code.length;
}

function lowerCasts(code) {
  const re = /\(\s*(float[234]|int[234]|vec[234]|ivec[234]|uint|float|int)\s*\)\s*/g;
  let out = "", last = 0, m;
  while ((m = re.exec(code))) {
    const type = ({ float: "float", float2: "vec2", float3: "vec3", float4: "vec4", int: "int", int2: "ivec2", int3: "ivec3", int4: "ivec4", uint: "uint", vec2: "vec2", vec3: "vec3", vec4: "vec4", ivec2: "ivec2", ivec3: "ivec3", ivec4: "ivec4" })[m[1]];
    let start = re.lastIndex;
    while (/\s/.test(code[start] || "")) start++;
    let end = balancedEnd(code, start);
    if (end === start) {
      const operand = /^(?:[A-Za-z_]\w*(?:\.[A-Za-z_]\w*)*|[-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?)/.exec(code.slice(start));
      if (!operand) fail(`unsupported cast operand near ${code.slice(start, start + 24)}`);
      end = start + operand[0].length;
    }
    out += code.slice(last, m.index) + `${type}(${code.slice(start, end).trim()})`;
    last = end; re.lastIndex = end;
  }
  return out + code.slice(last);
}

function lowerMatrixMultiplies(code) {
  const re = /\bmul\s*\(/g;
  let out = "", last = 0, m;
  while ((m = re.exec(code))) {
    const open = code.indexOf("(", m.index);
    let depth = 1, brackets = 0, split = -1, end = open + 1;
    for (; end < code.length && depth; end++) {
      const c = code[end];
      if (c === "(") depth++;
      else if (c === ")") depth--;
      else if (c === "[") brackets++;
      else if (c === "]") brackets--;
      else if (c === "," && depth === 1 && brackets === 0) {
        if (split >= 0) fail("mul has more than two arguments");
        split = end;
      }
    }
    if (depth) fail("unterminated mul call");
    if (split < 0) fail("mul requires a row-vector and matrix");
    const left = code.slice(open + 1, split).trim();
    const right = code.slice(split + 1, end - 1).trim();
    if (!/^(?:wvp|wv|wvn|tm\s*\[\s*[0-3]\s*\])$/.test(right)) fail(`unsupported HLSL mul matrix ${right}`);
    out += code.slice(last, m.index) + `(${left} * ${right})`;
    last = end;
    re.lastIndex = end;
  }
  return out + code.slice(last);
}

function lowerSplats(code) {
  const re = /\.xxxx\b/g;
  let out = "", last = 0, m;
  while ((m = re.exec(code))) {
    let end = m.index, start;
    if (code[end - 1] === ")") {
      let depth = 1;
      start = end - 2;
      for (; start >= 0; start--) {
        if (code[start] === ")") depth++;
        else if (code[start] === "(" && --depth === 0) break;
      }
      if (start < 0) fail("unbalanced scalar splat");
    } else {
      const token = /[A-Za-z_]\w*$/.exec(code.slice(0, end));
      if (!token) fail("unsupported scalar splat expression");
      start = end - token[0].length;
    }
    const name = /[A-Za-z_]\w*$/.exec(code.slice(0, start));
    if (name && !/\s/.test(code[start - 1] || "")) start -= name[0].length;
    const expression = code.slice(start, end);
    if (!/\b(?:dot|magclamp|uintBitsToFloat|inversesqrt)\s*\(/.test(expression) && !/^\([^()]*[/*+-][^()]*\)$/.test(expression)) {
      out += code.slice(last, re.lastIndex);
      last = re.lastIndex;
      continue;
    }
    out += code.slice(last, start) + `vec4(${expression})`;
    last = re.lastIndex;
  }
  return out + code.slice(last);
}

function decimalizeFloatLiterals(code) {
  const types = new Map();
  const declarations = /\b(vec[234]|ivec[234]|float|int|uint|bool)\s+([^;]+);/g;
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
  for (const name of ["c", "surf", "fogp", "vD0", "vD1", "vB0", "vB1", "vTc0", "vTc1", "vTc2", "vTc3", "p", "q", "oPos", "oD0", "oD1", "oB0", "oB1", "oT0", "oT1", "oT2", "oT3", "a", "b", "s"]) types.set(name, "vec4");
  for (const name of ["vFog", "v", "d", "w"]) types.set(name, "float");
  const floatType = (type) => type === "float" || /^vec[234]$/.test(type || "");
  const baseType = (expr) => {
    const m = /([A-Za-z_]\w*)\s*(?:\.[xyzwrgba]{1,4})?\s*$/.exec(expr);
    if (m) return types.get(m[1]);
    return undefined;
  };
  const replacements = [];
  const literal = /(?<![\w.])\d+(?:\.\d*)?(?:[eE][+-]?\d+)?(?![\w.])/g;
  for (const m of code.matchAll(literal)) {
    const at = m.index, value = m[0];
    if(/[.eE]/.test(value))continue;
    let inIndex = false, depth = 0;
    for (let i = 0; i < at; i++) {
      if (code[i] === "[") { if (!depth) inIndex = true; depth++; }
      else if (code[i] === "]") { depth--; if (!depth) inIndex = false; }
    }
    if (inIndex) continue;
    const before = code.slice(Math.max(0, at - 120), at), after = code.slice(at + value.length, at + value.length + 120);
    if (/\b(?:uint|int|ivec[234])\s*\([^()]*$/.test(before) || /&\s*$/.test(before) || /^\s*&/.test(after)) continue;
    let inferred;
    const left = /(?:==|!=|<=|>=|[+*/<>-])\s*$/.exec(before);
    if (left) inferred = baseType(before.slice(0, left.index));
    if (!inferred) {
      const right = /^\s*(?:==|!=|<=|>=|[+*/<>-])/.exec(after);
      if (right) {
        const operand=/^\s*([A-Za-z_]\w*)/.exec(after.slice(right[0].length));
        inferred=operand?types.get(operand[1]):undefined;
      }
    }
    if(!inferred&&/\bfloat\s+\w+\s*=[^;]*$/.test(before))inferred='float';
    if(!inferred){
      const assignment=/\b([A-Za-z_]\w*)(?:\.[xyzwrgba]+)?\s*(?:[+*/-]?=)\s*[^;{}]*$/.exec(before);
      if(assignment&&floatType(types.get(assignment[1])))inferred=types.get(assignment[1]);
    }
    if (floatType(inferred)) replacements.push([at, at + value.length, `${value}.0`]);
  }
  for (let i = replacements.length - 1; i >= 0; i--) {
    const [start, end, replacement] = replacements[i];
    code = code.slice(0, start) + replacement + code.slice(end);
  }
  return code;
}

function vectorScalarInitializers(code) {
  const decl = /^([ \t]*)(vec[234])\s+([^;\r\n]+);/gm;
  return code.replace(decl, (_whole, indent, type, tail) => {
    const parts = [];
    let start = 0, depth = 0;
    for (let i = 0; i < tail.length; i++) {
      if (tail[i] === "(") depth++;
      else if (tail[i] === ")") depth--;
      else if (tail[i] === "," && depth === 0) { parts.push(tail.slice(start, i)); start = i + 1; }
    }
    parts.push(tail.slice(start));
    return `${indent}${type} ${parts.map((part) => part.replace(/(=\s*)([-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?)(\s*)$/, (_m, before, n, after) => `${before}${type}(${n.includes(".") || /[eE]/.test(n) ? n : `${n}.0`})${after}`)).join(",")};`;
  });
}

function lowerBody(body) {
  body = body.replace(/\/\/[^\r\n]*/g, "");
  let code = body
    .replace(/\bfloat([234])\b/g, "vec$1")
    .replace(/\bint([234])\b/g, "ivec$1")
    .replace(/\bprecise\s+/g, "")
    .replace(/\basfloat\s*\(\s*(0x[0-9A-Fa-f]+)\s*\)/g, "uintBitsToFloat($1u)")
    .replace(/\brsqrt\s*\(/g, "inversesqrt(")
    .replace(/\bi\.v(\d+)\b/g, "aV$1")
    .replace(/\bVOut\s+o\s*;/g, "")
    .replace(/\bo\.pos\b/g, "gl_Position")
    .replace(/\bo\.(d0|d1|b0|b1|tc0|tc1|tc2|tc3)\b/g, (_m, n) => ({ d0: "vD0", d1: "vD1", b0: "vB0", b1: "vB1", tc0: "vTc0", tc1: "vTc1", tc2: "vTc2", tc3: "vTc3" })[n])
    .replace(/\bo\.fogamt\b/g, "vFog")
    .replace(/\bsaturate\s*\(/g, "sat(")
    .replace(/\bc\[clamp\(a0\s*\+\s*(\d+),\s*0,\s*191\)\]/g, "c[clamp(a0 + $1, 0, 191)]")
    .replace(/\breturn\s+o\s*;/g, "return;");
  code = lowerMatrixMultiplies(code);
  if (/\b(?:mul|transpose|mad|rsqrt)\b/.test(code)) fail("unlowered HLSL construct");
  const known = new Set(["if", "float", "vec2", "vec3", "vec4", "int", "ivec2", "ivec3", "ivec4", "uint", "dot", "min", "max", "abs", "clamp", "floor", "exp", "exp2", "log2", "sqrt", "inversesqrt", "normalize", "length", "any", "equal", "mix", "uintBitsToFloat", "sat", "mac_mul", "mac_arl", "magclamp", "ilu_rcp", "ilu_rcc", "ilu_rsq", "ilu_exp", "ilu_log", "ilu_lit", "fogfactor"]);
  known.add('if');
  known.add('pow');known.add('reflect');
  for (const [, name] of code.matchAll(/\b([A-Za-z_]\w*)\s*\(/g)) if (!known.has(name)) fail(`unknown helper ${name}`);
  code = lowerCasts(code);
  code = code.replace(/vec4\((.+)\s(>=|<)\s(.+)\)/g, (_m, left, op, right) => `vec4(${op === "<" ? "lessThan" : "greaterThanEqual"}(${left}, ${right}))`);
  if (/\b(?:VOut|VIn|VRaw|SV_|ATTR\d|i\.|o\.)/.test(code)) fail("unlowered SDK wrapper field");
  // The SDK HLSL helper's conditional is vector-valued; lower it lane-wise.
  code = code.replace(/return \(a == 0 \|\| b == 0\) \? vec4\(0\.0\) : a \* b;/,
    "return vec4(a.x == 0 || b.x == 0 ? 0.0 : a.x * b.x, a.y == 0 || b.y == 0 ? 0.0 : a.y * b.y, a.z == 0 || b.z == 0 ? 0.0 : a.z * b.z, a.w == 0 || b.w == 0 ? 0.0 : a.w * b.w);");
  code = lowerSplats(code);
  code = decimalizeFloatLiterals(code);
  code = vectorScalarInitializers(code);
  if (/\b(?:float[234]|int[234]|rsqrt|asfloat|precise)\b/.test(code)) fail("HLSL token remains");
  return code;
}

function lowerHelpers(hlsl) {
  // Preserve the SDK's helper formulas by extracting them from the supplied generator output.
  const start = hlsl.indexOf("float4 mac_mul(");
  const end = hlsl.indexOf("struct VIn", start);
  if (start < 0 || end < 0) fail("missing pinned SDK vertex helper block");
  let helpers = hlsl.slice(start, end);
  helpers = helpers
    .replace(/\bfloat([234])\b/g, "vec$1")
    .replace(/\bint([234])\b/g, "ivec$1")
    .replace(/\bprecise\s+/g, "")
    .replace(/\basfloat\s*\(\s*(0x[0-9A-Fa-f]+)\s*\)/g, "uintBitsToFloat($1u)")
    .replace(/\brsqrt\s*\(/g, "inversesqrt(")
    .replace(/\bclamp\(abs\(v\),\s*5\.42101e-20,\s*1\.84467e\+19\)/g, "clamp(abs(v), 5.42101e-20, 1.84467e+19)")
    .replace(/\(a == 0 \|\| b == 0\) \? \(vec4\)0 : a \* b/g, "vec4(a.x == 0 || b.x == 0 ? 0.0 : a.x * b.x, a.y == 0 || b.y == 0 ? 0.0 : a.y * b.y, a.z == 0 || b.z == 0 ? 0.0 : a.z * b.z, a.w == 0 || b.w == 0 ? 0.0 : a.w * b.w)")
    .replace(/\(vec4\)0/g, "vec4(0.0)")
    .replace(/\bfloat4\(/g, "vec4(")
    ;
  helpers = helpers
    .replace(/\bfogp\.w\s*==\s*([123])\b/g, "fogp.w == $1.0")
    .replace(/\bmag\s*==\s*0\b/g, "mag == 0.0")
    .replace(/\?\s*spec\s*:\s*0\b/g, "? spec : 0.0")
    .replace(/\?\s*1\s*:\s*0\b/g, "? 1.0 : 0.0")
    .replace(/\bmax\((s\.[xyzw]),\s*0\)/g, "max($1, 0.0)")
    .replace(/\bmax\((\w+),\s*0\)/g, "max($1, 0.0)")
    .replace(/\breturn\s+1\s*;/g, "return 1.0;");
  helpers = helpers.replace(/\bprecise\b/g, "");
  helpers = lowerCasts(helpers);
  helpers = lowerSplats(helpers);
  helpers = decimalizeFloatLiterals(helpers);
  helpers = vectorScalarInitializers(helpers);
  if (/\b(?:float[234]|int[234]|asfloat|rsqrt|precise)\b/.test(helpers)) fail("unsupported SDK helper syntax");
  return helpers;
}

export function vertexGlsl(hlsl, regs, flat = false) {
  if (typeof hlsl !== "string" || !Number.isInteger(regs) || regs < 0 || regs > 0xffff || typeof flat !== "boolean")
    throw new TypeError("vertexGlsl expects generated HLSL, a 16-bit register mask, and optional flat flag");
  const body = block(hlsl, "VOut\\s+main\\s*\\(\\s*VIn\\s+i\\s*\\)\\s*\\{");
  // The register mask describes supplied attributes. Unused supplied inputs are valid; absent reads already use SDK defaults.
  let lowered = lowerBody(body)
    .replace(/q\.x\s*=\s*p\.x\s*\*\s*2(?:\.0)?\s*\/\s*surf\.x(?:\s*-\s*1(?:\.0)?)?\s*;/, "q.x = p.x * 2.0 / surf.x - 1.0;")
    .replace(/q\.y\s*=\s*1(?:\.0)?\s*-\s*p\.y\s*\*\s*2(?:\.0)?\s*\/\s*surf\.y\s*;/, "q.y = p.y * 2.0 / surf.y - 1.0;")
    .replace(/q\.z\s*=\s*p\.z\s*\*\s*surf\.z\s*;/, "q.z = p.z * surf.z * 2.0 - 1.0;");
  lowered = lowered.replace(/gl_Position\s*=\s*cp\s*;/, "gl_Position = vec4(cp.x, -cp.y, 2.0 * cp.z - cp.w, cp.w);");
  if (/q\.y\s*=|q\.z\s*=/.test(lowered) && (!/q\.y\s*=\s*p\.y\s*\*\s*2\.0\s*\/\s*surf\.y\s*-\s*1\.0/.test(lowered) || !/q\.z\s*=\s*p\.z\s*\*\s*surf\.z\s*\*\s*2\.0\s*-\s*1\.0/.test(lowered))) fail("unexpected SDK clip-space epilogue");
  const attrs = Array.from({ length: 16 }, (_, i) => `layout(location=${i}) in vec4 aV${i};`).join("\n");
  const quals = flat ? "flat " : "";
  const varyings = outputs.map((name) => `${["vD0", "vD1", "vB0", "vB1"].includes(name) ? quals : ""}out vec4 ${name};`).join("\n");
  const helper = lowerHelpers(hlsl);
  return `#version 300 es
precision highp float;
precision highp int;
invariant gl_Position;
uniform vec4 c[192];
uniform vec4 surf;
uniform vec4 fogp;
uniform mat4 wvp, wv, wvn;
uniform mat4 tm[4];
uniform vec4 mat[5], amb, lt[56];
${attrs}
${varyings}
out float vFog;
vec4 sat(vec4 x) { return clamp(x, 0.0, 1.0); }
float sat(float x) { return clamp(x, 0.0, 1.0); }
vec2 sat(vec2 x) { return clamp(x, 0.0, 1.0); }
vec3 sat(vec3 x) { return clamp(x, 0.0, 1.0); }
${helper}
void main() {\n${lowered}\n}\n`;
}
