const CACHE_NAME = 'fuzion-webgl-shader-recipes-v2';
const MAX_PROGRAMS = 64;
const MAX_BYTES = 1024 * 1024;

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

export async function openShaderWarmup(titleDigest) {
  const cache = await caches.open(CACHE_NAME);
  const address = new URL(`/sdk-browser/local-shader-recipes/${titleDigest}`, self.location.origin).href;
  const recipes = new Map();
  const observed = [];
  const encoder = new TextEncoder();
  let bytes = 2;
  let dirty = false;

  function remember(key, recipe) {
    let record = recipes.get(key);
    if (record) recipes.delete(key);
    else {
      if (!validRecipe(recipe)) return;
      const size = encoder.encode(key).byteLength + 1;
      if (size + 2 > MAX_BYTES) return;
      while (recipes.size >= MAX_PROGRAMS || size + bytes > MAX_BYTES) {
        const oldest = recipes.keys().next().value;
        bytes -= recipes.get(oldest).size;
        recipes.delete(oldest);
      }
      bytes += size;
      record = {recipe, size};
    }
    recipes.set(key, record);
    dirty = true;
  }

  // Ignore a damaged record and let successful draws populate a fresh one.
  let saved;
  try { saved = await readRecipes(await cache.match(address)); }
  catch { saved = []; }
  for (const recipe of saved) {
    if (validRecipe(recipe)) remember(JSON.stringify(recipe), recipe);
  }
  dirty = false;

  // The collector retains owned source references. Size accounting and
  // cache writes run after the title has stopped.
  return {
    recipes,
    observe(recipe, key) {
      if (observed.length === MAX_PROGRAMS) observed.shift();
      observed.push({recipe, key});
    },
    async save() {
      for (const {recipe, key} of observed) remember(key ?? JSON.stringify(recipe), recipe);
      observed.length = 0;
      if (!dirty) return;
      const body = `[${Array.from(recipes.keys()).join(',')}]`;
      await cache.put(address, new Response(body, {headers: {'Content-Type': 'application/json'}}));
      dirty = false;
    },
  };
}
