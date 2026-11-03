/* Independently authored line-geometry cases; no original content or GPU. */
#include "../src/runtime/nv2a_line.h"
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

static void assert_pair(const FFNv2aRasterVertex pair[2],
                        const FFNv2aPostVertex *a, const FFNv2aPostVertex *b,
                        const FFNv2aPostVertex *flat_source)
{
    const float delta_x = b->position[0] - a->position[0];
    const float delta_y = b->position[1] - a->position[1];
    const float rotated[4] = { -delta_y + a->position[0], delta_x + a->position[1],
                               a->position[2], a->position[3] };
    const FFNv2aPostVertex *source[2] = { a, b };
    for (unsigned i = 0; i < 2; ++i) {
        assert(!memcmp(pair[i].clip, source[i]->clip, sizeof(pair[i].clip)));
        assert(pair[i].point_size == source[i]->point_size);
        assert(pair[i].fog == source[i]->fog);
        assert(!memcmp(pair[i].texcoord, source[i]->texcoord, sizeof(pair[i].texcoord)));
        assert(!memcmp(pair[i].color, flat_source->color, sizeof(pair[i].color)));
        assert(!memcmp(pair[i].triangle_position[0], a->position,
                       sizeof(pair[i].triangle_position[0])));
        assert(!memcmp(pair[i].triangle_position[1], b->position,
                       sizeof(pair[i].triangle_position[1])));
        assert(!memcmp(pair[i].triangle_position[2], rotated, sizeof(rotated)));
        assert(pair[i].depth_slope == 0.0f);
    }
}

