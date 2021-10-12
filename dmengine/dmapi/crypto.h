/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
