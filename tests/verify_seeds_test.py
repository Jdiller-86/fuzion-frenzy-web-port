"""Owned checks for the seed-manifest verifier. Uses temporary fixtures only."""
import hashlib
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
spec = importlib.util.spec_from_file_location("verify_seeds", ROOT / "translator/verify-seeds.py")
verifier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(verifier)


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


class VerifySeedsTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.root = Path(self.directory.name)
        self.translation = self.root / "translation"
        self.translation.mkdir()
        self.first = self.root / "a.json"
        self.second = self.root / "b.json"
        self.first.write_text('{"a":1}', encoding="utf-8", newline="\n")
        self.second.write_text('{"b":2}', encoding="utf-8", newline="\n")
        self.record = {"seedSha256s": [digest(self.first), digest(self.second)]}
        (self.translation / "generation.json").write_text(json.dumps(self.record), encoding="utf-8", newline="\n")

    def tearDown(self):
        self.directory.cleanup()

    def run_main(self, *seeds):
        arguments = ["--translation", str(self.translation)]
        for seed in seeds:
            arguments += ["--seed", str(seed)]
        return verifier.main(arguments)

    def test_matching_seeds_are_accepted(self):
        self.assertEqual(self.run_main(self.first, self.second), 0)

    def test_altered_missing_duplicate_or_unexpected_seeds_are_rejected(self):
        self.first.write_text('{"a":2}', encoding="utf-8", newline="\n")
        self.assertEqual(self.run_main(self.first, self.second), 1)
        self.first.write_text('{"a":1}', encoding="utf-8", newline="\n")
        self.assertEqual(self.run_main(self.first), 1)
        self.assertEqual(self.run_main(self.first, self.first), 1)
        third = self.root / "c.json"
        third.write_text('{"c":3}', encoding="utf-8", newline="\n")
        self.assertEqual(self.run_main(self.first, self.second, third), 1)

    def test_broken_inputs_are_errors(self):
        self.assertEqual(self.run_main(self.first, self.root / "absent.json"), 2)
        (self.translation / "generation.json").write_text("{}", encoding="utf-8", newline="\n")
        self.assertEqual(self.run_main(self.first, self.second), 2)
        (self.translation / "generation.json").write_text(
            json.dumps({"seedSha256s": [digest(self.first), digest(self.first)]}), encoding="utf-8", newline="\n")
        self.assertEqual(self.run_main(self.first, self.second), 1)
        (self.translation / "generation.json").unlink()
        self.assertEqual(self.run_main(self.first, self.second), 2)

    def test_missing_seed_reports_its_recorded_index(self):
        import contextlib
        import io
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            self.assertEqual(self.run_main(self.first), 1)
        self.assertIn(f"missing {digest(self.second)} (recorded index 1)",
                      output.getvalue())
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            self.assertEqual(self.run_main(self.second), 1)
        self.assertIn(f"missing {digest(self.first)} (recorded index 0) [first recorded seed]",
                      output.getvalue())


if __name__ == "__main__":
    unittest.main()
