"""Reproduce the synthetic mixed-dispatch fixture from the 0x1FCCB0 evidence.

The fixture is source-owned and contains no original content: a prologue
dispatcher, an 8-entry absolute jump table at 0x11030, four ``ret 8`` case
blocks and alignment padding. It is never executed here. The test pins:

- the 96-byte fixture digest recorded in the admission checkpoint;
- the LF-normalized generated-C artifact digest and its four case labels;
- that the bounded layout and dispatch guards accept exactly this shape.

The committed generated C is the translator's LF-normalized text. The retained
Windows copy has the same content with CRLF line endings, which hashes
differently; this test reads the committed bytes, so the recorded digest is
reproducible without the private toolchain.
"""
import hashlib
import importlib.util
import struct
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SPEC = importlib.util.spec_from_file_location(
    "prep_title", ROOT / "tools/prepare-title-recomp.py")
PREP = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PREP)

BASE = 0x11000
TARGETS = [0x11010, 0x11018, 0x11020, 0x11028]
FIXTURE_SHA256 = "402462c08d414a58196bb7529c9cb36a2b0f3350e892c4a39df10e372383b12b"
GENERATED_SHA256 = "e11bc5905f4115371d7c0752f02d607c2f72091fffaec4f918f69aaaac61ad38"
GENERATED_PATH = ROOT / "tests/fixtures/mixed-dispatch.generated.c"


def build_blob():
    blob = bytearray()
    blob += bytes.fromhex("8b44240483f8077747ff248530100100")
    for value in (1, 2, 3, 4):
        blob += bytes.fromhex("b8") + struct.pack("<I", value) + bytes.fromhex("c20800")
    for target in TARGETS * 2:
        blob += struct.pack("<I", target)
    blob += bytes.fromhex("33c0c20800")
    blob += b"\x90" * (0x60 - len(blob))
    return bytes(blob)


BLOB = build_blob()


def reader(address, size):
    start = address - BASE
    if start < 0 or start + size > len(BLOB):
        raise ValueError("synthetic read outside the fixture")
    return BLOB[start:start + size]


def instruction(start, length):
    return {"kind": "instruction", "start": f"{start:08x}", "bytes": length}


def data(start, target):
    return {"kind": "data", "start": f"{start:08x}", "bytes": 4,
            "dataType": "pointer", "analysisBytesHex": struct.pack("<I", target).hex()}


def undefined(start, size, fill=b"\x90"):
    return {"kind": "undefined", "start": f"{start:08x}", "bytes": size,
            "analysisBytesHex": (fill * size).hex()}


def row(address, mnemonic, length, flows=None, fallthrough=None, computed=False):
    return {"address": f"{address:08x}", "length": length, "mnemonic": mnemonic,
            "analysisBytesHex": reader(address, length).hex(), "call": False,
            "computed": computed, "flows": flows or [],
            **({"fallthrough": f"{fallthrough:08x}"} if fallthrough else {})}


def code_rows():
    rows = [
        row(0x11000, "MOV", 4, fallthrough=0x11004),
        row(0x11004, "CMP", 3, fallthrough=0x11007),
        row(0x11007, "JA", 2, flows=[f"{0x11050:08x}"], fallthrough=0x11009),
        row(0x11009, "JMP", 7, flows=[f"{t:08x}" for t in TARGETS], computed=True),
    ]
    for target in TARGETS:
        rows.append(row(target, "MOV", 5, fallthrough=target + 5))
        rows.append(row(target + 5, "RET", 3))
    rows.append(row(0x11050, "XOR", 2, fallthrough=0x11052))
    rows.append(row(0x11052, "RET", 3))
    return rows


def fixture():
    rows = code_rows()
    layout = [instruction(int(r["address"], 16), r["length"])
              for r in rows if int(r["address"], 16) < 0x11030]
    layout += [data(0x11030 + 4 * index, target) for index, target in enumerate(TARGETS * 2)]
    layout += [instruction(int(r["address"], 16), r["length"])
               for r in rows if int(r["address"], 16) >= 0x11030]
    layout += [undefined(0x11055, 0x60 - 0x55)]
    return {
        "established": True, "external": False, "thunk": False,
        "entry": f"{BASE:08x}",
        "bodyRanges": [{"start": f"{0x11000:08x}", "endInclusive": f"{0x1102f:08x}"},
                       {"start": f"{0x11030:08x}", "endInclusive": f"{0x1105f:08x}"}],
        "bodyBytes": 0x60, "envelopeBytes": 0x60,
        "instructions": rows, "layout": layout,
    }


class MixedDispatchFixtureTest(unittest.TestCase):
    def test_fixture_and_generated_artifact_digests(self):
        self.assertEqual(hashlib.sha256(BLOB).hexdigest(), FIXTURE_SHA256)
        generated = GENERATED_PATH.read_bytes()
        self.assertEqual(hashlib.sha256(generated).hexdigest(), GENERATED_SHA256)
        text = generated.decode("utf-8")
        self.assertNotIn("RECOMP_UNIMPL", text)
        self.assertIn("switch: 8 entries, 4 targets", text)
        self.assertEqual(text.count("esp += 12; return;"), 5)
        for target in TARGETS:
            self.assertIn(f"loc_{target:08X}:", text)

    def test_layout_and_dispatch_census_accept_the_fixture(self):
        layout = PREP.checked_function_layout(fixture(), reader)
        self.assertTrue(layout["mixed"])
        self.assertEqual(layout["dataBytes"], 32)
        self.assertEqual(layout["paddingBytes"], 11)
        self.assertEqual(len(layout["tables"]), 1)
        self.assertEqual(layout["tables"][0]["targets"], TARGETS * 2)
        site = {"address": 0x11009, "base": None, "index": "eax",
                "scale": 4, "size": 4, "anchor": 0x11030}
        census = PREP.checked_dispatch_tables(layout, [site])
        self.assertEqual(census, {0x11030: TARGETS * 2})


if __name__ == "__main__":
    unittest.main(verbosity=2)
