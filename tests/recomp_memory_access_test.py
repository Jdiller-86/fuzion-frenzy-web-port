"""Owned C-emitter surfaces only; no title bytes or private tooling required."""
import importlib.util
from pathlib import Path
import unittest

spec = importlib.util.spec_from_file_location("memory_access", Path(__file__).resolve().parents[1] / "tools/recomp_memory_access.py")
guard = importlib.util.module_from_spec(spec); spec.loader.exec_module(guard)

SITE = 0x00100000


class MemoryAccessTests(unittest.TestCase):
    def test_register_and_reads_remain_checked_without_admitting_pointer_escape(self):
        result, counts = guard.instrument_memory_accesses('eax = eax & MEM32(edi);', SITE)
        self.assertEqual(result, 'eax = eax & FF_MEMORY_SITE_U32((edi), 1u, 0x00100000u);')
        self.assertEqual(counts['reads'], 1)
        for source in ['eax = &MEM32(edi);', 'eax = eax & &MEM32(edi);',
                       'return &MEM32(edi);', 'call(&MEM32(edi));', 'eax = other & MEM32(edi);']:
            with self.subTest(source=source), self.assertRaises(ValueError):
                guard.instrument_memory_accesses(source, SITE)

    def test_scalar_direction_width_nesting_and_comparisons(self):
        source = 'eax = MEM32(ebp + 4); MEM16(edi) = eax; MEM8(esi) += 1; ++MEM32(edi); if (MEM8(esi) == 0) eax = SMEM64(MEM32(ebp));'
        result, counts = guard.instrument_memory_accesses(source, SITE)
        self.assertIn('FF_MEMORY_SITE_U32((ebp + 4), 1u, 0x00100000u)', result)
        self.assertIn('FF_MEMORY_SITE_U16((edi), 2u, 0x00100000u)', result)
        self.assertIn('FF_MEMORY_SITE_U8((esi), 3u, 0x00100000u)', result)
        self.assertIn('++FF_MEMORY_SITE_U32((edi), 3u, 0x00100000u)', result)
        self.assertIn('FF_MEMORY_SITE_I64((FF_MEMORY_SITE_U32((ebp), 1u, 0x00100000u)), 1u, 0x00100000u)', result)
        self.assertEqual((counts['reads'], counts['writes'], counts['readWrites']), (4, 1, 2))
        self.assertEqual(counts['sites'], 1)

    def test_comments_literals_stack_and_bulk_extents(self):
        source = '/* MEM32(bad) */ const char *s = "MEM8(bad)"; PUSH32(esp, MEM32(ebp)); POP32(esp, eax);\n'
        source += '{ uint64_t _n = (uint64_t)ecx * 4u; uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); memcpy(_d, _s, _n); }'
        result, counts = guard.instrument_memory_accesses(source, SITE)
        self.assertIn('/* MEM32(bad) */', result); self.assertIn('"MEM8(bad)"', result)
        self.assertIn('FF_MEMORY_PUSH(esp, FF_MEMORY_SITE_U32((ebp), 1u, 0x00100000u))', result)
        self.assertIn('FF_MEMORY_POINTER(edi, _n, 2u)', result)
        self.assertIn('FF_MEMORY_POINTER(esi, _n, 1u)', result)
        self.assertEqual((counts['stack'], counts['bulkRanges']), (2, 2))
        result, counts = guard.instrument_memory_accesses('memset((void*)XBOX_PTR(edi), (uint8_t)eax, ecx);', SITE)
        self.assertIn('FF_MEMORY_POINTER(edi, ecx, 2u)', result)

    def test_block_sites_follow_generated_labels_and_entry_fallback(self):
        source = ('eax = MEM32(ebp);\n'
                  'loc_00100010: ;\nMEM16(edi) = eax;\n'
                  'loc_00100020: ;\nMEM8(esi) += 1;')
        result, counts = guard.instrument_memory_accesses(source, SITE, 0x00100030)
        self.assertIn('FF_MEMORY_SITE_U32((ebp), 1u, 0x00100000u)', result)
        self.assertIn('FF_MEMORY_SITE_U16((edi), 2u, 0x00100010u)', result)
        self.assertIn('FF_MEMORY_SITE_U8((esi), 3u, 0x00100020u)', result)
        self.assertEqual(counts['sites'], 3)

    def test_nested_accesses_use_the_block_base_in_a_later_label(self):
        source = ('loc_00100000: ;\neax = MEM32(edi);\n'
                  'loc_00100010: ;\neax = SMEM32(MEM32(ebp));')
        result, counts = guard.instrument_memory_accesses(source, SITE, 0x00100030)
        self.assertIn('FF_MEMORY_SITE_U32((edi), 1u, 0x00100000u)', result)
        self.assertIn('FF_MEMORY_SITE_I32((FF_MEMORY_SITE_U32((ebp), 1u, 0x00100010u)), 1u, 0x00100010u)', result)
        self.assertEqual(counts['sites'], 2)

    def test_string_moves_are_split_before_staging(self):
        for width, ctype, step in (("8", "uint8_t", 1), ("16", "uint16_t", 2), ("32", "uint32_t", 4)):
            source = (f"MEM{width}(edi) = MEM{width}(esi); esi += RECOMP_DF_STEP({step});"
                      f" edi += RECOMP_DF_STEP({step});")
            result, counts = guard.instrument_memory_accesses(source, SITE)
            self.assertIn(f"{{ {ctype} _ff_copy{width} = FF_MEMORY_SITE_U{width}((esi), 1u, 0x00100000u);", result)
            self.assertIn(f"FF_MEMORY_SITE_U{width}((edi), 2u, 0x00100000u) = _ff_copy{width}; }}", result)
            self.assertEqual((counts['reads'], counts['writes']), (1, 1))

    def test_write_value_expressions_are_split_before_staging(self):
        cases = [
            ('MEM32(edx) = MEM32(esi);',
             'uint32_t _ff_wtmp0 = FF_MEMORY_SITE_U32((esi), 1u, 0x00100000u);',
             'FF_MEMORY_SITE_U32((edx), 2u, 0x00100000u) = _ff_wtmp0;'),
            ('MEM32(edi) += MEM16(esi);',
             'uint32_t _ff_wtmp0 = FF_MEMORY_SITE_U16((esi), 1u, 0x00100000u);',
             'FF_MEMORY_SITE_U32((edi), 3u, 0x00100000u) += _ff_wtmp0;'),
            ('MEM32(edi) = (MEM32(esi) + 1);',
             'uint32_t _ff_wtmp0 = (FF_MEMORY_SITE_U32((esi), 1u, 0x00100000u) + 1);',
             'FF_MEMORY_SITE_U32((edi), 2u, 0x00100000u) = _ff_wtmp0;'),
            ('MEM32(ebp + -3068) = MEM32(ebp + -3068) & 0;',
             'uint32_t _ff_wtmp0 = FF_MEMORY_SITE_U32((ebp + -3068), 1u, 0x00100000u) & 0;',
             'FF_MEMORY_SITE_U32((ebp + -3068), 2u, 0x00100000u) = _ff_wtmp0;'),
        ]
        for source, read, write in cases:
            with self.subTest(source=source):
                result, counts = guard.instrument_memory_accesses(source, SITE)
                self.assertIn(read, result)
                self.assertIn(write, result)
                self.assertEqual((counts['reads'], counts['writes'] + counts['readWrites']), (1, 1))
        result, counts = guard.instrument_memory_accesses('MEM32(MEM32(edi)) = 1;', SITE)
        self.assertIn('FF_MEMORY_SITE_U32((edi), 1u, 0x00100000u)', result)
        self.assertIn('FF_MEMORY_SITE_U32((FF_MEMORY_SITE_U32((edi), 1u, 0x00100000u)), 2u, 0x00100000u)', result)
        self.assertEqual(counts['writes'], 1)

    def test_labels_in_comments_and_literals_are_ignored(self):
        source = '/* loc_00100010: */ const char *s = "loc_00100020:";\nloc_00100000: ;\neax = MEM32(edi);'
        result, _ = guard.instrument_memory_accesses(source, SITE, 0x00100030)
        self.assertIn('FF_MEMORY_SITE_U32((edi), 1u, 0x00100000u)', result)
        self.assertNotIn('0x00100010u', result); self.assertNotIn('0x00100020u', result)

    def test_missing_invalid_or_out_of_range_sites_fail_closed(self):
        for site in (0, -1, True, 0x100000000):
            with self.subTest(site=site), self.assertRaises(ValueError):
                guard.instrument_memory_accesses('eax = MEM32(edi);', site)
        for limit in (SITE, SITE - 1, 0x100000001, True, '0x101000'):
            with self.subTest(limit=limit), self.assertRaises(ValueError):
                guard.instrument_memory_accesses('eax = MEM32(edi);', SITE, limit)
        cases = [
            ('loc_000FFFF0: ;\neax = MEM32(edi);', SITE, 0x00100010),      # label precedes function
            ('loc_00100010: ;\neax = MEM32(edi);\nloc_00100000: ;', SITE, 0x00100030),  # descending
            ('loc_00100010: ;\neax = MEM32(edi);', SITE, 0x00100010),      # label at function end
        ]
        for source, site, limit in cases:
            with self.subTest(source=source), self.assertRaises(ValueError):
                guard.instrument_memory_accesses(source, site, limit)
        with self.assertRaises(ValueError):
            guard.instrument_memory_accesses('eax = MEM32(edi);', SITE, 0x00100030)
        result, counts = guard.instrument_memory_accesses('loc_00100000: ;\neax = MEM32(edi);', SITE, 0x00100030)
        self.assertIn('FF_MEMORY_SITE_U32((edi), 1u, 0x00100000u)', result)
        self.assertEqual(counts['sites'], 1)

    def test_checked_xmm_loads_are_rewritten_and_pure_sse_is_allowed(self):
        result, counts = guard.instrument_memory_accesses(
            'loc_00100000: ;\nxmm2 = XMM_MUL(xmm2, XMM_MEM(eax));', SITE, 0x00100030)
        self.assertIn('(*(RecompXmm*)FF_MEMORY_SITE_POINTER((eax), 16u, 1u, 0x00100000u))', result)
        self.assertIn('XMM_MUL', result)
        self.assertEqual(counts['reads'], 1)
        self.assertEqual(counts['sites'], 1)
        stored, store_counts = guard.instrument_memory_accesses(
            'loc_00100000: ;\nXMM_STORE(eax, xmm0);', SITE, 0x00100030)
        self.assertIn('(*(RecompXmm*)FF_MEMORY_SITE_POINTER((eax), 16u, 2u, 0x00100000u)) = (xmm0)', stored)
        self.assertEqual(store_counts['writes'], 1)
        nested, nested_counts = guard.instrument_memory_accesses(
            'loc_00100000: ;\nXMM_STORE(eax, XMM_ADD(xmm0, XMM_MEM(ebx)));', SITE, 0x00100030)
        self.assertIn('_ff_xmmtmp0', nested)
        self.assertIn('FF_MEMORY_SITE_POINTER((ebx), 16u, 1u', nested)
        self.assertEqual(nested_counts['reads'], 1)
        self.assertEqual(nested_counts['writes'], 1)

    def test_unknown_helpers_escaped_pointers_and_malformed_surfaces_fail_closed(self):
        for source in ['&MEM32(eax)', 'XBOX_PTR(eax)', 'XMM_STOREU(eax, value)', 'XMM_LOAD_LOW(xmm0, eax)',
                       'XMM_STORE_LOW(eax, xmm0)', 'XMM_UNKNOWN(eax)', 'MMX_STORE(eax, value)',
                       '__atomic_fetch_add(XBOX_PTR(eax), 1, 0)', 'MEM80(eax)', 'MEM32(', 'MEM32()', 'MEM32;',
                       '/* unclosed', '"unclosed',
                       'uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi);']:
            with self.subTest(source=source), self.assertRaises(ValueError): guard.instrument_memory_accesses(source, SITE)


if __name__ == '__main__': unittest.main()
