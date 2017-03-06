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
#include <sys/stat.h>

#ifdef WIN32
#include <direct.h>
#else
#include <pthread.h>
#endif /*WIN32*/

#include "dropzone.h"
#include "dm.h"
#include "repository.h"
#include "exceptions.h"
#include "charptr.h"
#include "filefind.h"
#include "thread.h"
#include "tinyxml.h"
#include "expr.h"
#include "datetime.h"
#include "context.h"

///////////////////////////////////////////////////////////////////////////////
// DropzoneFile
///////////////////////////////////////////////////////////////////////////////

DropzoneFile::DropzoneFile(
		IDropzone &dz, RepositoryImpl *repoImpl,
		const char *repopath, const char *dzpath, const char *version,
		class ComponentItem *item, bool ischeckin
	)
	: m_dz(dz), m_repoImpl(repoImpl), m_repopath(DUP_NULL(repopath)),
	  m_dzpath(DUP_NULL(dzpath)), m_version(DUP_NULL(version)), m_item(item),
	  m_error(NULL), m_ischeckin(ischeckin), m_basedOn(NULL),
	  m_iscreated(false), m_ismodified(false), m_isrenamed(false),
	  m_isdeleted(false), m_size(-1), m_md5(NULL)
{
	debug1("DropzoneFile(@'%s', %d, '%s', '%s', '%s', %s)", m_dz.name(),
		(m_repoImpl ? m_repoImpl->implId() : -1), (m_repopath ? m_repopath : "(null)"),
		(m_dzpath ? m_dzpath : "(null)"), (m_version ? m_version : "(null)"),
		(m_ischeckin ? "true" : "false"));
	calcSizeAndSum();
}

DropzoneFile::DropzoneFile(
		IDropzone &dz, DropzoneFile *basedOn,
		const char *dzpath, bool iscreated, bool ismodified
	)
	: m_dz(dz), m_repoImpl(NULL), m_repopath(NULL), m_dzpath(DUP_NULL(dzpath)),
	  m_version(NULL), m_item(NULL), m_error(NULL), m_ischeckin(false),
	  m_basedOn(basedOn), m_iscreated(iscreated), m_ismodified(ismodified),
	  m_isrenamed(false), m_isdeleted(false), m_size(-1), m_md5(NULL)
{
	debug1("DropzoneFile(@'%s', '%s', '%s', %s)", m_dz.name(),
		(m_basedOn ? m_basedOn->dzpath() : "(null)"),
		(m_dzpath ? m_dzpath : "(null)"), (m_ismodified ? "true" : "false"));
	calcSizeAndSum();
}


DropzoneFile::~DropzoneFile()
{
	debug3("~DropzoneFile('%s')", m_dzpath);
	m_repoImpl = NULL;			// don't own
	SAFE_FREE(m_repopath);
	SAFE_FREE(m_dzpath);
	SAFE_FREE(m_version);
	m_basedOn = NULL;		// don't own
}


void DropzoneFile::setRepopath(const char *repopath)
{
	SAFE_FREE(m_repopath);
	m_repopath = DUP_NULL(repopath);
}


void DropzoneFile::setVersion(const char *version)
{
	SAFE_FREE(m_version);
	m_version = DUP_NULL(version);
}


void DropzoneFile::setError(const char *errormsg)
{
	SAFE_FREE(m_error);
	m_error = DUP_NULL(errormsg);
}


void DropzoneFile::markAsModified()
{
	m_ismodified = true;
}


void DropzoneFile::markAsDeleted()
{
	m_isdeleted = true;
}


void DropzoneFile::rename(const char *newdzpath)
{
	//printf("DropzoneFile::rename('%s')\n", newdzpath);
	m_dz.removeFile(m_dzpath);
	SAFE_FREE(m_dzpath);
	m_dzpath = DUP_NULL(newdzpath);
	m_isrenamed = true;
	m_dz.addFile(this);
}


char *DropzoneFile::dropzoneFilename()
{
	char *ret = (char*) malloc(strlen(m_dz.pathname()) + strlen(m_dzpath) + 2);
	sprintf(ret, "%s%s%s", m_dz.pathname(), DIR_SEP_STR, m_dzpath);
	return ret;
}


void DropzoneFile::calcSizeAndSum()
{
	CharPtr dzpath = dropzoneFilename();
	struct stat sb;
	if(stat(dzpath, &sb) != -1) {
		m_size = sb.st_size;
	} else {
		m_size = -1;
	}

	// TODO: calculate md5 sum here
}


DateTime *DropzoneFile::getCreationTime()
{
	CharPtr dzpath = dropzoneFilename();
	struct stat sb;
	if(stat(dzpath, &sb) != -1) {
		return new DateTime(sb.st_ctime);
	}
	return NULL;
}


DateTime *DropzoneFile::getModifiedTime()
{
	CharPtr dzpath = dropzoneFilename();
	struct stat sb;
	if(stat(dzpath, &sb) != -1) {
		return new DateTime(sb.st_mtime);
	}
	return NULL;
}


