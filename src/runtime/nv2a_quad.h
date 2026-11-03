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

/* Quad-strip fill. primitive_id parity follows the public shader: odd adjacency
 * primitives emit nothing. On success *emitted is 0 (odd) or 6 (even); output
 * is untouched for odd primitives. Returns 0 on success, -1 for invalid input
 * without writing output.
 */
int ff_nv2a_expand_quad_strip(const FFNv2aPostVertex input[4],
                              const FFNv2aQuadState *state,
                              FFNv2aRasterVertex output[6], uint32_t arithmetic,
                              uint32_t primitive_id, uint32_t *emitted);

/* Polygon LINE mode for PRIM_TYPE_QUADS: edges 0-1 and 1-2 use the first
 * triangle's slope, edges 2-3 and 3-0 the second triangle's. Emits eight
 * raster vertices. Flat color uses input 3; smooth color follows each input.
 * Returns 0 on success, -1 for null pointers/invalid state/profile without
 * writing output.
 */
int ff_nv2a_expand_quad_lines(const FFNv2aPostVertex input[4],
                              const FFNv2aQuadState *state,
                              FFNv2aRasterVertex output[8], uint32_t arithmetic);

/* Polygon POINT mode for PRIM_TYPE_QUADS: the first triangle supplies points for
 * inputs 0-2 and input 3 repeats the second triangle's third row. Emits four
 * raster vertices. Returns 0 on success, -1 for null pointers/invalid
 * state/profile without writing output.
 */
int ff_nv2a_expand_quad_points(const FFNv2aPostVertex input[4],
                               const FFNv2aQuadState *state,
                               FFNv2aRasterVertex output[4], uint32_t arithmetic);

/* Polygon LINE mode for PRIM_TYPE_QUAD_STRIP. Odd adjacency primitives emit
 * nothing. Edges follow 0-1, 1-3, 3-2, 2-0 with alternating triangle slopes.
 * On success *emitted is 0 (odd) or 8 (even); output is untouched when odd.
 * Returns 0 on success, -1 for invalid input without writing output.
 */
int ff_nv2a_expand_quad_strip_lines(const FFNv2aPostVertex input[4],
                                    const FFNv2aQuadState *state,
                                    FFNv2aRasterVertex output[8], uint32_t arithmetic,
                                    uint32_t primitive_id, uint32_t *emitted);

/* Polygon POINT mode for PRIM_TYPE_QUAD_STRIP. Odd adjacency primitives emit
 * nothing; the rotated first-triangle rows map back to inputs 0-2. On success
 * *emitted is 0 (odd) or 4 (even); output is untouched when odd.
 * Returns 0 on success, -1 for invalid input without writing output.
 */
int ff_nv2a_expand_quad_strip_points(const FFNv2aPostVertex input[4],
                                     const FFNv2aQuadState *state,
                                     FFNv2aRasterVertex output[4], uint32_t arithmetic,
                                     uint32_t primitive_id, uint32_t *emitted);

#ifdef __cplusplus
}
#endif
#endif
