import assert from 'node:assert/strict';
import crypto from 'node:crypto';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import test from 'node:test';
import { findSeeds } from '../tools/verify-recomp-package.mjs';

const digest = file => crypto.createHash('sha256').update(fs.readFileSync(file)).digest('hex');

test('findSeeds matches recorded digests and reports missing seeds', () => {
  const root = fs.mkdtempSync(path.join(os.tmpdir(), 'recomp-package-'));
  try {
    /* Keep the fixtures above the 100-byte scan floor. */
    const first = path.join(root, 'a.json');
    const second = path.join(root, 'b.json');
    fs.writeFileSync(first, JSON.stringify({ a: 'x'.repeat(200) }));
    fs.writeFileSync(second, JSON.stringify({ b: 'y'.repeat(200) }));
    const missing = 'f'.repeat(64);
    const result = findSeeds([root], [digest(first), digest(second), missing]);
    assert.deepEqual([...result.seeds].sort(), [first, second].sort());
    assert.deepEqual(result.missing, [missing]);
  } finally {
    fs.rmSync(root, { recursive: true, force: true });
  }
});

test('findSeeds rejects a missing seed root', () => {
  assert.throws(() => findSeeds([path.join(os.tmpdir(), 'absent-recomp-package-root')], []));
});
