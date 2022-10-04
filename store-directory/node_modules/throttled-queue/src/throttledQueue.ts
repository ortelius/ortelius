function throttledQueue(
  maxRequestsPerInterval: number,
  interval: number,
  evenlySpaced = false,
) {
  /**
   * If all requests should be evenly spaced, adjust to suit.
   */
  if (evenlySpaced) {
    interval = interval / maxRequestsPerInterval;
    maxRequestsPerInterval = 1;
  }
  const queue: Array<() => Promise<void>> = [];
  let lastIntervalStart = 0;
  let numRequestsPerInterval = 0;
  let timeout: NodeJS.Timeout | undefined;
  /**
   * Gets called at a set interval to remove items from the queue.
   * This is a self-adjusting timer, since the browser's setTimeout is highly inaccurate.
   */
  const dequeue = () => {
    const intervalEnd = lastIntervalStart + interval;
    const now = Date.now();
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
    for (const callback of queue.splice(0, maxRequestsPerInterval)) {
      numRequestsPerInterval++;
      void callback();
    }
    if (queue.length) {
      timeout = setTimeout(dequeue, interval);
    } else {
      timeout = undefined;
    }
  };

  return <Return = unknown>(fn: () => Promise<Return> | Return): Promise<Return> => new Promise<Return>(
    (resolve, reject) => {
      const callback = () => Promise.resolve().then(fn).then(resolve).catch(reject);
      const now = Date.now();
      if (timeout === undefined && (now - lastIntervalStart) > interval) {
        lastIntervalStart = now;
        numRequestsPerInterval = 0;
      }
      if (numRequestsPerInterval++ < maxRequestsPerInterval) {
        void callback();
      } else {
        queue.push(callback);
        if (timeout === undefined) {
          timeout = setTimeout(dequeue, lastIntervalStart + interval - now);
        }
      }
    },
  );
}
module.exports = throttledQueue;
export default throttledQueue;

