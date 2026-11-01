"""Owned C-emitter surfaces only; no title bytes or private tooling required."""
import importlib.util
from pathlib import Path
import unittest

spec = importlib.util.spec_from_file_location("memory_access", Path(__file__).resolve().parents[1] / "tools/recomp_memory_access.py")
guard = importlib.util.module_from_spec(spec); spec.loader.exec_module(guard)


class MemoryAccessTests(unittest.TestCase):
    def test_scalar_direction_width_nesting_and_comparisons(self):
        source = 'eax = MEM32(ebp + 4); MEM16(edi) = eax; MEM8(esi) += 1; ++MEM32(edi); if (MEM8(esi) == 0) eax = SMEM64(MEM32(ebp));'
        result, counts = guard.instrument_memory_accesses(source)
        self.assertIn('FF_MEMORY_U32((ebp + 4), 1u)', result)
        self.assertIn('FF_MEMORY_U16((edi), 2u)', result)
        self.assertIn('FF_MEMORY_U8((esi), 3u)', result)
        self.assertIn('++FF_MEMORY_U32((edi), 3u)', result)
        self.assertIn('FF_MEMORY_I64((FF_MEMORY_U32((ebp), 1u)), 1u)', result)
        self.assertEqual((counts['reads'], counts['writes'], counts['readWrites']), (4, 1, 2))

    def test_comments_literals_stack_and_bulk_extents(self):
        source = '/* MEM32(bad) */ const char *s = "MEM8(bad)"; PUSH32(esp, MEM32(ebp)); POP32(esp, eax);\n'
        source += '{ uint64_t _n = (uint64_t)ecx * 4u; uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); memcpy(_d, _s, _n); }'
        result, counts = guard.instrument_memory_accesses(source)
        self.assertIn('/* MEM32(bad) */', result); self.assertIn('"MEM8(bad)"', result)
        self.assertIn('FF_MEMORY_PUSH(esp, FF_MEMORY_U32((ebp), 1u))', result)
        self.assertIn('FF_MEMORY_POINTER(edi, _n, 2u)', result)
        self.assertIn('FF_MEMORY_POINTER(esi, _n, 1u)', result)
        self.assertEqual((counts['stack'], counts['bulkRanges']), (2, 2))
        result, counts = guard.instrument_memory_accesses('memset((void*)XBOX_PTR(edi), (uint8_t)eax, ecx);')
        self.assertIn('FF_MEMORY_POINTER(edi, ecx, 2u)', result)

    def test_unknown_helpers_escaped_pointers_and_malformed_surfaces_fail_closed(self):
        for source in ['&MEM32(eax)', 'XBOX_PTR(eax)', 'XMM_MEM(eax)', 'MMX_STORE(eax, value)',
                       '__atomic_fetch_add(XBOX_PTR(eax), 1, 0)', 'MEM80(eax)', 'MEM32(', 'MEM32()', 'MEM32;',
                       '/* unclosed', '"unclosed',
                       'uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi);']:
            with self.subTest(source=source), self.assertRaises(ValueError): guard.instrument_memory_accesses(source)


if __name__ == '__main__': unittest.main()
