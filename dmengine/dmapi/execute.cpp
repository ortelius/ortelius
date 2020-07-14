#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "execute.h"
#include "platform.h"


#if 0
#define TRIDEBUG(x) printf ##x; printf("\n")
#else
#define TRIDEBUG(x) 
#endif
#define TRIERROR(x) printf ##x; printf("\n")


///////////////////////////////////////////////////////////////////////////////
// CaptureData
///////////////////////////////////////////////////////////////////////////////

CapturedData::CapturedData()
	: m_bytesOfStandardOut(0), m_bytesOfStandardErr(0),
	  m_standardOut(NULL), m_standardErr(NULL),
	  m_capacityOfStandardOut(0), m_capacityOfStandardErr(0)
#ifdef WIN32
	  , m_bytesOfStandardIn(0), m_standardIn(NULL)
#endif /*WIN32*/
{}


CapturedData::~CapturedData()
{
	SAFE_FREE(m_standardOut);
	SAFE_FREE(m_standardErr);
#ifdef WIN32
	SAFE_FREE(m_standardIn);
#endif /*WIN32*/
}


void CapturedData::reallocateStandardOut(long len)
{
	TRIDEBUG(("reallocateStandardOut - Need to increment buffer, capacity=%ld, occupied=%ld, len=%ld", m_capacityOfStandardOut, m_bytesOfStandardOut, len));

	long increments = (len / STD_BUFFER_SIZE_INCREMENT) + 1;
	m_capacityOfStandardOut += (increments * STD_BUFFER_SIZE_INCREMENT);
	m_standardOut = (char*) realloc(m_standardOut, m_capacityOfStandardOut);

	TRIDEBUG(("reallocateStandardOut - capacity=%ld, occupied=%ld", m_capacityOfStandardOut, m_bytesOfStandardOut));
}


void CapturedData::reallocateStandardErr(long len)
{
	TRIDEBUG(("reallocateStandardErr - Need to increment buffer, capacity=%ld, occupied=%ld, len=%ld", m_capacityOfStandardErr, m_bytesOfStandardErr, len));

	long increments = (len / STD_BUFFER_SIZE_INCREMENT) + 1;
	m_capacityOfStandardErr += (increments * STD_BUFFER_SIZE_INCREMENT);
	m_standardErr = (char*) realloc(m_standardErr, m_capacityOfStandardErr);

	TRIDEBUG(("reallocateStandardErr - capacity=%ld, occupied=%ld", m_capacityOfStandardErr, m_bytesOfStandardErr));
}


void CapturedData::appendStandardOut(const char *text, long len)
{
	// If first time through, create the buffer
	if(!m_standardOut) {
		m_standardOut = (char*) malloc(STD_BUFFER_SIZE_INCREMENT);
		m_capacityOfStandardOut = STD_BUFFER_SIZE_INCREMENT;
	}

	// If text we are appending is bigger than buffer, increase buffer size
	if(m_capacityOfStandardOut < (m_bytesOfStandardOut + len)) {
		reallocateStandardOut(len);
	}

	// Append the new text to the end of the buffer
	memcpy(&m_standardOut[m_bytesOfStandardOut], text, len);

	// Add more data to the total
	m_bytesOfStandardOut += len;

	// If we've only got so much space left then we need to create extend the buffer
	if(m_capacityOfStandardOut < (m_bytesOfStandardOut + STD_BUFFER_THRESHOLD))  {
		reallocateStandardOut(STD_BUFFER_THRESHOLD);
	}
}


void CapturedData::appendStandardErr(const char *text, long len)
{
	// If first time through, create the buffer
	if(!m_standardErr) {
		m_standardErr = (char*) malloc(STD_BUFFER_SIZE_INCREMENT);
		m_capacityOfStandardErr = STD_BUFFER_SIZE_INCREMENT;
	}

	// If text we are appending is bigger than buffer, increase buffer size
	if(m_capacityOfStandardErr < (m_bytesOfStandardErr + len)) {
		reallocateStandardErr(len);
	}

	// Append the new text to the end of the buffer
	memcpy(&m_standardErr[m_bytesOfStandardErr], text, len);

	// Add more data to the total
	m_bytesOfStandardErr += len;

	// If we've only got so much space left then we need to create extend the buffer
	if(m_capacityOfStandardErr < (m_bytesOfStandardErr + STD_BUFFER_THRESHOLD))  {
		reallocateStandardErr(STD_BUFFER_THRESHOLD);
	}
}