Expr *DropzoneFile::getAttribute(const char *name, Context &ctx)
{
	//debug1("getAttribute('%s') on file '%s'", name, dropzoneFilename());

	if(strcmp(name, "dzpath") == 0) {
		return new Expr(m_dzpath);
	} else if(strcmp(name, "repopath") == 0) {
		return new Expr(m_repopath);
	} else if(strcmp(name, "size") == 0) {
		return new Expr((int) m_size);
	} else if(strcmp(name, "ctime") == 0) {
		AutoPtr<DateTime> date = getCreationTime();
		return new Expr((DateTime*) date);
	} else if(strcmp(name, "mtime") == 0) {
		AutoPtr<DateTime> date = getModifiedTime();
		return new Expr((DateTime*) date);
	}
	return NULL;
}


//DMArray *DropzoneFile::getArrayAttribute(const char *name, Context &ctx)
//{
//	return NULL;
//}


//IObject *DropzoneFile::getObjectAttribute(const char *name, Context &ctx)
//{
//	return NULL;
//}


Object *DropzoneFile::toObject()
{
	// return NULL;
	return (Object *)this;
}


void prettyprint(int flen, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buf[1024];
	int len = vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	if(len < flen) {
		for(int n = len; n < flen; n++) {
			buf[n] = ' ';
		}
		buf[flen] = '\0';
	} else {
		int skip = len - flen;
		int pos = flen/2 - 1;
		buf[pos++] = '.';
		buf[pos++] = '.';
		for(int n = pos; n < (len - skip); n++) {
			buf[n] = buf[n+skip];
		}
		buf[flen] = '\0';
	}

	printf("%s", buf);
}

void DropzoneFile::dump(const char *prefix)
{
	// ----d C:\Users\P..\tdm.441.1; httptest:/;1          ; NULL           ;
	printf("%s%c%c%c%c%c ", prefix,
		(m_ischeckin  ? 'i' : '-'),
		(m_iscreated  ? 'c' : '-'),
		(m_ismodified ? 'm' : '-'),
		(m_isrenamed  ? 'r' : '-'),
		(m_isdeleted  ? 'd' : '-'));

	if(m_dzpath) {
		prettyprint(22-strlen(prefix), "%s", m_dzpath);
	} else {
		prettyprint(22-strlen(prefix), "NULL");
	}

	printf("; ");

	if(m_repopath) {
		prettyprint(22, "%s:%s;%s",
			(m_repoImpl ? m_repoImpl->repository().name() : "NULL"),
			m_repopath, NULL_CHECK(m_version));
	} else {
		prettyprint(22, "NULL");
	}

	printf("; ");

	if(m_error) {
		prettyprint(15, "%s", m_error);
	} else {
		prettyprint(15, "NULL");
	}

	printf("; ");

	if(m_size != -1) {
		printf("%d", m_size);
	}

	printf("\n");

	if(m_basedOn) {
		m_basedOn->dump("--> ");
	}
}


///////////////////////////////////////////////////////////////////////////////
// RepositoryFolder
///////////////////////////////////////////////////////////////////////////////

RepositoryFolder::RepositoryFolder(Repository &repo, const char *folder)
	: m_repo(repo), m_folder(DUP_NULL(folder))
{}


RepositoryFolder::~RepositoryFolder()
{
	SAFE_FREE(m_folder);
}


///////////////////////////////////////////////////////////////////////////////
// DropzoneBase
///////////////////////////////////////////////////////////////////////////////

DropzoneBase::DropzoneBase(const char *name, TempFolder *folder)
	: m_name(DUP_NULL(name)), m_folder(folder), m_files(NULL),
	  m_folders(NULL), m_infos(NULL)
{}


DropzoneBase::~DropzoneBase()
{
	SAFE_FREE(m_name);
	SAFE_DELETE(m_files);
	SAFE_DELETE(m_folders);
	SAFE_DELETE(m_infos);
}


void DropzoneBase::addFile(DropzoneFile *file)
{
	if(!m_files) {
		m_files = new Hashtable<DropzoneFile>(false, true);
	}
	m_files->put(file->dzpath(), file);
}


DropzoneFile *DropzoneBase::getFile(const char *dzpath)
{
	if (m_files) {
		debug1("m_files->count()=%d",m_files->count());
		// dump();
	}
	else
	{
		debug1("m_files is null");
	}
	return m_files ? m_files->get(dzpath) : NULL;
}


DropzoneFile *DropzoneBase::removeFile(const char *dzpath)
{
	if(m_files) {
		DropzoneFile *ret = m_files->get(dzpath);
		m_files->remove(dzpath, true);
		return ret;
	}
	return NULL;
}


bool DropzoneBase::addFolder(Repository &repo, const char *repopath, const char *infoloc)
{
	if(!m_folders) {
		m_folders = new HashtableById<StringHashtable>(true);
	}
	if(!m_infos) {
		m_infos = new Hashtable<HashtableById<List<RepositoryFolder> > >(false, true);
	}

	StringHashtable *folders = m_folders->get(repo.id());
	if(!folders) {
		folders = new StringHashtable();
		m_folders->put(repo.id(), folders);
	}
	HashtableById<List<RepositoryFolder> > *infos = m_infos->get(infoloc);
	if(!infos) {
		infos = new HashtableById<List<RepositoryFolder> >(true);
		m_infos->put(infoloc, infos);
	}

	if(folders->get(repopath)) {
		// Existing folder - return false to indicate we have already processed it
		return false;
	}

	// New folder - register it
	folders->put(repopath, infoloc);

	// Record the infoloc in the reverse structure
	List<RepositoryFolder> *list = infos->get(repo.id());
	if(!list) {
		list = new List<RepositoryFolder>(true);
		infos->put(repo.id(), list);
	}
	list->add(new RepositoryFolder(repo, repopath));

	// return true to indicate we added it
	return true;
}


