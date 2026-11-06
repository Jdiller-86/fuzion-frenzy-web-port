// Explicit source allowlist shared by the local server and source-only build.
export const webFiles=Object.freeze([
  'web/app/index.html','web/app/style.css','web/app/main.mjs','web/app/asset-worker.mjs','renderer/preview.mjs',
  'web/prototype/twisted-system.mjs','runtime/browser/input/local-input.mjs','runtime/browser/input/browser-safety.mjs',
  ...['xdvdfs','xbp','identity','obe','character','animation','clips','transforms','texture'].map(name=>`host/${name}.mjs`)
]);
