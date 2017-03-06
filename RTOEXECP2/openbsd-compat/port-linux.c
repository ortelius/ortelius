#include "includes.h"
#include "xmalloc.h"

/*RHT extern "C"*/ size_t strlcpy(char *s1, const char *s2, size_t len)
{
  size_t ret = snprintf(s1,len,"%s",s2);
  /*debug3( "strlcpy(\"%s\",\"%s\",%d) returning %d", s1, s2, len, ret );*/
  return ret;
}

/*RHT extern "C"*/ size_t strlcat(char *s1, const char *s2, size_t len)
{
  char *s3 = strdup(s1);
  size_t ret = snprintf(s1,len,"%s%s",s3,s2);
  /*debug3( "strlcat(\"%s\",\"%s\",%d) returning %d, \"%s\"", s3, s2, len, ret, s1 );*/
  free(s3);
  return ret;
}

