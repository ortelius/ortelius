export const fetch: typeof globalThis.fetch;
export const FormData: {
    new (form?: HTMLFormElement | undefined): FormData;
    prototype: FormData;
};
export const Headers: {
    new (init?: HeadersInit | undefined): Headers;
    prototype: Headers;
};
export const Request: {
    new (input: RequestInfo, init?: RequestInit | undefined): Request;
    prototype: Request;
};
export const Response: {
    new (body?: BodyInit | null | undefined, init?: ResponseInit | undefined): Response;
    prototype: Response;
    error(): Response;
    redirect(url: string | URL, status?: number | undefined): Response;
};
export const Blob: {
    new (blobParts?: BlobPart[] | undefined, options?: BlobPropertyBag | undefined): Blob;
    prototype: Blob;
};
export const File: {
    new (fileBits: BlobPart[], fileName: string, options?: FilePropertyBag | undefined): File;
    prototype: File;
};
export const ReadableStream: {
    new <R = any>(underlyingSource?: UnderlyingSource<R> | undefined, strategy?: QueuingStrategy<R> | undefined): ReadableStream<R>;
    prototype: ReadableStream<any>;
};
export const Blockstore: typeof MemoryBlockStore;
import { MemoryBlockStore } from "ipfs-car/blockstore/memory";
//# sourceMappingURL=platform.web.d.ts.map