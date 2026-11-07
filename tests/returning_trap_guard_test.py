"""Synthetic fail-closed tests for returning INT 2D / stepped INT3 layout."""
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "prep_title", ROOT / "translator/prepare-title-recomp.py")
PREP = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PREP)


def row(address, mnemonic, raw):
    return {"address": f"{address:08x}", "mnemonic": mnemonic,
            "length": len(raw), "analysisBytesHex": raw.hex()}


class ReturningTrapGuardTests(unittest.TestCase):
    def test_terminal_returning_int2d_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "no contiguous"):
            PREP.checked_returning_traps([row(0x100, "INT", b"\xcd\x2d")], 0x100, 0x102)

    def test_terminal_stepped_int3_is_rejected(self):
        rows = [row(0x100, "INT", b"\xcd\x2d"), row(0x102, "INT3", b"\xcc")]
        with self.assertRaisesRegex(ValueError, "no contiguous"):
            PREP.checked_returning_traps(rows, 0x100, 0x103)

    def test_gapped_successor_is_rejected(self):
        rows = [row(0x100, "INT", b"\xcd\x2d"), row(0x103, "LEAVE", b"\xc9")]
        with self.assertRaisesRegex(ValueError, "no contiguous"):
            PREP.checked_returning_traps(rows, 0x100, 0x104)

    def test_wrong_declared_fallthrough_is_rejected(self):
        trap = row(0x100, "INT", b"\xcd\x2d")
        trap["fallthrough"] = "00000103"
        rows = [trap, row(0x102, "INT3", b"\xcc"), row(0x103, "LEAVE", b"\xc9")]
        with self.assertRaisesRegex(ValueError, "inconsistent fallthrough"):
            PREP.checked_returning_traps(rows, 0x100, 0x104)

    def test_contiguous_int2d_int3_leave_ret_is_admitted(self):
        rows = [row(0x100, "INT", b"\xcd\x2d"), row(0x102, "INT3", b"\xcc"),
                row(0x103, "LEAVE", b"\xc9"), row(0x104, "RET", b"\xc3")]
        checked = PREP.checked_returning_traps(rows, 0x100, 0x105)
        self.assertEqual(checked[:-1], [
            {"site": 0x100, "next": 0x102, "kind": "returning-int-2d", "nextMnemonic": "INT3"},
            {"site": 0x102, "next": 0x103, "kind": "stepped-over-int3", "nextMnemonic": "LEAVE"},
        ])
        self.assertEqual(checked[-1]["terminals"], [{"address": 0x104, "kind": "return"}])

    def test_reachable_terminal_leave_or_move_is_rejected(self):
        for tail in (row(0x102, "LEAVE", b"\xc9"), row(0x102, "MOV", b"\x90")):
            with self.subTest(mnemonic=tail["mnemonic"]), self.assertRaisesRegex(ValueError, "falls off"):
                PREP.checked_returning_traps([row(0x100, "INT", b"\xcd\x2d"), tail], 0x100, 0x103)

    def test_referenced_adjacent_int3_is_not_misclassified_as_stepped(self):
        rows = [row(0x100, "INT", b"\xcd\x2d"), row(0x102, "INT3", b"\xcc"),
                row(0x103, "RET", b"\xc3"), row(0x104, "JMP", b"\xeb\xfc")]
        rows[3]["flows"] = ["00000102"]
        with self.assertRaisesRegex(ValueError, "not a verified stepped-over slide"):
            PREP.checked_returning_traps(rows, 0x100, 0x106)

    def test_checked_external_tail_jump_is_a_guarded_terminal(self):
        rows = [row(0x100, "INT", b"\xcd\x2d"), row(0x102, "JMP", b"\xe9\x00\x00\x00\x00")]
        rows[1]["flows"] = ["00000200"]
        result = PREP.checked_returning_traps(rows, 0x100, 0x107, guarded_targets={0x200})
        self.assertEqual(result[-1]["terminals"], [{"address": 0x200, "kind": "guarded-tail-transfer"}])

    def test_unverified_computed_jump_is_rejected(self):
        rows = [row(0x100, "INT", b"\xcd\x2d"), row(0x102, "JMP", b"\xff\xe0"),
                row(0x104, "RET", b"\xc3")]
        rows[1]["computed"] = True
        with self.assertRaisesRegex(ValueError, "computed JMP"):
            PREP.checked_returning_traps(rows, 0x100, 0x105)

    def test_conditional_branch_without_target_is_rejected(self):
        rows = [row(0x100, "INT", b"\xcd\x2d"), row(0x102, "JZ", b"\x74\x01"),
                row(0x104, "RET", b"\xc3")]
        rows[1]["fallthrough"] = "00000104"
        with self.assertRaisesRegex(ValueError, "conditional branch"):
            PREP.checked_returning_traps(rows, 0x100, 0x105)

    def test_conditional_paths_and_call_fallthrough_are_closed(self):
        conditional = [row(0x100, "INT", b"\xcd\x2d"), row(0x102, "JZ", b"\x74\x02"),
                       row(0x104, "CALL", b"\xe8\x00\x00\x00\x00"), row(0x109, "RET", b"\xc3"),
                       row(0x10a, "RET", b"\xc3")]
        conditional[1]["flows"] = ["0000010a"]
        conditional[1]["fallthrough"] = "00000104"
        conditional[2]["call"] = True
        conditional[2]["flows"] = ["00000300"]
        conditional[2]["fallthrough"] = "00000109"
        conditional[3]["fallthrough"] = None
        conditional[4]["fallthrough"] = None
        result = PREP.checked_returning_traps(conditional, 0x100, 0x10b, guarded_targets={0x300})
        self.assertEqual({row["kind"] for row in result[-1]["terminals"]}, {"return"})

    def test_unsupported_int_service_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "Unsupported INT service"):
            PREP.checked_returning_traps([row(0x100, "INT", b"\xcd\x30"),
                                          row(0x102, "RET", b"\xc3")], 0x100, 0x103)


if __name__ == "__main__":
    unittest.main()
