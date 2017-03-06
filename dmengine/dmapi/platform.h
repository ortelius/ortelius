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
#ifndef __platform_h

#define __platform_h

#ifdef WIN32
#include <stdarg.h>
// Windows
#undef BIG_ENDIAN

#define DIR_SEP_CHAR '\\'
#define DIR_SEP_STR  "\\"
#define PATH_SEP_STR  ";"
#define EXE_EXT  ".exe"

#define snprintf		_snprintf
// #define vsnprintf		_vsnprintf
#define STRCASECMP		_stricmp
#define STRNCASECMP		_strnicmp
#define STRTOK_R		strtok_s

#define popen			_popen
#define pclose			_pclose

#define DMINT32			long
#define ATOINT32(x)		atol(x)

#define CLOSESOCKET(x)	closesocket(x)
#define SOCKLEN int
//#define	SSIZE_T int - defined in ???.h

#define FILE_WRITE_PERMISSIONS _S_IREAD | _S_IWRITE
#define FILE_CREATE_PERMISSIONS _O_CREAT | _O_BINARY | _O_TRUNC | _O_WRONLY
#define FILE_READ_PERMISSIONS _O_RDONLY | _O_BINARY
#define STAT_IS_DIRECTORY _S_IFDIR
#define FILE_CREATE_MODE _S_IREAD | _S_IWRITE

#define CHMOD(file,mode) _chmod(file, mode)
#define MKDIR(dirname) _mkdir(dirname)
#define RMDIR(dirname) _rmdir(dirname)

#define THREAD_ID HANDLE
#define THREAD_RETURN_TYPE DWORD
#define THREAD_RETURN_TYPE_DECL THREAD_RETURN_TYPE WINAPI

// Stupid POSIX warnings
#define strdup _strdup

// Features
#define HAVE_ODBC

// These functions do not exist on Windows
#define va_copy(dest, src) (dest = src)
int vasprintf(char **strp, const char *fmt, va_list ap);
int asprintf(char **strp, const char *fmt, ...);

// End Windows

#else

// Unix

#if defined(__LINUX__) || defined(__linux__)
#undef BIG_ENDIAN
#endif

#if defined(__SUNOS__) || defined(__HPUX__)
#define BIG_ENDIAN
#endif

#define DIR_SEP_CHAR '/'
#define DIR_SEP_STR  "/"
#define PATH_SEP_STR  ":"
#define EXE_EXT  ""

#define STRCASECMP			strcasecmp
#define STRNCASECMP			strncasecmp
#define STRTOK_R			strtok_r

#define DMINT32				long
#define ATOINT32(x)			atol(x)

#define CloseHandle(h)      close(h)
#define EnterCriticalSection(x)
#define LeaveCriticalSection(x)

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define CLOSESOCKET(x)	close(x)

#ifdef __HPUX__
#define SOCKLEN int
#else
#define SOCKLEN socklen_t
#endif /*__HPUX__*/

#define	SSIZE_T ssize_t

#define FILE_WRITE_PERMISSIONS 0777
#define FILE_CREATE_PERMISSIONS O_CREAT | O_TRUNC  | O_WRONLY | S_IRUSR | S_IWUSR | S_IRGRP
#define FILE_READ_PERMISSIONS O_RDONLY
#define STAT_IS_DIRECTORY S_IFDIR
#define FILE_CREATE_MODE S_IRWXU

#define CHMOD(file,mode) chmod(file, mode)
#define MKDIR(dirname) ::mkdir(dirname,0777)
#define RMDIR(dirname) rmdir(dirname)

#define THREAD_ID pthread_t
#define THREAD_RETURN_TYPE void*
#define THREAD_RETURN_TYPE_DECL extern "C" THREAD_RETURN_TYPE

// Features
#ifdef HAVE_UNIXODBC
#define HAVE_ODBC
#endif /*HAVE_UNIXODBC*/

// End Unix

#endif /*WIN32*/

// Common definitions
#define SAFE_FREE(x)	if(x) { ::free(x); x = NULL; }
#define SECURE_FREE(x)	if(x) { memset(x, 0, strlen(x)); ::free(x); x = NULL; }
#define SAFE_DELETE(x)	if(x) { delete x; x = NULL; }
#define DUP_NULL(x)		((x) ? strdup(x) : NULL)
#define NULL_CHECK(x)	((x) ? (x) : "(null)")

// DB access common definitions
#define IS_NULL(x)  (ni_ ## x == -1)
#define NULL_IND(x,y)  ((ni_ ## x != -1) ? x : y)
#define BOOL_NULL_IND(x,y)  ((ni_ ## x != -1) ? ((x[0] == 'Y') ? true : false) : y)
#define STR_NULL_IND(x,y)  (((ni_ ## x != -1) && (x[0])) ? x : y)


// Turn DM Exceptions on and off for crypto
#ifdef SETODBC
#undef HAVE_DMEXCEPTIONS
#undef HAVE_DMAPIEXPORT
#else
#define HAVE_DMEXCEPTIONS 1
#define HAVE_DMAPIEXPORT 1
#endif


#include "dmapiexport.h"

// Debug output
int debuglevel();

// TODO: Add macros to compile these out
void DMAPI_API debug1(const char *fmt, ...);
void DMAPI_API debug1(const char *fmt, ...);
void DMAPI_API debug2(const char *fmt, ...);
void DMAPI_API debug3(const char *fmt, ...);

void DMAPI_API catFile1(const char* filename);
void DMAPI_API catFile2(const char* filename);
void DMAPI_API catFile3(const char* filename);

void DMAPI_API dumpbuffer(const char *buf, int len);

#endif /*__platform_h*/
