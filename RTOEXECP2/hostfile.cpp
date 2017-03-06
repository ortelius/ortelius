/*
 * Author: Tatu Ylonen <ylo@cs.hut.fi>
 * Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
 *                    All rights reserved
 * Functions for manipulating the known hosts files.
 *
 * As far as I am concerned, the code I have written for this software
 * can be used freely for any purpose.  Any derived versions of this
 * software must be clearly marked as such, and if the derived work is
 * incompatible with the protocol description in the RFC file, it must be
 * called by a name other than "ssh" or "Secure Shell".
 *
 *
 * Copyright (c) 1999, 2000 Markus Friedl.  All rights reserved.
 * Copyright (c) 1999 Niels Provos.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

extern "C" { /*RHT*/

#include "includes.h"
RCSID("$OpenBSD: hostfile.c,v 1.36 2005/11/22 03:36:03 dtucker Exp $");

#ifndef WIN32
#include <resolv.h>
#endif /*WIN32*/
#include <openssl/hmac.h>
#include <openssl/sha.h>

#include "packet.h"
#include "match.h"
#include "key.h"
#include "hostfile.h"
#include "log.h"
#include "xmalloc.h"

#include "bufaux.h"

#include "uuencode.h" /*RHT*/

} /* extern "C" RHT */


// ============================================================================
// MOVED FROM triserv.cpp - MEANS WE DON'T NEED TALK TO OUR SERVER ANY MORE
// - RHT 15/02/2011
// ============================================================================
#include "storage.h"

static const char* keytypeToString( int keytype )
{
    switch( keytype )
    {
    case KEY_RSA1: return "rsa1";
    case KEY_RSA:  return "rsa2";
    case KEY_DSA:  return "dsa";
    default:       return "unknown";
    }
}


/**
 * Line format is:
 * keytype@port:host 0x12,0x1234567890abcdef
 * Added RHT - 22/06/2007; XDR removed 17/02/2011
 */
HostStatus SFTP_CheckHostInHostfile(const char *dummyFilename, const char *host, int keytype, const char *key, char **found, int *numret)
{
    debug3("SFTP_CheckHostInHostfile( \"%s\", \"%s\" )", "sshhostkeys" /*dummyFilename*/, host);
    
    *found = NULL;
    *numret = 0;

    HostStatus status = HOST_NEW;

	const char *keytypestr = keytypeToString(keytype);

	int ret = verify_host_key( host, 22, keytypestr, key );
	debug("ret(%s) = %d", keytypestr, ret);

	if(ret == 0) {
		status = HOST_OK;
		*numret = 1;
		*found  = strdup( key );
	} else if(ret == 2) {
		status = HOST_CHANGED;
		*numret = 1;
		*found  = strdup( "changed value" );
	}
 	
    debug3("SFTP_CheckHostInHostfile - Exit - rc: %d", status);
	return status;
}	


/* Added RHT - 22/06/2007; XDR removed 17/02/2011 */
int SFTP_LookupKeyInHostfileByType(const char *dummyFilename, const char *host, int keytype, char **found, int *numret)
{
	debug3("SFTP_LookupKeyInHostfileByType( \"%s\", \"%s\", %d, ... )", "sshhostkeys" /*dummyFilename*/, host, keytype);
        
    *found = NULL;    
    *numret = 0;

	const char *keytypestr = keytypeToString( keytype );
	int ret = (lookup_host_key( host, 22, keytypestr, found ) == 2);
	if(ret) {
		*numret = 1;
	}
                   	
    debug3("SFTP_LookupKeyInHostfileByType - Exit - rc: %d", ret);
	return ret;
}	

/**
 * SFTP_AddKnownHostToServer
 *
 * Request:
 *   string host
 *   long   port
 *   long   keytype
 *   string key
 *
 * Reply:
 *   enum   status
 *
 * Added RHT - 12/06/2007
 * Protcol changed RHT - 07/09/2007; XDR removed 17/02/2011
 */