const char *DropzoneBase::pathname()
{
	return (m_folder ? m_folder->pathname() : NULL);
}


bool DropzoneBase::mkdir(const char *dirname)
{
	if(!m_folder) {
		return false;
	}

	debug3("DropzoneBase::mkdir(%s)",dirname);
	const char *ssdz = m_folder->pathname();
	CharPtr temp = (char*) malloc(strlen(ssdz) + strlen(dirname) + 2);
	sprintf(temp, "%s%s%s", ssdz, DIR_SEP_STR, dirname);
	debug2("mkdir('%s')", (const char*) temp);
	int ret = MKDIR(temp);

	if(ret != 0) {
		// mkdir failed - check to see if parent directory exists
		ConstCharPtr parentdir = Dropzone::dirname(dirname);
		debug3("mkdir failed - checking parent '%s'", (const char*) parentdir);
		if((strlen(parentdir) > 0) && (strcmp(parentdir, DIR_SEP_STR) != 0) && !dirExists(parentdir)) {
			debug3("parent '%s' does not exist", (const char*) parentdir);
			if(this->mkdir(parentdir)) {
				debug2("mkdir('%s') (2)", (const char*) temp);
				ret = MKDIR(temp);
			}
		}
	}

	return (ret == 0) ? true : false;
}


bool DropzoneBase::dirExists(const char *dirname)
{
	if(!m_folder) {
		return false;
	}

	debug2("dirExists(%s)",dirname);

	const char *ssdz = m_folder->pathname();
	CharPtr temp = (char*) malloc(strlen(ssdz) + strlen(dirname) + 2);
	if ((dirname[0]=='/' || dirname[0]=='\\') && dirname[1]=='\0') {
		strcpy(temp,ssdz);
	} else {
		sprintf(temp, "%s%s%s", ssdz, DIR_SEP_STR, dirname);
	}

	struct stat sb;
	if(stat(temp, &sb) != -1) {
		if(sb.st_mode & STAT_IS_DIRECTORY) {
			debug3("Dir '%s' exists", (const char*) temp);
			return true;
		}
	}
	debug2("Dir '%s' does not exist", (const char*) temp);
	//char *t = (char *)malloc(strlen(temp)+1000);
	//sprintf(t,"dir \"%s\"",temp);
	//system(t);
	//Sleep(20000);	// wait for a bit
	return false;
}


void DropzoneBase::unlink()
{
	if(m_folder) {
		debug1("Deleting server dropzone folder '%s'", m_folder->pathname());
		m_folder->unlink();
	}
}


/*private*/ void DropzoneBase::internalReconcileFolder(
	const char *rootpath, const char *pathname,
	List<RepositoryIgnorePattern> *ignores)
{
	if(!pathname) {
		pathname = rootpath;
	}

	FileFind finder(pathname, NULL);
	if(finder.find()) {
		do {
			ConstCharPtr fp = finder.fullpath();
			const char *rp = strstr(fp, rootpath) ? &fp[strlen(rootpath)+1] : fp;

			if(finder.isDots()) {
				continue;
			}

			if(ignores) {
				bool ignoreme = false;
				ListIterator<RepositoryIgnorePattern> iit(*ignores);
				for(RepositoryIgnorePattern *ign = iit.first(); ign; ign = iit.next()) {
					if(ign->matches(rp, finder.isDirectory())) {
						debug1("Reconcile dropzone '%s' - ignoring %s '%s'\n", m_name, (finder.isDirectory() ? "dir" : "file"), rp);
						ignoreme = true;
					}
				}
				if(ignoreme) {
					continue;
				}
			}

			if(!finder.isDirectory()) {
				// File, so add to list
				debug2("Reconcile dropzone '%s' - checking file '%s'", m_name, rp);
				DropzoneFile *dzf = m_files ? m_files->get(rp) : NULL;
				if(!dzf) {
					printf("WARNING: File '%s' in dropzone '%s' is not recorded\n", rp, m_name);
				}
			} else if(!finder.isDots()) {
				// Directory, so recurse
				debug2("Reconcile dropzone '%s' - checking dir '%s'", m_name, rp);
				internalReconcileFolder(rootpath, fp, ignores);
			}
		} while(finder.next());
	}
	finder.close();
}


