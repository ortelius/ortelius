import Body from "./Body";
import Headers from "./Headers";

class Response {
    constructor(body, options = {}) {
        this.type = "basic";
        this.status = options.status ?? 200;
        this.ok = this.status >= 200 && this.status < 300;
        this.statusText = options.statusText ?? "";
        this.headers = new Headers(options.headers);
        this.url = options.url ?? "";
        this._body = new Body(body);

        if (!this.headers.has("content-type") && this._body._mimeType) {
            this.headers.set("content-type", this._body._mimeType);
        }
    }

    get bodyUsed() {
        return this._body.bodyUsed;
    }

    clone() {
        return new Response(this._body._bodyInit, {
            status: this.status,
            statusText: this.statusText,
            headers: new Headers(this.headers),
            url: this.url,
        });
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

    get body() {
        return this._body.body;
    }
}

Response.error = () => {
    const response = new Response(null, { status: 0, statusText: "" });
    response.type = "error";
    return response;
};

Response.redirect = (url, status) => {
    const redirectStatuses = [301, 302, 303, 307, 308];

    if (!redirectStatuses.includes(status)) {
        throw new RangeError(`Invalid status code: ${status}`);
    }

    return new Response(null, { status: status, headers: { location: url } });
};

export default Response;
