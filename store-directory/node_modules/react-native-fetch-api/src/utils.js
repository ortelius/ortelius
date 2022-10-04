function createBlobReader(blob) {
    const reader = new FileReader();
    const fileReaderReady = new Promise((resolve, reject) => {
        reader.onload = function () {
            resolve(reader.result);
        };
        reader.onerror = function () {
            reject(reader.error);
        };
    });

    return {
        readAsArrayBuffer: async () => {
            reader.readAsArrayBuffer(blob);
            return fileReaderReady;
        },
        readAsText: async () => {
            reader.readAsText(blob);
            return fileReaderReady;
        },
    };
}

async function drainStream(stream) {
    const chunks = [];
    const reader = stream.getReader();

    function readNextChunk() {
        return reader.read().then(({ done, value }) => {
            if (done) {
                return chunks.reduce(
                    (bytes, chunk) => [...bytes, ...chunk],
                    []
                );
            }

            chunks.push(value);

            return readNextChunk();
        });
    }

    const bytes = await readNextChunk();

    return new Uint8Array(bytes);
}

function readArrayBufferAsText(array) {
    const decoder = new TextDecoder();

    return decoder.decode(array);
}

export { createBlobReader, drainStream, readArrayBufferAsText };
