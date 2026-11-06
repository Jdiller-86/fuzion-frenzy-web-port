#ifndef FF_RECOMP_SERVICES_H
#define FF_RECOMP_SERVICES_H

#include <stdint.h>

/* Bounded record of guest debug-service traps (INT 0x2D).
 *
 * The pinned translator turns the trap into a call to recomp_debug_service();
 * its retained implementation documents service 1 as "print this ANSI_STRING"
 * and consumes the trap without touching a guest register. This module keeps
 * the same guest-visible effect (none) while recording the call for evidence,
 * because the browser probe has no stderr sink. */
#define FF_DEBUG_SERVICE_CAPACITY 8

typedef struct
{
    uint32_t service;
    uint32_t argument;
    uint32_t length;
    uint32_t buffer;
} ff_debug_service_call;

typedef struct
{
    const uint8_t *ram;
    uint32_t bytes;
} ff_debug_service_memory;

typedef struct
{
    uint32_t count;
    uint32_t dropped;
    ff_debug_service_call calls[FF_DEBUG_SERVICE_CAPACITY];
} ff_debug_service_log;

/* 0 recorded; the remaining values report why a descriptor could not be kept. */
#define FF_DEBUG_SERVICE_OK 0u
#define FF_DEBUG_SERVICE_INVALID 1u
#define FF_DEBUG_SERVICE_DROPPED 2u
#define FF_DEBUG_SERVICE_UNREADABLE 3u

void ff_debug_service_reset(ff_debug_service_log *log);
uint32_t ff_debug_service_record(ff_debug_service_log *log,
                                 const ff_debug_service_memory *memory,
                                 uint32_t service,
                                 uint32_t argument);

#endif
