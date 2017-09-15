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
#ifndef __model_h

#define __model_h


#include "list.h"
#include "hashtable.h"
#include "dmapiexport.h"
#include "modelbase.h"
#include "timedjob.h"
#include "datetime.h"

#define EVERYONE_ID 1

//
// defines for finding objects by fully qualified name
//
#define OBJECT_NOT_FOUND		0
#define DOMAIN_NOT_FOUND		-1
#define DOMAIN_NOT_SPECIFIED	-2
#define DOMAIN_OBJECT_AMBIGUOUS	-3
#define DOMAIN_SYNTAX_ERROR		-4


typedef enum action_kind_tag {
	ACTION_KIND_SCRIPT			= 1,
	ACTION_KIND_IN_DB			= 2,
	ACTION_KIND_LOCAL_EXTERNAL	= 3,
	ACTION_KIND_REMOTE_EXTERNAL	= 4,
	ACTION_KIND_PLUGIN			= 5,
	ACTION_KIND_GRAPHICAL		= 6
} ACTION_KIND;


typedef enum password_kind_tag {
	CREDENTIALS_USE_DIALOG					=  0,		// Only used to indicate no credentials
	CREDENTIALS_ENCRYPTED					=  1,		// Encrypted value in memory
	CREDENTIALS_IN_DATABASE					=  2,		// Encrypted value in database
	CREDENTIALS_RTI3_DFO_IN_FILESYSTEM		=  3,
	CREDENTIALS_FROM_VARS					=  4,		// Values taken from variables
	CREDENTIALS_PRIVATE_KEY					=  5,		// Private Key file
	CREDENTIALS_HARVEST_DFO_IN_FILESYSTEM	= 10
	//DFO_IN_HARVEST=3,
	//CLEARTEXT_AS_DFO=4
} CREDENTIALS_KIND;


typedef enum line_end_format_tag{
	LINE_ENDS_OFF			= 0,
	LINE_ENDS_UNIX			= 1,
	LINE_ENDS_WINDOWS		= 2,
	LINE_ENDS_MAC			= 3
} LINE_END_FORMAT;


typedef enum task_kind_tag {
	TASK_UNKNOWN	= 0,
	TASK_DEPLOY		= 2,
	TASK_APPROVE	= 3,
	TASK_MOVE		= 4,
	TASK_COPY		= 5,
	TASK_REQUEST	= 8
} TASK_KIND;


typedef enum field_kind_tag {
	FIELD_KIND_CHECKBOX		= 1,
	FIELD_KIND_COMBO_ARRAY	= 2,
	FIELD_KIND_COMBO_DB		= 3,
	FIELD_KIND_COMBO_ACTION = 4,
	FIELD_KIND_TEXT			= 5
} FIELD_KIND;


class DMAPI_API Object : public IObject
{
protected:
	class Model &m_model;
	int m_id;
	char *m_name;
	class User *m_owner;
	class UserGroup *m_owngrp;
	bool m_ownerSet;
	class Domain *m_domain;
	bool m_domainSet;
	char *m_summary;
	char *m_notes;
	class Scope *m_vars;
	HashtableById<class ObjectAccess> *m_access;
	char *m_cacheFQDomain;
	char *m_cacheFQName;
	bool m_metaDataSet;
	int m_creationtime;
	int m_modifiedtime;
	class User *m_creator;
	class User *m_modifier;

	Object(class Model &model, int id, const char *name);
	virtual ~Object();

	bool internalHasDomainAccess(class User *user);

	//void addAccess(class UserGroup &usrgrp, class ObjectAccess *oa);

public:
	int id()			{ return m_id; }
	const char *name()  { return m_name; }
	//virtual OBJECT_KIND kind() = 0;
	virtual const char *table() = 0;
	virtual const char *fk() = 0;

	virtual class Object *getOwner();
	virtual class DateTime *getCreationTime();
	virtual class DateTime *getModifiedTime();
	virtual class User *getCreator();
	virtual class User *getModifier();

	void setOwner(class User *owner);
	void setOwner(class UserGroup *owngrp);
	void setCreator(class User *creator);
	void setModifier(class User *modifier);
	void setCreationTime(int created);
	void setModifiedTime(int modified);
	virtual bool hasAccess(class User *user);
	virtual bool hasReadAccess(class User *user);
	virtual bool hasWriteAccess(class User *user);

	class Domain *getDomain();
	void setDomain(class Domain *domain);
	const char *getFQDomain();
	const char *getFQName();

	const char *getSummary();
	void setSummary(const char *summary);
	const char *getNotes();
	void setNotes(const char *notes);
	// void setBuildId(int buildid);

	void indent(int indent);
	virtual void print(int indent) = 0;
	void printObject(int indent);

	// IObject
	virtual class Expr *getAttribute(const char *name, class Context &ctx);
	virtual class Object *toObject() { return this; }

	virtual class Scope *getVars();

	friend class Model;
};


class DMAPI_API ObjectKindAndId
{
private:
	OBJECT_KIND m_objkind;
	int m_id;

public:
	ObjectKindAndId(const char *otid);
	ObjectKindAndId(OBJECT_KIND objkind, int id);
	ObjectKindAndId(Object &obj);

	OBJECT_KIND getObjectKind()  { return m_objkind; }
	int getId()  { return m_id; }
	
	char *toString();
};


class DMAPI_API ProviderObject : public Object
{
protected:
	class Credentials *m_credentials;
	Hashtable<class Property> *m_props;
	class ProviderObjectDef *m_def;

	ProviderObject(class Model &model, int id, const char *name);
	virtual ~ProviderObject();

public:
	virtual const char *def_kind() = 0;

	virtual class Expr *getAttribute(const char *name, class Context &ctx);

	class Credentials *getCredentials();
	void setCredentials(class Credentials *creds);

	class ProviderObjectDef &getDef(class Context &ctx);

	class Property *getProperty(const char *name);
	void setProperty(class Property *prop);

	class Property *getDefProperty(const char *name);
	void setDefProperty(class Property *prop);

	virtual bool testProvider(class ExtendedStmt &stmt, class Context &ctx) = 0;

	virtual void printProviderObject(int indent);
};


class DMAPI_API ProviderObjectDef
{
private:
	int m_id;
	char *m_name;
	class Plugin *m_plugin;
	char *m_kind;
	Hashtable<class Property> *m_defprops;
	List<class PropertyDef> *m_propdefs;
	class PropertyChecker *m_checker;

	ProviderObjectDef(int id, const char *name, const char *kind, Plugin *plugin);

	void add(class PropertyDef *propdef);

	void createChecker();

public:
	~ProviderObjectDef();

	int id()			{ return m_id; }
	const char *name()	{ return m_name; }

	List<class PropertyDef> *getPropertyDefs();

	Plugin *getPlugin();

