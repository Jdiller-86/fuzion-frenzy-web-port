/* SDL's pointer signature aliases need normalization in the pinned SDK's
 * legacy dynamic-call path. This adapter is deliberately wasm32-only.
 */
#if MEMORY64
#error wasm32-dyncall.js cannot be used for memory64
#endif
#if !DYNCALLS
#error wasm32-dyncall.js requires -sDYNCALLS=1
#endif
mergeInto(LibraryManager.library, {
  $dynCall__deps: ['$dynCallLegacy'],
  $dynCall: function(signature, pointer, args) {
    return dynCallLegacy(signature.replace(/p/g, 'i'), pointer, args || []);
  }
});
