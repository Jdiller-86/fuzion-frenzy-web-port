/* CPU quad-fill geometry prepass for the browser renderer candidate.
 * Behavioral reference: pinned xemu pgraph/glsl/geom.c at
 * dc10b4cea96e403d30c17ed53551fdaf023bc17f, PRIM_TYPE_QUADS/POLY_MODE_FILL
 * (calc_quadz/calc_triz/emit_vertex) and the LINE/POINT polygon-mode branches.
 * This is a source-owned candidate, not original-hardware fidelity. Build with
 * -ffp-contract=off; fast math is not supported.
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

static int invalid_state(const FFNv2aQuadState *state, uint32_t arithmetic)
{
    return !state || state->smooth > 1 || state->perspective_depth > 1 ||
           arithmetic > FF_NV2A_SEPARATE;
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

/* Shader emit_vertex with a caller-supplied position row set. */
static void emit_vertex(const FFNv2aPostVertex input[4], unsigned index,
                        unsigned color_index, const float rows[3][4],
                        float depth_slope, FFNv2aRasterVertex *output)
{
    memcpy(output->clip, input[index].clip, sizeof(output->clip));
    output->point_size = input[index].point_size;
    memcpy(output->color, input[color_index].color, sizeof(output->color));
    output->fog = input[index].fog;
    memcpy(output->texcoord, input[index].texcoord, sizeof(output->texcoord));
    for (unsigned j = 0; j < 3; ++j) {
        memcpy(output->triangle_position[j], rows[j], sizeof(output->triangle_position[j]));
    }
    output->depth_slope = depth_slope;
}

/* Shader emit_line for polygon LINE modes: two vertices sharing the rotated
 * interpolation helper row and the triangle's depth slope. */
static void emit_line(const FFNv2aPostVertex input[4], unsigned i0, unsigned i1,
                      unsigned flat_color, uint32_t smooth, float depth_slope,
                      FFNv2aRasterVertex *output)
{
    const float *v0 = input[i0].position;
    const float *v1 = input[i1].position;
    const float delta_x = v1[0] - v0[0];
    const float delta_y = v1[1] - v0[1];
    const float rotated[4] = { -delta_y + v0[0], delta_x + v0[1], v0[2], v0[3] };
    float rows[3][4];
    memcpy(rows[0], v0, sizeof(rows[0]));
    memcpy(rows[1], v1, sizeof(rows[1]));
    memcpy(rows[2], rotated, sizeof(rows[2]));
    const unsigned indices[2] = { i0, i1 };
    for (unsigned i = 0; i < 2; ++i) {
        const unsigned index = indices[i];
        emit_vertex(input, index, smooth ? index : flat_color, rows,
                    depth_slope, &output[i]);
    }
}

