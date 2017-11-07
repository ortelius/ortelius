/*
 *  DeployHub is an Agile Application Release Automation Solution
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
