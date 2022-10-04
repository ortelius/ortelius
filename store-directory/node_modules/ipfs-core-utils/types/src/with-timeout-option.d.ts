/**
 * @template {any[]} Args
 * @template {Promise<any> | AsyncIterable<any>} R - The return type of `fn`
 * @param {(...args:Args) => R} fn
 * @param {number} [optionsArgIndex]
 * @returns {(...args:Args) => R}
 */
export function withTimeoutOption<Args extends any[], R extends Promise<any> | AsyncIterable<any>>(fn: (...args: Args) => R, optionsArgIndex?: number | undefined): (...args: Args) => R;
//# sourceMappingURL=with-timeout-option.d.ts.map