int SFTP_AddKnownHostToServer(const char *host, long  port, long  keytype, char *key)
{
	debug3("SFTP_AddKnownHostToServer( \"%s\", %d, %d, \"%s\" )", host, port, keytype, key);

	const char *keytypestr = keytypeToString( keytype );

	store_host_key( host, port, keytypestr, key );
	
    debug3("SFTP_AddKnownHostToServer - Exit");
	return 1;
}
// ============================================================================
// END MOVED FROM triserv.cpp
// ============================================================================

static int
extract_salt(const char *s, u_int l, char *salt, size_t salt_len)
{
	char *p, *b64salt;
	u_int b64len;
	int ret;

	if (l < sizeof(HASH_MAGIC) - 1) {
		debug2("extract_salt: string too short");
		return (-1);
	}
	if (strncmp(s, HASH_MAGIC, sizeof(HASH_MAGIC) - 1) != 0) {
		debug2("extract_salt: invalid magic identifier");
		return (-1);
	}
	s += sizeof(HASH_MAGIC) - 1;
	l -= sizeof(HASH_MAGIC) - 1;
	if ((p = (char *)memchr(s, HASH_DELIM, l)) == NULL) {
		debug2("extract_salt: missing salt termination character");
		return (-1);
	}

	b64len = p - s;
	/* Sanity check */
	if (b64len == 0 || b64len > 1024) {
		debug2("extract_salt: bad encoded salt length %u", b64len);
		return (-1);
	}
	b64salt = (char *)xmalloc(1 + b64len);
	memcpy(b64salt, s, b64len);
	b64salt[b64len] = '\0';

	ret = __b64_pton(b64salt, (u_char *)salt, salt_len);
	xfree(b64salt);
	if (ret == -1) {
		debug2("extract_salt: salt decode error");
		return (-1);
	}
	if (ret != SHA_DIGEST_LENGTH) {
		debug2("extract_salt: expected salt len %d, got %d",
		    SHA_DIGEST_LENGTH, ret);
		return (-1);
	}

	return (0);
}

char *
host_hash(const char *host, const char *name_from_hostfile, u_int src_len)
{
	const EVP_MD *md = EVP_sha1();
	HMAC_CTX mac_ctx;
	char salt[256], result[256], uu_salt[512], uu_result[512];
	static char encoded[1024];
	u_int i, len;

	len = EVP_MD_size(md);

	if (name_from_hostfile == NULL) {
		/* Create new salt */
		for (i = 0; i < len; i++)
			salt[i] = arc4random();
	} else {
		/* Extract salt from known host entry */
		if (extract_salt(name_from_hostfile, src_len, salt,
		    sizeof(salt)) == -1)
			return (NULL);
	}

	HMAC_Init(&mac_ctx, salt, len, md);
	HMAC_Update(&mac_ctx, (u_char *)host, strlen(host));
	HMAC_Final(&mac_ctx, (u_char *)result, NULL);
	HMAC_cleanup(&mac_ctx);

	if (__b64_ntop((u_char *)salt, len, uu_salt, sizeof(uu_salt)) == -1 ||
	    __b64_ntop((u_char *)result, len, uu_result, sizeof(uu_result)) == -1)
		fatal("host_hash: __b64_ntop failed");

	snprintf(encoded, sizeof(encoded), "%s%s%c%s", HASH_MAGIC, uu_salt,
	    HASH_DELIM, uu_result);

	return (encoded);
}

/*
 * Parses an RSA (number of bits, e, n) or DSA key from a string.  Moves the
 * pointer over the key.  Skips any whitespace at the beginning and at end.
 */

int
hostfile_read_key(char **cpp, u_int *bitsp, Key *ret)
{
	char *cp;

	/* Skip leading whitespace. */
	for (cp = *cpp; *cp == ' ' || *cp == '\t'; cp++)
		;

	if (key_read(ret, &cp) != 1)
		return 0;

	/* Skip trailing whitespace. */
	for (; *cp == ' ' || *cp == '\t'; cp++)
		;

	/* Return results. */
	*cpp = cp;
	*bitsp = key_size(ret);
	return 1;
}

