/*
 * uxstore.c: Unix-specific implementation of the interface defined
 * in storage.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "storage.h"

#include <stdarg.h>

extern "C" {
#include "xmalloc.h"
}


#define snewn(size,type)  (type*) xmalloc(size)
#define sresize(p,size,type)  (type*) xrealloc(p, size)
#define sfree(p)  xfree(p)

char *dupprintf(const char *fmt, ...) {
	char buf[4096];
	va_list args;

	va_start(args,fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	return xstrdup(buf);
}

char *fgetline(FILE *fp)
{
    char *ret = snewn(512, char);
    int size = 512, len = 0;
    while (fgets(ret + len, size - len, fp)) {
	len += strlen(ret + len);
	if (ret[len-1] == '\n')
	    break;		       /* got a newline, we're done */
	size = len + 512;
	ret = sresize(ret, size, char);
    }
    if (len == 0) {		       /* first fgets returned NULL */
	sfree(ret);
	return NULL;
    }
    ret[len] = '\0';
    return ret;
}


enum {
    INDEX_DIR, INDEX_HOSTKEYS, INDEX_HOSTKEYS_TMP, INDEX_RANDSEED,
    INDEX_SESSIONDIR, INDEX_SESSION,
};

static const char hex[17] = "0123456789ABCDEF";

static char *mungestr(const char *in)
{
    char *out, *ret;

    if (!in || !*in)
        in = "Default Settings";

    ret = out = snewn(3*strlen(in)+1, char);

    while (*in) {
        /*
         * There are remarkably few punctuation characters that
         * aren't shell-special in some way or likely to be used as
         * separators in some file format or another! Hence we use
         * opt-in for safe characters rather than opt-out for
         * specific unsafe ones...
         */
	if (*in!='+' && *in!='-' && *in!='.' && *in!='@' && *in!='_' &&
            !(*in >= '0' && *in <= '9') &&
            !(*in >= 'A' && *in <= 'Z') &&
            !(*in >= 'a' && *in <= 'z')) {
	    *out++ = '%';
	    *out++ = hex[((unsigned char) *in) >> 4];
	    *out++ = hex[((unsigned char) *in) & 15];
	} else
	    *out++ = *in;
	in++;
    }
    *out = '\0';
    return ret;
}

static char *unmungestr(const char *in)
{
    char *out, *ret;
    out = ret = snewn(strlen(in)+1, char);
    while (*in) {
	if (*in == '%' && in[1] && in[2]) {
	    int i, j;

	    i = in[1] - '0';
	    i -= (i > 9 ? 7 : 0);
	    j = in[2] - '0';
	    j -= (j > 9 ? 7 : 0);

	    *out++ = (i << 4) + j;
	    in += 3;
	} else {
	    *out++ = *in++;
	}
    }
    *out = '\0';
    return ret;
}

static void make_filename(char *filename, int index, const char *subname)
{
    const char *home;
    int len;
    home = getenv("TRIDENTHOME");
    if (!home)
	home = getenv("HOME");
    if (!home)
        home="/";
    strncpy(filename, home, FILENAME_MAX);
    len = strlen(filename);
    if (index == INDEX_SESSION) {
        char *munged = mungestr(subname);
        char *fn = dupprintf("/.hostkeys/sessions/%s", munged);
        strncpy(filename + len, fn, FILENAME_MAX - len);
        sfree(fn);
        sfree(munged);
    } else {
        strncpy(filename + len,
                index == INDEX_DIR ? "/.hostkeys" :
                index == INDEX_SESSIONDIR ? "/.hostkeys/sessions" :
                index == INDEX_HOSTKEYS ? "/.hostkeys/sshhostkeys" :
                index == INDEX_HOSTKEYS_TMP ? "/.hostkeys/sshhostkeys.tmp" :
                index == INDEX_RANDSEED ? "/.hostkeys/randomseed" :
                "/.hostkeys/ERROR", FILENAME_MAX - len);
    }
    filename[FILENAME_MAX-1] = '\0';
}


/*
 * Lines in the host keys file are of the form
 * 
 *   type@port:hostname keydata
 * 
 * e.g.
 * 
 *   rsa@22:foovax.example.org 0x23,0x293487364395345345....2343
 */
