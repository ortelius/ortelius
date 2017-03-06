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
#define PERFORCE_EXPORT __declspec(dllexport)
#else
#define PERFORCE_EXPORT
#endif /*WIN32*/

#include <stdio.h>
#include <sys/stat.h>

#include "perforce.h"

#include "cmdline.h"
#include "exceptions.h"
#include "expr.h"
#include "charptr.h"
#include "node.h"
#include "context.h"
#include "dm.h"
#include "properties.h"
#include "logprocess.h"
#include "execute.h"
#include "dropzone.h"


#define PERFORCE_PLUGIN_NAME    "perforce"
#define PERFORCE_REPO_NAME      "perforce"

#define PERFORCE_PLUGIN_VERSION "1.00"

#define PERFORCE_DEFAULT_PORT 1666


///////////////////////////////////////////////////////////////////////////////

char *perforce_getBinary(Repository &repo, const char *bin, Context &ctx)
{
	printf("perforce_getBinary\n");
	char propname[128];
	sprintf(propname, "%s_bin", bin);
	Property *exe = repo.getDefProperty(propname);
//#ifdef DEV_VERSION
//	// Dev version runs two platforms out of one database - this code allows
//	// two different values to be specified
//	if(!exe) {
//#ifdef WIN32
//		sprintf(propname, "%s_bin_win", bin);
//#else
//		sprintf(propname, "%s_bin_unix", bin);
//#endif /*WIN32*/
//		exe = repo.getDefProperty(propname);
//	}
//#endif /*DEV_VERSION*/
	if(!exe) {
		throw RuntimeError(ctx.stack(),
			"Missing provider definition property '%s_bin' for provider '%s'",
			bin, repo.name());
	}

	printf("exe decryptedValue is [%s]\n",exe->decryptedValue(repo));

	// Expand any embedded variables
	Node nexe(NODE_STR, exe->decryptedValue(repo), true);
	ExprPtr eexe = nexe.evaluate(ctx);
	return eexe->stringify();
}


///////////////////////////////////////////////////////////////////////////////
// PerforceRepositoryImpl
///////////////////////////////////////////////////////////////////////////////

PerforceRepositoryImpl::PerforceRepositoryImpl(
		RepositoryImplFactory &factory, DMINT32 implId, Repository &repository,
		const char *host, int port, const char *view
	)
	: RepositoryImpl(factory, implId, repository),
	  m_host(DUP_NULL(host)), m_port(port), m_view(DUP_NULL(view))
{
	debug1("PerforceRepositoryImpl('%s', %d, '%s')",
		NULL_CHECK(m_host), m_port, NULL_CHECK(m_view));
}


PerforceRepositoryImpl::~PerforceRepositoryImpl()
{
	SAFE_FREE(m_host);
	SAFE_FREE(m_view);
}


/**
 * Very simple test to make sure that the repository is correctly configured
 * with all the mandatory properties.  Just tests to make sure that "p4"
 * binary is available.
 */
bool PerforceRepositoryImpl::testProvider(class Context &ctx)
{
	if(!m_host) {
		ctx.writeToStdErr("\"host\" property must be specified");
		return false;
	}

	ConstCharPtr p4_exe = perforce_getBinary(m_repository, "p4", ctx);

	struct stat buf;
	if(stat(p4_exe, &buf) == -1) {
		ctx.writeToStdErr("Executable '%s' not found", (const char*) p4_exe);
		return false;
	}

	return true;
}