static int
hostfile_check_key(int bits, const Key *key, const char *host, const char *filename, int linenum)
{
	if (key == NULL || key->type != KEY_RSA1 || key->rsa == NULL)
		return 1;
	if (bits != BN_num_bits(key->rsa->n)) {
		logit("Warning: %s, line %d: keysize mismatch for host %s: "
		    "actual %d vs. announced %d.",
		    filename, linenum, host, BN_num_bits(key->rsa->n), bits);
		logit("Warning: replace %d with %d in %s, line %d.",
		    bits, BN_num_bits(key->rsa->n), filename, linenum);
	}
	return 1;
}

/*
 * Checks whether the given host (which must be in all lowercase) is already
 * in the list of our known hosts. Returns HOST_OK if the host is known and
 * has the specified key, HOST_NEW if the host is not known, and HOST_CHANGED
 * if the host is known but used to have a different host key.
 *
 * If no 'key' has been specified and a key of type 'keytype' is known
 * for the specified host, then HOST_FOUND is returned.
 */

char *DumpBufferInHex(Buffer *buffer)
{
	u_int len,stringlen;
	u_char *value;
	u_int k;
	int lz=1;
	char *s/*RHT,*fmtstr*/;
	int n;
	const char *hex="0123456789abcdef";
	short n1,n2;

	buffer_get_int_ret(&len, buffer);
	stringlen = (len*2)+10;	/* each byte is 2 chars long + "0x" + space for nulls and whatnot */
	s = (char *)xmalloc(stringlen);
	s[0]='0';
	s[1]='x';
	s[2]='\0';
	n=2;
	value = (u_char *)xmalloc(len + 1);
	buffer_get_ret(buffer,value,len);
	for (k=0;k<len;k++)
	{
		n1=(value[k] & 0xf0) >> 4;
		n2=(value[k] & 0x0f);
		if (lz)
		{
			if (n1) s[n++]=hex[n1];
			if (n2) s[n++]=hex[n2];
			if (n1 || n2) lz=0;
		}
		else
		{
			s[n++]=hex[n1];
			s[n++]=hex[n2];
		}
		s[n]='\0';
	}
	free(value);
	return s;
}

static char *dss_fmtkey(const Key *key)
{
	/*RHT char *p;*/
	char *ps,*pq,*pg,*ppub,*retstr;
	int len /*RHT, i, pos, nibbles*/;
	static const char hex[] = "0123456789abcdef";
	if (!key->dsa->p)
	return NULL;
	len = 8 + 4 + 1;		       /* 4 x "0x", punctuation, \0 */

	Buffer bufferp,bufferq,bufferg,bufferpub;

	buffer_init(&bufferp);
	buffer_init(&bufferq);
	buffer_init(&bufferg);
	buffer_init(&bufferpub);

	buffer_put_bignum2_ret(&bufferp,key->dsa->p);
	buffer_put_bignum2_ret(&bufferq,key->dsa->q);
	buffer_put_bignum2_ret(&bufferg,key->dsa->g);
	buffer_put_bignum2_ret(&bufferpub,key->dsa->pub_key);

	ps=DumpBufferInHex(&bufferp);
	pq=DumpBufferInHex(&bufferq);
	pg=DumpBufferInHex(&bufferg);
	ppub=DumpBufferInHex(&bufferpub);

	retstr=(char *)malloc(strlen(ps)+strlen(pq)+strlen(pg)+strlen(ppub)+5);

	sprintf(retstr,"%s,%s,%s,%s",ps,pq,pg,ppub);
	free(ps);
	free(pq);
	free(pg);
	free(ppub);

	buffer_free(&bufferp);
	buffer_free(&bufferq);
	buffer_free(&bufferg);
	buffer_free(&bufferpub);
	return retstr;
}

