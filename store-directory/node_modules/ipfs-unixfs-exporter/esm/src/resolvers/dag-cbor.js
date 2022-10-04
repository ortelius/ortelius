import { CID } from 'multiformats/cid';
import errCode from 'err-code';
import * as dagCbor from '@ipld/dag-cbor';
const resolve = async (cid, name, path, toResolve, resolve, depth, blockstore, options) => {
  const block = await blockstore.get(cid);
  const object = dagCbor.decode(block);
  let subObject = object;
  let subPath = path;
  while (toResolve.length) {
    const prop = toResolve[0];
    if (prop in subObject) {
      toResolve.shift();
      subPath = `${ subPath }/${ prop }`;
      const subObjectCid = CID.asCID(subObject[prop]);
      if (subObjectCid) {
        return {
          entry: {
            type: 'object',
            name,
            path,
            cid,
            node: block,
            depth,
            size: block.length,
            content: async function* () {
              yield object;
            }
          },
          next: {
            cid: subObjectCid,
            name: prop,
            path: subPath,
            toResolve
          }
        };
      }
      subObject = subObject[prop];
    } else {
      throw errCode(new Error(`No property named ${ prop } found in cbor node ${ cid }`), 'ERR_NO_PROP');
    }
  }
  return {
    entry: {
      type: 'object',
      name,
      path,
      cid,
      node: block,
      depth,
      size: block.length,
      content: async function* () {
        yield object;
      }
    }
  };
};
export default resolve;