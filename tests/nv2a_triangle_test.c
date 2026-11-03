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

static void assert_line_edge(const FFNv2aRasterVertex out[2], const FFNv2aPostVertex *input,
                             unsigned i0, unsigned i1, unsigned color_index, float slope)
{
    const float *v0 = input[i0].position;
    const float *v1 = input[i1].position;
    const float rotated[4] = {
        -(v1[1] - v0[1]) + v0[0], (v1[0] - v0[0]) + v0[1], v0[2], v0[3]
    };
    const unsigned indices[2] = { i0, i1 };
    for (unsigned k = 0; k < 2; ++k) {
        const unsigned index = indices[k];
        assert(out[k].clip[0] == input[index].clip[0]);
        assert(out[k].point_size == input[index].point_size);
        assert(out[k].fog == input[index].fog);
        assert(!memcmp(out[k].color, input[color_index].color, sizeof(out[k].color)));
        assert(!memcmp(out[k].texcoord, input[index].texcoord, sizeof(out[k].texcoord)));
        assert(!memcmp(out[k].triangle_position[0], v0, sizeof(v0[0]) * 4));
        assert(!memcmp(out[k].triangle_position[1], v1, sizeof(v0[0]) * 4));
        assert(!memcmp(out[k].triangle_position[2], rotated, sizeof(rotated)));
        assert(out[k].depth_slope == slope);
    }
}

static void assert_point(const FFNv2aRasterVertex *out, const FFNv2aPostVertex *input,
                         unsigned index, unsigned color_index, float slope)
{
    assert(out->clip[0] == input[index].clip[0]);
    assert(out->point_size == input[index].point_size);
    assert(out->fog == input[index].fog);
    assert(!memcmp(out->color, input[color_index].color, sizeof(out->color)));
    assert(!memcmp(out->texcoord, input[index].texcoord, sizeof(out->texcoord)));
    for (unsigned j = 0; j < 3; ++j)
        assert(!memcmp(out->triangle_position[j], input[index].position,
                       sizeof(input[index].position)));
    assert(out->depth_slope == slope);
}

