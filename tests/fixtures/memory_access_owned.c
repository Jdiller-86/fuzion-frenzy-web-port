/* Independently written emitter-input fixture, not an original game routine. */
static uint32_t owned_memory_and(uint32_t eax, uint32_t edi)
{
    eax = eax & MEM32(edi);
    return eax;
}
static uint32_t owned_memory_expression(uint32_t source, uint32_t destination, uint32_t stack)
{
    uint32_t result = MEM32(source + 1);
    MEM32(destination + 3) = result;
    MEM16(destination + 7) += 2;
    PUSH32(stack, MEM32(destination + 3));
    POP32(stack, result);
    return result;
}
static void owned_memory_bulk(uint32_t edi, uint32_t esi, uint32_t ecx)
{
    uint64_t _n = (uint64_t)ecx * 4u;
    uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi);
    memcpy(_d, _s, (size_t)_n);
}
/* Two labelled blocks exercise site attribution. The gotos keep both labels
 * used under -Wall -Wextra -Werror; they do not model original control flow. */
static uint32_t owned_memory_site(uint32_t choose, uint32_t edi)
{
    if (choose) goto loc_00100010;
    goto loc_00100000;
loc_00100000: ;
    return MEM32(edi);
loc_00100010: ;
    return MEM16(edi + 3);
}
/* A float/double pair at a third label keeps the width-specific site macros
 * compiled; the values are irrelevant to attribution. */
static double owned_memory_site_float(uint32_t edi)
{
    goto loc_00100020;
loc_00100020: ;
    return MEMF(edi) + MEMD(edi + 8);
}
