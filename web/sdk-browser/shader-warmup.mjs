const CACHE_NAME = 'fuzion-webgl-shader-recipes-v1';
const MAX_PROGRAMS = 64;
const MAX_BYTES = 1024 * 1024;
const SAVE_DELAY_MS = 1000;

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

export async function openShaderWarmup(titleDigest) {
  const cache = await caches.open(CACHE_NAME);
  const address = new URL(`/sdk-browser/local-shader-recipes/${titleDigest}`, self.location.origin).href;
  const recipes = new Map();
  const encoder = new TextEncoder();
  let bytes = 2;
  let dirty = false;
  let saveTimer;
  let pendingSave = Promise.resolve();

  function remember(sources, key = JSON.stringify(sources)) {
    const existing = recipes.get(key);
    if (existing) {
      recipes.delete(key);
      recipes.set(key, existing);
      dirty = true;
      return;
    }
    const size = encoder.encode(key).byteLength + 1;
    if (size + 2 > MAX_BYTES) return;
    while (recipes.size >= MAX_PROGRAMS || size + bytes > MAX_BYTES) {
      const oldest = recipes.keys().next().value;
      bytes -= recipes.get(oldest).size;
      recipes.delete(oldest);
    }
    recipes.set(key, {sources, size});
    bytes += size;
    dirty = true;
  }

  // Ignore a damaged record and let successful draws populate a fresh one.
  let saved;
  try { saved = await readRecipes(await cache.match(address)); }
  catch { saved = []; }
  for (const sources of saved) {
    if (Array.isArray(sources) && sources.length === 2 &&
        sources.every(source => typeof source === 'string' && source.startsWith('#version 300 es\n'))) {
      remember(sources);
    }
  }
  dirty = false;

  function save() {
    clearTimeout(saveTimer);
    saveTimer = undefined;
    // Serialize writes so a Stop flush cannot leave an older snapshot behind.
    pendingSave = pendingSave.catch(() => {}).then(async () => {
      if (!dirty) return;
      dirty = false;
      try {
        // The URL names this origin's cache entry. No network request is made.
        const body = `[${Array.from(recipes.keys()).join(',')}]`;
        await cache.put(address, new Response(body, {headers: {'Content-Type': 'application/json'}}));
      } catch (error) {
        dirty = true;
        throw error;
      }
    });
    return pendingSave;
  }

  return {
    recipes,
    remember(sources, key) {
      remember(sources, key);
      if (dirty && saveTimer === undefined) {
        saveTimer = setTimeout(() => save().catch(() => {}), SAVE_DELAY_MS);
      }
    },
    save,
  };
}
