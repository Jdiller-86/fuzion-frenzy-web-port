/* Independently authored quad-geometry cases; no original content or GPU. */
#include "../renderer/nv2a_quad.h"
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

int main(void)
{
    forwarding_colors_and_depth();
    invalid_state_and_null_inputs();
    quad_strip_cases();
    puts("Owned quad geometry forwarding, provoking colors, two triangles, depth, strip parity and state rejection passed");
    return 0;
}
