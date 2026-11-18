const CACHE_NAME = 'fuzion-webgl-shader-recipes-v2';
const MAX_PROGRAMS = 64;
const MAX_BYTES = 1024 * 1024;
const CHECKPOINT_DELAY_MS = 1500;

async function readRecipes(response) {
  if (!response?.body) return [];
  const reader = response.body.getReader();
  const buffer = new Uint8Array(MAX_BYTES);
  let length = 0;
  for (;;) {
    const {done, value} = await reader.read();
    if (done) break;
    if (value.byteLength > buffer.length - length) {
      await reader.cancel();
      return [];
    }
    buffer.set(value, length);
    length += value.byteLength;
  }
  const records = JSON.parse(new TextDecoder('utf-8', {fatal: true}).decode(buffer.subarray(0, length)));
  return Array.isArray(records) && records.length <= MAX_PROGRAMS ? records : [];
}

function validRecipe(recipe) {
  if (!Array.isArray(recipe) || recipe.length !== 6) return false;
  const [vertex, pixel, kinds, shadows, flat, registers] = recipe;
  return (vertex === null || typeof vertex === 'string') && typeof pixel === 'string' &&
    Array.isArray(kinds) && kinds.length === 4 && kinds.every(kind => kind === 0 || kind === 1) &&
    Array.isArray(shadows) && shadows.length === 4 &&
    shadows.every((shadow, i) => shadow === 0 || ((shadow === 1 || shadow === 2) && kinds[i] === 1)) &&
    typeof flat === 'boolean' && Number.isInteger(registers) && registers >= 0 && registers <= 0xffff;
}

function validDrawLayout(layout) {
  return Array.isArray(layout) && layout.length === 17 &&
    Number.isInteger(layout[0]) && layout[0] >= 1 && layout[0] <= 3 &&
    layout.slice(1).every(format => Number.isInteger(format) && format >= 0 && format <= 255);
}

export async function openShaderWarmup(titleDigest) {
  const cache = await caches.open(CACHE_NAME);
  const address = new URL(`/sdk-browser/local-shader-recipes/${titleDigest}`, self.location.origin).href;
  const recipes = new Map();
  const observed = [];
  const encoder = new TextEncoder();
  let bytes = 2;
  let dirty = false;
  let checkpointTimer;
  let pendingWrite = Promise.resolve();

  function remember(recipe, key, drawLayout) {
    if (!validRecipe(recipe)) return;
    key ??= JSON.stringify(recipe);
    const previous = recipes.get(key);
    if (!validDrawLayout(drawLayout)) drawLayout = previous?.drawLayout;
    const serialized = drawLayout ? JSON.stringify([...recipe, drawLayout]) : key;
    const size = encoder.encode(serialized).byteLength + 1;
    if (size + 2 > MAX_BYTES) return;
    if (previous) {
      bytes -= previous.size;
      recipes.delete(key);
    }
    while (recipes.size >= MAX_PROGRAMS || size + bytes > MAX_BYTES) {
      const oldest = recipes.keys().next().value;
      bytes -= recipes.get(oldest).size;
      recipes.delete(oldest);
    }
    bytes += size;
    recipes.set(key, {recipe, drawLayout, serialized, size});
    dirty = true;
  }

  // Ignore a damaged record and let successful draws populate a fresh one.
  let saved;
  try { saved = await readRecipes(await cache.match(address)); }
  catch { saved = []; }
  for (const record of saved) {
    if (!Array.isArray(record) || (record.length !== 6 && record.length !== 7)) continue;
    remember(record.slice(0, 6), undefined, record[6]);
  }
  dirty = false;

  function save() {
    clearTimeout(checkpointTimer);
    checkpointTimer = undefined;
    // Stop and a background checkpoint can overlap. Drain observations and
    // write in order so a later snapshot cannot be overwritten by an older one.
    pendingWrite = pendingWrite.catch(() => {}).then(async () => {
      for (const {recipe, key, drawLayout} of observed) remember(recipe, key, drawLayout);
      observed.length = 0;
      if (!dirty) return;
      const body = `[${Array.from(recipes.values(), record => record.serialized).join(',')}]`;
      await cache.put(address, new Response(body, {headers: {'Content-Type': 'application/json'}}));
      dirty = false;
    });
    return pendingWrite;
  }

  return {
    recipes,
    observe(recipe, key, drawLayout) {
      if (observed.length === MAX_PROGRAMS) observed.shift();
      observed.push({recipe, key, drawLayout});
      // Resident draws also teach exact shaders for the next launch.
      if (checkpointTimer === undefined)
        checkpointTimer = setTimeout(() => save().catch(() => {}), CHECKPOINT_DELAY_MS);
    },
    save,
  };
}
