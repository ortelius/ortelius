"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function throttledQueue(maxRequestsPerInterval, interval, evenlySpaced) {
    if (evenlySpaced === void 0) { evenlySpaced = false; }
    /**
     * If all requests should be evenly spaced, adjust to suit.
     */
    if (evenlySpaced) {
        interval = interval / maxRequestsPerInterval;
        maxRequestsPerInterval = 1;
    }
    var queue = [];
    var lastIntervalStart = 0;
    var numRequestsPerInterval = 0;
    var timeout;
    /**
     * Gets called at a set interval to remove items from the queue.
     * This is a self-adjusting timer, since the browser's setTimeout is highly inaccurate.
     */
    var dequeue = function () {
        var intervalEnd = lastIntervalStart + interval;
        var now = Date.now();
        /**
         * Adjust the timer if it was called too early.
         */
        if (now < intervalEnd) {
            // eslint-disable-next-line @typescript-eslint/no-unsafe-argument
            timeout !== undefined && clearTimeout(timeout);
            timeout = setTimeout(dequeue, intervalEnd - now);
            return;
        }
        lastIntervalStart = now;
        numRequestsPerInterval = 0;
        for (var _i = 0, _a = queue.splice(0, maxRequestsPerInterval); _i < _a.length; _i++) {
            var callback = _a[_i];
            numRequestsPerInterval++;
            void callback();
        }
        if (queue.length) {
            timeout = setTimeout(dequeue, interval);
        }
        else {
            timeout = undefined;
        }
    };
    return function (fn) { return new Promise(function (resolve, reject) {
        var callback = function () { return Promise.resolve().then(fn).then(resolve).catch(reject); };
        var now = Date.now();
        if (timeout === undefined && (now - lastIntervalStart) > interval) {
            lastIntervalStart = now;
            numRequestsPerInterval = 0;
        }
        if (numRequestsPerInterval++ < maxRequestsPerInterval) {
            void callback();
        }
        else {
            queue.push(callback);
            if (timeout === undefined) {
                timeout = setTimeout(dequeue, lastIntervalStart + interval - now);
            }
        }
    }); };
}
module.exports = throttledQueue;
exports.default = throttledQueue;
//# sourceMappingURL=throttledQueue.js.map