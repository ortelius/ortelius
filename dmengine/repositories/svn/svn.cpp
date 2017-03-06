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
#ifdef WIN32
#include "windows.h"
#define SVN_EXPORT __declspec(dllexport)
#else
#define SVN_EXPORT
#endif /*WIN32*/

#include <stdio.h>
#include <sys/stat.h>

#include "svn.h"

#include "../../dmapi/properties.h"
#include "../../dmapi/model.h"
#include "../../dmapi/dropzone.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/cmdline.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/expr.h"
#include "../../dmapi/charptr.h"
#include "../../dmapi/node.h"
#include "../../dmapi/context.h"
#include "../../dmapi/execute.h"


#define SVN_PLUGIN_VERSION "1.00"


///////////////////////////////////////////////////////////////////////////////
// SvnRepositoryImpl
///////////////////////////////////////////////////////////////////////////////

SvnRepositoryImpl::SvnRepositoryImpl(
		RepositoryImplFactory &factory, DMINT32 implId, Repository &repository,
		const char *url, const char *path
	)
	: RepositoryImpl(factory, implId, repository),
	  m_url(DUP_NULL(url)), m_path(DUP_NULL(path))
{
	debug1("SvnRepositoryImpl('%s', '%s')",
		NULL_CHECK(m_url), NULL_CHECK(m_path));
}


SvnRepositoryImpl::~SvnRepositoryImpl()
{
	SAFE_FREE(m_url);
	SAFE_FREE(m_path);
}


/**
 * Very simple test to make sure that the repository is correctly configured
 * with all the mandatory properties.  Just tests to make sure that "svn"
 * binary is available.
 */
bool SvnRepositoryImpl::testProvider(class Context &ctx)
{
	if(!m_url) {
		ctx.writeToStdErr("\"url\" property must be specified");
		return false;
	}

	Property *svn_bin = m_repository.getDefProperty("svn_bin");

	if (svn_bin) {
		ConstCharPtr svn_exe = svn_bin->decryptedValue(m_repository);
		struct stat buf;
		if(stat(svn_exe, &buf) == -1) {
			ctx.writeToStdErr("Executable '%s' not found", (const char*) svn_exe);
			return false;
		}
	} else {
		ctx.writeToStdErr("Missing provider definition property 'svn_bin' for provider '%s'",
			m_repository.name());
	}
	return true;
}


