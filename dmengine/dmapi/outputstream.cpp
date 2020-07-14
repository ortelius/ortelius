#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>

#include "outputstream.h"
#include "platform.h"


///////////////////////////////////////////////////////////////////////////////
// OutputStream
///////////////////////////////////////////////////////////////////////////////

OutputStream::OutputStream()
	: m_buffer(NULL), m_len(0), m_refCount(0), m_autoNewline(true)
{}


OutputStream::OutputStream(bool autoNewline)
	: m_buffer(NULL), m_len(0), m_refCount(0), m_autoNewline(autoNewline)
{}


OutputStream::~OutputStream()
{
	if(m_buffer) { free(m_buffer); m_buffer = NULL; }
}


void OutputStream::addRef()
{
	m_refCount++;
	debug3("addRef() - count now %d", m_refCount);
}


void OutputStream::releaseRef()
{
	m_refCount--;
	debug3("releaseRef() - count now %d", m_refCount);
	if(m_refCount == 0) {
		debug3("ref count zero on outputstream - deleting");
		delete this;
	}
}


void OutputStream::writeToStdOut(const char *fmt, ...)
{
	char *temp = NULL;
	va_list args;
	va_start(args, fmt);
	vasprintf(&temp,fmt,args);
	va_end(args);
	
	writevToStdOut(0, temp);
	free(temp);
}


void OutputStream::writevToStdOut(long threadId, const char *buffer)
{
	int len = strlen(buffer);
	
	if(m_buffer) {
		m_buffer = (char*)realloc(m_buffer, m_len + len + 2);
		// if this is not the first line we automatically start a new line
	} else {
		m_buffer = (char *)malloc(len+2);
		*m_buffer = '\0';
	}
	
	if (m_autoNewline && m_len) {
		strcat(m_buffer,"\n");
		strcat(m_buffer,buffer);
		m_len =strlen(m_buffer);
	} else {
		strcat(m_buffer,buffer);
		m_len =strlen(m_buffer);
	}			
	return;
}


void OutputStream::writeBufferToStdOut(long threadId, const char *buffer, int len)
{
	if(m_buffer) {
		m_buffer = (char*) realloc(m_buffer, m_len + len + 1);
		memcpy(&m_buffer[m_len], buffer, len);
		m_len += len;
		m_buffer[m_len] = '\0';
	} else {
		m_buffer = (char*) malloc(len + 1);
		memcpy(m_buffer, buffer, len);
		m_len = len;
		m_buffer[m_len] = '\0';
	}
}


void OutputStream::writeToStdErr(const char *fmt, ...)
{
	char *temp = NULL;
	va_list args;
	va_start(args, fmt);
	vasprintf(&temp,fmt,args);
	va_end(args);
	
	writevToStdErr(0, temp);
	free(temp);
}


void OutputStream::writevToStdErr(long threadId, const char *buffer)
{
	// error just redirects to out so that the order of the lines is preserved
	writevToStdOut(0, buffer);
}


void OutputStream::writeBufferToStdErr(long threadId, const char *buffer, int len)
{
	// error just redirects to out so that the order of the lines is preserved
	writeBufferToStdOut(0, buffer, len);
}


void OutputStream::readFromStdIn()
{
	// Read input from stdin
	char line[1024];
	while(fgets(line, sizeof(line), stdin)) {
		//debug1("STDIN: %s", line);
		writeBufferToStdOut(0, line, strlen(line));
	}
	//debug1("STDIN END");
}


const char *OutputStream::buffer()
{
	return m_buffer ? m_buffer : "";
}


long OutputStream::size()
{
	return m_len;
}

void OutputStream::truncate()
{
	if (m_buffer) {
		free(m_buffer);
		m_buffer = NULL;
	}
	m_len=0;
}
