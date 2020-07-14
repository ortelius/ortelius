#include <string.h>

#include "crypto.h"

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#include "platform.h"

#ifdef HAVE_DMEXCEPTIONS
#include "exceptions.h"
#endif /**/


#define MAGIC_VALUE "Salted__"


BIO *addEncryptionBIO(BIO *bmem, const char *passphrase, const unsigned char *salt, bool enc)
{
	const EVP_CIPHER *cipher = EVP_des_ede3_cbc();

	unsigned char key[EVP_MAX_KEY_LENGTH];
	unsigned char iv[EVP_MAX_IV_LENGTH];

	// Turn the passphrase into a key and iv flavoured with salt
	/*int keylen =*/ EVP_BytesToKey(cipher, EVP_md5(), (const unsigned char*) salt,
		(const unsigned char*) passphrase, strlen(passphrase), 1, key, iv);
//printf("DEBUG: EVP_BytesToKey returned %d\n", keylen);

	/*
	printf("PASSPHRASE is: ");
	for (int i=0;i<strlen(passphrase);i++) {
		printf("%02x ",passphrase[i]);
	}
	printf("\n");

	printf("KEY is: ");
	for (int i=0;i<EVP_MAX_KEY_LENGTH;i++) {
		printf("%02x ",key[i]);
	}
	printf("\n");

	printf("IV is: ");
	for (int i=0;i<EVP_MAX_IV_LENGTH;i++) {
		printf("%02x ",iv[i]);
	}
	printf("\n");
	*/
	// Now decrypt the rest of the input
	EVP_CIPHER_CTX *ctx = NULL;
	BIO *benc = BIO_new(BIO_f_cipher());
	BIO_get_cipher_ctx(benc, &ctx);
	if(!EVP_CipherInit_ex(ctx, cipher, NULL, NULL, NULL, (enc ? 1 : 0))) {
		BIO_free_all(bmem);
#ifdef HAVE_DMEXCEPTIONS
		throw RuntimeError("ERROR: Failed setting cipher '%s'", EVP_CIPHER_name(cipher));
#else
		throw "Failed setting cipher";
#endif /*HAVE_DMEXCEPTIONS*/
	}
	if(!EVP_CipherInit_ex(ctx, NULL, NULL, key, iv, (enc ? 1 : 0))) {
		BIO_free_all(bmem);
#ifdef HAVE_DMEXCEPTIONS
		throw RuntimeError("ERROR: Failed setting key for cipher '%s'", EVP_CIPHER_name(cipher));
#else
		throw "Failed setting key for cipher";
#endif /*HAVE_DMEXCEPTIONS*/
	}

	return BIO_push(benc, bmem);
}


BIO *addBase64BIO(BIO *bmem)
{
	BIO *b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	return BIO_push(b64, bmem);
}


char *decryptBIO(BIO *bmem, const char *passphrase, int *len)
{
	// Convert passphrase into key and iv using the salt from the start of
	// the encrypted value: "Salted__saltsaltdatadatadatadata"
	char dummy[8];
	unsigned char salt[8];

	bmem = addBase64BIO(bmem);

	// Read "Salted__" followed by salt from input
	if((BIO_read(bmem, dummy, sizeof(dummy)) != sizeof(dummy))
			|| (BIO_read(bmem, salt, sizeof(salt)) != sizeof(salt))) {
		BIO_free_all(bmem);
#ifdef HAVE_DMEXCEPTIONS
		throw RuntimeError("ERROR: Failed to read input in order to decrypt");
#else
		throw "Failed to read input in order to decrypt";
#endif /*HAVE_DMEXCEPTIONS*/
	}

	

	// Sanity check
	if(strncmp(dummy, MAGIC_VALUE, strlen(MAGIC_VALUE)) != 0) {
		BIO_free_all(bmem);
#ifdef HAVE_DMEXCEPTIONS
		throw RuntimeError("ERROR: Invalid input");
#else
		throw "Invalid input";
#endif /*HAVE_DMEXCEPTIONS*/
	}

	// Add decryptor
	bmem = addEncryptionBIO(bmem, passphrase, salt, false);

	int datalen = 1024;
	char *res = (char*) malloc(datalen + 1);

	int rlen = BIO_read(bmem, res, datalen);
//printf("DEBUG: BIO_read returned %d\n", rlen);
	if(rlen >= 0) {
		res[rlen] = '\0';
	}

	BIO_free_all(bmem);

//printf("DEBUG: output '%s'\n", ((res[0] == 'p') ? res : "corrupt"));
//dumpbuffer(res, rlen);

	if(len) {
		*len = rlen - sizeof(dummy);
	}

	return (rlen >= 0) ? res : NULL;
}