void SvnRepositoryImpl::checkout(
	const char *dzbase, const char *dzpath,
	bool recursive, StringList *pattern,
	ExtendedStmt &stmt, DropzoneCallback &callback,
	Context &ctx)
{
	// Dropzone: $TEMP/$DEPID.$DEPMAJOR
	// Url: whatever is specified on the repository
	// Username/Password: ???

	// TODO: Use pattern and recursive parameters

	Credentials *creds = m_repository.getCredentials();

	const char *url = m_url;

	// Now add the complete path to the url
	CharPtr tempurl;
	if(m_path) {
		tempurl = (char*) malloc(strlen(url) + strlen(m_path) + 2);
		sprintf(tempurl, ((m_path[0] == '/') ? "%s%s" : "%s/%s"), url, m_path);
		url = tempurl;
	}

	ConstCharPtr revision = stmt.getArgAsString("revision", ctx);

	Property *svn_bin = m_repository.getDefProperty("svn_bin");
//#ifdef DEV_VERSION
//	// Dev version runs two platforms out of one database - this code allows
//	// two different values to be specified
//	if(!svn_bin) {
//#ifdef WIN32
//		svn_bin = m_repository.getDefProperty("svn_bin_win");
//#else
//		svn_bin = m_repository.getDefProperty("svn_bin_unix");
//#endif /*WIN32*/
//	}
//#endif /*DEV_VERSION*/
	if(!svn_bin) {
		throw RuntimeError(ctx.stack(),
			"Missing provider definition property 'svn_bin' for provider '%s'",
			m_repository.name());
	}
	char *svn_exe = svn_bin->decryptedValue(m_repository);
	// Expand binary location (may have DMHOME or similar in path)
	Node expandedText1(NODE_STR, svn_exe, true);	// will free svn_exe on delete
	ExprPtr etext1 = expandedText1.evaluate(ctx);
	ConstCharPtr stext1 = etext1->stringify();
	debug1("%s", (const char *)stext1);
	CmdLine cmd(stext1);


	//printf("svn --non-interactive --trust-server-cert --no-auth-cache --username robert --password robert co %s %s\n", (m_url ? m_url : "(null)"), path);

	// CmdLine cmd(svn_exe);
	cmd.add("--non-interactive").add("--trust-server-cert").add("--no-auth-cache");

	if(revision) {
		cmd.add("-r").add(revision);
	}

	if(creds && creds->hasUsername() && creds->hasPassword()) {
		char *username = creds->getDecryptedUsername(ctx);
		char *password = creds->getDecryptedPassword(ctx);
		cmd.add("--username").add(username).add("--password").add(password);
		SECURE_FREE(username);
		SECURE_FREE(password);
	}

	cmd.add("checkout").add(url).add(dzpath);

	char *pocmd = cmd.toCommandString(true);
	debug1("%s", pocmd);
	free(pocmd);

	CapturedData *cd = NULL;
	int tempExitStatus;
	int ret = executeAndCapture(ctx.stream(), cmd, NULL, false, ctx.threadId(), &tempExitStatus, &cd, NULL);
	if(ret) {
		if(cd && (cd->bytesOfStandardErr() > 0)) {
			cd->appendStandardErr("\0", 1);
			ctx.dm().writeToStdOut("ERR: %s", cd->standardErr());
		}
		if(cd && (cd->bytesOfStandardOut() > 0)) {
			cd->appendStandardOut("\0", 1);
			ctx.dm().writeToStdOut("OUT: %s", cd->standardOut());
		}
		ctx.dm().writeToStdOut("svn checkout failed to execute");
		SAFE_DELETE(cd);
		throw RuntimeError(ctx.stack(), "Command did not execute successfully");
	}
	debug1("tempExitStatus = %d", tempExitStatus);

	if(cd) {
		cd->appendStandardOut("\0", 1);
		debug1("cd->stdout=[%s]",cd->standardOut());
		debug1("cd->stderr=[%s]",cd->standardErr());
		char *temp = NULL;
		int revision = -1;
		// First pass - find the revision number
		int msglen = strlen("Checked out revision ");
		const char *pos = strstr(cd->standardOut(), "Checked out revision ");
		if(pos) {
			if(strstr((pos + msglen), "Checked out revision ")) {
				ctx.writeToStdErr("WARNING: Unable to handle multiple revision numbers on svn checkout");
			}
			revision = atoi(pos + msglen);
		}
		if(revision == -1) {
			ctx.writeToStdErr("WARNING: No revision number given during svn checkout");
		}
		debug1("revision = %d", revision);
		char version[33];
		sprintf(version, "%d", revision);

		// Record root and path folders up front, as these all map to the info file for the repository
		callback.checked_out_folder("/", "/", true);
		if(m_path) {
			callback.checked_out_folder(m_path, "/", true);
		}

		int total = 0, success = 0, failed = 0, not_processed = 0;

		// Second pass - parse the filenames
		// TODO: This is destructive to the buffer - CaptureData should provide an iterator
		temp = NULL;
		for(char *line = STRTOK_R((char*) cd->standardOut(), "\r\n", &temp);
			line; line = STRTOK_R(NULL, "\r\n", &temp)) {
				//printf("INFO: %s\n", line);
				if(*line == 'A') {
					// Callback to the repository to say we have checked out a file
					++line;
					while((*line == ' ') || (*line == '\t')) { ++line; }

					struct stat s;
					int res = stat(line, &s);
					if(res != 0) {
						debug1("Failed to stat checked out file \"%s\"", line);
					}

					// lop off path to dropzone to give relative dropzone path
					char *dzfn = line;
					if(strstr(dzfn, dzbase)) {
						dzfn += strlen(dzbase);
					}
					if((dzfn[0] == DIR_SEP_CHAR) && dzfn[1]) {
						dzfn++;
					}
					debug2("dzfn: '%s' minus '%s' gives '%s'", line, dzbase, dzfn);

					// lop off the path to the checkout folder to get the
					// relative repo path, then prepend "path"
					char *rpfn = line;
					if(strstr(rpfn, dzpath)) {
						rpfn += strlen(dzpath);
					}
					debug2("rpfn: '%s' minus '%s' gives '%s'", line, dzpath, rpfn);

					CharPtr repopath;
					if(m_path) {
						if((rpfn[0] == DIR_SEP_CHAR) && rpfn[1]) {
							rpfn++;
						}
						repopath = (char*) malloc(strlen(rpfn) + strlen(m_path) + 2);
						sprintf(repopath, "%s/%s", m_path, rpfn);
					} else {
						repopath = strdup(rpfn);
					}
					debug2("repopath: '%s'", (const char*) repopath);

#ifdef WIN32
					// Repo path needs slashes converting for windows
					for(char *x = repopath; x && *x; x++) {
						if(*x == '\\') { *x = '/'; }
					}
#endif /*WIN32*/

					// If this is a file (and not a folder)
					if(s.st_mode & STAT_IS_DIRECTORY) {
						callback.checked_out_folder(repopath, dzfn, false);
					} else {
						++total; ++success;
						callback.checked_out_file(this, repopath, dzfn, version);
					}
				}
		}

		callback.checkout_summary(total, success, failed, not_processed);
	}

	SAFE_DELETE(cd);
}


