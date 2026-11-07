#!/usr/bin/env python3
"""Minimal contract tests for the offline SDK title-generation launcher."""
import importlib.util
from pathlib import Path
import tempfile
import unittest
from unittest import mock

SOURCE = Path(__file__).resolve().parents[1] / "tools" / "prepare-sdk-title.py"
SPEC = importlib.util.spec_from_file_location("prepare_sdk_title", SOURCE)
launcher = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(launcher)


class SDKTitlePrepareTests(unittest.TestCase):
    def setUp(self):
        self.sdk = Path("/private/sdk")
        self.config = Path("/private/research/config.toml")
        self.xbe = Path("/private/research/title.xbe")
        self.tool = Path("/private/research/tool/xbe_recomp")
        self.output = Path("/private/research/verification/run/generated")

    def test_pinned_input_digest_and_size_are_both_enforced(self):
        with tempfile.TemporaryDirectory(dir=Path(__file__).resolve().parents[1]) as temp:
            file = Path(temp) / "fixture"
            file.write_bytes(b"local-fixture")
            digest = launcher.sha256_file(file)
            self.assertEqual(launcher.check_pinned_file(file, digest, 32, 13), digest)
            with self.assertRaisesRegex(ValueError, "digest"):
                launcher.check_pinned_file(file, "0" * 64, 32, 13)
            with self.assertRaisesRegex(ValueError, "size"):
                launcher.check_pinned_file(file, digest, 4)

    def test_relative_private_input_becomes_absolute_before_invocation(self):
        with tempfile.TemporaryDirectory(dir=Path(__file__).resolve().parents[1]) as temp:
            workspace = Path(temp)
            research = workspace / "research"
            research.mkdir()
            config = research / "config.toml"
            config.write_text("fixture", encoding="utf-8")
            relative = config.relative_to(Path.cwd())
            resolved = launcher.resolve_private(relative, research, suffix=".toml")
            self.assertTrue(resolved.is_absolute())
            self.assertEqual(resolved, config.resolve())

    def test_native_command_passes_args_without_shell(self):
        with mock.patch.object(launcher.platform, "system", return_value="Linux"):
            command = launcher.build_command(sdk_root=self.sdk, config=self.config, xbe=self.xbe,
                                             tool=self.tool, generated=self.output, use_wsl=False)
        self.assertEqual(command, [str(self.tool), str(self.config), str(self.xbe),
                                   str(self.output), "--xdk", str(self.sdk / "xdk")])
        self.assertTrue(all(isinstance(arg, str) for arg in command))

    def test_windows_requires_explicit_wsl_and_builds_argv(self):
        with mock.patch.object(launcher.platform, "system", return_value="Windows"):
            with self.assertRaisesRegex(ValueError, "explicit --wsl"):
                launcher.build_command(sdk_root=self.sdk, config=self.config, xbe=self.xbe,
                                       tool=self.tool, generated=self.output, use_wsl=False)
            with mock.patch.object(launcher.shutil, "which", return_value="C:/Windows/wsl.exe"), \
                 mock.patch.object(launcher, "wsl_path", side_effect=lambda p: "/mnt/c/" + Path(p).name):
                command = launcher.build_command(sdk_root=self.sdk, config=self.config, xbe=self.xbe,
                                                 tool=self.tool, generated=self.output, use_wsl=True)
        self.assertEqual(command[:4], ["C:/Windows/wsl.exe", "--cd", "/mnt/c/run", "--exec"])
        self.assertEqual(command[-2:], ["--xdk", "/mnt/c/xdk"])

    def test_wsl_path_conversion_rejects_unc_paths(self):
        self.assertEqual(launcher.windows_path_to_wsl(r"C:\Codex\Project\file.xbe"),
                         "/mnt/c/Codex/Project/file.xbe")
        with self.assertRaisesRegex(ValueError, "local drive-letter"):
            launcher.windows_path_to_wsl(r"\\server\share\file.xbe")

    def test_existing_output_is_never_overwritten(self):
        launcher.VERIFY.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=launcher.VERIFY) as temp:
            with self.assertRaisesRegex(ValueError, "already exists"):
                launcher.prepare(Path(temp), sdk_root=self.sdk, config=self.config,
                                 xbe=self.xbe, tool=self.tool, use_wsl=False)

    def test_unexpected_output_path_is_rejected(self):
        with tempfile.TemporaryDirectory(dir=Path(__file__).resolve().parents[1]) as temp:
            root = Path(temp)
            generated = root / "generated"
            generated.mkdir()
            log = root / "translator.log"
            log.write_text("fixture", encoding="utf-8")
            launcher.ensure_output_scope(root, generated, log)
            (root / "unexpected.txt").write_text("fixture", encoding="utf-8")
            with self.assertRaisesRegex(ValueError, "unexpected path"):
                launcher.ensure_output_scope(root, generated, log)


if __name__ == "__main__":
    unittest.main()
