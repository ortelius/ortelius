import { toByteArray } from "base64-js";
import Response from "./Response";

class StreamArrayBufferResponse {
    constructor(base64, stream, streamController, options) {
        const buffer = toByteArray(base64);
        this._base64 = base64;
        this._arrayBufferResponse = new Response(buffer, options);
        this._streamResponse = new Response(stream, options);
        streamController.enqueue(buffer);
    }

    get type() {
        return this._arrayBufferResponse.type;
    }

    get status() {
        return this._arrayBufferResponse.status;
    }

    get ok() {
        return this._arrayBufferResponse.ok;
    }

    get statusText() {
        return this._arrayBufferResponse.statusText;
    }

    get headers() {
        return this._arrayBufferResponse.headers;
    }

    get url() {
        return this._arrayBufferResponse.url;
    }

    get bodyUsed() {
        return (
            this._streamResponse.bodyUsed || this._arrayBufferResponse.bodyUsed
        );
    }

    clone() {
        let controller;
        const stream = new ReadableStream({
            start(c) {
                controller = c;
            },
        });

        return new StreamArrayBufferResponse(this._base64, stream, controller, {
            status: this._arrayBufferResponse.status,
            statusText: this._arrayBufferResponse.statusText,
            headers: new Headers(this._arrayBufferResponse.headers),
            url: this._arrayBufferResponse.url,
        });
    }

    blob() {
        return this._arrayBufferResponse.blob();
    }

    arrayBuffer() {
        return this._arrayBufferResponse.arrayBuffer();
    }

    text() {
        return this._arrayBufferResponse.text();
    }

    json() {
        return this._arrayBufferResponse.json();
    }

    formData() {
        return this._arrayBufferResponse.formData();
    }

    get body() {
        return this._streamResponse.body;
    }
}

export default StreamArrayBufferResponse;