void SvnRepositoryImpl::checkin(
	const char *dzbase, const char *dzpath, ExtendedStmt &stmt,
	DropzoneCallback &callback, Context &ctx)
{
	throw RuntimeError(ctx.stack(), "Checkin not implemented");
}


char *SvnRepositoryImpl::dirname(const char *filename)
{
	const char *ls = strrchr(filename, '/');
	if(!ls) {
		return NULL;
	}

	int len = ls - filename;

	char *ret = (char*) malloc(len + 1);
	if (ret) {
		strncpy(ret, filename, len);
		ret[len] = '\0';
	}
	return ret;
}


Expr *SvnRepositoryImpl::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "url") == 0) {
		return new Expr(m_url);
	} else if(strcmp(name, "path") == 0) {
		return new Expr(m_path);
	}

	return RepositoryImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// SvnRepositoryImplFactory
///////////////////////////////////////////////////////////////////////////////

SvnRepositoryImplFactory::SvnRepositoryImplFactory()
{}


RepositoryImpl *SvnRepositoryImplFactory::create(
	DMINT32 implId, Repository &parent, ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr url  = stmt.getOverridableArgAsString("url", parent, true, ctx);
	ConstCharPtr path = stmt.getOverridableAppendableArgAsString("path", '/', '/', parent, false, ctx);
	return new SvnRepositoryImpl(*this, implId, parent, url, path);
}


///////////////////////////////////////////////////////////////////////////////

// TODO: remove "svn_" prefix when moving to a DLL

extern "C" SVN_EXPORT int svn_PluginStart(DM &dm)
{
	dm.writeToLogFile("Subversion plugin V" SVN_PLUGIN_VERSION);
	
	RepositoryImplRegistry::instance().registerFactory("svn", new SvnRepositoryImplFactory());
	return 0;
}


extern "C" SVN_EXPORT int svn_PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("url",      true,  false));
	propdefs->add(new PropertyDef("path",     false, true ));
	propdefs->add(new PropertyDef("revision", false, false));

	dm.installProviderImpl("svn", NULL, OBJ_KIND_REPOSITORY, propdefs);
	return 0;
}