	void verifyProperties(class Context &ctx, Hashtable<class Property> *props);
	void verifyArgs(class Context &ctx, class StmtList *args, Hashtable<class Property> *props);

	class Property *getDefProperty(const char *name);
	void setDefProperty(class Property *prop);

	friend class Model;
	friend class Credentials;	// Temp
};


class DMAPI_API ProviderObjectImpl : public IObject
{
protected:
	DMINT32 m_implId;
	ProviderObject &m_provider;
	class DelegatingScope *m_vars;

public:
	ProviderObjectImpl(DMINT32 implId, ProviderObject &prov);
	virtual ~ProviderObjectImpl();

	DMINT32 implId()				{ return m_implId; }
	ProviderObject &provider()  { return m_provider; }

	class Scope *getVars();

	virtual class Expr *getAttribute(const char *name, class Context &ctx);

	virtual bool testProvider(class Context &ctx) = 0;
};


class DMAPI_API Credentials : public Object
{
private:
	// now part of object
	// int   m_id;
	// char *m_name;
	CREDENTIALS_KIND m_credkind;
	char *m_encusername;
	char *m_encpassword;
	char *m_filename;
	Hashtable<class CredentialsProvider> *m_impls;

	char *internalDecryptValue(const char *value, class Context &ctx);

	class CredentialsProvider *createImpl(const char *provider, class Context &ctx);

public:
	Credentials(class Model &model,CREDENTIALS_KIND credkind);
	Credentials(class Model &model,int id, const char *name, CREDENTIALS_KIND credkind, const char *encusername, const char *encpassword);
	Credentials(class Model &model,int id, const char *name, CREDENTIALS_KIND credkind, const char *filename);
	Credentials(class Model &model,int id, const char *name, const char *username, const char *filename);
	Credentials(class Model &model,class Property *username, class Property *password);
	~Credentials();

	// now part of object
	// int id()				 { return m_id; }
	CREDENTIALS_KIND credkind()  { return m_credkind; }
	OBJECT_KIND kind()	 { return OBJ_KIND_CREDENTIALS; }
	class Expr *getAttribute(const char *name, class Context &ctx);

	char *getDecryptedUsername(class Context &ctx);
	char *getDecryptedPassword(class Context &ctx);
	bool hasUsername();
	bool hasPassword();
	char *filename(class Context &ctx);

	bool canTransform(const char *provider, class Context &ctx);
	const char *transform(const char *provider, class Object &auth, class Context &ctx);

	void print(int indent);

	const char *table()  { return "dm_credentials"; }
	const char *fk()     { return "credid"; }

};


class DMAPI_API Environment : public Object
{
private:
	List<class Server> *m_servers;
	List<class Application> *m_apps;
	char *m_basedir;
	Credentials *m_credentials;
	class DMArray *m_serversArrayCache;

	Environment(
		class Model &model, int id, const char *name, const char *basedir);

	const char *table()  { return "dm_environment"; }
	const char *fk()     { return "envid"; }

	void setCredentials(Credentials *creds);

	void add(class Server *server);
	void add(class Application *app);

public:
	~Environment();

	OBJECT_KIND kind()	 { return OBJ_KIND_ENVIRONMENT; }

	const char *basedir()   { return m_basedir; }

	class Expr *getAttribute(const char *name, class Context &ctx);
	class Scope *getVars();
	void setVar(const char *attname,Expr *attval);

	//class Domain *getDomain();
	List<class Server> *getServers();
	List<class Application> *getAllowedApplications();
	Application *getDeployedApplication(Application &app);
	Credentials *getCredentials();

	class Server *server(const char *name);
	class Application *application(const char *name);
	bool isAvailable();

	void print(int indent);

	friend class Model;
};


class DMAPI_API Domain : public Object
{
private:
	List<class Domain>		*m_doms;
	List<Environment>       *m_envs;
	List<class Application>	*m_apps;
	List<class Repository>	*m_reps;
	List<class Notify>		*m_nfys;
	List<class Datasource>	*m_dats;
	class DMArray *m_childrenCache;

	int m_pos;
	bool m_lifecycle;

	Domain(class Model &model, int id, const char *name);

	const char *table()  { return "dm_domain"; }
	const char *fk()     { return "domainid"; }

	void add(class Domain *dom);
	void add(Environment *env);
	void add(class Application *app);
	void add(class Repository *rep);
	void add(class Notify *nfy);
	void add(class Datasource *dat);

public:
	~Domain();

	OBJECT_KIND kind()	 { return OBJ_KIND_DOMAIN; }

	List<class Domain>		*getSubdomains();
	List<Environment>   	*getEnvironments();
	List<class Application>	*getApplications();
	List<class Repository>	*getRepositories();
	List<class Notify>		*getNotifys();
	List<class Datasource>	*getDatasources();
	void					setPosition(int pos);
	void					setLifecycle(bool lc);
	int						getPosition();
	bool					getLifecycle();

	virtual Expr *getAttribute(const char *name, class Context &ctx);

	void print(int indent);

	friend class Model;
};


class DMAPI_API ServerType
{
private:
	int m_id;
	char *m_hosttype;
	LINE_END_FORMAT m_lineends;
	char *m_pathformat;
	char *m_scriptext;

	ServerType(
		int id, const char *hosttype, LINE_END_FORMAT lineends,
		const char *pathformat,const char *scriptext);

public:
	~ServerType();

	int id()				   { return m_id;        }
	const char *hosttype()	   { return m_hosttype;  }
	LINE_END_FORMAT lineends() { return m_lineends;  }
	const char *scriptext()    { return m_scriptext; }

	class PathNameImpl *createPath(const char* path);

	friend class Model;
};

class DMAPI_API Defect : public Object
{
private:
	// Bug Id and Title are id and name of "object"
	char *m_title;
	char *m_status;
	char *m_api_url;
	char *m_html_url;

	Defect(class Model &model,char *bugid,char *title,char *status,char *api_url,char *html_url);

	const char *table()  { return "dm_defects"; }
	const char *fk()     { return "bugid"; }


public:
	~Defect();
	char *getApiUrl()						{ return m_api_url; }
	char *getHtmlUrl()						{ return m_html_url; }
	OBJECT_KIND kind()						{ return OBJ_KIND_DEFECT; }
	Object *getOwner()						{ return (Object*) 0; }
	bool hasAccess(class User *user)		{ return true; }
	bool hasReadAccess(class User *user)	{ return true; }
	bool hasWriteAccess(class User *user)	{ return true; }
	void print(int indent);
	Expr *getAttribute(const char *name, class Context &ctx);

friend class Model;
};

class DMAPI_API Server : public Object
{
private:
	char *m_hostname;
	char *m_protocol;
	char *m_basedir;
	char *m_uname;
	Credentials *m_credentials;
	ServerType *m_servertype;
	class NotifyTemplate *m_PingTemplate;
	class NotifyTemplate *m_MD5Template;
	int m_sshport;

