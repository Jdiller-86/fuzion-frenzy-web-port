/* Source-owned callback ABI regression cases for the pinned wasm32 SDK. */
#include <stdint.h>
#include <stdio.h>
#include <emscripten.h>

EM_JS_DEPS(dyncall_check, "$dynCall");
EM_JS(int, call_int, (uintptr_t fn, void *value), {
    return dynCall('ip', fn, [value]);
});
EM_JS(void *, call_pointer, (uintptr_t fn, int offset), {
    return dynCall('pi', fn, [offset]);
});
EM_JS(void, call_void, (uintptr_t fn, void *value), {
    dynCall('vp', fn, [value]);
});
EM_JS(uint64_t, call_long, (uintptr_t fn, void *value), {
    return dynCall('jp', fn, [value]);
});
EM_JS(double, call_double, (uintptr_t fn, double a, double b), {
    return dynCall('ddd', fn, [a, b]);
});

static char buffer[8];
static int load(void *value) { return *(int *)value; }
static void *address(int offset) { return buffer + offset; }
static void increment(void *value) { ++*(int *)value; }
static uint64_t wide(void *value) { return UINT64_C(0x100000000) + *(int *)value; }
static double blend(double a, double b) { return a * 2.0 + b; }

int main(void)
{
    int value = 17;
    if (call_int((uintptr_t)load, &value) != 17) return 1;
    if (call_pointer((uintptr_t)address, 5) != buffer + 5) return 2;
    call_void((uintptr_t)increment, &value);
    if (value != 18) return 3;
    if (call_long((uintptr_t)wide, &value) != UINT64_C(0x100000012)) return 4;
    if (call_double((uintptr_t)blend, 1.25, -0.5) != 2.0) return 5;
    puts("Wasm dynamic calls: pointer args/return, void, uint64 and double checks passed");
    return 0;
}