void PerforceRepositoryImpl::checkout(
	const char *dzbase, const char *dzpath,
	bool recursive, StringList *pattern,
	ExtendedStmt &stmt, DropzoneCallback &callback,
	Context &ctx)
{
	// Dropzone: $TEMP/$DEPID.$DEPMAJOR
	// Viewpath: \ or whatever is specified on the repository
	// Username/Password: ???

	Credentials *creds = m_repository.getCredentials();

	ConstCharPtr p4_exe = perforce_getBinary(m_repository, "p4", ctx);

	char workspace[64];
	sprintf(workspace, "tdm_%d_%d", ctx.dm().deployId(), implId());

	CharPtr p4port = (char*) malloc(strlen(m_host) + 32);
	sprintf(p4port, "%s:%d", m_host, m_port ? m_port : PERFORCE_DEFAULT_PORT);

	CmdLine cmd1(p4_exe);
	if(creds && creds->hasUsername() && creds->hasPassword()) {
		char *username = creds->getDecryptedUsername(ctx);
		char *password = creds->getDecryptedPassword(ctx);
		cmd1.add("-u").add(username).add("-P").add(password);
		SECURE_FREE(username);
		SECURE_FREE(password);
	}
	cmd1.add("-p").add(p4port).add("client").add("-i");

	char *pocmd = cmd1.toCommandString(true);
	// ctx.writeToStdOut("%s", pocmd);

	/*
	ctx.dm().writeToStdOut("Client: %s\n", workspace);
	ctx.dm().writeToStdOut("Root:	%s\n", dzpath);
	ctx.dm().writeToStdOut("Options:	noallwrite noclobber nocompress unlocked nomodtime normdir\n");
	ctx.dm().writeToStdOut("SubmitOptions:	submitunchanged\n");
	ctx.dm().writeToStdOut("LineEnd:	local\n");
	ctx.dm().writeToStdOut("View:	\"%s/*\" \"//%s/*\"\n", m_view, workspace);
	*/
	FILE *p = popen(pocmd, "w");
	if(p) {
		fprintf(p, "Client: %s\n", workspace);
		fprintf(p, "Root:	%s\n", dzpath);
		fprintf(p, "Options:	noallwrite noclobber nocompress unlocked nomodtime normdir\n");
		fprintf(p, "SubmitOptions:	submitunchanged\n");
		fprintf(p, "LineEnd:	local\n");
		fprintf(p, "View:	\"%s/*\" \"//%s/*\"\n", m_view, workspace);
		pclose(p);
	} else {
		debug1("Failed to popen command");
	}
	free(pocmd);

	CmdLine cmd2(p4_exe);
	if(creds && creds->hasUsername() && creds->hasPassword()) {
		char *username = creds->getDecryptedUsername(ctx);
		char *password = creds->getDecryptedPassword(ctx);
		cmd2.add("-u").add(username).add("-P").add(password);
		SECURE_FREE(username);
		SECURE_FREE(password);
	}
	cmd2.add("-c").add(workspace).add("-p").add(p4port).add("sync");
	if(pattern) {
		StringListIterator iter(*pattern);
		for(const char *patt = iter.first(); patt; patt = iter.next()) {
			if(recursive) {
				CharPtr patt2 = (char*) malloc(strlen(m_view) + strlen(patt) + 6);
				sprintf(patt2, "%s/.../%s", m_view, patt);	// TODO: Does this work for files in the top level?
				cmd2.add(patt2);
			} else {
				CharPtr patt2 = (char*) malloc(strlen(m_view) + strlen(patt) + 2);
				sprintf(patt2, "%s/%s", m_view, patt);
				cmd2.add(patt2);
			}
		}
	} else {
		CharPtr patt2 = (char*) malloc(strlen(m_view) + 5);
		sprintf(patt2, "%s/%s", m_view, recursive ? "..." : "*");
		cmd2.add(patt2);
	}
	
	pocmd = cmd2.toCommandString(true);
	// ctx.writeToStdOut("%s", pocmd);
	free(pocmd);
	CapturedData *cd = NULL;
	int res = cmd2.exec(&cd, ctx);
	if (res != 0) {
		// Sync command has failed
		cd->appendStandardErr("\0",1);
		ctx.writeToStdErr(cd->standardErr());
		SAFE_DELETE(cd);
		throw RuntimeError(stmt, ctx.stack(),
				"Command did not execute successfully");
	}
	CmdLine cmd3(p4_exe);
	cmd3.add("-p").add(p4port).add("client").add("-d").add(workspace);
	pocmd = cmd3.toCommandString(true);
	// ctx.writeToStdOut("%s", pocmd);
	free(pocmd);
	CapturedData *cd2 = NULL;
	int res2 = cmd3.exec(&cd2,ctx);

	if(res2 == -1) {
		cd2->appendStandardErr("\0",1);
		ctx.dm().writeToStdErr(cd2->standardErr());
		SAFE_DELETE(cd2);
		throw RuntimeError(stmt, ctx.stack(),
				"Command did not execute successfully");
	}
	SAFE_DELETE(cd2);

	// Record viewpath folders up front, as these all map to the info file for the repository
	//callback.checked_out_folder("\\", "\\", true);
	callback.checked_out_folder(m_view, "\\", true);

	if(cd) {
		cd->appendStandardOut("\0", 1);

		// //streamsDepot/mainline/dmdemo/scripts/applysql.bat#1 - added as C:\cygwin\tmp\tdm.547.perforce1\scripts\applysql.bat
		MessageMatcher mm("msgtext");
		mm.setRegexp("^([a-zA-Z0-9/._]*)#([0-9]*) - added as ([a-zA-Z0-9/\\._:]*)$");
		mm.addMapping(1, "dp");
		mm.addMapping(2, "rv");
		mm.addMapping(3, "cp");

		char *temp = NULL;
		for(char *line = STRTOK_R((char*) cd->standardOut(), "\r\n", &temp);
			line; line = STRTOK_R(NULL, "\r\n", &temp)) {
			// ctx.writeToStdOut("line=[%s]",line);
			AutoPtr<StringHashtable> matches = mm.match(line);
			if(matches) {
				const char *dp = matches->get("dp");
				const char *rv = matches->get("rv");
				const char *cp = matches->get("cp");
				// ctx.writeToStdOut("%s;%s -> %s", dp, rv, cp);

				const char *pn = cp ? strstr(cp, dzbase) : NULL;
				if(pn) {
					pn += strlen(dzbase);
					if(*pn == DIR_SEP_CHAR) {
						pn++;
					}
					callback.checked_out_file(this, dp, pn, rv);
					callback.checkout_summary(1,1,0,0);
					
					// Now work out the folder the file is in and where it maps to
					ConstCharPtr dpfolder = dirname(dp);
					ConstCharPtr dzfolder = Dropzone::dirname(pn);				
					callback.checked_out_folder(
						(dpfolder ? (const char*) dpfolder : m_view),					// TODO: not sure dirname returns null any more
						(dzfolder ? (const char*) dzfolder : DIR_SEP_STR), false);
				}
			} else {
				debug1("UNHANDLED MESSAGE: %s", line);
			}
		}

		SAFE_DELETE(cd);
	}
}


