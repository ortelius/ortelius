#ifndef _VIS_H
#define _VIS_H

char * vis(char *dst, int c, int flag, int nextc);
int strvis(char *dst, const char *src, int flag);
int strnvis(char *dst, const char *src, size_t siz, int flag);
int strvisx(char *dst, const char *src, size_t len, int flag);

#endif
