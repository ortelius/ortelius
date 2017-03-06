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
