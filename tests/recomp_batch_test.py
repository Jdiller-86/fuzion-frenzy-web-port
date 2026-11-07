#!/usr/bin/env python3
"""Focused fixture tests for reusable bounded batch preparation helpers."""
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

SOURCE = Path(__file__).resolve().parents[1] / "translator" / "extend-recomp-batch.py"
SPEC = importlib.util.spec_from_file_location("extend_recomp_batch", SOURCE)
batch = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(batch)


def seed(entries, source="a" * 64):
    return {"kind": batch.SEED_KIND, "sourceSha256": source,
            "functions": [{"entry": f"{entry:08x}", "bodyRanges": [], "instructions": []} for entry in entries]}


class BatchPackingTests(unittest.TestCase):
    def setUp(self):
        self.generation = {"sourceSha256": "a" * 64, "functionsPerExport": 32}
        self.groups = [(Path("seed-a.json"), seed([0x1000, 0x2000]), b"a"),
                       (Path("seed-b.json"), seed([0x3000]), b"b")]

    def test_addition_packs_into_last_group_preserving_order_and_rows(self):
        selected, merged = batch.pack_addition(self.groups, seed([0x4000]), self.generation)
        self.assertEqual(selected, 1)
        self.assertEqual([row["entry"] for row in merged["functions"]],
                         ["00003000", "00004000"])
        self.assertEqual([row["entry"] for row in self.groups[0][1]["functions"]],
                         ["00001000", "00002000"])

    def test_duplicate_entry_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "duplicates"):
            batch.pack_addition(self.groups, seed([0x3000]), self.generation)

    def test_full_last_group_is_rejected_without_expanding_export(self):
        groups = [(Path("full.json"), seed(range(32)), b"c"),
                  (Path("room.json"), seed([0x4001]), b"d")]
        selected, merged = batch.pack_addition(groups, seed([0x4000]), self.generation)
        self.assertEqual(selected, 1)
        self.assertEqual(len(merged["functions"]), 2)

    def test_all_full_groups_rejected(self):
        groups = [(Path("a.json"), seed(range(32)), b"a"),
                  (Path("b.json"), seed(range(32, 64)), b"b")]
        with self.assertRaisesRegex(ValueError, "No existing seed group"):
            batch.pack_addition(groups, seed([0x4000]), self.generation)

    def test_source_mismatch_rejected(self):
        with self.assertRaisesRegex(ValueError, "source identity"):
            batch.pack_addition(self.groups, seed([0x4000], "b" * 64), self.generation)


