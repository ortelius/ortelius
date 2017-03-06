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
#ifndef __repositories_harvest_h

#define __repositories_harvest_h


#include "repository.h"
#include "credentials.h"
#include "extended.h"
#include "model.h"


typedef enum tagPathOption {
	PATH_OPTION_NOT_SPECIFIED,
	PATH_OPTION_PRESERVE,
	PATH_OPTION_PRESERVE_AND_CREATE,
	PATH_OPTION_ALL_TO_SAME
} PathOption;


typedef enum tagItemOption {
	ITEM_OPTION_NOT_SPECIFIED,
	ITEM_OPTION_NEW_ONLY,
	ITEM_OPTION_EXISTING_ONLY,
	ITEM_OPTION_NEW_AND_EXISTING
} ItemOption;


typedef enum tagReplaceOption {
	REPLACE_OPTION_NOT_SPECIFIED,
	REPLACE_OPTION_READ_ONLY,
	REPLACE_OPTION_READ_WRITE,
	REPLACE_OPTION_ALL
} ReplaceOption;


class HarvestOptions
{
private:
	bool m_isCheckin;

public:
	HarvestOptions(bool isCheckin);

	PathOption pathOption;
	ItemOption itemOption;
	bool createEmptyDirs;
	ReplaceOption replaceOption;

	void parseOptions(const char *options);
	void generateOptions(FILE *f);
};


///////////////////////////////////////////////////////////////////////////////
// HarvestRepository

class HarvestRepositoryImpl : public virtual RepositoryImpl
{
private:
	char *m_broker;
	char *m_project;
	char *m_state;
	char *m_viewpath;
	char *m_options;
	char *m_package;
	char *m_process;
	char *m_coprocess;
	class DM *m_dm;

	void handleCredentials(
		class ExtendedStmt &stmt, class Credentials *&creds,
		const char *& dfoFile, class Context &ctx);

public:
	HarvestRepositoryImpl(
		class RepositoryImplFactory &factory, DMINT32 implId, class Repository &repository,
		const char *broker, const char *project, const char *state, const char *viewpath,
		const char *options, const char *package, const char *process, const char *coprocess);

	virtual ~HarvestRepositoryImpl();

	bool testProvider(class Context &ctx);

	void checkout(
		const char *dzbase, const char *dzpath, bool recursive,
		class StringList *pattern, class ExtendedStmt &stmt,
		class DropzoneCallback &callback, class Context &ctx);

	void checkin(
		const char *dzbase, const char *dzpath, class ExtendedStmt &stmt,
		class DropzoneCallback &callback, class Context &ctx);

	char *dirname(const char *filename);

	// RepositoryImpl
	class Expr *getAttribute(const char *name, class Context &ctx);
};


class HarvestRepositoryImplFactory : public virtual RepositoryImplFactory
{
public:
	HarvestRepositoryImplFactory();

	RepositoryImpl *create(
		DMINT32 implId, class Repository &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


///////////////////////////////////////////////////////////////////////////////
// SetPackages

class Harvest_SetPackagesStmtImpl : public virtual ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;

public:
	Harvest_SetPackagesStmtImpl(class ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class Harvest_SetPackagesStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	Harvest_SetPackagesStmtImplFactory();

	bool allowsBody()		{ return false; }
	bool allowsPrePost()	{ return false; }
	bool isThreaded()       { return false; }

	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


///////////////////////////////////////////////////////////////////////////////
// CreatePackage

class Harvest_CreatePackageStmtImpl : public virtual ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;

public:
	Harvest_CreatePackageStmtImpl(class ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class Harvest_CreatePackageStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	Harvest_CreatePackageStmtImplFactory();

	bool allowsBody()		{ return false; }
	bool allowsPrePost()	{ return false; }
	bool isThreaded()       { return false; }

	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


///////////////////////////////////////////////////////////////////////////////
// PackageLoop

class Harvest_PackageLoopStmtImpl : public virtual ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;

public:
	Harvest_PackageLoopStmtImpl(class ExtendedStmt &parent);

	void execute(class Context &ctx);
};


class Harvest_PackageLoopStmtImplFactory : public virtual ExtendedStmtImplFactory
{
public:
	Harvest_PackageLoopStmtImplFactory();

	bool allowsBody()		{ return true; }
	bool allowsPrePost()	{ return false; }
	bool isThreaded()       { return false; }

	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


///////////////////////////////////////////////////////////////////////////////
// CredentialsProvider

class HarvestCredentialsProvider : public virtual CredentialsProvider
{
private:
	char *m_dfoFileName;

public:
	HarvestCredentialsProvider(
		class CredentialsProviderFactory &factory,
		class Credentials &credentials);
	~HarvestCredentialsProvider();

	bool canTransform();

	const char *transform(class Object &auth, class Context &ctx);
};


class HarvestCredentialsProviderFactory : public virtual CredentialsProviderFactory
{
public:
	HarvestCredentialsProviderFactory();

	CredentialsProvider *create(class Credentials &parent);
};


///////////////////////////////////////////////////////////////////////////////
// User

class HarvestUser : public IObject
{
private:
	long m_id;
	char *m_name;
	char *m_email;

public:
	HarvestUser(long id, const char *name, const char *email);
	~HarvestUser();

	long id()			{ return m_id; }
	const char *name()	{ return m_name; }

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_PLUGIN_OBJECT; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//class IObject *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject()  { return (Object*) 0; }
};


///////////////////////////////////////////////////////////////////////////////
// Package

class HarvestPackage : public IObject
{
private:
	class HarvestPluginObject &m_plugin;
	long m_id;
	char *m_name;
	class Scope *m_vars;
	HarvestUser *m_creator;
	bool m_creatorSet;
	HarvestUser *m_assignee;
	bool m_assigneeSet;
	List<HarvestUser> *m_contributors;
	class DMArray *m_contributorsCache;

	void setCreator(HarvestUser *creator);
	void setAssignee(HarvestUser *assignee);
	void addContributor(HarvestUser *contributor);

public:
	HarvestPackage(class HarvestPluginObject &plugin, long id, const char *name);
	~HarvestPackage();

	long id()			{ return m_id; }
	const char *name()  { return m_name; }

	// IObject
	OBJECT_KIND kind()  { return OBJ_KIND_PLUGIN_OBJECT; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	//class DMArray *getArrayAttribute(const char *name, class Context &ctx);
	//class IObject *getObjectAttribute(const char *name, class Context &ctx);
	class Object *toObject()  { return (Object*) 0; }

	class Scope *getVars();

	HarvestUser *getCreator();
	HarvestUser *getAssignee();
	List<HarvestUser> *getContributors();

	friend class HarvestPluginObject;
};


///////////////////////////////////////////////////////////////////////////////
// PluginObject

class HarvestPluginObject : public PluginObject
{
private:
	class Repository &m_repository;
	char *m_broker;
	char *m_project;
	List<HarvestPackage> m_packages;
	class triODBC *m_odbc;
	class DM *m_dm;

	void getCreator(HarvestPackage &pkg);
	void getAssignee(HarvestPackage &pkg);
	void getContributors(HarvestPackage &pkg);

public:
	HarvestPluginObject(
		class DM *dm,
		class Repository &repository,
		const char *broker,
		const char *project);
	~HarvestPluginObject();

	void connectToDatabase();
	void disconnectFromDatabase();
	HarvestPackage *findPackage(const char *pkg);
	void addPackage(HarvestPackage *pkg);
	List<HarvestPackage> &packages();

	friend class HarvestPackage;
};


#endif /*__repositories_harvest_h*/
