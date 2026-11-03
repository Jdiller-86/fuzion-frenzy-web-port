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
    /* Odd-parity rotation for triangle strips. Ignored by the list, fan, line
     * and point paths.
     */
    uint32_t rotation_odd;
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

/* Polygon mode selector for the whole-stream triangle assemblers. */
enum FFNv2aPolygonMode {
    FF_NV2A_POLYGON_FILL = 0,
    FF_NV2A_POLYGON_LINE = 1,
    FF_NV2A_POLYGON_POINT = 2
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

/* PRIM_TYPE_TRIANGLE_STRIP fill for one host-assembled triangle. Even
 * primitive IDs use `rotation`; odd IDs use `rotation_odd`. The flat provoking
 * vertex follows the shader: the rotated first vertex when first_provoking is
 * set, otherwise the rotated `indices[2 - (primitive_id & 1)]`. Smooth color
 * follows each emitted input.
 * Returns 0 on success, -1 for null pointers/invalid state/profile without
 * writing output.
 */
int ff_nv2a_expand_triangle_strip(const FFNv2aPostVertex input[3],
                                  const FFNv2aTriangleState *state,
                                  FFNv2aRasterVertex output[3], uint32_t arithmetic,
                                  uint32_t primitive_id);

/* PRIM_TYPE_TRIANGLE_FAN fill for one host-assembled triangle. The shader uses
 * one fan rotation for every triangle; the flat provoking vertex is the rotated
 * first vertex when first_provoking is set, otherwise the rotated second
 * vertex. Smooth color follows each emitted input.
 * Returns 0 on success, -1 for null pointers/invalid state/profile without
 * writing output.
 */
int ff_nv2a_expand_triangle_fan(const FFNv2aPostVertex input[3],
                                const FFNv2aTriangleState *state,
                                FFNv2aRasterVertex output[3], uint32_t arithmetic);

/* Polygon LINE mode for PRIM_TYPE_TRIANGLE_STRIP. Uses the same even/odd
 * rotation and provoking selection as the strip fill, then emits the three
 * rotated edges sharing the triangle's depth slope.
 * Returns 0 on success, -1 for null pointers/invalid state/profile without
 * writing output.
 */
int ff_nv2a_expand_triangle_strip_lines(const FFNv2aPostVertex input[3],
                                        const FFNv2aTriangleState *state,
                                        FFNv2aRasterVertex output[6], uint32_t arithmetic,
                                        uint32_t primitive_id);

/* Polygon POINT mode for PRIM_TYPE_TRIANGLE_STRIP: one point per rotated strip
 * vertex with the position row repeated, using the strip's provoking color.
 * Returns 0 on success, -1 for null pointers/invalid state/profile without
 * writing output.
 */
int ff_nv2a_expand_triangle_strip_points(const FFNv2aPostVertex input[3],
                                         const FFNv2aTriangleState *state,
                                         FFNv2aRasterVertex output[3], uint32_t arithmetic,
                                         uint32_t primitive_id);

/* Polygon LINE mode for PRIM_TYPE_TRIANGLE_FAN: three rotated edges sharing the
 * fan triangle's depth slope, with the fan provoking color.
 * Returns 0 on success, -1 for null pointers/invalid state/profile without
 * writing output.
 */
int ff_nv2a_expand_triangle_fan_lines(const FFNv2aPostVertex input[3],
                                      const FFNv2aTriangleState *state,
                                      FFNv2aRasterVertex output[6], uint32_t arithmetic);

/* Polygon POINT mode for PRIM_TYPE_TRIANGLE_FAN: one point per rotated fan
 * vertex with the position row repeated, using the fan provoking color.
 * Returns 0 on success, -1 for null pointers/invalid state/profile without
 * writing output.
 */
int ff_nv2a_expand_triangle_fan_points(const FFNv2aPostVertex input[3],
                                       const FFNv2aTriangleState *state,
                                       FFNv2aRasterVertex output[3], uint32_t arithmetic);

/* Assemble a PRIM_TYPE_TRIANGLE_STRIP vertex stream into per-triangle geometry
 * for the requested polygon mode. Triangle t uses input[t], input[t + 1] and
 * input[t + 2] with primitive ID t, so the stream must start at primitive 0 of
 * the draw for the even/odd rotation and provoking parity to apply. A stream of
 * `count` vertices produces `count - 2` triangles: three raster vertices each
 * for fill/point and six for line. `capacity` is the number of
 * FFNv2aRasterVertex slots available at `output`.
 * Returns 0 on success, -1 for null pointers, invalid state/profile/mode,
 * count < 3, an undersized capacity or a 32-bit size overflow, without writing
 * output.
 */
int ff_nv2a_assemble_triangle_strip(const FFNv2aPostVertex *input, uint32_t count,
                                    const FFNv2aTriangleState *state,
                                    FFNv2aRasterVertex *output, uint32_t capacity,
                                    uint32_t arithmetic, uint32_t polygon_mode);

/* Assemble a PRIM_TYPE_TRIANGLE_FAN vertex stream into per-triangle geometry
 * for the requested polygon mode. Triangle t uses input[0], input[t + 1] and
 * input[t + 2] with the single fan rotation in `state`; a stream of `count`
 * vertices produces `count - 2` triangles with the same per-mode output counts
 * as ff_nv2a_assemble_triangle_strip.
 * Returns 0 on success, -1 for null pointers, invalid state/profile/mode,
 * count < 3, an undersized capacity or a 32-bit size overflow, without writing
 * output.
 */
int ff_nv2a_assemble_triangle_fan(const FFNv2aPostVertex *input, uint32_t count,
                                  const FFNv2aTriangleState *state,
                                  FFNv2aRasterVertex *output, uint32_t capacity,
                                  uint32_t arithmetic, uint32_t polygon_mode);

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