/* RHT added 30/07/2007 */
static void dss_unfmtkey( Key* key, const char *hex )
{
    char *cp, *cp2, *hex2;

    key->type  = KEY_DSA;
    key->flags = 0;
    key->dsa   = DSA_new();
    key->rsa   = NULL;

    cp = cp2 = hex2 = xstrdup( hex );

    // Skip leading "0x" on p
    if( (cp[0] == '0') && (cp[1] == 'x') )
        cp += 2;

    // Find the comma after p
    for( cp2 = cp; *cp2 && (*cp2 != ','); cp2++ ) ;

    // Terminate string on the comma after p
    if( *cp2 == ',' )
        *cp2++ = '\0';

    int valp = BN_hex2bn( &key->dsa->p, cp );

    cp = cp2;

    // Skip leading "0x" on q
    if( (cp[0] == '0') && (cp[1] == 'x') )
        cp += 2;

    // Find the comma after q
    for( cp2 = cp; *cp2 && (*cp2 != ','); cp2++ ) ;

    // Terminate string on the comma after q
    if( *cp2 == ',' )
        *cp2++ = '\0';

    int valq = BN_hex2bn( &key->dsa->q, cp );

    cp = cp2;

    // Skip leading "0x" on g
    if( (cp[0] == '0') && (cp[1] == 'x') )
        cp += 2;

    // Find the comma after g
    for( cp2 = cp; *cp2 && (*cp2 != ','); cp2++ ) ;

    // Terminate string on the comma after g
    if( *cp2 == ',' )
        *cp2++ = '\0';

    int valg = BN_hex2bn( &key->dsa->g, cp );

    // Skip leading "0x" on pub_key
    if( (cp2[0] == '0') && (cp2[1] == 'x') )
        cp2 += 2;

    int valpub = BN_hex2bn( &key->dsa->pub_key, cp2 );

    xfree( hex2 );
}

static char *rsa_fmtkey(const Key *key)
{
	/* size of modulus 'n' */
	int bits;
	bits = BN_num_bits(key->rsa->n);

	Buffer buffere,buffern;
	char *pe,*pn,*retstr;

	buffer_init(&buffere);
	buffer_init(&buffern);

	buffer_put_bignum2_ret(&buffere,key->rsa->e);
	buffer_put_bignum2_ret(&buffern,key->rsa->n);

	pe=DumpBufferInHex(&buffere);
	pn=DumpBufferInHex(&buffern);

	retstr=(char *)malloc(strlen(pe)+strlen(pn)+10);

	sprintf(retstr,"%s,%s",pe,pn);
	free(pe);
	free(pn);

	buffer_free(&buffere);
	buffer_free(&buffern);

	return retstr;
}

/* RHT added 30/07/2007 */
static void rsa_unfmtkey( Key* key, const char *hex )
{
    char *cp, *cp2, *hex2;

    key->type  = KEY_RSA;
    key->flags = 0;
    key->dsa   = NULL;
    key->rsa   = RSA_new();

    cp = cp2 = hex2 = xstrdup( hex );

    // Skip leading "0x" on e
    if( (cp[0] == '0') && (cp[1] == 'x') )
        cp += 2;

    // Find the comma after e
    for( cp2 = cp; *cp2 && (*cp2 != ','); cp2++ ) ;

    // Terminate string on the comma after e
    if( *cp2 == ',' )
        *cp2++ = '\0';

    int vale = BN_hex2bn( &key->rsa->e, cp );

    //debug2( "vale = %d;     cp  = \"%s\"", vale, cp );
    //debug2( "BN_bn2hex(e) returned \"%s\"", BN_bn2hex(key->rsa->e) );

    // Skip leading "0x" on n
    if( (cp2[0] == '0') && (cp2[1] == 'x') )
        cp2 += 2;

    int valn = BN_hex2bn( &key->rsa->n, cp2 );

    //debug2( "valn = %d;     cp2 = \"%s\"", valn, cp2 );
    //debug2( "BN_bn2hex(n) returned \"%s\"", BN_bn2hex(key->rsa->n) );

    xfree( hex2 );
}

