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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "transfer.h"
#include "model.h"
#include "cmdline.h"
#include "exceptions.h"
#include "context.h"
#include "dm.h"
#include "expr.h"	// For CharPtr only
#include "execute.h"
#include "tinyxml.h"
#include "properties.h"
#include "pathname.h"
#ifndef WIN32
#include <pthread.h>
#endif
#include "thread.h"
#include "node.h"

#ifndef WIN32
#define stricmp strcasecmp
#endif


class RtiTransferProviderImpl : public virtual TransferProviderImpl
{
private:
	const char *m_protocol;
	Server &m_target;
	Environment *m_tgtenv;
	Credentials *m_credentials;
	Component *m_comp;
	char *m_targetPath;

public:
	RtiTransferProviderImpl(
		const char *protocol, Server &target,
		Environment *tgtenv, Credentials *creds,
		Component *comp, const char *targetPath);
	virtual ~RtiTransferProviderImpl();

	void deleteFromServer(Component &comp,Context &ctx);
	List<TransferResult> *transferToServer(const char *dropzone,  bool ServerSpecific, Context &ctx);

	void transferFromServer(const char *dropzone, Context &ctx);

	int runScriptOnServer(
		const char *shell, const char *script, class StringList *params,
		bool copy, bool useTTY, Context &ctx); 

	int checkServer(Context &ctx);
};


class RtiTransferProviderImplFactory : public virtual TransferProviderImplFactory
{
private:
	const char *m_protocol;

public:
	RtiTransferProviderImplFactory(const char *protocol);

	TransferProviderImpl *create(
		Server &target, Environment *tgtenv, Credentials *creds,
		Component *comp, const char *targetPath);
};



///////////////////////////////////////////////////////////////////////////////
// TransferResult
///////////////////////////////////////////////////////////////////////////////

TransferResult::TransferResult()
	: m_sourcedir(NULL), m_targetdir(NULL), m_filename(NULL),
	  m_error(NULL), m_size(-1), m_md5(NULL), m_istext(false),
	  m_dzfile(NULL)
{}


TransferResult::~TransferResult()
{
	SAFE_FREE(m_sourcedir);
	SAFE_FREE(m_targetdir);
	SAFE_FREE(m_filename);
	SAFE_FREE(m_error);
	SAFE_FREE(m_md5);
	m_dzfile = NULL;	// owned by dropzone
}


void TransferResult::setProperty(const char* prop, const char *value)
{
	if(strcmp(prop, "sourcedir") == 0) {
		m_sourcedir = DUP_NULL(value);
	} else if(strcmp(prop, "targetdir") == 0) {
		m_targetdir = DUP_NULL(value);
	} else if(strcmp(prop, "filename") == 0) {
		m_filename = DUP_NULL(value);
	} else if(strcmp(prop, "error") == 0) {
		if(!m_error) {
			m_error = DUP_NULL(value);
		}
	} else if(strcmp(prop, "errortext") == 0) {
		m_error = DUP_NULL(value);
	} else if(strcmp(prop, "size") == 0) {
		m_size = value ? ATOINT32(value) : -1;
	} else if(strcmp(prop, "md5") == 0) {
		m_md5 = DUP_NULL(value);
	} else if(strcmp(prop, "mode") == 0) {
		m_istext = (value && (STRCASECMP(value, "Text") == 0)) ? true : false;
	} else {
		debug1("Unknown transfer property '%s'", prop);
	}
}


char *TransferResult::targetFilename()
{
	if(!m_filename) {
		return NULL;
	}
	if(!m_targetdir) {
		return strdup(m_filename);
	}

	char *ret = (char*) malloc(strlen(m_targetdir) + strlen(m_filename) + 2);
	sprintf(ret, "%s/%s", m_targetdir, m_filename);
	return ret;
}


