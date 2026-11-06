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

#ifdef __cplusplus
}
#endif
#endif
