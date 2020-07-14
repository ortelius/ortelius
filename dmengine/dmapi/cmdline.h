#ifndef __cmdline_h

#define __cmdline_h


#include "dmapiexport.h"


class DMAPI_API CmdLine
{
private:
	char **m_argv;
	int m_argc;
	int m_size;

public:
	CmdLine(const char *cmd = 0);
	~CmdLine();
	CmdLine &add(const char *arg);
	CmdLine &add(const class StringList &args);
	char *toCommandString(bool forpopen = false);
	char **toArgv();
	int exec(class Context &ctx);
	int exec(class CapturedData **cd, class Context &ctx);
};


#endif /*__cmdline_h*/
