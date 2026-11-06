#ifndef FF_THREAD_BOOTSTRAP_H
#define FF_THREAD_BOOTSTRAP_H
#include <stdint.h>

/* Independently authored, bounded HLE boot-thread context. No host threads,
 * waits, preemption, exception dispatch or complete kernel-object ABI yet. */
#define FF_BOOT_MAX_THREADS 4u
#define FF_BOOT_OK 0u
#define FF_BOOT_INVALID 0xc000000du
#define FF_BOOT_NO_MEMORY 0xc0000017u
#define FF_BOOT_BAD_HANDLE 0xc0000008u
#define FF_BOOT_UNSUPPORTED 0xc00000bbu
enum { FF_BOOT_READY = 1, FF_BOOT_SUSPENDED, FF_BOOT_RUNNING };
typedef struct {
    uint32_t id, handle, handle_open, state, object, fs, stack_low, stack_top;
    uint32_t tls, tls_size, routine, context, system, initial_sp;
} ff_boot_thread;
typedef struct {
    uint8_t *ram;
    uint32_t bytes, cursor, end, count, current;
    ff_boot_thread threads[FF_BOOT_MAX_THREADS];
} ff_boot_runtime;

uint32_t ff_boot_init(ff_boot_runtime *, uint8_t *, uint32_t, uint32_t, uint32_t);
uint32_t ff_boot_create(ff_boot_runtime *, const uint32_t args[10]);
uint32_t ff_boot_close(ff_boot_runtime *, uint32_t handle);
ff_boot_thread *ff_boot_next(ff_boot_runtime *);
/* Recursive uncontended critical sections only; contention stops the probe. */
uint32_t ff_boot_enter_critical(ff_boot_runtime *, uint32_t address);
uint32_t ff_boot_leave_critical(ff_boot_runtime *, uint32_t address);
#endif
