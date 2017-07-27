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
#ifndef __dropzone_h

#define __dropzone_h


#include "dmapiexport.h"
#include "list.h"
#include "hashtable.h"
#include "model.h"


/**
* Representation of a file in a dropzone.  The file can have got there by a
* checkout, a checkin or been created by a modification.  These objects will
* later be married up to one or more TransferResults.
*
* repo - set if file was checked-out or has been checked-in - null if the file
*   was created or hasn't yet been checked in
* version - some kind of representation of the file version in a repository
*   specific format - e.g. mapped version for Harvest, revision for subversion
* error - any error message from the repository implemtation indicating a
*   problem with the checkout/checkin/creation
* ischeckin - true if the file is being checked in, false if it is being
*   checked out
*
* repo path, source server path (for checkin), dzpath
*/
class DMAPI_API DropzoneFile : public IObject
{
private:
	class IDropzone &m_dz;
	class RepositoryImpl *m_repoImpl;
	char *m_repopath;
	char *m_dzpath;
	char *m_version;
	class ComponentItem *m_item;
	char *m_error;
	bool m_ischeckin;
	class DropzoneFile *m_basedOn;
	bool m_iscreated;
	bool m_ismodified;
	bool m_isrenamed;
	bool m_isdeleted;
	DMINT32 m_size;
	char *m_md5;
	bool m_zipfile;
	time_t m_modtime;

public:
	DropzoneFile(
		class IDropzone &dz, class RepositoryImpl *repoImpl,
		const char *repopath, const char *dzpath, const char *version,
		class ComponentItem *item, bool ischeckin);

	DropzoneFile(
		class IDropzone &dz, class DropzoneFile *basedOn,
		const char *dzpath, bool iscreated, bool ismodified);

	// For results of a dir on a zipfile
	DropzoneFile(
		IDropzone &dz, const char *dzpath, DMINT32 size, time_t modtime);

	~DropzoneFile();

	class RepositoryImpl *repoImpl()  { return m_repoImpl; }
	const char *repopath()       { return m_repopath; }
	const char *dzpath()         { return m_dzpath; }
	const char *version()        { return m_version; }
	class ComponentItem *item()  { return m_item; }
	const char *error()          { return m_error; }
	bool isCreated()             { return m_iscreated; }
	bool isModified()            { return m_ismodified; }
	bool isRenamed()             { return m_isrenamed; }
	bool isDeleted()             { return m_isdeleted; }
	DMINT32 size()               { return m_size; }
	DropzoneFile *basedOn()      { return m_basedOn; }

	void setRepopath(const char *repopath);
	void setVersion(const char *version);
	void setError(const char *errormsg);
	void markAsModified();
	void markAsDeleted();

	//void setModified(bool modified);
	//void setCheckIn(bool checkin);
	void rename(const char *newdzpath);

	void setProperty(const char* prop, const char *value);

	char *dropzoneFilename();

	void calcSizeAndSum();
	class DateTime *getCreationTime();
	class DateTime *getModifiedTime();

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_DROPZONEFILE; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//class IObject *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject();

	void dump(const char *prefix);
};


class DMAPI_API RepositoryFolder
{
private:
	class Repository &m_repo;
	char *m_folder;

public:
	RepositoryFolder(class Repository &repo, const char *folder);
	~RepositoryFolder();

	class Repository &repository()  { return m_repo; }
	const char *folder()  { return m_folder; }
};


class IDropzone
{
public:
	virtual const char *name() = 0;
	virtual const char *pathname() = 0;
	virtual void addFile(DropzoneFile *file) = 0;
	virtual DropzoneFile *getFile(const char *dzpath) = 0;
	virtual DropzoneFile *removeFile(const char *dzpath) = 0;
	virtual void addRepository(class Repository *repo) = 0;
	virtual bool addFolder(class Repository &repo, const char *repopath, const char *infoloc) = 0;
	virtual bool mkdir(const char *dirame) = 0;
	virtual bool dirExists(const char *dirname) = 0;
};


class DropzoneBase : public IDropzone
{
protected:
	char *m_name;
	class TempFolder *m_folder;
	Hashtable<DropzoneFile> *m_files;
	HashtableById<StringHashtable> *m_folders;
	Hashtable<HashtableById<List<RepositoryFolder> > > *m_infos;

	DropzoneBase(const char *name, class TempFolder *folder);

	void internalReconcileFolder(
		const char *rootpath, const char *pathname,
		List<class RepositoryIgnorePattern> *ignores);

public:
	virtual ~DropzoneBase();

