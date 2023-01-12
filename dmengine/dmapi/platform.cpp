// Copyright (c) 2021 Linux Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "platform.h"
#ifdef WIN32
#include <errno.h>
#include <windows.h>

#else
#define GetCurrentThreadId pthread_self
#include <pthread.h>
#endif

#ifdef WIN32
// Version of vasprintf for Windows

int vasprintf(char **strp, const char *fmt, va_list ap)
{
  int r = -1, size = _vscprintf(fmt, ap);
  if ((size >= 0) && (size < INT_MAX))
  {
    *strp = (char *)malloc(size+1); //+1 for null
    if (*strp)
    {
      r = vsnprintf(*strp, size+1, fmt, ap);  //+1 for null
      if (r == -1) { SAFE_FREE(*strp); }
    }
  }
  else { *strp = 0; }
  return(r);
}
#endif

static int s_debuglevel = 0;


int debuglevel()
{
	return s_debuglevel;
}


void setdebuglevel(int level)
{
	s_debuglevel = level;
}


void debug0(const char *fmt, ...)
{
	char *msg = NULL;
	va_list args;
	va_start(args, fmt);
	vasprintf(&msg,fmt, args);
	printf("debug1: (0x%lx) %s\n", GetCurrentThreadId(),msg);
	free(msg);
	va_end(args);
}


void debug1(const char *fmt, ...)
{
	char *msg = NULL;
	if(s_debuglevel >= 1) {
		va_list args;
		va_start(args, fmt);
		vasprintf(&msg,fmt, args);
		printf("DEBUG1: (0x%lx) %s\n", GetCurrentThreadId(),msg);
		free(msg);
		va_end(args);
	}
}


void debug2(const char *fmt, ...)
{
	char *msg = NULL;
	if(s_debuglevel >= 2) {
		va_list args;
		va_start(args, fmt);
		vasprintf(&msg,fmt, args);
		printf("DEBUG2: (0x%lx) %s\n", GetCurrentThreadId(),msg);
		free(msg);
		va_end(args);
	}
}


void debug3(const char *fmt, ...)
{
	char *msg = NULL;
	if(s_debuglevel >= 3) {
		va_list args;
		va_start(args, fmt);
		vasprintf(&msg,fmt, args);
		printf("DEBUG3: (0x%lx) %s\n", GetCurrentThreadId(),msg);
		free(msg);
		va_end(args);
	}
}


void catFile(const char *filename)
{
	if(s_debuglevel >= 1) {
		printf("file: %s\n----- 8< -----\n", filename);
		FILE *f = fopen(filename, "r");
		if(f) {
			char buf[1024];
			int n;
			while(n = fread(buf, 1, sizeof(buf)-1, f)) {
				buf[n] = '\0';
				printf("%s", buf);
			}
			fclose(f);
		}
		printf("----- 8< -----\n");
	}
}


void catFile1(const char *filename)
{
	if(s_debuglevel >= 1) {
		catFile(filename);
	}
}


void catFile2(const char *filename)
{
	if(s_debuglevel >= 2) {
		catFile(filename);
	}
}


void catFile3(const char *filename)
{
	if(s_debuglevel >= 3) {
		catFile(filename);
	}
}


void dumpbuffer(const char *buf, int len)
{
	for(int x = 0; x < ((len/16)+((len%16) ? 1 : 0)); x++) {
		printf("%02x: ", x);
		int y;
		for(y = 0; y < 16; y++) {
			if((x*16 + y) >= len) { break; }
			int v = (unsigned char) buf[x*16 + y];
			printf("%02x ", v);
		}
		for(; y < 16; y++) {
			printf("   ");
		}
		for(y = 0; y < 16; y++) {
			if((x*16 + y) >= len) { break; }
			int v = buf[x*16 + y];
			printf("%c", (((v >= 32) && (v <= 127)) ? v : '.'));
		}
		printf("\n");
	}
}
