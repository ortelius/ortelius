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
#define FTP_EXPORT __declspec(dllexport)
#include <direct.h>
#else
#define FTP_EXPORT
#define stricmp strcasecmp
#define strnicmp strncasecmp
#include <unistd.h>
#include <dlfcn.h>
#endif /*WIN32*/

#include <stdio.h>
#include <sys/stat.h>

#include <errno.h>

#include "ftp.h"

#include "../../dmapi/properties.h"
#include "../../dmapi/model.h"
#include "../../dmapi/dropzone.h"
#include "../../dmapi/expr.h"
#include "../../dmapi/charptr.h"
#include "../../dmapi/node.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/filefind.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/context.h"

#include "../../dmapi/tinyxml.h"

#define FTP_PLUGIN_VERSION "1.00"
#define FTP_CRED_NAME      "ftpserver"



///////////////////////////////////////////////////////////////////////////////
// FtpCredentialsProvider
///////////////////////////////////////////////////////////////////////////////

FtpCredentialsProvider::FtpCredentialsProvider(
		class CredentialsProviderFactory &factory,
		class Credentials &credentials
	)
	: CredentialsProvider(factory, credentials), m_dfoFileName(NULL)
{}


FtpCredentialsProvider::~FtpCredentialsProvider()
{
	SAFE_FREE(m_dfoFileName);
}



bool FtpCredentialsProvider::canTransform()
{
	switch(m_credentials.kind()) {
	case CREDENTIALS_ENCRYPTED:
	case CREDENTIALS_FROM_VARS:
		return (m_credentials.hasUsername() && m_credentials.hasPassword());
	case CREDENTIALS_HARVEST_DFO_IN_FILESYSTEM:
		return true;
	}

	return false;
}


