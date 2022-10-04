export type FetchOptions = import('../types').FetchOptions;
export type ProgressFn = import('../types').ProgressFn;
/**
 * @param {string | Request} url
 * @param {FetchOptions} options
 */
declare function fetchWith(url: string | Request, options?: FetchOptions): any;
import { Request } from "../fetch";
import { Headers } from "../fetch";
export { fetchWith as fetch, Request, Headers };
//# sourceMappingURL=fetch.browser.d.ts.map