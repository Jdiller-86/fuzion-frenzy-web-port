/* Analytic source-owned cases; broader coverage uses the actual shader oracle. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/runtime/nv2a_triangle.h"

int main(void)
{
    FFNv2aPostVertex input[3] = {0};
    const float positions[3][4] = { {0,0,1,1}, {1,0,2,2}, {0,1,3,4} };
    for (unsigned i = 0; i < 3; ++i) {
        memcpy(input[i].position, positions[i], sizeof(positions[i]));
        input[i].clip[0] = (float)i;
        input[i].point_size = (float)i + 1;
        input[i].fog = (float)i + 10;
        for (unsigned k = 0; k < 4; ++k) {
            for (unsigned j = 0; j < 4; ++j) {
                input[i].color[k][j] = (float)(100*i + 10*k + j);
                input[i].texcoord[k][j] = (float)(100*i + 10*k + j + 1000);
            }
        }
    }
    FFNv2aRasterVertex output[3];
    FFNv2aTriangleState state = {1,0,0,0};
    assert(ff_nv2a_expand_triangle(input, &state, output, FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 3; ++i) {
        assert(output[i].depth_slope == 2.0f);
        assert(!memcmp(output[i].color, input[i].color, sizeof(input[i].color)));
        assert(!memcmp(output[i].texcoord, input[i].texcoord, sizeof(input[i].texcoord)));
        assert(!memcmp(output[i].triangle_position, positions, sizeof(positions)));
        assert(output[i].clip[0] == (float)i && output[i].fog == (float)i + 10);
    }
    state.smooth = 0;
    assert(ff_nv2a_expand_triangle(input, &state, output, FF_NV2A_FUSED) == 0);
    assert(output[0].color[0][0] == 200 && output[2].color[3][3] == 233);
    state.first_provoking = 1;
    state.perspective_depth = 1;
    assert(ff_nv2a_expand_triangle(input, &state, output, FF_NV2A_FUSED) == 0);
    assert(output[2].color[0][0] == 0 && output[1].depth_slope == 0.75f);
    state.rotation = 1;
    assert(ff_nv2a_expand_triangle(input, &state, output, FF_NV2A_FUSED) == 0);
    assert(output[0].clip[0] == 2 && output[1].clip[0] == 0 && output[2].clip[0] == 1);
    assert(output[1].color[0][0] == 200);
    state = (FFNv2aTriangleState){1,0,0,0};
    memcpy(input[2].position, input[1].position, sizeof(input[2].position));
    assert(ff_nv2a_expand_triangle(input, &state, output, FF_NV2A_FUSED) == 0);
    assert(output[0].depth_slope == 0);
    FFNv2aRasterVertex before[3];
    memcpy(before, output, sizeof(before));
    state.rotation = 3;
    assert(ff_nv2a_expand_triangle(input, &state, output, FF_NV2A_FUSED) == -1);
    assert(!memcmp(before, output, sizeof(before)));
    assert(ff_nv2a_expand_triangle(NULL, &state, output, FF_NV2A_FUSED) == -1);

    /* Source-owned cancellation case, independently captured from the shader.
     * Host profiles must remain distinguishable; a tolerance would hide this.
     */
    const uint32_t sensitive[3][4] = {
        {1138787601,1133833133,1164205520,1091335899},
        {3279244079,3273274946,3293779152,1093276307},
        {1131899770,1129275120,3304187498,1091484617}
    };
    for (unsigned i = 0; i < 3; ++i) memcpy(input[i].position, sensitive[i], sizeof(sensitive[i]));
    state = (FFNv2aTriangleState){1,0,0,0};
    uint32_t slope_bits;
    assert(ff_nv2a_expand_triangle(input, &state, output, FF_NV2A_FUSED) == 0);
    memcpy(&slope_bits, &output[0].depth_slope, sizeof(slope_bits));
    assert(slope_bits == 1121056463);
    assert(ff_nv2a_expand_triangle(input, &state, output, FF_NV2A_SEPARATE) == 0);
    memcpy(&slope_bits, &output[0].depth_slope, sizeof(slope_bits));
    assert(slope_bits == 1121056465);
    memcpy(before, output, sizeof(before));
    assert(ff_nv2a_expand_triangle(input, &state, output, 2) == -1);
    assert(!memcmp(before, output, sizeof(before)));
    puts("Triangle prepass analytic geometry/color/depth/layout cases passed");
    return 0;
}
