// Functions for handling host keys in the Windows registry
// RHT 25/02/2011 - converted all memory allocations to xmalloc/xstrdup/xfree etc.

// Uncomment these to use debug()
extern "C" {
#include "includes.h"
//#include "log.h"
#include "xmalloc.h"
}

//#include "memory.h"
//#include "logging.h"

#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <winreg.h>

#define PUTTY_REG_POS "Software\\Openmake\\DeploymentManager"
static const char *const puttystr = PUTTY_REG_POS "\\Sessions";

static const char hex[17] = "0123456789ABCDEF";

static void mungestr(
				const char *in,
				char *out )
{
    int candot = 0;

    while( *in )
	{
		if( (*in == ' ') || (*in == '\\') || (*in == '*') || (*in == '?') ||
			(*in == '%') || (*in < ' ') || (*in > '~') || ((*in == '.') && !candot) )
		{
			*out++ = '%';
			*out++ = hex[((unsigned char) *in) >> 4];
			*out++ = hex[((unsigned char) *in) & 15];
		}
		else
			*out++ = *in;
		in++;
		candot = 1;
    }
    *out = '\0';
    return;
}


static void hostkey_regname(
				char *buffer,
				const char *hostname,
			    int port,
				const char *keytype )
{
    int len;
    strcpy( buffer, keytype );
    strcat( buffer, "@" );
    len = strlen(buffer);
    len += sprintf( buffer + len, "%d:", port );
    mungestr( hostname, buffer + strlen(buffer) );
}


int verify_host_key(
				const char *hostname,
				int port,
				const char *keytype,
				const char *key )
{
    char *otherstr, *regname;
    int len;
    HKEY rkey;
    DWORD readlen;
    DWORD type;
    int ret, compare;

//	TRIDEBUG(("verify_host_key( \"%s\", %d, \"%s\", \"%s\" )", hostname, port, keytype, key));

    len = 1 + strlen(key);

    /*
     * Now read a saved key in from the registry and see what it
     * says.
     */
    otherstr = (char*) xmalloc(len);
	otherstr[0] = '\0';		// RHT 18/08/2008 - prevent random debug when query fails
    regname  = (char*) xmalloc(3 * (strlen(hostname) + strlen(keytype)) + 15);

    hostkey_regname( regname, hostname, port, keytype );

//	TRIDEBUG(("hostkey_regname returned \"%s\"", regname));

    if( RegOpenKey(HKEY_CURRENT_USER, PUTTY_REG_POS "\\SshHostKeys", &rkey) != ERROR_SUCCESS )
		return 1;		       /* key does not exist in registry */

    readlen = len;
    ret = RegQueryValueEx( rkey, regname, NULL, &type, (unsigned char*) otherstr, &readlen );

//	TRIDEBUG(("otherstr = \"%s\"", otherstr));

    if( (ret != ERROR_SUCCESS) && (ret != ERROR_MORE_DATA) && !strcmp(keytype, "rsa") )
	{
		/*
		 * Key didn't exist. If the key type is RSA, we'll try
		 * another trick, which is to look up the _old_ key format
		 * under just the hostname and translate that.
		 */
		char *justhost = regname + 1 + strcspn(regname, ":");
		char *oldstyle = (char*) xmalloc(len + 10);	/* safety margin */
		readlen = len;
		ret = RegQueryValueEx( rkey, justhost, NULL, &type, (unsigned char*) oldstyle, &readlen);

		if( (ret == ERROR_SUCCESS) && (type == REG_SZ))
		{
			/*
			 * The old format is two old-style bignums separated by
			 * a slash. An old-style bignum is made of groups of
			 * four hex digits: digits are ordered in sensible
			 * (most to least significant) order within each group,
			 * but groups are ordered in silly (least to most)
			 * order within the bignum. The new format is two
			 * ordinary C-format hex numbers (0xABCDEFG...XYZ, with
			 * A nonzero except in the special case 0x0, which
			 * doesn't appear anyway in RSA keys) separated by a
			 * comma. All hex digits are lowercase in both formats.
			 */
			char *p = otherstr;
			char *q = oldstyle;
			int i, j;

			for( i = 0; i < 2; i++ )
			{
				int ndigits, nwords;
				*p++ = '0';
				*p++ = 'x';
				ndigits = strcspn(q, "/");	/* find / or end of string */
				nwords = ndigits / 4;
				/* now trim ndigits to remove leading zeros */
				while( (q[(ndigits - 1) ^ 3] == '0') && (ndigits > 1) )
					ndigits--;
				/* now move digits over to new string */
				for (j = 0; j < ndigits; j++)
					p[ndigits - 1 - j] = q[j ^ 3];
				p += ndigits;
				q += nwords * 4;
				if( *q )
				{
					q++;	       /* eat the slash */
					*p++ = ',';	       /* add a comma */
				}
				*p = '\0';	       /* terminate the string */
			}

			/*
			 * Now _if_ this key matches, we'll enter it in the new
			 * format. If not, we'll assume something odd went
			 * wrong, and hyper-cautiously do nothing.
			 */
			if( !strcmp(otherstr, key) )
				RegSetValueEx( rkey, regname, 0, REG_SZ, (const unsigned char*) otherstr, strlen(otherstr) + 1 );
		}
    }

    RegCloseKey(rkey);

    compare = strcmp(otherstr, key);

    xfree(otherstr);
    xfree(regname);

    if( (ret == ERROR_MORE_DATA)
		|| ((ret == ERROR_SUCCESS) && (type == REG_SZ && compare)))
		return 2;		       /* key is different in registry */
    else if (ret != ERROR_SUCCESS || type != REG_SZ)
		return 1;		       /* key does not exist in registry */
    else
		return 0;		       /* key matched OK in registry */
}