	const char *name()  { return m_name; }
	const char *pathname();

	void addFile(DropzoneFile *file);
	DropzoneFile *getFile(const char *dzpath);
	DropzoneFile *removeFile(const char *dzpath);

	bool addFolder(class Repository &repo, const char *repopath, const char *infoloc);

	bool mkdir(const char *dirame);
	bool dirExists(const char *dirname);

	virtual void unlink();

	virtual void reconcile(List<class RepositoryIgnorePattern> *ignores);
	virtual List<class RepositoryIgnorePattern> *generateIgnoreList() = 0;
	virtual void generateDmInfos();

	virtual void dump();
};


class ServerDropzone : public DropzoneBase
{
private:
	class Dropzone &m_dz;

	ServerDropzone(class Dropzone &dz, const char *name, class TempFolder *folder);

	void remove(class DM &dm);

public:
	~ServerDropzone();

	void addRepository(class Repository *repo);

	List<class RepositoryIgnorePattern> *generateIgnoreList();

	friend class Dropzone;
};


class DMAPI_API Dropzone : public IObject, public DropzoneBase
{
private:
	class DM &m_dm;
	HashtableById<ServerDropzone> *m_serverDropzones;
	class Scope *m_vars;
	bool m_autoUnlink;
	int m_refCount;
	HashtableById<Repository> *m_repos;

	void findInternal(
		DMArray *ret, const char *rootpath,
		const char *pathname, const char *pattern);

public:
	Dropzone(class DM &dm, const char *name, bool autoUnlink);
	~Dropzone();

	const char *name()  { return m_name; }

	bool hasServerDropzones();
	bool exists(class Server &server);

	ServerDropzone *getServerDropzone(Server &server);

	bool create(bool empty);
	bool create(class Server &server, bool empty);

	void unlink();
	void remove();

	class Scope *getVars();
	DMArray *find(const char* pattern);

	void addRef();
	void releaseRef();

	void addRepository(class Repository *repo);

	List<class RepositoryIgnorePattern> *generateIgnoreList();
	void generateDmInfos();

	static char *dirname(const char *filename);
	static char *slashify(const char *filename);

	void reconcile(List<class RepositoryIgnorePattern> *ignores);

	void dump();

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_DROPZONE; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//class IObject *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject();

	friend class DropzoneCallback;
	friend class ModifyStmtImpl;
};


class DMAPI_API DropzoneCallback
{
private:
	class Repository *m_repo;
	DMINT32 m_repoImplId;
	IDropzone &m_dz;
	class ComponentItem *m_item;
	List<DropzoneFile> *m_files;
	int m_total;
	int m_success;
	int m_failed;
	int m_notProcessed;
	class Context *m_ctx;

private:
	void checkout_from_repository(class Repository *repo);

	void checkin_to_repository(class Repository *repo);

public:
	// Summary
	DropzoneCallback(IDropzone &dz);

	// Per Repository Impl
	DropzoneCallback(
		class Repository *repo, DMINT32 repoImplId,
		IDropzone &dz, class Context &ctx, class ComponentItem *item = NULL);

	~DropzoneCallback();

	class Repository *repo()  { return m_repo; }
	DMINT32 repoImplId()      { return m_repoImplId; }

	int total()         { return m_total; }
	int success()       { return m_success; }
	int failed()        { return m_failed; }
	int notProcessed()  { return m_notProcessed; }


	void checked_out_file(class RepositoryImpl *impl, const char *repopath, const char *dzpath, const char *version);

	/**
	 * Called to indicate where a repository folder should have its dm.info
	 * file located.
	 *
	 * folder  - full repository folder path in repository format (as listed in
	 *           dm_repositorytextpattern)
	 * infoloc - relative path of target folder in dropzone (using platform-
	 *           specific path)
	 * fixed   - if true indicates that the location is the same for all
	 *           parents of this folder, if false, then the parent file locations
	 *           are automatically calculated
	 */
	void checked_out_folder(const char *repopath, const char *infoloc, bool fixed);

	void checkout_summary(int total, int success, int failed, int notProcessed);

	void checked_in_file(const char *repopath, const char *dzpath, const char *version);

	void checkin_file_error(const char *dzpath, const char *errormsg);

	void checkin_summary(int total, int success, int failed, int notProcessed);

	bool mkdir(const char *dirame);
	bool dirExists(const char *dirname);

	friend class Repository;
};


#endif /*__dropzone_h*/
