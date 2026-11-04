#include "pfifo_channel.h"
#include "thread_bootstrap.h"

static uint32_t valid_range(uint32_t base, uint32_t end)
{
    return !(base & 3u) && !(end & 3u) && end > base && end <= FF_PFIFO_CHANNEL_MAX_BYTES;
}

void ff_pfifo_channel_init(ff_pfifo_channel *channel)
{
    if (!channel) return;
    channel->base = channel->end = channel->put = channel->get = 0;
    channel->ready = channel->enabled = 0;
}

uint32_t ff_pfifo_channel_configure(ff_pfifo_channel *channel, uint32_t base, uint32_t end)
{
    if (!channel || !valid_range(base, end)) return FF_BOOT_INVALID;
    channel->base = base;
    channel->end = end;
    channel->put = base;
    channel->get = 0;
    channel->ready = 1;
    channel->enabled = 0;
    return 0;
}

uint32_t ff_pfifo_channel_enable(ff_pfifo_channel *channel)
{
    if (!channel || !channel->ready) return FF_BOOT_INVALID;
    channel->enabled = 1;
    if (channel->put == channel->base) channel->get = channel->put;
    return 0;
}

uint32_t ff_pfifo_channel_set_put(ff_pfifo_channel *channel, uint32_t put)
{
    if (!channel || !channel->ready || (put & 3u) || put < channel->base || put > channel->end)
        return FF_BOOT_INVALID;
    channel->put = put;
    if (channel->enabled && put == channel->base) channel->get = put;
    return 0;
}

uint32_t ff_pfifo_channel_set_get(ff_pfifo_channel *channel, uint32_t get)
{
    if (!channel || !channel->ready || (get & 3u) || get > FF_PFIFO_CHANNEL_MAX_BYTES)
        return FF_BOOT_INVALID;
    channel->get = get;
    return 0;
}

uint32_t ff_pfifo_channel_read_get(const ff_pfifo_channel *channel)
{
    return channel ? channel->get : 0;
}

uint32_t ff_pfifo_channel_empty(const ff_pfifo_channel *channel)
{
    return channel && channel->ready && channel->put == channel->base;
}

uint32_t ff_pfifo_channel_should_serve_empty_get(const ff_pfifo_channel *channel, uint32_t put)
{
    if (!channel || !channel->ready || !channel->enabled || (put & 3u) ||
        put < channel->base || put > channel->end || channel->put != put) return 0;
    return put == channel->base;
}
