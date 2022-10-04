export class SimpleCarSplitter {
    /**
     * @param {Blob} blob
     * @param {number} targetSize
     */
    static fromBlob(blob: Blob, targetSize: number): Promise<SimpleCarSplitter>;
    /**
     * @param {AsyncIterable<Uint8Array>} iterable
     * @param {number} targetSize
     */
    static fromIterable(iterable: AsyncIterable<Uint8Array>, targetSize: number): Promise<SimpleCarSplitter>;
    /**
     * @param {ICarReader} reader
     * @param {number} targetSize
     */
    constructor(reader: ICarReader, targetSize: number);
    _reader: ICarReader;
    _targetSize: number;
    cars(): AsyncGenerator<AsyncIterable<Uint8Array>, void, unknown>;
}
export type ICarReader = import('@ipld/car/api').BlockReader & import('@ipld/car/api').RootsReader;
//# sourceMappingURL=splitter.d.ts.map