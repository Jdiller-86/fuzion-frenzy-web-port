#ifndef FF_NV2A_LINE_H
#define FF_NV2A_LINE_H

#include "nv2a_triangle.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Line primitive state from the pinned public xemu geometry generator. */
typedef struct FFNv2aLineState {
    uint32_t smooth;
    uint32_t first_provoking;
} FFNv2aLineState;

/* Expand one two-vertex line into the two raster vertices the shader emits.
 * The third position row is the shader's 90-degree rotated interpolation
 * helper; the depth slope is zero for a standalone line. No guest code,
 * device emulation or rasterization is performed here.
 * Returns 0 on success, -1 for null pointers or invalid state without writing.
 */
int ff_nv2a_expand_line(const FFNv2aPostVertex input[2],
                        const FFNv2aLineState *state,
                        FFNv2aRasterVertex output[2]);

/* Assemble PRIM_TYPE_LINE_STRIP host pairs: input[i] with input[i + 1] for
 * i in [0, count - 1), writing 2 * (count - 1) raster vertices in pair order.
 * Each pair uses the same two-vertex line body as ff_nv2a_expand_line, so the
 * flat provoking color is the first or second vertex of that pair. The
 * generator treats line strips as a sequence of independent two-vertex lines
 * with no strip parity. `capacity` is the number of FFNv2aRasterVertex slots
 * available at `output`.
 * Returns 0 on success, -1 for null pointers, invalid state, count < 2, an
 * undersized capacity or an arithmetic overflow, without writing output.
 */
int ff_nv2a_expand_line_strip(const FFNv2aPostVertex *input, uint32_t count,
                              const FFNv2aLineState *state,
                              FFNv2aRasterVertex *output, uint32_t capacity);

/* Assemble PRIM_TYPE_LINE_LOOP host pairs: input[i] with input[i + 1] for
 * i in [0, count - 1) plus the closing input[count - 1] to input[0] pair,
 * writing 2 * count raster vertices in pair order. Selection and validation
 * match ff_nv2a_expand_line_strip.
 * Returns 0 on success, -1 for null pointers, invalid state, count < 2, an
 * undersized capacity or an arithmetic overflow, without writing output.
 */
int ff_nv2a_expand_line_loop(const FFNv2aPostVertex *input, uint32_t count,
                             const FFNv2aLineState *state,
                             FFNv2aRasterVertex *output, uint32_t capacity);

#ifdef __cplusplus
}
#endif
#endif