/* RHT added 27/07/2007 */
static char *FormatKey( const Key *key )
{
	switch(key->type)
	{
	case KEY_RSA1:
	case KEY_RSA:
		if(key->rsa != NULL) {
			return rsa_fmtkey(key);
		}
		break;
    case KEY_DSA:
		if(key->dsa != NULL) {
	        return dss_fmtkey(key);
		}
		break;
    }

    return NULL;
}

/* RHT added 27/07/2007 */
static void UnformatKey( int keytype, Key *key, const char *str )
{
	switch(keytype)		// RHT 04/03/2011 - was key->type
	{
	case KEY_RSA1:
	case KEY_RSA:
        rsa_unfmtkey( key, str );
		key->type = keytype;
		break;
    case KEY_DSA:
        dss_unfmtkey( key, str );
		break;
    }
}


//extern TRISOCKET *RTO_Server;


#if 0 /*RHT - just to prove this isn't used */
static HostStatus
check_host_in_hostfile_by_key_or_type(const char *filename,
    const char *host, const Key *key, int keytype, Key *found, int *numret)
{
	FILE *f;
	char line[8192];
	int linenum = 0;
	u_int kbits;
	char *cp, *cp2, *hashed_host;
	HostStatus end_return;

	debug3("check_host_in_hostfile: filename %s", filename);

	/* Open the file containing the list of known hosts. */
	f = fopen(filename, "r");
	if (!f)
		return HOST_NEW;

	/*
	 * Return value when the loop terminates.  This is set to
	 * HOST_CHANGED if we have seen a different key for the host and have
	 * not found the proper one.
	 */
	end_return = HOST_NEW;

	/* Go through the file. */
	while (fgets(line, sizeof(line), f)) {
		cp = line;
		linenum++;

		/* Skip any leading whitespace, comments and empty lines. */
		for (; *cp == ' ' || *cp == '\t'; cp++)
			;
		if (!*cp || *cp == '#' || *cp == '\n')
			continue;

		/* Find the end of the host name portion. */
		for (cp2 = cp; *cp2 && *cp2 != ' ' && *cp2 != '\t'; cp2++)
			;

		/* Check if the host name matches. */
		if (match_hostname(host, cp, (u_int) (cp2 - cp)) != 1) {
			if (*cp != HASH_DELIM)
				continue;
			hashed_host = host_hash(host, cp, (u_int) (cp2 - cp));
			if (hashed_host == NULL) {
				debug("Invalid hashed host line %d of %s",
				    linenum, filename);
				continue;
			}
			if (strncmp(hashed_host, cp, (u_int) (cp2 - cp)) != 0)
				continue;
		}

		/* Got a match.  Skip host name. */
		cp = cp2;

		/*
		 * Extract the key from the line.  This will skip any leading
		 * whitespace.  Ignore badly formatted lines.
		 */
		if (!hostfile_read_key(&cp, &kbits, found))
			continue;

		if (numret != NULL)
			*numret = linenum;

		if (key == NULL) {
			/* we found a key of the requested type */
			if (found->type == keytype)
				return HOST_FOUND;
			continue;
		}

		if (!hostfile_check_key(kbits, found, host, filename, linenum))
			continue;

		/* Check if the current key is the same as the given key. */
		if (key_equal(key, found)) {
			/* Ok, they match. */
			debug3("check_host_in_hostfile: match line %d", linenum);
			fclose(f);
			return HOST_OK;
		}
		/*
		 * They do not match.  We will continue to go through the
		 * file; however, we note that we will not return that it is
		 * new.
		 */
		end_return = HOST_CHANGED;
	}
	/* Clear variables and close the file. */
	fclose(f);

	/*
	 * Return either HOST_NEW or HOST_CHANGED, depending on whether we
	 * saw a different key for the host.
	 */
	return end_return;
}
#endif /* 0 - RHT */


