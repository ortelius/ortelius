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