void DropzoneBase::reconcile(List<RepositoryIgnorePattern> *ignores)
{
	// First pass - check whether all the files we have records for actually exist
	if(m_files && m_folder) {
		AutoPtr<StringList> keys = m_files->keys();
		StringListIterator iter(*keys);
		for(const char *key = iter.first(); key; key = iter.next()) {
			DropzoneFile *dzf = m_files->get(key);

			debug2("Reconcile dropzone '%s' - checking dropzone file '%s'", m_name, dzf->dzpath());
			CharPtr dzpath = dzf->dropzoneFilename();
			struct stat sb;
			if(stat(dzpath, &sb) != -1) {
				// file exists
				if(dzf->isDeleted()) {
					printf("WARNING: File '%s' in dropzone '%s' exists but is marked as deleted\n", dzf->dzpath(), m_name);
					//unlink?
				}
				if(!dzf->isModified() && (dzf->size() != sb.st_size)) {
					printf("WARNING: File '%s' in dropzone '%s' has changed in size\n", dzf->dzpath(), m_name);
				}
			} else {
				// file does not exist
				if(!dzf->isDeleted()) {
					printf("WARNING: File '%s' in dropzone '%s' does NOT exist and is NOT marked as deleted\n", dzf->dzpath(), m_name);
					dzf->markAsDeleted();
				}
			}
		}
	}

	// Second pass - search the dropzone looking for files we haven't recorded
	if(m_folder) {
		internalReconcileFolder(m_folder->pathname(), NULL, ignores);
	}
}


void DropzoneBase::generateDmInfos()
{
	if(!m_infos) {
		debug1("No infos recorded for dropzone '%s'", m_name);
		return;
	}
	AutoPtr<StringList> infokeys = m_infos->keys();
	if(!infokeys) {
		debug1("Failed to get keys for recorded infos for dropzone '%s'", m_name);
		return;
	}
	StringListIterator ikit(*infokeys);
	int minlen = 999;
	const char *rootinfo = NULL;
	for(const char *info = ikit.first(); info; info = ikit.next()) {
		if(strlen(info) < minlen) {
			minlen = strlen(info);
			rootinfo = info;
		}
	}

	List<RepositoryIgnorePattern> *ignores = generateIgnoreList();

	debug1("About to iterate through ikit list (infokeys)");
	
	for(const char *info = ikit.first(); info; info = ikit.next()) {
		debug1("info=[%s]",info);
		HashtableById<List<RepositoryFolder> > *repos = m_infos->get(info);
		if(!repos) {
			debug1("Failed to get repos for recorded infos for info '%s'", info);
			continue;
		}
		AutoPtr<IntList> repokeys = repos->keys();
		if(!repokeys) {
			debug1("Failed to get keys for recorded repos for info '%s'", info);
			continue;
		}

		const char *dropzone = pathname();

		debug1("dropzone=[%s]",dropzone);
		//char *t = (char *)malloc(strlen(dropzone)+100);
		//sprintf(t,"dir %s",dropzone);
		//system(t);

		char *baseloc = (char*) malloc(strlen(dropzone) + strlen(info) + 11);

		if((info[0] == '\\') || (info[0] == '/')) {
			if(info[1] == '\0') {
				// info is either / or \ - drop and just use platform separator
				sprintf(baseloc, "%s%s", dropzone, DIR_SEP_STR);
			} else {
				sprintf(baseloc, "%s%s%s", dropzone, info, DIR_SEP_STR);
			}
		} else {
			sprintf(baseloc, "%s%s%s%s", dropzone, DIR_SEP_STR, info, DIR_SEP_STR);
		}
		char *infofile = (char *)malloc(strlen(baseloc)+11);
		sprintf(infofile,"%sdm.info",baseloc);

		struct stat tt;
		bool infoFileExists=false;
		bool folderinfoFileExists=false;
		if (stat(infofile,&tt)==0) {
			// the dm.info file already exists
			debug3("dm.info already exists");
			infoFileExists=true;
		}

		debug3("infofile=[%s]",infofile);
		TiXmlDocument doc(infoFileExists?infofile:"dm.info");
		if (infoFileExists) doc.LoadFile();	// Load Existing file
		TiXmlElement *root = infoFileExists?doc.FirstChildElement("files"):new TiXmlElement("files");
		TiXmlDocument *pdoc = &doc;
		TiXmlElement *inforoot = root;
		int count = 0;

		IntListIterator rkit(*repokeys);
		for(int repoid = rkit.first(); rkit.more(); repoid = rkit.next()) {
			debug3("Repository ID: %d", repoid);
			char *comment = (char*) malloc(32 + 15);
			sprintf(comment, " repository: %d ", repoid);
			debug3("   <!--%s-->", comment);
			root->LinkEndChild(new TiXmlComment(comment));
			SAFE_FREE(comment);

			// ignores only go in the root dm.info
			if((info == rootinfo) && ignores) {
				ListIterator<RepositoryIgnorePattern> iit(*ignores);
				for(RepositoryIgnorePattern *ign = iit.first(); ign; ign = iit.next()) {
					debug3("   ignore name=\"%s\" isdir=\"%s\"", ign->pattern(), (ign->isDir() ? "true" : "false"));
					TiXmlElement *ignele = new TiXmlElement(ign->isDir() ? "dir" : "file");
					ignele->SetAttribute("name", ign->pattern());
					ignele->SetAttribute("ignore", "true");
					root->LinkEndChild(ignele);
					count++;
				}
			}

			List<RepositoryFolder> *folders = repos->get(repoid);
			debug3("%d:", repoid);

			ListIterator<RepositoryFolder> rfit(*folders);
			for(RepositoryFolder *rf = rfit.first(); rf; rf = rfit.next()) {
				debug3("rf->folder=[%s]",rf->folder());
				//
				// Check if the repository folder exists. If it does then we will create another
				// dm.info file in that folder
				//
				

				// TiXmlDocument doc2("dm.info");
				TiXmlDocument *doc2 = new TiXmlDocument("dm.info");
				char *finfo = (char *)0;
				bool folderinfo=false;
				struct stat s;
				char *infodir=(char *)malloc(strlen(baseloc)+strlen(rf->folder())+10);
				sprintf(infodir,"%s%s%s",baseloc,DIR_SEP_STR,rf->folder());
				if (stat(infodir,&s)==0) {
					// Folder exists
					folderinfo=true;
					finfo = (char *)malloc(strlen(infodir)+11);
					sprintf(finfo,"%s%sdm.info",infodir,DIR_SEP_STR);
					struct stat tt;
					
					if (stat(finfo,&tt)==0) {
						// the dm.info file already exists
						debug3("dm.info already exists in this folder");
						if (doc2->LoadFile(finfo)) {
							// Successfully loaded existing dm.info file
							debug3("Changing inforoot");
							inforoot = doc2->FirstChildElement("files");
							folderinfoFileExists=true;
						}
					} else {
						debug3("changing inforoot (2)");
						inforoot = new TiXmlElement("files");
					}
					pdoc = doc2;
				}
				free(infodir);


				char *comment = (char*) malloc(strlen(rf->folder()) + 12);
				sprintf(comment, " folder: %s ", rf->folder());
				debug3("   <!--%s-->", comment);
				
				debug3("Adding comment to inforoot (0x%lx)",inforoot);
				inforoot->LinkEndChild(new TiXmlComment(comment));
				SAFE_FREE(comment);

				List<RepositoryTextPattern> *txtlist = rf->repository().getTextPatternsForPath(rf->folder());
				debug3("txtlist->size=%d",txtlist->size());
				if(txtlist) {
					ListIterator<RepositoryTextPattern> lit(*txtlist);
					for(RepositoryTextPattern *rtp = lit.first(); rtp; rtp = lit.next()) {
						debug3("   <file name=\"%s\" text=\"%s\" />", rtp->pattern(), (rtp->text() ? "true" : "false"));
						TiXmlElement *filele = new TiXmlElement("file");
						filele->SetAttribute("name", rtp->pattern());
						filele->SetAttribute("text", (rtp->text() ? "true" : "false"));
						debug3("Linking summat to inforoot (0x%lx)",inforoot);
						inforoot->LinkEndChild(filele);
						count++;
					}
					SAFE_DELETE(txtlist);
				}

				// Save the dm.info file for this folder if it's been created
				if (folderinfo) {
					if (count>0) {
						bool SaveOkay = pdoc->SaveFile(finfo);
						printf("SaveOkay=%s finfo (%s) follows:\n",SaveOkay?"true":"false",finfo);
						catFile1(finfo);
					} else {
						debug2("count is zero - not generating %s", finfo);
					}
				}
			}
		}

		debug3("folderinfoFileExists=%s",folderinfoFileExists?"true":"false");
		if (!folderinfoFileExists) {
			if (!infoFileExists) pdoc->LinkEndChild(inforoot);
			debug3("pdoc->Print()");
			pdoc->Print();
			if(count > 0) {
				//doc.Print();
				pdoc->SaveFile(infofile);
				catFile1(infofile);
			} else {
				debug2("Count is zero - not generating %s", infofile);
			}
		}
		

		SAFE_FREE(infofile);
	}

	SAFE_DELETE(ignores);
}


