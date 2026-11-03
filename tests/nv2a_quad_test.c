/* Independently authored quad-geometry cases; no original content or GPU. */
#include "../src/runtime/nv2a_quad.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void vertex(FFNv2aPostVertex *v, float x, float y, float z, float w, float tag)
{
    memset(v, 0, sizeof *v);
    v->clip[0] = x + 0.5f; v->clip[1] = y + 0.25f; v->clip[2] = z - 1.0f; v->clip[3] = w;
    v->point_size = tag;
    for (unsigned c = 0; c < 4; ++c) for (unsigned j = 0; j < 4; ++j) v->color[c][j] = tag * 10.0f + c * 4.0f + j;
    v->fog = tag + 0.5f;
    for (unsigned t = 0; t < 4; ++t) for (unsigned j = 0; j < 4; ++j) v->texcoord[t][j] = tag * 20.0f + t * 4.0f + j;
    v->position[0] = x; v->position[1] = y; v->position[2] = z; v->position[3] = w;
}

static void quad_inputs(FFNv2aPostVertex input[4])
{
    vertex(&input[0], 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    vertex(&input[1], 1.0f, 0.0f, 1.0f, 1.0f, 2.0f);
    vertex(&input[2], 1.0f, 1.0f, 2.0f, 1.0f, 3.0f);
    vertex(&input[3], 0.0f, 1.0f, 1.0f, 1.0f, 4.0f);
}

static void forwarding_colors_and_depth(void)
{
    static const unsigned emit[6] = { 1, 2, 0, 2, 3, 0 };
    FFNv2aPostVertex input[4]; quad_inputs(input);
    FFNv2aRasterVertex output[6];
    FFNv2aQuadState state = { .smooth = 0, .perspective_depth = 0 };
    assert(ff_nv2a_expand_quad(input, &state, output, FF_NV2A_SEPARATE) == 0);
    for (unsigned i = 0; i < 6; ++i) {
        const unsigned index = emit[i];
        assert(!memcmp(output[i].clip, input[index].clip, sizeof(output[i].clip)));
        assert(output[i].point_size == input[index].point_size);
        assert(output[i].fog == input[index].fog);
        assert(!memcmp(output[i].texcoord, input[index].texcoord, sizeof(output[i].texcoord)));
        assert(!memcmp(output[i].color, input[3].color, sizeof(output[i].color)));
        assert(output[i].depth_slope == 1.0f);
    }
    const float *expected[6][3] = {
        { input[0].position, input[1].position, input[2].position },
        { input[0].position, input[1].position, input[2].position },
        { input[0].position, input[1].position, input[2].position },
        { input[0].position, input[2].position, input[3].position },
        { input[0].position, input[2].position, input[3].position },
        { input[0].position, input[2].position, input[3].position },
    };
    for (unsigned i = 0; i < 6; ++i) for (unsigned j = 0; j < 3; ++j)
        assert(!memcmp(output[i].triangle_position[j], expected[i][j], sizeof(output[i].triangle_position[j])));
    state.smooth = 1;
    assert(ff_nv2a_expand_quad(input, &state, output, FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 6; ++i)
        assert(!memcmp(output[i].color, input[emit[i]].color, sizeof(output[i].color)));
}

static void invalid_state_and_null_inputs(void)
{
    FFNv2aPostVertex input[4]; quad_inputs(input);
    FFNv2aRasterVertex output[6];
    FFNv2aQuadState state = { .smooth = 0, .perspective_depth = 0 };
    assert(ff_nv2a_expand_quad(input, &state, output, FF_NV2A_SEPARATE) == 0);
    FFNv2aRasterVertex saved[6]; memcpy(saved, output, sizeof(saved));
    state.smooth = 2; assert(ff_nv2a_expand_quad(input, &state, output, FF_NV2A_SEPARATE) == -1);
    assert(!memcmp(output, saved, sizeof(saved)));
    state.smooth = 0; state.perspective_depth = 2;
    assert(ff_nv2a_expand_quad(input, &state, output, FF_NV2A_SEPARATE) == -1);
    assert(!memcmp(output, saved, sizeof(saved)));
    assert(ff_nv2a_expand_quad(input, &state, output, 2) == -1);
    assert(ff_nv2a_expand_quad(NULL, &state, output, FF_NV2A_SEPARATE) == -1);
    assert(ff_nv2a_expand_quad(input, NULL, output, FF_NV2A_SEPARATE) == -1);
    assert(ff_nv2a_expand_quad(input, &state, NULL, FF_NV2A_SEPARATE) == -1);
    state.perspective_depth = 1;
    assert(ff_nv2a_expand_quad(input, &state, output, FF_NV2A_SEPARATE) == 0);
    for (unsigned i = 0; i < 6; ++i) assert(output[i].depth_slope == 0.0f);
}

static void quad_strip_cases(void)
{
    static const unsigned emit[6] = { 0, 1, 2, 2, 1, 3 };
    FFNv2aPostVertex input[4]; quad_inputs(input);
    FFNv2aRasterVertex output[6];
    FFNv2aQuadState state = { .smooth = 0, .perspective_depth = 0 };
    uint32_t emitted = 0;
    assert(ff_nv2a_expand_quad_strip(input, &state, output, FF_NV2A_SEPARATE, 0, &emitted) == 0 && emitted == 6);
    const float *expected[6][3] = {
        { input[2].position, input[0].position, input[1].position },
        { input[2].position, input[0].position, input[1].position },
        { input[2].position, input[0].position, input[1].position },
        { input[2].position, input[1].position, input[3].position },
        { input[2].position, input[1].position, input[3].position },
        { input[2].position, input[1].position, input[3].position },
    };
    for (unsigned i = 0; i < 6; ++i) {
        const unsigned index = emit[i];
        assert(!memcmp(output[i].clip, input[index].clip, sizeof(output[i].clip)));
        assert(output[i].point_size == input[index].point_size);
        assert(output[i].fog == input[index].fog);
        assert(!memcmp(output[i].texcoord, input[index].texcoord, sizeof(output[i].texcoord)));
        assert(!memcmp(output[i].color, input[3].color, sizeof(output[i].color)));
        assert(output[i].depth_slope == 1.0f);
        for (unsigned j = 0; j < 3; ++j)
            assert(!memcmp(output[i].triangle_position[j], expected[i][j], sizeof(output[i].triangle_position[j])));
    }
    state.smooth = 1;
    assert(ff_nv2a_expand_quad_strip(input, &state, output, FF_NV2A_FUSED, 0, &emitted) == 0);
    for (unsigned i = 0; i < 6; ++i)
        assert(!memcmp(output[i].color, input[emit[i]].color, sizeof(output[i].color)));
    FFNv2aRasterVertex saved[6]; memcpy(saved, output, sizeof(saved));
    assert(ff_nv2a_expand_quad_strip(input, &state, output, FF_NV2A_SEPARATE, 1, &emitted) == 0 && emitted == 0);
    assert(!memcmp(output, saved, sizeof(saved)));
    assert(ff_nv2a_expand_quad_strip(input, &state, output, FF_NV2A_SEPARATE, 0, NULL) == -1);
    state.smooth = 2;
    assert(ff_nv2a_expand_quad_strip(input, &state, output, FF_NV2A_SEPARATE, 0, &emitted) == -1);
    assert(!memcmp(output, saved, sizeof(saved)));
}

static void polygon_inputs(FFNv2aPostVertex input[4])
{
    vertex(&input[0], 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    vertex(&input[1], 1.0f, 0.0f, 1.0f, 1.0f, 2.0f);
    vertex(&input[2], 1.0f, 1.0f, 2.0f, 1.0f, 3.0f);
    vertex(&input[3], 0.0f, 1.0f, 4.0f, 1.0f, 4.0f);
}

static void check_line(const FFNv2aRasterVertex *out, const FFNv2aPostVertex input[4],
                       unsigned i0, unsigned i1, unsigned color_index, float slope)
{
    const float *v0 = input[i0].position;
    const float *v1 = input[i1].position;
    const float rotated[4] = {
        -(v1[1] - v0[1]) + v0[0], (v1[0] - v0[0]) + v0[1], v0[2], v0[3]
    };
    for (unsigned k = 0; k < 2; ++k) {
        const unsigned index = k ? i1 : i0;
        assert(out[k].clip[0] == input[index].clip[0]);
        assert(out[k].point_size == input[index].point_size);
        assert(out[k].fog == input[index].fog);
        assert(!memcmp(out[k].color, input[color_index].color, sizeof(out[k].color)));
        assert(!memcmp(out[k].texcoord, input[index].texcoord, sizeof(out[k].texcoord)));
        assert(!memcmp(out[k].triangle_position[0], v0, sizeof(v0[0]) * 4));
        assert(!memcmp(out[k].triangle_position[1], v1, sizeof(v1[0]) * 4));
        assert(!memcmp(out[k].triangle_position[2], rotated, sizeof(rotated)));
        assert(out[k].depth_slope == slope);
    }
}

static void quad_polygon_modes(void)
{
    FFNv2aPostVertex input[4]; polygon_inputs(input);
    FFNv2aQuadState state = { .smooth = 0, .perspective_depth = 0 };

    /* QUADS LINE: two edges use each triangle's own slope. */
    FFNv2aRasterVertex lines[8];
    assert(ff_nv2a_expand_quad_lines(input, &state, lines, FF_NV2A_SEPARATE) == 0);
    check_line(&lines[0], input, 0, 1, 3, 1.0f);
    check_line(&lines[2], input, 1, 2, 3, 1.0f);
    check_line(&lines[4], input, 2, 3, 3, 4.0f);
    check_line(&lines[6], input, 3, 0, 3, 4.0f);
    state.smooth = 1;
    assert(ff_nv2a_expand_quad_lines(input, &state, lines, FF_NV2A_SEPARATE) == 0);
    static const unsigned edges[4][2] = { {0,1}, {1,2}, {2,3}, {3,0} };
    for (unsigned e = 0; e < 4; ++e) for (unsigned k = 0; k < 2; ++k) {
        const unsigned index = k ? edges[e][1] : edges[e][0];
        assert(!memcmp(lines[e * 2 + k].color, input[index].color,
                       sizeof(lines[0].color)));
    }
    state.smooth = 0;

    /* QUADS POINT: inputs 0-2 and the repeated second-triangle third row. */
    FFNv2aRasterVertex points[4];
    assert(ff_nv2a_expand_quad_points(input, &state, points, FF_NV2A_SEPARATE) == 0);
    for (unsigned i = 0; i < 4; ++i) {
        assert(points[i].clip[0] == input[i].clip[0]);
        assert(!memcmp(points[i].color, input[3].color, sizeof(points[0].color)));
        for (unsigned j = 0; j < 3; ++j)
            assert(!memcmp(points[i].triangle_position[j], input[i].position,
                           sizeof(input[i].position)));
        assert(points[i].depth_slope == (i < 3 ? 1.0f : 4.0f));
    }
    state.smooth = 1;
    assert(ff_nv2a_expand_quad_points(input, &state, points, FF_NV2A_SEPARATE) == 0);
    for (unsigned i = 0; i < 4; ++i)
        assert(!memcmp(points[i].color, input[i].color, sizeof(points[0].color)));
    state.smooth = 0;

    /* QUAD_STRIP LINE: same edge slopes as the shader's rotated calc_quadz. */
    uint32_t emitted = 0;
    FFNv2aRasterVertex strip_lines[8];
    assert(ff_nv2a_expand_quad_strip_lines(input, &state, strip_lines, FF_NV2A_SEPARATE,
                                           0, &emitted) == 0 && emitted == 8);
    check_line(&strip_lines[0], input, 0, 1, 3, 1.0f);
    check_line(&strip_lines[2], input, 1, 3, 3, 2.0f);
    check_line(&strip_lines[4], input, 3, 2, 3, 2.0f);
    check_line(&strip_lines[6], input, 2, 0, 3, 1.0f);

    /* QUAD_STRIP POINT: emitted indices 0-3, fourth row from the second triangle. */
    FFNv2aRasterVertex strip_points[4];
    assert(ff_nv2a_expand_quad_strip_points(input, &state, strip_points,
                                            FF_NV2A_SEPARATE, 0, &emitted) == 0 && emitted == 4);
    for (unsigned i = 0; i < 4; ++i) {
        assert(strip_points[i].clip[0] == input[i].clip[0]);
        assert(!memcmp(strip_points[i].color, input[3].color, sizeof(strip_points[0].color)));
        for (unsigned j = 0; j < 3; ++j)
            assert(!memcmp(strip_points[i].triangle_position[j], input[i].position,
                           sizeof(input[i].position)));
        assert(strip_points[i].depth_slope == (i < 3 ? 1.0f : 2.0f));
    }
    state.smooth = 1;
    assert(ff_nv2a_expand_quad_strip_points(input, &state, strip_points,
                                            FF_NV2A_SEPARATE, 0, &emitted) == 0);
    for (unsigned i = 0; i < 4; ++i)
        assert(!memcmp(strip_points[i].color, input[i].color,
                       sizeof(strip_points[0].color)));
    state.smooth = 0;

    /* Odd adjacency primitives emit nothing and leave the output untouched. */
    FFNv2aRasterVertex saved[8];
    memcpy(saved, strip_lines, sizeof(saved));
    assert(ff_nv2a_expand_quad_strip_lines(input, &state, strip_lines, FF_NV2A_SEPARATE,
                                           1, &emitted) == 0 && emitted == 0);
    assert(!memcmp(strip_lines, saved, sizeof(saved)));
    FFNv2aRasterVertex saved_points[4];
    memcpy(saved_points, strip_points, sizeof(saved_points));
    assert(ff_nv2a_expand_quad_strip_points(input, &state, strip_points,
                                            FF_NV2A_SEPARATE, 1, &emitted) == 0 && emitted == 0);
    assert(!memcmp(strip_points, saved_points, sizeof(saved_points)));

    /* Rejection must leave output untouched. */
    FFNv2aRasterVertex saved_lines[8], saved_quad_points[4];
    memcpy(saved_lines, lines, sizeof(saved_lines));
    memcpy(saved_quad_points, points, sizeof(saved_quad_points));
    assert(ff_nv2a_expand_quad_lines(input, NULL, lines, FF_NV2A_SEPARATE) == -1);
    assert(ff_nv2a_expand_quad_lines(NULL, &state, lines, FF_NV2A_SEPARATE) == -1);
    assert(ff_nv2a_expand_quad_lines(input, &state, NULL, FF_NV2A_SEPARATE) == -1);
    state.perspective_depth = 2;
    assert(ff_nv2a_expand_quad_lines(input, &state, lines, FF_NV2A_SEPARATE) == -1);
    assert(ff_nv2a_expand_quad_points(input, &state, points, FF_NV2A_SEPARATE) == -1);
    assert(!memcmp(lines, saved_lines, sizeof(saved_lines)));
    assert(!memcmp(points, saved_quad_points, sizeof(saved_quad_points)));
    state.perspective_depth = 0;
    assert(ff_nv2a_expand_quad_points(input, &state, points, 2) == -1);
    assert(!memcmp(points, saved_quad_points, sizeof(saved_quad_points)));
    assert(ff_nv2a_expand_quad_strip_lines(input, &state, strip_lines, FF_NV2A_SEPARATE,
                                           0, NULL) == -1);
    assert(ff_nv2a_expand_quad_strip_points(input, &state, strip_points, FF_NV2A_SEPARATE,
                                            0, NULL) == -1);
    state.smooth = 2;
    assert(ff_nv2a_expand_quad_strip_points(input, &state, strip_points,
                                            FF_NV2A_SEPARATE, 0, &emitted) == -1);
    assert(!memcmp(strip_points, saved_points, sizeof(saved_points)));
}

int main(void)
{
    forwarding_colors_and_depth();
    invalid_state_and_null_inputs();
    quad_strip_cases();
    quad_polygon_modes();
    puts("Owned quad geometry forwarding, provoking colors, two triangles, depth, strip parity, polygon modes and state rejection passed");
    return 0;
}
