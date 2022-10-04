# Safari 14 IndexedDB fix

Safari on macOS Big Sur 11.4 and iOS 14.6 has a [nasty bug](https://bugs.webkit.org/show_bug.cgi?id=226547) where IndexedDB requests get lost and never resolve. The issue was fixed in Safari 14.7.

This library (well, function) works around the issue and tells you when IndexedDB is actually available.

To install:

```
npm i safari-14-idb-fix
```

To use:

```js
import idbReady from 'safari-14-idb-fix';

idbReady().then(() => {
  // Safari has definitely figured out where IndexedDB is.
  // You can use IndexedDB as usual.
});
```

## All bundles

A modern build tool will handle the above example fine, but if you need a specific build:

- `safari-14-idb-fix/dist/index.js` EcmaScript module.
- `safari-14-idb-fix/dist/index.cjs` CommonJS module.
- `safari-14-idb-fix/dist/iife.min.js` Minified plain JS, which creates an `idbReady` global.
