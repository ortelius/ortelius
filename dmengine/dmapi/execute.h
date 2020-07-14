#ifndef __execute_h

#define __execute_h


#include "dmapiexport.h"


#ifdef WIN32

#define CHILD_EXITED(status) (true)
#define CHILD_EXITSTATUS(status) (status)
#define CHILD_SIGNALED(status) (false)
#define CHILD_TERMSIG(status) (0)

#else

#define CHILD_EXITED(status) WIFEXITED(status)
#define CHILD_EXITSTATUS(status) WEXITSTATUS(status)
#define CHILD_SIGNALED(status) WIFSIGNALED(status)
#define CHILD_TERMSIG(status) WTERMSIG(status)

#endif /*WIN32*/


#define STD_BUFFER_SIZE_INCREMENT 1024
#define STD_BUFFER_THRESHOLD 128


class DMAPI_API CapturedData
{
private:
	long	m_bytesOfStandardOut;
	long	m_bytesOfStandardErr;
	char	*m_standardOut;
	char	*m_standardErr;
	long	m_capacityOfStandardOut;
	long	m_capacityOfStandardErr;
#ifdef WIN32
	// Since Win32 uses async IO we need to store the stdin info
	// somewhere and this is currently the best place for it.
	// TODO: Hold this in thread local storage!
	long m_bytesOfStandardIn;
	char *m_standardIn;
#endif /*WIN32*/

	void reallocateStandardOut(long len);
	void reallocateStandardErr(long len);

public:
	CapturedData();
	~CapturedData();

	const char *standardOut()	{ return m_standardOut; }
	const char *standardErr()	{ return m_standardErr; }

	long bytesOfStandardOut()	{ return m_bytesOfStandardOut; }
	long bytesOfStandardErr()	{ return m_bytesOfStandardErr; }
#ifdef WIN32
	long bytesOfStandardIn()	{ return m_bytesOfStandardIn; }
#endif /*WIN32*/

	void appendStandardOut(const char *text, long len);
	void appendStandardErr(const char *text, long len);
};

int executeAndCapture(
	class IOutputStream &ostr, class CmdLine &cmd, const char *cwd,
	bool ShowOutput, long threadId, int *ChildExitStatus,
	CapturedData **cd, char **EnvPtr);

#endif /*__execute_h*/
