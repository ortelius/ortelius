/* RHT 15/02/2010 - Multiplatform support header - selects correct opensslconf for platform */

#ifdef WIN32
#include "opensslconf-win32.h"
#endif

#ifdef __LINUX__
#include "opensslconf-linux.h"
#endif

#ifdef __SUNOS__
#include "opensslconf-solaris.h"
#endif