	Server(
		class Model &model, int id, const char *name,
		const char *hostname, const char *protocol,
		const char *basedir, ServerType *servertype, const char *uname);

	const char *table()  { return "dm_server"; }
	const char *fk()     { return "serverid"; }

	void setCredentials(Credentials *creds);

public:
	~Server();

	OBJECT_KIND kind()	 { return OBJ_KIND_SERVER; }

	const char	*hostname();
	const char	*protocol()      { return m_protocol; }
	const char	*basedir()       { return m_basedir; }
	const char	*uname()         { return m_uname; }
	void		setUname(const char *uname);

	class ServerType *serverType()  { return m_servertype; }
	LINE_END_FORMAT lineends();

	class PathNameImpl *createPath(const char* path);

	class Expr *getAttribute(const char *name, class Context &ctx);
	class Scope *getVars();
	void setVar(const char *attname,Expr *attval);
	class Credentials *getCredentials();

	class Component *getDeployedComponent(class Component &comp);
	class NotifyTemplate *getPingTemplate(class Context &ctx);
	class NotifyTemplate *getMD5Template(class Context &ctx);

	void setSSHPort(int portnum);
	int getSSHPort();

	void print(int indent);

	friend class Model;
};


class DMAPI_API Application : public Object
{
protected:
	class Action *m_action;
	class Action *m_preAction;
	class Action *m_postAction;
	bool m_actionsSet;
	class NotifyTemplate *m_successTemplate;
	class NotifyTemplate *m_failureTemplate;
	List<class Component> *m_components;
	class ApplicationComponentGraph *m_componentGraph;
	class ReleaseApplicationGraph *m_applicationGraph;
	Hashtable<class ApplicationVersion> *m_versionsCache;
	// Release
	class DMThread *m_thread;
	bool m_isRelease;
	List<class Application> m_children;
	List<class Application> m_parents;			// pending
	List<class Application> m_completed;		// completed parents
	
	

	Application(class Model &model, int id, const char *name);

	const char *table()  { return "dm_application"; }
	const char *fk()     { return "appid"; }

	void setActions(
		class Action *action,
		class Action *preAction,
		class Action *postAction);

	void addComponent(Component *comp);

public:
	virtual ~Application();

	OBJECT_KIND kind()	 { return OBJ_KIND_APPLICATION; }
	virtual class Expr *getAttribute(const char *name, class Context &ctx);
	class Scope *getVars();
	void setVar(const char *attname,Expr *attval);

	class Action *getCustomAction();
	class Action *getPreAction();
	class Action *getPostAction();
	class NotifyTemplate *getSuccessTemplate(Context &ctx);
	class NotifyTemplate *getFailureTemplate(Context &ctx);
	List<class Component> *getComponents();
	ApplicationComponentGraph *getApplicationComponentGraph();
	ReleaseApplicationGraph *getReleaseApplicationGraph();

	virtual Application *getParent()            { return NULL; }
	virtual List<Application> *getParentList();
	class ApplicationVersion *getVersion(const char *name);
	bool isAvailable(Environment &env);
	List<Domain> *getApprovalDomains(bool Approved=true);
	List<Defect> *getDefects(bool onlyAppDefects=false);
	List<Application> *getChildren(bool AllDescendents=false);
	// Release
	bool isRelease();
	void setRelease(bool rel);
	void addChild(Application *av);
	void addParent(Application *av);
	void markParentCompleted(Application *av);
	void dump();
	bool canDeploy();
	void markCompleted(Context &ctx);
	void deploy(Context &ctx);
	void setThread(class DMThread *t);

	virtual class ApplicationVersion *toApplicationVersion();
	/*
	{ 
		return (ApplicationVersion *)this;
	}
	*/

	bool approve(class Domain &tgtDomain, bool approve, const char *note);
	bool move(class Domain &tgtDomain, const char *note);
	bool request(class Task &linkedTask, const char *note);
	void recordDeployedToEnv(class DM &dm, class Environment &env, bool success);

	class ApplicationVersion *newVersion(
		Domain &tgtDomain, Application *predecessor);
	class ApplicationVersion *getLatestVersion(char *branchid = (char *)0);

	char *getBranchName();

	void print(int indent);

	friend class Model;
};


class DMAPI_API ApplicationVersion : public Application
{
private:
	char *m_version;
	Application *m_parent;

	class ApplicationVersion *m_predecessor;
	bool m_predecessorSet;
	List<ApplicationVersion> *m_successors;
	class DMArray *m_approvalsArrayCache;

	ApplicationVersion(
		class Model &model, int id, const char *name,
		const char *version, Application *parent);

	void setPredecessor(ApplicationVersion *predecessor);
	void addSuccessor(ApplicationVersion *successor);

public:
	~ApplicationVersion();

	OBJECT_KIND kind()	 { return OBJ_KIND_APPVERSION; }
	class Expr *getAttribute(const char *name, class Context &ctx);

	Application *getParent()  { return m_parent; }
	List<Application> *getParentList();
	ApplicationVersion *getPredecessor();
	List<ApplicationVersion> *getSuccessors();
	class DMArray *getApprovals();

	ApplicationVersion *toApplicationVersion()  { return this; }

	ApplicationVersion *operator_plus(int incr, class Context &ctx) const;
	ApplicationVersion *operator_minus(int decr, class Context &ctx) const;
	// void alterVars(class DMArray *attrs);

	friend class Model;
};


typedef enum tagComponentFilter {
	OFF = 0,
	ON = 1,
	ALL = 2
} ComponentFilter;


class DMAPI_API ComponentItem : public Object
{
private:
	//class Model &m_model;
	//int m_id;
	class Component &m_parent;
	int m_repoid;
	char *m_target;
	ComponentFilter m_rollup;
	ComponentFilter m_rollback;
	Hashtable<class Property> *m_props;
	class Repository *m_repo;

	ComponentItem(
		class Model &model, int id, const char *name, class Component &parent,
		int repoid, const char *target, ComponentFilter rollup, ComponentFilter rollback);

	const char *table()  { return "dm_componentitem"; }
	const char *fk()     { return "compitemid"; }

	int repoid()  { return m_repoid; }
	void setRepository(class Repository *repo);

public:
	~ComponentItem();

	int id()						{ return m_id; }
	OBJECT_KIND kind()				{ return OBJ_KIND_COMPONENTITEM; }
	class Component& component()	{ return m_parent; }
	//Object *toObject()				{ return NULL; }

	const char *getTargetFolder()  { return m_target; }
	class Repository *getRepository();

	ComponentFilter getRollup()    { return m_rollup; }
	ComponentFilter getRollback()  { return m_rollback; }

	class Property *getProperty(const char *name);
	void setProperty(class Property *prop);
	class StmtList *getPropertiesAsArgs();

	class Expr *getAttribute(const char *name, class Context &ctx);

