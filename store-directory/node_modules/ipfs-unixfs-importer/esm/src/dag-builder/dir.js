import { UnixFS } from 'ipfs-unixfs';
import persist from '../utils/persist.js';
import {
  encode,
  prepare
} from '@ipld/dag-pb';
const dirBuilder = async (item, blockstore, options) => {
  const unixfs = new UnixFS({
    type: 'directory',
    mtime: item.mtime,
    mode: item.mode
  });
  const buffer = encode(prepare({ Data: unixfs.marshal() }));
  const cid = await persist(buffer, blockstore, options);
  const path = item.path;
  return {
    cid,
    path,
    unixfs,
    size: buffer.length
  };
};
export default dirBuilder;