bool encryptBIO(BIO *bmem, const char *input, int length, const char *passphrase)
{
	// Random salt
	unsigned char salt[8];
	if(RAND_pseudo_bytes(salt, sizeof(salt)) < 0) {
		return false;
	}

	// Add base64 encoder
	bmem = addBase64BIO(bmem);

	// Write magic "Salted__"
	BIO_write(bmem, (void*) MAGIC_VALUE, strlen(MAGIC_VALUE));

	// Add a pinch of salt
	BIO_write(bmem, (void*) salt, sizeof(salt));

	// Add the encryptor
	if(!(bmem = addEncryptionBIO(bmem, passphrase, salt, true))) {
		return false;
	}

	// Write out the encrypted passphrase
	BIO_write(bmem, (void*) input, length);
	BIO_flush(bmem);
	return true;
}


/**
 * The passphrase that will be use for database decryptions.
 */
static const char *s_passphrase = NULL;

/**
 * Decrypt a triple DES encrypted and base-64 encoded value from the database.
 * The salt is stored on the front of the encoded value.  The key is generated
 * from a passphrase.
 */
char *decryptValue(const char *input, int length)
{
// printf("DEBUG: decrypt - input '%s'\n", input);

	if(!s_passphrase) {
#ifdef HAVE_DMEXCEPTIONS
		throw RuntimeError("ERROR: passphrase not initialised");
#else
		throw "Passphrase not initialised";
#endif /*HAVE_DMEXCEPTIONS*/
	}

	BIO *bmem = BIO_new_mem_buf((void*) input, length);
	return decryptBIO(bmem, s_passphrase, NULL);
}


char *encryptValue(const char *input, int length)
{
//printf("DEBUG: encrypt - input '%s'\n", input);

	if(!s_passphrase) {
#ifdef HAVE_DMEXCEPTIONS
		throw RuntimeError("ERROR: passphrase not initialised");
#else
		throw "Passphrase not initialised";
#endif /*HAVE_DMEXCEPTIONS*/
	}

	// Write to memory buffer
	BIO *bmem = BIO_new(BIO_s_mem());

	if(encryptBIO(bmem, input, length, s_passphrase)) {
		BUF_MEM *bptr = NULL;
		BIO_get_mem_ptr(bmem, &bptr);

		char *buf = (char*) malloc(bptr->length + 1);
		memcpy(buf, bptr->data, bptr->length);
		buf[bptr->length] = '\0';

		BIO_free_all(bmem);
		return buf;
	}

	BIO_free_all(bmem);
	return NULL;
}


/**
 * Decrypt a triple DES encrypted and base-64 encoded file.   The salt is
 * stored on the front of the encoded value.  The key is generated from a
 * passphrase.
 */
char *decryptFile(const char *filename, int *len)
{
	if(!s_passphrase) {
#ifdef HAVE_DMEXCEPTIONS
		throw RuntimeError("ERROR: passphrase not initialised");
#else
		throw "Passphrase not initialised";
#endif /*HAVE_DMEXCEPTIONS*/
	}
	BIO *bfile = BIO_new_file(filename, "r");
	return decryptBIO(bfile, s_passphrase, len);
}


/**
 * Decrypt a triple DES encrypted and base-64 encoded passphrase from a file.
 * The salt is stored on the front of the encoded value.  The key is generated
 * from another passphrase.
 */
bool readPassphraseFile(const char *filename)
{
	const char *passphrase = "dm15k1ng";
	BIO *bfile = BIO_new_file(filename, "r");
	if(!bfile) {
		return false;
	}
	s_passphrase = decryptBIO(bfile, passphrase, NULL);
	/*
	printf("s_passphrase is: ");
	for (int i=0;i<strlen(s_passphrase);i++)
	{
		printf("%02x ",s_passphrase[i]);
	}
	printf("\n");
	*/
	return s_passphrase ? true : false;
}


/**
 * Generate a random passphrase and store it in an encrypted file.
 * The passphrase is a random sequence of 16 bytes which is then base-64
 * encoded to give only printable characters.  This is then encrypted using
 * triple DES and base-64 encoded before being written to a file.
 * 
 */
bool createPassphraseFile(const char *filename)
{
	const char *passphrase = "dm15k1ng";

	// Generate a random passphrase - start with random bytes, then sanitise
	// using base64 to get printable characters
	unsigned char randomBytes[16];
	if(RAND_pseudo_bytes(randomBytes, sizeof(randomBytes)) < 0) {
		return false;
	}

	BIO *bpp = BIO_new(BIO_s_mem());
	bpp = addBase64BIO(bpp);

	BIO_write(bpp, (void*) randomBytes, sizeof(randomBytes));
	BIO_flush(bpp);

	BUF_MEM *bptr = NULL;
	BIO_get_mem_ptr(bpp, &bptr);

	char *randomString = (char*) malloc(bptr->length + 1);
	memcpy(randomString, bptr->data, bptr->length);
	randomString[bptr->length] = '\0';

	BIO_free_all(bpp);

	// Now write the file
	BIO *bmem = BIO_new_file(filename, "w");
	if(!bmem) {
		return false;
	}
	
	bool ret = encryptBIO(bmem, randomString, strlen(randomString), passphrase);

	SAFE_FREE(randomString);
	BIO_free_all(bmem);
	return ret;
}


