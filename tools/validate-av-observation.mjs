#!/usr/bin/env node
/* Read a local capture record, validate it and print a summary.
 *
 * This tool never writes, uploads or reads original game content. The raw
 * capture stays wherever the player stored it; only the validated summary is
 * printed. A measured word outside the consumer domain is reported, not
 * rejected or replaced.
 */
import fs from 'node:fs';
import { validateAvObservation } from '../runtime/browser/av-observation.mjs';

const file = process.argv[2];
if (!file) {
  console.error('usage: node tools/validate-av-observation.mjs <local-capture.json>');
  process.exit(2);
}
let input;
try {
  input = JSON.parse(fs.readFileSync(file, 'utf8').replace(/^\uFEFF/, ''));
} catch (error) {
  console.error(`rejected: cannot read observation JSON: ${error.message}`);
  process.exit(1);
}
try {
  console.log(JSON.stringify(validateAvObservation(input), null, 2));
} catch (error) {
  console.error(`rejected: ${error.message}`);
  process.exit(1);
}
