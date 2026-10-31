// Explicit source allowlist shared by the local server and source-only build.
export const webFiles=Object.freeze([
  'apps/web/index.html','apps/web/style.css','apps/web/main.mjs','apps/web/asset-worker.mjs','apps/web/renderer.mjs',
  'packages/simulation/twisted-system.mjs','packages/input/local-input.mjs','packages/input/browser-safety.mjs',
  ...['xdvdfs','xbp','identity','obe','character','animation','clips','transforms','texture'].map(name=>`src/${name}.mjs`)
]);
