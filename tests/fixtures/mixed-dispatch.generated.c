/**
 * owned_mixed_dispatch
 * Original: 0x00011000 - 0x00011060 (96 bytes, 41 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void owned_mixed_dispatch(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = 0; /* carry flag */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00011000: ;
    eax = MEM32(esp + 4);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(7) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 7 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_A(_fa, _fb)) goto loc_00011050; /* ja: above (unsigned >) */

loc_00011009: ;
    { uint32_t _jt = MEM32(eax * 4 + 0x11030); /* switch: 8 entries, 4 targets */
    if (_jt == 0x00011010u) goto loc_00011010;
    if (_jt == 0x00011018u) goto loc_00011018;
    if (_jt == 0x00011020u) goto loc_00011020;
    if (_jt == 0x00011028u) goto loc_00011028;
    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }

loc_00011010: ;
    eax = 1;
    esp += 12; return; /* ret 8 */

loc_00011018: ;
    eax = 2;
    esp += 12; return; /* ret 8 */

loc_00011020: ;
    eax = 3;
    esp += 12; return; /* ret 8 */

loc_00011028: ;
    eax = 4;
    esp += 12; return; /* ret 8 */

loc_00011050: ;
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    esp += 12; return; /* ret 8 */

}
