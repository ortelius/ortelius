import $protobuf from "protobufjs/minimal.js";
/** Properties of a Data. */
export interface IData {

    /** Data Type */
    Type: Data.DataType;

    /** Data Data */
    Data?: (Uint8Array|null);

    /** Data filesize */
    filesize?: (number|null);

    /** Data blocksizes */
    blocksizes?: (number[]|null);

    /** Data hashType */
    hashType?: (number|null);

    /** Data fanout */
    fanout?: (number|null);

    /** Data mode */
    mode?: (number|null);

    /** Data mtime */
    mtime?: (IUnixTime|null);
}

/** Represents a Data. */
export class Data implements IData {

    /**
     * Constructs a new Data.
     * @param [p] Properties to set
     */
    constructor(p?: IData);

    /** Data Type. */
    public Type: Data.DataType;

    /** Data Data. */
    public Data: Uint8Array;

    /** Data filesize. */
    public filesize: number;

    /** Data blocksizes. */
    public blocksizes: number[];

    /** Data hashType. */
    public hashType: number;

    /** Data fanout. */
    public fanout: number;

    /** Data mode. */
    public mode: number;

    /** Data mtime. */
    public mtime?: (IUnixTime|null);

    /**
     * Encodes the specified Data message. Does not implicitly {@link Data.verify|verify} messages.
     * @param m Data message or plain object to encode
     * @param [w] Writer to encode to
     * @returns Writer
     */
    public static encode(m: IData, w?: $protobuf.Writer): $protobuf.Writer;

    /**
     * Decodes a Data message from the specified reader or buffer.
     * @param r Reader or buffer to decode from
     * @param [l] Message length if known beforehand
     * @returns Data
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    public static decode(r: ($protobuf.Reader|Uint8Array), l?: number): Data;

    /**
     * Creates a Data message from a plain object. Also converts values to their respective internal types.
     * @param d Plain object
     * @returns Data
     */
    public static fromObject(d: { [k: string]: any }): Data;

    /**
     * Creates a plain object from a Data message. Also converts values to other types if specified.
     * @param m Data
     * @param [o] Conversion options
     * @returns Plain object
     */
    public static toObject(m: Data, o?: $protobuf.IConversionOptions): { [k: string]: any };

    /**
     * Converts this Data to JSON.
     * @returns JSON object
     */
    public toJSON(): { [k: string]: any };
}

export namespace Data {

    /** DataType enum. */
    enum DataType {
        Raw = 0,
        Directory = 1,
        File = 2,
        Metadata = 3,
        Symlink = 4,
        HAMTShard = 5
    }
}

/** Properties of an UnixTime. */
export interface IUnixTime {

    /** UnixTime Seconds */
    Seconds: number;

    /** UnixTime FractionalNanoseconds */
    FractionalNanoseconds?: (number|null);
}

/** Represents an UnixTime. */
export class UnixTime implements IUnixTime {

    /**
     * Constructs a new UnixTime.
     * @param [p] Properties to set
     */
    constructor(p?: IUnixTime);

    /** UnixTime Seconds. */
    public Seconds: number;

    /** UnixTime FractionalNanoseconds. */
    public FractionalNanoseconds: number;

    /**
     * Encodes the specified UnixTime message. Does not implicitly {@link UnixTime.verify|verify} messages.
     * @param m UnixTime message or plain object to encode
     * @param [w] Writer to encode to
     * @returns Writer
     */
    public static encode(m: IUnixTime, w?: $protobuf.Writer): $protobuf.Writer;

    /**
     * Decodes an UnixTime message from the specified reader or buffer.
     * @param r Reader or buffer to decode from
     * @param [l] Message length if known beforehand
     * @returns UnixTime
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    public static decode(r: ($protobuf.Reader|Uint8Array), l?: number): UnixTime;

    /**
     * Creates an UnixTime message from a plain object. Also converts values to their respective internal types.
     * @param d Plain object
     * @returns UnixTime
     */
    public static fromObject(d: { [k: string]: any }): UnixTime;

    /**
     * Creates a plain object from an UnixTime message. Also converts values to other types if specified.
     * @param m UnixTime
     * @param [o] Conversion options
     * @returns Plain object
     */
    public static toObject(m: UnixTime, o?: $protobuf.IConversionOptions): { [k: string]: any };

    /**
     * Converts this UnixTime to JSON.
     * @returns JSON object
     */
    public toJSON(): { [k: string]: any };
}

/** Properties of a Metadata. */
export interface IMetadata {

    /** Metadata MimeType */
    MimeType?: (string|null);
}

/** Represents a Metadata. */
export class Metadata implements IMetadata {

    /**
     * Constructs a new Metadata.
     * @param [p] Properties to set
     */
    constructor(p?: IMetadata);

    /** Metadata MimeType. */
    public MimeType: string;

    /**
     * Encodes the specified Metadata message. Does not implicitly {@link Metadata.verify|verify} messages.
     * @param m Metadata message or plain object to encode
     * @param [w] Writer to encode to
     * @returns Writer
     */
    public static encode(m: IMetadata, w?: $protobuf.Writer): $protobuf.Writer;

    /**
     * Decodes a Metadata message from the specified reader or buffer.
     * @param r Reader or buffer to decode from
     * @param [l] Message length if known beforehand
     * @returns Metadata
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    public static decode(r: ($protobuf.Reader|Uint8Array), l?: number): Metadata;

    /**
     * Creates a Metadata message from a plain object. Also converts values to their respective internal types.
     * @param d Plain object
     * @returns Metadata
     */
    public static fromObject(d: { [k: string]: any }): Metadata;

    /**
     * Creates a plain object from a Metadata message. Also converts values to other types if specified.
     * @param m Metadata
     * @param [o] Conversion options
     * @returns Plain object
     */
    public static toObject(m: Metadata, o?: $protobuf.IConversionOptions): { [k: string]: any };

    /**
     * Converts this Metadata to JSON.
     * @returns JSON object
     */
    public toJSON(): { [k: string]: any };
}
