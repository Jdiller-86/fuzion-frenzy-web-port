#ifndef FF_NV2A_QUAD_H
#define FF_NV2A_QUAD_H

#include "nv2a_triangle.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Quad fill state from the pinned public xemu geometry generator. */
typedef struct FFNv2aQuadState {
    uint32_t smooth;
    uint32_t perspective_depth;
} FFNv2aQuadState;

/* Expand one four-vertex quad into the two triangle-strip triangles the shader
 * emits (1,2,0 and 2,3,0), preserving attribute forwarding, provoking colors,
 * the triangle position rows and per-triangle depth slope. Arithmetic selects
 * the explicit fused/separate host profile.
 * Returns 0 on success, -1 for null pointers or invalid state without writing.
 */
int ff_nv2a_expand_quad(const FFNv2aPostVertex input[4],
                        const FFNv2aQuadState *state,
                        FFNv2aRasterVertex output[6], uint32_t arithmetic);

#ifdef __cplusplus
}
#endif
#endif
