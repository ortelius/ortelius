import BlobManager from "react-native/Libraries/Blob/BlobManager";
import Response from "./Response";
import { createBlobReader } from "./utils";

class StreamBlobResponse {
    constructor(blobData, stream, streamController, options) {
        const blob = BlobManager.createFromOptions(blobData);
        this._blobData = blobData;
        this._blobResponse = new Response(blob, options);

        return createBlobReader(blob)
            .readAsArrayBuffer()
            .then((arrayBuffer) => {
                this._streamResponse = new Response(stream, options);
                this._arrayBufferResponse = new Response(arrayBuffer, options);
                streamController.enqueue(new Uint8Array(arrayBuffer));

                return this;
            })
            .catch(() => {
                return this;
            });
    }

    get bodyUsed() {
        return (
            this._blobResponse.bodyUsed ||
            !!this._streamResponse?.bodyUsed ||
            !!this._arrayBufferResponse?.bodyUsed
        );
    }

    get type() {
        return this._blobResponse.type;
    }

    get status() {
        return this._blobResponse.status;
    }

    get ok() {
        return this._blobResponse.ok;
    }

    get statusText() {
        return this._blobResponse.statusText;
    }

    get headers() {
        return this._blobResponse.headers;
    }

    get url() {
        return this._blobResponse.url;
    }

    clone() {
        let controller;
        const stream = new ReadableStream({
            start(c) {
                controller = c;
            },
        });

        return new StreamBlobResponse(this._blobData, stream, controller, {
            status: this._blobResponse.status,
            statusText: this._blobResponse.statusText,
            headers: new Headers(this._blobResponse.headers),
            url: this._blobResponse.url,
        });
    }

    blob() {
        return this._blobResponse.blob();
    }

    arrayBuffer() {
        return (
            this._arrayBufferResponse?.arrayBuffer() ??
            this._blobResponse.arrayBuffer()
        );
    }

    text() {
        return this._blobResponse.text();
    }

    json() {
        return this._blobResponse.json();
    }

    formData() {
        return this._blobResponse.formData();
    }

    get body() {
        return this._streamResponse?.body;
    }
}

export default StreamBlobResponse;
