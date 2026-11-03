/* CPU line-geometry prepass for the browser renderer candidate.
 * Behavioral reference: pinned xemu pgraph/glsl/geom.c at
 * dc10b4cea96e403d30c17ed53551fdaf023bc17f, emit_line/emit_vertex for
 * PRIM_TYPE_LINES. This is a source-owned candidate, not original-hardware
 * fidelity. Build with -ffp-contract=off; fast math is not supported.
 */
#include "nv2a_line.h"
#include <float.h>
#include <stdint.h>
#include <string.h>

#ifdef __FAST_MATH__
#error The geometry prepass must not use fast math
#endif

_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 &&
               FLT_MAX_EXP == 128, "The geometry ABI requires binary32");
_Static_assert(sizeof(FFNv2aPostVertex) == 42 * sizeof(float), "Post-vertex layout changed");
_Static_assert(sizeof(FFNv2aRasterVertex) == 51 * sizeof(float), "Raster-vertex layout changed");

/* One host-supplied line pair through the shader's emit_line/emit_vertex body. */
static void emit_line_pair(const FFNv2aPostVertex *a, const FFNv2aPostVertex *b,
                           const float (*flat_color)[4], uint32_t smooth,
                           FFNv2aRasterVertex output[2])
{
    const float *v0 = a->position;
    const float *v1 = b->position;
    const float delta_x = v1[0] - v0[0];
    const float delta_y = v1[1] - v0[1];
    const float rotated[4] = { -delta_y + v0[0], delta_x + v0[1], v0[2], v0[3] };
    const FFNv2aPostVertex *source[2] = { a, b };

    for (unsigned i = 0; i < 2; ++i) {
        memcpy(output[i].clip, source[i]->clip, sizeof(output[i].clip));
        output[i].point_size = source[i]->point_size;
        memcpy(output[i].color, smooth ? source[i]->color : flat_color,
               sizeof(output[i].color));
        output[i].fog = source[i]->fog;
        memcpy(output[i].texcoord, source[i]->texcoord, sizeof(output[i].texcoord));
        memcpy(output[i].triangle_position[0], v0, sizeof(output[i].triangle_position[0]));
        memcpy(output[i].triangle_position[1], v1, sizeof(output[i].triangle_position[1]));
        memcpy(output[i].triangle_position[2], rotated, sizeof(output[i].triangle_position[2]));
        output[i].depth_slope = 0.0f;
    }
}

static int invalid_state(const FFNv2aLineState *state)
{
    return !state || state->smooth > 1 || state->first_provoking > 1;
}

int ff_nv2a_expand_line(const FFNv2aPostVertex input[2],
                        const FFNv2aLineState *state,
                        FFNv2aRasterVertex output[2])
{
    if (!input || !output || invalid_state(state)) {
        return -1;
    }
    emit_line_pair(&input[0], &input[1],
                   input[state->first_provoking ? 0 : 1].color, state->smooth,
                   output);
    return 0;
}

int ff_nv2a_expand_line_strip(const FFNv2aPostVertex *input, uint32_t count,
                              const FFNv2aLineState *state,
                              FFNv2aRasterVertex *output, uint32_t capacity)
{
    if (!input || !output || invalid_state(state) || count < 2) {
        return -1;
    }
    const uint64_t required = 2ull * ((uint64_t)count - 1u);
    if (required > capacity) {
        return -1;
    }
#if SIZE_MAX <= UINT32_MAX
    if ((uint64_t)count > SIZE_MAX / sizeof(FFNv2aPostVertex) ||
        required > SIZE_MAX / sizeof(FFNv2aRasterVertex)) {
        return -1;
    }
#endif
    const uint32_t flat_offset = state->first_provoking ? 0u : 1u;
    for (uint32_t i = 0; i + 1u < count; ++i) {
        emit_line_pair(&input[i], &input[i + 1u],
                       input[i + flat_offset].color, state->smooth,
                       &output[i * 2u]);
    }
    return 0;
}

int ff_nv2a_expand_line_loop(const FFNv2aPostVertex *input, uint32_t count,
                             const FFNv2aLineState *state,
                             FFNv2aRasterVertex *output, uint32_t capacity)
{
    if (!input || !output || invalid_state(state) || count < 2) {
        return -1;
    }
    const uint64_t required = 2ull * count;
    if (required > capacity) {
        return -1;
    }
#if SIZE_MAX <= UINT32_MAX
    if ((uint64_t)count > SIZE_MAX / sizeof(FFNv2aPostVertex) ||
        required > SIZE_MAX / sizeof(FFNv2aRasterVertex)) {
        return -1;
    }
#endif
    for (uint32_t i = 0; i < count; ++i) {
        const uint32_t next = i + 1u == count ? 0u : i + 1u;
        emit_line_pair(&input[i], &input[next],
                       input[state->first_provoking ? i : next].color,
                       state->smooth, &output[i * 2u]);
    }
    return 0;
}
