'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

var TimeoutController = require('timeout-abort-controller');
var anySignal = require('any-signal');
var parseDuration = require('parse-duration');
var errors = require('./errors.js');

function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

var TimeoutController__default = /*#__PURE__*/_interopDefaultLegacy(TimeoutController);
var parseDuration__default = /*#__PURE__*/_interopDefaultLegacy(parseDuration);

function withTimeoutOption(fn, optionsArgIndex) {
  return (...args) => {
    const options = args[optionsArgIndex == null ? args.length - 1 : optionsArgIndex];
    if (!options || !options.timeout)
      return fn(...args);
    const timeout = typeof options.timeout === 'string' ? parseDuration__default["default"](options.timeout) : options.timeout;
    const controller = new TimeoutController__default["default"](timeout);
    options.signal = anySignal.anySignal([
      options.signal,
      controller.signal
    ]);
    const fnRes = fn(...args);
    const timeoutPromise = new Promise((_resolve, reject) => {
      controller.signal.addEventListener('abort', () => {
        reject(new errors.TimeoutError());
      });
    });
    const start = Date.now();
    const maybeThrowTimeoutError = () => {
      if (controller.signal.aborted) {
        throw new errors.TimeoutError();
      }
      const timeTaken = Date.now() - start;
      if (timeTaken > timeout) {
        controller.abort();
        throw new errors.TimeoutError();
      }
    };
    if (fnRes[Symbol.asyncIterator]) {
      return async function* () {
        const it = fnRes[Symbol.asyncIterator]();
        try {
          while (true) {
            const {value, done} = await Promise.race([
              it.next(),
              timeoutPromise
            ]);
            if (done) {
              break;
            }
            maybeThrowTimeoutError();
            yield value;
          }
        } catch (err) {
          maybeThrowTimeoutError();
          throw err;
        } finally {
          controller.clear();
          if (it.return) {
            it.return();
          }
        }
      }();
    }
    return (async () => {
      try {
        const res = await Promise.race([
          fnRes,
          timeoutPromise
        ]);
        maybeThrowTimeoutError();
        return res;
      } catch (err) {
        maybeThrowTimeoutError();
        throw err;
      } finally {
        controller.clear();
      }
    })();
  };
}

exports.withTimeoutOption = withTimeoutOption;
