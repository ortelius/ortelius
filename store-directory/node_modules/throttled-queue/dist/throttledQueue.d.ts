declare function throttledQueue(maxRequestsPerInterval: number, interval: number, evenlySpaced?: boolean): <Return = unknown>(fn: () => Return | Promise<Return>) => Promise<Return>;
export default throttledQueue;
//# sourceMappingURL=throttledQueue.d.ts.map