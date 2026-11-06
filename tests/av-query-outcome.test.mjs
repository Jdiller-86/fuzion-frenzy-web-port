import assert from 'node:assert/strict';
import test from 'node:test';
import { describeAvQueryOutcome } from '../runtime/browser/local-title-proof.mjs';

const report = (edit = {}) => ({
  experiments: ['avQuery'], avQueryWord: 0x00800303, calls: [], ...edit,
});

test('the startup checker labels an avQuery completion, denial or prequery truthfully', () => {
  assert.equal(describeAvQueryOutcome({}), '');
  assert.equal(describeAvQueryOutcome({ experiments: ['writeClearing'] }), '');
  assert.match(describeAvQueryOutcome(report({
    calls: [{
      ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264],
      output: { rawWord: 0x00800303, address: 2111264 },
    }],
  })), /completed: 0x00800303 at 0x00203720/);
  assert.match(describeAvQueryOutcome(report({
    calls: [{ ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] }],
  })), /denied\/unsupported at ordinal 2 \(configured 0x00800303\)/);
  assert.match(describeAvQueryOutcome(report()), /configured \(0x00800303\) but not reached \(prequery\)/);
  /* A completed call followed by a denial is still a denial outcome. */
  const completedThenDenied = describeAvQueryOutcome(report({
    stop: { status: 1, target: 0xFE000094, ordinal: 2 },
    calls: [
      { ordinal: 2, status: 0, arguments: [0, 6, 0, 2111264],
        output: { rawWord: 0x00800303, address: 2111264 } },
      { ordinal: 2, status: 1, arguments: [0, 6, 0, 2111264] },
    ],
  }));
  assert.match(completedThenDenied, /denied\/unsupported at ordinal 2/);
  assert.doesNotMatch(completedThenDenied, /completed/);
  assert.match(describeAvQueryOutcome(report({
    calls: [{ ordinal: 187, status: 0, arguments: [1] }],
  })), /prequery/);
});
