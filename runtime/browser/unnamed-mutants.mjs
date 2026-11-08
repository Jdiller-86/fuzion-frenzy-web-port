const HEADER_INDEX = 0xffffffff;
const CAPACITY = 4;
const PROFILE_KEYS = ['kind', 'synthetic', 'capacity'];
const STATE_KEYS = [...PROFILE_KEYS, 'count', 'status', 'handles'];
const STATUSES = new Set([0, 0xc000000d, 0xc00000bb, 0xc0000005, 0xc000009a]);

export function parseMutantProfile(value) {
  if (value === undefined) return undefined;
  if (value === null || typeof value !== 'object' || Array.isArray(value) ||
      Reflect.ownKeys(value).length !== 3 || PROFILE_KEYS.some(key => !Object.hasOwn(value, key)) ||
      value.kind !== 'synthetic-unnamed-unowned-mutants' || value.synthetic !== true || value.capacity !== CAPACITY) {
    throw new Error('Invalid synthetic unnamed mutant profile');
  }
  return { kind: 'synthetic-unnamed-unowned-mutants', synthetic: true, capacity: CAPACITY };
}

function readU32(getter, index, field, label) {
  const value = getter(index, field);
  if (!Number.isInteger(value) || value < 0 || value > 0xffffffff) throw new Error(`Invalid mutant ${label}`);
  return value;
}

function exactKeys(value, keys, label) {
  if (value === null || typeof value !== 'object' || Array.isArray(value)) throw new Error(`Invalid mutant ${label}`);
  const actual = Reflect.ownKeys(value);
  if (actual.length !== keys.length || keys.some(key => !Object.hasOwn(value, key)) || actual.some(key => !keys.includes(key))) {
    throw new Error(`Invalid mutant ${label} shape`);
  }
}

export function readMutantState(getter, profile) {
  if (typeof getter !== 'function') throw new Error('Invalid mutant state getter');
  const selected = parseMutantProfile(profile);
  if (selected === undefined) throw new Error('Mutant profile is required');
  const header = field => readU32(getter, HEADER_INDEX, field, `header field ${field}`);
  const ready = header(0), count = header(1), capacity = header(2), status = header(3);
  if (ready !== 1 || count > CAPACITY || capacity !== CAPACITY || !STATUSES.has(status)) {
    throw new Error('Invalid mutant state header');
  }
  const handles = [];
  for (let index = 0; index < count; index++) {
    const handle = readU32(getter, index, 0, `handle ${index}`);
    const owned = readU32(getter, index, 1, `ownership ${index}`);
    if (handle !== 0x00080000 + 4 * (index + 1) || owned !== 0) throw new Error('Invalid mutant row state');
    handles.push({ handle, owned: false });
  }
  return { ...selected, count, status, handles };
}

export function validateMutantState(value, profile) {
  const selected = parseMutantProfile(profile);
  if (selected === undefined) throw new Error('Mutant profile is required');
  exactKeys(value, STATE_KEYS, 'serialized state');
  if (PROFILE_KEYS.some(key => value[key] !== selected[key])) throw new Error('Serialized mutant profile does not match');
  for (const field of ['count', 'status']) {
    if (!Number.isInteger(value[field]) || value[field] < 0 || value[field] > 0xffffffff) throw new Error(`Invalid serialized mutant ${field}`);
  }
  if (value.count > CAPACITY) throw new Error('Serialized mutant count exceeds capacity');
  if (!Array.isArray(value.handles) || value.handles.length !== value.count) throw new Error('Invalid serialized mutant handles');
  const rows = value.handles.map((row, index) => {
    exactKeys(row, ['handle', 'owned'], `row ${index}`);
    if (!Number.isInteger(row.handle) || row.handle < 0 || row.handle > 0xffffffff || row.owned !== false) {
      throw new Error(`Invalid serialized mutant row ${index}`);
    }
    return { handle: row.handle, owned: false };
  });
  const canonical = readMutantState((index, field) => {
    if (index === HEADER_INDEX) return [1, value.count, CAPACITY, value.status][field];
    return [rows[index]?.handle, rows[index]?.owned ? 1 : 0][field];
  }, selected);
  if (JSON.stringify(canonical) !== JSON.stringify({ ...selected, count: value.count, status: value.status, handles: rows })) {
    throw new Error('Serialized mutant state is inconsistent');
  }
  return canonical;
}