	void print(int indent);

	friend class Model;
};



class DMAPI_API Component : public Object
{
private:
	ComponentFilter m_rollup;
	ComponentFilter m_rollback;
	bool m_filterItems;
	bool m_deployAlways;
	bool m_deploySequentially;
	List<ComponentItem> *m_items;
	class DMArray *m_itemsArrayCache;
	class DMArray *m_serversArrayCache;
	class Action *m_action;
	class Action *m_preAction;
	class Action *m_postAction;
	bool m_actionsSet;
	char *m_basedir;
	int m_parentid;
	int m_predecessorid;
	int m_datasourceid;
	int m_buildid;

	Component(
		class Model &model, int id, const char *name, const char *basedir,
		ComponentFilter rollup, ComponentFilter rollback,
		bool filterItems, bool deployAlways, bool deploySequentially, int parentid,
		int predecessorid, int datasourceid, int buildid);

	const char *table()  { return "dm_component"; }
	const char *fk()     { return "compid"; }

	void add(ComponentItem *item);

	void setActions(
		class Action *action,
		class Action *preAction,
		class Action *postAction);

public:
	~Component();

	OBJECT_KIND kind()	 { return OBJ_KIND_COMPONENT; }
	class Expr *getAttribute(const char *name, class Context &ctx);
	class Scope *getVars();

	const char *basedir() { return m_basedir; }

	ComponentFilter getRollup()    { return m_rollup; }
	ComponentFilter getRollback()  { return m_rollback; }
	bool getFilterItems()          { return m_filterItems; }
	bool isDeployAlways()          { return m_deployAlways; }
	bool isDeploySequentially()    { return m_deploySequentially; }
	int getParentId()              { return m_parentid; }	
	int getPredecessorId()         { return m_predecessorid; }
	int buildid()				   { return m_buildid; }
	int datasourceid()             { return m_datasourceid; }
	
	
	List<Defect> *getDefects();
	List<Server> *getServerSubset(class Context &ctx);
	List<ComponentItem> *getItems();

	class Action *getCustomAction();
	class Action *getPreAction();
	class Action *getPostAction();

	void recordDeployedToServer(class DM &dm, class Server &server, bool success);
	void setBuildId(int buildid);

	void print(int indent);

	StringList *getFilesForLastDeployment(class Server &s,int deployid);

	friend class Model;
};


class DMAPI_API ApplicationComponent
{
private:
	class Component &m_comp;
	List<class ApplicationComponent> m_children;
	List<class ApplicationComponent> m_parents;			// pending
	List<class ApplicationComponent> m_completed;		// completed parents
	class DMThread *m_thread;

	void markParentCompleted(ApplicationComponent *parent);

public:
	ApplicationComponent(class Component &comp);
	virtual ~ApplicationComponent();

	class Component &component()  { return m_comp; }

	void addChild(ApplicationComponent *ac);
	void addParent(ApplicationComponent *ac);

	void setThread(class DMThread *t);

	void markCompleted(class Context &ctx,int retcode);
	bool canDeploy();
	void deploy(class Context &ctx);
	void stop(class Context &ctx);

	void dump();
	void dump(class Context &ctx);
};


class DMAPI_API ApplicationComponentGraph
{
private:
	HashtableById<class ApplicationComponent> m_appComps;
	List<class ApplicationComponent> m_starts;

public:
	ApplicationComponentGraph();
	virtual ~ApplicationComponentGraph();

	void addComponent(Component &comp);
	void addFlow(int compfromid, int comptoid);

	int deploy(class Context &ctx);

	void dump();
	void dump(class Context &ctx);
};

// Release
class DMAPI_API ReleaseApplicationGraph
{
private:
	HashtableById<class Application> m_apps;
	List<class Application> m_starts;

public:
	ReleaseApplicationGraph();
	virtual ~ReleaseApplicationGraph();

	void addApplication(Application *app);
	void addFlow(int appfromid, int apptoid);

	int deploy(class Context &ctx);

	void dump();
	List<class Application> *getApplications();
};



class DMAPI_API User : public Object
{
private:
	char *m_email;
	char *m_realname;
	char *m_phone;
	List<class UserGroup> *m_groups;
	class DMArray *m_groupsArrayCache;
	char *m_accessibleDomainsCache;

	User(
		class Model &model, int id, const char *name, const char *email,
		const char *realname, const char *phone);

	const char *table()  { return "dm_user"; }
	const char *fk()     { return (const char*) 0; }

public:
	~User();

	OBJECT_KIND kind()	 { return OBJ_KIND_USER; }

	Object *getOwner()   { return (Object*) 0; }

	class Expr *getAttribute(const char *name, class Context &ctx);
	List<class UserGroup> *getUserGroups();
	void add(class UserGroup *group);
	bool validateHashedPassword(const char *passhash);
	const char *getAccessibleDomains();

	void print(int indent);

	friend class Model;
};


class DMAPI_API UserGroup : public Object
{
private:
	char *m_email;
	List<User> *m_users;
	class DMArray *m_usersArrayCache;
	static UserGroup *s_everyone;

	UserGroup(class Model &model, int id, const char *name, const char *email);

	const char *table()  { return "dm_usergroup"; }
	const char *fk()     { return (const char*) 0; }

public:
	~UserGroup();

	OBJECT_KIND kind()	 { return OBJ_KIND_USERGROUP; }

	Object *getOwner()   { return (Object*) 0; }

	class Expr *getAttribute(const char *name, class Context &ctx);
	List<User> *getUsers();
	void add(User *user);

	void print(int indent);

	static UserGroup *getEveryoneGroup(class Model &model);

	friend class Model;
};


class DMAPI_API Repository : public ProviderObject
{
private:
	HashtableById<class RepositoryImpl> *m_impls;

	Repository(class Model &model, int id, const char *name);

	const char *table()				{ return s_table(); }
	const char *fk()				{ return "repositoryid"; }
	const char *def_kind()			{ return "repository"; }
	static const char *s_table()	{ return "dm_repository"; }

	class RepositoryImpl *createImpl(
		DMINT32 implId, class ExtendedStmt &stmt, class AuditEntry *entry, class Context &ctx);

public:
	~Repository();

	OBJECT_KIND kind()  { return OBJ_KIND_REPOSITORY; }

	class Scope *getVars();
	class Scope *getVars(DMINT32 implId, class ExtendedStmt &stmt, class AuditEntry *entry, class Context &ctx);

	bool testProvider(class ExtendedStmt &stmt, class Context &ctx);

	void checkout(
		const char *dzbase, const char *dzpath, class ExtendedStmt &stmt,
		class AuditEntry *entry, class DropzoneCallback &cb, class Context &ctx);

	void checkin(
		const char *dzbase, const char *dzpath, class ExtendedStmt &stmt,
		class AuditEntry *entry, class DropzoneCallback &cb, class Context &ctx);

