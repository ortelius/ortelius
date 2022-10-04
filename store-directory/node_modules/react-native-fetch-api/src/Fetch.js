import { Networking } from "react-native";
import pDefer from "p-defer";
import Request from "./Request";
import Response from "./Response";
import StreamBlobResponse from "./StreamBlobResponse";
import StreamArrayBufferResponse from "./StreamArrayBufferResponse";

class AbortError extends Error {
    constructor() {
        super("Aborted");
        this.name = "AbortError";
        Error.captureStackTrace?.(this, this.constructor);
    }
}

function createStream(cancel) {
    let streamController;

    const stream = new ReadableStream({
        start(controller) {
            streamController = controller;
        },
        cancel,
    });

    return {
        stream,
        streamController,
    };
}

class Fetch {
    _nativeNetworkSubscriptions = new Set();
    _nativeResponseType = "blob";
    _nativeRequestHeaders = {};
    _nativeResponseHeaders = {};
    _nativeRequestTimeout = 0;
    _nativeResponse;
    _textEncoder = new TextEncoder();
    _requestId;
    _response;
    _streamController;
    _deferredPromise;
    _responseStatus = 0; // requests shall not time out
    _responseUrl = "";

    constructor(resource, options = {}) {
        this._request = new Request(resource, options);

        if (this._request.signal?.aborted) {
            throw new AbortError();
        }

        this._abortFn = this.__abort.bind(this);
        this._deferredPromise = pDefer();
        this._request.signal?.addEventListener("abort", this._abortFn);

        for (const [name, value] of this._request.headers.entries()) {
            this._nativeRequestHeaders[name] = value;
        }

        this.__setNativeResponseType(options);
        this.__doFetch();

        return this._deferredPromise.promise;
    }

    __setNativeResponseType({ reactNative }) {
        if (reactNative?.textStreaming) {
            this._nativeResponseType = "text";

            return;
        }

        this._nativeResponseType =
            reactNative?.__nativeResponseType ?? this._nativeResponseType;
    }

    __subscribeToNetworkEvents() {
        [
            "didReceiveNetworkResponse",
            "didReceiveNetworkData",
            "didReceiveNetworkIncrementalData",
            // "didReceiveNetworkDataProgress",
            "didCompleteNetworkResponse",
        ].forEach((eventName) => {
            const subscription = Networking.addListener(eventName, (args) => {
                this[`__${eventName}`](...args);
            });
            this._nativeNetworkSubscriptions.add(subscription);
        });
    }

    __clearNetworkSubscriptions() {
        this._nativeNetworkSubscriptions.forEach((subscription) =>
            subscription.remove()
        );
        this._nativeNetworkSubscriptions.clear();
    }

    __abort() {
        this._requestId && Networking.abortRequest(this._requestId);
        this._streamController?.error(new AbortError());
        this._deferredPromise.reject(new AbortError());
        this.__clearNetworkSubscriptions();
    }

    __doFetch() {
        this.__subscribeToNetworkEvents();

        Networking.sendRequest(
            this._request.method,
            this._request.url.toString(), // request tracking name
            this._request.url.toString(),
            this._nativeRequestHeaders,
            this._request._body._bodyInit ?? null,
            this._nativeResponseType,
            this._nativeResponseType === "text", // send incremental events only when response type is text
            this._nativeRequestTimeout,
            this.__didCreateRequest.bind(this),
            ["include", "same-origin"].includes(this._request.credentials) // with credentials
        );
    }

    __didCreateRequest(requestId) {
        // console.log("fetch __didCreateRequest", { requestId });
        this._requestId = requestId;
    }

    __didReceiveNetworkResponse(requestId, status, headers, url) {
        // console.log("fetch __didReceiveNetworkResponse", {
        //     requestId,
        //     status,
        //     headers,
        //     url,
        // });

        if (requestId !== this._requestId) {
            return;
        }

        const { stream, streamController } = createStream(() => {
            this.__clearNetworkSubscriptions();
            Networking.abortRequest(this._requestId);
        });

        this._streamController = streamController;
        this._stream = stream;
        this._responseStatus = status;
        this._nativeResponseHeaders = headers;
        this._responseUrl = url;

        if (this._nativeResponseType === "text") {
            this._response = new Response(stream, { status, headers, url });
            this._deferredPromise.resolve(this._response);
        }
    }

    __didReceiveNetworkData(requestId, response) {
        // console.log("fetch __didReceiveNetworkData", { requestId, response });
        if (requestId !== this._requestId) {
            return;
        }

        this._nativeResponse = response;
    }

    __didReceiveNetworkIncrementalData(
        requestId,
        responseText,
        progress,
        total
    ) {
        // console.log("fetch __didReceiveNetworkIncrementalData", {
        //     requestId,
        //     responseText,
        //     progress,
        //     total,
        // });
        if (requestId !== this._requestId) {
            return;
        }

        const typedArray = this._textEncoder.encode(responseText, {
            stream: true,
        });
        this._streamController.enqueue(typedArray);
    }

    // __didReceiveNetworkDataProgress(requestId, loaded, total) {
    //     console.log('fetch __didReceiveNetworkDataProgress', { requestId, loaded, total });
    //     if (requestId !== this._requestId) {
    //         return;
    //     }
    // }

    async __didCompleteNetworkResponse(requestId, errorMessage, didTimeOut) {
        // console.log("fetch __didCompleteNetworkResponse", {
        //     requestId,
        //     errorMessage,
        //     didTimeOut,
        // });

        if (requestId !== this._requestId) {
            return;
        }

        this.__clearNetworkSubscriptions();
        this._request.signal?.removeEventListener("abort", this._abortFn);

        if (didTimeOut) {
            this.__closeStream();

            return this._deferredPromise.reject(
                new TypeError("Network request timed out")
            );
        }

        if (errorMessage) {
            this.__closeStream();

            return this._deferredPromise.reject(
                new TypeError(`Network request failed: ${errorMessage}`)
            );
        }

        if (this._nativeResponseType === "text") {
            this.__closeStream();
            return;
        }

        let ResponseClass;

        if (this._nativeResponseType === "blob") {
            ResponseClass = StreamBlobResponse;
        }

        if (this._nativeResponseType === "base64") {
            ResponseClass = StreamArrayBufferResponse;
        }

        try {
            this._response = await new ResponseClass(
                this._nativeResponse,
                this._stream,
                this._streamController,
                {
                    status: this._responseStatus,
                    url: this._responseUrl,
                    headers: this._nativeResponseHeaders,
                }
            );
            this._deferredPromise.resolve(this._response);
        } catch (error) {
            this._deferredPromise.reject(error);
        } finally {
            this.__closeStream();
        }
    }

    __closeStream() {
        this._streamController?.close();
    }
}

export default Fetch;
