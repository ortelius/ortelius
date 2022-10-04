import Body from "./Body";
import Headers from "./Headers";

class Request {
    credentials = "same-origin";
    method = "GET";

    constructor(input, options = {}) {
        this.url = input;

        if (input instanceof Request) {
            if (input._body.bodyUsed) {
                throw new TypeError("Already read");
            }

            this.__handleRequestInput(input, options);
        }

        this._body = this._body ?? new Body(options.body);
        this.method = options.method ?? this.method;
        this.method = this.method.toUpperCase();

        if (this._body._bodyInit && ["GET", "HEAD"].includes(this.method)) {
            throw new TypeError("Body not allowed for GET or HEAD requests");
        }

        if (this._body._bodyReadableStream) {
            throw new TypeError("Streaming request bodies is not supported");
        }

        this.credentials = options.credentials ?? this.credentials;
        this.headers = this.headers ?? new Headers(options.headers);
        this.signal = options.signal ?? this.signal;

        if (!this.headers.has("content-type") && this._body._mimeType) {
            this.headers.set("content-type", this._body._mimeType);
        }

        this.__handleCacheOption(options.cache);
    }

    __handleRequestInput(request, options) {
        this.url = request.url;
        this.credentials = request.credentials;
        this.method = request.method;
        this.signal = request.signal;
        this.headers = new Headers(options.headers ?? request.headers);

        if (!options.body && request._body._bodyInit) {
            this._body = new Body(request._body._bodyInit);
            request._body.bodyUsed = true;
        }
    }

    __handleCacheOption(cache) {
        if (!["GET", "HEAD"].includes(this.method)) {
            return;
        }

        if (!["no-store", "no-cache"].includes(cache)) {
            return;
        }

        const currentTime = Date.now();
        // Search for a '_' parameter in the query string
        const querySearchRegExp = /([?&])_=[^&]*/;

        if (querySearchRegExp.test(this.url)) {
            this.url = this.url.replace(
                querySearchRegExp,
                `$1_=${currentTime}`
            );

            return;
        }

        const hasQueryRegExp = /\?/;
        const querySeparator = hasQueryRegExp.test(this.url) ? "&" : "?";

        this.url += `${querySeparator}_=${currentTime}`;
    }

    get bodyUsed() {
        return this._body.bodyUsed;
    }

    clone() {
        return new Request(this, { body: this._body._bodyInit });
    }

    blob() {
        return this._body.blob();
    }

    arrayBuffer() {
        return this._body.arrayBuffer();
    }

    text() {
        return this._body.text();
    }

    json() {
        return this._body.json();
    }

    formData() {
        return this._body.formData();
    }
}

export default Request;