	void generateDmInfos(const char *dropzone);

	List<class RepositoryTextPattern> *getTextPatternsForPath(const char *path);
	List<class RepositoryIgnorePattern> *getIgnorePatterns();

	char *dirname(const char *filename);

	void print(int indent);

	friend class Model;
};


class DMAPI_API Notify : public ProviderObject
{
private:
	HashtableById<class NotifyProviderImpl> *m_impls;

	Notify(class Model &model, int id, const char *name);

	const char *table()				{ return s_table(); }
	const char *fk()				{ return "notifyid"; }
	const char *def_kind()			{ return "notify"; }
	static const char *s_table()	{ return "dm_notify"; }

	class NotifyProviderImpl *createImpl(
		DMINT32 implId, class ExtendedStmt &stmt, class AuditEntry *entry, class Context &ctx);

public:
	~Notify();

	OBJECT_KIND kind()  { return OBJ_KIND_NOTIFY; }

	class Scope *getVars();
	class Scope *getVars(
		DMINT32 implId, class ExtendedStmt &stmt, class AuditEntry *entry,
		class Context &ctx);

	bool testProvider(class ExtendedStmt &stmt, class Context &ctx);

	void notify(
		DMINT32 implId, class ExtendedStmt &stmt, class AuditEntry *entry,
		class OutputStream &body, List<class Attachment> *atts,
		class Context &ctx);

	void print(int indent);

	friend class Model;
};


class DMAPI_API Datasource : public ProviderObject
{
private:
	HashtableById<class DatasourceProviderImpl> *m_impls;

	Datasource(class Model &model, int id, const char *name);

	const char *table()				{ return s_table(); }
	const char *fk()				{ return "datasourceid"; }
	const char *def_kind()			{ return "datasource"; }
	static const char *s_table()	{ return "dm_datasource"; }

	class DatasourceProviderImpl *createImpl(
		DMINT32 implId, class ExtendedStmt &stmt,
		class AuditEntry *entry, class Context &stack);

public:
	~Datasource();

	OBJECT_KIND kind()				{ return OBJ_KIND_DATASOURCE; }

	class Scope *getVars();
	class Scope *getVars(
		DMINT32 implId, class ExtendedStmt &stmt,
		class AuditEntry *entry, class Context &ctx);

	bool testProvider(class ExtendedStmt &stmt, class Context &ctx);

	void query(
		DMINT32 implId, class ExtendedStmt &stmt,
		class AuditEntry *entry, class Context &ctx);

	void print(int indent);

	friend class Model;
};


class DMAPI_API Task : public Object
{
private:
	HashtableById<class TaskImpl> *m_impls;

	char *m_taskType;
	class Action *m_preAction;
	class Action *m_postAction;
	bool m_actionsSet;

	const char *table()				{ return "dm_task"; }
	const char *fk()				{ return "taskid"; }

	void setActions(class Action *preAction, class Action *postAction);

	class TaskImpl *createImpl(
		DMINT32 implId, class ExtendedStmt &stmt,
		class AuditEntry *entry, class Context &stack);

public:
	Task(class Model &model, int id, const char *name, const char *kind);
	~Task();

	OBJECT_KIND kind()				{ return OBJ_KIND_TASK; }
	const char *taskType()			{ return m_taskType; }

	class Action *getPreAction();
	class Action *getPostAction();

	class Scope *getVars();
	class Scope *getVars(
		DMINT32 implId, class ExtendedStmt &stmt,
		class AuditEntry *entry, class Context &ctx);

	int run(
		DMINT32 implId, class ExtendedStmt &stmt,
		class AuditEntry *entry, class Context &ctx);

	class Domain *getApprovalDomain();		// For approve
	class Domain *getTargetDomain();		// For copy/move
	class Task *getLinkedTask();			// For request

	class NotifyTemplate *getSuccessTemplate(Context &ctx);
	class NotifyTemplate *getFailureTemplate(Context &ctx);

	virtual void print(int indent);

	friend class Model;
};


/**
 * A deployment manager script - referred to by filepath and repositoryid or
 * name and db textid.  parse() checks out the script and parses it, with the
 * actions ending up in the model's global list of actions.
 */
class DMAPI_API Script
{
private:
	Model &m_model;
	int m_repoid;
	char *m_filepath;
	Action *m_action;
	int m_textid;
	Repository *m_repo;
	bool m_parsed;
	char *m_checksum;
	// TODO: Actions should probably be declared here

	Script(class Model &model, int repoid, const char *filepath);
	Script(class Model &model, Action *action, int textid);

	int repoid()	{ return m_repoid; }
	void setRepository(class Repository *repo);

	class Dropzone &getScriptDropzone(class Context &ctx);
	

public:
	~Script();

	char *getFileContent(const char *filename);

	class Repository *getRepository();

	int parse(Action &action, Context &ctx);
	const char *checksum()  { return m_checksum; }

	void print(int indent);

	friend class Model;
};


typedef enum tagSwitchMode {
	SWITCH_MODE_NONE,
	SWITCH_MODE_SWITCH,
	SWITCH_MODE_PREFIX,
	SWITCH_MODE_ALWAYS,
	SWITCH_MODE_BOOLEAN
} SwitchMode;

/**
 * Definition of an argument to an external script used for mapping arguments
 * to positional and switched parameters.
 */
class ActionArg
{
private:
	char *m_name;
	int m_position;
	bool m_required;
	bool m_pad;
	SwitchMode m_mode;
	char *m_switch;
	char *m_negswitch;
	char *m_type;

	ActionArg(
		const char *name, int pos, bool reqd, bool pad,
		SwitchMode mode, const char *sw, const char *nsw, const char *type);

	void process(const char *val, class StringList &params, class Context &ctx);
	char *getExpandedSwitch(class Context &ctx);
	char *getExpandedNegSwitch(class Context &ctx);

public:
	~ActionArg();

	const char *name()			{ return m_name; }
	int position()				{ return m_position; }
	bool required()				{ return m_required; }
	bool pad()					{ return m_pad; }
	SwitchMode getMode()		{ return m_mode; }
	const char *getSwitch()		{ return m_switch; }
	const char *getNegSwitch()	{ return m_negswitch; }
	char *getType()             { return m_type; }

	bool isAlways()  { return (m_mode == SWITCH_MODE_ALWAYS) ? true : false; }

	friend class Action;
	friend class Model;
};


class DMAPI_API Action : public Object
{
private:
	ACTION_KIND m_actkind;
	bool m_isFunction;
	//bool m_isRemote;
	bool m_copyToRemote;
	bool m_resultIsExpr;
	bool m_parsed;
	bool m_useTTY;
	Script *m_script;

	char *m_filepath;
	char *m_shell;
	List<ActionArg> *m_args;
	Hashtable<ActionArg> *m_actionArgMap;
	HashtableById<ActionArg> *m_functionArgMap;
	StringHashtable *m_serverTypeFilepaths;

