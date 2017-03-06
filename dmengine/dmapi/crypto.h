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