class BatchIdentityTests(unittest.TestCase):
    def test_reached_target_must_match_addition(self):
        report = {"sourceSha256": "a" * 64, "moduleSha256": "b" * 64,
                  "report": {"stop": {"status": 2, "target": 0x3055A}}}
        with self.assertRaisesRegex(ValueError, "exact status-2 stop target"):
            batch.validate_reached(report, "a" * 64, 100, seed([0x4000]))
        batch.validate_reached(report, "a" * 64, 100, seed([0x3055A]))

    def test_source_mismatch_rejected(self):
        report = {"sourceSha256": "b" * 64, "moduleSha256": "c" * 64,
                  "report": {"stop": {"status": 2, "target": 0x3055A}}}
        with self.assertRaisesRegex(ValueError, "source identity"):
            batch.validate_reached(report, "a" * 64, 100, seed([0x3055A]))

    def test_binding_delta_requires_exact_allowlist_and_xbe_pair(self):
        old = {"kernelBindings": [{"slot": "00001000", "ordinal": 10}]}
        new = {"kernelBindings": [{"slot": "00001000", "ordinal": 10},
                                  {"slot": "00002000", "ordinal": 11}]}
        profile = {"profile": {"imports": [{"address": 0x1000, "ordinal": 10},
                                             {"address": 0x2000, "ordinal": 11}],
                               "sections": [{"address": 0x2000, "fileBytes": 8, "fileOffset": 0}]}}
        xbe = (0x8000000B).to_bytes(4, "little") + b"\0" * 4
        self.assertEqual(batch.validate_binding_delta(old, new, {(0x2000, 11)}, profile, xbe),
                         {(0x2000, 11)})
        with self.assertRaisesRegex(ValueError, "allow-list"):
            batch.validate_binding_delta(old, new, set(), profile, xbe)
        with self.assertRaisesRegex(ValueError, "not match"):
            batch.validate_binding_delta(old, new, {(0x2000, 11)},
                                         {"profile": {"imports": [{"address": 0x1000, "ordinal": 10}],
                                                       "sections": [{"address": 0x2000, "fileBytes": 8, "fileOffset": 0}]}}, xbe)

    def test_binding_removal_rejected(self):
        old = {"kernelBindings": [{"slot": "00001000", "ordinal": 10}]}
        with self.assertRaisesRegex(ValueError, "removed"):
            batch.validate_binding_delta(old, {"kernelBindings": []}, set(), {"profile": {"imports": []}}, b"")

    def test_allowlist_format_and_duplicates_rejected(self):
        self.assertEqual(batch.parse_allowlist(["0x2000:11"]), {(0x2000, 11)})
        with self.assertRaisesRegex(ValueError, "exact slot:ordinal"):
            batch.parse_allowlist(["0x2000"])
        with self.assertRaisesRegex(ValueError, "Duplicate"):
            batch.parse_allowlist(["0x2000:11", "00002000:11"])

    def test_prior_generated_function_bodies_must_remain_byte_identical(self):
        old = "void ff_title_00001000(void) {\n int x = 1;\n}\n"
        new = "void ff_title_00001000(void) {\n int x = 2;\n}\n"
        with self.assertRaisesRegex(ValueError, "changed or disappeared"):
            batch.check_prior_bodies({"untranslatedDirectTargets": []}, {"functions": []},
                                     batch.function_definitions(old), batch.function_definitions(new))

    def test_only_old_direct_stop_stub_may_be_replaced(self):
        old = "void ff_title_00001000(void) { stop(); }\nvoid ff_title_00002000(void) { keep(); }\n"
        new = "void ff_title_00001000(void) { translated(); }\nvoid ff_title_00002000(void) { keep(); }\n"
        retained, replaced = batch.check_prior_bodies(
            {"untranslatedDirectTargets": [0x1000]}, {"functions": [{"entry": 0x1000}]},
            batch.function_definitions(old), batch.function_definitions(new), {0x1000})
        self.assertEqual(retained, {"ff_title_00002000"})
        self.assertEqual(replaced, {"ff_title_00001000"})

    def test_actual_generation_shapes_multiline_bodies_views_and_numeric_import(self):
        source = "\n".join(f"void ff_title_{address:08X}(void)\n{{\n return;\n}}"
                           for address in range(0x1000, 0x1005))
        bodies = batch.function_definitions(source)
        generation = {"functions": [{"entry": 0x1000}],
                      "checkedDirectEntryViews": [{"entry": i} for i in range(3)],
                      "untranslatedDirectTargets": [0x1004]}
        census = len(generation["functions"]) + len(generation["checkedDirectEntryViews"]) + len(generation["untranslatedDirectTargets"])
        self.assertEqual(len(bodies), census)
        old = {"kernelBindings": []}
        new = {"kernelBindings": [{"slot": 0x2000, "ordinal": 11}]}
        profile = {"profile": {"imports": [{"address": 0x2000, "ordinal": 11}],
                               "sections": [{"address": 0x2000, "fileBytes": 4, "fileOffset": 0}]}}
        self.assertEqual(batch.validate_binding_delta(old, new, {(0x2000, 11)}, profile,
                                                       (0x8000000B).to_bytes(4, "little")),
                         {(0x2000, 11)})

    def test_body_parser_ignores_comment_apostrophes_and_braces(self):
        source = """void ff_title_00001000(void)
{
  // don't count this brace: }
  /* neither { nor this apostrophe ' closes a character */
  return;
}
"""
        body = batch.function_definitions(source)["ff_title_00001000"]
        self.assertTrue(body.endswith("}"))
        self.assertIn("don't count", body)

    def test_generated_output_hashes_are_required_and_checked(self):
        with tempfile.TemporaryDirectory(dir=batch.ROOT) as temporary:
            folder = Path(temporary)
            functions = b"generated functions fixture"
            profile = b"generated profile fixture"
            (folder / "title-functions.inc").write_bytes(functions)
            (folder / "title-profile.h").write_bytes(profile)
            generation = {"outputSha256": {"title-functions.inc": batch.sha(functions),
                                           "title-profile.h": batch.sha(profile)}}
            batch.verify_generation_outputs(generation, folder)
            generation["outputSha256"]["title-functions.inc"] = "0" * 64
            with self.assertRaisesRegex(ValueError, "identity differs"):
                batch.verify_generation_outputs(generation, folder)

    def test_seed_index_checks_exact_hash_order_and_duplicate_paths(self):
        research = batch.ROOT / "research"
        with tempfile.TemporaryDirectory(dir=research) as temporary:
            folder = Path(temporary) / "retained-local-evidence" / "source" / "research" / "verification"
            folder.mkdir(parents=True)
            first = json.dumps(seed([0x1000]), separators=(",", ":")).encode()
            second = json.dumps(seed([0x2000]), separators=(",", ":")).encode()
            (folder / "a.json").write_bytes(first)
            (folder / "b.json").write_bytes(second)
            metadata = folder / "profile.json"
            metadata.write_text('{"kind":"static-execution-dependencies"}', encoding="utf-8")
            # The retained evidence path is read in place; it is not copied into verification.
            self.assertEqual(batch.research_path(metadata), metadata.resolve())
            self.assertEqual(batch.read_json(metadata)[0]["kind"], "static-execution-dependencies")
            generation = {"seedSha256s": [batch.sha(first), batch.sha(second)], "exportLimit": 2,
                          "sourceSha256": "a" * 64}
            generation_bytes = b"generation-fixture"
            index_path = folder / "index.json"
            index = {"kind": batch.INDEX_KIND, "baseGenerationSha256": batch.sha(generation_bytes),
                     "entries": [{"path": str(folder / "a.json"), "sha256": batch.sha(first)},
                                 {"path": str(folder / "b.json"), "sha256": batch.sha(second)}]}
            loaded = batch.load_seed_index(index, index_path, generation, generation_bytes)
            self.assertEqual([item[1]["functions"][0]["entry"] for item in loaded], ["00001000", "00002000"])
            index["entries"][1]["path"] = str(folder / "a.json")
            with self.assertRaisesRegex(ValueError, "Duplicate seed path"):
                batch.load_seed_index(index, index_path, generation, generation_bytes)
            index["entries"][1]["path"] = str(folder / "b.json")
            index["entries"][1]["sha256"] = "0" * 64
            with self.assertRaisesRegex(ValueError, "identity/order"):
                batch.load_seed_index(index, index_path, generation, generation_bytes)

    def test_private_path_rejects_external_path(self):
        with tempfile.TemporaryDirectory(dir=batch.ROOT) as temporary:
            root = Path(temporary)
            verification = root / "research" / "verification"
            verification.mkdir(parents=True)
            external = root / "external.json"
            external.write_text("{}", encoding="utf-8")
            with patch.object(batch, "VERIFY", verification):
                with self.assertRaisesRegex(ValueError, "inside research/verification"):
                    batch.private_path(external)

    def test_json_size_is_rejected_before_read(self):
        with tempfile.TemporaryDirectory(dir=batch.ROOT) as temporary:
            path = Path(temporary) / "bounded.json"
            path.write_bytes(b"{} ")
            with self.assertRaisesRegex(ValueError, "exceeds 1 bytes"):
                batch.read_json(path, limit=1)

    def test_generation_flags_are_preserved_and_unknown_views_fail(self):
        generation = {"functionLimit": 512, "instructionLimit": 32768,
                      "checkedMemoryAccesses": [{"function": 1}],
                      "timestampCounterGuards": [{"site": 2}],
                      "checkedDirectEntryViews": [{"site": 0x001E4DC2}]}
        args = batch.generation_args(generation, Path("profile.json"), Path("xbe"), [Path("seed")], "run")
        self.assertIn("--guard-memory", args)
        self.assertIn("--guard-timestamp-counter", args)
        self.assertIn("--crt-cleanup-entry", args)
        self.assertNotIn("--crt-seek-cleanup-entry", args)
        generation["checkedDirectEntryViews"].append({"site": 0x1234})
        with self.assertRaisesRegex(ValueError, "unknown/duplicate CRT"):
            batch.generation_args(generation, Path("profile.json"), Path("xbe"), [Path("seed")], "run")


if __name__ == "__main__":
    unittest.main()
