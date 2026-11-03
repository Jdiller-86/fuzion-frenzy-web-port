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

static int invalid_state(const FFNv2aTriangleState *state, uint32_t arithmetic)
{
    return !state || state->smooth > 1 || state->first_provoking > 1 ||
           state->perspective_depth > 1 || state->rotation > 2 ||
           arithmetic > FF_NV2A_SEPARATE;
}

/* Shader calc_triz: shared by the fill, line and point polygon modes. */
static void calc_triz(const FFNv2aPostVertex input[3], const unsigned indices[3],
                      uint32_t perspective_depth, uint32_t arithmetic,
                      float rows[3][4], float *slope)
{
    const float *a = input[indices[0]].position;
    const float *b = input[indices[1]].position;
    const float *c = input[indices[2]].position;
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
static void emit_vertex(const FFNv2aPostVertex *input, unsigned index,
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

/* Shader emit_line's 90-degree rotated interpolation helper row. */
static void rotated_row(const float *v0, const float *v1, float out[4])
{
    const float delta_x = v1[0] - v0[0];
    const float delta_y = v1[1] - v0[1];
    out[0] = -delta_y + v0[0];
    out[1] = delta_x + v0[1];
    out[2] = v0[2];
    out[3] = v0[3];
}

/* Shader emit_line for polygon LINE modes: two vertices sharing the rotated
 * interpolation helper row and the triangle's depth slope. */
static void emit_line(const FFNv2aPostVertex input[3], unsigned i0, unsigned i1,
                      unsigned flat_color, uint32_t smooth, float depth_slope,
                      FFNv2aRasterVertex *output)
{
    const float *v0 = input[i0].position;
    const float *v1 = input[i1].position;
    float rows[3][4];
    memcpy(rows[0], v0, sizeof(rows[0]));
    memcpy(rows[1], v1, sizeof(rows[1]));
    rotated_row(v0, v1, rows[2]);
    const unsigned indices[2] = { i0, i1 };
    for (unsigned i = 0; i < 2; ++i) {
        const unsigned index = indices[i];
        emit_vertex(input, index, smooth ? index : flat_color, rows,
                    depth_slope, &output[i]);
    }
}

/* Shared shader fill body for the triangle, strip, fan and polygon branches. */
static void emit_fill(const FFNv2aPostVertex input[3], const unsigned *indices,
                      uint32_t perspective_depth, uint32_t arithmetic,
                      unsigned flat_color, uint32_t smooth,
                      FFNv2aRasterVertex output[3])
{
    float rows[3][4], slope;
    calc_triz(input, indices, perspective_depth, arithmetic, rows, &slope);
    FFNv2aRasterVertex expanded[3];
    for (unsigned i = 0; i < 3; ++i) {
        const unsigned index = indices[i];
        emit_vertex(input, index, smooth ? index : flat_color, rows, slope,
                    &expanded[i]);
    }
    memcpy(output, expanded, sizeof(expanded));
}

int ff_nv2a_expand_triangle(const FFNv2aPostVertex input[3],
                            const FFNv2aTriangleState *state,
                            FFNv2aRasterVertex output[3], uint32_t arithmetic)
{
    if (!input || !output || invalid_state(state, arithmetic)) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const unsigned *indices = order[state->rotation];
    const unsigned flat_color = state->first_provoking ? indices[0] : indices[2];
    emit_fill(input, indices, state->perspective_depth, arithmetic, flat_color,
              state->smooth, output);
    return 0;
}

int ff_nv2a_expand_triangle_strip(const FFNv2aPostVertex input[3],
                                  const FFNv2aTriangleState *state,
                                  FFNv2aRasterVertex output[3], uint32_t arithmetic,
                                  uint32_t primitive_id)
{
    if (!input || !output || invalid_state(state, arithmetic) ||
        state->rotation_odd > 2) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const uint32_t parity = primitive_id & 1u;
    const unsigned *indices = order[parity ? state->rotation_odd : state->rotation];
    const unsigned flat_color = state->first_provoking ? indices[0]
                                                       : indices[2 - parity];
    emit_fill(input, indices, state->perspective_depth, arithmetic, flat_color,
              state->smooth, output);
    return 0;
}

int ff_nv2a_expand_triangle_fan(const FFNv2aPostVertex input[3],
                                const FFNv2aTriangleState *state,
                                FFNv2aRasterVertex output[3], uint32_t arithmetic)
{
    if (!input || !output || invalid_state(state, arithmetic)) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const unsigned *indices = order[state->rotation];
    const unsigned flat_color = state->first_provoking ? indices[0] : indices[1];
    emit_fill(input, indices, state->perspective_depth, arithmetic, flat_color,
              state->smooth, output);
    return 0;
}

int ff_nv2a_expand_polygon_fill(const FFNv2aPostVertex input[3],
                                const FFNv2aPolygonState *state,
                                FFNv2aRasterVertex output[3], uint32_t arithmetic)
{
    if (!input || !output || !state || state->smooth > 1 ||
        state->perspective_depth > 1 || state->rotation > 2 ||
        arithmetic > FF_NV2A_SEPARATE) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const unsigned *indices = order[state->rotation];
    /* The pinned polygon fill branch always takes the rotated third vertex. */
    emit_fill(input, indices, state->perspective_depth, arithmetic, indices[2],
              state->smooth, output);
    return 0;
}

int ff_nv2a_expand_polygon_line(const FFNv2aPostVertex input[2],
                                const FFNv2aPolygonState *state,
                                FFNv2aRasterVertex output[2])
{
    if (!input || !state || !output || state->smooth > 1 ||
        state->perspective_depth > 1 || state->rotation > 2) {
        return -1;
    }
    float rows[3][4];
    memcpy(rows[0], input[0].position, sizeof(rows[0]));
    memcpy(rows[1], input[1].position, sizeof(rows[1]));
    rotated_row(input[0].position, input[1].position, rows[2]);

    FFNv2aRasterVertex expanded[2];
    for (unsigned i = 0; i < 2; ++i) {
        /* The line branch fixes the flat provoking vertex to index 0. */
        emit_vertex(input, i, state->smooth ? i : 0u, rows, 0.0f, &expanded[i]);
    }
    memcpy(output, expanded, sizeof(expanded));
    return 0;
}

/* Shared shader LINE body for triangle, strip and fan polygon modes. */
static void expand_line_mode(const FFNv2aPostVertex input[3], const unsigned *indices,
                             unsigned flat_color, uint32_t smooth,
                             uint32_t perspective_depth, uint32_t arithmetic,
                             FFNv2aRasterVertex output[6])
{
    float rows[3][4], slope;
    calc_triz(input, indices, perspective_depth, arithmetic, rows, &slope);
    static const unsigned edges[3][2] = { {0, 1}, {1, 2}, {2, 0} };
    for (unsigned i = 0; i < 3; ++i) {
        emit_line(input, indices[edges[i][0]], indices[edges[i][1]], flat_color,
                  smooth, slope, &output[i * 2]);
    }
}

/* Shared shader POINT body for triangle, strip and fan polygon modes. */
static void expand_point_mode(const FFNv2aPostVertex input[3], const unsigned *indices,
                              unsigned flat_color, uint32_t smooth,
                              uint32_t perspective_depth, uint32_t arithmetic,
                              FFNv2aRasterVertex output[3])
{
    float rows[3][4], slope;
    calc_triz(input, indices, perspective_depth, arithmetic, rows, &slope);
    for (unsigned i = 0; i < 3; ++i) {
        const unsigned index = indices[i];
        const float point_rows[3][4] = {
            { rows[i][0], rows[i][1], rows[i][2], rows[i][3] },
            { rows[i][0], rows[i][1], rows[i][2], rows[i][3] },
            { rows[i][0], rows[i][1], rows[i][2], rows[i][3] },
        };
        emit_vertex(input, index, smooth ? index : flat_color, point_rows, slope,
                    &output[i]);
    }
}

int ff_nv2a_expand_triangle_lines(const FFNv2aPostVertex input[3],
                                  const FFNv2aTriangleState *state,
                                  FFNv2aRasterVertex output[6], uint32_t arithmetic)
{
    if (!input || !output || invalid_state(state, arithmetic)) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const unsigned *indices = order[state->rotation];
    const unsigned flat_color = state->first_provoking ? indices[0] : indices[2];
    expand_line_mode(input, indices, flat_color, state->smooth,
                     state->perspective_depth, arithmetic, output);
    return 0;
}

int ff_nv2a_expand_triangle_points(const FFNv2aPostVertex input[3],
                                   const FFNv2aTriangleState *state,
                                   FFNv2aRasterVertex output[3], uint32_t arithmetic)
{
    if (!input || !output || invalid_state(state, arithmetic)) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const unsigned *indices = order[state->rotation];
    const unsigned flat_color = state->first_provoking ? indices[0] : indices[2];
    expand_point_mode(input, indices, flat_color, state->smooth,
                      state->perspective_depth, arithmetic, output);
    return 0;
}

int ff_nv2a_expand_triangle_strip_lines(const FFNv2aPostVertex input[3],
                                        const FFNv2aTriangleState *state,
                                        FFNv2aRasterVertex output[6], uint32_t arithmetic,
                                        uint32_t primitive_id)
{
    if (!input || !output || invalid_state(state, arithmetic) ||
        state->rotation_odd > 2) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const uint32_t parity = primitive_id & 1u;
    const unsigned *indices = order[parity ? state->rotation_odd : state->rotation];
    const unsigned flat_color = state->first_provoking ? indices[0]
                                                       : indices[2 - parity];
    expand_line_mode(input, indices, flat_color, state->smooth,
                     state->perspective_depth, arithmetic, output);
    return 0;
}

int ff_nv2a_expand_triangle_strip_points(const FFNv2aPostVertex input[3],
                                         const FFNv2aTriangleState *state,
                                         FFNv2aRasterVertex output[3], uint32_t arithmetic,
                                         uint32_t primitive_id)
{
    if (!input || !output || invalid_state(state, arithmetic) ||
        state->rotation_odd > 2) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const uint32_t parity = primitive_id & 1u;
    const unsigned *indices = order[parity ? state->rotation_odd : state->rotation];
    const unsigned flat_color = state->first_provoking ? indices[0]
                                                       : indices[2 - parity];
    expand_point_mode(input, indices, flat_color, state->smooth,
                      state->perspective_depth, arithmetic, output);
    return 0;
}

int ff_nv2a_expand_triangle_fan_lines(const FFNv2aPostVertex input[3],
                                      const FFNv2aTriangleState *state,
                                      FFNv2aRasterVertex output[6], uint32_t arithmetic)
{
    if (!input || !output || invalid_state(state, arithmetic)) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const unsigned *indices = order[state->rotation];
    const unsigned flat_color = state->first_provoking ? indices[0] : indices[1];
    expand_line_mode(input, indices, flat_color, state->smooth,
                     state->perspective_depth, arithmetic, output);
    return 0;
}

int ff_nv2a_expand_triangle_fan_points(const FFNv2aPostVertex input[3],
                                       const FFNv2aTriangleState *state,
                                       FFNv2aRasterVertex output[3], uint32_t arithmetic)
{
    if (!input || !output || invalid_state(state, arithmetic)) {
        return -1;
    }
    static const unsigned order[3][3] = { {0, 1, 2}, {2, 0, 1}, {1, 2, 0} };
    const unsigned *indices = order[state->rotation];
    const unsigned flat_color = state->first_provoking ? indices[0] : indices[1];
    expand_point_mode(input, indices, flat_color, state->smooth,
                      state->perspective_depth, arithmetic, output);
    return 0;
}

/* Per-triangle raster-vertex count for a stream assembly mode. */
static int assembly_bounds(uint32_t polygon_mode, uint32_t count, uint32_t capacity,
                           uint32_t *per_triangle)
{
    switch (polygon_mode) {
        case FF_NV2A_POLYGON_FILL: *per_triangle = 3; break;
        case FF_NV2A_POLYGON_LINE: *per_triangle = 6; break;
        case FF_NV2A_POLYGON_POINT: *per_triangle = 3; break;
        default: return -1;
    }
    if (count < 3) {
        return -1;
    }
    const uint64_t required = (uint64_t)*per_triangle * ((uint64_t)count - 2u);
    if (required > capacity) {
        return -1;
    }
#if SIZE_MAX <= UINT32_MAX
    if ((uint64_t)count > SIZE_MAX / sizeof(FFNv2aPostVertex) ||
        required > SIZE_MAX / sizeof(FFNv2aRasterVertex)) {
        return -1;
    }
#endif
    return 0;
}

/* State was validated by the assembler, so the per-triangle calls cannot fail. */
static void emit_assembled(uint32_t polygon_mode, int strip,
                           const FFNv2aPostVertex input[3],
                           const FFNv2aTriangleState *state,
                           FFNv2aRasterVertex *output, uint32_t arithmetic,
                           uint32_t primitive_id)
{
    switch (polygon_mode) {
        case FF_NV2A_POLYGON_FILL:
            if (strip) {
                (void)ff_nv2a_expand_triangle_strip(input, state, output, arithmetic,
                                                    primitive_id);
            } else {
                (void)ff_nv2a_expand_triangle_fan(input, state, output, arithmetic);
            }
            break;
        case FF_NV2A_POLYGON_LINE:
            if (strip) {
                (void)ff_nv2a_expand_triangle_strip_lines(input, state, output, arithmetic,
                                                          primitive_id);
            } else {
                (void)ff_nv2a_expand_triangle_fan_lines(input, state, output, arithmetic);
            }
            break;
        case FF_NV2A_POLYGON_POINT:
            if (strip) {
                (void)ff_nv2a_expand_triangle_strip_points(input, state, output, arithmetic,
                                                           primitive_id);
            } else {
                (void)ff_nv2a_expand_triangle_fan_points(input, state, output, arithmetic);
            }
            break;
        default:
            break; /* assembly_bounds rejected every other mode */
    }
}

int ff_nv2a_assemble_triangle_strip(const FFNv2aPostVertex *input, uint32_t count,
                                    const FFNv2aTriangleState *state,
                                    FFNv2aRasterVertex *output, uint32_t capacity,
                                    uint32_t arithmetic, uint32_t polygon_mode)
{
    uint32_t per_triangle;
    if (!input || !output || invalid_state(state, arithmetic) ||
        state->rotation_odd > 2 ||
        assembly_bounds(polygon_mode, count, capacity, &per_triangle) < 0) {
        return -1;
    }
    for (uint32_t i = 0; i + 2u < count; ++i) {
        emit_assembled(polygon_mode, 1, input + i, state,
                       output + (size_t)i * per_triangle, arithmetic, i);
    }
    return 0;
}

int ff_nv2a_assemble_triangle_fan(const FFNv2aPostVertex *input, uint32_t count,
                                  const FFNv2aTriangleState *state,
                                  FFNv2aRasterVertex *output, uint32_t capacity,
                                  uint32_t arithmetic, uint32_t polygon_mode)
{
    uint32_t per_triangle;
    if (!input || !output || invalid_state(state, arithmetic) ||
        assembly_bounds(polygon_mode, count, capacity, &per_triangle) < 0) {
        return -1;
    }
    for (uint32_t i = 0; i + 2u < count; ++i) {
        const FFNv2aPostVertex triangle[3] = { input[0], input[i + 1u], input[i + 2u] };
        emit_assembled(polygon_mode, 0, triangle, state,
                       output + (size_t)i * per_triangle, arithmetic, 0);
    }
    return 0;
}
