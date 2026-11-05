#include "pfifo_pusher.h"

static uint32_t aligned_ring(const ff_pfifo_pusher *pusher)
{
    return pusher->base < pusher->end && !(pusher->base & 3u) && !(pusher->end & 3u) &&
           pusher->end - pusher->base <= FF_PFIFO_PUSHER_MAX_BYTES;
}

static uint32_t inside_ring(const ff_pfifo_pusher *pusher, uint32_t address)
{
    return address >= pusher->base && address < pusher->end;
}

static void low_mark_clear(ff_pfifo_status *status)
{
    uint32_t value;
    if (!status) return;
    value = ff_pfifo_status_read(status, FF_PFIFO_CACHE1_STATUS_OFFSET);
    if (value == UINT32_MAX) return;
    (void)ff_pfifo_status_consumer_update(status, FF_PFIFO_CACHE1_STATUS_OFFSET,
                                          value & ~FF_PFIFO_CACHE1_STATUS_LOW_MARK);
}

static void low_mark_set(ff_pfifo_status *status)
{
    uint32_t value;
    if (!status) return;
    value = ff_pfifo_status_read(status, FF_PFIFO_CACHE1_STATUS_OFFSET);
    if (value == UINT32_MAX) return;
    (void)ff_pfifo_status_consumer_update(status, FF_PFIFO_CACHE1_STATUS_OFFSET,
                                          value | FF_PFIFO_CACHE1_STATUS_LOW_MARK);
}

void ff_pfifo_pusher_init(ff_pfifo_pusher *pusher, uint32_t base, uint32_t end,
                          uint32_t get, uint32_t put)
{
    if (!pusher) return;
    pusher->base = base;
    pusher->end = end;
    pusher->get = get;
    pusher->put = put;
    pusher->words_consumed = 0;
    pusher->consumer_updates = 0;
    pusher->last_header = 0;
    pusher->classification = FF_PFIFO_PUSH_OK;
}

static uint32_t dispatch(ff_pfifo_pusher *pusher, ff_pfifo_status *status, uint32_t words)
{
    /* The pinned pusher clears LOW_MARK before a dispatch and sets it again
     * after processing, so an empty cache keeps its reset value. */
    low_mark_clear(status);
    if (status) ++pusher->consumer_updates;
    pusher->words_consumed += words;
    low_mark_set(status);
    if (status) ++pusher->consumer_updates;
    return FF_PFIFO_PUSH_OK;
}

uint32_t ff_pfifo_pusher_step(ff_pfifo_pusher *pusher, ff_pfifo_read_word_fn read_word,
                              ff_pfifo_span_written_fn span_written, void *context,
                              ff_pfifo_status *status, uint32_t max_words)
{
    uint32_t header, count, bytes, target;

    if (!pusher) return FF_PFIFO_PUSH_RING;
    pusher->classification = FF_PFIFO_PUSH_RING;
    if (!aligned_ring(pusher)) return FF_PFIFO_PUSH_RING;
    if ((pusher->get & 3u) || pusher->get < pusher->base || pusher->get > pusher->end)
        return pusher->classification = FF_PFIFO_PUSH_GET_OUTSIDE;
    if ((pusher->put & 3u) || pusher->put < pusher->base || pusher->put > pusher->end)
        return pusher->classification = FF_PFIFO_PUSH_PUT_OUTSIDE;
    if (pusher->get == pusher->put) return pusher->classification = FF_PFIFO_PUSH_EMPTY;
    if (!max_words || max_words > FF_PFIFO_PUSHER_MAX_WORDS)
        return pusher->classification = FF_PFIFO_PUSH_BUDGET;
    if (!read_word || read_word(context, pusher->get, &header))
        return pusher->classification = FF_PFIFO_PUSH_READ_FAILED;
    pusher->last_header = header;

    /* A call or a return leaves the modeled subset: halt and report. */
    if ((header & 3u) == 2u || (header & 0xFFFF0003u) == 0x00020000u)
        return pusher->classification = FF_PFIFO_PUSH_CALL_RETURN;

    if ((header & 3u) == 1u || (header & 0xE0000003u) == 0x20000000u) {
        target = ((header & 0xE0000003u) == 0x20000000u)
                     ? (header & 0x1FFFFFFCu)
                     : (header & 0xFFFFFFFCu);
        if (!inside_ring(pusher, target)) return pusher->classification = FF_PFIFO_PUSH_RING;
        if (!span_written || !span_written(context, pusher->get, 4u))
            return pusher->classification = FF_PFIFO_PUSH_SPAN_NOT_WRITTEN;
        pusher->get = target;
        return dispatch(pusher, status, 1u);
    }

    if ((header & 0xE0030003u) != 0u && (header & 0xE0030003u) != 0x40000000u)
        return pusher->classification = FF_PFIFO_PUSH_UNKNOWN_WORD;

    count = (header >> 18) & 0x7FFu;
    if (count + 1u > max_words) return pusher->classification = FF_PFIFO_PUSH_BUDGET;
    bytes = (count + 1u) * 4u;
    if (pusher->get + bytes > pusher->end) return pusher->classification = FF_PFIFO_PUSH_PROTECTION;
    /* A packet may not cross the submitted PUT: that would read unwritten
     * space. A wrapped ring is not inferred from pointers. */
    if (pusher->get < pusher->put && pusher->get + bytes > pusher->put)
        return pusher->classification = FF_PFIFO_PUSH_PROTECTION;
    if (!span_written || !span_written(context, pusher->get, bytes))
        return pusher->classification = FF_PFIFO_PUSH_SPAN_NOT_WRITTEN;

    pusher->get += bytes;
    return dispatch(pusher, status, count + 1u);
}