static void forwarded_and_flat_colors(void)
{
    FFNv2aPostVertex input[2]; vertex(&input[0], 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    vertex(&input[1], 4.0f, 2.0f, 3.0f, 7.0f, 2.0f);
    FFNv2aRasterVertex output[2];
    FFNv2aLineState state = { .smooth = 0, .first_provoking = 1 };
    assert(ff_nv2a_expand_line(input, &state, output) == 0);
    for (unsigned i = 0; i < 2; ++i) {
        assert(!memcmp(output[i].clip, input[i].clip, sizeof(output[i].clip)));
        assert(output[i].point_size == input[i].point_size);
        assert(output[i].fog == input[i].fog);
        assert(!memcmp(output[i].texcoord, input[i].texcoord, sizeof(output[i].texcoord)));
        assert(!memcmp(output[i].color, input[0].color, sizeof(output[i].color)));
        assert(!memcmp(output[i].triangle_position[0], input[0].position, sizeof(output[i].triangle_position[0])));
        assert(!memcmp(output[i].triangle_position[1], input[1].position, sizeof(output[i].triangle_position[1])));
        const float rotated[4] = { -2.0f, 4.0f, 1.0f, 1.0f };
        assert(!memcmp(output[i].triangle_position[2], rotated, sizeof(rotated)));
        assert(output[i].depth_slope == 0.0f);
    }
    state.first_provoking = 0;
    assert(ff_nv2a_expand_line(input, &state, output) == 0);
    assert(!memcmp(output[0].color, input[1].color, sizeof(output[0].color)));
    assert(!memcmp(output[1].color, input[1].color, sizeof(output[1].color)));
    state.smooth = 1;
    assert(ff_nv2a_expand_line(input, &state, output) == 0);
    assert(!memcmp(output[0].color, input[0].color, sizeof(output[0].color)));
    assert(!memcmp(output[1].color, input[1].color, sizeof(output[1].color)));
}

static void rotated_helper_and_invalid_inputs(void)
{
    FFNv2aPostVertex input[2]; vertex(&input[0], 2.0f, 3.0f, -4.0f, 5.0f, 3.0f);
    vertex(&input[1], 0.0f, 0.0f, 8.0f, 9.0f, 4.0f);
    FFNv2aRasterVertex output[2];
    FFNv2aLineState state = { .smooth = 0, .first_provoking = 1 };
    assert(ff_nv2a_expand_line(input, &state, output) == 0);
    const float rotated[4] = { 5.0f, 1.0f, -4.0f, 5.0f };
    assert(!memcmp(output[0].triangle_position[2], rotated, sizeof(rotated)));
    FFNv2aRasterVertex saved[2]; memcpy(saved, output, sizeof(saved));
    state.smooth = 2; assert(ff_nv2a_expand_line(input, &state, output) == -1);
    assert(!memcmp(output, saved, sizeof(saved)));
    state.smooth = 0; state.first_provoking = 2; assert(ff_nv2a_expand_line(input, &state, output) == -1);
    assert(!memcmp(output, saved, sizeof(saved)));
    assert(ff_nv2a_expand_line(NULL, &state, output) == -1);
    assert(ff_nv2a_expand_line(input, NULL, output) == -1);
    assert(ff_nv2a_expand_line(input, &state, NULL) == -1);
}

static void line_strip_pairs(void)
{
    FFNv2aPostVertex input[3];
    vertex(&input[0], 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    vertex(&input[1], 4.0f, 2.0f, 3.0f, 7.0f, 2.0f);
    vertex(&input[2], -2.0f, 5.0f, 6.0f, 8.0f, 3.0f);
    FFNv2aRasterVertex output[4];
    FFNv2aLineState state = { .smooth = 0, .first_provoking = 1 };

    /* Two adjacent pairs in input order; each pair provokes its own first. */
    assert(ff_nv2a_expand_line_strip(input, 3, &state, output, 4) == 0);
    assert_pair(&output[0], &input[0], &input[1], &input[0]);
    assert_pair(&output[2], &input[1], &input[2], &input[1]);

    /* Last-vertex provoking switches within each pair. */
    state.first_provoking = 0;
    assert(ff_nv2a_expand_line_strip(input, 3, &state, output, 4) == 0);
    assert_pair(&output[0], &input[0], &input[1], &input[1]);
    assert_pair(&output[2], &input[1], &input[2], &input[2]);

    /* Smooth color follows every emitted input rather than the pair provoking. */
    state.smooth = 1;
    assert(ff_nv2a_expand_line_strip(input, 3, &state, output, 4) == 0);
    static const unsigned smooth_indices[4] = { 0, 1, 1, 2 };
    for (unsigned i = 0; i < 4; ++i)
        assert(!memcmp(output[i].color, input[smooth_indices[i]].color,
                       sizeof(output[i].color)));

    /* A two-vertex strip is exactly one pair. */
    state = (FFNv2aLineState){ .smooth = 0, .first_provoking = 1 };
    assert(ff_nv2a_expand_line_strip(input, 2, &state, output, 2) == 0);
    assert_pair(&output[0], &input[0], &input[1], &input[0]);
}

static void line_loop_pairs(void)
{
    FFNv2aPostVertex input[3];
    vertex(&input[0], 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    vertex(&input[1], 4.0f, 2.0f, 3.0f, 7.0f, 2.0f);
    vertex(&input[2], -2.0f, 5.0f, 6.0f, 8.0f, 3.0f);
    FFNv2aRasterVertex output[6];
    FFNv2aLineState state = { .smooth = 0, .first_provoking = 1 };

    /* Adjacent pairs plus the closing pair from the last input back to input[0]. */
    assert(ff_nv2a_expand_line_loop(input, 3, &state, output, 6) == 0);
    assert_pair(&output[0], &input[0], &input[1], &input[0]);
    assert_pair(&output[2], &input[1], &input[2], &input[1]);
    assert_pair(&output[4], &input[2], &input[0], &input[2]);

    /* Closing pair direction stays last-to-first; only provoking changes. */
    state.first_provoking = 0;
    assert(ff_nv2a_expand_line_loop(input, 3, &state, output, 6) == 0);
    assert_pair(&output[0], &input[0], &input[1], &input[1]);
    assert_pair(&output[2], &input[1], &input[2], &input[2]);
    assert_pair(&output[4], &input[2], &input[0], &input[0]);

    /* Smooth color on the wrap pair: emitted last input then first input. */
    state.smooth = 1;
    assert(ff_nv2a_expand_line_loop(input, 3, &state, output, 6) == 0);
    static const unsigned smooth_indices[6] = { 0, 1, 1, 2, 2, 0 };
    for (unsigned i = 0; i < 6; ++i)
        assert(!memcmp(output[i].color, input[smooth_indices[i]].color,
                       sizeof(output[i].color)));
    const float delta_x = input[0].position[0] - input[2].position[0];
    const float delta_y = input[0].position[1] - input[2].position[1];
    const float closing_rotated[4] = { -delta_y + input[2].position[0],
                                       delta_x + input[2].position[1],
                                       input[2].position[2], input[2].position[3] };
    assert(!memcmp(output[4].triangle_position[2], closing_rotated,
                   sizeof(closing_rotated)));

    /* A two-vertex loop emits the pair in both directions. */
    state = (FFNv2aLineState){ .smooth = 0, .first_provoking = 1 };
    assert(ff_nv2a_expand_line_loop(input, 2, &state, output, 4) == 0);
    assert_pair(&output[0], &input[0], &input[1], &input[0]);
    assert_pair(&output[2], &input[1], &input[0], &input[1]);
}

static void strip_loop_rejection(void)
{
    FFNv2aPostVertex input[3];
    vertex(&input[0], 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    vertex(&input[1], 4.0f, 2.0f, 3.0f, 7.0f, 2.0f);
    vertex(&input[2], -2.0f, 5.0f, 6.0f, 8.0f, 3.0f);
    FFNv2aRasterVertex output[6];
    FFNv2aLineState state = { .smooth = 0, .first_provoking = 1 };
    memset(output, 0x5a, sizeof(output));
    assert(ff_nv2a_expand_line_strip(input, 3, &state, output, 6) == 0);
    FFNv2aRasterVertex saved[6];
    memcpy(saved, output, sizeof(saved));

    /* count < 2 has no pairs. */
    assert(ff_nv2a_expand_line_strip(input, 0, &state, output, 6) == -1);
    assert(ff_nv2a_expand_line_strip(input, 1, &state, output, 6) == -1);
    assert(ff_nv2a_expand_line_loop(input, 0, &state, output, 6) == -1);
    assert(ff_nv2a_expand_line_loop(input, 1, &state, output, 6) == -1);

    /* Capacity is expressed in raster vertices and must cover every pair. */
    assert(ff_nv2a_expand_line_strip(input, 3, &state, output, 3) == -1);
    assert(ff_nv2a_expand_line_loop(input, 3, &state, output, 5) == -1);

    /* Invalid state and null pointers reject before any write. */
    state.smooth = 2;
    assert(ff_nv2a_expand_line_strip(input, 3, &state, output, 6) == -1);
    assert(ff_nv2a_expand_line_loop(input, 3, &state, output, 6) == -1);
    state.smooth = 0;
    state.first_provoking = 2;
    assert(ff_nv2a_expand_line_strip(input, 3, &state, output, 6) == -1);
    assert(ff_nv2a_expand_line_loop(input, 3, &state, output, 6) == -1);
    state.first_provoking = 1;
    assert(ff_nv2a_expand_line_strip(NULL, 3, &state, output, 6) == -1);
    assert(ff_nv2a_expand_line_loop(NULL, 3, &state, output, 6) == -1);
    assert(ff_nv2a_expand_line_strip(input, 3, NULL, output, 6) == -1);
    assert(ff_nv2a_expand_line_loop(input, 3, NULL, output, 6) == -1);
    assert(ff_nv2a_expand_line_strip(input, 3, &state, NULL, 6) == -1);
    assert(ff_nv2a_expand_line_loop(input, 3, &state, NULL, 6) == -1);
    assert(!memcmp(output, saved, sizeof(saved)));
}

int main(void)
{
    forwarded_and_flat_colors();
    rotated_helper_and_invalid_inputs();
    line_strip_pairs();
    line_loop_pairs();
    strip_loop_rejection();
    puts("Owned line geometry forwarding, provoking colors, rotated helper, strip/loop pairs and state rejection passed");
    return 0;
}