int verify_host_key(const char *hostname, int port,
		    const char *keytype, const char *key)
{
    FILE *fp;
    char filename[FILENAME_MAX];
    char *line;
    int ret;

    make_filename(filename, INDEX_HOSTKEYS, NULL);
    fp = fopen(filename, "r");
    if (!fp)
	return 1;		       /* key does not exist */

    ret = 1;
    while ( (line = fgetline(fp)) ) {
	int i;
	char *p = line;
	char porttext[20];

	line[strcspn(line, "\n")] = '\0';   /* strip trailing newline */

	i = strlen(keytype);
	if (strncmp(p, keytype, i))
	    goto done;
	p += i;

	if (*p != '@')
	    goto done;
	p++;

	sprintf(porttext, "%d", port);
	i = strlen(porttext);
	if (strncmp(p, porttext, i))
	    goto done;
	p += i;

	if (*p != ':')
	    goto done;
	p++;

	i = strlen(hostname);
	if (strncmp(p, hostname, i))
	    goto done;
	p += i;

	if (*p != ' ')
	    goto done;
	p++;

	/*
	 * Found the key. Now just work out whether it's the right
	 * one or not.
	 */
	if (!strcmp(p, key))
	    ret = 0;		       /* key matched OK */
	else
	    ret = 2;		       /* key mismatch */

	done:
	sfree(line);
	if (ret != 1)
	    break;
    }

    fclose(fp);
    return ret;
}


int lookup_host_key(const char *hostname, int port,
		    const char *keytype, char **pKey)
{
    FILE *fp;
    char filename[FILENAME_MAX];
    char *line;
    int ret;

    make_filename(filename, INDEX_HOSTKEYS, NULL);
    fp = fopen(filename, "r");
    if (!fp)
	return 1;		       /* key does not exist */

    ret = 1;
    while ( (line = fgetline(fp)) ) {
	int i;
	char *p = line;
	char porttext[20];

	line[strcspn(line, "\n")] = '\0';   /* strip trailing newline */

	i = strlen(keytype);
	if (strncmp(p, keytype, i))
	    goto done;
	p += i;

	if (*p != '@')
	    goto done;
	p++;

	sprintf(porttext, "%d", port);
	i = strlen(porttext);
	if (strncmp(p, porttext, i))
	    goto done;
	p += i;

	if (*p != ':')
	    goto done;
	p++;

	i = strlen(hostname);
	if (strncmp(p, hostname, i))
	    goto done;
	p += i;

	if (*p != ' ')
	    goto done;
	p++;

        ret = 2;		       /* key found */
        *pKey = xstrdup(p);

	done:
	sfree(line);
	if (ret != 1)
	    break;
    }

    fclose(fp);
    return ret;
}


void store_host_key(const char *hostname, int port,
		    const char *keytype, const char *key)
{
    FILE *rfp, *wfp;
    char *newtext, *line;
    int headerlen;
    char filename[FILENAME_MAX], tmpfilename[FILENAME_MAX];

    newtext = dupprintf("%s@%d:%s %s\n", keytype, port, hostname, key);
    headerlen = 1 + strcspn(newtext, " ");   /* count the space too */

    /*
     * Open both the old file and a new file.
     */
    make_filename(tmpfilename, INDEX_HOSTKEYS_TMP, NULL);
    wfp = fopen(tmpfilename, "w");
    if (!wfp) {
        char dir[FILENAME_MAX];

        make_filename(dir, INDEX_DIR, NULL);
        mkdir(dir, 0700);
        wfp = fopen(tmpfilename, "w");
    }
    if (!wfp)
	return;
    make_filename(filename, INDEX_HOSTKEYS, NULL);
    rfp = fopen(filename, "r");

    /*
     * Copy all lines from the old file to the new one that _don't_
     * involve the same host key identifier as the one we're adding.
     */
    if (rfp) {
        while ( (line = fgetline(rfp)) ) {
            if (strncmp(line, newtext, headerlen))
                fputs(line, wfp);
        }
        fclose(rfp);
    }

    /*
     * Now add the new line at the end.
     */
    fputs(newtext, wfp);

    fclose(wfp);

    rename(tmpfilename, filename);

    sfree(newtext);
}
