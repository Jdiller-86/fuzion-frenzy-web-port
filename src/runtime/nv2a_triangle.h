#ifndef FF_NV2A_TRIANGLE_H
#define FF_NV2A_TRIANGLE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Post-vertex-stage input. This adapter currently expands filled triangles. */
typedef struct FFNv2aPostVertex {
    float clip[4];
    float point_size;
    float color[4][4];
    float fog;
    float texcoord[4][4];
    float position[4];
} FFNv2aPostVertex;

typedef struct FFNv2aRasterVertex {
    float clip[4];
    float point_size;
    float color[4][4];
    float fog;
    float texcoord[4][4];
    float triangle_position[3][4];
    float depth_slope;
} FFNv2aRasterVertex;

typedef struct FFNv2aTriangleState {
    uint32_t smooth;
    uint32_t first_provoking;
    uint32_t perspective_depth;
    uint32_t rotation;
} FFNv2aTriangleState;

/* PRIM_TYPE_POLYGON state. The pinned fill branch always uses the rotated third
 * vertex as the flat provoking vertex, so there is no first/last selection.
 * The line branch receives two-vertex line segments from the host and ignores
 * rotation.
 */
typedef struct FFNv2aPolygonState {
    uint32_t smooth;
    uint32_t perspective_depth;
    uint32_t rotation;
} FFNv2aPolygonState;

/* Host shader implementations can fuse or separate multiply-add operations.
 * The caller must explicitly select its measured reference profile.
 */
enum FFNv2aArithmetic {
    FF_NV2A_FUSED = 0,
    FF_NV2A_SEPARATE = 1
};

/* Returns 0 on success, -1 for null pointers/invalid state/profile without writing output.
 * No guest code, device emulation or rasterization is performed here.
 */
int ff_nv2a_expand_triangle(const FFNv2aPostVertex input[3],
                            const FFNv2aTriangleState *state,
                            FFNv2aRasterVertex output[3], uint32_t arithmetic);

/* Polygon LINE mode: three line primitives (0-1, 1-2, 2-0 in the selected
 * rotation) sharing the triangle's depth slope. Emits six raster vertices in
 * that order, each line carrying the shader's rotated helper row. Flat color
 * uses the triangle's provoking index, smooth color follows each input.
 * Returns 0 on success, -1 for null pointers/invalid state/profile without
 * writing output.
 */
int ff_nv2a_expand_triangle_lines(const FFNv2aPostVertex input[3],
                                  const FFNv2aTriangleState *state,
                                  FFNv2aRasterVertex output[6], uint32_t arithmetic);

/* Polygon POINT mode: one point per rotated triangle vertex, each repeating its
 * position row three times and carrying the triangle's depth slope.
 * Returns 0 on success, -1 for null pointers/invalid state/profile without
 * writing output.
 */
int ff_nv2a_expand_triangle_points(const FFNv2aPostVertex input[3],
                                   const FFNv2aTriangleState *state,
                                   FFNv2aRasterVertex output[3], uint32_t arithmetic);

/* PRIM_TYPE_POLYGON fill: emits the three rotated vertices with the triangle
 * depth slope. The pinned branch always takes the rotated third vertex as the
 * flat provoking color; smooth color follows each emitted input.
 * Returns 0 on success, -1 for null pointers/invalid state/profile without
 * writing output.
 */
int ff_nv2a_expand_polygon_fill(const FFNv2aPostVertex input[3],
                                const FFNv2aPolygonState *state,
                                FFNv2aRasterVertex output[3], uint32_t arithmetic);

/* PRIM_TYPE_POLYGON line: one two-vertex segment with the rotated helper row,
 * a zero depth slope and flat provoking color 0. The generator documents that
 * its line input has no triangle plane, so no slope is computed. `rotation` and
 * `perspective_depth` are validated but unused.
 * Returns 0 on success, -1 for null pointers/invalid state without writing
 * output.
 */
int ff_nv2a_expand_polygon_line(const FFNv2aPostVertex input[2],
                                const FFNv2aPolygonState *state,
                                FFNv2aRasterVertex output[2]);

#ifdef __cplusplus
}
#endif
#endif
