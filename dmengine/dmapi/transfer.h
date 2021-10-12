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

#ifndef __transfer_h

#define __transfer_h


#include "hashtable.h"


/**
 * dzfile is tied back afterwards to the dropzone file that we have transferred
 * ischeckin - true if the file is being transferred from the target server,
 *   false if it is being transferred to the target server
 */
class TransferResult
{
private:
	char *m_sourcedir;
	char *m_targetdir;
	char *m_filename;
	char *m_error;
	DMINT32 m_size;
	char *m_md5;
	bool m_istext;
	bool m_ischeckin;
	class DropzoneFile *m_dzfile;

public:
	TransferResult();
	~TransferResult();

	void setProperty(const char* prop, const char *value);

	DropzoneFile *dropzoneFile()             { return m_dzfile; }
	void setDropzoneFile(DropzoneFile *dzf)  { m_dzfile = dzf; }

	const char *md5sum()					 { return m_md5; }
	DMINT32 size()							 { return m_size; }
	bool isText()							 { return m_istext; }

	char *targetFilename();
	char *dropzoneFilename();
};


class TransferProviderImpl
{
public:
	virtual ~TransferProviderImpl();

	// Remove old files
	virtual void deleteFromServer(
		class Component &comp,class Context &ctx) = 0;

	// Checkout type transfer
	virtual List<TransferResult> *transferToServer(
		const char *dropzone,  bool ServerSpecific, class Context &ctx) = 0;

	// Checkin type transfer
	virtual void transferFromServer(
		const char *dropzone, class Context &ctx) = 0;

	// Run a RemoteScript
	virtual int runScriptOnServer(
		const char *shell, const char *script, class StringList *params,
		bool copy, bool useTTY, class Context &ctx) = 0; 

	// Check server is reachable and base directory is okay
	virtual int checkServer(
		Context &ctx) = 0;
};


class TransferProviderImplFactory
{
public:
	virtual ~TransferProviderImplFactory();

	virtual TransferProviderImpl *create(
		class Server &target,
		class Environment *tgtenv,
		class Credentials *creds,
		class Component *comp,
		const char *targetPath) = 0;
};


class TransferProviderImplRegistry
{
private:
	Hashtable<TransferProviderImplFactory> m_factories;

	static TransferProviderImplRegistry *s_instance;

public:
	TransferProviderImplRegistry();

	static TransferProviderImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, TransferProviderImplFactory *factory);
	void registerBuiltIns();

	TransferProviderImplFactory *getFactory(const char *name);
};


#endif /*__transfer_h*/
