export default DirFlat;
export type ImporterOptions = import('./types').ImporterOptions;
export type ImportResult = import('./types').ImportResult;
export type InProgressImportResult = import('./types').InProgressImportResult;
export type Blockstore = import('interface-blockstore').Blockstore;
export type DirProps = import('./dir').DirProps;
export type PBNode = import('@ipld/dag-pb').PBNode;
export type PBLink = import('@ipld/dag-pb').PBLink;
/**
 * @typedef {import('./types').ImporterOptions} ImporterOptions
 * @typedef {import('./types').ImportResult} ImportResult
 * @typedef {import('./types').InProgressImportResult} InProgressImportResult
 * @typedef {import('interface-blockstore').Blockstore} Blockstore
 * @typedef {import('./dir').DirProps} DirProps
 * @typedef {import('@ipld/dag-pb').PBNode} PBNode
 * @typedef {import('@ipld/dag-pb').PBLink} PBLink
 */
declare class DirFlat extends Dir {
    /** @type {{ [key: string]: InProgressImportResult | Dir }} */
    _children: {
        [key: string]: import("./types").InProgressImportResult | Dir;
    };
    childCount(): number;
    directChildrenCount(): number;
    onlyChild(): import("./types").InProgressImportResult | Dir;
}
import Dir from "./dir.js";
//# sourceMappingURL=dir-flat.d.ts.map