	class Plugin *m_plugin;

	Action(
		class Model &model, int id, ACTION_KIND kind, bool isFunction,
		/*bool isRemote,*/ bool copyToRemote, bool resultIsExpr,
		const char *name, const char *filepath, const char *shell, bool useTTY);

	const char *table()				{ return "dm_action"; }
	const char *fk()				{ return "actionid"; }

	void setScript(class Script *script);
	void setPlugin(class Plugin *plugin);
	void setServerTypeFilepaths(StringHashtable *filepaths);

	void add(ActionArg *arg);

public:
	~Action();

	OBJECT_KIND kind()				{ return OBJ_KIND_ACTION; }
	ACTION_KIND actionKind()		{ return m_actkind; }

	//bool isRemote()					{ return (m_actkind == ACTION_KIND_REMOTE_EXTERNAL); }
	bool copyToRemote()				{ return m_copyToRemote; }
	bool resultIsExpr()				{ return m_resultIsExpr; }
	const char *filepath()			{ return m_filepath; }
	const char *shell()				{ return m_shell; }
	bool useTTY()					{ return m_useTTY; }
	bool isFunction()				{ return m_isFunction; }

	const char *filepath(ServerType &type);
	Hashtable<ActionArg> *getActionArgMap();
	HashtableById<ActionArg> *getFunctionArgMap();
	List<ActionArg> *getArgs();

	class StringList *mapArgsForFunction(
		class FunctionNode &func, class ExprList *args,
		class Context &ctx, class Envp *env = NULL);
	class StringList *mapArgsForAction(
		class ExtendedStmt &stmt,
		class Context &ctx, class Envp *env = NULL);

	char *getSignature();
	class Script *getScript();
	int parse(class Context &ctx);

	class Plugin *getPlugin();

	class ActionNode *getActionNode(class Context &ctx);

	class Action *getCalledAction(int actionid);

	void print(int indent);

	friend class Model;
};


class DMAPI_API Field
{
protected:
	class Model &m_model;
	int m_id;
	char *m_name;
	FIELD_KIND m_kind;

	class Action *m_action;

	char *m_querystring;
	class Datasource *m_datasource;

	Field(
		class Model &model, int id, const char *name,
		FIELD_KIND kind, const char *querystring);

	void setAction(class Action *script);

	void setDatasource(class Datasource *datasource);

public:
	~Field();

	int id()			{ return m_id; }
	const char *name()  { return m_name; }

	class Action *getAction();

	class Datasource *getDatasource();

	void populate(class DM &dm);

	friend class Model;
};


class Plugin
{
private:
	int m_id;
	int m_version;
	char *m_library;
	bool m_loaded;

	Plugin(int id, int version, const char *library);

public:
	~Plugin();

	int id()				{ return m_id; }
	const char *library()	{ return m_library; }

	bool loadPlugin(class DM &dm);

	friend class Model;
};


class DMAPI_API PluginObject
{
public:
	PluginObject();
	virtual ~PluginObject();
};


class RepositoryTextPattern
{
private:
	char *m_path;
	char *m_pattern;
	bool m_text;

	RepositoryTextPattern(const char *path, const char *pattern, bool text);

public:
	~RepositoryTextPattern();

	const char *path()		{ return m_path; }
	const char *pattern()	{ return m_pattern; }
	bool text()				{ return m_text; }

	friend class Model;
};


class RepositoryIgnorePattern
{
private:
	char *m_pattern;
	bool m_isDir;
	class RegExpWrapper *m_re;

	RepositoryIgnorePattern(const char *pattern, bool isDir);

public:
	~RepositoryIgnorePattern();

	const char *pattern()	{ return m_pattern; }
	bool isDir()			{ return m_isDir; }

	bool matches(const char *filepath, bool isDir);

	friend class Model;
};


#include "cache.h"


class DMAPI_API Model
{
private:
	class triODBC			&m_odbc;
	char					*m_engineHostname;
	int						 m_engineId;
	class EngineConfig		*m_config;
	int						 m_deploymentStartTime;
	List<Environment>		*m_envs;
	List<Application>		*m_apps;
	class ActionNodeList	*m_actionNodes;
	User					*m_currentUser;
	class Audit				*m_audit;
	char					*m_DomainList;
	CacheByIdAndName<Environment>	m_envCache;
	CacheByIdAndName<Credentials>	m_crdCache;
	CacheById<Domain>				m_domCache;
	CacheById<ServerType>			m_styCache;
	CacheByIdAndName<Server>		m_srvCache;
	CacheByIdAndName<Application>	m_appCache;
	CacheByIdAndName<Component>		m_comCache;
	CacheByIdAndName<User>			m_usrCache;
	CacheById<UserGroup>			m_usgCache;
	CacheByIdAndName<Repository>	m_repCache;
	CacheByIdAndName<Notify>		m_nfyCache;
	CacheByIdAndName<Datasource>	m_datCache;
	// CacheById<Credentials>			m_crdCache;
	CacheByIdAndName<Field>			m_fldCache;
	CacheByIdAndName<Action>		m_actCache;
	Hashtable<Script>				m_scrCache;
	CacheById<ProviderObjectDef>	m_podCache;
	CacheById<Plugin>				m_plgCache;
	Hashtable<PluginObject>			m_pluginObjects;
	CacheByIdAndName<Task>			m_tskCache;

	// Internals
	int getDomainID2(char *DomainName,int parent);	
	char *ProcessQualifiedName(const char *QualifiedName,int *domain);
	Environment *internalGetEnvironmentByName(const char *envname,bool throwRTE=false);
	Credentials *internalGetCredentialsByName(const char *credname,bool throwRTE=false);
	Application *internalGetApplicationByName(const char *appname,bool throwRTE=false);
	Component *internalGetComponentByName(const char *compname,bool throwRTE=false);
	User *internalGetUserByName(const char *username,bool throwRTE=false);
	Repository *internalGetRepositoryByName(const char *repname,bool throwRTE=false);
	Server *internalGetServerByName(const char *servname,bool throwRTE=false);
	UserGroup *internalGetUserGroupByName(const char *grpname,bool throwRTE=false);
	void internalGetChildrenOfApp(List<Application> *applist,Application &app,bool AllDescendents);
	
	List<Environment> *internalGetEnvironments(
		const char *whereClause, bool checkDomain);
	List<Server> *internalGetServers(
		const char *fromClause, const char *whereClause);
	List<Domain> *internalGetDomains(
		const char *fromClause, const char *whereClause);
	List<Application> *internalGetApplications(
		const char *fromClause, const char *whereClause, bool checkDomain);
	List<Component> *internalGetComponents(
		const char *fromClause, const char *whereClause);
	List<User> *internalGetUsers(
		const char *fromClause, const char *whereClause, bool checkDomain);
	List<UserGroup> *internalGetUserGroups(
		const char *fromClause, const char *whereClause, bool checkDomain);
	Credentials *internalGetCredentials(
		const char *fromClause, const char *whereClause);
	Action *internalGetAction(
		const char *fromClause, const char *whereClause, bool checkDomain);
	List<Property> *internalGetProperties(
		const char *table, const char *fk, int id, const char *fk2 = 0, int id2 = 0);

