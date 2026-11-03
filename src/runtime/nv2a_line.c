/* CPU line-geometry prepass for the browser renderer candidate.
 * Behavioral reference: pinned xemu pgraph/glsl/geom.c at
 * dc10b4cea96e403d30c17ed53551fdaf023bc17f, emit_line/emit_vertex for
 * PRIM_TYPE_LINES. This is a source-owned candidate, not original-hardware
 * fidelity. Build with -ffp-contract=off; fast math is not supported.
 */
#include "nv2a_line.h"
#include <float.h>
#include <string.h>

#ifdef __FAST_MATH__
#error The geometry prepass must not use fast math
#endif

_Static_assert(sizeof(float) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24 &&
               FLT_MAX_EXP == 128, "The geometry ABI requires binary32");
_Static_assert(sizeof(FFNv2aPostVertex) == 42 * sizeof(float), "Post-vertex layout changed");
_Static_assert(sizeof(FFNv2aRasterVertex) == 51 * sizeof(float), "Raster-vertex layout changed");

int ff_nv2a_expand_line(const FFNv2aPostVertex input[2],
                        const FFNv2aLineState *state,
                        FFNv2aRasterVertex output[2])
{
    if (!input || !state || !output || state->smooth > 1 || state->first_provoking > 1) {
        return -1;
    }
    const float *v0 = input[0].position;
    const float *v1 = input[1].position;
    const float delta_x = v1[0] - v0[0];
    const float delta_y = v1[1] - v0[1];
    float rotated[4] = { -delta_y + v0[0], delta_x + v0[1], v0[2], v0[3] };
    const unsigned flat_color = state->first_provoking ? 0u : 1u;

    FFNv2aRasterVertex expanded[2];
    for (unsigned i = 0; i < 2; ++i) {
        const unsigned color_index = state->smooth ? i : flat_color;
        memcpy(expanded[i].clip, input[i].clip, sizeof(expanded[i].clip));
        expanded[i].point_size = input[i].point_size;
        memcpy(expanded[i].color, input[color_index].color, sizeof(expanded[i].color));
        expanded[i].fog = input[i].fog;
        memcpy(expanded[i].texcoord, input[i].texcoord, sizeof(expanded[i].texcoord));
        memcpy(expanded[i].triangle_position[0], v0, sizeof(expanded[i].triangle_position[0]));
        memcpy(expanded[i].triangle_position[1], v1, sizeof(expanded[i].triangle_position[1]));
        memcpy(expanded[i].triangle_position[2], rotated, sizeof(expanded[i].triangle_position[2]));
        expanded[i].depth_slope = 0.0f;
    }
    memcpy(output, expanded, sizeof(expanded));
    return 0;
}
