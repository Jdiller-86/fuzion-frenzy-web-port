#include "sha256.h"

#include <string.h>

static const uint32_t round_constants[64] = {
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u,
    0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
    0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u,
    0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
    0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu,
    0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
    0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
    0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
    0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u,
    0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
    0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u,
    0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
    0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u,
    0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u,
    0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u
};

static uint32_t rotate_right(uint32_t value, unsigned amount)
{
    return (value >> amount) | (value << (32u - amount));
}

static uint32_t load_be32(const uint8_t *bytes)
{
    return ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) |
           ((uint32_t)bytes[2] << 8) | (uint32_t)bytes[3];
}

static void store_be32(uint8_t *bytes, uint32_t value)
{
    bytes[0] = (uint8_t)(value >> 24);
    bytes[1] = (uint8_t)(value >> 16);
    bytes[2] = (uint8_t)(value >> 8);
    bytes[3] = (uint8_t)value;
}

static void process_block(ff_sha256 *context, const uint8_t block[64])
{
    uint32_t schedule[64];
    for (unsigned i = 0; i < 16; ++i) schedule[i] = load_be32(block + i * 4u);
    for (unsigned i = 16; i < 64; ++i) {
        uint32_t x = schedule[i - 15];
        uint32_t y = schedule[i - 2];
        uint32_t small_sigma0 = rotate_right(x, 7) ^ rotate_right(x, 18) ^ (x >> 3);
        uint32_t small_sigma1 = rotate_right(y, 17) ^ rotate_right(y, 19) ^ (y >> 10);
        schedule[i] = schedule[i - 16] + small_sigma0 + schedule[i - 7] + small_sigma1;
    }

    uint32_t a = context->state[0], b = context->state[1];
    uint32_t c = context->state[2], d = context->state[3];
    uint32_t e = context->state[4], f = context->state[5];
    uint32_t g = context->state[6], h = context->state[7];

    for (unsigned i = 0; i < 64; ++i) {
        uint32_t big_sigma1 = rotate_right(e, 6) ^ rotate_right(e, 11) ^ rotate_right(e, 25);
        uint32_t choose = (e & f) ^ (~e & g);
        uint32_t temp1 = h + big_sigma1 + choose + round_constants[i] + schedule[i];
        uint32_t big_sigma0 = rotate_right(a, 2) ^ rotate_right(a, 13) ^ rotate_right(a, 22);
        uint32_t majority = (a & b) ^ (a & c) ^ (b & c);
        uint32_t temp2 = big_sigma0 + majority;
        h = g; g = f; f = e; e = d + temp1;
        d = c; c = b; b = a; a = temp1 + temp2;
    }

    context->state[0] += a; context->state[1] += b;
    context->state[2] += c; context->state[3] += d;
    context->state[4] += e; context->state[5] += f;
    context->state[6] += g; context->state[7] += h;
}

void ff_sha256_init(ff_sha256 *context)
{
    static const uint32_t initial_state[8] = {
        0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au,
        0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u
    };
    memcpy(context->state, initial_state, sizeof initial_state);
    context->total_bytes = 0;
    context->block_bytes = 0;
}

void ff_sha256_update(ff_sha256 *context, const uint8_t *bytes, uint32_t count)
{
    context->total_bytes += count;
    while (count) {
        uint32_t available = 64u - context->block_bytes;
        uint32_t take = count < available ? count : available;
        memcpy(context->block + context->block_bytes, bytes, take);
        context->block_bytes += take;
        bytes += take;
        count -= take;
        if (context->block_bytes == 64u) {
            process_block(context, context->block);
            context->block_bytes = 0;
        }
    }
}

void ff_sha256_final(const ff_sha256 *context, uint8_t output[32])
{
    ff_sha256 final_context = *context;
    uint64_t bit_count = final_context.total_bytes * 8u;
    uint8_t padding[128] = { 0x80u };
    uint32_t padding_bytes = final_context.block_bytes < 56u
        ? 56u - final_context.block_bytes
        : 120u - final_context.block_bytes;
    ff_sha256_update(&final_context, padding, padding_bytes);

    uint8_t length[8];
    for (unsigned i = 0; i < 8; ++i)
        length[7u - i] = (uint8_t)(bit_count >> (i * 8u));
    ff_sha256_update(&final_context, length, sizeof length);

    for (unsigned i = 0; i < 8; ++i) store_be32(output + i * 4u, final_context.state[i]);
}
