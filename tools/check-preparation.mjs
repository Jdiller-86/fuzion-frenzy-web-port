import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import { execFileSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const required = ['AGENTS.md', 'README.md', 'DEVELOPMENT_PLAN.md', 'docs/DEVELOPMENT_HANDOFF.md',
  'docs/PROJECT_STATUS.md', 'docs/PROJECT_CONCERNS.md', 'docs/DECISIONS.md', 'docs/ANALYSIS_SETUP.md',
  'docs/PLAYER_SUPPLIED_CONTENT.md', 'docs/SCOPE_AND_FIDELITY.md',
  '.codex/config.toml', '.github/workflows/prepare.yml', '.node-version', 'package-lock.json'];
for (const name of required) assert.ok(fs.existsSync(path.join(root, name)), `Missing ${name}`);
const pkg = JSON.parse(fs.readFileSync(path.join(root, 'package.json'), 'utf8'));
assert.equal(pkg.private, true);
assert.equal(pkg.packageManager, 'npm@11.9.0');
assert.equal(fs.readFileSync(path.join(root, '.node-version'), 'utf8').trim(), '24.14.0');
const model = fs.readFileSync(path.join(root, '.codex/config.toml'), 'utf8');
assert.match(model, /^model = "gpt-6\.1-terra"$/m);
assert.match(model, /^model_reasoning_effort = "high"$/m);
assert.doesNotMatch(model, /approval_policy|sandbox_mode|mcp_servers|api_key/);
const manifest = JSON.parse(fs.readFileSync(path.join(root, 'tools/analysis-toolchain.json'), 'utf8'));
assert.equal(manifest.ghidra.version, '12.1.4');
assert.equal(manifest.xbeLoader.ghidraVersion, manifest.ghidra.version);
const files = [...new Set(execFileSync('git', ['ls-files', '-co', '--exclude-standard', '-z'],
  { cwd: root, encoding: 'utf8' }).split('\0').filter(Boolean))];
assert.ok(files.length > 0, 'No source files discovered');
const forbidden = /(^|\/)(?:research|\.tools|node_modules|\.venv|Fuzion Frenzy \(USA\))(?:\/|$)|\.(?:7z|zip|iso|xbe|xbp|zro|xpr|xwb|xsb|dds|exe|dll|jar|class|pdb|gpr|db|sqlite3?|har|saz|pem|key)$/i;
const credentialPatterns = [/gh[pousr]_[A-Za-z0-9]{30,}/, /github_pat_[A-Za-z0-9_]{40,}/,
  /-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----/];
for (const name of files) {
  assert.ok(!forbidden.test(name), `Forbidden publication path: ${name}`);
  assert.ok(!/(^|\/)\.env(?:\.|$)/.test(name) || name === '.env.example', `Environment file: ${name}`);
  const file = path.join(root, name);
  const stat = fs.lstatSync(file);
  assert.ok(stat.isFile() && !stat.isSymbolicLink(), `Only regular source files allowed: ${name}`);
  assert.ok(stat.size < 5 * 1024 * 1024, `Review large file before publication: ${name}`);
  const bytes = fs.readFileSync(file);
  assert.ok(!bytes.includes(0), `Binary content is not allowed in this preparation-only repository: ${name}`);
  const contents = new TextDecoder('utf-8', { fatal: true }).decode(bytes);
  for (const pattern of credentialPatterns) assert.ok(!pattern.test(contents), `Potential credential in ${name}`);
}
const ignoreProbes = ['research/reference/default.xbe', '.tools/example.exe', 'Fuzion Frenzy (USA).7z', 'Fuzion Frenzy (USA)/Packages/BootSeq.xbp', '.env'];
for (const name of ignoreProbes) execFileSync('git', ['check-ignore', '--no-index', '--quiet', name], { cwd: root });
console.log(`Preparation contract and publication guard passed for ${files.length} source files. No gameplay checks exist yet.`);
