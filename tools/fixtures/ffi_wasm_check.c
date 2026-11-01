/* Source-owned check of the foreign-call ABI needed by the TCG interpreter. */
#include <stdint.h>
#include <stdio.h>
#include <ffi.h>

static int32_t add32(int32_t a, int32_t b) { return a + b; }
static uint64_t add64(uint64_t a, uint64_t b) { return a + b; }
static double blend(double a, double b) { return a * 2.0 + b; }

int main(void)
{
    ffi_cif cif;
    ffi_type *types[2];
    void *values[2];
    int32_t a = -7, b = 19, result32 = 0;
    types[0] = types[1] = &ffi_type_sint32; values[0] = &a; values[1] = &b;
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 2, &ffi_type_sint32, types) != FFI_OK) return 1;
    ffi_call(&cif, FFI_FN(add32), &result32, values);
    if (result32 != 12) return 2;

    uint64_t x = UINT64_C(0x100000000), y = 7, result64 = 0;
    types[0] = types[1] = &ffi_type_uint64; values[0] = &x; values[1] = &y;
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 2, &ffi_type_uint64, types) != FFI_OK) return 3;
    ffi_call(&cif, FFI_FN(add64), &result64, values);
    if (result64 != UINT64_C(0x100000007)) return 4;

    double c = 1.25, d = -0.5, result_double = 0;
    types[0] = types[1] = &ffi_type_double; values[0] = &c; values[1] = &d;
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 2, &ffi_type_double, types) != FFI_OK) return 5;
    ffi_call(&cif, FFI_FN(blend), &result_double, values);
    if (result_double != 2.0) return 6;
    puts("Wasm libffi: signed32, uint64 and double call ABI checks passed");
    return 0;
}
