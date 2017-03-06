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
#include <direct.h>
#define RTC_EXPORT __declspec(dllexport)
#else
#define RTC_EXPORT
#endif /*WIN32*/

#include <stdio.h>
#include <sys/stat.h>

#include "rtc.h"

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


#define RTC_PLUGIN_NAME    "rtc"
#define RTC_REPO_NAME      "rtc"

#define RTC_PLUGIN_VERSION "1.00"

#define RTC_DEFAULT_PORT 1666


///////////////////////////////////////////////////////////////////////////////

char *rtc_getProperty(Repository &repo, const char *propname, Context &ctx)
{
	Property *exe = repo.getDefProperty(propname);
	if(!exe) {
		throw RuntimeError(ctx.stack(),
			"Missing provider definition property '%s' for provider '%s'",
			propname, repo.name());
	}

	// Expand any embedded variables
	Node nexe(NODE_STR, exe->decryptedValue(repo), true);
	ExprPtr eexe = nexe.evaluate(ctx);
	return eexe->stringify();
}


///////////////////////////////////////////////////////////////////////////////
// RTCRepositoryImpl
///////////////////////////////////////////////////////////////////////////////

RTCRepositoryImpl::RTCRepositoryImpl(
		RepositoryImplFactory &factory, DMINT32 implId, Repository &repository,
		const char *repositoryurl, const char *stream, const char *uuid
)
	: RepositoryImpl(factory, implId, repository),
	  m_repositoryurl(DUP_NULL(repositoryurl)),m_stream(DUP_NULL(stream)),m_uuid(DUP_NULL(uuid))
{
	debug1("RTCRepositoryImpl('%s')",
		NULL_CHECK(m_repositoryurl));
}


RTCRepositoryImpl::~RTCRepositoryImpl()
{
	SAFE_FREE(m_repositoryurl);
}


/**
 * Very simple test to make sure that the repository is correctly configured
 * with all the mandatory properties.  Just tests to make sure that "p4"
 * binary is available.
 */
bool RTCRepositoryImpl::testProvider(class Context &ctx)
{
	if(!m_repositoryurl) {
		ctx.writeToStdErr("\"repositoryurl\" property must be specified");
		return false;
	}

	ConstCharPtr lscm_exe = rtc_getProperty(m_repository, "lscm", ctx);

	struct stat buf;
	if(stat(lscm_exe, &buf) == -1) {
		ctx.writeToStdErr("Executable '%s' not found", (const char*) lscm_exe);
		return false;
	}

	return true;
}

