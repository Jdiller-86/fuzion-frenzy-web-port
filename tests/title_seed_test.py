"""Source-owned adversarial cases for bounded function-envelope admission."""
import copy
import importlib.util
from pathlib import Path
import unittest

spec = importlib.util.spec_from_file_location("title_prepare", Path(__file__).resolve().parents[1] / "tools/prepare-title-recomp.py")
prepare = importlib.util.module_from_spec(spec)
spec.loader.exec_module(prepare)


def fixture():
    return {"established": True, "entry": "100", "bodyBytes": 8, "envelopeBytes": 12,
            "bodyRanges": [{"start": "100", "endInclusive": "103"}, {"start": "108", "endInclusive": "10b"}]}


class EnvelopeTests(unittest.TestCase):
    def test_only_declared_kernel_jump_thunks_are_admitted(self):
        item = {"established": True, "thunk": True, "entry": "100", "bodyBytes": 6,
                "bodyRanges": [{"start": "100", "endInclusive": "105"}],
                "instructions": [{"mnemonic": "JMP", "computed": True,
                                  "references": [{"type": "INDIRECTION", "target": "800"}]}]}
        self.assertEqual(prepare.function_envelope(item, {0x800: 1}), (0x100, 0x106))
        for bindings in [None, {}, {0x804: 1}]:
            with self.assertRaises(ValueError):
                prepare.function_envelope(item, bindings)
        item["instructions"][0]["mnemonic"] = "CALL"
        with self.assertRaises(ValueError):
            prepare.function_envelope(item, {0x800: 1})

    def test_split_body_keeps_its_entire_exported_envelope(self):
        self.assertEqual(prepare.function_envelope(fixture()), (0x100, 0x10c))

    def test_legacy_contiguous_seed_remains_valid(self):
        item = fixture()
        item.pop("envelopeBytes")
        item["bodyRanges"] = [{"start": "100", "endInclusive": "107"}]
        self.assertEqual(prepare.function_envelope(item), (0x100, 0x108))

    def test_split_body_without_matching_envelope_is_rejected(self):
        for size in [None, 8, 11, 13]:
            item = fixture()
            item["envelopeBytes"] = size
            with self.subTest(size=size), self.assertRaises(ValueError):
                prepare.function_envelope(item)


class SeedCompositionTests(unittest.TestCase):
    def seed(self, entry="100", sha="a" * 64):
        item = fixture()
        item["entry"] = entry
        if entry != "100":
            start = int(entry, 16)
            item.update(bodyBytes=4, envelopeBytes=4, bodyRanges=[{"start": entry, "endInclusive": f"{start + 3:x}"}])
        return {"kind": "bounded-ghidra-recompilation-seed", "sourceSha256": sha, "functions": [item]}

    def test_composition_preserves_all_functions_and_order(self):
        items = prepare.combine_seed_functions([self.seed(), self.seed("200")], "a" * 64)
        self.assertEqual([item["entry"] for item in items], ["100", "200"])

    def test_duplicate_or_foreign_build_never_replaces_existing_code(self):
        for seeds in [[self.seed(), self.seed()], [self.seed(), self.seed("200", "b" * 64)]]:
            with self.assertRaises(ValueError):
                prepare.combine_seed_functions(seeds, "a" * 64)

    def test_seed_count_function_count_and_kind_are_bounded(self):
        oversized = self.seed()
        oversized["functions"] *= 9
        wrong = self.seed()
        wrong["kind"] = "unrelated"
        for seeds in [[], [self.seed()] * 17, [oversized], [wrong]]:
            with self.assertRaises(ValueError):
                prepare.combine_seed_functions(seeds, "a" * 64)

    def test_more_exports_retain_the_original_total_function_budget(self):
        seeds = []
        for group in range(16):
            seed = self.seed()
            seed["functions"] = [self.seed(f"{0x100 + (group * 4 + item) * 0x20:x}")["functions"][0] for item in range(4)]
            seeds.append(seed)
        items = prepare.combine_seed_functions(seeds, "a" * 64)
        self.assertEqual(len(items), 64)
        self.assertEqual(len({item["entry"] for item in items}), 64)
        seeds[0]["functions"].append(self.seed("f000")["functions"][0])
        with self.assertRaisesRegex(ValueError, "Aggregate function"):
            prepare.combine_seed_functions(seeds, "a" * 64)

    def test_metadata_count_individual_and_total_bytes_are_bounded(self):
        prepare.validate_seed_sizes([4 * 1024 * 1024] * 16)
        prepare.validate_seed_sizes([8 * 1024 * 1024] * 8)
        with self.assertRaisesRegex(ValueError, "Aggregate seed metadata"):
            prepare.validate_seed_sizes([8 * 1024 * 1024] * 9)
        for sizes in [[], [1] * 17, [-1], [0.5], [True], [8 * 1024 * 1024 + 1]]:
            with self.assertRaises(ValueError): prepare.validate_seed_sizes(sizes)

