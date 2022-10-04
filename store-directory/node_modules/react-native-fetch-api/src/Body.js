import { createBlobReader, drainStream, readArrayBufferAsText } from "./utils";

class Body {
    constructor(body) {
        this.bodyUsed = false;
        this._bodyInit = body;

        if (!body) {
            this._bodyText = "";
            return this;
        }

        if (body instanceof Blob) {
            this._bodyBlob = body;
            this._mimeType = body.type;
            return this;
        }

        if (body instanceof FormData) {
            this._bodyFormData = body;
            this._mimeType = "multipart/form-data";
            return this;
        }

        if (body instanceof URLSearchParams) {
            // URLSearchParams is not handled natively so we reassign bodyInit for fetch to send it as text
            this._bodyText = this._bodyInit = body.toString();
            this._mimeType = "application/x-www-form-urlencoded;charset=UTF-8";
            return this;
        }

        if (body instanceof ArrayBuffer || ArrayBuffer.isView(body)) {
            this._bodyArrayBuffer = body.slice?.(0) ?? body.buffer;
            this._mimeType = "application/octet-stream";
            return this;
        }

        if (body instanceof ReadableStream) {
            this._bodyReadableStream = body;
            this._mimeType = "application/octet-stream";
            return this;
        }

        this._bodyText = body.toString();
        this._mimeType = "text/plain;charset=UTF-8";
    }

    __consumed() {
        if (this.bodyUsed) {
            return Promise.reject(new TypeError("Already read"));
        }
        this.bodyUsed = true;
    }

    async blob() {
        const alreadyConsumed = this.__consumed();
        if (alreadyConsumed) {
            return alreadyConsumed;
        }

        if (this._bodyBlob) {
            return this._bodyBlob;
        }

        // Currently not supported by React Native. It will throw.
        // Blobs cannot be constructed from ArrayBuffers or ArrayBufferViews.
        if (this._bodyReadableStream) {
            const typedArray = await drainStream(this._bodyReadableStream);

            return new Blob([typedArray]);
        }

        // Currently not supported by React Native. It will throw.
        // Blobs cannot be constructed from ArrayBuffers or ArrayBufferViews.
        if (this._bodyArrayBuffer) {
            if (ArrayBuffer.isView(this._bodyArrayBuffer)) {
                return new Blob([this._bodyArrayBuffer.buffer]);
            }

            return new Blob([this._bodyArrayBuffer]);
        }

        if (this._bodyFormData) {
            throw new Error("Could not read FormData body as blob");
        }

        return new Blob([this._bodyText]);
    }

    async arrayBuffer() {
        if (this._bodyText) {
            const blob = await this.blob();

            return createBlobReader(blob).readAsArrayBuffer();
        }

        const alreadyConsumed = this.__consumed();
        if (alreadyConsumed) {
            return alreadyConsumed;
        }

        if (this._bodyReadableStream) {
            const typedArray = await drainStream(this._bodyReadableStream);

            return typedArray.buffer;
        }

        if (this._bodyArrayBuffer) {
            if (ArrayBuffer.isView(this._bodyArrayBuffer)) {
                const {
                    buffer,
                    byteOffset,
                    byteLength,
                } = this._bodyArrayBuffer;

                return Promise.resolve(
                    buffer.slice(byteOffset, byteOffset + byteLength)
                );
            }

            return Promise.resolve(this._bodyArrayBuffer);
        }
    }

    async text() {
        const alreadyConsumed = this.__consumed();
        if (alreadyConsumed) {
            return alreadyConsumed;
        }

        if (this._bodyReadableStream) {
            const typedArray = await drainStream(this._bodyReadableStream);

            return readArrayBufferAsText(typedArray);
        }

        if (this._bodyBlob) {
            return createBlobReader(this._bodyBlob).readAsText();
        }

        if (this._bodyArrayBuffer) {
            return readArrayBufferAsText(this._bodyArrayBuffer);
        }

        if (this._bodyFormData) {
            throw new Error("Could not read FormData body as text");
        }

        return this._bodyText;
    }

    async json() {
        const text = await this.text();

        return JSON.parse(text);
    }

    async formData() {
        const text = await this.text();
        const formData = new FormData();

        text.trim()
            .split("&")
            .forEach((pairs) => {
                if (!pairs) {
                    return;
                }

                const split = pairs.split("=");
                const name = split.shift().replace(/\+/g, " ");
                const value = split.join("=").replace(/\+/g, " ");
                formData.append(
                    decodeURIComponent(name),
                    decodeURIComponent(value)
                );
            });

        return formData;
    }

    get body() {
        if (this._bodyReadableStream) {
            return this._bodyReadableStream;
        }

        if (this._bodyArrayBuffer) {
            const typedArray = new Uint8Array(this._bodyArrayBuffer);

            return new ReadableStream({
                start(controller) {
                    typedArray.forEach((chunk) => {
                        controller.enqueue(chunk);
                    });

                    controller.close();
                },
            });
        }

        if (this._bodyBlob) {
            return new ReadableStream({
                start: async (controller) => {
                    const arrayBuffer = await createBlobReader(
                        this._bodyBlob
                    ).readAsArrayBuffer();
                    const typedArray = new Uint8Array(arrayBuffer);

                    typedArray.forEach((chunk) => {
                        controller.enqueue(chunk);
                    });

                    controller.close();
                },
            });
        }

        const text = this._bodyFormData?.toString() ?? this._bodyText;

        return new ReadableStream({
            start: async (controller) => {
                const typedArray = new Uint8Array(text);

                typedArray.forEach((chunk) => {
                    controller.enqueue(chunk);
                });

                controller.close();
            },
        });
    }
}

export default Body;
