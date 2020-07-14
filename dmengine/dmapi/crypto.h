#ifndef __crypto_h

#define __crypto_h

char *decryptValue(const char *input, int length);

char *encryptValue(const char *input, int length);

char *decryptFile(const char *filename, int *len);

char *digestValue(const char *input, int length, bool sha256);

bool readPassphraseFile(const char *filename);

bool createPassphraseFile(const char *filename);

char *cmac_md5(const char *challenge, const char *username, const char *secret);

unsigned char *decryptbase64(char *md,int *retlen);

#endif /*__crypto_h*/
