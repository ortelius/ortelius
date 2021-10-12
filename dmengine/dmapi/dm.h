/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __dm_h

#define __dm_h


#include "list.h"
#include "hashtable.h"
#include "outputstream.h"
#include "scopestack.h"
#include "dmapiexport.h"
#include "modelbase.h"


#ifdef WIN32
#define DM_PLATFORM "Windows"
#elif defined(__linux__)
#define DM_PLATFORM "Linux"
#elif defined(__sparc)
#define DM_PLATFORM "Solaris"
#else
#define DM_PLATFORM "Unknown"
#endif


class DMAPI_API TempFile
{
private:
	char *m_filename;

public:
	TempFile(int n);
	~TempFile();

	const char *filename()  { return m_filename; }

	FILE *open(bool forRead = false, bool append = false);
	bool unlink();
};


class DMAPI_API TempFolder
{
private:
	char *m_pathname;

public:
	TempFolder(const char *deployId);
	~TempFolder();

	const char *pathname()  { return m_pathname; }

	bool mkdir(bool empty);
	bool unlink();
	class TempFolder *parent();
};


class DMAPI_API DM : public virtual IOutputStream
{
private:
	class Environment *m_environment;
	class Application *m_application;
	class Application *m_application2;
	class Credentials *m_dialogCredentials;
	class Model *m_model;
	const char *m_dmhome;
	List<class TempFile> m_tempFiles;
	List<class TempFolder> m_tempFolders;
	Hashtable<class Dropzone> m_dropzones;
	class ScopeStack m_stack;
	bool m_initializationDone;
	int m_eventid;
	const char *m_BaseDir;
	bool m_removeApp;
	char **m_argv;
	char **m_envp;

public:
	DM();
	~DM();

	void setModel(Model *model);
	Model *getModel();
	class ScopeStack &getScopeStack();
	class EngineConfig &getEngineConfig();
	bool setTargetEnvironment(const char *env);
	bool setTargetEnvironment(int envid);
	class Environment *getTargetEnvironment();
	class Environment *getEnvironment(const char *name);
	bool setTargetApplication(const char *app);
	bool setTargetApplication(int appid);
	class Application *getTargetApplication();
	bool setSecondApplication(int appid);
	class Application *getSecondApplication();
	void setDialogCredentials(class Credentials *creds);
	void setRemoveApp(bool removeApp);
	bool getRemoveApp();
	class Credentials *getDialogCredentials();
	bool setCurrentUser(const char *username, const char *domainlist, const char *password);
	class User *getCurrentUser();
	void updateUserLastLogin(User &user);
	void setEventId(int eventid);
	int getEventId();
	int  internalNotify(Context &ctx,class NotifyTemplate *t);
	int DumpScript(int actionid);
	int importScript();

	class Server *getPhysicalServer(const char *name);
	class Component *getComponent(const char *name,bool throwRTE = false);
	class Component *getComponentById(int compid);
	class DateTime *getDeployTime(Component *comp,Server *serv);
	class DateTime *getDeployTime(Application *app,Environment *env);
	class Application *getApplication(const char *name);
	class Repository *getRepository(const char *name);
	class Notify *getNotifier(const char *name);
	class Notify *getNotifier(int id);
	class Datasource *getDatasource(const char *name);
	class Action *getAction(const char *name);
	class Action *getActionTask(class Task &task);
	class Task *getTask(const char *name);
	class User *waitForTask(class Task &task,class Application &app,int deploymentid);
	class Task *getTask(int id);

	// TODO: These will be removed after we add a Notification object
	class User *getUser(int id);
	class UserGroup *getUserGroup(int id);
	char *getNotifyText(int id);

	// Added by PAG - can we get rid of the calls above?
	class NotifyTemplate *getSuccessTemplate(Context &ctx,int taskid);
	class NotifyTemplate *getFailureTemplate(Context &ctx,int taskid);