	template<class T> List<T> *internalGetProviderObjects(
		CacheByIdAndName<T> &cache, const char *fromClause,
		const char *whereClause, bool checkDomain);

	Environment *findOrCreateEnvironment(
		int id, const char* name, const char *basedir);
	Domain *findOrCreateDomain(int id, const char* name);
	ServerType *findOrCreateServerType(
		int id, const char *hosttype, LINE_END_FORMAT lineends,
		const char *pathformat, const char *scriptext);
	Server *findOrCreateServer(
		int id, const char *name, const char *hostname, const char *protocol,
		const char *basedir, ServerType *servertype, const char *uname);
	Application *findOrCreateApplication(int id, const char *name);
	ApplicationVersion *findOrCreateApplicationVersion(
		int id, const char *name, const char *version, Application *parent);
	Component *findOrCreateComponent(
		int id, const char *name, const char *basedir, ComponentFilter rollup,
		ComponentFilter rollback, bool filterItems, bool deployAlways, bool deploySequentially,
		int parentid, int predecessorid, int datasourceid, int buildid);
	User *findOrCreateUser(
		int id, const char *name, const char* email,
		const char *realname, const char *phone);
	UserGroup *findOrCreateUserGroup(int id, const char *name, const char* email);
	ProviderObjectDef *findOrCreateProviderObjectDef(
		int id, const char *name, const char *kind, Plugin *plugin);
	Credentials *findOrCreateCredentials(
		int id, const char *name, CREDENTIALS_KIND kind,
		char *encuser, const char *encpass, const char *filename);
	Action *findOrCreateAction(
		int id, ACTION_KIND kind, bool isFunc, /*bool isRemote,*/ bool copyTo,
		bool resIsExpr, const char *name, const char *filepath, const char *shell, bool useTTY);
	Script *findOrCreateScript(int repoid, const char *filepath);
	Script *findOrCreateScript(Action *action, int textid);
	Plugin *findOrCreatePlugin(int id, int version, const char *library);
	Task *findOrCreateTask(int id, const char *name, const char *kind);

	template<class T> void getProviderObjectsForDomain(
		CacheByIdAndName<T> &cache, Domain *domain);

	// Environment
	void getServersForEnvironment(Environment &env);
	void getApplicationsAllowedInEnvironment(Environment &env);
	Application *getApplicationDeployedToEnvironment(Environment &env, Application &app);
	//void getDomainForEnvironment(Environment &env);
	void getCredentialsForEnvironment(Environment &server);
	bool isEnvironmentAvailable(Environment &env);

	// Server
	Component *getComponentDeployedToServer(Server &server, Component &comp);
	void getCredentialsForServer(Server &server);
	List <Component> *getComponentsOnServer(Server *server);

	// Application
	void getActionsForApplication(Application &app);
	int addCompsForApp2(Application &app,HashtableById< List<Component> > &preds, int compid);
	void getComponentsForApplication(Application &app);
	ApplicationComponentGraph *getApplicationComponentGraph(Application &app);
	ReleaseApplicationGraph *getReleaseApplicationGraph(Application &app);
	ApplicationVersion *getVersionOfApplication(Application &app, const char *name);
	bool isApplicationAvailable(Application &app, Environment &env);
	bool approveApplication(
		Application &app, Domain &tgtDomain, bool approve, const char *note);
	long nextAutoDeployTime(Application &app,char **penv);
	bool isApproved(Application &app,Domain &domain);
	bool moveApplication(
		Application &app, Domain &tgtDomain, const char *note);
	bool request(
		Application &app,Task &linkedTask, const char *note);
	ApplicationVersion* newVersionOfApplication(
		Application &app, Domain &tgtDomain, Application *predecessor);
	ApplicationVersion* getLatestVersionOfApplication(
		Application &app, char *branch = (char *)0);
	void recordAppInEnv(
		class DM &dm, Application &app, Environment &env, bool success);
	void recordCompOnServ(
		class DM &dm, Component &comp, Server &server, bool success);

	// ApplicationVersion
	void getPredecessorForApplicationVersion(ApplicationVersion &av);
	void getSuccessorsForApplicationVersion(ApplicationVersion &av);
	class DMArray *getApprovalsForApplicationVersion(ApplicationVersion &av);
	void alterObjectVars(ApplicationVersion &av, class DMArray *attrs);

	// Component
	List<Server> *getAllServersForComponent(Component &comp, Environment *env);
	List<Server> *getServerSubsetForComponent(
		Component &comp, List<Server> &servers);
	void getItemsForComponent(Component &comp);
	void getActionsForComponent(Component &comp);
	StringList *getFilesForLastDeployment(Component &comp,Server &s,int deployid);

	// ComponentItem
	void getRepositoryForComponentItem(ComponentItem &ci);
	void getPropertiesForComponentItem(ComponentItem &ci);

	// Domain
	void getSubdomainsForDomain(Domain *domain);
	void getEnvironmentsForDomain(Domain *domain);
	void getApplicationsForDomain(Domain *domain);
	void getRepositoriesForDomain(Domain *domain);
	void getNotifysForDomain(Domain *domain);
	void getDatasourcesForDomain(Domain *domain);
	void getParentForDomain(Domain &domain);
	char *internalGetAccessibleChildDomains(int domainid);
	char *internalGetAccessibleParentDomains(int domainid);
	char *getAccessibleDomains(Domain &domain);

	// UserGroup
	void getUsersForUserGroup(UserGroup &group);

	// User
	void getUserGroupsForUser(User &user);
	DateTime *getLastLogin(int userid);

	// Object
	void getOwnerForObject(Object &obj);
	void getMetaDataForObject(Object &obj);
	void getVariablesForObject(Object &obj, Scope &vars);
	void setVariableForObject(Object *obj,const char *attname,Expr *attval);
	void addAccessForDomain(Domain &dom, HashtableById<class ObjectAccess> &ia);
	HashtableById<class ObjectAccess> *getAccessForObject(Object &obj);
	void getDomainForObject(Object &obj);
	void getSummaryForObject(Object &obj);
	void getNotesForObject(Object &obj);

	// ProviderObject
	void getCredentialsForProviderObject(ProviderObject &po);
	void getPropertiesForProviderObject(ProviderObject &obj);
	ProviderObjectDef *getDefForProviderObject(ProviderObject &obj);
	void getPropertyDefsForProviderObjectDef(ProviderObjectDef &pod);

