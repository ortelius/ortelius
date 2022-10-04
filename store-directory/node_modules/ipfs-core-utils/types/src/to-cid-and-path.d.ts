/**
 * @param {string|Uint8Array|CID} string
 * @returns {{cid:CID, path?:string}}
 */
export function toCidAndPath(string: string | Uint8Array | CID): {
    cid: CID;
    path?: string | undefined;
};
import { CID } from "multiformats/cid";
//# sourceMappingURL=to-cid-and-path.d.ts.map