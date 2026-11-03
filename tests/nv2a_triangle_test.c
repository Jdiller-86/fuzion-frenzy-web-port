/* Analytic source-owned cases; broader coverage uses the actual shader oracle. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/runtime/nv2a_triangle.h"

static void triangle_inputs(FFNv2aPostVertex input[3], const float positions[3][4])
{
    memset(input, 0, 3 * sizeof *input);
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
}

static void triangle_polygon_modes(void)
{
    const float positions[3][4] = { {0,0,1,1}, {1,0,2,2}, {0,1,3,4} };
    FFNv2aPostVertex input[3];
    triangle_inputs(input, positions);
    FFNv2aRasterVertex output[6];
    FFNv2aTriangleState state = {0,0,0,0};

    /* LINE mode: shared slope, rotated helper row, triangle provoking color. */
    assert(ff_nv2a_expand_triangle_lines(input, &state, output, FF_NV2A_FUSED) == 0);
    static const unsigned edges[3][2] = { {0,1}, {1,2}, {2,0} };
    for (unsigned e = 0; e < 3; ++e) {
        const float *v0 = positions[edges[e][0]];
        const float *v1 = positions[edges[e][1]];
        const float rotated[4] = {
            -(v1[1] - v0[1]) + v0[0], (v1[0] - v0[0]) + v0[1], v0[2], v0[3]
        };
        for (unsigned k = 0; k < 2; ++k) {
            const unsigned index = edges[e][k];
            const FFNv2aRasterVertex *out = &output[e * 2 + k];
            assert(out->clip[0] == (float)index);
            assert(out->point_size == (float)index + 1 && out->fog == (float)index + 10);
            assert(!memcmp(out->color, input[2].color, sizeof(out->color)));
            assert(!memcmp(out->texcoord, input[index].texcoord, sizeof(out->texcoord)));
            assert(!memcmp(out->triangle_position[0], v0, sizeof(v0[0]) * 4));
            assert(!memcmp(out->triangle_position[1], v1, sizeof(v1[0]) * 4));
            assert(!memcmp(out->triangle_position[2], rotated, sizeof(rotated)));
            assert(out->depth_slope == 2.0f);
        }
    }
    state.smooth = 1;
    assert(ff_nv2a_expand_triangle_lines(input, &state, output, FF_NV2A_FUSED) == 0);
    for (unsigned e = 0; e < 3; ++e) for (unsigned k = 0; k < 2; ++k)
        assert(!memcmp(output[e * 2 + k].color, input[edges[e][k]].color,
                       sizeof(output[0].color)));
    state.smooth = 0;
    state.first_provoking = 1;
    assert(ff_nv2a_expand_triangle_lines(input, &state, output, FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 6; ++i)
        assert(!memcmp(output[i].color, input[0].color, sizeof(output[0].color)));
    state.first_provoking = 0;
    state.rotation = 1;
    assert(ff_nv2a_expand_triangle_lines(input, &state, output, FF_NV2A_FUSED) == 0);
    assert(output[0].clip[0] == 2 && output[1].clip[0] == 0);
    assert(output[2].clip[0] == 0 && output[3].clip[0] == 1);
    assert(output[4].clip[0] == 1 && output[5].clip[0] == 2);
    for (unsigned i = 0; i < 6; ++i)
        assert(!memcmp(output[i].color, input[1].color, sizeof(output[0].color)));

    /* POINT mode: one repeated position row per rotated vertex. */
    state = (FFNv2aTriangleState){0,0,0,0};
    FFNv2aRasterVertex points[3];
    assert(ff_nv2a_expand_triangle_points(input, &state, points, FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 3; ++i) {
        const float *position = positions[i];
        assert(points[i].clip[0] == (float)i);
        assert(points[i].point_size == (float)i + 1 && points[i].fog == (float)i + 10);
        assert(!memcmp(points[i].color, input[2].color, sizeof(points[i].color)));
        assert(!memcmp(points[i].texcoord, input[i].texcoord, sizeof(points[i].texcoord)));
        for (unsigned j = 0; j < 3; ++j)
            assert(!memcmp(points[i].triangle_position[j], position, sizeof(position[0]) * 4));
        assert(points[i].depth_slope == 2.0f);
    }
    state.smooth = 1;
    state.rotation = 2;
    assert(ff_nv2a_expand_triangle_points(input, &state, points, FF_NV2A_FUSED) == 0);
    static const unsigned point_order[3] = { 1, 2, 0 };
    for (unsigned i = 0; i < 3; ++i) {
        const unsigned index = point_order[i];
        assert(points[i].clip[0] == (float)index);
        assert(!memcmp(points[i].color, input[index].color, sizeof(points[i].color)));
        for (unsigned j = 0; j < 3; ++j)
            assert(!memcmp(points[i].triangle_position[j], positions[index],
                           sizeof(positions[index][0]) * 4));
    }

    /* Rejection must leave every output byte unchanged. */
    FFNv2aRasterVertex saved[6], saved_points[3];
    memcpy(saved, output, sizeof(saved));
    memcpy(saved_points, points, sizeof(saved_points));
    state = (FFNv2aTriangleState){0,0,0,0};
    assert(ff_nv2a_expand_triangle_lines(NULL, &state, output, FF_NV2A_FUSED) == -1);
    assert(ff_nv2a_expand_triangle_lines(input, NULL, output, FF_NV2A_FUSED) == -1);
    assert(ff_nv2a_expand_triangle_lines(input, &state, NULL, FF_NV2A_FUSED) == -1);
    state.smooth = 2;
    assert(ff_nv2a_expand_triangle_lines(input, &state, output, FF_NV2A_FUSED) == -1);
    assert(!memcmp(output, saved, sizeof(saved)));
    state.smooth = 0; state.rotation = 3;
    assert(ff_nv2a_expand_triangle_lines(input, &state, output, FF_NV2A_FUSED) == -1);
    state.rotation = 0; state.perspective_depth = 2;
    assert(ff_nv2a_expand_triangle_lines(input, &state, output, FF_NV2A_FUSED) == -1);
    assert(ff_nv2a_expand_triangle_points(input, &state, points, FF_NV2A_FUSED) == -1);
    state.perspective_depth = 0;
    assert(ff_nv2a_expand_triangle_lines(input, &state, output, 2) == -1);
    assert(!memcmp(output, saved, sizeof(saved)));
    assert(ff_nv2a_expand_triangle_points(input, &state, points, 2) == -1);
    assert(ff_nv2a_expand_triangle_points(NULL, &state, points, FF_NV2A_FUSED) == -1);
    assert(ff_nv2a_expand_triangle_points(input, &state, NULL, FF_NV2A_FUSED) == -1);
    assert(!memcmp(points, saved_points, sizeof(saved_points)));
}

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
    triangle_polygon_modes();
    puts("Triangle prepass analytic geometry/color/depth/layout/polygon-mode cases passed");
    return 0;
}