static void emit_fill(const FFNv2aPostVertex input[4], const float pz[3][4], const float pz2[3][4],
                      float dz, float dz2, uint32_t smooth, const unsigned order[6],
                      FFNv2aRasterVertex output[6])
{
    FFNv2aRasterVertex expanded[6];
    for (unsigned i = 0; i < 6; ++i) {
        const unsigned index = order[i];
        const unsigned color_index = smooth ? index : 3u;
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
}

int ff_nv2a_expand_quad(const FFNv2aPostVertex input[4],
                        const FFNv2aQuadState *state,
                        FFNv2aRasterVertex output[6], uint32_t arithmetic)
{
    if (!input || !output || invalid_state(state, arithmetic)) {
        return -1;
    }
    float pz[3][4], pz2[3][4];
    float dz, dz2;
    calc_triz(input, 0, 1, 2, state->perspective_depth, arithmetic, pz, &dz);
    calc_triz(input, 0, 2, 3, state->perspective_depth, arithmetic, pz2, &dz2);
    static const unsigned order[6] = { 1, 2, 0, 2, 3, 0 };
    emit_fill(input, (const float (*)[4])pz, (const float (*)[4])pz2, dz, dz2,
              state->smooth, order, output);
    return 0;
}

int ff_nv2a_expand_quad_strip(const FFNv2aPostVertex input[4],
                              const FFNv2aQuadState *state,
                              FFNv2aRasterVertex output[6], uint32_t arithmetic,
                              uint32_t primitive_id, uint32_t *emitted)
{
    if (!input || !output || !emitted || invalid_state(state, arithmetic)) {
        return -1;
    }
    *emitted = 0;
    if (primitive_id & 1u) return 0;
    float pz[3][4], pz2[3][4];
    float dz, dz2;
    calc_triz(input, 2, 0, 1, state->perspective_depth, arithmetic, pz, &dz);
    calc_triz(input, 2, 1, 3, state->perspective_depth, arithmetic, pz2, &dz2);
    static const unsigned order[6] = { 0, 1, 2, 2, 1, 3 };
    emit_fill(input, (const float (*)[4])pz, (const float (*)[4])pz2, dz, dz2,
              state->smooth, order, output);
    *emitted = 6;
    return 0;
}

/* Shader PRIM_TYPE_QUADS, POLY_MODE_LINE: four edges, first two using the
 * first triangle's slope and the last two using the second triangle's. */
int ff_nv2a_expand_quad_lines(const FFNv2aPostVertex input[4],
                              const FFNv2aQuadState *state,
                              FFNv2aRasterVertex output[8], uint32_t arithmetic)
{
    if (!input || !output || invalid_state(state, arithmetic)) {
        return -1;
    }
    float pz[3][4], pz2[3][4];
    float dz, dz2;
    calc_triz(input, 0, 1, 2, state->perspective_depth, arithmetic, pz, &dz);
    calc_triz(input, 0, 2, 3, state->perspective_depth, arithmetic, pz2, &dz2);
    static const unsigned edges[4][2] = { {0, 1}, {1, 2}, {2, 3}, {3, 0} };
    FFNv2aRasterVertex expanded[8];
    for (unsigned i = 0; i < 4; ++i) {
        emit_line(input, edges[i][0], edges[i][1], 3u, state->smooth,
                  i < 2 ? dz : dz2, &expanded[i * 2]);
    }
    memcpy(output, expanded, sizeof(expanded));
    return 0;
}

/* Shader PRIM_TYPE_QUADS, POLY_MODE_POINT: the first triangle contributes
 * inputs 0-2 and the fourth point repeats the second triangle's third row. */
int ff_nv2a_expand_quad_points(const FFNv2aPostVertex input[4],
                               const FFNv2aQuadState *state,
                               FFNv2aRasterVertex output[4], uint32_t arithmetic)
{
    if (!input || !output || invalid_state(state, arithmetic)) {
        return -1;
    }
    float pz[3][4], pz2[3][4];
    float dz, dz2;
    calc_triz(input, 0, 1, 2, state->perspective_depth, arithmetic, pz, &dz);
    calc_triz(input, 0, 2, 3, state->perspective_depth, arithmetic, pz2, &dz2);
    FFNv2aRasterVertex expanded[4];
    for (unsigned i = 0; i < 4; ++i) {
        const unsigned index = i;
        const float (*row)[4] = i < 3 ? (const float (*)[4])pz + i : (const float (*)[4])pz2 + 2;
        const float point_rows[3][4] = {
            { row[0][0], row[0][1], row[0][2], row[0][3] },
            { row[0][0], row[0][1], row[0][2], row[0][3] },
            { row[0][0], row[0][1], row[0][2], row[0][3] },
        };
        emit_vertex(input, index, state->smooth ? index : 3u, point_rows,
                    i < 3 ? dz : dz2, &expanded[i]);
    }
    memcpy(output, expanded, sizeof(expanded));
    return 0;
}

/* Shader PRIM_TYPE_QUAD_STRIP, POLY_MODE_LINE: odd adjacency primitives are
 * skipped; edges follow 0-1, 1-3, 3-2, 2-0 with alternating slopes. */
int ff_nv2a_expand_quad_strip_lines(const FFNv2aPostVertex input[4],
                                    const FFNv2aQuadState *state,
                                    FFNv2aRasterVertex output[8], uint32_t arithmetic,
                                    uint32_t primitive_id, uint32_t *emitted)
{
    if (!input || !output || !emitted || invalid_state(state, arithmetic)) {
        return -1;
    }
    *emitted = 0;
    if (primitive_id & 1u) return 0;
    float pz[3][4], pz2[3][4];
    float dz, dz2;
    calc_triz(input, 2, 0, 1, state->perspective_depth, arithmetic, pz, &dz);
    calc_triz(input, 2, 1, 3, state->perspective_depth, arithmetic, pz2, &dz2);
    static const unsigned edges[4][2] = { {0, 1}, {1, 3}, {3, 2}, {2, 0} };
    FFNv2aRasterVertex expanded[8];
    for (unsigned i = 0; i < 4; ++i) {
        const uint32_t first_triangle = i == 0 || i == 3;
        emit_line(input, edges[i][0], edges[i][1], 3u, state->smooth,
                  first_triangle ? dz : dz2, &expanded[i * 2]);
    }
    memcpy(output, expanded, sizeof(expanded));
    *emitted = 8;
    return 0;
}

/* Shader PRIM_TYPE_QUAD_STRIP, POLY_MODE_POINT: odd primitives are skipped;
 * the shader maps the rotated first triangle rows back to inputs 0-2. */
int ff_nv2a_expand_quad_strip_points(const FFNv2aPostVertex input[4],
                                     const FFNv2aQuadState *state,
                                     FFNv2aRasterVertex output[4], uint32_t arithmetic,
                                     uint32_t primitive_id, uint32_t *emitted)
{
    if (!input || !output || !emitted || invalid_state(state, arithmetic)) {
        return -1;
    }
    *emitted = 0;
    if (primitive_id & 1u) return 0;
    float pz[3][4], pz2[3][4];
    float dz, dz2;
    calc_triz(input, 2, 0, 1, state->perspective_depth, arithmetic, pz, &dz);
    calc_triz(input, 2, 1, 3, state->perspective_depth, arithmetic, pz2, &dz2);
    static const unsigned source_rows[4] = { 1, 2, 0, 2 };
    FFNv2aRasterVertex expanded[4];
    for (unsigned i = 0; i < 4; ++i) {
        const float (*row)[4] = i < 3 ? (const float (*)[4])pz + source_rows[i]
                                      : (const float (*)[4])pz2 + 2;
        const float point_rows[3][4] = {
            { row[0][0], row[0][1], row[0][2], row[0][3] },
            { row[0][0], row[0][1], row[0][2], row[0][3] },
            { row[0][0], row[0][1], row[0][2], row[0][3] },
        };
        emit_vertex(input, i, state->smooth ? i : 3u, point_rows,
                    i < 3 ? dz : dz2, &expanded[i]);
    }
    memcpy(output, expanded, sizeof(expanded));
    *emitted = 4;
    return 0;
}