void PerforceRepositoryImpl::checkin(
	const char *dzbase, const char *dzpath, ExtendedStmt &stmt,
	DropzoneCallback &callback, Context &ctx)
{}


char *PerforceRepositoryImpl::dirname(const char *filename)
{
	const char *ls = strrchr(filename, '\\');
	if(!ls) {
		return NULL;
	}

	int len = ls - filename;

	char *ret = (char*) malloc(len + 1);
	strncpy(ret, filename, len);
	ret[len] = '\0';
	return ret;
}


Expr *PerforceRepositoryImpl::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "host") == 0) {
		return new Expr(m_host);
	} else if(strcmp(name, "port") == 0) {
		//char portstr[32];
		//sprintf(portstr, "%d", m_port);
		//return m_port ? strdup(portstr) : NULL;
		return m_port ? new Expr(m_port) : NULL;
	} else if(strcmp(name, "view") == 0) {
		return new Expr(m_view);
	}

	return RepositoryImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// PerforceRepositoryImplFactory
///////////////////////////////////////////////////////////////////////////////

PerforceRepositoryImplFactory::PerforceRepositoryImplFactory()
{}


RepositoryImpl *PerforceRepositoryImplFactory::create(
	DMINT32 implId, Repository &parent, ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr host    = stmt.getOverridableArgAsString("host", parent, true, ctx);
	ConstCharPtr portstr = stmt.getOverridableArgAsString("port", parent, false, ctx);
	ConstCharPtr view    = stmt.getOverridableAppendableArgAsString("view", '/', '/', parent, true, ctx);
	int port = 0;
	if(portstr) {
		port = atoi(portstr);
	}

	return new PerforceRepositoryImpl(*this, implId, parent, host, port, view);
}


///////////////////////////////////////////////////////////////////////////////

//extern "C" PERFORCE_EXPORT int Swarm_PluginStart(DM &dm);
//extern "C" PERFORCE_EXPORT int Swarm_PluginInstall(DM &dm);

extern "C" PERFORCE_EXPORT int PluginStart(DM &dm)
{
	dm.writeToLogFile("Perforce plugin V" PERFORCE_PLUGIN_VERSION);
	
	RepositoryImplRegistry::instance()
		.registerFactory(PERFORCE_REPO_NAME, new PerforceRepositoryImplFactory());

	//Swarm_PluginStart(dm);

	return 0;
}


extern "C" PERFORCE_EXPORT int PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("host", true, false ));
	propdefs->add(new PropertyDef("port", false, false));
	propdefs->add(new PropertyDef("view", true, true ));

	dm.installProviderImpl(PERFORCE_REPO_NAME, PERFORCE_PLUGIN_NAME, OBJ_KIND_REPOSITORY, propdefs);

	//Swarm_PluginInstall(dm);

	return 0;
}
