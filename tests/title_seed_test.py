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
        for seeds in [[], [self.seed()] * 9, [oversized], [wrong]]:
            with self.assertRaises(ValueError):
                prepare.combine_seed_functions(seeds, "a" * 64)

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


if __name__ == "__main__":
    unittest.main()
