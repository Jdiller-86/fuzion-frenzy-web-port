// Pinned Emscripten compiler declaration only. The standalone Wasm's sole
// import is supplied by the bounded browser clock capability, not this glue.
mergeInto(LibraryManager.library, {
  ff_host_clock__sig: 'vi',
  ff_host_clock: function () {
    throw new Error('Supply the admitted browser clock capability');
  }
});
