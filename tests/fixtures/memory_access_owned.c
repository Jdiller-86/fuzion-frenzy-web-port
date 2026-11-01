/* Independently written emitter-input fixture, not an original game routine. */
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
