/* Validator for the external ordinal-2 original-hardware observation.
 *
 * This module does not model AvSendTVEncoderOption or choose a result. It checks
 * that a player-supplied capture is complete, attributable to original hardware
 * and internally consistent. A measured word outside the observed consumer
 * domain is retained as evidence, not rejected or altered.
 */
const HEX_BYTE = /^0x[0-9a-f]{2}$/i;
const HEX_WORD = /^0x[0-9a-f]{1,8}$/i;
const SHA256 = /^[0-9a-f]{64}$/i;
const STANDARDS = new Set([0x01, 0x02, 0x03]);
const PACKS = new Set([0x00, 0x03, 0x04, 0x05]);
const REFRESHES = new Set([0x00400000, 0x00800000]);

function text(value, field) {
  if (typeof value !== 'string' || !value.trim())
    throw new Error(`The observation requires a non-empty ${field}`);
  return value;
}
function bytes(value, field) {
  if (!Array.isArray(value) || value.length !== 4)
    throw new Error(`${field} must contain exactly four bytes`);
  return value.map((byte, index) => {
    if (typeof byte !== 'string' || !HEX_BYTE.test(byte))
      throw new Error(`${field}[${index}] must be one 0xNN byte`);
    return Number.parseInt(byte.slice(2), 16);
  });
}
function word(value) {
  return (value[0] | (value[1] << 8) | (value[2] << 16) | (value[3] << 24)) >>> 0;
}
function object(value, field) {
  if (!value || typeof value !== 'object' || Array.isArray(value))
    throw new Error(`The observation requires an object ${field}`);
  return value;
}

export function validateAvObservation(input) {
  const record = object(input, 'record');
  if (record.schema !== 'av-observation/1')
    throw new Error('Unsupported observation schema');
  if (record.source !== 'original-hardware')
    throw new Error('Only an original-hardware observation is accepted');

  const game = object(record.game, 'game');
  if (typeof game.sha256 !== 'string' || !SHA256.test(game.sha256))
    throw new Error('The observation requires the game SHA-256');
  text(game.name, 'game name');
  const kernel = object(record.kernel, 'kernel');
  text(kernel.version, 'kernel version');
  const consoleInfo = object(record.console, 'console');
  text(consoleInfo.videoConfiguration, 'console video configuration');
  text(consoleInfo.avPack, 'console AV pack');
  text(consoleInfo.cable, 'console AV cable');

  const call = object(record.call, 'call');
  if (call.registerBase !== 0 || call.option !== 6 || call.param !== 0)
    throw new Error('The observation must capture AvSendTVEncoderOption(0, 6, 0, Result)');
  if (typeof call.resultAddress !== 'string' || !HEX_WORD.test(call.resultAddress))
    throw new Error('The observation requires a hexadecimal Result address');
  const resultAddress = Number.parseInt(call.resultAddress, 16);
  if (!resultAddress || (resultAddress & 3))
    throw new Error('The Result address must be non-null and four-byte aligned');

  const before = bytes(record.before, 'before');
  const after = bytes(record.after, 'after');
  if (record.returnedNormally !== true)
    throw new Error('The original service must have returned normally');
  text(record.collection, 'collection method');
  text(record.instrumentation, 'instrumentation description');

  if (!Array.isArray(record.confirmations) || record.confirmations.length < 1 ||
      record.confirmations.length > 4)
    throw new Error('The observation requires one to four confirmations');
  const measured = word(after);
  const confirmations = record.confirmations.map((confirmation, index) => {
    const item = object(confirmation, `confirmations[${index}]`);
    const cBefore = bytes(item.before, `confirmations[${index}].before`);
    const cAfter = bytes(item.after, `confirmations[${index}].after`);
    if (item.returnedNormally !== true)
      throw new Error(`confirmation ${index} did not return normally`);
    const cWord = word(cAfter);
    if (cWord !== measured)
      text(item.variationNote, `confirmation ${index} variation note`);
    return {
      beforeWord: word(cBefore),
      word: cWord,
      returnedNormally: true,
      variation: cWord !== measured,
      variationNote: cWord !== measured ? item.variationNote : undefined,
    };
  });

  const standard = (measured >>> 8) & 0xff;
  const pack = measured & 0xff;
  const refresh = measured & 0xc00000;
  const consumerDomainMatch = STANDARDS.has(standard) && PACKS.has(pack) && REFRESHES.has(refresh);
  const notes = [];
  if (measured === 0)
    notes.push('Measured word is zero; it matches no observed consumer row.');
  if (!consumerDomainMatch)
    notes.push('Measured word is outside the observed consumer domain; retained as evidence, not altered.');
  if (word(before) === measured)
    notes.push('Result bytes did not change; retained as evidence.');
  if (confirmations.some(confirmation => confirmation.variation))
    notes.push('A confirmation differs; its variation note is recorded.');

  return {
    schema: 'av-observation/1',
    source: 'original-hardware',
    gameSha256: game.sha256.toLowerCase(),
    gameName: game.name,
    kernelVersion: kernel.version,
    console: {
      videoConfiguration: consoleInfo.videoConfiguration,
      avPack: consoleInfo.avPack,
      cable: consoleInfo.cable,
    },
    resultAddress: call.resultAddress.toLowerCase(),
    beforeWord: word(before),
    word: measured,
    changed: word(before) !== measured,
    consumerDomainMatch,
    confirmations,
    notes,
  };
}
