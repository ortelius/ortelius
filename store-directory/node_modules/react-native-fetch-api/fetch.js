import Headers from "./src/Headers";
import Request from "./src/Request";
import Response from "./src/Response";
import Fetch from "./src/Fetch";

const fetch = (resource, options) => new Fetch(resource, options);

export { Headers, Request, Response, fetch };
