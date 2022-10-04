import TimeoutController from 'timeout-abort-controller';
import { anySignal } from 'any-signal';
import parseDuration from 'parse-duration';
import { TimeoutError } from './errors.js';
export function withTimeoutOption(fn, optionsArgIndex) {
  return (...args) => {
    const options = args[optionsArgIndex == null ? args.length - 1 : optionsArgIndex];
    if (!options || !options.timeout)
      return fn(...args);
    const timeout = typeof options.timeout === 'string' ? parseDuration(options.timeout) : options.timeout;
    const controller = new TimeoutController(timeout);
    options.signal = anySignal([
      options.signal,
      controller.signal
    ]);
    const fnRes = fn(...args);
    const timeoutPromise = new Promise((_resolve, reject) => {
      controller.signal.addEventListener('abort', () => {
        reject(new TimeoutError());
      });
    });
    const start = Date.now();
    const maybeThrowTimeoutError = () => {
      if (controller.signal.aborted) {
        throw new TimeoutError();
      }
      const timeTaken = Date.now() - start;
      if (timeTaken > timeout) {
        controller.abort();
        throw new TimeoutError();
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