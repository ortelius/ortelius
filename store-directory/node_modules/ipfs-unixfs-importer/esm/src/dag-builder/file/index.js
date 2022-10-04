import errCode from 'err-code';
import { UnixFS } from 'ipfs-unixfs';
import persist from '../../utils/persist.js';
import {
  encode,
  prepare
} from '@ipld/dag-pb';
import parallelBatch from 'it-parallel-batch';
import * as rawCodec from 'multiformats/codecs/raw';
import * as dagPb from '@ipld/dag-pb';
import dagFlat from './flat.js';
import dagBalanced from './balanced.js';
import dagTrickle from './trickle.js';
import bufferImporterFn from './buffer-importer.js';
const dagBuilders = {
  flat: dagFlat,
  balanced: dagBalanced,
  trickle: dagTrickle
};
async function* buildFileBatch(file, blockstore, options) {
  let count = -1;
  let previous;
  let bufferImporter;
  if (typeof options.bufferImporter === 'function') {
    bufferImporter = options.bufferImporter;
  } else {
    bufferImporter = bufferImporterFn;
  }
  for await (const entry of parallelBatch(bufferImporter(file, blockstore, options), options.blockWriteConcurrency)) {
    count++;
    if (count === 0) {
      previous = entry;
      continue;
    } else if (count === 1 && previous) {
      yield previous;
      previous = null;
    }
    yield entry;
  }
  if (previous) {
    previous.single = true;
    yield previous;
  }
}
const reduce = (file, blockstore, options) => {
  async function reducer(leaves) {
    if (leaves.length === 1 && leaves[0].single && options.reduceSingleLeafToSelf) {
      const leaf = leaves[0];
      if (file.mtime !== undefined || file.mode !== undefined) {
        let buffer = await blockstore.get(leaf.cid);
        leaf.unixfs = new UnixFS({
          type: 'file',
          mtime: file.mtime,
          mode: file.mode,
          data: buffer
        });
        buffer = encode(prepare({ Data: leaf.unixfs.marshal() }));
        leaf.cid = await persist(buffer, blockstore, {
          ...options,
          codec: dagPb,
          hasher: options.hasher,
          cidVersion: options.cidVersion
        });
        leaf.size = buffer.length;
      }
      return {
        cid: leaf.cid,
        path: file.path,
        unixfs: leaf.unixfs,
        size: leaf.size
      };
    }
    const f = new UnixFS({
      type: 'file',
      mtime: file.mtime,
      mode: file.mode
    });
    const links = leaves.filter(leaf => {
      if (leaf.cid.code === rawCodec.code && leaf.size) {
        return true;
      }
      if (leaf.unixfs && !leaf.unixfs.data && leaf.unixfs.fileSize()) {
        return true;
      }
      return Boolean(leaf.unixfs && leaf.unixfs.data && leaf.unixfs.data.length);
    }).map(leaf => {
      if (leaf.cid.code === rawCodec.code) {
        f.addBlockSize(leaf.size);
        return {
          Name: '',
          Tsize: leaf.size,
          Hash: leaf.cid
        };
      }
      if (!leaf.unixfs || !leaf.unixfs.data) {
        f.addBlockSize(leaf.unixfs && leaf.unixfs.fileSize() || 0);
      } else {
        f.addBlockSize(leaf.unixfs.data.length);
      }
      return {
        Name: '',
        Tsize: leaf.size,
        Hash: leaf.cid
      };
    });
    const node = {
      Data: f.marshal(),
      Links: links
    };
    const buffer = encode(prepare(node));
    const cid = await persist(buffer, blockstore, options);
    return {
      cid,
      path: file.path,
      unixfs: f,
      size: buffer.length + node.Links.reduce((acc, curr) => acc + curr.Tsize, 0)
    };
  }
  return reducer;
};
function fileBuilder(file, block, options) {
  const dagBuilder = dagBuilders[options.strategy];
  if (!dagBuilder) {
    throw errCode(new Error(`Unknown importer build strategy name: ${ options.strategy }`), 'ERR_BAD_STRATEGY');
  }
  return dagBuilder(buildFileBatch(file, block, options), reduce(file, block, options), options);
}
export default fileBuilder;