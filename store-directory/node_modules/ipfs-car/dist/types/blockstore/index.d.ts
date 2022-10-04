import { Block } from '@ipld/car/api';
import type { Blockstore as IpfsBlockstore } from 'interface-blockstore';
export interface Blockstore extends IpfsBlockstore {
    blocks(): AsyncGenerator<Block, void, unknown>;
}
