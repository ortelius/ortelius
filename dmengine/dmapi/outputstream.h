/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __outputstream_h

#define __outputstream_h


#include <stdarg.h>

#include "dmapiexport.h"


/**
 * Simple interface that allows output to be written to the implementing object.
 */
class DMAPI_API IOutputStream
{
public:
	virtual void writeToStdOut(const char *fmt, ...) = 0;
	virtual void writevToStdOut(long threadId, const char *buffer) = 0;
	virtual void writeBufferToStdOut(long threadId, const char *buffer, int len) = 0;

	virtual void writeToStdErr(const char *fmt, ...) = 0;
	virtual void writevToStdErr(long threadId, const char *buffer) = 0;
	virtual void writeBufferToStdErr(long threadId, const char *buffer, int len) = 0;
};


/**
 * Concrete class that implements the OutputStream interface and writes the
 * output to a buffer which can be later printed into another OutputStream.
 */
class DMAPI_API OutputStream : public /*virtual*/ IOutputStream
{
private:
	char *m_buffer;
	long m_len;
	int m_refCount;
	bool m_autoNewline;

public:
	OutputStream();
	OutputStream(bool autoNewline);
	~OutputStream();

	void addRef();
	void releaseRef();

	void writeToStdOut(const char *fmt, ...);
	void writevToStdOut(long threadId, const char *buffer);
	void writeBufferToStdOut(long threadId, const char *buffer, int len);

	void writeToStdErr(const char *fmt, ...);
	void writevToStdErr(long threadId, const char *buffer);
	void writeBufferToStdErr(long threadId, const char *buffer, int len);

	void readFromStdIn();

	const char *buffer();
	long size();
	void truncate();
};


#endif /*__outputstream_h*/