void DropzoneBase::dump()
{
	printf("Dropzone '%s' contains:\n", m_name);
	if(m_files) {
		AutoPtr<StringList> keys = m_files->keys();
		StringListIterator iter(*keys);
		for(const char *key = iter.first(); key; key = iter.next()) {
			DropzoneFile *dzf = m_files->get(key);
			printf("key=%s\n",key);
			dzf->dump("");
		}
	} else {
	  printf("  <nothing>\n");
	}
}


///////////////////////////////////////////////////////////////////////////////
// ServerDropzone
///////////////////////////////////////////////////////////////////////////////

ServerDropzone::ServerDropzone(
		Dropzone &dz, const char *name, TempFolder *folder)
	: DropzoneBase(name, folder), m_dz(dz)
{}


ServerDropzone::~ServerDropzone()
{}


void ServerDropzone::addRepository(Repository *repo)
{
	// delegate back to parent dropzone
	m_dz.addRepository(repo);
}


List<class RepositoryIgnorePattern> *ServerDropzone::generateIgnoreList()
{
	// delegate back to parent dropzone
	return m_dz.generateIgnoreList();
}


void ServerDropzone::remove(DM &dm)
{
	if(m_folder) {
		dm.removeTemporaryFolder(m_folder);
		m_folder = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////
// Dropzone
///////////////////////////////////////////////////////////////////////////////

Dropzone::Dropzone(DM &dm, const char *name, bool autoUnlink)
	: DropzoneBase(name, NULL), m_dm(dm), m_autoUnlink(autoUnlink),
	  m_serverDropzones(NULL), m_vars(NULL), m_refCount(0), m_repos(NULL)
{}


Dropzone::~Dropzone()
{
	debug3("~Dropzone('%s')", m_name);
	SAFE_DELETE(m_serverDropzones);
	// We release our ref count here in preference to deleting as we can't
	// guarantee the threads have all been cleaned up at this point
	if(m_vars) {
		m_vars->unsetScopeObject();
		m_vars->releaseRef();
		m_vars = NULL;
	}
	SAFE_DELETE(m_repos);
}


bool Dropzone::hasServerDropzones()
{
	return (m_serverDropzones && (m_serverDropzones->count() > 0)) ? true : false;
}


ServerDropzone *Dropzone::getServerDropzone(Server &server)
{
	return m_serverDropzones ? m_serverDropzones->get(server.id()) : NULL;
}


bool Dropzone::exists(Server &server)
{
	return getServerDropzone(server) ? true : false;
}


bool Dropzone::create(bool empty)
{
	CharPtr tfname = (char*) malloc(strlen(m_name) + 32);
	sprintf(tfname, "%d.%s", m_dm.deployId(), m_name);

	m_folder = m_dm.createTemporaryFolder(tfname, empty);
	return (m_folder ? true : false);
}


bool Dropzone::create(class Server &server, bool empty)
{
	if(!m_serverDropzones) {
		m_serverDropzones = new HashtableById<ServerDropzone>(true);
	}

	CharPtr sdzname = (char*) malloc(strlen(m_name) + 32);
	sprintf(sdzname, "%s.%d", m_name, server.id());

	char *tfname = (char*) malloc(strlen(m_name) + 32);
	sprintf(tfname, "%d.%s.%d", m_dm.deployId(), m_name, server.id());

	TempFolder *tf = m_dm.createTemporaryFolder(tfname, empty);
	if(tf) {
		m_serverDropzones->put(server.id(), new ServerDropzone(*this, sdzname, tf));
		return true;
	}

	m_dm.writeToStdOut("Failed to create dropzone %s\n",tfname);
	return false;
}



void Dropzone::unlink()
{
	DropzoneBase::unlink();

	if(m_serverDropzones) {
		IntList *keys = m_serverDropzones->keys();
		IntListIterator kit(*keys);
		for(int key = kit.first(); kit.more(); key = kit.next()) {
			ServerDropzone *sdz = m_serverDropzones->get(key);
			if(sdz) {
				debug2("Deleting dropzone '%s' folder '%s'", m_name, sdz->pathname());
				sdz->unlink();
			}
		}
		SAFE_DELETE(keys);
	}
}


void Dropzone::remove()
{
	if(m_folder) {
		m_dm.removeTemporaryFolder(m_folder);
		m_folder = NULL;
	}
	if(m_serverDropzones) {
		IntList *keys = m_serverDropzones->keys();
		IntListIterator kit(*keys);
		for(int key = kit.first(); kit.more(); key = kit.next()) {
			ServerDropzone *sdz = m_serverDropzones->get(key);
			if(sdz) {
				sdz->remove(m_dm);
			}
		}
		SAFE_DELETE(keys);
		m_serverDropzones->clear();
	}
}

	
void Dropzone::addRef()
{
	++m_refCount;
	debug3("Dropzone '%s' refcount %d", m_name, m_refCount);
}


void Dropzone::releaseRef()
{
	Thread::lock(__LINE__,__FILE__,"releaseRef");
	--m_refCount;
	debug3("Thread %d Dropzone '%s' refcount %d", Thread::currentThreadId(), m_name, m_refCount);
	if(m_autoUnlink && (m_refCount == 0)) {
		if(!getenv("trinounlink")) {
			debug2("Thread %d Automatically deleting dropzone '%s'", Thread::currentThreadId(), m_name);
			unlink();
		}
		m_dm.removeDropzone(this);
	}
	Thread::unlock(__LINE__,__FILE__,"releaseRef");
}


void Dropzone::addRepository(Repository *repo)
{
	if(!m_repos) {
		m_repos = new HashtableById<Repository>(false);
	}

	if(!m_repos->get(repo->id())) {
		m_repos->put(repo->id(), repo);
	}
}


/**
 * Iterates through all the repositories that this dropzone has touched
 * building up a single list of all the ignore patterns.
 */
List<RepositoryIgnorePattern> *Dropzone::generateIgnoreList()
{
	if(!m_repos) {
		return NULL;
	}
	AutoPtr<IntList> repokeys = m_repos->keys();
	if(!repokeys) {
		return NULL;
	}

	List<RepositoryIgnorePattern> *ret = new List<RepositoryIgnorePattern>(false);

	IntListIterator rkit(*repokeys);
	for(int repoid = rkit.first(); rkit.more(); repoid = rkit.next()) {
		Repository *repo = m_repos->get(repoid);
		List<RepositoryIgnorePattern> *ignlist = repo->getIgnorePatterns();
		if(ignlist) {
			ListIterator<RepositoryIgnorePattern> iit(*ignlist);
			for(RepositoryIgnorePattern *ign = iit.first(); ign; ign = iit.next()) {
				debug1("ignore repo='%s'; pattern='%s'; isdir='%s'", repo->name(), ign->pattern(), (ign->isDir() ? "true" : "false"));
				ret->add(ign);
			}
			//SAFE_DELETE(ignlist); - can't delete as the list owns the patterns
			}
	}

	return ret;
}


void Dropzone::generateDmInfos()
{
	DropzoneBase::generateDmInfos();

	if(m_serverDropzones) {
		AutoPtr<IntList> keys = m_serverDropzones->keys();
		IntListIterator kit(*keys);
		for(int key = kit.first(); kit.more(); key = kit.next()) {
			ServerDropzone *sdz = m_serverDropzones->get(key);
			if(sdz) {
				sdz->generateDmInfos();
			}
		}
	}
}


Scope *Dropzone::getVars()
{
	if(!m_vars) {
		m_vars = new Scope(DROPZONE_SCOPE, this);
		m_vars->addRef();
	}

	return m_vars;
}


void Dropzone::findInternal(
		DMArray *ret, const char *rootpath,
		const char *pathname, const char *pattern)
{
	if(!m_files) {
		debug1("Dropzone has no files recorded - find returning nothing");
		return;
	}
	if(!pathname) {
		pathname = rootpath;
	}

	// Find files that match the pattern
	FileFind ffinder(pathname, pattern);
	if(ffinder.find()) {
		do {
			ConstCharPtr fp = ffinder.fullpath();
			if(!ffinder.isDirectory()) {
				// File, so add to list
				const char *rp = strstr(fp, rootpath) ? &fp[strlen(rootpath)+1] : fp;
				DropzoneFile *dzf = m_files->get(rp);
				if(dzf) {
					ret->put(rp, new Variable(NULL, dzf));
				} else {
					debug1("No DropzoneFile for '%s' in dropzone '%s' - need to reconcile", rp, m_name);
				}
			}
		} while(ffinder.next());
	}
	ffinder.close();

	// Process any subdirectories
	FileFind dfinder(pathname, NULL);
	if(dfinder.find()) {
		do {
			ConstCharPtr fp = dfinder.fullpath();
			if(dfinder.isDirectory() && !dfinder.isDots()) {
				findInternal(ret, rootpath, fp, pattern);
			}
		} while(dfinder.next());
	}
	dfinder.close();
}


DMArray *Dropzone::find(const char* pattern)
{
	DMArray *ret = new DMArray(false, false, true);
	findInternal(ret, m_folder->pathname(), NULL, pattern);
	return ret;
}


/*static*/ char *Dropzone::dirname(const char *filename)
{
	const char *ls = strrchr(filename, DIR_SEP_CHAR);
	if(!ls) {
		return strdup(DIR_SEP_STR);
	}

	int len = ls - filename;

	char *ret = (char*) malloc(len + 1);
	if (ret) {
		strncpy(ret, filename, len);
		ret[len] = '\0';
	}
	return ret;
}


/*static*/ char *Dropzone::slashify(const char *filename)
{
	char *ret = strdup(filename);
	for(char *x = ret; x && *x; x++) {
#ifdef WIN32
		if(*x == '/') {
#else
		if(*x == '\\') {
#endif /*WIN32*/
			*x = DIR_SEP_CHAR;
		}
	}
	return ret;
}


void Dropzone::dump()
{
	AutoPtr<List<RepositoryIgnorePattern> > ignores = generateIgnoreList();
	reconcile(ignores);	// TODO: This should not be done here!

	printf("----- 8< -----\n");
	DropzoneBase::dump();

	if(m_serverDropzones) {
		AutoPtr<IntList> keys = m_serverDropzones->keys();
		IntListIterator iter(*keys);
		for(int key = iter.first(); iter.more(); key = iter.next()) {
			ServerDropzone *sdz = m_serverDropzones->get(key);
			printf("\n");
			sdz->dump();
		}
	}
	printf("----- 8< -----\n");
}


void Dropzone::reconcile(List<RepositoryIgnorePattern> *ignores)
{
	DropzoneBase::reconcile(ignores);

	if(m_serverDropzones) {
		AutoPtr<IntList> keys = m_serverDropzones->keys();
		IntListIterator iter(*keys);
		for(int key = iter.first(); iter.more(); key = iter.next()) {
			ServerDropzone *sdz = m_serverDropzones->get(key);
			sdz->reconcile(ignores);
		}
	}
}


Expr *Dropzone::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "name") == 0) {
		return new Expr(m_name);
	} else if(strcmp(name, "path") == 0) {
		return m_folder ? new Expr(m_folder->pathname()) : NULL;
	}
	if((strcmp(name, "files") == 0) && m_folder) {
		return new Expr(find(NULL));
	}
	return NULL;
}


