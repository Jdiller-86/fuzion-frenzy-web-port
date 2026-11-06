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
    "prep_title", ROOT / "translator/prepare-title-recomp.py")
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


def item(ranges, body_bytes, envelope_bytes, layout, rows=None):
    return {
        "established": True,
        "external": False,
        "thunk": False,
        "entry": "00100000",
        "bodyRanges": ranges,
        "bodyBytes": body_bytes,
        "envelopeBytes": envelope_bytes,
        "instructions": MOV if rows is None else rows,
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

    def test_contiguous_defined_bytes_are_accepted(self):
        memory = dict(CODE)
        for offset in range(4):
            memory[f"{0x100004 + offset:08x}"] = "00"
        layout = PREP.checked_function_layout(item(
            [{"start": "00100000", "endInclusive": "00100003"},
             {"start": "00100004", "endInclusive": "00100007"}], 8, 8,
            [instruction("00100000", 4),
             data("00100004", "00", "byte"),
             data("00100005", "00", "byte"),
             data("00100006", "00", "byte"),
             data("00100007", "00", "byte")]), reader(memory))
        self.assertTrue(layout["mixed"])
        self.assertEqual(layout["dataBytes"], 4)
        self.assertEqual(layout["tables"], [])

    def test_two_pointer_tables_separated_by_defined_byte(self):
        memory = dict(CODE)
        memory["00100004"] = "00001000"
        memory["00100008"] = "00001000"
        memory["0010000c"] = "ab"
        memory["0010000d"] = "00001000"
        layout = PREP.checked_function_layout(item(
            [{"start": "00100000", "endInclusive": "00100010"}], 17, 17,
            [instruction("00100000", 4),
             data("00100004", "00001000"),
             data("00100008", "00001000"),
             data("0010000c", "ab", "byte"),
             data("0010000d", "00001000")]), reader(memory))
        self.assertTrue(layout["mixed"])
        self.assertEqual(layout["dataBytes"], 13)
        self.assertEqual([table["targets"] for table in layout["tables"]],
                         [[0x100000, 0x100000], [0x100000]])

    def test_twenty_eight_byte_gap_is_accepted(self):
        memory = dict(CODE)
        memory["00100004"] = "00001000"
        memory["00100008"] = "00001000"
        parts = [instruction("00100000", 4),
                 data("00100004", "00001000"),
                 data("00100008", "00001000")]
        for index in range(28):
            address = 0x10000c + index
            value = f"{index:02x}"
            memory[f"{address:08x}"] = value
            parts.append(data(f"{address:08x}", value, "byte"))
        memory["00100028"] = "00001000"
        parts.append(data("00100028", "00001000"))
        layout = PREP.checked_function_layout(item(
            [{"start": "00100000", "endInclusive": "0010002b"}], 44, 44,
            parts), reader(memory))
        self.assertTrue(layout["mixed"])
        self.assertEqual(layout["dataBytes"], 40)
        self.assertEqual([len(table["targets"]) for table in layout["tables"]],
                         [2, 1])

    def test_defined_byte_budget_is_bounded(self):
        memory = dict(CODE)
        parts = [instruction("00100000", 4)]
        for index in range(257):
            address = 0x100004 + index
            value = f"{index % 256:02x}"
            memory[f"{address:08x}"] = value
            parts.append(data(f"{address:08x}", value, "byte"))
        with self.assertRaisesRegex(ValueError, "Defined byte-data budget exceeded"):
            PREP.checked_function_layout(item(
                [{"start": "00100000", "endInclusive": "00100104"}], 261, 261,
                parts), reader(memory))

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

    def test_defined_byte_data_is_accepted_without_becoming_a_table(self):
        memory = dict(CODE)
        memory["00100004"] = "ab"
        layout = PREP.checked_function_layout(item(
            [{"start": "00100000", "endInclusive": "00100004"}], 5, 5,
            [instruction("00100000", 4), data("00100004", "ab", "byte")]),
            reader(memory))
        self.assertTrue(layout["mixed"])
        self.assertEqual(layout["dataBytes"], 1)
        self.assertEqual(layout["tables"], [])

    def test_flow_into_defined_byte_data_is_rejected(self):
        memory = dict(CODE)
        memory["00100004"] = "ab"
        with self.assertRaisesRegex(
                ValueError,
                "Mixed-layout control flow must remain in checked local code"):
            PREP.checked_function_layout(item(
                [{"start": "00100000", "endInclusive": "00100004"}], 5, 5,
                [instruction("00100000", 4), data("00100004", "ab", "byte")],
                rows=[mov_row(flows=["00100004"])]),
                reader(memory))

    def test_byte_data_must_be_one_byte(self):
        memory = dict(CODE)
        memory["00100004"] = "abcd"
        with self.assertRaisesRegex(
                ValueError,
                "Only declared 32-bit local pointer-table data or single defined bytes are supported"):
            PREP.checked_function_layout(item(
                [{"start": "00100000", "endInclusive": "00100005"}], 6, 6,
                [instruction("00100000", 4), data("00100004", "abcd", "byte")]),
                reader(memory))

    def test_unknown_data_type_is_rejected(self):
        memory = dict(CODE)
        memory["00100004"] = "00000000"
        with self.assertRaisesRegex(
                ValueError,
                "Only declared 32-bit local pointer-table data or single defined bytes are supported"):
            PREP.checked_function_layout(item(
                [{"start": "00100000", "endInclusive": "00100007"}], 8, 8,
                [instruction("00100000", 4), data("00100004", "00000000", "dword")]),
                reader(memory))

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


def mov_row(flows=None, fallthrough=None):
    row = {"address": "00100000", "length": 4, "mnemonic": "MOV",
           "analysisBytesHex": "8b442404", "call": False, "flows": flows or []}
    if fallthrough is not None:
        row["fallthrough"] = fallthrough
    return row


def pointer_memory():
    memory = dict(CODE)
    memory["00100004"] = "00001000"
    return memory


POINTER_LAYOUT = [instruction("00100000", 4), data("00100004", "00001000")]


class FlowAndEnvelopeContractTest(unittest.TestCase):
    def test_fallthrough_to_data_is_rejected(self):
        with self.assertRaisesRegex(
                ValueError, "Fallthrough reaches data or excluded padding"):
            PREP.checked_function_layout(item(
                [{"start": "00100000", "endInclusive": "00100007"}], 8, 8,
                POINTER_LAYOUT, rows=[mov_row(fallthrough="00100004")]),
                reader(pointer_memory()))

    def test_flow_to_data_is_rejected(self):
        with self.assertRaisesRegex(
                ValueError,
                "Mixed-layout control flow must remain in checked local code"):
            PREP.checked_function_layout(item(
                [{"start": "00100000", "endInclusive": "00100007"}], 8, 8,
                POINTER_LAYOUT, rows=[mov_row(flows=["00100004"])]),
                reader(pointer_memory()))

    def test_split_envelope_requires_exported_full_envelope(self):
        with self.assertRaisesRegex(
                ValueError,
                "Split functions require an explicitly exported full envelope"):
            PREP.checked_function_layout(item(
                [{"start": "00100000", "endInclusive": "00100003"},
                 {"start": "00100008", "endInclusive": "0010000b"}], 8, 8,
                [instruction("00100000", 4), instruction("00100008", 4)]),
                reader(CODE))

    def test_unestablished_function_is_rejected(self):
        record = item(
            [{"start": "00100000", "endInclusive": "00100003"}], 4, 4,
            [instruction("00100000", 4)])
        record["established"] = False
        with self.assertRaisesRegex(
                ValueError, "Only established local functions are admitted"):
            PREP.checked_function_layout(record, reader(CODE))


def dispatch_layout(flows, tables):
    row = {"address": "00100004", "computed": True, "call": False,
           "mnemonic": "JMP", "flows": [f"{value:08x}" for value in flows]}
    return {"start": 0x100000, "end": 0x10000c, "rows": [row],
            "tables": tables, "mixed": True}


def table(start, targets):
    return {"start": start, "end": start + 4 * len(targets), "targets": targets}


def site(anchor=0x100004):
    return {"address": 0x100004, "base": None, "index": "eax",
            "scale": 4, "size": 4, "anchor": anchor}


class DispatchTableContractTest(unittest.TestCase):
    def test_two_tables_and_sites_are_censused(self):
        rows = [
            {"address": "00100004", "computed": True, "call": False,
             "mnemonic": "JMP", "flows": ["00100000"]},
            {"address": "00100008", "computed": True, "call": False,
             "mnemonic": "JMP", "flows": ["00100000"]},
        ]
        layout = {"start": 0x100000, "end": 0x100011, "rows": rows,
                  "tables": [table(0x100004, [0x100000, 0x100000]),
                             table(0x10000d, [0x100000])], "mixed": True}
        second = site(anchor=0x10000d)
        second["address"] = 0x100008
        census = PREP.checked_dispatch_tables(layout, [site(anchor=0x100004), second])
        self.assertEqual(census, {0x100004: [0x100000, 0x100000],
                                  0x10000d: [0x100000]})

    def test_matching_table_and_flows_are_accepted(self):
        layout = dispatch_layout([0x100000], [table(0x100004, [0x100000, 0x100000])])
        census = PREP.checked_dispatch_tables(layout, [site()])
        self.assertEqual(census, {0x100004: [0x100000, 0x100000]})

    def test_wrong_flows_are_rejected(self):
        layout = dispatch_layout([0x200000], [table(0x100004, [0x100000])])
        with self.assertRaisesRegex(
                ValueError, "Computed jump lacks one complete declared local table"):
            PREP.checked_dispatch_tables(layout, [site()])

    def test_missing_site_is_rejected(self):
        layout = dispatch_layout([0x100000], [table(0x100004, [0x100000])])
        with self.assertRaisesRegex(
                ValueError, "Computed-jump census does not cover every established dispatch"):
            PREP.checked_dispatch_tables(layout, [])

    def test_unused_table_is_rejected(self):
        layout = dispatch_layout([0x100000], [
            table(0x100004, [0x100000]),
            table(0x10000c, [0x100000]),
        ])
        with self.assertRaisesRegex(
                ValueError, "Declared data is not accounted for by validated dispatches"):
            PREP.checked_dispatch_tables(layout, [site()])

    def test_unsupported_operand_is_rejected(self):
        layout = dispatch_layout([0x100000], [table(0x100004, [0x100000])])
        bad = site()
        bad["base"] = "eax"
        with self.assertRaisesRegex(ValueError, "Unsupported computed-jump operand"):
            PREP.checked_dispatch_tables(layout, [bad])


if __name__ == "__main__":
    unittest.main(verbosity=2)