	// ProviderObjectDef
	void getPropertiesForProviderObjectDef(ProviderObjectDef &def);
	List<RepositoryIgnorePattern> *getIgnorePatternsForRepositoryDef(
		ProviderObjectDef &def);

	// Repository
	List<RepositoryTextPattern> *getTextPatternsForRepositoryPath(
		Repository &repo, const char *path);

	// Notify
	char *getNotifyText(int textid);
	class NotifyTemplate *getSuccessTemplate(Context &ctx,int taskid);
	class NotifyTemplate *getFailureTemplate(Context &ctx,int taskid);
	class NotifyTemplate *getNotifyTemplate(Context &ctx,Task &task,bool useFailure = false);
	class NotifyTemplate *getApplicationTemplate(Context &ctx,Application &app,bool useFailure = false);
	class NotifyTemplate *internalGetTemplate(const char *query,Context &ctx,long id);
	class NotifyTemplate *getPingTemplate(Context &ctx,Server &server);
	class NotifyTemplate *getMD5Template(Context &ctx,Server &server);

	// Field
	void getActionForField(Field &field);
	StringList *getFieldValuesForField(Field &field);
	StringList *getFieldValuesForField(const char *querystring);

	// Script
	void getRepositoryForScript(Script &script);
	// class GraphicalScriptGenerator *createGraphicalScriptGenerator(Dropzone &dz, Action &action);

	// Action
	Action *getAction(const char *name);
	Action *getActionTask(Task &task);
	void getArgsForAction(Action &act);
	void getScriptForAction(Action *action);
	char *getActionText(int textid);
	void getPluginForAction(Action &act);
	void getServerTypeFilepathsForAction(Action &act);

	

	// Task
	Task *getTask(const char *name);
	Task *getTask(int id);
	User *waitForTask(Task &task,Application &app,int deploymentid);
	void getActionsForTask(Task &task);
	void removeRequest(long taskid,Context &ctx);

	// This is private so that no-one other than friends can change the user
	void setCurrentUser(User *user);
	void setDomainList(int domainid=0);
	void updateUserLastLogin(User &user);
	bool validateHashedPassword(User &user, const char *passhash);
	int getNextObjectId(const char *objectType);

	void setActionNodes(class ActionNodeList *actions);
	class ActionNode *firstActionNode();
	class ActionNode *getActionNode(const char *name);

	bool checkPropertyDefs(
		const char *name, List<class PropertyDef> *propdefs, int pdId);

	// Timed jobs
	bool DomainBelongsToEngine(int domainid);
	

public:
	Model(class triODBC &odbc, const char *engineHostname);
	~Model();

	class EngineConfig &getEngineConfig();

	void alterObjectVars(class Object *obj, class DMArray *attrs);

	int getDomainID(char *DomainName);
	int GetIDFromQualifiedName(const char *QualifiedName,char *TableName,bool throwRTE=false);

	List<Environment> *getEnvironments();
	List<Application> *getApplications();
	Environment *getEnvironment(const char *name,bool throwRTE=false);
	Credentials *getCredential(const char *name, bool throwRTE=false);
	Application *getApplication(const char *name, bool throwRTE=false);
	Server *getServer(const char *name, bool throwRTE=false);
	Environment *getEnvironment(int id);
	Credentials *getCredentials(int id);
	// Application *getApplication(const char *name);
	List <Application> *getApplications(Environment *env);
	Action *getAction(int id);
	Application *getApplication(int id);
	Server *getServer(int id);
	Component *getComponent(const char *name, bool throwRTE=false);
	DateTime *getDeployTime(Component *comp,Server *serv);
	DateTime *getDeployTime(Application *app,Environment *env);
	Component *getComponentById(int compid);
	User *getUser(const char *name,bool throwRTE=false);
	User *getUserById(int id);
	UserGroup *getUserGroup(int id);
	UserGroup *getUserGroup(const char *name,bool throwRTE=false);
	Repository *getRepository(const char *name, bool throwRTE=false);
	Repository *getRepository(int id);
	Notify *getNotifier(const char *name);
	Notify *getNotifier(int id);
	Datasource *getDatasource(const char *name);
	Datasource *getDatasource(int id);
	Server *getServerById(int id);
	void setUname(Server &s,const char *uname);

	Field *getField(const char *name);

	// ApproveTask
	Domain *getApprovalDomainForApproveTask(Task &task);
	Domain *getTargetDomainForCopyMoveTask(Task &task);
	Task *getLinkedTaskForRequestTask(Task &task);

	List<Domain> *getDomainsApprovedForApp(Application &app,bool approved=true);
	List<Defect> *getDefectsForApp(Application &app,bool onlyAppDefects=false);
	List<Defect> *getDefectsForComp(Component &comp);
	List <Application> *getChildrenOfApplication(Application &app, bool AllDescendents=false);

	char *getBranchName(int appid);

	User *getCurrentUser();

	List<TimedJob> *getTimedJobs();
	List<Server> *getServersToCheck();
	bool checkMD5(int serverid);

	class Audit &getAudit();
	class Audit &getDummyAudit();
	void addDeploymentAction(Action *action);

	void setPluginObject(const char *name, PluginObject *obj);
	PluginObject *getPluginObject(const char *name);

	void addProvider(int kind,const char *name,List<class PropertyDef> *propdefs, int pluginid = 0);
	bool installProviderImpl(
		const char *name, const char *plugin,
		int kind, List<class PropertyDef> *propdefs);

	class GraphicalScriptGenerator *createGraphicalScriptGenerator(Dropzone &dz, Action &action);

	friend class DM;
	friend class Object;
	friend class ProviderObject;
	friend class Environment;
	friend class Domain;
	friend class Server;
	friend class Application;
	friend class ApplicationVersion;
	friend class Component;
	friend class ComponentItem;
	friend class Repository;
	friend class Notify;
	friend class Datasource;
	friend class User;
	friend class UserGroup;
	friend class Field;
	friend class Script;
	friend class Action;
	friend class Task;
	friend int testThings(class triODBC &odbc);
};


// Database character field sizes - these are 1 byte longer than the field to
// allow for the null terminator - have to go here as impl uses them
#define DB_BOOL_LEN			2
#define DB_SCRIPTEXT_LEN	11
#define DB_PHONE_LEN		33
#define DB_VERSION_LEN		41
#define DB_NAME_LEN			65
#define DB_ATTYPE_LEN		65
#define DB_HOSTNAME_LEN		257
#define DB_SUMMARY_LEN		257
#define DB_ARRAYKEY_LEN		257
#define DB_EMAIL_LEN		513
#define DB_NOTES_LEN		2049
#define DB_PATH_LEN			2049
#define DB_QUERY_LEN		2049
#define DB_VARVAL_LEN		2049
#define DB_SHELL_LEN		2049


#include "model.impl"


#endif /*__model_h*/