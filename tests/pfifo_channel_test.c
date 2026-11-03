/* Owned empty-ring PFIFO channel scenarios; no original code or device. */
#include "../src/runtime/pfifo_channel.h"
#include "../src/runtime/thread_bootstrap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    ff_pfifo_channel channel;
    memset(&channel, 0xa5, sizeof channel);
    ff_pfifo_channel_init(&channel);
    assert(!channel.ready && !channel.enabled);

    assert(ff_pfifo_channel_configure(&channel, 0x03B7D000u, 0x03BFD000u) == 0);
    assert(channel.ready && channel.base == 0x03B7D000u && channel.end == 0x03BFD000u);
    assert(channel.put == 0x03B7D000u && channel.get == 0);
    assert(ff_pfifo_channel_empty(&channel) == 1);

    assert(ff_pfifo_channel_set_get(&channel, 0) == 0);
    assert(ff_pfifo_channel_enable(&channel) == 0);
    assert(channel.enabled);
    assert(ff_pfifo_channel_read_get(&channel) == 0x03B7D000u);

    assert(ff_pfifo_channel_set_put(&channel, 0x03B7D400u) == 0);
    assert(!ff_pfifo_channel_empty(&channel));
    assert(ff_pfifo_channel_read_get(&channel) == 0x03B7D000u);

    assert(ff_pfifo_channel_set_put(&channel, 0x03B7D000u) == 0);
    assert(ff_pfifo_channel_empty(&channel));
    assert(ff_pfifo_channel_read_get(&channel) == 0x03B7D000u);

    ff_pfifo_channel_init(&channel);
    assert(ff_pfifo_channel_configure(&channel, 0x1000u, 0x2000u) == 0);
    assert(ff_pfifo_channel_set_put(&channel, 0x1800u) == 0);
    assert(ff_pfifo_channel_enable(&channel) == 0);
    assert(ff_pfifo_channel_read_get(&channel) == 0);

    assert(ff_pfifo_channel_configure(NULL, 0x1000u, 0x2000u) == FF_BOOT_INVALID);
    assert(ff_pfifo_channel_enable(NULL) == FF_BOOT_INVALID);
    assert(ff_pfifo_channel_set_put(NULL, 0x1000u) == FF_BOOT_INVALID);
    assert(ff_pfifo_channel_set_get(NULL, 0) == FF_BOOT_INVALID);
    ff_pfifo_channel_init(&channel);
    assert(ff_pfifo_channel_configure(&channel, 0x1002u, 0x2000u) == FF_BOOT_INVALID);
    assert(ff_pfifo_channel_configure(&channel, 0x2000u, 0x2000u) == FF_BOOT_INVALID);
    assert(ff_pfifo_channel_configure(&channel, 0x1000u, 0x4000004u) == FF_BOOT_INVALID);
    assert(ff_pfifo_channel_configure(&channel, 0x1000u, 0x2000u) == 0);
    assert(ff_pfifo_channel_set_put(&channel, 0x0FFCu) == FF_BOOT_INVALID);
    assert(ff_pfifo_channel_set_put(&channel, 0x2004u) == FF_BOOT_INVALID);
    assert(ff_pfifo_channel_set_put(&channel, 0x1002u) == FF_BOOT_INVALID);
    assert(ff_pfifo_channel_set_get(&channel, 0x4000004u) == FF_BOOT_INVALID);
    assert(ff_pfifo_channel_set_get(&channel, 0x1002u) == FF_BOOT_INVALID);
    puts("owned PFIFO channel: empty-ring GET initialization, put/get updates and rejection passed");
    return 0;
}
