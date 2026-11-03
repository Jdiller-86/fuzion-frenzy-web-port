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

int main(void)
{
    forwarded_and_flat_colors();
    rotated_helper_and_invalid_inputs();
    puts("Owned line geometry forwarding, provoking colors, rotated helper and state rejection passed");
    return 0;
}
