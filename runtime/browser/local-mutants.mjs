const HEADER_INDEX = 0xffffffff;
const CAPACITY = 4;
const PROFILE_KEYS = ['kind', 'synthetic', 'capacity'];
const STATE_KEYS = [...PROFILE_KEYS, 'objectCount', 'handleCount', 'status', 'objects', 'handles'];
const STATUSES = new Set([0, 0x40000000, 0xc000000d, 0xc00000bb, 0xc0000005, 0xc000009a, 0xc0000024]);

export function parseLocalMutantProfile(value) {
  if (value === undefined) return undefined;
  if (!value || typeof value !== 'object' || Array.isArray(value) || Reflect.ownKeys(value).length !== 3 ||
      PROFILE_KEYS.some(key => !Object.hasOwn(value, key)) || value.kind !== 'synthetic-win32-mutants' ||
      value.synthetic !== true || value.capacity !== CAPACITY) throw new Error('Invalid synthetic Win32 mutant profile');
  return { kind: 'synthetic-win32-mutants', synthetic: true, capacity: CAPACITY };
}

function u32(getter, index, field, label) {
  const value = getter(index, field);
  if (!Number.isInteger(value) || value < 0 || value > 0xffffffff) throw new Error(`Invalid local mutant ${label}`);
  return value;
}
function exact(value, keys, label) {
  if (!value || typeof value !== 'object' || Array.isArray(value)) throw new Error(`Invalid local mutant ${label}`);
  const actual = Reflect.ownKeys(value);
  if (actual.length !== keys.length || keys.some(key => !Object.hasOwn(value, key)) || actual.some(key => !keys.includes(key)))
    throw new Error(`Invalid local mutant ${label} shape`);
}

export function readLocalMutantState(getter, profile) {
  const selected = parseLocalMutantProfile(profile);
  if (typeof getter !== 'function' || !selected) throw new Error('Local mutant profile and state getter are required');
  const h = field => u32(getter, HEADER_INDEX, field, `header ${field}`);
  const ready = h(0), objectCount = h(1), handleCount = h(2), capacity = h(3), status = h(4);
  if (ready !== 1 || objectCount > CAPACITY || handleCount > CAPACITY || objectCount > handleCount || capacity !== CAPACITY || !STATUSES.has(status))
    throw new Error('Invalid local mutant state header');
  const objects = [];
  for (let i = 0; i < objectCount; i++) {
    const object = u32(getter, i, 0, `object ${i}`), nameBytes = u32(getter, i, 1, `name length ${i}`);
    const named = u32(getter, i, 2, `named flag ${i}`), owned = u32(getter, i, 3, `ownership ${i}`);
    if (object !== i + 1 || nameBytes > 63 || named > 1 || owned !== 0 || (named ? nameBytes < 1 : nameBytes !== 0))
      throw new Error('Invalid local mutant object row');
    objects.push({ object, nameBytes, named: !!named, owned: false });
  }
  const handles = [];
  for (let i = 0; i < handleCount; i++) {
    const handle = u32(getter, i, 4, `handle ${i}`), object = u32(getter, i, 5, `handle object ${i}`);
    if (handle !== 0x80000 + 4 * (i + 1) || object < 1 || object > objectCount) throw new Error('Invalid local mutant handle row');
    handles.push({ handle, object });
  }
  const seen = new Set(); let nextObject = 1;
  for (const row of handles) if (!seen.has(row.object)) {
    if (row.object !== nextObject++) throw new Error('Local mutant objects must appear in first-reference order');
    seen.add(row.object);
  }
  if (seen.size !== objectCount) throw new Error('Every local mutant object must have a handle');
  for (const object of objects) if (!object.named && handles.filter(row => row.object === object.object).length !== 1)
    throw new Error('Anonymous mutant objects cannot be reopened');
  if (status === 0x40000000 && (handleCount < 2 || !handles.slice(0, -1).some(row => row.object === handles.at(-1).object) || !objects[handles.at(-1).object - 1].named))
    throw new Error('Informational mutant status requires opening an existing named object');
  if (status === 0 && handleCount && handles.slice(0, -1).some(row => row.object === handles.at(-1).object))
    throw new Error('Fresh mutant status cannot end with an existing object open');
  if (status === 0xc000009a && handleCount !== CAPACITY) throw new Error('Mutant capacity refusal requires a full handle table');
  return { ...selected, objectCount, handleCount, status, objects, handles };
}

export function readLocalMutantStateFromWasm(getter, profile) {
  if (typeof getter !== 'function') throw new Error('Invalid local mutant WASM getter');
  return readLocalMutantState((index, field) => {
    const value = getter(index, field);
    if (!Number.isInteger(value) || value < -0x80000000 || value > 0xffffffff)
      throw new Error(`Invalid local mutant WASM word ${index}:${field}`);
    return value >>> 0;
  }, profile);
}

export function validateLocalMutantState(value, profile) {
  const selected = parseLocalMutantProfile(profile);
  if (!selected) throw new Error('Local mutant profile is required');
  exact(value, STATE_KEYS, 'serialized state');
  if (PROFILE_KEYS.some(key => value[key] !== selected[key])) throw new Error('Serialized local mutant profile mismatch');
  if (!Number.isInteger(value.objectCount) || value.objectCount < 0 || value.objectCount > CAPACITY ||
      !Number.isInteger(value.handleCount) || value.handleCount < 0 || value.handleCount > CAPACITY ||
      !Array.isArray(value.objects) || value.objects.length !== value.objectCount ||
      !Array.isArray(value.handles) || value.handles.length !== value.handleCount)
    throw new Error('Invalid serialized local mutant tables');
  value.objects.forEach((row, i) => exact(row, ['object', 'nameBytes', 'named', 'owned'], `object ${i}`));
  value.handles.forEach((row, i) => exact(row, ['handle', 'object'], `handle ${i}`));
  const canonical = readLocalMutantState((index, field) => {
    if (index === HEADER_INDEX) return [1, value.objectCount, value.handleCount, CAPACITY, value.status][field];
    if (field < 4) return [value.objects[index]?.object, value.objects[index]?.nameBytes, value.objects[index]?.named ? 1 : 0, value.objects[index]?.owned ? 1 : 0][field];
    return [value.handles[index]?.handle, value.handles[index]?.object][field - 4];
  }, selected);
  if (JSON.stringify(canonical) !== JSON.stringify(value)) throw new Error('Serialized local mutant state is inconsistent');
  return canonical;
}
