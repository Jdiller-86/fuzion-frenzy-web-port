// Generic fixed-function vertex fallback. Runtime key fields mirror gpu::FixedKey
// and the exact FixedVertexHlsl arguments; geometry and constants remain supplied
// by the normal SDK draw path.
export function fixedFallbackVertexGlsl(flat = false) {
  if (typeof flat !== 'boolean') throw new TypeError('fixedFallbackVertexGlsl expects a flat flag');
  const color = flat ? 'flat ' : '';
  const attributes = Array.from({ length: 16 }, (_, i) => `layout(location=${i}) in vec4 aV${i};`).join('\n');
  const cases = Array.from({ length: 16 }, (_, i) =>
    `    case ${i}: return (fallbackVertexKey[0] & (1u << ${i}u)) != 0u ? ((fallbackVertexKey[10] & (1u << ${i}u)) != 0u ? aV${i}.zyxw : aV${i}) : fallbackDefault;`).join('\n');
  return `#version 300 es
precision highp float;
precision highp int;
invariant gl_Position;
uniform highp uint fallbackVertexKey[11];
uniform vec4 surf, fogp;
uniform mat4 wvp, wv, wvn, tm[4];
uniform vec4 mat[5], amb, lt[56];
${attributes}
${color}out vec4 vD0;
${color}out vec4 vD1;
${color}out vec4 vB0;
${color}out vec4 vB1;
out vec4 vTc0;
out vec4 vTc1;
out vec4 vTc2;
out vec4 vTc3;
out float vFog;
vec4 readV(int reg, vec4 fallbackDefault) {
  switch (reg) {
${cases}
    default: return fallbackDefault;
  }
}
float magclamp(float v) {
  float m = clamp(abs(v), 5.42101e-20, 1.84467e+19);
  return v < 0.0 ? -m : m;
}
float fogfactor(float d) {
  if (fogp.w == 3.0) return (fogp.y - d) / (fogp.y - fogp.x);
  if (fogp.w == 1.0) return exp(-d * fogp.z);
  if (fogp.w == 2.0) return exp(-(d * fogp.z) * (d * fogp.z));
  return 1.0;
}
vec4 finishPosition(vec4 p) {
  p.w = magclamp(p.w);
  if (abs(p.x - (surf.x - 0.5)) < 1e-3) p.x = surf.x;
  if (abs(p.y - (surf.y - 0.5)) < 1e-3) p.y = surf.y;
  vec3 q;
  q.x = p.x * 2.0 / surf.x - 1.0;
  q.y = p.y * 2.0 / surf.y - 1.0;
  q.z = p.z * surf.z * 2.0 - 1.0;
  q *= p.w;
  return vec4(q, p.w);
}
vec4 finishClip(vec4 cp) {
  vec2 sp = (vec2(cp.x, -cp.y) / cp.w + 1.0) * 0.5 * surf.xy;
  if (abs(sp.x - (surf.x - 0.5)) < 1e-3) cp.x = cp.w;
  if (abs(sp.y - (surf.y - 0.5)) < 1e-3) cp.y = -cp.w;
  return vec4(cp.x, -cp.y, 2.0 * cp.z - cp.w, cp.w);
}
vec4 texcoord(int stage, vec3 vp, vec3 vn, bool pretransformed) {
  uint u = fallbackVertexKey[4 + stage];
  uint setIndex = u & 3u;
  uint gen = (u >> 4u) & 7u;
  uint count = (u >> 8u) & 7u;
  uint size = (u >> 12u) & 7u;
  vec4 inputCoord = readV(9 + int(setIndex), vec4(0.0, 0.0, 0.0, 1.0));
  if (pretransformed || (gen == 0u && count == 0u)) return inputCoord;
  vec4 coord;
  if (gen == 1u) coord = vec4(vn, 1.0);
  else if (gen == 2u) coord = vec4(vp, 1.0);
  else if (gen == 3u) coord = vec4(reflect(normalize(vp), vn), 1.0);
  else if (size == 1u) coord = vec4(inputCoord.x, 1.0, 0.0, 0.0);
  else if (size == 2u) coord = vec4(inputCoord.xy, 1.0, 0.0);
  else if (size == 3u) coord = vec4(inputCoord.xyz, 1.0);
  else coord = inputCoord;
  if (count == 0u) return coord;
  vec4 tc = coord * tm[stage];
  if ((u & 0x800u) != 0u) return vec4(tc.xy, tc.z, tc[int(min(count, 4u) - 1u)]);
  return vec4(tc.xy, count >= 3u ? tc.z : 0.0, 1.0);
}
void main() {
  uint regs = fallbackVertexKey[0];
  bool pretransformed = fallbackVertexKey[1] != 0u;
  uint light = fallbackVertexKey[2];
  vec4 oPos = vec4(0.0), oD0 = vec4(0.0, 0.0, 0.0, 1.0), oD1 = vec4(0.0);
  vec4 oB0 = vec4(0.0, 0.0, 0.0, 1.0), oB1 = vec4(0.0), oFog = vec4(0.0);
  vec4 v3 = readV(3, vec4(1.0)), v4 = readV(4, vec4(0.0));
  vec3 vp = vec3(0.0), vn = vec3(0.0);
  if (pretransformed) {
    vec4 rhw = readV(0, vec4(0.0, 0.0, 0.0, 1.0));
    float w = rhw.w == 0.0 ? 1.0 : 1.0 / rhw.w;
    oPos = vec4(rhw.xy, rhw.z / surf.z, w);
  } else {
    vec4 ip = vec4(readV(0, vec4(0.0, 0.0, 0.0, 1.0)).xyz, 1.0);
    vec4 cp = ip * wvp;
    vp = (ip * wv).xyz;
    vn = (vec4(readV(2, vec4(0.0)).xyz, 0.0) * wvn).xyz;
    if ((light & 8u) != 0u) vn = dot(vn, vn) > 0.0 ? normalize(vn) : vn;
    bool lit = (light & 1u) != 0u;
    if (lit) {
      uint diffuseMode = (light >> 8u) & 3u;
      uint specularMode = (light >> 10u) & 3u;
      uint ambientMode = (light >> 12u) & 3u;
      uint emissiveMode = (light >> 14u) & 3u;
      vec4 md = diffuseMode == 1u && (regs & 8u) != 0u ? readV(3, vec4(1.0)) : mat[0];
      if (diffuseMode == 2u && (regs & 16u) != 0u) md = readV(4, vec4(0.0));
      vec4 ms = specularMode == 2u && (regs & 16u) != 0u ? readV(4, vec4(0.0)) : mat[2];
      if (specularMode == 1u && (regs & 8u) != 0u) ms = readV(3, vec4(1.0));
      vec4 ma = ambientMode == 1u && (regs & 8u) != 0u ? readV(3, vec4(1.0)) : mat[1];
      if (ambientMode == 2u && (regs & 16u) != 0u) ma = readV(4, vec4(0.0));
      vec4 me = emissiveMode == 1u && (regs & 8u) != 0u ? readV(3, vec4(1.0)) : mat[3];
      if (emissiveMode == 2u && (regs & 16u) != 0u) me = readV(4, vec4(0.0));
      vec3 la = vec3(0.0), ld = vec3(0.0), ls = vec3(0.0);
      vec3 eye = (light & 4u) != 0u ? -normalize(vp) : vec3(0.0, 0.0, 1.0);
      uint types = fallbackVertexKey[3];
      for (int n = 0; n < 8; ++n) {
        uint type = (types >> uint(n * 2)) & 3u;
        if (type == 0u) continue;
        int b = n * 7;
        vec3 l;
        float att = 1.0;
        if (type == 3u) l = -normalize(lt[b + 4].xyz);
        else {
          vec3 d = lt[b + 3].xyz - vp;
          float dist = length(d);
          l = d / max(dist, 1e-6);
          att = dist > lt[b + 3].w ? 0.0 : 1.0 / max(lt[b + 5].x + lt[b + 5].y * dist + lt[b + 5].z * dist * dist, 1e-6);
          if (type == 2u) {
            float rho = dot(-l, normalize(lt[b + 4].xyz));
            att *= rho > lt[b + 6].x ? 1.0 : rho <= lt[b + 6].y ? 0.0 : pow(clamp((rho - lt[b + 6].y) / (lt[b + 6].x - lt[b + 6].y), 0.0, 1.0), lt[b + 4].w);
          }
        }
        la += att * lt[b + 2].rgb;
        float nl = dot(vn, l);
        ld += att * lt[b].rgb * max(nl, 0.0);
        if ((light & 2u) != 0u && nl > 0.0) ls += att * lt[b + 1].rgb * pow(max(dot(vn, normalize(l + eye)), 0.0), mat[4].x);
      }
      oD0 = vec4(me.rgb + ma.rgb * (amb.rgb + la) + md.rgb * ld, md.a);
      oD1 = (light & 2u) != 0u ? vec4(ms.rgb * ls, ms.a) : vec4(0.0);
    } else {
      oD0 = v3;
      oD1 = v4;
    }
    oB0 = oD0;
    oB1 = oD1;
    oPos = cp;
  }
  if (pretransformed) { oD0 = v3; oD1 = v4; oB0 = oD0; oB1 = oD1; }
  vec4 t0 = texcoord(0, vp, vn, pretransformed);
  vec4 t1 = texcoord(1, vp, vn, pretransformed);
  vec4 t2 = texcoord(2, vp, vn, pretransformed);
  vec4 t3 = texcoord(3, vp, vn, pretransformed);
  oFog = readV(5, vec4(0.0));
  vec4 clip = pretransformed ? finishPosition(oPos) : finishClip(oPos);
  gl_Position = clip;
  vD0 = clamp(oD0, 0.0, 1.0); vD1 = clamp(oD1, 0.0, 1.0);
  vB0 = clamp(oB0, 0.0, 1.0); vB1 = clamp(oB1, 0.0, 1.0);
  vTc0 = t0; vTc1 = t1; vTc2 = t2; vTc3 = t3;
  vFog = fogfactor(pretransformed ? oFog.x : abs(oPos.z));
}
`;
}
