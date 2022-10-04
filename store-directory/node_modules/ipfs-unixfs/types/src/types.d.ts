export interface Mtime {
    secs: number;
    nsecs?: number;
}
export declare type MtimeLike = Mtime | {
    Seconds: number;
    FractionalNanoseconds?: number;
} | [number, number] | Date;
//# sourceMappingURL=types.d.ts.map