int lookup_host_key(
				const char *hostname,
				int port,
				const char *keytype,
				char **pkey )
{
    char *otherstr, *regname;
    int len;
    HKEY rkey;
    DWORD readlen;
    DWORD type;
    int ret; //, compare;

//	TRIDEBUG(("lookup_host_key( \"%s\", %d, \"%s\" )", hostname, port, keytype));

    len = 1 + 2048;

    /*
     * Now read a saved key in from the registry and see what it
     * says.
     */
    otherstr = (char*) xmalloc(len);
	otherstr[0] = '\0';		// RHT 18/08/2008 - prevent random debug when query fails
    regname  = (char*) xmalloc(3 * (strlen(hostname) + strlen(keytype)) + 15);

    hostkey_regname( regname, hostname, port, keytype );

//	TRIDEBUG(("hostkey_regname returned \"%s\"", regname));

    if( RegOpenKey(HKEY_CURRENT_USER, PUTTY_REG_POS "\\SshHostKeys", &rkey) != ERROR_SUCCESS )
		return 1;		       /* key does not exist in registry */

    readlen = len;
    ret = RegQueryValueEx( rkey, regname, NULL, &type, (unsigned char*) otherstr, &readlen );

//	TRIDEBUG(("otherstr = \"%s\"", otherstr));

    if( (ret != ERROR_SUCCESS) && (ret != ERROR_MORE_DATA) && !strcmp(keytype, "rsa") )
	{
		/*
		 * Key didn't exist. If the key type is RSA, we'll try
		 * another trick, which is to look up the _old_ key format
		 * under just the hostname and translate that.
		 */
		char *justhost = regname + 1 + strcspn(regname, ":");
		char *oldstyle = (char*) xmalloc(len + 10);	/* safety margin */
		readlen = len;
		ret = RegQueryValueEx( rkey, justhost, NULL, &type, (unsigned char*) oldstyle, &readlen);

		if( (ret == ERROR_SUCCESS) && (type == REG_SZ))
		{
			/*
			 * The old format is two old-style bignums separated by
			 * a slash. An old-style bignum is made of groups of
			 * four hex digits: digits are ordered in sensible
			 * (most to least significant) order within each group,
			 * but groups are ordered in silly (least to most)
			 * order within the bignum. The new format is two
			 * ordinary C-format hex numbers (0xABCDEFG...XYZ, with
			 * A nonzero except in the special case 0x0, which
			 * doesn't appear anyway in RSA keys) separated by a
			 * comma. All hex digits are lowercase in both formats.
			 */
			char *p = otherstr;
			char *q = oldstyle;
			int i, j;

			for( i = 0; i < 2; i++ )
			{
				int ndigits, nwords;
				*p++ = '0';
				*p++ = 'x';
				ndigits = strcspn(q, "/");	/* find / or end of string */
				nwords = ndigits / 4;
				/* now trim ndigits to remove leading zeros */
				while( (q[(ndigits - 1) ^ 3] == '0') && (ndigits > 1) )
					ndigits--;
				/* now move digits over to new string */
				for (j = 0; j < ndigits; j++)
					p[ndigits - 1 - j] = q[j ^ 3];
				p += ndigits;
				q += nwords * 4;
				if( *q )
				{
					q++;	       /* eat the slash */
					*p++ = ',';	       /* add a comma */
				}
				*p = '\0';	       /* terminate the string */
			}

			/*
			 * Now _if_ this key matches, we'll enter it in the new
			 * format. If not, we'll assume something odd went
			 * wrong, and hyper-cautiously do nothing.
			 */
			//if( !strcmp(otherstr, key) )
			//	RegSetValueEx( rkey, regname, 0, REG_SZ, (const unsigned char*) otherstr, strlen(otherstr) + 1 );
		}
    }

    RegCloseKey(rkey);

    //compare = strcmp(otherstr, key);

    //free(otherstr);	RHT 25/02/2011 - er, no
    xfree(regname);

    if( (ret == ERROR_MORE_DATA)
		|| ((ret == ERROR_SUCCESS) && (type == REG_SZ /*&& compare*/)))
	{
		*pkey = otherstr;
		return 2;		       /* key is different in registry */
	}
    else if (ret != ERROR_SUCCESS || type != REG_SZ)
	{
		xfree(otherstr);			// RHT 25/02/2011 - this is the only way out that should free otherstr
		return 1;		       /* key does not exist in registry */
	}
    else
	{
		*pkey = otherstr;
		return 0;		       /* key matched OK in registry */
	}
}


void store_host_key(
				const char *hostnameList,
				int port,
				const char *keytype,
				const char *key )
{
    char *regname;
    HKEY rkey;

	// RHT 18/06/2008 - Changed to store comma delimited hostnames in separate values
	//   Typically get passed "sun,192.168.0.4" and this needs to be recorded at two
	//   values as the checking code will look for BOTH dsa2@22:sun2 and dsa2@22:192..
	char *buf = xstrdup(hostnameList);

	for( char *hostname = strtok(buf, ","); hostname ; hostname = strtok(NULL, ",") )
	{
		regname = (char*) xmalloc(3 * (strlen(hostname) + strlen(keytype)) + 15);

		hostkey_regname( regname, hostname, port, keytype );

		if( RegCreateKey(HKEY_CURRENT_USER, PUTTY_REG_POS "\\SshHostKeys", &rkey) != ERROR_SUCCESS) {
			/* key does not exist in registry */
			// RHT 25/02/2011 - free and break instead of return
			xfree(regname);			
			break;		
		}
		RegSetValueEx( rkey, regname, 0, REG_SZ, (const unsigned char*) key, strlen(key) + 1 );
		RegCloseKey(rkey);

		xfree(regname);	// RHT 25/02/2011 - safe to free this
	}

	xfree(buf);
}