static void triangle_polygon_modes(void)
{
    const float positions[3][4] = { {0,0,1,1}, {1,0,2,2}, {0,1,3,4} };
    FFNv2aPostVertex input[3];
    triangle_inputs(input, positions);
    FFNv2aRasterVertex output[6];
    FFNv2aTriangleState state = {0,0,0,0,0};

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
    state = (FFNv2aTriangleState){0,0,0,0,0};
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
    state = (FFNv2aTriangleState){0,0,0,0,0};
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

static void polygon_primitive_modes(void)
{
    const float positions[3][4] = { {0,0,1,1}, {1,0,2,2}, {0,1,3,4} };
    FFNv2aPostVertex input[3];
    triangle_inputs(input, positions);
    FFNv2aPolygonState state = {0,0,0};
    FFNv2aRasterVertex fill[3];

    /* POLYGON fill: always provoking at the rotated third vertex. */
    assert(ff_nv2a_expand_polygon_fill(input, &state, fill, FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 3; ++i) {
        assert(fill[i].clip[0] == (float)i);
        assert(!memcmp(fill[i].color, input[2].color, sizeof(fill[i].color)));
        assert(!memcmp(fill[i].triangle_position[0], positions[0], sizeof(positions[0])));
        assert(!memcmp(fill[i].triangle_position[1], positions[1], sizeof(positions[1])));
        assert(!memcmp(fill[i].triangle_position[2], positions[2], sizeof(positions[2])));
        assert(fill[i].depth_slope == 2.0f);
    }
    state.smooth = 1;
    assert(ff_nv2a_expand_polygon_fill(input, &state, fill, FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert(!memcmp(fill[i].color, input[i].color, sizeof(fill[i].color)));
    state.smooth = 0; state.rotation = 1;
    assert(ff_nv2a_expand_polygon_fill(input, &state, fill, FF_NV2A_FUSED) == 0);
    assert(fill[0].clip[0] == 2 && fill[1].clip[0] == 0 && fill[2].clip[0] == 1);
    for (unsigned i = 0; i < 3; ++i)
        assert(!memcmp(fill[i].color, input[1].color, sizeof(fill[i].color)));
    state.rotation = 0; state.perspective_depth = 1;
    assert(ff_nv2a_expand_polygon_fill(input, &state, fill, FF_NV2A_FUSED) == 0);
    assert(fill[0].depth_slope == 0.75f);

    /* POLYGON line: two-vertex segment, zero slope, provoking at index 0. */
    FFNv2aPostVertex line_input[2];
    memcpy(&line_input[0], &input[0], sizeof(line_input[0]));
    memcpy(&line_input[1], &input[1], sizeof(line_input[1]));
    const float *v0 = line_input[0].position;
    const float *v1 = line_input[1].position;
    const float rotated[4] = {
        -(v1[1] - v0[1]) + v0[0], (v1[0] - v0[0]) + v0[1], v0[2], v0[3]
    };
    state = (FFNv2aPolygonState){0,0,0};
    FFNv2aRasterVertex line[2];
    assert(ff_nv2a_expand_polygon_line(line_input, &state, line) == 0);
    for (unsigned i = 0; i < 2; ++i) {
        assert(line[i].clip[0] == line_input[i].clip[0]);
        assert(line[i].point_size == line_input[i].point_size);
        assert(line[i].fog == line_input[i].fog);
        assert(!memcmp(line[i].color, line_input[0].color, sizeof(line[i].color)));
        assert(!memcmp(line[i].texcoord, line_input[i].texcoord, sizeof(line[i].texcoord)));
        assert(!memcmp(line[i].triangle_position[0], v0, sizeof(positions[0])));
        assert(!memcmp(line[i].triangle_position[1], v1, sizeof(positions[0])));
        assert(!memcmp(line[i].triangle_position[2], rotated, sizeof(rotated)));
        assert(line[i].depth_slope == 0.0f);
    }
    /* The line branch has no triangle input: rotation and perspective state must
     * not reorder vertices or create a slope. */
    FFNv2aRasterVertex baseline_line[2];
    memcpy(baseline_line, line, sizeof(baseline_line));
    state.rotation = 1; state.perspective_depth = 1;
    assert(ff_nv2a_expand_polygon_line(line_input, &state, line) == 0);
    assert(!memcmp(line, baseline_line, sizeof(baseline_line)));
    state.rotation = 0; state.perspective_depth = 0;
    state.smooth = 1;
    assert(ff_nv2a_expand_polygon_line(line_input, &state, line) == 0);
    for (unsigned i = 0; i < 2; ++i)
        assert(!memcmp(line[i].color, line_input[i].color, sizeof(line[i].color)));

    /* Rejection must leave every output byte unchanged. */
    FFNv2aRasterVertex saved_fill[3], saved_line[2];
    memcpy(saved_fill, fill, sizeof(saved_fill));
    memcpy(saved_line, line, sizeof(saved_line));
    state = (FFNv2aPolygonState){0,0,0};
    assert(ff_nv2a_expand_polygon_fill(NULL, &state, fill, FF_NV2A_FUSED) == -1);
    assert(ff_nv2a_expand_polygon_fill(input, NULL, fill, FF_NV2A_FUSED) == -1);
    assert(ff_nv2a_expand_polygon_fill(input, &state, NULL, FF_NV2A_FUSED) == -1);
    state.smooth = 2;
    assert(ff_nv2a_expand_polygon_fill(input, &state, fill, FF_NV2A_FUSED) == -1);
    state.smooth = 0; state.rotation = 3;
    assert(ff_nv2a_expand_polygon_fill(input, &state, fill, FF_NV2A_FUSED) == -1);
    state.rotation = 0; state.perspective_depth = 2;
    assert(ff_nv2a_expand_polygon_fill(input, &state, fill, FF_NV2A_FUSED) == -1);
    state.perspective_depth = 0;
    assert(ff_nv2a_expand_polygon_fill(input, &state, fill, 2) == -1);
    assert(!memcmp(fill, saved_fill, sizeof(saved_fill)));
    state.smooth = 2;
    assert(ff_nv2a_expand_polygon_line(line_input, &state, line) == -1);
    state.smooth = 0; state.rotation = 3;
    assert(ff_nv2a_expand_polygon_line(line_input, &state, line) == -1);
    state.rotation = 0; state.perspective_depth = 2;
    assert(ff_nv2a_expand_polygon_line(line_input, &state, line) == -1);
    assert(ff_nv2a_expand_polygon_line(NULL, &state, line) == -1);
    assert(ff_nv2a_expand_polygon_line(line_input, NULL, line) == -1);
    assert(ff_nv2a_expand_polygon_line(line_input, &state, NULL) == -1);
    assert(!memcmp(line, saved_line, sizeof(saved_line)));
}

static void triangle_strip_and_fan_modes(void)
{
    const float positions[3][4] = { {0,0,1,1}, {1,0,2,2}, {0,1,3,4} };
    FFNv2aPostVertex input[3];
    triangle_inputs(input, positions);
    FFNv2aRasterVertex output[3];

    /* TRIANGLE_STRIP: even/odd primitive IDs select different rotations and
     * alternate the flat provoking vertex. */
    FFNv2aTriangleState state = {0,0,0,0,1};
    static const unsigned even_order[3] = { 0, 1, 2 };
    static const unsigned odd_order[3] = { 2, 0, 1 };
    assert(ff_nv2a_expand_triangle_strip(input, &state, output, FF_NV2A_FUSED, 0) == 0);
    for (unsigned i = 0; i < 3; ++i) {
        const unsigned index = even_order[i];
        assert(output[i].clip[0] == (float)index);
        assert(!memcmp(output[i].color, input[2].color, sizeof(output[i].color)));
        for (unsigned j = 0; j < 3; ++j)
            assert(!memcmp(output[i].triangle_position[j], positions[even_order[j]],
                           sizeof(positions[0])));
        assert(output[i].depth_slope == 2.0f);
    }
    assert(ff_nv2a_expand_triangle_strip(input, &state, output, FF_NV2A_FUSED, 1) == 0);
    for (unsigned i = 0; i < 3; ++i) {
        const unsigned index = odd_order[i];
        assert(output[i].clip[0] == (float)index);
        assert(!memcmp(output[i].color, input[0].color, sizeof(output[i].color)));
        for (unsigned j = 0; j < 3; ++j)
            assert(!memcmp(output[i].triangle_position[j], positions[odd_order[j]],
                           sizeof(positions[0])));
        assert(output[i].depth_slope == 2.0f);
    }
    state.first_provoking = 1;
    assert(ff_nv2a_expand_triangle_strip(input, &state, output, FF_NV2A_FUSED, 0) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert(!memcmp(output[i].color, input[0].color, sizeof(output[i].color)));
    assert(ff_nv2a_expand_triangle_strip(input, &state, output, FF_NV2A_FUSED, 1) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert(!memcmp(output[i].color, input[2].color, sizeof(output[i].color)));
    state.first_provoking = 0;
    state.smooth = 1;
    assert(ff_nv2a_expand_triangle_strip(input, &state, output, FF_NV2A_FUSED, 0) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert(!memcmp(output[i].color, input[even_order[i]].color,
                       sizeof(output[0].color)));
    assert(ff_nv2a_expand_triangle_strip(input, &state, output, FF_NV2A_FUSED, 1) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert(!memcmp(output[i].color, input[odd_order[i]].color,
                       sizeof(output[0].color)));

    /* TRIANGLE_FAN: one rotation, flat provoking at the rotated second vertex. */
    state = (FFNv2aTriangleState){0,0,0,2,0};
    static const unsigned fan_order[3] = { 1, 2, 0 };
    assert(ff_nv2a_expand_triangle_fan(input, &state, output, FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 3; ++i) {
        const unsigned index = fan_order[i];
        assert(output[i].clip[0] == (float)index);
        assert(!memcmp(output[i].color, input[2].color, sizeof(output[i].color)));
        for (unsigned j = 0; j < 3; ++j)
            assert(!memcmp(output[i].triangle_position[j], positions[fan_order[j]],
                           sizeof(positions[0])));
        assert(output[i].depth_slope == 2.0f);
    }
    state.first_provoking = 1;
    assert(ff_nv2a_expand_triangle_fan(input, &state, output, FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert(!memcmp(output[i].color, input[1].color, sizeof(output[i].color)));
    state.first_provoking = 0;
    state.rotation = 0;
    state.smooth = 1;
    assert(ff_nv2a_expand_triangle_fan(input, &state, output, FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert(!memcmp(output[i].color, input[i].color, sizeof(output[0].color)));

    /* Rejection must leave every output byte unchanged. */
    state = (FFNv2aTriangleState){0,0,0,0,0};
    assert(ff_nv2a_expand_triangle_strip(input, &state, output, FF_NV2A_FUSED, 0) == 0);
    FFNv2aRasterVertex saved[3];
    memcpy(saved, output, sizeof(saved));
    state.rotation_odd = 3;
    assert(ff_nv2a_expand_triangle_strip(input, &state, output, FF_NV2A_FUSED, 0) == -1);
    assert(ff_nv2a_expand_triangle_strip(input, &state, output, FF_NV2A_FUSED, 1) == -1);
    assert(ff_nv2a_expand_triangle_strip(input, NULL, output, FF_NV2A_FUSED, 0) == -1);
    state.rotation_odd = 0;
    assert(ff_nv2a_expand_triangle_strip(input, &state, output, 2, 0) == -1);
    assert(ff_nv2a_expand_triangle_strip(NULL, &state, output, FF_NV2A_FUSED, 0) == -1);
    assert(ff_nv2a_expand_triangle_strip(input, &state, NULL, FF_NV2A_FUSED, 0) == -1);
    state.rotation = 3;
    assert(ff_nv2a_expand_triangle_fan(input, &state, output, FF_NV2A_FUSED) == -1);
    state.rotation = 0;
    assert(ff_nv2a_expand_triangle_fan(input, &state, output, 2) == -1);
    assert(ff_nv2a_expand_triangle_fan(input, NULL, output, FF_NV2A_FUSED) == -1);
    assert(ff_nv2a_expand_triangle_fan(NULL, &state, output, FF_NV2A_FUSED) == -1);
    assert(!memcmp(output, saved, sizeof(saved)));
}

static void triangle_strip_fan_line_point_modes(void)
{
    const float positions[3][4] = { {0,0,1,1}, {1,0,2,2}, {0,1,3,4} };
    FFNv2aPostVertex input[3];
    triangle_inputs(input, positions);
    FFNv2aTriangleState state = {0,0,0,0,1};
    FFNv2aRasterVertex lines[6], points[3];

    /* Strip LINE/POINT: even parity, rotation 0, flat provoking index 2. */
    assert(ff_nv2a_expand_triangle_strip_lines(input, &state, lines,
                                               FF_NV2A_FUSED, 0) == 0);
    assert_line_edge(&lines[0], input, 0, 1, 2, 2.0f);
    assert_line_edge(&lines[2], input, 1, 2, 2, 2.0f);
    assert_line_edge(&lines[4], input, 2, 0, 2, 2.0f);
    assert(ff_nv2a_expand_triangle_strip_points(input, &state, points,
                                                FF_NV2A_FUSED, 0) == 0);
    for (unsigned i = 0; i < 3; ++i) assert_point(&points[i], input, i, 2, 2.0f);

    /* Odd parity uses rotation_odd 1 and the rotated indices[1] provoking. */
    assert(ff_nv2a_expand_triangle_strip_lines(input, &state, lines,
                                               FF_NV2A_FUSED, 1) == 0);
    assert_line_edge(&lines[0], input, 2, 0, 0, 2.0f);
    assert_line_edge(&lines[2], input, 0, 1, 0, 2.0f);
    assert_line_edge(&lines[4], input, 1, 2, 0, 2.0f);
    static const unsigned odd_order[3] = { 2, 0, 1 };
    assert(ff_nv2a_expand_triangle_strip_points(input, &state, points,
                                                FF_NV2A_FUSED, 1) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert_point(&points[i], input, odd_order[i], 0, 2.0f);

    /* First-vertex provoking overrides the parity selection. */
    state.first_provoking = 1;
    assert(ff_nv2a_expand_triangle_strip_lines(input, &state, lines,
                                               FF_NV2A_FUSED, 1) == 0);
    assert_line_edge(&lines[0], input, 2, 0, 2, 2.0f);
    assert(ff_nv2a_expand_triangle_strip_points(input, &state, points,
                                                FF_NV2A_FUSED, 0) == 0);
    for (unsigned i = 0; i < 3; ++i) assert_point(&points[i], input, i, 0, 2.0f);

    /* Smooth strip colors follow every emitted input in edge order. */
    state = (FFNv2aTriangleState){1,0,0,0,1};
    assert(ff_nv2a_expand_triangle_strip_lines(input, &state, lines,
                                               FF_NV2A_FUSED, 0) == 0);
    static const unsigned even_emit[6] = { 0, 1, 1, 2, 2, 0 };
    for (unsigned i = 0; i < 6; ++i)
        assert(!memcmp(lines[i].color, input[even_emit[i]].color, sizeof(lines[0].color)));
    assert(ff_nv2a_expand_triangle_strip_points(input, &state, points,
                                                FF_NV2A_FUSED, 1) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert(!memcmp(points[i].color, input[odd_order[i]].color, sizeof(points[0].color)));

    /* Fan LINE/POINT: rotation 2, flat provoking at rotated index 1. */
    state = (FFNv2aTriangleState){0,0,0,2,0};
    static const unsigned fan_order[3] = { 1, 2, 0 };
    assert(ff_nv2a_expand_triangle_fan_lines(input, &state, lines,
                                             FF_NV2A_FUSED) == 0);
    assert_line_edge(&lines[0], input, 1, 2, 2, 2.0f);
    assert_line_edge(&lines[2], input, 2, 0, 2, 2.0f);
    assert_line_edge(&lines[4], input, 0, 1, 2, 2.0f);
    assert(ff_nv2a_expand_triangle_fan_points(input, &state, points,
                                              FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert_point(&points[i], input, fan_order[i], 2, 2.0f);
    state.first_provoking = 1;
    assert(ff_nv2a_expand_triangle_fan_lines(input, &state, lines,
                                             FF_NV2A_FUSED) == 0);
    assert_line_edge(&lines[0], input, 1, 2, 1, 2.0f);
    assert(ff_nv2a_expand_triangle_fan_points(input, &state, points,
                                              FF_NV2A_FUSED) == 0);
    for (unsigned i = 0; i < 3; ++i)
        assert_point(&points[i], input, fan_order[i], 1, 2.0f);

    /* Rejection must leave every output byte unchanged. */
    state = (FFNv2aTriangleState){0,0,0,0,0};
    assert(ff_nv2a_expand_triangle_strip_lines(input, &state, lines,
                                               FF_NV2A_FUSED, 0) == 0);
    FFNv2aRasterVertex saved_lines[6], saved_points[3];
    memcpy(saved_lines, lines, sizeof(saved_lines));
    assert(ff_nv2a_expand_triangle_strip_points(input, &state, points,
                                                FF_NV2A_FUSED, 0) == 0);
    memcpy(saved_points, points, sizeof(saved_points));
    state.rotation_odd = 3;
    assert(ff_nv2a_expand_triangle_strip_lines(input, &state, lines,
                                               FF_NV2A_FUSED, 0) == -1);
    assert(ff_nv2a_expand_triangle_strip_points(input, &state, points,
                                                FF_NV2A_FUSED, 0) == -1);
    state.rotation_odd = 0;
    assert(ff_nv2a_expand_triangle_strip_lines(input, &state, lines, 2, 0) == -1);
    assert(ff_nv2a_expand_triangle_strip_points(input, &state, points, 2, 0) == -1);
    assert(ff_nv2a_expand_triangle_strip_lines(NULL, &state, lines,
                                               FF_NV2A_FUSED, 0) == -1);
    assert(ff_nv2a_expand_triangle_strip_points(input, NULL, points,
                                                FF_NV2A_FUSED, 0) == -1);
    state.rotation = 3;
    assert(ff_nv2a_expand_triangle_fan_lines(input, &state, lines,
                                             FF_NV2A_FUSED) == -1);
    assert(ff_nv2a_expand_triangle_fan_points(input, &state, points,
                                              FF_NV2A_FUSED) == -1);
    state.rotation = 0;
    assert(ff_nv2a_expand_triangle_fan_lines(input, &state, lines, 2) == -1);
    assert(ff_nv2a_expand_triangle_fan_points(input, &state, points, 2) == -1);
    assert(ff_nv2a_expand_triangle_fan_lines(input, &state, NULL,
                                             FF_NV2A_FUSED) == -1);
    assert(ff_nv2a_expand_triangle_fan_points(NULL, &state, points,
                                              FF_NV2A_FUSED) == -1);
    assert(!memcmp(lines, saved_lines, sizeof(saved_lines)));
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
    FFNv2aTriangleState state = {1,0,0,0,0};
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
    state = (FFNv2aTriangleState){1,0,0,0,0};
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
    state = (FFNv2aTriangleState){1,0,0,0,0};
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
    polygon_primitive_modes();
    triangle_strip_and_fan_modes();
    triangle_strip_fan_line_point_modes();
    puts("Triangle prepass analytic geometry/color/depth/layout/polygon-mode/polygon/strip-fan/line-point cases passed");
    return 0;
}
