/* RHT 15/02/2010 - Multiplatform support header - selects correct opensslconf for platform */

#undef __opensslconf_include_done

#ifdef WIN32
#ifdef _WIN64
#include "opensslconf-win64.h"
#else
#include "opensslconf-win32.h"
#endif /*_WIN64*/
#define __opensslconf_include_done
#endif /*WIN32*/

#ifdef __LINUX__
#include "opensslconf-linux.h"
#define __opensslconf_include_done
#endif /*__LINUX__*/

#ifdef __SUNOS__
#include "opensslconf-solaris.h"
#define __opensslconf_include_done
#endif /*__SUNOS__*/

#ifndef __opensslconf_include_done
#error No suitable opensslconf.h could be found
#endif