char *TransferResult::dropzoneFilename()
{
	if(!m_filename) {
		return NULL;
	}
	if(!m_sourcedir) {
		return strdup(m_filename);
	}

	char *ret = (char*) malloc(strlen(m_sourcedir) + strlen(m_filename) + 2);
	sprintf(ret, "%s%s%s", m_sourcedir, DIR_SEP_STR, m_filename);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////
// TransferProviderImpl
///////////////////////////////////////////////////////////////////////////////

/*virtual*/ TransferProviderImpl::~TransferProviderImpl()
{}


///////////////////////////////////////////////////////////////////////////////
// TransferProviderImplFactory
///////////////////////////////////////////////////////////////////////////////

/*virtual*/ TransferProviderImplFactory::~TransferProviderImplFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// TransferProviderImplRegistry
///////////////////////////////////////////////////////////////////////////////

TransferProviderImplRegistry *TransferProviderImplRegistry::s_instance = NULL;


TransferProviderImplRegistry::TransferProviderImplRegistry()
	: m_factories(false, true)
{}


/*static*/ TransferProviderImplRegistry &TransferProviderImplRegistry::instance()
{
	if(!s_instance) {
		s_instance = new TransferProviderImplRegistry();
	}
	return *s_instance;
}


/*static*/ void TransferProviderImplRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void TransferProviderImplRegistry::registerFactory(const char *name, TransferProviderImplFactory *factory)
{
	m_factories.put(name, factory);
}


void TransferProviderImplRegistry::registerBuiltIns()
{
	registerFactory("ftp",  new RtiTransferProviderImplFactory("ftp"));
	registerFactory("ftps", new RtiTransferProviderImplFactory("ftps"));
	registerFactory("sftp", new RtiTransferProviderImplFactory("sftp"));
	registerFactory("win",  new RtiTransferProviderImplFactory("win"));
}


TransferProviderImplFactory *TransferProviderImplRegistry::getFactory(const char *name)
{
	return m_factories.get(name);
}


///////////////////////////////////////////////////////////////////////////////
// RtiTransferProviderImpl
///////////////////////////////////////////////////////////////////////////////

RtiTransferProviderImpl::RtiTransferProviderImpl(
	const char *protocol, Server &target, Environment *tgtenv,
	Credentials *creds, Component *comp, const char *targetPath
	)
	: m_protocol(protocol), m_target(target), m_tgtenv(tgtenv),
	  m_credentials(creds), m_comp(comp), m_targetPath(DUP_NULL(targetPath))
{}


/*virtual*/ RtiTransferProviderImpl::~RtiTransferProviderImpl()
{
	SAFE_FREE(m_targetPath);
}

void RtiTransferProviderImpl::deleteFromServer(Component &comp,Context &ctx)
{
	debug3("Removing files associated with the last deployment of component %s to server %s",comp.name(),m_target.name());
	//
	// Create a temporary file containing a list of files to remove on this server
	//
	char *listfile = NULL;
	FILE *out = ctx.dm().createTemporaryFile(&listfile);
	debug3("out = 0x%lx Temp File: %s", out, listfile);
	if (out) {
		// Create a list of files and add them to the temporary file
		StringList *filelist = comp.getFilesForLastDeployment(m_target,ctx.dm().deployId());
		if (filelist->size() > 0) {
			StringListIterator iter(*filelist);
			for(const char *pathname = iter.first(); pathname; pathname = iter.next()) {
				debug3("Removing File [%s]", pathname);
				fprintf(out,"%s\n",pathname);
		}
			fclose(out);
	}
	} else {
		// Problem creating temporary file
	}
	char dmtransfer[1024];
	sprintf(dmtransfer, "%s%slib%sdmtransfer%s",
		ctx.dm().dmHome(), DIR_SEP_STR, DIR_SEP_STR, EXE_EXT);

	CmdLine cmd(dmtransfer);
	if (strcmp(m_target.hostname(),"localhost") ==0)
	{
#ifdef WIN32
		cmd.add("-protocol").add("win").add("-server").add(m_target.hostname());		
#else 
		cmd.add("-protocol").add("sftp").add("-server").add(m_target.hostname());
#endif					
	}
	else
	{
 	cmd.add("-protocol").add(m_protocol).add("-server").add(m_target.hostname());		
	}

	if (strcmp(m_protocol,"sftp")==0 && m_target.getSSHPort() != 22) {
		// Default port number has changed
		char szPortNum[128];
		sprintf(szPortNum,"%d",m_target.getSSHPort());
		cmd.add("-port").add(szPortNum);
	}
	if (m_credentials) {
		switch(m_credentials->credkind()) {
		case CREDENTIALS_ENCRYPTED:
		case CREDENTIALS_IN_DATABASE:
		case CREDENTIALS_FROM_VARS: {
			// decrypt always allocates memory
			char *username = m_credentials->getDecryptedUsername(ctx); // was m_tgtenv PAG 18/02/2015
			char *password = m_credentials->getDecryptedPassword(ctx); // was m_tgtenv PAG 18/02/2015
			cmd.add("-user").add(username).add("-password").add(password);
			SECURE_FREE(username);
			SECURE_FREE(password);
			}
			break;
		case CREDENTIALS_PRIVATE_KEY: {
			char *username = m_credentials->getDecryptedUsername(ctx);
			char *filename = (char *)m_credentials->filename(ctx);
			cmd.add("-user").add(username).add("-keyfile").add(filename);
			SECURE_FREE(username);
			SECURE_FREE(filename);
			}
		    break;
		default:
			throw RuntimeError(ctx.stack(), "Credentials kind %d not yet implemented", m_credentials->kind());
		}
	}
	cmd.add("-dellist").add(listfile);
	// Lock on the hostname of the target server
	debug1("%s\n",cmd.toCommandString());
	ctx.dm().writeToStdOut("INFO: Removing Component %s from Server %s",comp.name(),m_target.name());
	Thread::lock(__LINE__,__FILE__,m_target.hostname());

	CapturedData *cd = NULL;
	int tempExitStatus;
	int ret = executeAndCapture(ctx.stream(), cmd, NULL, false, ctx.threadId(), &tempExitStatus, &cd, NULL);
	debug1("dmtransfer complete, ret=%d tempexitstatus=%d",ret,tempExitStatus);

	// Unlock again
	Thread::unlock(__LINE__,__FILE__,m_target.hostname());

	if(ret || tempExitStatus) {
		// Get any stderr output
		if(cd) {
			char *temp = NULL;
			cd->appendStandardErr("\0", 1);
			for(char *eline = STRTOK_R((char*) cd->standardErr(), "\r\n", &temp);
				eline; eline = STRTOK_R(NULL, "\r\n", &temp)) {
				ctx.dm().writeToStdErr(eline);
			}
		}
		SAFE_DELETE(cd);
		throw RuntimeError(ctx.stack(), "dmtransfer failed when removing files");
	}
	else
	{
		// Successful completion - rattle through the standard output and write to log
		if(cd) {
			char *temp = NULL;
			cd->appendStandardOut("\0", 1);
			for(char *eline = STRTOK_R((char*) cd->standardOut(), "\r\n", &temp);
				eline; eline = STRTOK_R(NULL, "\r\n", &temp)) {
				ctx.dm().writeToStdOut(eline);
			}
		}
		SAFE_DELETE(cd);
		//
		// Now write the DELETED entries into dm_deploymentxfer
		//

	}
}

List<TransferResult> *RtiTransferProviderImpl::transferToServer(const char *dropzone, bool ServerSpecific, Context &ctx)
{
	// Dropzone: $TEMP/$DEPID.$DEPMAJOR
	// Targetpath: whatever is specified on the statement, server or environment
	// Username/Password: ???

	ConstCharPtr basedir = DUP_NULL(m_target.basedir());
	if(!basedir && m_tgtenv) {
		basedir = DUP_NULL(m_tgtenv->basedir());
	}
	

	if(!basedir) {
		throw RuntimeError(ctx.stack(), "basedir not specified for server '%s'", m_target.name());
	}

	

	// Now examine the component base directory
	if(m_comp && m_comp->basedir()) {
		PathNameImpl *compPath = m_target.createPath(m_comp->basedir());
		if(!compPath) {
			throw RuntimeError(ctx.stack(), "Failed to create path for server '%s'", m_target.name()); 
		}
		if(compPath->isAbsolute()) {
			// Component basedir is absolute - override server/env basedir
			basedir = DUP_NULL(m_comp->basedir());
			debug1("overridden basedir = '%s'", (const char*) basedir);
		} else {
			// Append the component basedir to server/env basedir
			AutoPtr<PathNameImpl> basedirPath = m_target.createPath(basedir);
			if(!basedirPath) {
				throw RuntimeError(ctx.stack(), "Failed to create path for server '%s'", m_target.name()); 
			}
			AutoPtr<PathNameImpl> appendedPath = basedirPath->append(m_comp->basedir());
			basedir = DUP_NULL(appendedPath->path());
			debug1("appended basedir = '%s' (1)", (const char*) basedir);
		}
	}

	// Now append any target path specified on a single deploy or transfer statement
	if(m_targetPath) {
		AutoPtr<PathNameImpl> basedirPath = m_target.createPath(basedir);
		if(!basedirPath) {
			throw RuntimeError(ctx.stack(), "Failed to create path for server '%s'", m_target.name()); 
		}
		AutoPtr<PathNameImpl> appendedPath = basedirPath->append(m_targetPath);
		basedir = DUP_NULL(appendedPath->path());
		debug1("appended basedir = '%s' (2)", (const char*) basedir);
	}

	//dmtransfer -protocol sftp -sourcedir "C:\development\Project Manager" -targetdir /tmp/testing -user phil -password ncc1701 -server 192.168.8.150

	// Expand the base directory
	char *btext = strdup(basedir);
	Node expandedText2(NODE_STR, btext, true);	// will free btext on delete
	ExprPtr etext2 = expandedText2.evaluate(ctx);
	ConstCharPtr btext2 = etext2->stringify();
	char *btext3 = strdup(btext2);
	int i=0;
	int len=0;

	if (strcmp(m_target.hostname(),"localhost") ==0)
	{ 
		if (btext3 != NULL)
		{
		 #ifdef WIN32
			 len = strlen(btext3);
			 for (i=0;i<len;i++)
				{
					if (btext3[i] == '/')
						btext3[i] = '\\';
				}
			#else
			 len = strlen(btext3);
			 for (i=0;i<len;i++)
				{
					if (btext3[i] == '\\')
						btext3[i] = '/';
				}				
			#endif		
		}
	}
	debug1("basedir for server '%s' is '%s'", m_target.name(),
		((const char*) btext3 ? (const char*)  btext3 : "(null)"));

	char dmtransfer[1024];
	sprintf(dmtransfer, "%s%slib%sdmtransfer%s",
		ctx.dm().dmHome(), DIR_SEP_STR, DIR_SEP_STR, EXE_EXT);

	CmdLine cmd(dmtransfer);
	if (strcmp(m_target.hostname(),"localhost") ==0)
	{
#ifdef WIN32
	 cmd.add("-protocol").add("win").add("-sourcedir").add(dropzone).add("-targetdir").add(btext3);
#else 
	 cmd.add("-protocol").add("sftp").add("-sourcedir").add(dropzone).add("-targetdir").add(btext3);
#endif					
	}
	else
	{
	 cmd.add("-protocol").add(m_protocol).add("-sourcedir").add(dropzone).add("-targetdir").add(btext3);
	}
	if (strcmp(m_protocol,"sftp")==0 && m_target.getSSHPort() != 22) {
		// Change to default port number
		char szPortNum[128];
		sprintf(szPortNum,"%d",m_target.getSSHPort());
		cmd.add("-port").add(szPortNum);
	}

	if (m_credentials) {
		switch(m_credentials->credkind()) {
		case CREDENTIALS_ENCRYPTED:
		case CREDENTIALS_IN_DATABASE:
		case CREDENTIALS_FROM_VARS: {
			// decrypt always allocates memory
			char *username = m_credentials->getDecryptedUsername(ctx); // was m_tgtenv PAG 18/02/2015
			char *password = m_credentials->getDecryptedPassword(ctx); // was m_tgtenv PAG 18/02/2015
			cmd.add("-user").add(username).add("-password").add(password);
			SECURE_FREE(username);
			SECURE_FREE(password);
			}
			break;
		case CREDENTIALS_PRIVATE_KEY: {
			char *username = m_credentials->getDecryptedUsername(ctx);
			char *filename = (char *)m_credentials->filename(ctx);
			cmd.add("-user").add(username).add("-keyfile").add(filename);
			SECURE_FREE(username);
			SECURE_FREE(filename);
			}
		    break;
		default:
			throw RuntimeError(ctx.stack(), "Credentials kind %d not yet implemented", m_credentials->kind());
		}
	}

	if(!m_target.serverType() || !m_target.serverType()->id()) {
		ctx.writeToStdOut("WARNING: Server '%s' has no server type set", m_target.name());
	}

	LINE_END_FORMAT lineends = m_target.lineends();
	if (strcmp(m_target.hostname(),"localhost") ==0)
	{
#ifdef WIN32
	  switch(lineends) {
	  case LINE_ENDS_OFF:     cmd.add("-target").add("off");  debug2("lineends = OFF"); break;
	  case LINE_ENDS_UNIX:    cmd.add("-target").add("win");  debug2("lineends = UNIX"); break;
	  case LINE_ENDS_WINDOWS: cmd.add("-target").add("win");  debug2("lineends = WIN"); break;
	  case LINE_ENDS_MAC:     cmd.add("-target").add("mac");  debug2("lineends = MAC"); break;
	 }	
#else 
	  switch(lineends) {
	  case LINE_ENDS_OFF:     cmd.add("-target").add("off");  debug2("lineends = OFF"); break;
	  case LINE_ENDS_UNIX:    cmd.add("-target").add("unix"); debug2("lineends = UNIX"); break;
	  case LINE_ENDS_WINDOWS: cmd.add("-target").add("unix"); debug2("lineends = WIN"); break;
	  case LINE_ENDS_MAC:     cmd.add("-target").add("mac");  debug2("lineends = MAC"); break;
  }
#endif					
	}
	else
	{
	 switch(lineends) {
	 case LINE_ENDS_OFF:     cmd.add("-target").add("off");  debug2("lineends = OFF"); break;
	 case LINE_ENDS_UNIX:    cmd.add("-target").add("unix"); debug2("lineends = UNIX"); break;
	 case LINE_ENDS_WINDOWS: cmd.add("-target").add("win");  debug2("lineends = WIN"); break;
	 case LINE_ENDS_MAC:     cmd.add("-target").add("mac");  debug2("lineends = MAC"); break;
	 }
 }

	cmd.add("-server").add(m_target.hostname());

	/**/char *pocmd = cmd.toCommandString(true);
	/**/debug1("%s", pocmd);

	//int res = pclose(popen(pocmd, "w"));
	/**/free(pocmd);

	//if(res == -1) {
	//	throw RuntimeError(ctx.stack(), "Command did not execute successfully");
	//}

	// Lock on the hostname of the target server
	Thread::lock(__LINE__,__FILE__,m_target.hostname());

	if (m_comp) {
		if (ServerSpecific) {
			ctx.dm().writeToStdOut("INFO: Deploying Server Specific files to Server %s",m_target.name());
		} else {
			ctx.dm().writeToStdOut("INFO: Deploying Component %s to Server %s",m_comp->name(),m_target.name());
		}
	}
	//char *dbg = cmd.toCommandString(true);
	//printf("%s\n",dbg);

	CapturedData *cd = NULL;
	int tempExitStatus;
	int ret = executeAndCapture(ctx.stream(), cmd, NULL, false, ctx.threadId(), &tempExitStatus, &cd, NULL);
	debug1("dmtransfer complete, ret=%d",ret);

	// Unlock again
	Thread::unlock(__LINE__,__FILE__,m_target.hostname());

	if(ret || tempExitStatus) {
		// Get any stderr output
		if(cd) {
			char *temp = NULL;
			cd->appendStandardErr("\0", 1);
			for(char *eline = STRTOK_R((char*) cd->standardErr(), "\r\n", &temp);
				eline; eline = STRTOK_R(NULL, "\r\n", &temp)) {
				ctx.dm().writeToStdErr(eline);
			}
		}
		//ctx.dm().writeToStdOut("dmtransfer failed to execute");
		SAFE_DELETE(cd);
		throw RuntimeError(ctx.stack(), "dmtransfer did not execute successfully");
	}
	debug1("tempExitStatus = %d", tempExitStatus);

	if(cd) {
		char *temp = NULL;
		cd->appendStandardErr("\0", 1);
		for(char *eline = STRTOK_R((char*) cd->standardErr(), "\r\n", &temp);
			eline; eline = STRTOK_R(NULL, "\r\n", &temp)) {
			printf("ERROR: %s\n", eline);
		}

		cd->appendStandardOut("\0", 1);
		CapturedData *xml = new CapturedData();
		temp = NULL;
		for(char *line = STRTOK_R((char*) cd->standardOut(), "\r\n", &temp);
			line; line = STRTOK_R(NULL, "\r\n", &temp)) {
				char *x;
				for(x = line; *x && ((*x == ' ') || (*x == '\t')); x++) { /*empty*/ }
				if(*x == '<') {
					// printf("XML: %s\n",line);
					xml->appendStandardOut(line, strlen(line));
				} else {
					printf("INFO: %s\n", line);
				}
		}

		xml->appendStandardOut("\0", 1);
		//ctx.dm().writeBufferToStdOut(ctx.threadId(), xml->standardOut(), xml->bytesOfStandardOut());
		//ctx.dm().writeToStdOut("");

		List<TransferResult> *ret = new List<TransferResult>(true);

		TiXmlDocument doc;
		if (xml->standardOut()[0]) {
			doc.Parse(xml->standardOut());

			if(!doc.Error()) {
				TiXmlElement *root = doc.FirstChildElement("files");
				if(root) {
					for(TiXmlElement *file = root->FirstChildElement("file"); file; file = file->NextSiblingElement("file")) {
						TransferResult *result = new TransferResult();
						TiXmlElement *tgtdir = file->FirstChildElement("targetdir");
						TiXmlElement *tgtfile = file->FirstChildElement("filename");
						TiXmlElement *error = file->FirstChildElement("error");
						TiXmlElement *errortext = file->FirstChildElement("errortext");
						const char *szDir = tgtdir?tgtdir->GetText():(char *)0;
						const char *szFile = tgtfile?tgtfile->GetText():(char *)0;
						const char *szError = error?error->GetText():"N";
						const char *szErrorText = errortext?errortext->GetText():"";
						if (szDir && szFile) {
							ServerType *st = m_target.serverType();
							char sepchr = (st && stricmp(st->hosttype(),"windows")==0)?'\\':'/';
							// Let's get the paths consistent
							char *tgtpath = (char *)malloc(strlen(szDir)+strlen(szFile)+10);
							sprintf(tgtpath,"%s%c%s",szDir,sepchr,szFile);
							char *p=tgtpath;
							while (*p) {
								if (*p=='\\' || *p=='/') *p=sepchr;
								p++;
							}
							if (szError[0]=='Y') {
								ctx.dm().writeToStdErr("ERROR: Failed to Deploy File %s -> %s:%s: %s",szFile,m_target.name(),tgtpath);
							} else {
								ctx.dm().writeToStdOut("INFO: Deployed File %s -> %s:%s",szFile,m_target.name(),tgtpath);
							}
							free(tgtpath);
						}

						for(TiXmlElement *node = file->FirstChildElement(); node; node = node->NextSiblingElement()) {
							result->setProperty(node->Value(), node->GetText());
						}
						if (szError[0]!='Y') {
							ret->add(result);
						}
					}
				} else {
					ctx.dm().writeToStdOut("root not found");
				}
			} else {
				ctx.dm().writeToStdOut("error parsing dmtransfer XML");
				ctx.dm().writeToStdOut(xml->standardOut());
				ctx.dm().writeToStdOut("---");
			}
		}

		SAFE_DELETE(xml);
		SAFE_DELETE(cd);
		return ret;
	}

	return NULL;
}


void RtiTransferProviderImpl::transferFromServer(const char *dropzone, Context &ctx)
{
	// TODO: Implement checkin transfer
	throw RuntimeError(ctx.stack(), "NOT YET IMPLEMENTED!");
}

int RtiTransferProviderImpl::checkServer(Context &ctx)
{
	char dmtransfer[1024];
	sprintf(dmtransfer, "%s%slib%sdmtransfer%s",
		ctx.dm().dmHome(), DIR_SEP_STR, DIR_SEP_STR, EXE_EXT);

	CmdLine cmd(dmtransfer);
	//RTOEXECP2:cmd.add("-m").add(m_target.hostname()).add("-syn").add("-force");
	if (strcmp(m_target.hostname(),"localhost") ==0)
	{
#ifdef WIN32
	 cmd.add("-protocol").add("win").add("-server").add(m_target.hostname());
#else 
	 cmd.add("-protocol").add("sftp").add("-server").add(m_target.hostname());
#endif					
	}
	else
	{
	 cmd.add("-protocol").add(m_protocol).add("-server").add(m_target.hostname());
 }
	if (strcmp(m_protocol,"sftp")==0 && m_target.getSSHPort()!=22) {
		// Change to default port number
		char szPortNum[128];
		sprintf(szPortNum,"%d",m_target.getSSHPort());
		cmd.add("-port").add(szPortNum);
	}

	if (m_credentials) {
		switch(m_credentials->credkind()) {
		case CREDENTIALS_ENCRYPTED:
		case CREDENTIALS_IN_DATABASE:
		case CREDENTIALS_FROM_VARS: {
			// decrypt always allocates memory
			char *username = m_credentials->getDecryptedUsername(ctx); // was m_tgtenv PAG 18/02/2015
			char *password = m_credentials->getDecryptedPassword(ctx); // was m_tgtenv PAG 18/02/2015
			cmd.add("-user").add(username).add("-password").add(password);
			SECURE_FREE(username);
			SECURE_FREE(password);
			}
			break;
		case CREDENTIALS_PRIVATE_KEY: {
			char *username = m_credentials->getDecryptedUsername(ctx);
			char *filename = (char *)m_credentials->filename(ctx); 
			cmd.add("-user").add(username).add("-keyfile").add(filename);
			SECURE_FREE(username);
			SECURE_FREE(filename);
			}
			break;
		default:
			throw RuntimeError(ctx.stack(), "Credentials kind %d not yet implemented", m_credentials->kind());
		}
	}
	cmd.add("-checkbase");

	const char *bdir = m_target.basedir();
	if (bdir) {
		char *ptext = strdup(bdir);
		Node expandedText(NODE_STR, ptext, true);	// will free btext on delete
		ExprPtr etext = expandedText.evaluate(ctx);
		ConstCharPtr ptext2 = etext->stringify();
		char *btext3 = strdup(ptext2);
		int i=0;
		int len=0;

		if (strcmp(m_target.hostname(),"localhost") ==0)
		{ 
			if (btext3 != NULL)
			{
			 #ifdef WIN32
				 len = strlen(btext3);
				 for (i=0;i<len;i++)
					{
						if (btext3[i] == '/')
							btext3[i] = '\\';
					}
				#else
				 len = strlen(btext3);
				 for (i=0;i<len;i++)
					{
						if (btext3[i] == '\\')
							btext3[i] = '/';
					}				
				#endif		
			}
		}
		cmd.add(btext3);
	}
	debug1("Executing %s",cmd.toCommandString());
	Thread::lock(__LINE__,__FILE__,m_target.hostname());
	CapturedData *cd = NULL;
	int ExitStatus = -1;
	int ret = executeAndCapture(ctx.stream(), cmd, NULL, true, ctx.threadId(), &ExitStatus, &cd, NULL);
	debug1("ExitStatus=%d",ExitStatus);
	Thread::unlock(__LINE__,__FILE__,m_target.hostname());
	// printf("ret=%d tempExitStatus=%d\n",ret,ExitStatus);
	return (ret == 0)?ExitStatus:ret;
}

int RtiTransferProviderImpl::runScriptOnServer(
	const char *shell, const char *script, StringList *params, bool copy, bool useTTY, Context &ctx)
{
	// dmtransfer -protocol xxx -server mmm -user uuu -password ppp -exec ccc aaa aaa
	// dmtransfer -protocol xxx -server mmm -user uuu -password ppp -copyexec ccc aaa aaa

	char dmtransfer[1024];
	sprintf(dmtransfer, "%s%slib%sdmtransfer%s",
		ctx.dm().dmHome(), DIR_SEP_STR, DIR_SEP_STR, EXE_EXT);

	CmdLine cmd(dmtransfer);
	//RTOEXECP2:cmd.add("-m").add(m_target.hostname()).add("-syn").add("-force");
	if (strcmp(m_target.hostname(),"localhost") ==0)
	{
#ifdef WIN32
 	cmd.add("-protocol").add("win").add("-server").add(m_target.hostname());
#else 
 	cmd.add("-protocol").add("sftp").add("-server").add(m_target.hostname());
#endif					
	}
	else
	{
 	cmd.add("-protocol").add(m_protocol).add("-server").add(m_target.hostname());
 }
	if (strcmp(m_protocol,"sftp")==0 && m_target.getSSHPort()!=22) {
		char szPortNum[128];
		sprintf(szPortNum,"%d",m_target.getSSHPort());
		cmd.add("-port").add(szPortNum);
	}

	if (useTTY) {
		cmd.add("-tty");
	}

	if (m_credentials) {
		switch(m_credentials->credkind()) {
		case CREDENTIALS_ENCRYPTED:
		case CREDENTIALS_IN_DATABASE:
		case CREDENTIALS_FROM_VARS: {
			// decrypt always allocates memory
			char *username = m_credentials->getDecryptedUsername(ctx); // was m_tgtenv PAG 18/02/2015
			char *password = m_credentials->getDecryptedPassword(ctx); // was m_tgtenv PAG 18/02/2015
			//RTOEXECP2:cmd.add("-usr").add(username).add("-pw").add(password);
			cmd.add("-user").add(username).add("-password").add(password);
			SECURE_FREE(username);
			SECURE_FREE(password);
			}
			break;
		case CREDENTIALS_PRIVATE_KEY: {
			char *username = m_credentials->getDecryptedUsername(ctx);
			char *filename = (char *)m_credentials->filename(ctx);
			cmd.add("-user").add(username).add("-keyfile").add(filename);
			SECURE_FREE(username);
			SECURE_FREE(filename);
			}
			break;
		default:
			throw RuntimeError(ctx.stack(), "Credentials kind %d not yet implemented", m_credentials->kind());
		}
	}

	cmd.add(copy ? "-copyexec" : "-exec");

	if(shell) {
		// PAG 04/05/2015 - shell now preceeded with -shell switch to allow copyexec
		// to copy script and have it interpreted with different front end
		cmd.add("-shell");
		cmd.add(shell);
	}
	cmd.add(script);

	if(params) {
		StringListIterator iter(*params);
		for(const char *p = iter.first(); p; p = iter.next()) {
			//
			// We will expand again here. This may because something like ${server.basedir)
			// may well have other variables in it.
			//
			// ctx.dm().writeToStdOut("p=%s",p);
			char *ptext = strdup(p);
			Node expandedText(NODE_STR, ptext, true);	// will free btext on delete
			ExprPtr etext = expandedText.evaluate(ctx);
			ConstCharPtr ptext2 = etext->stringify();
			// cmd.add(p);
			// ctx.dm().writeToStdOut("ptext2=%s",ptext2);
			cmd.add(ptext2);
		}
	}

	debug1("%s", cmd.toCommandString(false));

	bool bShowOutput = true;

	// Lock on the hostname of the target server
	Thread::lock(__LINE__,__FILE__,m_target.hostname());
	// Thread::lock(__LINE__,__FILE__,m_target.name());

	CapturedData *cd = NULL;
	int tempExitStatus = 0;
	int ret = executeAndCapture(ctx.stream(), cmd, NULL, bShowOutput, ctx.threadId(), &tempExitStatus, &cd, NULL);
	if (ret == 0 && tempExitStatus < 0) tempExitStatus=0;	// getting negative results from some successful executions, not sure why

	// Unlock again
	Thread::unlock(__LINE__,__FILE__,m_target.hostname());
	// Thread::lock(__LINE__,__FILE__,m_target.name());

	if(ret) {
		ctx.dm().writeToStdOut("remotescript failed to execute");
	}

	SAFE_DELETE(cd);
	return ret==0?tempExitStatus:ret;
}


///////////////////////////////////////////////////////////////////////////////
// RtiTransferProviderImplFactory
///////////////////////////////////////////////////////////////////////////////

RtiTransferProviderImplFactory::RtiTransferProviderImplFactory(const char *protocol)
	: m_protocol(protocol)
{}


TransferProviderImpl *RtiTransferProviderImplFactory::create(
	Server &target, Environment *tgtenv, Credentials *creds,
	Component *comp, const char *targetPath)
{
	//const char *broker  = parent.getProperty("broker");

	return new RtiTransferProviderImpl(
		m_protocol, target, tgtenv, creds, comp, targetPath);
}


extern "C" int rti_PluginInstall(DM &dm)
{
	// name, required, overrideable, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);

	dm.installProviderImpl("ftp",  NULL, OBJ_KIND_TRANSFER, propdefs);
	dm.installProviderImpl("ftps", NULL, OBJ_KIND_TRANSFER, propdefs);
	dm.installProviderImpl("sftp", NULL, OBJ_KIND_TRANSFER, propdefs);
	dm.installProviderImpl("win",  NULL, OBJ_KIND_TRANSFER, propdefs);
	return 0;
}
