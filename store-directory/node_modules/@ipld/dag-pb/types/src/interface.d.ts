import { CID } from 'multiformats/cid';
export interface PBLink {
    Name?: string;
    Tsize?: number;
    Hash: CID;
}
export interface PBNode {
    Data?: Uint8Array;
    Links: PBLink[];
}
export interface RawPBLink {
    Name: string;
    Tsize: number;
    Hash: Uint8Array;
}
export interface RawPBNode {
    Data: Uint8Array;
    Links: RawPBLink[];
}
//# sourceMappingURL=interface.d.ts.map