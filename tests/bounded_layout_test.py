"""Pin the bounded mixed-layout data contract used by the recompilation guard.

These cases are synthetic. They do not read original content, execute anything,
or decide that a translated function is correct. They make the contract that
distinguishes the accepted 0x1FCCB0 pointer table from the blocked 0x1FCB70
byte-data gap explicit:

- instruction-only envelopes are accepted;
- contiguous 4-byte pointer entries become a declared local table;
- alignment padding is accepted;
- byte or other non-pointer data is rejected;
- pointer entries must target an established local instruction boundary.
"""
import importlib.util
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SPEC = importlib.util.spec_from_file_location(
    "prep_title", ROOT / "tools/prepare-title-recomp.py")
PREP = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PREP)

CODE = {"00100000": "8b442404"}
MOV = [{"address": "00100000", "length": 4, "mnemonic": "MOV",
        "analysisBytesHex": "8b442404", "call": False, "flows": []}]


def reader(memory):
    def read(address, size):
        raw = memory.get(f"{address:08x}")
        if raw is None or len(raw) != size * 2:
            raise ValueError(f"unmapped synthetic read at {address:#x}")
        return bytes.fromhex(raw)
    return read


def item(ranges, body_bytes, envelope_bytes, layout):
    return {
        "established": True,
        "external": False,
        "thunk": False,
        "entry": "00100000",
        "bodyRanges": ranges,
        "bodyBytes": body_bytes,
        "envelopeBytes": envelope_bytes,
        "instructions": MOV,
        "layout": layout,
    }


def instruction(start, size):
    return {"kind": "instruction", "start": start, "bytes": size}


def data(start, hex_bytes, data_type="pointer"):
    return {"kind": "data", "start": start, "bytes": len(hex_bytes) // 2,
            "dataType": data_type, "analysisBytesHex": hex_bytes}


def undefined(start, hex_bytes):
    return {"kind": "undefined", "start": start, "bytes": len(hex_bytes) // 2,
            "analysisBytesHex": hex_bytes}


class MixedLayoutContractTest(unittest.TestCase):
    def test_instruction_only_envelope(self):
        layout = PREP.checked_function_layout(item(
            [{"start": "00100000", "endInclusive": "00100003"}], 4, 4,
            [instruction("00100000", 4)]), reader(CODE))
        self.assertFalse(layout["mixed"])
        self.assertEqual(layout["tables"], [])

    def test_pointer_table_is_declared(self):
        memory = dict(CODE)
        memory["00100004"] = "00001000"
        memory["00100008"] = "00001000"
        layout = PREP.checked_function_layout(item(
            [{"start": "00100000", "endInclusive": "00100003"},
             {"start": "00100004", "endInclusive": "0010000b"}], 12, 12,
            [instruction("00100000", 4),
             data("00100004", "00001000"),
             data("00100008", "00001000")]), reader(memory))
        self.assertTrue(layout["mixed"])
        self.assertEqual(len(layout["tables"]), 1)
        self.assertEqual(layout["tables"][0]["targets"], [0x100000, 0x100000])

    def test_byte_data_is_rejected(self):
        memory = dict(CODE)
        for offset in range(4):
            memory[f"{0x100004 + offset:08x}"] = "00"
        with self.assertRaisesRegex(
                ValueError, "Only declared 32-bit local pointer-table data is supported"):
            PREP.checked_function_layout(item(
                [{"start": "00100000", "endInclusive": "00100003"},
                 {"start": "00100004", "endInclusive": "00100007"}], 8, 8,
                [instruction("00100000", 4),
                 data("00100004", "00", "byte"),
                 data("00100005", "00", "byte"),
                 data("00100006", "00", "byte"),
                 data("00100007", "00", "byte")]), reader(memory))

    def test_pointer_target_must_be_local_instruction(self):
        memory = dict(CODE)
        memory["00100004"] = "00200000"
        with self.assertRaisesRegex(
                ValueError, "Table data must target an established local instruction boundary"):
            PREP.checked_function_layout(item(
                [{"start": "00100000", "endInclusive": "00100003"},
                 {"start": "00100004", "endInclusive": "00100007"}], 8, 8,
                [instruction("00100000", 4),
                 data("00100004", "00200000")]), reader(memory))

    def test_alignment_padding_is_accepted(self):
        memory = dict(CODE)
        memory["00100004"] = "90"
        layout = PREP.checked_function_layout(item(
            [{"start": "00100000", "endInclusive": "00100003"},
             {"start": "00100004", "endInclusive": "00100004"}], 5, 5,
            [instruction("00100000", 4), undefined("00100004", "90")]),
            reader(memory))
        self.assertTrue(layout["mixed"])
        self.assertEqual(layout["paddingBytes"], 1)

    def test_non_padding_undefined_is_rejected(self):
        memory = dict(CODE)
        memory["00100004"] = "00"
        with self.assertRaisesRegex(
                ValueError, "Undefined bytes are not established alignment padding"):
            PREP.checked_function_layout(item(
                [{"start": "00100000", "endInclusive": "00100003"},
                 {"start": "00100004", "endInclusive": "00100004"}], 5, 5,
                [instruction("00100000", 4), undefined("00100004", "00")]),
                reader(memory))


if __name__ == "__main__":
    unittest.main(verbosity=2)