const char *FtpCredentialsProvider::transform(Object &auth, Context &ctx)
{
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
// FtpCredentialsProviderFactory
///////////////////////////////////////////////////////////////////////////////


FtpCredentialsProviderFactory::FtpCredentialsProviderFactory()
{}


CredentialsProvider *FtpCredentialsProviderFactory::create(
	class Credentials &parent)
{
	return new FtpCredentialsProvider(*this, parent);
}

///////////////////////////////////////////////////////////////////////////////
// FtpRepositoryImpl
///////////////////////////////////////////////////////////////////////////////

FtpRepositoryImpl::FtpRepositoryImpl(
		RepositoryImplFactory &factory, DMINT32 implId,
		Repository &repository, const char *server, const char *filepath, const char *version,
		bool usessl
	)
	: RepositoryImpl(factory, implId, repository),
	  m_server(DUP_NULL(server)), m_filepath(DUP_NULL(filepath)), m_version(DUP_NULL(version)),
	  m_connected(false)
{
	debug1("FtpRepositoryImpl('%s', '%s', '%s')",
		NULL_CHECK(m_server), NULL_CHECK(m_filepath), NULL_CHECK(m_version));

	LoadFtpLibrary(usessl);
}


/**
 * Very simple test to make sure that the repository is correctly configured
 * with all the mandatory properties.  Just tests to make sure that the folder
 * specified by the "filepath" property exists.
 */
bool FtpRepositoryImpl::testProvider(class Context &ctx)
{
	if(!m_filepath) {
		ctx.writeToStdErr("\"filepath\" property must be specified");
		return false;
	}

	struct stat buf;
	if(stat(m_filepath, &buf) == -1) {
		ctx.writeToStdErr("The folder '%s' specified by \"filepath\" does not exist", m_filepath);
		return false;
	}

	return true;
}


bool FtpRepositoryImpl::copyFile(const char *sourcePath, const char *targetPath)
{
	struct stat sbuf;
	if(stat(sourcePath, &sbuf) < 0) {
		debug1("failed to stat input file ('%s') - not copying", sourcePath);
		return false;
	}
#ifdef WIN32
	struct _utimbuf ubuf;
	ubuf.actime = sbuf.st_atime;
	ubuf.modtime = sbuf.st_mtime;
#else
	struct timeval ubuf[2];
	ubuf[0].tv_sec  = sbuf.st_atime;
	ubuf[0].tv_usec = 0;
	ubuf[1].tv_sec  = sbuf.st_mtime;
	ubuf[1].tv_usec = 0;
#endif /*WIN32*/

	char buf[8096];
	bool ret = true;
#ifdef WIN32
	FILE *fin = fopen(sourcePath, "rb");
	FILE *fout = fopen(targetPath, "wb");
#else
	FILE *fin = fopen(sourcePath, "r");
	FILE *fout = fopen(targetPath, "w");
#endif /*WIN32*/
	if(fin && fout) {
		int n = sizeof(buf);
		while((n > 0) && !feof(fin)) {
			n = fread(buf, 1, sizeof(buf), fin);
			if(fwrite(buf, 1, n, fout) != n) {
				debug1("failed to write all bytes to output file - aborting copy");
				ret = false;
				break;
			}
		}
	} else {
		if(!fin) { debug1("failed to open input file ('%s') - not copying", sourcePath); }
		if(!fout) { debug1("failed to open output file ('%s') - not copying", targetPath); }
		ret = false;
	}
	if(fin) { fclose(fin); }
	if(fout) { fclose(fout); }

	// Preserve timestamps
#ifdef WIN32
	if(_utime(targetPath, &ubuf) < 0) {
#else
	if(utimes(targetPath, ubuf) < 0) {
#endif /*WIN32*/
		debug1("failed set times on output file ('%s')", targetPath);
	}

	return ret;
}


void FtpRepositoryImpl::ensureFolder(
	const char *dzbase, const char *dzpath,
	DropzoneCallback &callback, Context &ctx)
{
	// Ensure that the target subfolder in the dropzone exists
	if(strcmp(dzbase, dzpath) != 0) {
		const char *relPath = &dzpath[strlen(dzbase)];
		if(relPath[0] == DIR_SEP_CHAR) { relPath++; }
		debug1("Checking sub-folder '%s'", relPath);
		if(relPath[0] && !callback.dirExists(relPath)) {
			debug1("Creating sub-folder '%s'", relPath);
			if(!callback.mkdir(relPath)) {
				throw RuntimeError(ctx.stack(), "Failed to create sub-folder '%s'", relPath);
			}
		}
	}
}

bool CreateLocalDirectory(char *fp)
{
	bool ret = true;
	char *cfp=strdup(fp);
	struct stat dirDetails;
	int n = stat(fp,&dirDetails);
	if (n)
	{
		// This path does not exist. Try and create it...
		errno = 0;
#ifdef WIN32
		n=mkdir(fp);
#else
		n=mkdir(fp,0777);
#endif
		if(n && !errno)	{
			errno = ENOENT;
		}

		if (n && errno==ENOENT)
		{
			// Failed to create directory. Go up a level and try again...
			int c=strlen(cfp)-1;
			while (c && cfp[c]!='/' && cfp[c]!='\\') c--;
			cfp[c]='\0';
			CreateLocalDirectory(cfp);
			//
			// And try again...
			//
			errno = 0;
#ifdef WIN32
		n=mkdir(fp);
#else
     		n=mkdir(fp,0777);
#endif
		}
		if (n) ret = false;
	}
	free(cfp);
	return ret;
}

RepositoryTextPattern *FtpRepositoryImpl::getTextPatternList(const char *dir,const char *basefile)
{
	bool FoundPattern=false;
	List<RepositoryTextPattern> *rtps;
	RepositoryTextPattern *rtp = (RepositoryTextPattern *)0;
	while (!FoundPattern) {
		List<RepositoryTextPattern> *rtps = m_repository.getTextPatternsForPath(dir[0]?dir:"/");
		// Found patterns for this directory
		if (rtps->size()==0) break;	// no entries
		ListIterator<RepositoryTextPattern> lit(*rtps);
		for(rtp = lit.first(); rtp; rtp = lit.next()) {
			debug3("*** Pattern for path [%s] is [%s] text is %s",dir,rtp->pattern(),rtp->text()?"true":"false");
			//
			// Once THAT is done, we need to get the transfer mode sorted accordiingly - probably need to add
			// something to ftplib.dll to switch between text and binary mode. We also *probably* need to create
			// the dm.info file here and have the creation in dropzone.cpp recognize the file(s) existance and
			// add to it (if appropriate). Finally, we need to get type/text information from the ftp server if
			// it happens to be an iSeries box. Fair bit to do!
			//
			const char *tpatt = rtp->pattern();
			if (tpatt[0]=='*') {
				if (tpatt[1]=='.' && tpatt[2]=='*' && tpatt[3]=='\0') {
					// *.* as a pattern - match all files
					FoundPattern=true;
					break;
				} else if (tpatt[1]=='\0') {
					// * as a pattern - match all files
					FoundPattern=true;
					break;
				} else if (tpatt[1]=='.') {
					// *.ext - check extension
					const char *p = basefile;
					const char *d= (char *)0;
					while (*p) {
						if (*p=='.') d=p+1;
						p++;
					}
					if (d && stricmp(d,&(tpatt[2]))==0) {
						FoundPattern=true;
					}
				}
			} else {
                               // Doesn't start with * - just match file
				if (stricmp(tpatt,basefile)==0) FoundPattern=true;
			}
		}
	}
	if (FoundPattern) return rtp;
	// Nothing for this directory - check parent directory
	if (dir[0] && !(dir[0]=='/' && dir[1]=='\0')) {
		char *pardir = strdup(dir);
		char *p=pardir;
		char *ep=(char *)0;
		while (*p) {
			if (*p=='\\' || *p=='/') ep=p;
			p++;
		}
		if (ep) {
			*ep='\0';
			debug3("Couldn't find a pattern for [%s] - checking [%s]",dir,pardir);
			RepositoryTextPattern *rtp = getTextPatternList(pardir,basefile);
			free(pardir);
			return rtp;
		}
	}
	// If we get here we didn't find any pattern
	return (RepositoryTextPattern *)0;
}

void FtpRepositoryImpl::checkoutFolder(
	const char *dzbase, const char *dzpath, const char *repobase,
	const char *repopath, bool recursive, StringList *pattern,
	DropzoneCallback &callback, Context &ctx)
{
	int total = 0, success = 0, failed = 0, not_processed = 0;
	bool first = true;
	StringHashtable *folders = new StringHashtable();

	debug3("FTP checkout folder:");
	debug3("dzbase: [%s]",dzbase); 
	debug3("dzpath: [%s]",dzpath);
	debug3("repobase: [%s]",repobase);
	debug3("repopath: [%s]",repopath);
	debug3("recursive: %s",recursive?"true":"false");
	debug3("pattern list: %d",pattern->size());
	const char *target = &(dzpath[strlen(dzbase)]);
	debug3("target:    [%s]",target);
	if (!repopath) repopath=strdup("/");

	int res = protocol_cd((char *)repopath);
	if (res == 0) throw RuntimeError(protocol_return_last_error());
	// we're in the right directory
	StringListIterator iter(*pattern);
	char *lastrepodir = (char *)0;
	for(const char *patt = iter.first(); patt; patt = iter.next()) {
		debug3("pattern  = '%s'", patt);
		char **filelist = protocol_get_file_list((char *)patt,recursive);
		int n=0;
		while (filelist[n]) {
			char cwd[2048];
			char *basedir=strdup(filelist[n]);
			debug3("basedir=[%s]",basedir);
			char *bd=basedir;	// for freeing later
			basedir+=strlen(repobase);
			debug3("basedir now=[%s]",basedir);
			char *basefile=basedir;
			char *p=basedir;
			bool dirFound=false;
			while (*p) {
				if (*p=='/' || *p=='\\') {
					basefile=p+1;
					dirFound=true;
				}
				p++;
			}
			if (dirFound) {
				if (basefile) *(basefile-1)='\0';
			} else {
				bd=strdup("/");
				basedir=bd;
			}
			
			debug3("filelist[%d]=[%s]\n",n,filelist[n]);
			debug3("basedir=[%s] basefile=[%s]",basedir,basefile);
			// cd to the correct directory and do a pull..	.
			char *dzdir = (char *)malloc(strlen(dzbase)+strlen(basedir)+10);
			sprintf(dzdir,"%s/%s",dzbase,(basedir[0]=='/' || basedir[0]=='\\')?basedir+1:basedir);
			debug3("protocol_lcd(%s)",dzdir);
			if (!protocol_lcd(dzdir)) {
				// failed - create the repository directory
				debug3("Creating local directory [%s]",dzdir);
				if (!CreateLocalDirectory(dzdir)) throw RuntimeError("Failed to create local dir: ",dzdir);
				debug3("protocol_lcd(%s)",dzdir);
				if (!protocol_lcd(dzdir)) throw RuntimeError("Failed to lcd to dir: ",dzdir);
				callback.checked_out_folder(basedir[0]=='/'?basedir+1:basedir, DIR_SEP_STR, false);
			}

			char *repodir = (char *)(bd[0]?bd:"/");
			if (!lastrepodir || strcmp(repodir,lastrepodir)) {
				debug3("protocol_cd(%s)",repodir);
				if (!protocol_cd(repodir)) throw RuntimeError("Failed to cd to dir: ",repodir);
				debug3("switched directory");
				if (lastrepodir) {
					// we've been in another directory - record the directory name. At the
					// end of processing we'll loop around those directories and see if
					// there's any additional information to capture (AS400)
					// folders->put(lastrepodir,"Y");
					free(lastrepodir);
				}
				lastrepodir = strdup(repodir);
			}
			debug3("repodir=[%s] dzdir=[%s]",repodir,dzdir);
			folders->put(repodir,dzdir);
			//
			// Look for a text pattern for this repository directory
			//
			debug3("Getting Text Pattern List");
			RepositoryTextPattern *rtp = getTextPatternList(basedir,basefile);
			debug3("got tpl");
			if (rtp) {
				// We matched a pattern - switch to text or binary depending
				// on returned text mode
				if (rtp->text()) {
					debug3("TEXT mode transfer");
					protocol_text();
				} else {
					debug3("BINARY mode transfer");
					protocol_binary();
				}
			} else {
				// No pattern - we'll transfer the file in binary mode
				debug3("no matching pattern - BINARY mode transfer");
				protocol_binary();
			}

			if (protocol_get(basefile)) {
				success++;
				total++;
				char *t = (char *)malloc(strlen(basedir)+strlen(basefile)+10);
				char *v = (char *)malloc(strlen(basedir)+strlen(target)+strlen(basefile)+10);
				*v='\0';
				*t='\0';
				if (*target) {
					strcat(v,(target[0]=='\\' || target[0]=='/')?target+1:target);
					char c = target[strlen(target)-1];
					if (c!='\\' && c!='/') {
						strcat(v,"\\");
					}
				}
				if (*basedir) {
					strcat(v,(basedir[0]=='\\' || basedir[0]=='/')?basedir+1:basedir);
					strcat(t,(basedir[0]=='\\' || basedir[0]=='/')?basedir+1:basedir);
					char c = basedir[strlen(basedir)-1];
					if (c!='\\' && c!='/') {
						strcat(v,"\\");
						strcat(t,"\\");
					}
				}
				strcat(t,basefile);
				strcat(v,basefile);

				for (char *x=t;*x;x++) if (*x=='/') *x='\\';
				for (char *x=v;*x;x++) if (*x=='/') *x='\\';

				callback.checked_out_file(this, t[0]=='\\'?t+1:t, v[0]=='\\'?v+1:v, "1");	// need to take version from parameter
				free(t);
				free(v);
			} else {
				total++;
				failed++;
			}
			n++;
			free(dzdir);
			free(bd);
		}
	}
	// folders->put(lastrepodir);
	callback.checkout_summary(total, success, failed, not_processed);
	if(folders->count() > 0) {
		StringList *keys = folders->keys();
		StringListIterator iter(*keys);
		for(const char *folder = iter.first(); folder; folder = iter.next()) {
			protocol_lcd((char *)(folders->get(folder)));	// switch to dropzone directory
			
			// Get the additional attributes for files in this folder
			// (Note, if the FTP server is not AS400, this will be ignored)
			char **atts = protocol_get_attributes((char *)folder);

			int n=0;
			TiXmlDocument doc("dm.info");
			TiXmlElement *root = new TiXmlElement("files");
			while (atts[n]) {	
				struct stat v;
				char *mname=atts[n];
				char *mtext=mname+strlen(mname)+1;
				char *mtype=mtext+strlen(mtext)+1;
				char *tname=(char *)malloc(strlen(mname)+10);
				sprintf(tname,"%s.MBR",mname);
				if (stat(tname,&v)==0)
				{
					// This file exists and was transferred from AS400 repository
					TiXmlElement *e = new TiXmlElement("file");
					e->SetAttribute("name", tname);
					e->SetAttribute("mbrtype", mtype);
					e->SetAttribute("mbrtext", mtext);
					root->LinkEndChild(e);
				}
				free(tname);
				n++;
			}
			if (n) {
				// complete the creation of dm.info
				doc.LinkEndChild(root);
				doc.SaveFile("dm.info");
			}
		}
	}
}

void FtpRepositoryImpl::LoadFtpLibrary(bool usessl) {
	try
	{
#ifdef WIN32
		m_ftplib = DLOPEN("../lib/ftplib.dll");
#else
  m_ftplib = DLOPEN("../lib/libftplib.so");
#endif
		if (m_ftplib == NULL) throw(ERROR_COULD_NOT_FIND_LIBRARY);	// Could not locate protocol library
		protocol_initialise = (PROTOCOL_INITIALISE)DLSYM(m_ftplib,"protocol_initialise");
		protocol_connect = (PROTOCOL_CONNECT)DLSYM(m_ftplib,"protocol_connect");
		protocol_disconnect = (PROTOCOL_DISCONNECT)DLSYM(m_ftplib,"protocol_disconnect");
		protocol_put = (PROTOCOL_PUT)DLSYM(m_ftplib,"protocol_put");
		protocol_get = (PROTOCOL_GET)DLSYM(m_ftplib,"protocol_get");
		protocol_exec = (PROTOCOL_EXEC)DLSYM(m_ftplib,"protocol_exec");
		protocol_cd = (PROTOCOL_CD)DLSYM(m_ftplib,"protocol_cd");
		protocol_register_callback = (PROTOCOL_REGISTER_CALLBACK)DLSYM(m_ftplib,"protocol_register_callback");
		protocol_return_last_error = (PROTOCOL_RETURN_LAST_ERROR)DLSYM(m_ftplib,"protocol_return_last_error");
		protocol_lcd = (PROTOCOL_LCD)DLSYM(m_ftplib,"protocol_lcd");
		protocol_mkdir = (PROTOCOL_MKDIR)DLSYM(m_ftplib,"protocol_mkdir");
		protocol_pwd = (PROTOCOL_PWD)DLSYM(m_ftplib,"protocol_pwd");
		protocol_get_file_list = (PROTOCOL_GET_FILE_LIST)DLSYM(m_ftplib,"protocol_get_file_list");
		protocol_text = (PROTOCOL_TEXT)DLSYM(m_ftplib,"protocol_text");
		protocol_binary = (PROTOCOL_BINARY)DLSYM(m_ftplib,"protocol_binary");
		protocol_get_attributes = (PROTOCOL_GET_ATTRIBUTES)DLSYM(m_ftplib,"protocol_get_attributes");
		protocol_set_attributes = (PROTOCOL_SET_ATTRIBUTES)DLSYM(m_ftplib,"protocol_set_attributes");
		protocol_delete = (PROTOCOL_DELETE)DLSYM(m_ftplib,"protocol_delete");
		
		//
		// Make sure all the functions have been set up correctly, throw error otherwise
		//
		if (protocol_initialise == NULL) throw(ERROR_NO_INITIALISE_FUNCTION);
		if (protocol_connect == NULL) throw(ERROR_NO_CONNECT_FUNCTION);
		if (protocol_disconnect == NULL) throw(ERROR_NO_DISCONNECT_FUNCTION);
		if (protocol_put == NULL) throw(ERROR_NO_PUT_FUNCTION);
		if (protocol_get == NULL) throw(ERROR_NO_GET_FUNCTION);
		if (protocol_exec == NULL) throw(ERROR_NO_EXEC_FUNCTION);
		if (protocol_cd == NULL) throw(ERROR_NO_CD_FUNCTION);
		if (protocol_register_callback == NULL) throw(ERROR_NO_CALLBACK_FUNCTION);
		if (protocol_return_last_error == NULL) throw(ERROR_NO_RETURN_LAST_ERROR_FUNCTION);
		if (protocol_lcd == NULL) throw(ERROR_NO_LCD_FUNCTION);
		if (protocol_mkdir == NULL) throw(ERROR_NO_MKDIR_FUNCTION);
		if (protocol_pwd == NULL) throw(ERROR_NO_PWD_FUNCTION);
		if (protocol_get_file_list == NULL) throw(ERROR_NO_FILE_LIST_FUNCTION);
		if (protocol_text == NULL) throw(ERROR_NO_TEXT_FUNCTION);
		if (protocol_binary == NULL) throw(ERROR_NO_BINARY_FUNCTION);
		if (protocol_get_attributes == NULL) throw(ERROR_NO_GET_ATTRIBUTES_FUNCTION);
		if (protocol_set_attributes == NULL) throw(ERROR_NO_SET_ATTRIBUTES_FUNCTION);
		if (protocol_delete == NULL) throw(ERROR_NO_DELETE_FUNCTION);
		//
		// All functions present, correct and loaded. Call initialise function
		//
		
		void *pv = (void *)(usessl?1:0);	// 1 for ftps, 0 for unencrypted ftp
		int initcode = protocol_initialise(pv);
		if (initcode != 0)
		{
			throw(ERROR_INITIALISE_FAILED);
		}
	}
	catch(ERRORCODE err)
	{
		char wd[1024];
		switch(err)
		{
		case ERROR_COULD_NOT_FIND_LIBRARY:
			throw RuntimeError("Could not find library ftplib.dll");
			break;
		case ERROR_INITIALISE_FAILED:
			throw RuntimeError("Initialise of FTP protocol failed");
			break;
		default:
#ifdef WIN32
			_getcwd(wd,sizeof(wd));
#else
			getcwd(wd,sizeof(wd));
#endif
			throw RuntimeError("Required Function not found in ftplib.dll (errcode %d) dir:%s",err,wd);
			break;
		}
	}
}

void ftp_handleCredentials(
	ExtendedStmt &stmt, Repository &repo, Credentials *&creds, Context &ctx)
{
	creds = repo.getCredentials();
	if(!creds) {
		throw RuntimeError(stmt, ctx.stack(),
				"No credentials specified for FTP repository");
	}

	if (creds->kind() == CREDENTIALS_HARVEST_DFO_IN_FILESYSTEM) {
		// Harvest DFO file given - this is a problem for FTP servers
		throw RuntimeError(stmt, ctx.stack(),
				"Harvest DFO file cannot be used for FTP repository");
	}

	if (creds->kind() == CREDENTIALS_PRIVATE_KEY) {
		// Cannot use private key for FTP
		throw RuntimeError(stmt, ctx.stack(),
				"Private Key SSH connection cannot be used for FTP repository");
	}
	
	if(!(creds->hasUsername() && creds->hasPassword())) {
		throw RuntimeError(stmt, ctx.stack(),
				"No usable credentials specified for FTP repository");
	}
}

void FtpRepositoryImpl::handleCredentials(
		class ExtendedStmt &stmt, class Credentials *&creds,class Context &ctx)
{
	ftp_handleCredentials(stmt, m_repository, creds, ctx);
}

void FtpRepositoryImpl::checkout(
	const char *dzbase, const char *dzpath,
	bool recursive, StringList *pattern,
	ExtendedStmt &stmt, DropzoneCallback &callback,
	Context &ctx)
{
	// Gets file(s) from an FTP server

	Credentials *creds = NULL;
	handleCredentials(stmt, creds, ctx);

	AutoPtr<StringList> pattern_ap;
	if(!pattern) {
		pattern_ap = pattern = new StringList();
		pattern->add("*");
	}

	debug2("FtpRepositoryImpl::checkout('%s','%s')", dzbase, dzpath);
	debug3("filepath = '%s'", m_filepath);

	if (!m_connected) {
		debug3("About to connect to FTP server [%s]...\n",m_server);
		protocol_connect(m_server,creds->getDecryptedUsername(ctx),creds->getDecryptedPassword(ctx));
		debug3("done connecting\n");
		m_connected = true;
	}
	char *ftphome = protocol_pwd();
	debug3("ftphome (from server) = [%s]",ftphome);

	char *repobase = strdup(m_filepath);
	Property *pfilepath = m_repository.getProperty("filepath");
	if(pfilepath) {
		char *propval = pfilepath->decryptedValue(m_repository);
		Node npropval(NODE_STR, propval, true);
		ExprPtr epropval = npropval.evaluate(ctx);
		if(epropval) {
			ConstCharPtr fpp = epropval->toString();
			if (fpp[0]=='/' || fpp[0]=='\\') {
				// Absolute path - ignore ftphome
				repobase = (char *)realloc(repobase,strlen(fpp)+1);
				strcpy(repobase,fpp);
				debug3("repobase now = [%s]",repobase);
			} else {
				// Relative path - append to ftphome
				char *t = (char *)malloc(strlen(ftphome)+strlen(fpp)+2);
				sprintf(t,(ftphome[strlen(ftphome)-1]=='/')?"%s%s":"%s/%s",ftphome,(const char *)fpp);
				repobase = t;
				debug3("repobase now = [%s]",repobase);
			}
		}
	} else {
		// filepath property not specified - just use the ftphome directory
		debug3("filepath not specified");
		repobase = (char *)realloc(repobase,strlen(ftphome)+1);
		strcpy(repobase,ftphome);
	}
	debug3("repobase=[%s]",repobase);


	// Record the root filepath up front, as this maps to the info file for the repository
	callback.checked_out_folder(DIR_SEP_STR, DIR_SEP_STR, true);

	checkoutFolder(
		dzbase, dzpath, repobase, m_filepath,
		recursive, pattern, callback, ctx);
	free(repobase);
}


void FtpRepositoryImpl::checkin(
	const char *dzbase, const char *dzpath, ExtendedStmt &stmt,
	DropzoneCallback &callback, Context &ctx)
{
	throw RuntimeError(stmt, ctx.stack(), "Checkin not supported");
}


char *FtpRepositoryImpl::dirname(const char *filename)
{
	const char *ls = strrchr(filename, DIR_SEP_CHAR);
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


Expr *FtpRepositoryImpl::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name,"server") == 0) {
		return new Expr(m_server);
	} else if(strcmp(name, "filepath") == 0) {
		return new Expr(m_filepath);
	} else if(strcmp(name, "version") == 0) {
		return m_version ? new Expr(m_version) : NULL;
	}

	return RepositoryImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// FtpRepositoryImplFactory
///////////////////////////////////////////////////////////////////////////////

FtpRepositoryImplFactory::FtpRepositoryImplFactory()
{}


RepositoryImpl *FtpRepositoryImplFactory::create(
	DMINT32 implId, Repository &parent, ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr server = stmt.getOverridableAppendableArgAsString(
		"server", DIR_SEP_CHAR, DIR_SEP_CHAR, parent, true, ctx);
	ConstCharPtr filepath = stmt.getOverridableAppendableArgAsString(
		"filepath", DIR_SEP_CHAR, DIR_SEP_CHAR, parent, false, ctx);
	ConstCharPtr version = stmt.getOverridableArgAsString(
		"version", parent, false, ctx);
	bool usessl = stmt.getOverridableArgAsBoolean(
		"UseSSL",parent,false,ctx);
	return new FtpRepositoryImpl(*this, implId, parent, server, filepath, version, usessl);
}


///////////////////////////////////////////////////////////////////////////////

// TODO: remove "ftp_" prefix when moving to a DLL

extern "C" FTP_EXPORT int ftp_PluginStart(DM &dm)
{
	dm.writeToLogFile("Ftp plugin V" FTP_PLUGIN_VERSION);
	
	RepositoryImplRegistry::instance().registerFactory("ftprep", new FtpRepositoryImplFactory());
	CredentialsProviderRegistry::instance()
		.registerFactory(FTP_CRED_NAME, new FtpCredentialsProviderFactory());
	return 0;
}


extern "C" FTP_EXPORT int ftp_PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("server", true,  false));
	propdefs->add(new PropertyDef("filepath", false, true));
	propdefs->add(new PropertyDef("version",  false, false));

	dm.installProviderImpl("ftpserver", NULL, OBJ_KIND_REPOSITORY, propdefs);
	return 0;
}
