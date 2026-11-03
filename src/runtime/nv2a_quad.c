/* CPU quad-fill geometry prepass for the browser renderer candidate.
 * Behavioral reference: pinned xemu pgraph/glsl/geom.c at
 * dc10b4cea96e403d30c17ed53551fdaf023bc17f, PRIM_TYPE_QUADS/POLY_MODE_FILL
 * (calc_quadz/calc_triz/emit_vertex). This is a source-owned candidate, not
 * original-hardware fidelity. Build with -ffp-contract=off; fast math is not
 * supported.
 */
#include "nv2a_quad.h"
#include <float.h>
#include <math.h>
#include <string.h>

#ifdef __FAST_MATH__
#error The geometry prepass must not use fast math
#endif

_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 &&
               FLT_MAX_EXP == 128, "The geometry ABI requires binary32");
_Static_assert(sizeof(FFNv2aPostVertex) == 42 * sizeof(float), "Post-vertex layout changed");
_Static_assert(sizeof(FFNv2aRasterVertex) == 51 * sizeof(float), "Raster-vertex layout changed");

static float multiply_add(float a, float b, float c, uint32_t arithmetic)
{
    if (arithmetic == FF_NV2A_FUSED) return fmaf(a, b, c);
    volatile float product = a * b;
    return product + c;
}

static float difference_of_products(float a, float b, float c, float d, uint32_t arithmetic)
{
    float product = c * d;
    float residual = multiply_add(-c, d, product, arithmetic);
    float difference = multiply_add(a, b, -product, arithmetic);
    return difference + residual;
}

static void calc_triz(const FFNv2aPostVertex input[4], unsigned i0, unsigned i1, unsigned i2,
                      uint32_t perspective_depth, uint32_t arithmetic,
                      float rows[3][4], float *slope)
{
    const float *a = input[i0].position;
    const float *b = input[i1].position;
    const float *c = input[i2].position;
    const float ux = b[0] - a[0], uy = b[1] - a[1];
    const float vx = c[0] - a[0], vy = c[1] - a[1];
    float bx, by;
    if (perspective_depth) {
        bx = (a[3] - b[3]) / (b[3] * a[3]);
        by = (a[3] - c[3]) / (c[3] * a[3]);
    } else {
        bx = b[2] - a[2];
        by = c[2] - a[2];
    }
    const float determinant = difference_of_products(ux, vy, vx, uy, arithmetic);
    float dx = difference_of_products(bx, vy, by, uy, arithmetic) / determinant;
    float dy = difference_of_products(by, ux, bx, vx, arithmetic) / determinant;
    dx = fabsf(dx);
    dy = fabsf(dy);
    float result = dx < dy ? dy : dx;
    if (!isfinite(result)) result = 0.0f;
    memcpy(rows[0], a, sizeof(rows[0]));
    memcpy(rows[1], b, sizeof(rows[1]));
    memcpy(rows[2], c, sizeof(rows[2]));
    *slope = result;
}

int ff_nv2a_expand_quad(const FFNv2aPostVertex input[4],
                        const FFNv2aQuadState *state,
                        FFNv2aRasterVertex output[6], uint32_t arithmetic)
{
    if (!input || !state || !output || state->smooth > 1 ||
        state->perspective_depth > 1 || arithmetic > FF_NV2A_SEPARATE) {
        return -1;
    }
    float pz[3][4], pz2[3][4];
    float dz, dz2;
    calc_triz(input, 0, 1, 2, state->perspective_depth, arithmetic, pz, &dz);
    calc_triz(input, 0, 2, 3, state->perspective_depth, arithmetic, pz2, &dz2);
    static const unsigned emit[6] = { 1, 2, 0, 2, 3, 0 };

    FFNv2aRasterVertex expanded[6];
    for (unsigned i = 0; i < 6; ++i) {
        const unsigned index = emit[i];
        const unsigned color_index = state->smooth ? index : 3u;
        const float (*rows)[4] = i < 3 ? (const float (*)[4])pz : (const float (*)[4])pz2;
        memcpy(expanded[i].clip, input[index].clip, sizeof(expanded[i].clip));
        expanded[i].point_size = input[index].point_size;
        memcpy(expanded[i].color, input[color_index].color, sizeof(expanded[i].color));
        expanded[i].fog = input[index].fog;
        memcpy(expanded[i].texcoord, input[index].texcoord, sizeof(expanded[i].texcoord));
        for (unsigned j = 0; j < 3; ++j)
            memcpy(expanded[i].triangle_position[j], rows[j], sizeof(expanded[i].triangle_position[j]));
        expanded[i].depth_slope = i < 3 ? dz : dz2;
    }
    memcpy(output, expanded, sizeof(expanded));
    return 0;
}