//class DMArray *Dropzone::getArrayAttribute(const char *name, Context &ctx)
//{
//	if((strcmp(name, "files") == 0) && m_folder) {
//		return find(NULL);
//	}
//	return NULL;
//}


//IObject *Dropzone::getObjectAttribute(const char *name, Context &ctx)
//{
//	return NULL;
//}


Object *Dropzone::toObject()
{
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
// DropzoneCallback
///////////////////////////////////////////////////////////////////////////////


DropzoneCallback::DropzoneCallback(IDropzone &dz)
	: m_repo(NULL), m_repoImplId(0), m_dz(dz), m_ctx(NULL), m_item(NULL), m_files(NULL),
	  m_total(0), m_success(0), m_failed(0), m_notProcessed(0)
{}


DropzoneCallback::DropzoneCallback(
		Repository *repo, DMINT32 repoImplId,
		IDropzone &dz, Context &ctx, ComponentItem *item /*= NULL*/
	)
	: m_repo(repo), m_repoImplId(repoImplId), m_dz(dz), m_ctx(&ctx), m_item(item), m_files(NULL),
	  m_total(0), m_success(0), m_failed(0), m_notProcessed(0)
{}


DropzoneCallback::~DropzoneCallback()
{
	SAFE_DELETE(m_files);
}


void DropzoneCallback::checkout_from_repository(Repository *repo)
{
	m_dz.addRepository(repo);
}


void DropzoneCallback::checked_out_file(
		RepositoryImpl *impl, const char *repopath, const char *dzpath, const char *version)
{
	m_ctx->dm().writeToStdOut("INFO: Checked out %s;%s",repopath,version);
	m_dz.addFile(new DropzoneFile(m_dz, impl, repopath, dzpath, version, m_item, false));
}


void DropzoneCallback::checked_out_folder(
		const char *repopath, const char *infoloc, bool fixed)
{
	if(!m_repo) {
		throw RuntimeError("No repository set during checked_out_folder callback");
	}
	debug1("Adding checked_out_folder repopath=%s infoloc=%s fixed=%s",repopath,infoloc,fixed?"true":"false");

	// Repeatedly take dirname and register
	{
	char *vpfn = strdup(repopath);
	char *infn = strdup(infoloc);
	while(vpfn && infn && vpfn[0] && infn[0]
		  && m_dz.addFolder(*m_repo, vpfn, infn)) {
		// Get parent folder and go around again
		char *temp = m_repo->dirname(vpfn);
		SAFE_FREE(vpfn);
		vpfn = temp;
		if(!fixed) {
			temp = Dropzone::dirname(infn);
			SAFE_FREE(infn);
			infn = temp;
		}
	}
	SAFE_FREE(vpfn);
	SAFE_FREE(infn);
	}
}


void DropzoneCallback::checkout_summary(
		int total, int success, int failed, int notProcessed)
{
	m_total   += total;
	m_success += success;
	m_failed  += failed;
	m_notProcessed += notProcessed;
}


void DropzoneCallback::checkin_to_repository(Repository *repo)
{
	m_dz.addRepository(repo);
}


void DropzoneCallback::checked_in_file(
	const char *repopath, const char *dzpath, const char *version)
{
	// Find the DropzoneFile we just checked in
	DropzoneFile *df = m_dz.getFile(dzpath);
	if(df) {
		debug1("Checked in DropzoneFile found for '%s'", df->dzpath());

		if(df->repopath()) {
			printf("WARNING: File '%s' already has a repository path\n", df->dzpath());
		}
		if(df->version()) {
			printf("WARNING: File '%s' already has a version\n", df->dzpath());
		}
		df->setRepopath(repopath);
		df->setVersion(version);

		if(!m_files) {
			m_files = new List<DropzoneFile>(false);
		}
		m_files->add(df);
	} else {
		debug1("Checked in DropzoneFile NOT found for '%s'", dzpath);
		// TODO: We need to create a DropzoneFile here for a file we previously knew nothing about
	}
}


void DropzoneCallback::checkin_file_error(
	const char *dzpath, const char *errormsg)
{
	DropzoneFile *df = m_dz.getFile(dzpath);
	if(df) {
		debug1("Failed checkin DropzoneFile found for '%s'", df->dzpath());

		if(df->error()) {
			printf("WARNING: File '%s' already has an error\n", df->dzpath());
		}
		df->setError(errormsg);

		if(!m_files) {
			m_files = new List<DropzoneFile>(false);
		}
		m_files->add(df);
	} else {
		debug1("Checked in DropzoneFile NOT found for '%s'", dzpath);
		// TODO: We need to create a DropzoneFile here for a file we previously knew nothing about
	}
}


void DropzoneCallback::checkin_summary(
		int total, int success, int failed, int notProcessed)
{
	m_total   += total;
	m_success += success;
	m_failed  += failed;
	m_notProcessed += notProcessed;

	if(m_files) {
		ListIterator<DropzoneFile> iter(*m_files);
		for(DropzoneFile *dzf = iter.first(); dzf; dzf = iter.next()) {
			char repobit[1204];
			if(dzf->repopath()) {
				sprintf(repobit, "%s;%s", dzf->repopath(), (dzf->version() ? dzf->version() : "(null)"));
			} else {
				sprintf(repobit, "NULL");
			}
			char errorbit[1024];
			if(dzf->error()) {
				sprintf(errorbit, " (%s)", dzf->error());
			} else {
				errorbit[0] = '\0';
			}

			printf("CHECKIN: %s -> %s%s\n", dzf->dzpath(), repobit, errorbit);
		}
	}
}

bool DropzoneCallback::mkdir(const char *dirname)
{
	return m_dz.mkdir(dirname);
}


bool DropzoneCallback::dirExists(const char *dirname)
{
	return m_dz.dirExists(dirname);
}