class EnvelopeRangeTests(unittest.TestCase):
    def test_overlaps_order_size_and_foreign_entries_are_rejected(self):
        base = fixture()
        variants = []
        for key, value in [("established", False), ("external", True), ("thunk", True),
                           ("entry", "104"), ("bodyBytes", 12)]:
            item = copy.deepcopy(base)
            item[key] = value
            variants.append(item)
        for ranges in [[], list(reversed(base["bodyRanges"])),
                       [{"start": "100", "endInclusive": "108"}, {"start": "108", "endInclusive": "10b"}],
                       [{"start": "100", "endInclusive": "ff"}],
                       [{"start": "100", "endInclusive": "10100"}]]:
            item = copy.deepcopy(base)
            item["bodyRanges"] = ranges
            variants.append(item)
        for item in variants:
            with self.subTest(item=item), self.assertRaises(ValueError):
                prepare.function_envelope(item)


class MixedLayoutTests(unittest.TestCase):
    def fixture(self, anchor=0x108):
        # Owned x86: indexed jump, one padding byte, two local pointers, rets.
        raw = bytearray(b"\xff\x24\x85" + anchor.to_bytes(4, "little") + b"\x90" +
                        (0x110).to_bytes(4, "little") + (0x111).to_bytes(4, "little") + b"\xc3\xc3")
        rows = [{"address": "100", "length": 7, "analysisBytesHex": raw[:7].hex(), "mnemonic": "JMP",
                 "computed": True, "call": False, "flows": ["110", "111"]},
                *[{"address": f"{address:x}", "length": 1, "analysisBytesHex": "c3", "mnemonic": "RET",
                   "computed": False, "call": False, "flows": []} for address in [0x110, 0x111]]]
        layout = [{"start": "100", "kind": "instruction", "bytes": 7},
                  {"start": "107", "kind": "undefined", "bytes": 1, "analysisBytesHex": "90"},
                  *[{"start": f"{address:x}", "kind": "data", "bytes": 4, "dataType": "pointer",
                     "analysisBytesHex": raw[address - 0x100:address - 0x100 + 4].hex()} for address in [0x108, 0x10c]],
                  {"start": "110", "kind": "instruction", "bytes": 1}, {"start": "111", "kind": "instruction", "bytes": 1}]
        item = {"established": True, "entry": "100", "bodyBytes": 9, "envelopeBytes": 18,
                "bodyRanges": [{"start": "100", "endInclusive": "106"}, {"start": "110", "endInclusive": "111"}],
                "instructions": rows, "layout": layout}
        read = lambda address, size: bytes(raw[address - 0x100:address - 0x100 + size])
        site = {"address": 0x100, "base": None, "index": "eax", "scale": 4, "size": 4, "anchor": anchor}
        return item, raw, read, site

    def test_all_original_bytes_and_local_dispatch_are_accounted_for(self):
        item, raw, read, site = self.fixture()
        result = prepare.checked_function_layout(item, read)
        self.assertEqual((result["end"] - result["start"], result["dataBytes"], result["paddingBytes"]), (18, 8, 1))
        self.assertEqual(prepare.checked_dispatch_tables(result, [site]), {0x108: [0x110, 0x111]})
        for padding in [b"\x90", b"\x8b\xff", b"\x8d\x49\x00", b"\x90\x8b\xff\x8d\x49\x00"]:
            self.assertTrue(prepare._alignment_padding(padding))
        self.assertFalse(prepare._alignment_padding(b"\x31\xc0"))

    def test_biased_anchors_preserve_loaded_target_values(self):
        for anchor in [0x104, 0x108, 0x10c, 0x110]:
            item, raw, read, site = self.fixture(anchor)
            result = prepare.checked_function_layout(item, read)
            self.assertEqual(prepare.checked_dispatch_tables(result, [site])[anchor], [0x110, 0x111])

    def test_missing_overlapping_or_inconsistent_layout_never_trims_the_envelope(self):
        item, raw, read, site = self.fixture()
        variants = []
        for part in range(len(item["layout"])):
            bad = copy.deepcopy(item); del bad["layout"][part]; variants.append(bad)
        bad = copy.deepcopy(item); bad["layout"][1]["start"] = "106"; variants.append(bad)
        bad = copy.deepcopy(item); bad["instructions"][1]["length"] = 2; variants.append(bad)
        bad = copy.deepcopy(item); bad.pop("layout"); variants.append(bad)
        for bad in variants:
            with self.subTest(layout=bad.get("layout")), self.assertRaises(ValueError):
                prepare.checked_function_layout(bad, read)

    def test_byte_identity_unknown_data_and_noncode_edges_are_rejected(self):
        item, raw, read, site = self.fixture()
        for change in [lambda f: f["layout"][1].update(analysisBytesHex="cc"),
                       lambda f: f["layout"][2].update(dataType="integer"),
                       lambda f: f["instructions"][0].update(fallthrough="107"),
                       lambda f: f["instructions"][0].update(flows=["108"]),
                       lambda f: f["instructions"][0].update(analysisBytesHex="90" * 7)]:
            bad = copy.deepcopy(item); change(bad)
            with self.assertRaises(ValueError): prepare.checked_function_layout(bad, read)
        raw[7] = 0xcc; item["layout"][1]["analysisBytesHex"] = "cc"
        with self.assertRaises(ValueError): prepare.checked_function_layout(item, read)
        item, raw, read, site = self.fixture()
        raw[8:12] = (0x107).to_bytes(4, "little"); item["layout"][2]["analysisBytesHex"] = raw[8:12].hex()
        with self.assertRaises(ValueError): prepare.checked_function_layout(item, read)

    def test_incomplete_ambiguous_or_wrong_operand_dispatches_are_rejected(self):
        item, raw, read, site = self.fixture(); result = prepare.checked_function_layout(item, read)
        for sites in [[], [site, site], [dict(site, scale=8)], [dict(site, base="edx")],
                      [dict(site, index=None)], [dict(site, size=8)], [dict(site, anchor=0x100)], [dict(site, anchor=0x109)]]:
            with self.subTest(sites=sites), self.assertRaises(ValueError): prepare.checked_dispatch_tables(result, sites)
        ambiguous = copy.deepcopy(result); ambiguous["tables"].append(copy.deepcopy(ambiguous["tables"][0]))
        with self.assertRaises(ValueError): prepare.checked_dispatch_tables(ambiguous, [site])
        foreign = copy.deepcopy(result); foreign["rows"][0]["flows"] = ["110"]
        with self.assertRaises(ValueError): prepare.checked_dispatch_tables(foreign, [site])


class ReturnObservationTests(unittest.TestCase):
    def test_local_frame_is_observed_at_each_existing_return_trace_without_replacing_operations(self):
        marker = 'RECOMP_TRACE_EXIT("ff_title_00000100", 0x00000100)'
        body = 'uint32_t other = 0, ebp = 0;\nebp = saved; ' + marker + '; esp += 4; return;\n' + marker + ';'
        observed, sites = prepare.observe_return_frames(body, 0x100)
        self.assertEqual(sites, 2)
        self.assertEqual(observed.replace('FF_RETURN_FRAME(0x00000100u, ebp); ', ''), body)
        self.assertNotIn('g_ebp =', observed)
        self.assertEqual(prepare.observe_return_frames('uint32_t eax = 0;', 0x100), ('uint32_t eax = 0;', 0))
        with self.assertRaises(ValueError): prepare.observe_return_frames('uint32_t ebp = 0; return;', 0x100)


if __name__ == "__main__":
    unittest.main()
