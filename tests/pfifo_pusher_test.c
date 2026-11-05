/* Owned PFIFO pusher prototype scenarios; no original code or device. */
#include "../src/runtime/pfifo_pusher.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define BASE 0x00001000u
#define END 0x00002000u
#define WORDS 1024u

static uint32_t ram[WORDS];
static uint8_t written[WORDS * 4u];

static void reset_memory(void)
{
    memset(ram, 0, sizeof ram);
    memset(written, 1, sizeof written);
}

static uint32_t word_offset(uint32_t address)
{
    return (address - BASE) / 4u;
}

static uint32_t read_word(void *context, uint32_t address, uint32_t *value)
{
    (void)context;
    if (address < BASE || address + 4u > END) return 1;
    *value = ram[word_offset(address)];
    return 0;
}

static uint32_t span_written(void *context, uint32_t address, uint32_t bytes)
{
    uint32_t i;
    (void)context;
    if (address < BASE || address + bytes > END) return 0;
    for (i = 0; i < bytes; ++i)
        if (!written[address - BASE + i]) return 0;
    return 1;
}

static void keep_low_mark(ff_pfifo_status *status)
{
    ff_pfifo_status_init(status);
    status->cache1_status = FF_PFIFO_CACHE1_STATUS_LOW_MARK;
    status->runout_status = FF_PFIFO_RUNOUT_STATUS_LOW_MARK;
}

int main(void)
{
    ff_pfifo_pusher pusher;
    ff_pfifo_status status;

    reset_memory();
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE);
    assert(pusher.classification == FF_PFIFO_PUSH_OK);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_EMPTY);
    assert(pusher.get == BASE && pusher.words_consumed == 0 && pusher.consumer_updates == 0);

    /* One increasing-method packet with two parameters. */
    ram[word_offset(BASE)] = (2u << 18);
    ram[word_offset(BASE) + 1] = 0xAAAA0001u;
    ram[word_offset(BASE) + 2] = 0xAAAA0002u;
    keep_low_mark(&status);
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 12u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, &status, 64u) ==
           FF_PFIFO_PUSH_OK);
    assert(pusher.get == BASE + 12u);
    assert(pusher.words_consumed == 3u);
    assert(pusher.consumer_updates == 2u);
    assert(pusher.last_header == (2u << 18));
    assert(status.consumer_updates == 2u);
    assert(status.cache1_status & FF_PFIFO_CACHE1_STATUS_LOW_MARK);

    /* A non-increasing header is the same packet shape. */
    ram[word_offset(BASE)] = 0x40000000u | (1u << 18);
    ram[word_offset(BASE) + 1] = 0xBBBB0001u;
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 8u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_OK);
    assert(pusher.get == BASE + 8u && pusher.words_consumed == 2u);

    /* GET outside the ring (the observed zero GET) is refused unchanged. */
    ff_pfifo_pusher_init(&pusher, BASE, END, 0u, BASE + 8u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_GET_OUTSIDE);
    assert(pusher.get == 0u && pusher.words_consumed == 0u);
    assert(pusher.classification == FF_PFIFO_PUSH_GET_OUTSIDE);

    /* PUT outside, unaligned pointers and an invalid ring are refused. */
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, END + 4u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_PUT_OUTSIDE);
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE + 2u, BASE + 8u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_GET_OUTSIDE);
    ff_pfifo_pusher_init(&pusher, BASE + 2u, END, BASE + 2u, BASE + 8u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_RING);
    ff_pfifo_pusher_init(&pusher, END, BASE, BASE, BASE);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_RING);

    /* A packet whose span was not CPU-written is refused. */
    ram[word_offset(BASE)] = (1u << 18);
    written[2] = 0;
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 8u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_SPAN_NOT_WRITTEN);
    assert(pusher.get == BASE);
    written[2] = 1;

    /* Unknown, call and return words halt. */
    ram[word_offset(BASE)] = 0xFFFFFFFFu;
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 8u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_UNKNOWN_WORD);
    ram[word_offset(BASE)] = 0x00000002u;
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 8u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_CALL_RETURN);
    ram[word_offset(BASE)] = 0x00020000u;
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 8u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_CALL_RETURN);

    /* A jump and a ring jump set GET to the encoded target. */
    ram[word_offset(BASE)] = 0x00001001u; /* jump to BASE */
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 8u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_OK);
    assert(pusher.get == BASE && pusher.words_consumed == 1u);
    ram[word_offset(BASE)] = 0x20001000u; /* ring jump to BASE */
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 8u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_OK);
    assert(pusher.get == BASE);
    ram[word_offset(BASE)] = 0x00001001u;
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 8u);
    ram[word_offset(BASE)] = 0x00002001u; /* target END is outside [BASE, END) */
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_RING);

    /* Budget and protection bounds. */
    ram[word_offset(BASE)] = (3u << 18);
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 16u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 2u) ==
           FF_PFIFO_PUSH_BUDGET);
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 4u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_PROTECTION);
    ff_pfifo_pusher_init(&pusher, END - 4u, END, END - 4u, END);
    ram[word_offset(END - 4u)] = (1u << 18);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_PROTECTION);

    /* A read failure is classified, and no parameters are written anywhere. */
    ff_pfifo_pusher_init(&pusher, END - 4u, END, END - 4u, END);
    ram[word_offset(END - 4u)] = 0u;
    assert(ff_pfifo_pusher_step(&pusher, NULL, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_READ_FAILED);
    assert(ram[word_offset(BASE)] == 0x00001001u || ram[word_offset(BASE)] != 0xDEADBEEFu);

    /* A NULL status still advances, without seam updates. */
    ram[word_offset(BASE)] = 0u;
    ff_pfifo_pusher_init(&pusher, BASE, END, BASE, BASE + 4u);
    assert(ff_pfifo_pusher_step(&pusher, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_OK);
    assert(pusher.consumer_updates == 0u && pusher.get == BASE + 4u);

    ff_pfifo_pusher_init(NULL, BASE, END, BASE, BASE);
    assert(ff_pfifo_pusher_step(NULL, read_word, span_written, NULL, NULL, 64u) ==
           FF_PFIFO_PUSH_RING);

    printf("pfifo_pusher: ok\n");
    return 0;
}
