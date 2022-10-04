import { CID } from 'multiformats/cid'

/*
  PBNode and PBLink match the DAG-PB logical format, as described at:
  https://github.com/ipld/specs/blob/master/block-layer/codecs/dag-pb.md#logical-format
*/

export interface PBLink {
  Name?: string,
  Tsize?: number,
  Hash: CID
}

export interface PBNode {
  Data?: Uint8Array,
  Links: PBLink[]
}

// Raw versions of PBNode and PBLink used internally to deal with the underlying
// encode/decode byte interface.
// A future iteration could make pb-encode.js and pb-decode.js aware of PBNode
// and PBLink specifics (including CID and optionals).

export interface RawPBLink {
  Name: string,
  Tsize: number,
  Hash: Uint8Array
}

export interface RawPBNode {
  Data: Uint8Array,
  Links: RawPBLink[]
}