	class ActionNode *firstActionNode();
	class ActionNode *getActionNode(const char *name);

	int deployId();
	void addDeploymentAction(class Action *action);
	class Audit &getAudit();
	class Audit &getDummyAudit();
	void startAudit();
	void finishAudit(int exitCode);
	//int getNextDepMajor()  { return ++m_depMajor; }

	const char *dmHome()  { return m_dmhome; }
	void setDmHome(const char *dmhome)  { m_dmhome = dmhome; }
	const char *getDmHome() { return m_dmhome; }

	FILE *createTemporaryFile(char **filename);
	void deleteTemporaryFilesAndFolders();

	Dropzone *getDropzone(const char *name);
	Dropzone *newDropzone(const char *name, bool autoUnlink);
	void removeDropzone(Dropzone *dropzone);

	char *getHostname();

	bool loadPlugin(const char *plugin);
	bool installPlugin(const char *plugin);
	bool installProviderImpl(
		const char *stmt, const char *plugin,
		int kind, List<class PropertyDef> *propdefs);

	int parse(const char *filename);
	int doNotify(int nfyid, const char *from, const char *subject, int templateid);
	int providerTest(enum object_kind_tag kind, int id, char *recipient = (char *)0);

	void writeToLogFile(const char *fmt, ...);
	bool API(Context &ctx ,const char *URL,const char *params,Expr **ret,DMArray *cjp=(DMArray *)0);

	// IOutputStream
	void writeToStdOut(const char *fmt, ...);
	void writevToStdOut(long threadId, const char *buffer);
	void writeBufferToStdOut(long threadId, const char *buffer, int len);

	void writeToStdErr(const char *fmt, ...);
	void writevToStdErr(long threadId, const char *buffer);
	void writeBufferToStdErr(long threadId, const char *buffer, int len);

	IOutputStream *toOutputStream();

	void exitWithError(const char *fmt, ...);

	void initialize(const char *baseDir, char **argv, char **envp);
	void removeRequest(int taskid);
	int runLinkedAction(class Action &action);
	int runAction(class Action &action);
	int runTask(class Task &task);
	int doDeployment(class Application &app, class Context *ctx = NULL);
	void cleanup();

	const char *getDefaultFields();
	const char *getTriField(int field);

	const char *getBaseDir();
	char **getArgv();
	char **getEnvp();

	void setPluginObject(const char *name, class PluginObject *obj);
	class PluginObject *getPluginObject(const char *name);

private:
	void registerBuiltins();
	void setEnvironmentVariables(char **envp);
	void setCommandLineVariables(char **argv);

	bool createTemporaryFolder(const char *deployId, char **pathname, bool empty);
	TempFolder *createTemporaryFolder(const char *deployId, bool empty);
	TempFolder *getTemporaryFolder(const char *deployId);
	void removeTemporaryFolder(TempFolder *folder);

	int internalRunAction(class Action &action,class Context *ctx = NULL);
	//int internalDeployComponent(class Component &comp);
	void recordDeployedToEnvironment(class Application &app, bool success);
	int internalDeployApplication(class Application &app,class Context *ctx = NULL);

	friend class Dropzone;
	friend class ServerDropzone;
};

// For posting to the DeployHub API
typedef enum message_type_t {
MESSAGE_TYPE_SOAP	= 1,
MESSAGE_TYPE_POST   = 2,
MESSAGE_TYPE_GET    = 3
} MESSAGE_TYPE;

DMAPI_API bool getConnectionDetails(const char *fullurl,char **server,int *port,bool *secure,char **url);
DMAPI_API int DoHttpRequest(const char *hostname, int port, const char *uri,	// where
			  const char *params, MESSAGE_TYPE mt, bool isSecure,const char *host, 
			  const char *soapaction, DMArray *cookieJar, DMArray *header,	// content
			  int *status, char **contentType, char **content,char *logfilename=NULL,int *datalen=NULL);



#endif /*__dm_h*/
