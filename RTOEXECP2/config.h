/* RHT 15/02/2010 - Multiplatform support header - selects correct config for platform */

#ifdef WIN32
#include "config-win32.h"
#endif

#ifdef __LINUX__
#include "config-linux.h"
#endif

#ifdef __SUNOS__
#include "config-solaris.h"
#endif
