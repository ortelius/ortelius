/**
 * @typedef {import('@ipld/car/api').BlockReader & import('@ipld/car/api').RootsReader} ICarReader
 */
export class SimpleCarJoiner {
    /**
     * @param {Iterable<ICarReader>} cars
     */
    constructor(cars: Iterable<ICarReader>);
    /** @type {ICarReader[]} */
    _cars: ICarReader[];
    car(): AsyncGenerator<Uint8Array, void, undefined>;
}
export type ICarReader = import('@ipld/car/api').BlockReader & import('@ipld/car/api').RootsReader;
//# sourceMappingURL=joiner.d.ts.map