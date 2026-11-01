/* CPU geometry prepass for the browser renderer candidate.
 * Behavioral reference: pinned xemu pgraph/glsl/geom.c at dc10b4cea96e403d30c17ed53551fdaf023bc17f.
 * Preserves forwarded attributes, provoking colors, position triples and depth slope.
 * Build with -ffp-contract=off; implicit contraction/fast math is not supported.
 */
#include "nv2a_triangle.h"
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

int ff_nv2a_expand_triangle(const FFNv2aPostVertex input[3],
                            const FFNv2aTriangleState *state,
                            FFNv2aRasterVertex output[3], uint32_t arithmetic)
{
    if (!input || !state || !output || state->smooth > 1 ||
        state->first_provoking > 1 || state->perspective_depth > 1 || state->rotation > 2 ||
        arithmetic > FF_NV2A_SEPARATE) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const unsigned *indices = order[state->rotation];
    const float *a = input[indices[0]].position;
    const float *b = input[indices[1]].position;
    const float *c = input[indices[2]].position;
    float ux = b[0] - a[0], uy = b[1] - a[1];
    float vx = c[0] - a[0], vy = c[1] - a[1];
    float bx, by;
    if (state->perspective_depth) {
        bx = (a[3] - b[3]) / (b[3] * a[3]);
        by = (a[3] - c[3]) / (c[3] * a[3]);
    } else {
        bx = b[2] - a[2];
        by = c[2] - a[2];
    }
    float determinant = difference_of_products(ux, vy, vx, uy, arithmetic);
    float dx = difference_of_products(bx, vy, by, uy, arithmetic) / determinant;
    float dy = difference_of_products(by, ux, bx, vx, arithmetic) / determinant;
    dx = fabsf(dx);
    dy = fabsf(dy);
    float slope = dx < dy ? dy : dx;
    if (!isfinite(slope)) slope = 0.0f;

    FFNv2aRasterVertex expanded[3];
    for (unsigned i = 0; i < 3; ++i) {
        unsigned index = indices[i];
        unsigned color_index = state->smooth ? index : indices[state->first_provoking ? 0 : 2];
        memcpy(expanded[i].clip, input[index].clip, sizeof(expanded[i].clip));
        expanded[i].point_size = input[index].point_size;
        memcpy(expanded[i].color, input[color_index].color, sizeof(expanded[i].color));
        expanded[i].fog = input[index].fog;
        memcpy(expanded[i].texcoord, input[index].texcoord, sizeof(expanded[i].texcoord));
        for (unsigned j = 0; j < 3; ++j) {
            memcpy(expanded[i].triangle_position[j], input[indices[j]].position,
                   sizeof(expanded[i].triangle_position[j]));
        }
        expanded[i].depth_slope = slope;
    }
    memcpy(output, expanded, sizeof(expanded));
    return 0;
}
