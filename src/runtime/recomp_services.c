#include "recomp_services.h"

static const uint8_t *checked(const ff_debug_service_memory *memory, uint32_t address, uint32_t bytes)
{
    if (!memory || !memory->ram || address > memory->bytes || bytes > memory->bytes - address) return 0;
    return memory->ram + address;
}

static uint32_t word(const uint8_t *bytes, uint32_t offset)
{
    return (uint32_t)bytes[offset] | (uint32_t)bytes[offset + 1u] << 8 |
           (uint32_t)bytes[offset + 2u] << 16 | (uint32_t)bytes[offset + 3u] << 24;
}

void ff_debug_service_reset(ff_debug_service_log *log)
{
    if (!log) return;
    log->count = 0;
    log->dropped = 0;
    for (unsigned i = 0; i < FF_DEBUG_SERVICE_CAPACITY; ++i) {
        log->calls[i].service = 0;
        log->calls[i].argument = 0;
        log->calls[i].length = 0;
        log->calls[i].buffer = 0;
        log->calls[i].textBytes = 0;
        for (unsigned b = 0; b < FF_DEBUG_SERVICE_TEXT_BYTES; ++b) log->calls[i].text[b] = 0;
    }
}

uint32_t ff_debug_service_record(ff_debug_service_log *log,
                                 const ff_debug_service_memory *memory,
                                 uint32_t service,
                                 uint32_t argument)
{
    if (!log) return FF_DEBUG_SERVICE_INVALID;
    if (log->count >= FF_DEBUG_SERVICE_CAPACITY) {
        ++log->dropped;
        return FF_DEBUG_SERVICE_DROPPED;
    }
    ff_debug_service_call *call = &log->calls[log->count++];
    call->service = service;
    call->argument = argument;
    call->length = 0;
    call->buffer = 0;
    call->textBytes = 0;
    for (unsigned b = 0; b < FF_DEBUG_SERVICE_TEXT_BYTES; ++b) call->text[b] = 0;
    /* The retained translator prints service 1 and reports every other
     * service as ignored; both leave the guest registers untouched. */
    if (service != 1u || !argument) return FF_DEBUG_SERVICE_OK;
    const uint8_t *descriptor = checked(memory, argument, 8u);
    if (!descriptor) return FF_DEBUG_SERVICE_UNREADABLE;
    uint32_t length = (uint32_t)descriptor[0] | (uint32_t)descriptor[1] << 8;
    uint32_t buffer = word(descriptor, 4u);
    call->length = length;
    call->buffer = buffer;
    if (!length || !buffer) return FF_DEBUG_SERVICE_OK;
    if (!checked(memory, buffer, length)) {
        call->length = 0;
        return FF_DEBUG_SERVICE_UNREADABLE;
    }
    uint32_t copied = length < FF_DEBUG_SERVICE_TEXT_BYTES ? length : FF_DEBUG_SERVICE_TEXT_BYTES;
    for (uint32_t i = 0; i < copied; ++i) call->text[i] = memory->ram[buffer + i];
    call->textBytes = copied;
    return FF_DEBUG_SERVICE_OK;
}
