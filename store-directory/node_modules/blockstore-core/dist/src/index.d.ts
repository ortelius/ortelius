export { BaseBlockstore } from "./base.js";
export { MemoryBlockstore } from "./memory.js";
export const Errors: {
    notFoundError(err?: Error | undefined): Error & import("err-code").Extensions;
    abortedError(err?: Error | undefined): Error & import("err-code").Extensions;
};
//# sourceMappingURL=index.d.ts.map