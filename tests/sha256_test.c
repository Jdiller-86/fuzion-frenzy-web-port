#include "../runtime/core/sha256.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void expect_digest(const ff_sha256 *context, const char *expected)
{
    static const char hex[] = "0123456789abcdef";
    uint8_t digest[32];
    ff_sha256_final(context, digest);
    for (unsigned i = 0; i < sizeof digest; ++i) {
        assert(hex[digest[i] >> 4] == expected[i * 2]);
        assert(hex[digest[i] & 15] == expected[i * 2 + 1]);
    }
}

static void known_vectors(void)
{
    ff_sha256 context;
    ff_sha256_init(&context);
    expect_digest(&context, "e3b0c44298fc1c149afbf4c8996fb924"
                            "27ae41e4649b934ca495991b7852b855");

    ff_sha256_init(&context);
    ff_sha256_update(&context, (const uint8_t *)"abc", 3);
    expect_digest(&context, "ba7816bf8f01cfea414140de5dae2223"
                            "b00361a396177a9cb410ff61f20015ad");

    ff_sha256_init(&context);
    ff_sha256_update(&context, (const uint8_t *)"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 56);
    expect_digest(&context, "248d6a61d20638b8e5c026930c3e6039"
                            "a33ce45964ff2167f6ecedd419db06c1");
}

static void chunking_and_final_are_non_destructive(void)
{
    const uint8_t message[] = "the evidence stream may arrive in uneven chunks";
    const uint32_t length = (uint32_t)(sizeof message - 1);
    ff_sha256 one_chunk, many_chunks;
    ff_sha256_init(&one_chunk);
    ff_sha256_update(&one_chunk, message, length);
    ff_sha256_init(&many_chunks);
    ff_sha256_update(&many_chunks, message, 1);
    ff_sha256_update(&many_chunks, message + 1, 7);
    ff_sha256_update(&many_chunks, message + 8, 0);
    ff_sha256_update(&many_chunks, message + 8, length - 8);

    uint8_t first[32], second[32], reference[32];
    ff_sha256_final(&many_chunks, first);
    ff_sha256_final(&many_chunks, second);
    ff_sha256_final(&one_chunk, reference);
    assert(memcmp(first, second, sizeof first) == 0);
    assert(memcmp(first, reference, sizeof first) == 0);

    /* Finalization used a copy: appending to the original context remains a
     * normal continuation and matches a fresh hash of the concatenation. */
    ff_sha256_update(&many_chunks, (const uint8_t *)"!", 1);
    ff_sha256_update(&one_chunk, (const uint8_t *)"!", 1);
    ff_sha256_final(&many_chunks, first);
    ff_sha256_final(&one_chunk, reference);
    assert(memcmp(first, reference, sizeof first) == 0);
}

static void million_a_vector(void)
{
    uint8_t block[1000];
    memset(block, 'a', sizeof block);
    ff_sha256 context;
    ff_sha256_init(&context);
    for (unsigned i = 0; i < 1000; ++i)
        ff_sha256_update(&context, block, sizeof block);
    expect_digest(&context, "cdc76e5c9914fb9281a1c7e284d73e67"
                            "f1809a48a497200e046d39ccc7112cd0");
}

int main(void)
{
    known_vectors();
    chunking_and_final_are_non_destructive();
    million_a_vector();
    puts("incremental SHA-256 vectors, chunking and finalization passed");
    return 0;
}
