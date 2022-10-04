# fetch

[![npm version][npm-image]][npm-url] [![ci][github-ci-image]][github-ci-url]

[npm-url]:https://www.npmjs.com/package/react-native-fetch-api
[npm-image]:https://img.shields.io/npm/v/react-native-fetch-api.svg
[github-ci-url]:https://github.com/react-native-community/fetch/actions
[github-ci-image]:https://github.com/react-native-community/fetch/workflows/Node%20CI/badge.svg

> A fetch API polyfill for React Native with text streaming support

This is a fork of GitHub's fetch [polyfill](https://github.com/github/fetch), the fetch implementation React Native currently [provides](https://github.com/facebook/react-native/blob/master/Libraries/Network/fetch.js). This project features an alternative fetch implementation directy built on top of React Native's [Networking API](https://github.com/facebook/react-native/tree/master/Libraries/Network) instead of `XMLHttpRequest` for performance gains. At the same time, it aims to fill in some gaps of the [WHATWG specification](https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API) for fetch, namely the support for text streaming.

In practice, this implementation is a drop-in replacement to GitHub's polyfill as it closely follows its implementation. Do not use this implementation if your application does not require to stream text.

## Motivation

GitHub's fetch polyfill, originally designed with the intention to be used in web browsers without support for the fetch standard, most notably does not support the consumption of a response body as a stream.

However, as React Native does not yet provide direct access to the underlying byte stream for responses, we either have to fallback to [XMLHttpRequest](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest) or React Native's networking API for [iOS](https://github.com/facebook/react-native/blob/v0.63.4/Libraries/Network/RCTNetworking.ios.js) and [Android](https://github.com/facebook/react-native/blob/v0.63.4/Libraries/Network/RCTNetworking.android.js). Currently, only strings can be transfered through the bridge, thus binary data has to be base64-encoded ([source](https://github.com/react-native-community/discussions-and-proposals/issues/107)) and while React Native's XHR provides [progress events](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/progress_event) to receive incremental data, it concatenates the response string as data comes in. Although [very inefficient](https://github.com/jonnyreeves/fetch-readablestream/blob/cabccb98788a0141b001e6e775fc7fce87c62081/src/defaultTransportFactory.js#L33), the response can be sliced up, each chunk encoded into its UTF-8 representation with [TextEncoder](https://developer.mozilla.org/en-US/docs/Web/API/TextEncoder) and finally enqueued to the stream.

Instead of relying on `XMLHttpRequest`, which degrades performance, we remove it out of the equation and have fetch interact with React Native's Networking API directly instead. To make `Response.body` work, `ReadableStream`'s controller was integrated with native progress events. It's important to stress that progress events are only fired when the native response type is set to `text` (https://github.com/facebook/react-native/blob/v0.63.4/Libraries/Network/RCTNetworking.mm#L544-L547), therefore limiting streaming to text-only transfers. If you wish to consume binary data, either `blob` or `base64` response types have to be used. In this case, the downside is that the final response body is read as a whole and enqueued to the stream's controller as a single chunk. There is no way to read a partial response of a binary transfer.

For more context, read the following:
- https://github.com/github/fetch/issues/746
- https://github.com/facebook/react-native/issues/27741
- https://hpbn.co/xmlhttprequest/#streaming-data-with-xhr

Related:
- https://github.com/react-native-community/discussions-and-proposals/issues/99

## Requirements

React Native v0.62.0+ is the minimum version supported where the [Networking API has been made public](https://github.com/facebook/react-native/commit/42ee5ec93425c95dee6125a6ff6864ec647636aa).

This implementation depends on the following web APIs which are not currently available in React Native:

- [`TextEncoder`](https://developer.mozilla.org/en-US/docs/Web/API/TextEncoder/TextEncoder) 
- [`TextDecoder`](https://developer.mozilla.org/en-US/docs/Web/API/TextEncoder/TextDecoder)
- [`ReadableStream`](https://developer.mozilla.org/en-US/docs/Web/API/ReadableStream)

It should be possible remove the dependency on `TextEncoder` and `TextDecoder`, but not on `ReadableStream`. Either way, beware the bundle size of your application will inevitable increase.

To polyfill the above APIs, use [react-native-polyfill-globals](https://github.com/acostalima/react-native-polyfill-globals).

## Install

```
$ npm install react-native-fetch-api --save
```

## Setup

The APIs provided by GitHub's implementation in React Native  have to be replaced by those provided by this implementation. To do so, check and install [react-native-polyfill-globals](https://github.com/acostalima/react-native-polyfill-globals) and follow the instructions therein.

## Usage

No need to import anything after the [setup](#setup) is done. All APIs will be available globally.

Example:

```js
fetch('https://jsonplaceholder.typicode.com/todos/1')
  .then(response => response.json())
  .then(json => console.log(json))
```

Check fetch's [official documentation](https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API) to learn more about the concepts and extended usage.

### Enable text streaming

A non-standard option was added to `fetch` to enable incremental events in React Native's networking layer.

```js
fetch('https://jsonplaceholder.typicode.com/todos/1', { reactNative: { textStreaming: true } })
  .then(response => response.body)
  .then(stream => ...)
```
### Aborting requests

It's possible to [abort an on-going request](https://developers.google.com/web/updates/2017/09/abortable-fetch) and React Native already supports [`AbortController`](https://developer.mozilla.org/en-US/docs/Web/API/AbortController), so there is no need for a polyfill.

```js
const controller = new AbortController();

fetch('https://jsonplaceholder.typicode.com/todos/1', { signal: controller.signal })
  .then(response => response.json())
  .then(json => console.log(json))
```

Learn more about aborting fetch at https://developers.google.com/web/updates/2017/09/abortable-fetch.

### Cookies

There is no concept of Cross-Origin Resource Sharing (CORS) in native apps. React Native only accepts a boolean value for the [`credentials`](https://developer.mozilla.org/en-US/docs/Web/API/Request/credentials) option. As such, to send cookies you can either use `same-origin` and `include`.

The `Set-Cookie` response header returned from the server is a [forbidden header name](https://developer.mozilla.org/en-US/docs/Glossary/Forbidden_header_name) and therefore can't be programmatically read with `response.headers.get()`. Instead, the platform's native networking stack automatically manages cookies for you.

If you run into issues with cookie-based authentication, read the following:
- https://reactnative.dev/docs/network#known-issues-with-fetch-and-cookie-based-authentication
- https://build.affinity.co/persisting-sessions-with-react-native-4c46af3bfd83
- https://medium.com/locastic/react-native-cookie-based-authentication-80ee18f4c71b

Alternatively, you may consider using the [react-native-cookies](https://github.com/react-native-cookies/cookies).

### Request caching directive

The only values supported for the [`cache`](https://developer.mozilla.org/en-US/docs/Web/API/Request/cache) option are `no-cache` and `no-store` and Both achieve exactly the same result. All other values are ignored. Following GitHub's implementation, a cache-busting mechanism is provided by using the query parameter `_` which holds the number of milliseconds elapsed since the Epoch when either `no-cache` or `no-store` are specified.

### Redirect modes directive

The fetch specification defines these values for the [`redirect`](https://developer.mozilla.org/en-US/docs/Web/API/Request/redirect) option: `follow` (the default), `error`, and `manual`. React Native does not accept such option but it does transparently follow a redirect response given the `Location` header for 30x status codes.

## Tests

To run the test suite, you must use [`react-native-test-runner`](https://github.com/acostalima/react-native-test-runner) CLI. Run the `run-tests.js` wrapper script to spin up a local HTTP server to execute the networking tests against.

### iOS

```
$ ./run-tests.js --platform ios --simulator '<simulator>' test/index.js 
```

Where `<simulator>` can be a combination of a device type and iOS version, e.g. `iPhone 11 (14.1)`, or a device UUID.
Check which simulators are available in your system by running the following command:

```
$ xcrun xctrace list devices
```

### Android

```
$ ./run-tests.js --platform android --emulator '<emulator>' test/index.js 
```

Where `<emulator>` is the name of the Android Virtual Device (AVD), e.g. `Pixel_API_28_AOSP`.
Check which emulators are available in your system by running the following command:

```
$ emulator -list-avds
```