/**
 * Creates a SHA256 digest of input and base64 encodes the result.
 */
char *digestValue(const char *input, int length, bool sha256)
{
	// Create a digest by writing input through into a buffer (the buffer will
	// contain the input, but the digester will have calculated the digest)
	BIO *bmem = BIO_new(BIO_s_mem());

	const EVP_MD *digest = sha256 ? EVP_sha256() : EVP_md5();

	EVP_MD_CTX *ctx = NULL;
	BIO *bmd = BIO_new(BIO_f_md());
	BIO_set_md(bmd, digest);	// This seems to be needed on Win x64
	BIO_get_md_ctx(bmd, &ctx);
	if(!ctx) {
		printf("ERROR: Failed getting md ctx\n");
		BIO_free_all(bmem);
		return NULL;
	}
	if(!EVP_DigestInit_ex(ctx, digest, NULL)) {
		printf("ERROR: Failed setting digest '%s'\n", EVP_MD_name(digest));
		BIO_free_all(bmem);
		return NULL;
	}

	bmem = BIO_push(bmd, bmem);

	BIO_write(bmem, (void*) input, length);
	BIO_flush(bmem);

	// Now pull the raw digest off the digester
	unsigned char md[SHA256_DIGEST_LENGTH];
	unsigned int mdlen = 0;
	if(!EVP_DigestFinal_ex(ctx, (unsigned char*) md, &mdlen)) {
		printf("ERROR: Failed getting digest '%s'\n", EVP_MD_name(digest));
		BIO_free_all(bmem);
		return NULL;
	}

	BIO_free_all(bmem);

	// Finally base64 encode the digest
	bmem = BIO_new(BIO_s_mem());
	bmem = addBase64BIO(bmem);

	BIO_write(bmem, (void*) md, mdlen);
	BIO_flush(bmem);

	BUF_MEM *bptr = NULL;
	BIO_get_mem_ptr(bmem, &bptr);

	char *buf = (char*) malloc(bptr->length + 1);
	memcpy(buf, bptr->data, bptr->length);
	buf[bptr->length] = '\0';

	BIO_free_all(bmem);

	return buf;
}


char *cmac_md5(const char *challenge, const char *username, const char *secret)
{
	// base64 decode the challenge
	BIO *bmem = BIO_new_mem_buf((void*) challenge, strlen(challenge));
	bmem = addBase64BIO(bmem);
	unsigned char chal[1024];
	int clen = BIO_read(bmem, chal, sizeof(chal)-1);
	if(clen >= 0) {
		chal[clen] = '\0';
	}
	BIO_free_all(bmem);

	if(clen == 0) {
		//debug1("Failed to decode challenge '%s'", challenge);
		return NULL;
	}

	//debug1("Decoded challenge is '%s'", chal);

	// Encrypt the challenge using the user's password/secret
	unsigned char *result = HMAC(EVP_md5(), secret, strlen(secret), chal, clen, NULL, NULL);

	// Convert to hex string
	int result_len = 16;
	char res_hexstring[33];
	for(int i = 0; i < result_len; i++) {
		sprintf(&(res_hexstring[i * 2]), "%02x", result[i]);
	}

	//debug1("Encrypted challenge is '%s'", res_hexstring);

	// prepend username
	char buf[1024];
	sprintf(buf, "%s %s", username, res_hexstring);

	//debug1("Raw response is '%s'", buf);

	// base64 encode
	bmem = BIO_new(BIO_s_mem());
	bmem = addBase64BIO(bmem);
	BIO_write(bmem, buf, strlen(buf));
	BIO_flush(bmem);
	BUF_MEM *bptr = NULL;
	BIO_get_mem_ptr(bmem, &bptr);

	char *resp = (char*) malloc(bptr->length + 1);
	memcpy(resp, bptr->data, bptr->length);
	resp[bptr->length] = '\0';
	BIO_free_all(bmem);

	//debug1("Response '%s'", resp);
	return resp;
}

unsigned char *decryptbase64(char *md,int *retlen)
{
	BIO *bmem = BIO_new_mem_buf((void*) md, strlen(md));
	
	BIO *b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	bmem=BIO_push(b64, bmem);

	unsigned char buf[1024];
	unsigned char *res=0;
	*retlen=0;
	int len = BIO_read(bmem, buf, sizeof(buf));
	do {
		if (len >= 0) {
			int op=*retlen;
			*retlen+=len;
			res=(unsigned char *)(res?realloc(res,*retlen):malloc(*retlen));
			memcpy(&(res[op]),(const void *)buf,len);
		}
		len = BIO_read(bmem, buf, sizeof(buf)-1);
	} while (len>0);
	BIO_free_all(bmem);
	return res;
}