HostStatus
check_host_in_hostfile(const char *filename, const char *host, const Key *key,
    Key *found, int *numret)
{
#ifdef THIS_IS_NOT_DEFINED
	if (key == NULL)
		fatal("no key to look up");
	return (check_host_in_hostfile_by_key_or_type(filename, host, key, 0,
	    found, numret));
#else
	char *keystring;
	char *foundstring = NULL;
	HostStatus ret;

	debug3("check_host_in_hostfile( \"%s\", \"%s\", ... )", "sshhostkeys" /*filename*/, host);

	if (key == NULL)
		fatal("no key to look up");

	keystring = FormatKey( key );
	// RHT 17/02/2011 - don't need RTO_Server param now
	ret = SFTP_CheckHostInHostfile( filename, host, key->type, keystring, &foundstring, numret );
	debug3("foundstring = \"%s\"; ret = %d", (foundstring ? foundstring : "(null)"), ret);
	if(*numret > 0) {
		UnformatKey( key->type, found, foundstring );
	}
	free( keystring );
	return ret;
#endif /*THIS_IS_NOT_DEFINED*/
}

int
lookup_key_in_hostfile_by_type(const char *filename, const char *host,
    int keytype, Key *found, int *numret)
{
#ifdef THIS_IS_NOT_DEFINED
	return (check_host_in_hostfile_by_key_or_type(filename, host, NULL,
	    keytype, found, numret) == HOST_FOUND);
#else
	char *foundstring = NULL;
	int ret;

	debug3("lookup_key_in_hostfile_by_type( \"%s\", \"%s\", %d, ... )", "sshhostkeys" /*filename*/, host, keytype);

	// RHT 17/02/2011 - don't need RTO_Server param now
	ret = SFTP_LookupKeyInHostfileByType( filename, host, keytype, &foundstring, numret );
	debug3("foundstring = \"%s\"; ret = %d", foundstring ? foundstring : "(null)", ret);
	if( ret ) {	// RHT 04/03/2011 - was ( *numret > 0 )
		UnformatKey( keytype, found, foundstring );
	}
	return ret;
#endif /*THIS_IS_NOT_DEFINED*/
}

/*
 * Appends an entry to the host file.  Returns false if the entry could not
 * be appended.
 */
int
add_host_to_hostfile(const char *filename, const char *host, const Key *key,
    int store_hash)
{
	debug3("add_host_to_hostfile( \"%s\", \"%s\", ... )", filename, host);

#ifdef THIS_IS_NOT_DEFINED
	FILE *f;
	int success = 0;
	char *hashed_host = NULL;

	if (key == NULL)
		return 1;	/* XXX ? */
	f = fopen(filename, "a");
	if (!f)
		return 0;

	if (store_hash) {
		if ((hashed_host = host_hash(host, NULL, 0)) == NULL) {
			error("add_host_to_hostfile: host_hash failed");
			fclose(f);
			return 0;
		}
	}
	fprintf(f, "%s ", store_hash ? hashed_host : host);

	if (key_write(key, f)) {
		success = 1;
	} else {
		error("add_host_to_hostfile: saving key in %s failed", filename);
	}
	fprintf(f, "\n");
	fclose(f);
	return success;
#else
	char *hashed_host = NULL;

	// RHT 25/02/2011 - sanity check
	if (key == NULL)
		return 1;

	char *keystring = key ? FormatKey( key ) : NULL;

	if (store_hash) {
		if ((hashed_host = host_hash(host, NULL, 0)) == NULL) {
			error("add_host_to_hostfile: host_hash failed");
			return 0;
		}
	}

	// RHT 17/02/2011 - don't need RTO_Server param now
	int ret = SFTP_AddKnownHostToServer(
					(store_hash ? hashed_host : host),
					22, key->type, keystring );
	free( keystring );

	return 1; //(ret == REQUEST_SUCCESS) ? 1 : 0;
#endif /*THIS_IS_NOT_DEFINED*/
}