void TraverseFiles(Context &ctx,char *dir)
{
#ifdef WIN32
	WIN32_FIND_DATA ffd;
	char *Pattern = (char *)malloc(strlen(dir)+10);
	sprintf(Pattern,"%s\\*",dir);
	HANDLE hFind = FindFirstFile(Pattern, &ffd);
	ctx.writeToStdOut("Pattern=[%s]",Pattern);
	do {
		ctx.writeToStdOut("ffd.cFileName=[%s]",ffd.cFileName);
		if (strcmp(ffd.cFileName,".") != 0 && strcmp(ffd.cFileName,"..") != 0) {
			ctx.writeToStdOut("looks like a file we're interested in");
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				//
				// Add the directory
				//
				char *newdir = (char *)malloc(strlen(dir)+strlen(ffd.cFileName)+10);
				sprintf(newdir,"%s\\%s",dir,ffd.cFileName);
				ctx.writeToStdOut("newdir=[%s]",newdir);
				// TraverseFiles(ctx,newdir);
				free(newdir);
			} else {
				ctx.writeToStdOut("filename=[%s\\%s]",dir,ffd.cFileName);
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	ctx.writeToStdOut("Finished recursing");
#endif
}


void RTCRepositoryImpl::checkout(
	const char *dzbase, const char *dzpath,
	bool recursive, StringList *pattern,
	ExtendedStmt &stmt, DropzoneCallback &callback,
	Context &ctx)
{
	// Dropzone: $TEMP/$DEPID.$DEPMAJOR
	// Viewpath: \ or whatever is specified on the repository
	// Username/Password: ???
	// ${repositoryurl}
    // ${user}
    // ${pass}
    // ${uuid}
    // ${filename}
	//


	Credentials *creds = m_repository.getCredentials();
	
	if (m_uuid) {
		// UUID has been set - must be a checkout of a build repository artefact
		ConstCharPtr ant_exe = rtc_getProperty(m_repository, "ant", ctx);
		ConstCharPtr antlib = rtc_getProperty(m_repository, "antlib", ctx);
		char *D_repositoryurl = (char *)malloc(strlen(m_repositoryurl)+50);
		sprintf(D_repositoryurl,"-Drepositoryurl=%s",m_repositoryurl);
		char *D_uuid = (char *)malloc(strlen(m_uuid)+50);
		sprintf(D_uuid,"-Duuid=%s",m_uuid);
		char *D_dzbase = (char *)malloc(strlen(dzbase)+50);
		sprintf(D_dzbase,"-Ddzbase=\"%s\"",dzbase);
		// buildxml file will be in DMHOME/bin
		char *buildxmlfile = (char *)malloc(strlen(ctx.dm().dmHome())+10);
		sprintf(buildxmlfile,"%s/bin/build.xml",ctx.dm().dmHome());
		if (!pattern) {
			// No pattern specified
			pattern = new StringList();
			pattern->add("*");	// temp
		}
		StringListIterator iter(*pattern);
		for (const char *patt = iter.first(); patt; patt = iter.next()) {
			char *D_filename= (char *)malloc(strlen(patt)+50);
			sprintf(D_filename,"-Dfilename=\"%s\"",patt);
			
			CmdLine cmd1(ant_exe);
			cmd1.add("-lib").add(antlib).add("-f").add(buildxmlfile).add(D_repositoryurl).add(D_uuid).add(D_filename).add(D_dzbase);
			if (creds && creds->hasUsername() && creds->hasPassword()) {
				char *username = creds->getDecryptedUsername(ctx);
				char *password = creds->getDecryptedPassword(ctx);
				char *D_username = (char *)malloc(strlen(username)+50);
				char *D_password = (char *)malloc(strlen(password)+50);
				sprintf(D_username,"-Duser=%s",username);
				sprintf(D_password,"-Dpass=%s",password);
				cmd1.add(D_username).add(D_password);
				SECURE_FREE(D_username);
				SECURE_FREE(D_password);
				SECURE_FREE(username);
				SECURE_FREE(password);
			}
			char *lscmcmd = cmd1.toCommandString(false);
			ctx.writeToStdOut("%s", lscmcmd);
			CapturedData *cd = NULL;
			int res1 = cmd1.exec(&cd,ctx);
			ctx.writeToStdOut("res1=%d",res1);
			ctx.writeToStdOut("cd->BytesOfStandardOut=%d",cd->bytesOfStandardOut());
			if (cd) {
				cd->appendStandardOut("\0", 1);
				cd->appendStandardErr("\0", 1);
				ctx.writeToStdOut("stdout=[%s]",cd->standardOut());
				ctx.writeToStdErr("stderr=[%s]",cd->standardErr());
			}
			if (res1 == -1) {
				throw RuntimeError(stmt, ctx.stack(),
						"Failed to run ANT extract");
			}
			callback.checked_out_folder("/", "\\", true);
			callback.checked_out_file(this, "/", patt, "1");	// version 1 for now until we figure out how to get that info
		}
	} else {
		// No UUID - must be a checkout from a stream

		ConstCharPtr lscm_exe = rtc_getProperty(m_repository, "lscm", ctx);
		

		char workspace[64];
		sprintf(workspace, "tdm_%d_%d", ctx.dm().deployId(), implId());

		//
		// Step 1 - login
		// --------------
		//
		CmdLine cmd1(lscm_exe);
		cmd1.add("login").add("-r").add(m_repositoryurl).add("-n").add("local");
		if (creds && creds->hasUsername() && creds->hasPassword()) {
			char *username = creds->getDecryptedUsername(ctx);
			char *password = creds->getDecryptedPassword(ctx);
			cmd1.add("-u").add(username).add("-P").add(password);
			SECURE_FREE(username);
			SECURE_FREE(password);
		}

		char *lscmcmd = cmd1.toCommandString(true);
		ctx.writeToStdOut("%s", lscmcmd);
		int res1 = cmd1.exec(ctx);
		if (res1 == -1) {
			throw RuntimeError(stmt, ctx.stack(),
					"Login to RTC repository failed");
		}

		//
		// Step 2 - Create Workspace
		// -------------------------
		//
		CmdLine cmd2(lscm_exe);
		cmd2.add("create").add("workspace").add("-r").add("local").add("-s").add(m_stream).add(workspace);
		lscmcmd = cmd2.toCommandString(true);
		ctx.writeToStdOut("%s", lscmcmd);
		int res2 = cmd2.exec(ctx);
		if (res2 == -1) {
			throw RuntimeError(stmt, ctx.stack(),
					"Failed to create Workspace");
		}

		//
		// Step 4 - Load the workspace
		// ---------------------------
		//
		CapturedData *cd = NULL;
		CmdLine cmd3(lscm_exe);
		cmd3.add("load").add("-d").add(dzbase).add("-r").add("local").add("--all").add(workspace);
		lscmcmd = cmd3.toCommandString(true);
		ctx.writeToStdOut("%s", lscmcmd);
		int res3 = cmd3.exec(&cd,ctx);
		if (cd) {
			cd->appendStandardOut("\0", 1);
			cd->appendStandardErr("\0", 1);
			ctx.writeToStdOut("stdout=[%s]",cd->standardOut());
			ctx.writeToStdErr("stderr=[%s]",cd->standardErr());
		}
		if (res3 == -1) {
			throw RuntimeError(stmt, ctx.stack(),
					"Failed to load Workspace");
		}

		//
		// Step 5 - Accept the changes
		// ---------------------------
		//
		CapturedData *cd2 = NULL;
		CmdLine cmd4(lscm_exe);
		cmd4.add("accept").add("-r").add("local").add("-o");
		lscmcmd = cmd4.toCommandString(true);
		ctx.writeToStdOut("%s", lscmcmd);
		int res4 = cmd4.exec(&cd2,ctx);
		if (cd2) {
			cd2->appendStandardOut("\0", 1);
			cd2->appendStandardErr("\0", 1);
			ctx.writeToStdOut("stdout=[%s]",cd2->standardOut());
			ctx.writeToStdErr("stderr=[%s]",cd2->standardErr());
		}
		if (res4 == -1) {
			throw RuntimeError(stmt, ctx.stack(),
					"Failed to accept Workspace");
		}

		//
		// Step 6 - Walk through the newly loaded direcotry and find files
		// ---------------------------------------------------------------
		//
		TraverseFiles(ctx,(char *)dzbase);

		//
		// Step 7 - Delete Workspace
		// -------------------------
		//
		CmdLine cmd7(lscm_exe);
		cmd7.add("delete").add("workspace").add(workspace);
		cmd7.exec(ctx);
		
		//
		// Step 8 - Logout
		// ---------------
		//
		CmdLine cmd8(lscm_exe);
		cmd8.add("logout").add("-r").add(m_repositoryurl);
		cmd8.exec(ctx);
	}


#ifdef THIS_IS_NOT_DEFINED
	free(pocmd);
	CapturedData *cd = NULL;
	int res = cmd2.exec(&cd, ctx);

	CmdLine cmd3(p4_exe);
	cmd3.add("-p").add(p4port).add("client").add("-d").add(workspace);
	pocmd = cmd3.toCommandString(true);
	/*debug1*/ ctx.writeToStdOut("%s", pocmd);
	free(pocmd);
	int res2 = cmd3.exec(ctx);

	if(res == -1) {
		throw RuntimeError(stmt, ctx.stack(),
				"Command did not execute successfully");
	}

	// Record viewpath folders up front, as these all map to the info file for the repository
	//callback.checked_out_folder("\\", "\\", true);
	callback.checked_out_folder(m_view, "\\", true);

	if(cd) {
		cd->appendStandardOut("\0", 1);

		// //streamsDepot/mainline/dmdemo/scripts/applysql.bat#1 - added as C:\cygwin\tmp\tdm.547.rtc\scripts\applysql.bat
		MessageMatcher mm("msgtext");
		mm.setRegexp("^([a-zA-Z0-9/._]*)#([0-9]*) - added as ([a-zA-Z0-9/\\._:]*)$");
		mm.addMapping(1, "dp");
		mm.addMapping(2, "rv");
		mm.addMapping(3, "cp");

		char *temp = NULL;
		for(char *line = STRTOK_R((char*) cd->standardOut(), "\r\n", &temp);
			line; line = STRTOK_R(NULL, "\r\n", &temp)) {
			AutoPtr<StringHashtable> matches = mm.match(line);
			if(matches) {
				const char *dp = matches->get("dp");
				const char *rv = matches->get("rv");
				const char *cp = matches->get("cp");
				/*debug3*/ ctx.writeToStdOut("%s;%s -> %s", dp, rv, cp);

				const char *pn = cp ? strstr(cp, dzbase) : NULL;
				if(pn) {
					pn += strlen(dzbase);
					if(*pn == DIR_SEP_CHAR) {
						pn++;
					}
					callback.checked_out_file(this, dp, pn, rv);
					
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
#endif
}


void RTCRepositoryImpl::checkin(
	const char *dzbase, const char *dzpath, ExtendedStmt &stmt,
	DropzoneCallback &callback, Context &ctx)
{}


char *RTCRepositoryImpl::dirname(const char *filename)
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


Expr *RTCRepositoryImpl::getAttribute(const char *name, Context &ctx)
{
	return RepositoryImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// RTCRepositoryImplFactory
///////////////////////////////////////////////////////////////////////////////

RTCRepositoryImplFactory::RTCRepositoryImplFactory()
{}


RepositoryImpl *RTCRepositoryImplFactory::create(
	DMINT32 implId, Repository &parent, ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr repositoryurl = stmt.getOverridableArgAsString("repositoryurl", parent, true, ctx);
	ConstCharPtr stream = stmt.getOverridableArgAsString("stream", parent, false, ctx);
	ConstCharPtr uuid = stmt.getOverridableArgAsString("uuid", parent, false, ctx);

	return new RTCRepositoryImpl(*this, implId, parent, repositoryurl, stream, uuid);
}


///////////////////////////////////////////////////////////////////////////////

extern "C" RTC_EXPORT int PluginStart(DM &dm)
{
	dm.writeToLogFile("RTC plugin V" RTC_PLUGIN_VERSION);
	
	RepositoryImplRegistry::instance()
		.registerFactory(RTC_REPO_NAME, new RTCRepositoryImplFactory());

	//Swarm_PluginStart(dm);

	return 0;
}


extern "C" RTC_EXPORT int PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("repositoryurl", true, false ));
	propdefs->add(new PropertyDef("stream", false, false ));
	propdefs->add(new PropertyDef("uuid", false, false ));

	dm.installProviderImpl(RTC_REPO_NAME, RTC_PLUGIN_NAME, OBJ_KIND_REPOSITORY, propdefs);

	return 0;
}
