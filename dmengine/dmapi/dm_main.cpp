#ifdef WIN32
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>
#define stricmp strcasecmp
#endif /*WIN32*/

//#ifdef WIN32
#undef HAVE_TRILOGYAPI
//#endif

#ifdef HAVE_TRILOGYAPI
#include <trilogyapi.h>
#endif /*HAVE_TRILOGYAPI*/

#include "dm.h"
#include "lexer.h"
#include "context.h"
#include "scopestack.h"
#include "exceptions.h"
#include "model.h"
//#include "db.h"
#include "triodbc.h"
#include "options.h"
#include "license.h"
#include "crypto.h"
//#include "expr.h"	// For CharPtr only
#include "charptr.h"
#include "audit.h"


#include "hashtable.h"

// For ScanServer
#include "transfer.h"

// For CheckServer
#include "ping.h"

// For listing generated DMScript
#include "dropzone.h"
#include "scriptgen.h"

#define DM_VERSION "2.0beta"
// #define SCHEMA_VERSION 2014080701L
#define SCHEMA_VERSION 2014071501L

// For parsing API output
#include "lexer.h"
#include "expr.h"

#ifdef WIN32 
#include "compat.h"
#endif

#include "zip.h"


OPTIONS options[]={
	{ "-fields",		'Y', "trifields"			},
	{ "-validate",		'Y', "trivalidate"			},	// TODO: use this
	{"-usr",			'Y',	"TRIDM_USER" 		},
	{"-pw",				'Y',	"tripassword"		},
	{"-h",				'N',	"trihelp"			},
	{"-home",			'Y',	"tridmhome"			},
	{"-sessionid",		'Y',	"trisessionid"		},	// RHT 24/07/2013 - sessionid from web interface
	{"-app",			'Y',	"triappname"		},	// RHT 16/10/2013 - application name from web interface
	{"-appid",			'Y',	"triappid"			},	// RHT 16/10/2013 - application id from web interface
	{"-app2id",			'Y',	"triapp2id"			},	// RHT 16/12/2013 - second application id from web interface
	{"-env",			'Y',	"trienvname"		},	// RHT 16/10/2013 - environment name from web interface
	{"-envid",			'Y',	"trienvid"			},	// RHT 16/10/2013 - environment id from web interface
	{"-log",			'Y',	"trilogfile"		},	// TODO: use this
	{"-showlog",		'Y',	"trideplog"			},	// RHT 22/09/2010 - dumps the given deployment log
	{"-color",			'N',	"triusecolor"       },  // RHT 25/05/2013 - display console output in colour
	{"-testmode",		'N',	"tritestmode"		},	// RHT 01/08/2012 - performs tests on the model
	{"-install",		'Y',	"triinstall"		},	// RHT 11/09/2012 - installs a plugin
	{"-parse",			'Y',	"triparse"			},	// RHT 01/10/2012 - parses the given file
	{"-encrypt",		'N',	"triencrypt"		},	// RHT 02/12/2013 - encrypts data from stdin
	{"-providertest",	'Y',	"triprovidertest"	},	// RHT 16/07/2013 - tests the given provider
	{"-notify",			'Y',	"trinotify"			},	// RHT 29/11/2013 - sends a notification
	{"-from",			'Y',	"trifrom"			},	// RHT 29/11/2013 - address notification is from
	{"-subject",		'Y',	"trisubject"		},	// RHT 29/11/2013 - subject line for notification
	{"-template",		'Y',	"tritemplate"		},	// RHT 29/11/2013 - notification template - if not given use stdin
	{"-runtask",		'Y',	"triruntask"		},	// RHT 05/12/2013 - runs a task and any linked actions
	{"-nosemantic",		'N',	"trinosemantic"		},	// RHT 09/10/2012 - disables semantic checks
	{"-noexec",			'N',	"trinoexec"			},	// TODO: dry-run mode - stops commands being run
	{"-nounlink",		'N',	"trinounlink"		},	// turn off temporary file deletion
	{"-nonotify",		'N',	"trinonotify"		},	// turn off notifications (for testing)
	{"-forceunlink",	'N',	"triforceunlink"	},	// force temporary file deletion if not empty
	{"-debug",			'Y',	"tridebuglevel"	    },	// debug level
	{"-lexdebug",		'Y',	"trilexdebug"	    },	// write lexer output to specified file
	{"-loadusers",		'Y',	"triloadusers"		},
	{"-loadservs",		'Y',	"triloadservs"		},
	{"-loadenvs",		'Y',	"triloadenvs"		},
	{"-dumpdb",			'N',	"tridumpdb"			},	// dump encrypted DSN, username and password to stdout
	{"-testdb",			'N',	"tritestdb"			},	// test DB connection
	{"-scanserver",		'Y',	"triscanserver"		},	// checks MD5 sums against previous deployments
	{"-checkserver",	'Y',	"tricheckserver"	},	// checks connectivity (protocol and credentials etc) to a server
	{"-remove",			'N',	"triremoveapp"		},	// Removes an application from an environment.
	{"-dumpscript",		'Y',	"tridumpscript"		},
	{"-importscript",	'N',	"triimpscript"		},
	{"-dmscript",		'Y',	"tridmscript"		},
	{"-taskid",			'Y',	"tritaskid"			},	// for deploy - indicates the task being used to instigate
	{"-compid",			'Y',	"tricompid"			}	// for runtask - indicates the component to be pushed onto the stack (if any)

};


extern void setdebuglevel(int level);


///////////////////////////////////////////////////////////////////////////////


char *getFieldContent(DM &dm, int fieldNumber)
{
	const char *fields = getenv("trifields");
	if(!fields) {
		fields = dm.getDefaultFields();
	}

	int n = 0;
	char *temp = strdup(fields);
	for(char *f = strtok(temp, ",:;|"); f; f = strtok(NULL, ",:;|")) {
		n++;
		if(n == fieldNumber) {
			char *ret = strdup(f);
			SAFE_FREE(temp);
			return ret;
		}
	}

	SAFE_FREE(temp);
	return NULL;
}


int getFieldNumber(DM &dm, const char *fieldName)
{
	const char *fields = getenv("trifields");
	if(!fields) {
		fields = dm.getDefaultFields();
	}

	int n = 0;
	char *temp = strdup(fields);
	for(char *f = strtok(temp, ",:;|"); f; f = strtok(NULL, ",:;|")) {
		n++;
		if(STRCASECMP(f, fieldName) == 0) {
			SAFE_FREE(temp);
			return n;
		}
	}

	SAFE_FREE(temp);
	return 0;
}


const char *getEnvironmentNameFromDialog(DM &dm)
{
	int field = getFieldNumber(dm, "environments");
	if(field) {
		return dm.getTriField(field);
	}
	return NULL;
}


const char *getUserNameFromDialog(DM &dm)
{
	int field = getFieldNumber(dm, "username");
	if(field) {
		return dm.getTriField(field);
	}
	return NULL;
}


const char *getPasswordFromDialog(DM &dm)
{
	int field = getFieldNumber(dm, "password");
	if(field) {
		return dm.getTriField(field);
	}
	return NULL;
}


const char *getApplicationNameFromDialog(DM &dm)
{
	int field = getFieldNumber(dm, "applications");
	if(field) {
		return dm.getTriField(field);
	}
	return NULL;
}


/**
 * Output the list of environments.  If an application is selected, then only
 * output the environments which are applicable to that application.
 */
int outputEnvironmentList(DM &dm, Model& model)
{
	const char *szChangedField = getenv("TRICHANGEDFIELD");
	const char *szApplicationName = NULL;
	if(szChangedField && szChangedField[0]) {
		// We've been triggered due to a field change
		int field = atoi(szChangedField);
		if(field == getFieldNumber(dm, "applications")) {
			// Application has changed
			szApplicationName = getApplicationNameFromDialog(dm);
		}
	}

	List<Environment> *envs = model.getEnvironments();
	if(szApplicationName) {
		dm.writeToLogFile("Listing valid logical servers for application [%s]", szApplicationName);
	}
	if(envs) {
		ListIterator<Environment> eiter(*envs);
		for(Environment *e = eiter.first(); e; e = eiter.next()) {
			if(e->hasAccess(dm.getCurrentUser())) {
				if(szApplicationName) {
					// There's an application and we're listing valid servers for this application
					List<Application> *apps = e->getAllowedApplications();
					if(apps) {
						ListIterator<Application> aiter(*apps);
						for(Application *a = aiter.first(); a; a = aiter.next()) {
							if(strcmp(a->name(), szApplicationName) == 0) {
								// The application is allowed on this server
								printf("%s\n",e->name());
								break;
							}
						}
					}
				} else {
					// no application - just print the environment name
					printf("%s\n", e->name());
				}
			}
		}
	}
	
	return 0;
}


/**
 * Output the list of applications.  If an environment is selected, then only
 * output the applications which are applicable to that environment.
 */
int outputApplicationList(DM& dm, Model& model)
{
	Environment *env = NULL;

	char *szChangedField = getenv("TRICHANGEDFIELD");
	if(szChangedField && szChangedField[0]) {
		// We've been triggered due to a field change
		int field = atoi(szChangedField);
		if(field == getFieldNumber(dm, "environments")) {
			// Server has changed
			const char *szEnvName = getEnvironmentNameFromDialog(dm);
			if(szEnvName) {
				dm.writeToLogFile("Environment Name = [%s]", szEnvName);
				env = model.getEnvironment(szEnvName);
				if(!env) {
					dm.exitWithError("Environment \"%s\" is invalid or you have no access", szEnvName);
				}
			} else {
				dm.writeToLogFile("Environment name not set");
			}
		}
	}

	List<Application> *apps = env ? env->getAllowedApplications() : model.getApplications();
	int appCount = 0;
	if(apps) {
		ListIterator<Application> iter(*apps);
		for(Application *a = iter.first(); a; a = iter.next()) {
			if(a->hasAccess(dm.getCurrentUser())) {
				appCount++;
				printf("%s\n", a->name());
			}
		}
	}

	if(appCount == 0) {
		dm.writeToLogFile("No Applications listed - exiting with failure (disable field)");
		//exit(1);
		return 1;
	}
	
	return 0;
}


/**
 * if we remembered the username for this box output it here.  If the server
 * has an encrypted username/password associated, exit with 1.
 */
int handleUserName(DM &dm, Model &model)
{
	// Get the environment name (if any)
	const char *szEnvName = getEnvironmentNameFromDialog(dm);
	if(szEnvName) {
		Environment *env = model.getEnvironment(szEnvName);
		if(env) {
			// Valid server
			Credentials *creds = env->getCredentials();
			if(creds) {
				// There is password data associated with this environment
				printf("********\n");

				// Exit with failure to disable the entry box
				//exit(1);
				return 1;
			} else {
				// No password data for the environment - what about the servers?
				bool allPhysicalServersHavePassword = true;
				List<Server> *servers = env->getServers();
				if(servers) {
					ListIterator<Server> iter(*servers);
					for(Server *s = iter.first(); s; s = iter.next()) {
						Credentials *creds = s->getCredentials();
						if(!creds) {
							// No password data for this server
							allPhysicalServersHavePassword = false;
						}
					}
				}
				if(allPhysicalServersHavePassword) {
					// They may differ but all servers have password data
					// Just exit with failure to disable the field.
					//exit(1);
					return 1;
				}
			}
		}
	}
	
	return 0;
}


int handlePassword(DM& dm, Model& model)
{
	// Get the environment name (if any)
	const char *szEnvName = getEnvironmentNameFromDialog(dm);
	if(szEnvName) {
		Environment *env = model.getEnvironment(szEnvName);
		if(env) {
			// Valid server
			Credentials *creds = env->getCredentials();
			if(creds) {
				// There is password data
				if(creds->kind() == CREDENTIALS_IN_DATABASE) {
					// Do we have password or just username?
					if(creds->hasPassword()) {
						printf("********\n");
						// Disable the entry box
						//exit(1);
						return 1;
					}
				} else {
					// Must be encrypted username/password
					// Disable the trilogy entry box
					printf("********\n");
					//exit(1);
					return 1;
				}
			} else {
				// No password data for the environment - what about the servers?
				bool allPhysicalServersHavePassword = true;
				List<Server> *servers = env->getServers();
				if(servers) {
					ListIterator<Server> iter(*servers);
					for(Server *s = iter.first(); s; s = iter.next()) {
						Credentials *creds = s->getCredentials();
						if(creds) {
							if(creds->kind() == CREDENTIALS_IN_DATABASE) {
								// Do we have password or just username?
								if(!creds->hasPassword()) {
									allPhysicalServersHavePassword = false;
								}
							}
						} else {
							// No password data for this server
							allPhysicalServersHavePassword = false;
						}
					}
				}
				if(allPhysicalServersHavePassword) {
					// They may differ but all servers have password data
					// Just exit with failure to disable the field.
					//exit(1);
					return 1;
				}
			}
		}
	}
	
	return 0;
}


int outputDropDownContent(DM &dm, Model& model, const char *type)
{
	int ret = 0;
	
	if(STRCASECMP(type, "environments") == 0) {
		ret = outputEnvironmentList(dm, model);
	} else if(STRCASECMP(type, "applications") == 0) {
		ret = outputApplicationList(dm, model);
	} else if(STRCASECMP(type, "username") == 0) {
		ret = handleUserName(dm, model);
	} else if(STRCASECMP(type, "password") == 0) {
		ret = handlePassword(dm, model);
	} else {
		Field *field = model.getField(type);
		if(field) {
			field->populate(dm);
			// TODO: how to return exit code from this?
		} else {
			dm.writeToStdErr("Unknown content type \"%s\"", type);
		}
	}
	
	return ret;
}


int handleFieldPopulation(DM &dm, Model& model)
{
	const char *szFieldNumber = getenv("TRICURRENTFIELD");
	if(!szFieldNumber) {
		dm.writeToStdErr("TRICURRENTFIELD not set!");
		return 2;
	}

	int fieldNumber = atoi(szFieldNumber);

	ConstCharPtr fieldToOutput = getFieldContent(dm, fieldNumber);
	if(fieldToOutput) {
		return outputDropDownContent(dm, model, fieldToOutput);
	}

	dm.writeToStdErr("Unknown field number \"%s\" in TRICURRENTFIELD", szFieldNumber);
	return 2;
}


void getOwnersForServers(List<Server> *servers)
{
	if(servers) {
		ListIterator<Server> iter(*servers);
		for(Server *s = iter.first(); s; s = iter.next()) {
			s->getOwner();
		}
	}
}


void getCredentialsForServers(List<Server> *servers)
{
	if(servers) {
		ListIterator<Server> iter(*servers);
		for(Server *s = iter.first(); s; s = iter.next()) {
			s->getCredentials();
		}
	}
}


int testThings(triODBC &odbc)
{
	
	AutoPtr<triSQL> sql = odbc.GetSQL();
	char schemaver[1024];
	int res;
	sql->BindColumn(1, SQL_CHAR, schemaver, sizeof(schemaver));
	res = sql->ExecuteSQL("SELECT schemaver FROM dm.dm_tableinfo");
	if (res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO) {
		fprintf(stderr,"Failed to execute SQL script to determine schema version\n");
		return 1;
	}
	res = sql->FetchRow();
	if (res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO) {
		fprintf(stderr,"Failed to fetch schema version from database\n");
		return 1;
	}
	sql->CloseSQL();
	printf("Schema Version from Database: %s\n",schemaver);
	return 0;

	/*
	const char *password = "password";
	char *testDigest = digestValue(password, strlen(password), true);
	printf("digested: '%s'\n", (testDigest ? testDigest : "(null)"));
	SAFE_FREE(testDigest);
	return 0;
	*/
}


int validateDialog(DM &dm, Model& model)
{
	dm.writeToLogFile("Validating Dialog Content...");

	// Ensure that contents of the Trilogy Dialog are valid before we proceed

	const char *environment  = getEnvironmentNameFromDialog(dm);
	const char *application  = getApplicationNameFromDialog(dm);
	const char *username     = getUserNameFromDialog(dm);
	const char *password     = getPasswordFromDialog(dm);

	// (1) Do we have a target server?
	if(!environment || !environment[0]) {
		dm.exitWithError("You must specify a Target Environment");
	}

	// (2) Do we have an application?
	if(!application || !application[0]) {
		dm.exitWithError("You must specify an Application");
	}

	// (3) Does the server have a username/password specified?
	Environment *env = model.getEnvironment(environment);
	if(!env) {
		dm.exitWithError("Invalid Environment '%s' specified", NULL_CHECK(environment));
	}

	bool needUserName = true;
	bool needPassword = true;

	Credentials *creds = env->getCredentials();
	if(creds) {
		// yes it does - what type of password data?
		switch(creds->kind()) {
		case CREDENTIALS_ENCRYPTED:
		case CREDENTIALS_IN_DATABASE:
			// UserName/Password
			needUserName = false;
			if(creds->hasPassword()) {
				needPassword = false;
			}
			break;
		case CREDENTIALS_RTI3_DFO_IN_FILESYSTEM:
		case CREDENTIALS_HARVEST_DFO_IN_FILESYSTEM:
			// DFO encrypted username/password
			needUserName = needPassword = false;
			break;
		}
	} else {
		needUserName = needPassword = false;

		List<Server> *servers = env->getServers();
		if(servers) {
			ListIterator<Server> iter(*servers);
			for(Server *s = iter.first(); s; s = iter.next()) {
				Credentials *creds = s->getCredentials();
				if(creds) {
					if(!creds->hasPassword()) {
						needPassword = true;
					}
				} else {
					dm.writeToLogFile("No password data for server %s, need=true", s->name());
					needUserName = needPassword = true;
					break;
				}
			}
		}
	}

	if((needUserName && !(username && username[0]))
		|| (needPassword && !(password && password[0]))) {
		dm.exitWithError("You must specify a username/password for this deployment");
	}

	// TODO: Does the application need a package specifying?

	// TODO: custom verify action

	return 0;
}


void ensurePassphraseFile(DM &dm, const char *homeDir)
{
	char ppfilename[1024];
	sprintf(ppfilename, "%s%sdm.asc", homeDir, DIR_SEP_STR);
	debug3("ppfilename = '%s'", ppfilename);

	try {
		// Does the passphrase file exist?
		struct stat sb;
		if(stat(ppfilename, &sb) == -1) {
			if(errno == ENOENT) {
				dm.exitWithError("FATAL ERROR: The file dm.asc does not exist.  Please run the dmsetup\n"
					             "             tool to create it.");
			} else {
				dm.exitWithError("FATAL ERROR: Unable to stat passphrase file");
			}
		}

		// Read the passphrase file to allow us to do encryption
		if(!readPassphraseFile(ppfilename)) {
			dm.writeToStdErr("FATAL ERROR: Unable to initialise encryption");
		}
	} catch(RuntimeError &e) {
		e.print(dm);
		dm.exitWithError("FATAL ERROR: Passphrase file mechanism failed");
	}
}


int runScript(DM &dm, const char *baseDir, char **argv, char **envp)
{
	char *reason = getenv("TRIREASON");
	bool onTimer = reason?(strcmp(reason,"TIMED")==0):false;
	dm.initialize(baseDir, argv, envp);
	if (!dm.getTargetApplication()) {
		dm.exitWithError("No application selected");
	}
	int res = dm.doDeployment(*(dm.getTargetApplication()));
	// PAG MOD 11/04/2017 - do not call cleanup if on timer (since there may be another deployment coming)
	if (!onTimer) dm.cleanup();	// PAG MOD 30/05/2015 - move cleanup to here. 
	return res;
}


int runListboxScript(DM &dm, const char *baseDir, char **argv, char **envp)
{
	dm.initialize(baseDir, argv, envp);

	Action *action = dm.getAction("populate_listbox");
	if(!action) {
		dm.exitWithError("No action named '%s'", "populate_listbox");
	}

	int res = dm.runAction(*action);
	dm.cleanup();
	return res;
}


int runTask(DM &dm, int taskid, const char *baseDir, char **argv, char **envp)
{
	dm.initialize(baseDir, argv, envp);

	Task *task = dm.getTask(taskid);
	if(!task) {
		dm.exitWithError("Invalid task id %d", taskid);
	}

	int res = dm.runTask(*task);
	dm.cleanup();
	return res;
}


typedef enum tagTriReason {
	REASON_NONE = 0,
	REASON_UNSET,
	REASON_POPULATE,
	REASON_LISTBOX,
	REASON_VALIDATION,
	REASON_SCRIPT,
	REASON_TIMED
#ifdef HAVE_TRILOGYAPI
	, REASON_PRELOAD
#endif /*HAVE_TRILOGYAPI*/
} TriReason;


TriReason triReasonFromString(const char *triReason)
{
	if(strcmp(triReason, "POPULATE") == 0) {
		return REASON_POPULATE;
	} else if(strcmp(triReason, "LISTBOX") == 0) {
		return REASON_LISTBOX;
	} else if(strcmp(triReason, "VALIDATION") == 0) {
		return REASON_VALIDATION;
	} else if(strcmp(triReason, "SCRIPT") == 0) {
		return REASON_SCRIPT;
	} else if(strcmp(triReason, "TIMED") == 0) {
		return REASON_TIMED;
	}
#ifdef HAVE_TRILOGYAPI
	 else if(strcmp(triReason, "PRELOAD") == 0) {
		return REASON_PRELOAD;
	}
#endif /*HAVE_TRILOGYAPI*/
	return REASON_NONE;
}

static char to_hex(char code)
{
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

static char *url_encode(const char *str)
{
	char *pstr = (char *)str;
	char *buf = (char *)malloc(strlen(str) * 3 + 1);
	char *pbuf = buf;
	while (*pstr) {
		if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
			*pbuf++ = *pstr;
		else if (*pstr == '/')
			*pbuf++ = '/';
		else {
			*pbuf++ = '%';
			*pbuf++ = to_hex(*pstr >> 4);
			*pbuf++ = to_hex(*pstr & 15);
		}
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}


/**
 * First check if -usr was specified, if not then use Trilogy's TRICLIENTUSERNAME
 */
void setCurrentUser(DM &dm)
{
	const char *username = getenv("TRIDM_USER");
	if(!username) {
		username = getenv("TRICLIENTUSERNAME");
	}
	if(!username) {
		dm.exitWithError("Username must be specified");
	}

	const char *password = getenv("tripassword");
	// PRO Version - password may be passed in TRIFIELD1 if being invoked from the Web UI
	if (!password) password = getenv("TRIFIELD1");
	if(password) {
		// Get WebUI to authenticate
		// 
		char *eu = url_encode(username);
		char *ep = url_encode(password);
		char *fmt = "/dmadminweb/API/login?user=%s&pass=%s";
		char *p = (char *)malloc(strlen(fmt)+strlen(eu)+strlen(ep)+10);
		sprintf(p,fmt,eu,ep);
	
		Expr *content;
		List<Server> servers = new List<Server>();
		ScopeStack *stack = new ScopeStack(dm);
		Context *ctx = new Context(dm,servers,*stack);
		if (dm.API(*ctx,p,NULL,&content)==false) {
			dm.exitWithError("Logon denied: invalid username or password");
		}
		/*
		char *passhash = digestValue(password, strlen(password), true);
		if(!dm.getCurrentUser()->validateHashedPassword(passhash)) {
			dm.exitWithError("Logon denied: invalid username or password");
		}
		*/
		dm.writeToLogFile("User '%s' authenticated", username);

		char *fmt2 = "/dmadminweb/API/domains?user=%s&pass=%s";
        char *p2 = (char *)malloc(strlen(fmt)+strlen(eu)+strlen(ep)+10);
		sprintf(p2,fmt2,eu,ep);

		if (dm.API(*ctx,p2,NULL,&content)) 
		{
			Expr *e = content;
			if (e) {
				// *ret = e->toArray();
				Variable *V = e->toArray()->get("result");
				if (V) 
				{
					const char *domainlist = V->getString();
					if(!dm.setCurrentUser(username, domainlist, password))
					{
					 dm.exitWithError("Logon denied: invalid username or password");
					}
					dm.updateUserLastLogin(*(dm.getCurrentUser()));
				}
			}
		}
		free(eu);
		free(ep);
		free(p);
	} else {
		if (dm.getEventId()==0) {
			// Password mandatory if we're not running a timed event
			dm.exitWithError("Password must be specified");
		}
	}
}


void setTargetAppAndEnvFromParams(DM &dm, bool required)
{
	const char *envname = getenv("trienvname");
	const char *envidstr = getenv("trienvid");
	const char *appname = getenv("triappname");
	const char *appidstr = getenv("triappid");
	const char *szRemoveApp = getenv("triremoveapp");
	

	if(required) {
		if(!((envname != NULL) ^ (envidstr != NULL))) {
			dm.exitWithError("-env and -envid are mutually exclusive but must be specified in this mode");
		}
		if(!((appname != NULL) ^ (appidstr != NULL))) {
			dm.exitWithError("-app and -appid are mutually exclusive but must be specified in this mode");
		}
	} else {
		if(envname && envidstr) {
			dm.exitWithError("-env and -envid are mutually exclusive");
		}
		if(appname && appidstr) {
			dm.exitWithError("-app and -appid are mutually exclusive");
		}
	}

	if(envname) {
		if(!dm.setTargetEnvironment(envname)) {
			dm.exitWithError("Invalid environment '%s' specified", envname);
		}
	} else if(envidstr) {
		int envid = atoi(envidstr);
		if(!envid || !dm.setTargetEnvironment(envid)) {
			dm.exitWithError("Invalid environment id '%s' specified", envidstr);
		}
	} else if(required) {
		dm.exitWithError("No environment specified");
	}

	bool removeApp = (szRemoveApp && (szRemoveApp[0]=='Y' || szRemoveApp[0]=='y'));
	dm.setRemoveApp(removeApp);

	if(appname) {
		if(!dm.setTargetApplication(appname)) {
			dm.exitWithError("Invalid application '%s' specified", appname);
		}
	} else if(appidstr) {
		int appid = atoi(appidstr);
		if(!appid || !dm.setTargetApplication(appid)) {
			dm.exitWithError("Invalid application id '%s' specified", appidstr);
		}
	} else if(required) {
		dm.exitWithError("No application specified");
	}

	const char *app2idstr = getenv("triapp2id");
	if(app2idstr) {
		int app2id = (app2idstr[0] == '+') ? -atoi(&app2idstr[1]) : atoi(app2idstr);
		debug1("app2id is %d", app2id);
		if(!app2id || !dm.setSecondApplication(app2id)) {
			dm.exitWithError("Invalid application id '%s' specified", app2idstr);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////


#ifdef WIN32
char *getHomeDirectory(const char *argv0)
{
	char moduleFileName[1024];
	GetModuleFileNameA(NULL, moduleFileName, sizeof(moduleFileName));

	// Split off the name of the executable
	char *lastsep = strrchr(moduleFileName, '\\');
	if(!lastsep) {
		// This should never happen!
		debug1("Module filename is '%s'", moduleFileName);
		return strdup(".");
	}
	*lastsep = '\0';
	// If we still have a slash, then do dirname
	if((lastsep = strrchr(moduleFileName, '\\')) != NULL) {
		*lastsep = '\0';
	} else {
		// otherwise must be a relative path, so append "\.."
		strcat(moduleFileName, "\\..");
	}
	return strdup(moduleFileName);
}
#else
char *getHomeDirectory(const char *argv0)
{
	if (getenv("DMHOME")) return strdup(getenv("DMHOME"));
	char *basedir = strdup(argv0);
	// Split off the name of the executable
	char *lastsep = strrchr(basedir, '/');
	if(!lastsep) {
		// This should never happen!
		debug1("Basedir is '%s'", basedir);
		return strdup(".");
	}
	*lastsep = '\0';
	// If we still have a slash, then do dirname
	if((lastsep = strrchr(basedir, '/')) != NULL) {
		*lastsep = '\0';
	} else {
		// otherwise must be a relative path
		free(basedir);
		chdir("..");
		// create an absolute path
		int n = 1024;
		char *cwd = getcwd(0,0);
		basedir=cwd;
	}
	return basedir;
}
#endif


triODBC *connectToDatabase(DM &dm, const char *homeDir)
{
	ensurePassphraseFile(dm, homeDir);	// dm.asc HAS to exist (for password encryption)
	triODBC *odbc = (triODBC *)0;
	int res=0;
	bool usingWebService=false;

	char odbcfilename[1024];
	sprintf(odbcfilename, "%s%sdm.odbc", homeDir, DIR_SEP_STR);
	debug3("odbcfilename = '%s'", odbcfilename);

	// Does the odbc file exist?
	struct stat sb;
	debug1("Checking for existence of %s\n",odbcfilename);
	if(stat(odbcfilename, &sb) == -1) {
		if(errno == ENOENT) {
			debug1("not found - connect to web service");
			// dm.odbc file does not exist - assume we're going to run SQL queries over Web Service
			char *clientid = getenv("CLIENTID");
			if (!clientid) dm.exitWithError("No dm.odbc file found and CLIENTID not set");
			//
			// Create an empty context for SQL queries.
			//
			List<Server> serverSet;
			ScopeStack stack(dm);
			stack.setGlobal("DMHOME", dm.getBaseDir());
			stack.setGlobal("TRIDM_PLATFORM", DM_PLATFORM);
			Context ctx(dm,serverSet,stack);

			odbc = new triODBC(&ctx,clientid);
			res = odbc->ConnectToDataSource(&dm);
			usingWebService=true;
		} else {
			dm.exitWithError("FATAL ERROR: Unable to stat odbc file");
		}
	} else {
		debug1("found - connecting locally over ODBC");
		// Decrypt dm.odbc
		int len = 0;
		CharPtr details = decryptFile(odbcfilename, &len);
		const char *dsn = details;
		const char *user = NULL;
		const char *pass = NULL;

		int nulls = 0;
		for(int n = 0; n < len; n++) {
			if(!details[n]) {
				nulls++;
				if(nulls == 1) {
					user = &details[n+1];
				} else if(nulls == 2) {
					pass = &details[n+1];
				}
			}
		}
		if(nulls != 2) {
			dm.exitWithError("FATAL ERROR: dm.odbc is not a valid odbc file");
		}

		char *DBUserName = getenv("DBUserName");
	    char *DBPassword = getenv("DBPassword");

		if (DBUserName != NULL)
		{
		 details = malloc(5);
		 memset(details,'\0',5);
		 strcpy(details,"dm");	

		 user = DBUserName;
		 pass = DBPassword;
		 dsn = details;
		}

		if (getenv("tritestdb")) {
			printf("Testing connecting to database\n");
			printf("DSN:       [%s]\n",(const char *)details);
			printf("Username:  [%s]\n",user);
			printf("Password:  [*****]\n");
		}

		odbc = new triODBC();
		res = odbc->ConnectToDataSource(&dm, (char*) dsn, (char*) user, (char*) pass);

		if (getenv("tritestdb")) {
			if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
				printf("Database connection successful\n");
			} else {
				char *MsgPtr;
				SQLINTEGER errnum;
				odbc->GetLastError(&MsgPtr,&errnum);
				dm.writeToStdErr("Failed to Connect to Database: %s",MsgPtr);
				free(MsgPtr);
			}
			odbc->DisconnectFromDataSource();
			exit(0);
		}
	}
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		char *MsgPtr;
		SQLINTEGER errnum;
		odbc->GetLastError(&MsgPtr,&errnum);
		dm.writeToStdErr("Failed to Connect to Database: %s",MsgPtr);
		free(MsgPtr);
		return NULL;
	}

	if (getenv("tritestmode") && !usingWebService) {
		printf("\nConnected as User \"%s\" via datasource \"%s\" to \"%s\"\nUsing driver %s version %s\n\n",
			odbc->GetUserName(),
			odbc->GetDSN(),
			odbc->GetDatabaseVendor(),
			odbc->GetDriverName(),
			odbc->GetDriverVer());
	}

	AutoPtr<triSQL> sql = odbc->GetSQL();
	long schemaver;
	sql->BindColumn(1, SQL_C_SLONG, &schemaver, sizeof(schemaver));
	res = sql->ExecuteSQL("select schemaver from dm_tableinfo");
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		return NULL;
	}
	res = sql->FetchRow();
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		return NULL;
	}
	sql->CloseSQL();
	return odbc;
}


int showDeploymentLog(triODBC &odbc, int logtoshow)
{
	AutoPtr<triSQL> sql = odbc.GetSQL();

	int stream;
	char line[2049];
	SQLLEN ni_line = 0;
	sql->BindColumn(1, SQL_INTEGER, &stream, sizeof(stream));
	sql->BindColumn(2, SQL_CHAR, line, sizeof(line), &ni_line);

	SQLRETURN res = sql->ExecuteSQL(
		"SELECT stream, line FROM dm_deploymentlog "
		"WHERE deploymentid=%d ORDER BY runtime", logtoshow);
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		fprintf(stderr, "Deployment log %d not found", logtoshow);
		return 1;
	}

	bool useColor = getenv("triusecolor") ? true : false;

	for(res = sql->FetchRow(); (res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO); res = sql->FetchRow()) {
		char *output = NULL_IND(line, NULL);
		if(output) {
			if(stream == 2) {
				fprintf(stderr, (useColor ? "\x1b[31m%s\x1b[0m\n" : "%s\n"), output);
			} else {
				fprintf(stdout, "%s\n", output);
			}
		}
	}
	sql->CloseSQL();
	return 0;
}


void printHelpAndExit()
{
	printf("OpenMake Deployment Manager " DM_VERSION "\n"
		"(c) 2013 OpenMake Software.  All rights reserved.\n"
		"\nUsage: dm <options>\n\n"
		"-fields <fieldlist> - Field mapping for dialog\n"
		//"-validate <validate>\n"
		"-usr <username>     - DM user\n"
		"-pw <password>      - DM password\n"
		"-h                  - This help text\n"
		//"-log <logfile>      - Set log filename\n"
#ifdef DEV_VERSION
		"-home <dir>         - Forcibly set DMHOME\n"
		"-testmode           - #Performs tests on the model\n"
#endif /*DEV_VERSION*/
		"-install            - Installs plugins\n"
		"-parse <file>       - Parses the given file\n"
#ifdef DEV_VERSION
		"-nosemantic         - #Disables semantic checks\n"
#endif /*DEV_VERSION*/
		//"-noexec             - Dry-run mode - stops commands being run\n"
		"-nounlink           - Turn off temporary file deletion\n"
		"-nonotify           - Turn off notifications (for testing)\n"
		"-forceunlink        - Force temporary file deletion if not empty\n"
		"-showlog <deployid> - Show the given deployment log\n"
#ifdef DEV_VERSION
		"-debug <level>      - #Debug level (0-3)\n"
		"-lexdebug <file>    - #Write lexer output to specified file\n"
		"\n"
		"# - development option\n"
#endif /*DEV_VERSION*/
		);
	exit(0);
}


// TODO: These will be removed when turned into DLLs
extern "C" int filesystem_PluginInstall(DM &dm);
extern "C" int svn_PluginInstall(DM &dm);
extern "C" int rti_PluginInstall(DM &dm);
extern "C" int smtpemail_PluginInstall(DM &dm);
extern "C" int txtlocal_PluginInstall(DM &dm);
#ifdef HAVE_ODBC
extern "C" int odbcdatasource_PluginInstall(DM &dm);
#endif /*HAVE_ODBC*/
extern "C" int xmlmodify_PluginInstall(DM &dm);
extern "C" int textmodify_PluginInstall(DM &dm);
extern "C" int ftp_PluginInstall(DM &dm);


/************** CODE FOR BULK LOADING *********************/

char *readLine(FILE *in)
{
	static char buf[1024];
	buf[0]='\0';
	char *res = fgets(buf,sizeof(buf),in);
	if (res) {
		char *hash = strchr(buf,'#');
		if (hash) *hash = '\0';
		// Skip any leading spaces
		char *sp = buf;
		while (*sp==' ' || *sp=='\t') sp++;
		// Get rid of any trailing spaces or newline chars
		char *ep = &(buf[strlen(buf)-1]);
		while (*ep==' ' || *ep=='\t' || *ep=='\n' || *ep=='\r') *ep--='\0';
		return sp;
	}
	return (char *)0;
}



int getCredentialID(Model &model,char *credname)
{ 
	static StringHashtable *cht = (StringHashtable *)0;
	if (!cht) cht = new StringHashtable();
	const char *v = cht->get(credname);
	if (!v) {
		// Not yet found this credential
		int credid = model.GetIDFromQualifiedName(credname,"dm_credentials");
		char szCredID[128];
		sprintf(szCredID,"%d",credid);
		cht->put(credname,szCredID);
		return credid;
	}
	return atoi(v);
}

int getUserID(Model &model,char *username)
{
	static StringHashtable *uht = new StringHashtable();
	if (!uht) uht = new StringHashtable();
	const char *v = uht->get(username);
	if (!v) {
		// Not yet found this user
		int userid = model.GetIDFromQualifiedName(username,"dm_user");
		char szUserID[128];
		sprintf(szUserID,"%d",userid);
		uht->put(username,szUserID);
		return userid;
	}
	return atoi(v);
}

int getUserGroupID(Model &model,char *groupname)
{
	static StringHashtable *ght = new StringHashtable();
	if (!ght) ght = new StringHashtable();
	const char *v = ght->get(groupname);
	if (!v) {
		// Not yet found this group
		int groupid = model.GetIDFromQualifiedName(groupname,"dm_usergroup");
		char szGroupID[128];
		sprintf(szGroupID,"%d",groupid);
		ght->put(groupname,szGroupID);
		return groupid;
	}
	return atoi(v);
}

int getEnvironmentID(Model &model,char *envname)
{
	static StringHashtable *eht = new StringHashtable();
	if (!eht) eht = new StringHashtable();
	const char *v = eht->get(envname);
	if (!v) {
		// Not yet found this environment
		int envid = model.GetIDFromQualifiedName(envname,"dm_environment");
		char szEnvID[128];
		sprintf(szEnvID,"%d",envid);
		eht->put(envname,szEnvID);
		return envid;
	}
	return atoi(v);
}

int getServerID(Model &model,char *servername)
{
	static StringHashtable *sht = new StringHashtable();
	if (!sht) sht = new StringHashtable();
	const char *v = sht->get(servername);
	if (!v) {
		// Not yet found this environment
		int servid = model.GetIDFromQualifiedName(servername,"dm_server");
		char szServID[128];
		sprintf(szServID,"%d",servid);
		sht->put(servername,szServID);
		return servid;
	}
	return atoi(v);
}

int getTypeID(triODBC &odbc,char *name)
{
	
	static StringHashtable *tht = (StringHashtable *)0;
	if (!tht) tht = new StringHashtable();
	const char *v = tht->get(name);
	if (!v) {
		AutoPtr<triSQL> sql = odbc.GetSQL();
		int id;
		sql->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
		sql->BindParameter(1,SQL_CHAR,strlen(name),name,strlen(name));
		sql->ExecuteSQL("SELECT id FROM dm_servertype WHERE name=?");
		int res = sql->FetchRow();
		if (res==SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
			char szTypeID[128];
			sprintf(szTypeID,"%d",id);
			tht->put(name,szTypeID);
			sql->CloseSQL();
			return id;
		} else {
			sql->CloseSQL();
			return 0;
		}
	}
	return atoi(v);

}

int getProtocolID(triODBC &odbc,char *name)
{
	static StringHashtable *pht = (StringHashtable *)0;
	if (!pht) pht = new StringHashtable();
	const char *v = pht->get(name);
	if (!v) {
		AutoPtr<triSQL> sql = odbc.GetSQL();
		int id;
		sql->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
		sql->BindParameter(1,SQL_CHAR,strlen(name),name,strlen(name));
		sql->ExecuteSQL("SELECT id FROM dm_providerdef WHERE kindstr='transport' AND name=?");
		int res = sql->FetchRow();
		if (res==SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
			char szTypeID[128];
			sprintf(szTypeID,"%d",id);
			pht->put(name,szTypeID);
			sql->CloseSQL();
			return id;
		} else {
			sql->CloseSQL();
			return 0;
		}
	}
	return atoi(v);

}

void loadEnvironmentsAndExit(Model &model,triODBC &odbc,char *filename)
{
	int lineno=0;
	FILE *in = fopen(filename,"r");
	if (in) {
		while (char *sp=readLine(in)) {
			lineno++;
			if (strlen(sp)==0) continue;
			// Syntax is:
			// environmentname,domain,[U|G]ownerid (optional), servers (optional)
			//
			char *nm = strtok(sp,",|");
			char *dn = strtok(NULL,",|");
			char *ow = strtok(NULL,",|");
			int owid=0;
			char collist[1024];
			char sellist[128];


			int domid=model.getDomainID(dn);
			switch (domid) {
				case DOMAIN_NOT_FOUND:
				case DOMAIN_SYNTAX_ERROR:
					printf("Error at line %d: Invalid Domain \"%s\" specified\n",lineno,dn);
					continue;
				case DOMAIN_OBJECT_AMBIGUOUS:
					printf("Error at line %d: Ambiguous Domain \"%s\" specified. Please use Fully Qualified Domain\n",lineno,dn);
					continue;
			}
			//                ?   ?     ?         ?        ?         ?       ?        ?
			sprintf(collist,"id,name,domainid,creatorid,created,modifierid,modified,status");
			sprintf(sellist,"?,?,?,?,?,?,?,'N'");
			if (ow) {
				switch(*ow) {
					case 'U':
						strcat(collist,",ownerid");
						strcat(sellist,",?");
						owid=getUserID(model,&(ow[1]));
						break;
					case 'G':
						strcat(collist,",ogrpid");
						strcat(sellist,",?");
						owid=getUserGroupID(model,&(ow[1]));
						break;
					default:
						printf("ERROR: Owner needs to start with U or G (line %d) - line ignored\n",lineno);
						continue;
				}
				switch (owid) {
				case DOMAIN_NOT_FOUND:
					printf("Error at line %d: Invalid %s \"%s\" specified\n",lineno,*ow=='U'?"User":"Usergroup",ow+1);
					continue;
				case DOMAIN_SYNTAX_ERROR:
					printf("Error at line %d: Invalid %s \"%s\" specified\n",lineno,*ow=='U'?"User":"Usergroup",ow+1);
					continue;
				case DOMAIN_OBJECT_AMBIGUOUS:
					printf("Error at line %d: Ambiguous %s \"%s\" specified. Please Qualify with Domain\n",lineno,*ow=='U'?"User":"Usergroup",ow+1);
					continue;
				}
			}
			AutoPtr<triSQL> sql = odbc.GetSQL();
			sql->SetAutoCommitMode(false);
			int me=1;	// temp (need to be able to login)
			time_t tn;
			time(&tn);
			int c,id;
			sql->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
			sql->ExecuteSQL("SELECT count(*) FROM dm.dm_environment WHERE name='%s' AND domainid = %d",nm,domid);
			sql->FetchRow();
			sql->CloseSQL();
			if (id>0) {
				printf("ERROR: Could not create environment \"%s\" - an environment with that name already exists in domain \"%s\"\n",nm,dn);
				continue;
			}
			sql->BindColumn(1,SQL_INTEGER,&c,sizeof(c));
			sql->ExecuteSQL("SELECT count(*) FROM dm.dm_environment");
			sql->FetchRow();
			sql->CloseSQL();
			if (c==0) {
				// No environments as yet
				id=1;
			} else {
				sql->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
				sql->ExecuteSQL("SELECT max(id)+1 FROM dm.dm_environment");
				sql->FetchRow();
				sql->CloseSQL();
			}
			sql->BindParameter(1,SQL_INTEGER,sizeof(id),&id,sizeof(id));				// id
			sql->BindParameter(2,SQL_CHAR,strlen(nm),nm,strlen(nm));					// environment name
			sql->BindParameter(3,SQL_INTEGER,sizeof(domid),&domid,sizeof(domid));		// domain id
			sql->BindParameter(4,SQL_INTEGER,sizeof(me),&me,sizeof(me));				// creator id
			sql->BindParameter(5,SQL_INTEGER,sizeof(tn),&tn,sizeof(tn));				// time now (created)
			sql->BindParameter(6,SQL_INTEGER,sizeof(me),&me,sizeof(me));				// modifier id
			sql->BindParameter(7,SQL_INTEGER,sizeof(tn),&tn,sizeof(tn));				// time now (modified)
			if (ow) sql->BindParameter(8,SQL_INTEGER,sizeof(owid),&owid,sizeof(owid));	// ownerid (either group or user)


			int res = sql->ExecuteSQL("INSERT INTO dm.dm_environment(%s) VALUES(%s)",collist,sellist);
			if ((res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) && sql->GetRowCount() > 0) {
				bool commit=true;

				// Go through server list (if specified)
				char *serv = strtok(NULL,",|");
				while (serv) {
					// Find server.
					char *sp = serv;
					while (*sp==' ' || *sp=='\t') sp++;
					int servid = getServerID(model,sp);
					switch (servid) {
					case DOMAIN_NOT_FOUND:
					case DOMAIN_SYNTAX_ERROR:
						printf("ERROR: Could not find server \"%s\" at line %d\n",sp,lineno);
						commit=false;
						break;
					case DOMAIN_OBJECT_AMBIGUOUS:
						printf("ERROR: Server \"%s\" at line %d is ambiguous. Please Qualify with Domain\n",sp,lineno);
						commit=false;
						break;
					default:
						// Found the server - add it to the mapping table
						sql->BindParameter(1,SQL_INTEGER,sizeof(id),&id,sizeof(id));
						sql->BindParameter(2,SQL_INTEGER,sizeof(servid),&servid,sizeof(servid));
						sql->ExecuteSQL("INSERT INTO dm.dm_serversinenv(envid,serverid) VALUES(?,?)");
						if (sql->GetRowCount() < 1) {
							printf("ERROR: Failed to add server %d to environment %d\n",servid,id);
							commit=false;
						}
					}
					serv=strtok(NULL," ,|");
				}

				if (commit) printf("Environment \"%s\" Created\n",nm);
				sql->EndTransaction(commit);
			} else {
				// Error somewhere
				sql->EndTransaction(false);
			}
			sql->CloseSQL();
		}
	} else {
		fprintf(stderr,"Could not open or read file \"%s\": %s\n",filename,strerror(errno));
	}
	exit(0);
}

void dumpDBCreds(DM &dm,const char *homeDir)
{
	// Open the dm.odbc file and dumps it to stdout
	char odbcfilename[1024];
	sprintf(odbcfilename, "%s%sdm.odbc", homeDir, DIR_SEP_STR);
	debug3("odbcfilename = '%s'", odbcfilename);

	int res=0;
	// Does the odbc file exist?
	struct stat sb;
	if(stat(odbcfilename, &sb) == -1) {
		if(errno == ENOENT) {
			// Doesn't exist - so create it
			dm.exitWithError("FATAL ERROR: The file dm.odbc does not exist.  Please configure\n"
				"             the database connection using the dmsetup tool.");
		} else {
			dm.exitWithError("FATAL ERROR: Unable to stat odbc file");
		}
	} else {
		int s = sb.st_size;
		char *buf = decryptFile(odbcfilename,&s);
		// EncryptBuffer(buf,s);
		write(1,buf,s);
	}
	exit(res);
}

void loadServersAndExit(Model &model,triODBC &odbc,char *filename)
{
	int lineno=0;
	FILE *in = fopen(filename,"r");
	if (in) {
		while (char *sp=readLine(in)) {
			lineno++;
			if (strlen(sp)==0) continue;
			// Syntax is:
			// servername,hostname,domain,type,protocol,basedir,credid (optional),[U|G]ownerid (optional), environments (optional)
			//
			char *nm = strtok(sp,",|");
			char *hn = strtok(NULL,",|");
			char *dn = strtok(NULL,",|");
			char *ty = strtok(NULL,",|");
			char *pr = strtok(NULL,",|");
			char *bd = strtok(NULL,",|");
			char *cr = bd?strtok(NULL,",|"):(char *)0;
			char *ow = cr?strtok(NULL,",|"):(char *)0;
			char collist[1024];
			char sellist[128];
			char *cp = (char *)0;
			if (cr && cr[0]=='*' && cr[1]=='\0') cr=NULL;
			if (ow && ow[0]=='*' && ow[1]=='\0') ow=NULL;
			int crid=0;
			int owid=0;
			int tyid=getTypeID(odbc,ty);
			if (tyid==0) {
				printf("ERROR: Invalid server type \"%s\" specified line %d\n",ty,lineno);
				continue;
			}
			int prid=getProtocolID(odbc,pr);
			if (prid==0) {
				printf("ERROR: Invalid Protocol \"%s\" specified line %d\n",pr,lineno);
				continue;
			}
			int domid=model.getDomainID(dn);
			switch (domid) {
				case DOMAIN_NOT_FOUND:
				case DOMAIN_SYNTAX_ERROR:
					printf("Error at line %d: Invalid Domain \"%s\" specified\n",lineno,dn);
					continue;
				case DOMAIN_OBJECT_AMBIGUOUS:
					printf("Error at line %d: Ambiguous Domain \"%s\" specified. Please use Fully Qualified Domain\n",lineno,dn);
					continue;
			}
			//                ?   ?     ?         ?       ?       ?       ?         ?        ?          ?        ?   
			sprintf(collist,"id,name,hostname,domainid,typeid,protocol,basedir,creatorid,created,modifierid,modified,status");
			sprintf(sellist,"?,?,?,?,?,?,?,?,?,?,?,'N'");
			if (cr) {
				strcat(collist,",credid");
				strcat(sellist,",?");
				crid=getCredentialID(model,cr);
				switch (crid) {
				case DOMAIN_NOT_FOUND:
					printf("Error at line %d: Invalid Domain \"%s\" specified\n",lineno,cr);
					continue;
				case DOMAIN_SYNTAX_ERROR:
					printf("Error at line %d: Invalid Credential \"%s\" specified\n",lineno,cr);
					continue;
				case DOMAIN_OBJECT_AMBIGUOUS:
					printf("Error at line %d: Ambiguous Credential \"%s\" specified. Please Qualify with Domain\n",lineno,cr);
					continue;
				}
			}
			if (ow) {
				switch(*ow) {
					case 'U':
						strcat(collist,",ownerid");
						strcat(sellist,",?");
						owid=getUserID(model,&(ow[1]));
						break;
					case 'G':
						strcat(collist,",ogrpid");
						strcat(sellist,",?");
						owid=getUserGroupID(model,&(ow[1]));
						break;
					default:
						printf("ERROR: Owner needs to start with U or G (line %d) - line ignored\n",lineno);
						continue;
				}
				switch (owid) {
				case DOMAIN_NOT_FOUND:
					printf("Error at line %d: Invalid %s \"%s\" specified\n",lineno,*ow=='U'?"User":"Usergroup",ow+1);
					continue;
				case DOMAIN_SYNTAX_ERROR:
					printf("Error at line %d: Invalid %s \"%s\" specified\n",lineno,*ow=='U'?"User":"Usergroup",ow+1);
					continue;
				case DOMAIN_OBJECT_AMBIGUOUS:
					printf("Error at line %d: Ambiguous %s \"%s\" specified. Please Qualify with Domain\n",lineno,*ow=='U'?"User":"Usergroup",ow+1);
					continue;
				}
			}
			AutoPtr<triSQL> sql = odbc.GetSQL();
			sql->SetAutoCommitMode(false);
			int me=1;	// temp (need to be able to login)
			time_t tn;
			time(&tn);
			int c,id;
			sql->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
			sql->ExecuteSQL("SELECT count(*) FROM dm.dm_server WHERE name='%s' AND domainid = %d",nm,domid);
			sql->FetchRow();
			sql->CloseSQL();
			if (id>0) {
				printf("ERROR: Could not load server \"%s\" - a server with that name already exists in domain \"%s\"\n",nm,dn);
				continue;
			}
			sql->BindColumn(1,SQL_INTEGER,&c,sizeof(c));
			sql->ExecuteSQL("SELECT count(*) FROM dm.dm_server");
			sql->FetchRow();
			sql->CloseSQL();
			if (c==0) {
				// No servers as yet
				id=1;
			} else {
				sql->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
				sql->ExecuteSQL("SELECT max(id)+1 FROM dm.dm_server");
				sql->FetchRow();
				sql->CloseSQL();
			}
			sql->BindParameter(1,SQL_INTEGER,sizeof(id),&id,sizeof(id));				// id
			sql->BindParameter(2,SQL_CHAR,strlen(nm),nm,strlen(nm));					// server name
			sql->BindParameter(3,SQL_CHAR,strlen(hn),hn,strlen(hn));					// hostname
			sql->BindParameter(4,SQL_INTEGER,sizeof(domid),&domid,sizeof(domid));		// domainid
			sql->BindParameter(5,SQL_INTEGER,sizeof(tyid),&tyid,sizeof(tyid));			// typeid
			sql->BindParameter(6,SQL_CHAR,strlen(pr),pr,strlen(pr));					// protocol
			sql->BindParameter(7,SQL_CHAR,strlen(bd),bd,strlen(bd));					// base directory
			sql->BindParameter(8,SQL_INTEGER,sizeof(me),&me,sizeof(me));				// creator id
			sql->BindParameter(9,SQL_INTEGER,sizeof(tn),&tn,sizeof(tn));				// time now (created)
			sql->BindParameter(10,SQL_INTEGER,sizeof(me),&me,sizeof(me));				// modifier id
			sql->BindParameter(11,SQL_INTEGER,sizeof(tn),&tn,sizeof(tn));				// time now (modified)
			if (cr) sql->BindParameter(12,SQL_INTEGER,sizeof(crid),&crid,sizeof(crid));	// credid
			if (ow) sql->BindParameter(cr?13:12,SQL_INTEGER,sizeof(owid),&owid,sizeof(owid));	// ownerid (either group or user)



			int res = sql->ExecuteSQL("INSERT INTO dm.dm_server(%s)	VALUES(%s)",collist,sellist);
			if ((res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) && sql->GetRowCount() > 0) {
				bool commit=true;
	
				// Go through environment list (if specified)
				char *env = strtok(NULL,",|");
				while (env) {
					// Find environment. If it does not exist, report it and ignore
					char *sp = env;
					while (*sp==' ' || *sp=='\t') sp++;
					int envid = getEnvironmentID(model,sp);
					switch (envid) {
					case DOMAIN_NOT_FOUND:
					case DOMAIN_SYNTAX_ERROR:
						printf("ERROR: Could not find environment \"%s\" at line %d\n",sp,lineno);
						commit=false;
						break;
					case DOMAIN_OBJECT_AMBIGUOUS:
						printf("ERROR: Environment \"%s\" at line %d is ambiguous. Please Qualify with Domain\n",sp,lineno);
						commit=false;
						break;
					default:
						// Found the environment - add it to the mapping table
						sql->BindParameter(1,SQL_INTEGER,sizeof(id),&id,sizeof(id));
						sql->BindParameter(2,SQL_INTEGER,sizeof(envid),&envid,sizeof(envid));
						sql->ExecuteSQL("INSERT INTO dm.dm_serversinenv(serverid,envid) VALUES(?,?)");
						if (sql->GetRowCount() < 1) {
							printf("ERROR: Failed to add server %d to environment %d\n",id,envid);
							commit=false;
						}
					}
					env=strtok(NULL," ,|");
				}

				if (commit) printf("Server \"%s\" Created\n",nm);
				sql->EndTransaction(commit);
			} else {
				// Error somewhere
				sql->EndTransaction(false);
			}
			sql->CloseSQL();
		}
	} else {
		fprintf(stderr,"Could not open or read file \"%s\": %s\n",filename,strerror(errno));
	}
	exit(0);
}

void loadUsersAndExit(Model &model,triODBC &odbc,char *filename)
{
	int lineno=0;
	FILE *in = fopen(filename,"r");
	if (in) {
		while (char *sp=readLine(in)) {
			lineno++;
			if (strlen(sp)==0) continue;
			// Syntax is:
			// loginname,fullname,domain,password (optional),email (optional),phone (optional), groups (optional)
			//
			char *nm = strtok(sp,",|");
			char *fn = strtok(NULL,",|");
			char *dn = strtok(NULL,",|");
			char *pw = dn?strtok(NULL,",|"):(char *)0;
			char *em = pw?strtok(NULL,",|"):(char *)0;
			char *pn = em?strtok(NULL,",|"):(char *)0;
			char collist[1024];
			char sellist[128];
			char *cp = (char *)0;
			int domid=model.getDomainID(dn);
			switch (domid) {
				case DOMAIN_NOT_FOUND:
				case DOMAIN_SYNTAX_ERROR:
					printf("Error at line %d: Invalid Domain \"%s\" specified\n",lineno,dn);
					continue;
				case DOMAIN_OBJECT_AMBIGUOUS:
					printf("Error at line %d: Ambiguous Domain \"%s\" specified. Please use Fully Qualified Domain\n",lineno,dn);
					continue;
			}
			//                ?   ?     ?        ?         ?        ?       ?          ?        
			sprintf(collist,"id,name,realname,domainid,creatorid,created,modifierid,modified,locked,forcechange,status");
			sprintf(sellist,"?,?,?,?,?,?,?,?,'N','N','N'");
			if (pw) {
				strcat(collist,",passhash");
				strcat(sellist,",?");
				cp = digestValue(pw, strlen(pw), true);
			}
			if (em) {
				strcat(collist,",email");
				strcat(sellist,",?");
			}
			if (pn) {
				strcat(collist,",phone");
				strcat(sellist,",?");
			}
			AutoPtr<triSQL> sql = odbc.GetSQL();
			sql->SetAutoCommitMode(false);
			int me=1;	// temp (need to be able to login)
			time_t tn;
			time(&tn);
			int id;
			sql->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
			sql->ExecuteSQL("SELECT count(*) FROM dm.dm_user WHERE name='%s' AND domainid = %d",nm,domid);
			sql->FetchRow();
			sql->CloseSQL();
			if (id>0) {
				printf("ERROR: Could not load user \"%s\" - a user with that name already exists in domain \"%s\"\n",nm,dn);
				continue;
			}
			sql->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
			sql->ExecuteSQL("SELECT max(id)+1 FROM dm.dm_user");
			sql->FetchRow();
			sql->CloseSQL();
			sql->BindParameter(1,SQL_INTEGER,sizeof(id),&id,sizeof(id));			// id
			sql->BindParameter(2,SQL_CHAR,strlen(nm),nm,strlen(nm));				// login name
			sql->BindParameter(3,SQL_CHAR,strlen(fn),fn,strlen(fn));				// full name
			sql->BindParameter(4,SQL_INTEGER,sizeof(domid),&domid,sizeof(domid));	// domain id
			sql->BindParameter(5,SQL_INTEGER,sizeof(me),&me,sizeof(me));			// creator id
			sql->BindParameter(6,SQL_INTEGER,sizeof(tn),&tn,sizeof(tn));			// time now (created)
			sql->BindParameter(7,SQL_INTEGER,sizeof(me),&me,sizeof(me));			// modifier id
			sql->BindParameter(8,SQL_INTEGER,sizeof(tn),&tn,sizeof(tn));			// time now (modified)
			if (pw) sql->BindParameter(9,SQL_CHAR,strlen(cp),cp,strlen(cp));		// converted password
			if (em) sql->BindParameter(10,SQL_CHAR,strlen(em),em,strlen(em));		// email
			if (pn) sql->BindParameter(11,SQL_CHAR,strlen(pn),pn,strlen(pn));		// phone number

			int res = sql->ExecuteSQL("INSERT INTO dm.dm_user(%s) VALUES (%s)",collist,sellist);
			if ((res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) && sql->GetRowCount() >0) {
				bool commit=true;
				if (pn) {	
					// We got a phone number - go through the group list
					char *gp = strtok(NULL,",|");
					while (gp) {
						// Find group. If it does not exist, report it and ignore
						char *sp = gp;
						while (*sp==' ' || *sp=='\t') sp++;
						int gid = getUserGroupID(model,sp);
						
						switch (gid) {
						case DOMAIN_NOT_FOUND:
						case DOMAIN_SYNTAX_ERROR:
							printf("ERROR: Could not find user group \"%s\" at line %d\n",sp,lineno);
							commit=false;
							break;
						case DOMAIN_OBJECT_AMBIGUOUS:
							printf("ERROR: User Group \"%s\" at line %d is ambiguous. Please Qualify with Domain\n",sp,lineno);
							commit=false;
							break;
						default:
							// Found the group - add it to the mapping table
							sql->BindParameter(1,SQL_INTEGER,sizeof(id),&id,sizeof(id));
							sql->BindParameter(2,SQL_INTEGER,sizeof(gid),&gid,sizeof(gid));
							sql->ExecuteSQL("INSERT INTO dm.dm_usersingroup(userid,groupid) VALUES(?,?)");
							if (sql->GetRowCount() < 1) {
								printf("ERROR: Failed to add user %d to group %d\n",id,gid);
								commit=false;
							}
						}
						gp=strtok(NULL," ,|");
					}
				}
				if (commit) printf("User \"%s\" Created\n",nm);
				sql->EndTransaction(commit);
			} else {
				// Error somewhere
				sql->EndTransaction(false);
			}
		}
		fclose(in);
	} else {
		fprintf(stderr,"Could not open or read file \"%s\": %s\n",filename,strerror(errno));
	}
	exit(0);
}

int getFirstEnvironmentForServer(triODBC &odbc,int serverid)
{
	int envid=0;
	AutoPtr<triSQL> sql = odbc.GetSQL();
	sql->BindColumn(1,SQL_INTEGER,&envid,sizeof(envid));
	sql->ExecuteSQL("SELECT min(envid) FROM dm_serversinenv WHERE serverid=%d",serverid);
	sql->FetchRow();
	sql->CloseSQL();
	return envid;
}

#define CHECK_NAME_RESOLUTION_OKAY 8
#define CHECK_PING_OKAY 4
#define CHECK_CONNECTION_OKAY 2
#define CHECK_BASEDIR_OKAY 1

void UpdateDB(triODBC &odbc,Context &ctx,int serverid,int status,char *ipaddr,unsigned long pingtime,char *uname,char *ErrorText)
{
	char nameresolution[2];
	char ping[2];
	char connection[2];
	char basedir[2];

	char orig_nameresolution[2];
	char orig_ping[2];
	char orig_connection[2];
	char orig_basedir[2];
	char servername[2049];
	char hostname[2049];
	SQLLEN ni_orig_nameresolution;
	SQLLEN ni_orig_ping;
	SQLLEN ni_orig_connection;
	SQLLEN ni_orig_basedir;
	SQLLEN ni_servername;
	SQLLEN ni_hostname;

	char *reason = getenv("TRIREASON");
	bool onTimer = reason?(strcmp(reason,"TIMED")==0):false;

	nameresolution[0]=(status & CHECK_NAME_RESOLUTION_OKAY)?'Y':'N';
	ping[0]=(status & CHECK_PING_OKAY)?'Y':'N';
	connection[0]=(status & CHECK_CONNECTION_OKAY)?'Y':'N';
	basedir[0]=(status & CHECK_BASEDIR_OKAY)?'Y':'N';
	nameresolution[1]=ping[1]=connection[1]=basedir[1]='\0';

	if (uname) {
		// uname is set - update the server record
		AutoPtr<triSQL> sqlun = odbc.GetSQL();
		sqlun->PrepareStatement("UPDATE dm_server SET uname=? WHERE id=?");
		sqlun->BindParameter(1,SQL_CHAR,strlen(uname),uname,strlen(uname));
		sqlun->BindParameter(2,SQL_INTEGER,sizeof(serverid),&serverid,sizeof(serverid));
		sqlun->Execute();
		sqlun->CloseSQL();
	}

	AutoPtr<triSQL> sql = odbc.GetSQL();

	sql->BindColumn(1,SQL_CHAR,orig_nameresolution,sizeof(orig_nameresolution),&ni_orig_nameresolution);
	sql->BindColumn(2,SQL_CHAR,orig_ping,sizeof(orig_ping),&ni_orig_ping);
	sql->BindColumn(3,SQL_CHAR,orig_connection,sizeof(orig_connection),&ni_orig_connection);
	sql->BindColumn(4,SQL_CHAR,orig_basedir,sizeof(orig_basedir),&ni_orig_basedir);
	sql->BindColumn(5,SQL_CHAR,servername,sizeof(servername),&ni_servername);
	sql->BindColumn(6,SQL_CHAR,hostname,sizeof(hostname),&ni_hostname);

	sql->ExecuteSQL(	"SELECT a.nameresolution,a.ping,a.connection,a.basedir,b.name,b.hostname "
						"FROM dm_serverstatus a,dm_server b WHERE a.serverid=%d AND b.id = a.serverid",serverid);
	SQLRETURN res = sql->FetchRow();
	sql->CloseSQL();
	long now = time(NULL);
	if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
		//
		// There's already a row for this server. Update the row. If we're changing any state then issue the
		// appropriate notification
		//
		bool failed=false;
		bool changed=false;
		if (ni_orig_nameresolution != -1 && nameresolution[0] != orig_nameresolution[0]) {
			if (nameresolution[0]=='N') failed=true;
			changed=true;
		}
		if (ni_orig_ping != -1 && ping[0] != orig_ping[0]) {
			if (ping[0]=='N') failed=true;
			changed=true;
		}
		if (ni_orig_connection != -1 && connection[0] != orig_connection[0]) {
			if (connection[0]=='N') failed=true;
			changed=true;
		}
		if (ni_orig_basedir != -1 && basedir[0] != orig_basedir[0]) {
			if (basedir[0]=='N') failed=true;
			changed=true;
		}
		if (changed && onTimer) {
			// Server has changed state since our last check.
			// Set variables for notification body expansion
			ctx.stack().setGlobal("SERVER_NAME",servername);
			ctx.stack().setGlobal("SERVER_HOSTNAME",hostname);
			ctx.stack().setGlobal("NAME_RESOLUTION",(nameresolution[0]=='Y')?"OK":"FAILED");
			ctx.stack().setGlobal("PING",(ping[0]=='Y')?"OK":"FAILED");
			ctx.stack().setGlobal("CONNECTION",(connection[0]=='Y')?"OK":"FAILED");
			ctx.stack().setGlobal("BASEDIR",(basedir[0]=='Y')?"OK":"FAILED");
			ctx.stack().setGlobal("SERVER_STATUS",failed?"FAILED":"OK");
			ctx.stack().setGlobal("ERROR_TEXT",ErrorText?ErrorText:"");
			Model *model = ctx.dm().getModel();
			if (model) {
				Server *server = model->getServer(serverid);
				if (server) {
					NotifyTemplate *tmpl = server->getPingTemplate(ctx);
					ctx.dm().setCurrentUser("admin",NULL, NULL);	// Mmmmm .... 
					if (tmpl) ctx.dm().internalNotify(ctx,tmpl);
				}
			}
		}
		char updsql[2048];
		AutoPtr<triSQL> sql2 = odbc.GetSQL();
		strcpy(updsql,"UPDATE dm_serverstatus SET nameresolution=?, ping=?, connection=?, basedir=?, lasterror=?, lasttime=?, pingtime=?");
		int n=8;
		if (ipaddr) {
			strcat(updsql,", ipaddr=?");
			sql2->BindParameter(n,SQL_CHAR,strlen(ipaddr),ipaddr,strlen(ipaddr));
			n++;
		}
		int a1=ErrorText?strlen(ErrorText):0;
		strcat(updsql," where serverid=?");
		sql2->PrepareStatement(updsql);
		sql2->BindParameter(1,SQL_CHAR,strlen(nameresolution),nameresolution,strlen(nameresolution));
		sql2->BindParameter(2,SQL_CHAR,strlen(ping),ping,strlen(ping));
		sql2->BindParameter(3,SQL_CHAR,strlen(connection),connection,strlen(connection));
		sql2->BindParameter(4,SQL_CHAR,strlen(basedir),basedir,strlen(basedir));
		sql2->BindParameter(5,SQL_CHAR,a1,ErrorText?ErrorText:(char *)"",a1);
		sql2->BindParameter(6,SQL_INTEGER,sizeof(now),&now,sizeof(now));
		sql2->BindParameter(7,SQL_INTEGER,sizeof(pingtime),&pingtime,sizeof(pingtime));
		sql2->BindParameter(n,SQL_INTEGER,sizeof(serverid),&serverid,sizeof(serverid));
		sql2->Execute();
		sql2->CloseSQL();
	} else {
		// First connection test for this server
		AutoPtr<triSQL> sql3 = odbc.GetSQL();
		sql3->PrepareStatement("INSERT INTO dm_serverstatus(serverid,nameresolution,ping,connection,basedir,ipaddr,lasterror,lasttime,pingtime)"
			"	VALUES(?,?,?,?,?,?,?,?,?)");
		int a1=ipaddr?strlen(ipaddr):0;
		int a2=uname?strlen(uname):0;
		int a3=ErrorText?strlen(ErrorText):0;
		sql3->BindParameter(1,SQL_INTEGER,sizeof(serverid),&serverid,sizeof(serverid));
		sql3->BindParameter(2,SQL_CHAR,strlen(nameresolution),nameresolution,strlen(nameresolution));
		sql3->BindParameter(3,SQL_CHAR,strlen(ping),ping,strlen(ping));
		sql3->BindParameter(4,SQL_CHAR,strlen(connection),connection,strlen(connection));
		sql3->BindParameter(5,SQL_CHAR,strlen(basedir),basedir,strlen(basedir));
		sql3->BindParameter(6,SQL_CHAR,a1,ipaddr?ipaddr:(char *)"",a1);
		sql3->BindParameter(7,SQL_CHAR,a3,ErrorText?ErrorText:(char *)"",a3);
		sql3->BindParameter(8,SQL_INTEGER,sizeof(now),&now,sizeof(now));
		sql3->BindParameter(9,SQL_INTEGER,sizeof(pingtime),&pingtime,sizeof(pingtime));
		sql3->Execute();
		sql3->CloseSQL();
	}
}

int CheckServerConnectivity(Model &model,DM &dm,triODBC &odbc,int serverid)
{
	Server *s = model.getServerById(serverid);
	ServerType *st = s->serverType();
	
	List<Server> serverSet;
	ScopeStack stack(dm);
	
	stack.setGlobal("DMHOME", dm.getBaseDir());
	stack.setGlobal("TRIDM_PLATFORM", DM_PLATFORM);

	serverSet.add(s);
	stack.push(s->getVars());

	Context ctx(dm,serverSet,stack);

	int status=0;
	char *ipaddr;
	unsigned long RoundTripTime;
	int res = ping(s->hostname(),&ipaddr,&RoundTripTime);
	switch(res)
	{
	case PING_OK:
		// Name Resolution Worked and the server responded to ICMP_ECHO
		status = CHECK_NAME_RESOLUTION_OKAY | CHECK_PING_OKAY;
		break;
	case PING_NAME_RESOLUTION_FAILED:
		// hostname of the server could not be resolved
		UpdateDB(odbc,ctx,serverid,status,(char *)0,0,(char *)0,"Could not resolve hostname");
		return status;
	case PING_NO_REPLY:
		// server was found but did not respond
		status = CHECK_NAME_RESOLUTION_OKAY;
		UpdateDB(odbc,ctx,serverid,status,ipaddr,0,(char *)0,"Server did not respond to ping");
		return status;
	case PING_INTERNAL_ERROR:
		UpdateDB(odbc,ctx,serverid,status,(char *)0,0,(char *)0,"Ping failed with internal error");
		return status;
	}
	// If we get here then ping is okay and status is CHECK_NAME_RESOLUTION_OKAY | CHECK_PING_OKAY
	// Get the server's environment - note there could be more than one of these. Shouldn't matter
	//
	TransferProviderImplRegistry::instance().registerBuiltIns();

	Environment *env = (Environment *)0;
	int envid = getFirstEnvironmentForServer(odbc,serverid);
	if (envid > 0) env = model.getEnvironment(envid);
	
	Credentials *creds = s->getCredentials();
	if(!creds && env) {
		creds = env->getCredentials();
	}

	dm.setCurrentUser("admin",NULL, NULL);	// what happens if someone renames the admin user?

	TransferProviderImplFactory *factory = TransferProviderImplRegistry::instance().getFactory(s->protocol());
	if(!factory) {
		UpdateDB(odbc,ctx,serverid,status,ipaddr,RoundTripTime,(char *)0,"Unable to find transfer provider");
		return status;
	}

	TransferProviderImpl *xfer = factory->create(*s, env, creds, NULL, NULL);
	if(!xfer) {
		UpdateDB(odbc,ctx,serverid,status,ipaddr,RoundTripTime,(char *)0,"Unable to create transfer provider");
		return status;
	}


	OutputStream output;
	Context newctx(ctx,&output);
	int exitcode = xfer->checkServer(newctx);
	switch(exitcode)
	{
	case 0:
		// All good - connection okay and base directory is accessible
		status = status | CHECK_CONNECTION_OKAY | CHECK_BASEDIR_OKAY;
		break;
	case 1:
		// Failed to connect
		break;
	case 2:
		// Connected okay but basedir could not be accessed
		status = status | CHECK_CONNECTION_OKAY;
		break;
	case 3:
		// Connected okay but no base directory was specified
		status = status | CHECK_CONNECTION_OKAY;
		break;
	}
	char *uname = (char *)0;
	char *ErrorText = (char *)0;
	
	if (!(status & CHECK_BASEDIR_OKAY)) {
		// Base Directory invalid
		ErrorText=strdup(output.buffer());
		int n=strlen(ErrorText)-1;
		while (n && (ErrorText[n]=='\n' || ErrorText[n]=='\r' || ErrorText[n]==' ' || ErrorText[n]=='\t')) ErrorText[n--]='\0';
	}
	if (status & CHECK_CONNECTION_OKAY) {
		if (strcmp(st->hosttype(),"unix")==0) {
			// Unix - let's try a uname command, see what we get.
			int cos = output.size();
			StringList *p = new StringList();
			exitcode = xfer->runScriptOnServer(NULL,"uname",p,false,false,newctx);
			if ((output.size() - cos) > 0) {
				// uname output
				uname = strdup(output.buffer());
				// strip off any CR/LF combos at the end of the line
				int n=strlen(uname)-1;
				while (n && (uname[n]=='\n' || uname[n]=='\r' || uname[n]==' ' || uname[n]=='\t')) uname[n--]='\0';
				char *uname2 = strdup(&(uname[cos]));
				free(uname);
				uname = uname2;
			}
		} else if (strcmp(st->hosttype(),"windows")==0) {
			uname = strdup("windows");
		} else if (strcmp(st->hosttype(),"as400")==0) {
			uname = strdup("as400");
		}
		
	} else {
		ErrorText=strdup(output.buffer());
		int n=strlen(ErrorText)-1;
		while (n && (ErrorText[n]=='\n' || ErrorText[n]=='\r' || ErrorText[n]==' ' || ErrorText[n]=='\t')) ErrorText[n--]='\0';
	}
	UpdateDB(odbc,ctx,serverid,status,ipaddr,RoundTripTime,uname,ErrorText);
	if (uname) free(uname);
	if (ErrorText) free(ErrorText);
	return status;
}

void ScanServer(Model &model,DM &dm,triODBC &odbc,int serverid)
{
	char targetpath[2049];
	char origmd5[128];
	char *discrepancyList = (char *)0;

	Server *s = model.getServerById(serverid);

	List<Server> serverSet;
	ScopeStack stack(dm);
	
	stack.setGlobal("DMHOME", dm.getBaseDir());
	stack.setGlobal("TRIDM_PLATFORM", DM_PLATFORM);

	serverSet.add(s);
	stack.push(s->getVars());
	Context ctx(dm,serverSet,stack);
	//
	// Get the server's environment - note there could be more than one of these. Shouldn't matter
	//
	int envid = getFirstEnvironmentForServer(odbc,serverid);
	Environment *env = model.getEnvironment(envid);

	if (!env) return;	// if no environment then bail out - we've never deployed to the server anyway

	TransferProviderImplRegistry::instance().registerBuiltIns();
	Credentials *creds = s->getCredentials();
	if(!creds) {
		debug1("Getting Environment '%s' credentials", env->name());
		creds = env->getCredentials();
	}

	dm.setCurrentUser("admin",NULL, NULL);	// what happens if someone renames the admin user?

	// printf("envid=%d name=[%s]\n",env->id(),env->name());

	AutoPtr<triSQL> sql1 = odbc.GetSQL();
	AutoPtr<triSQL> sql2 = odbc.GetSQL();
	AutoPtr<triSQL> sql3 = odbc.GetSQL();
	AutoPtr<triSQL> sql4 = odbc.GetSQL();
	AutoPtr<triSQL> sql5 = odbc.GetSQL();
	AutoPtr<triSQL> sql6 = odbc.GetSQL();
	AutoPtr<triSQL> sql7 = odbc.GetSQL();
	// Get list of all the files that have been deployed to this server
	int res1,res2;
	sql1->BindColumn(1,SQL_C_CHAR,targetpath,sizeof(targetpath));
	
	
	res1 = sql1->ExecuteSQL("SELECT DISTINCT targetfilename FROM dm_deploymentxfer WHERE serverid=%d",serverid);
	for(res1 = sql1->FetchRow(); (res1 == SQL_SUCCESS) || (res1 == SQL_SUCCESS_WITH_INFO); res1 = sql1->FetchRow()) {
		sql2->BindColumn(1,SQL_C_CHAR,origmd5,sizeof(origmd5));
		res2 = sql2->ExecuteSQL(	"select checksum2 from dm_deploymentxfer a where a.serverid=%d and a.targetfilename='%s' and a.componentid is null "
								"and	a.deploymentid =(select max(b.deploymentid) from dm_deploymentxfer b where b.targetfilename=a.targetfilename)",
								serverid,targetpath);
		res2 = sql2->FetchRow();
		if (res2 == SQL_NO_DATA) {
			// No row where componentid is null (modified) - try not null
			sql3->BindColumn(1,SQL_C_CHAR,origmd5,sizeof(origmd5));
			res2 = sql3->ExecuteSQL(	"select checksum2 from dm_deploymentxfer a where a.serverid=%d and a.targetfilename='%s' and a.componentid is not null "
								"and	a.deploymentid =(select max(b.deploymentid) from dm_deploymentxfer b where b.targetfilename=a.targetfilename)",
								serverid,targetpath);
			res2 = sql3->FetchRow();
			sql3->CloseSQL();
		}
		sql2->CloseSQL();
		if (res2 == SQL_SUCCESS || res2 == SQL_SUCCESS_WITH_INFO) {
			// Got an MD5 sum for this path
			printf("File:     %s\nOrig MD5: %s\n",targetpath,origmd5);
				

			TransferProviderImplFactory *factory = TransferProviderImplRegistry::instance().getFactory(s->protocol());
			if(!factory) {
				fprintf(stderr,"Unable to find transfer provider for protocol '%s'",
					(s->protocol() ? s->protocol() : "(null)"));
				exit(1);
			}

			TransferProviderImpl *xfer = factory->create(*s, env, creds, NULL, NULL);	// PAG
			if(!xfer) {
				fprintf(stderr,"Unable to create transfer provider for server '%s'", s->name());
				exit(1);
			}

			StringList *params = new StringList();

			List<Server> emptySet;
			ScopeStack stack(dm);

			Context ctx(dm,emptySet,stack);

			// printf("Running md5sum %s on server %s over protocol %s\n",targetpath,s->name(),s->protocol());

			OutputStream output;
			Context newctx(ctx,&output);
			//
			// md5sum is intercepted by the windows "invisible agent". If we're running against a Unix/Linux
			// server then we'll need to check the uname to see what we need to do...
			//
			ServerType *st = s->serverType();
			char *uname = (char *)0;
			char *md5cmd = "md5sum";
			int bsp=0;	// buffer start point (in case we run uname)
			bool SolarisFormat=false;
			if (stricmp(st->hosttype(),"unix")==0) {
				// We're targeting a Unix/Linux server - do we have a uname?
				if (!s->uname()) {
					// uname has not yet been confirmed - execute it here and store it away
					// printf("uname not confirmed - getting it now\n");
					StringList *noparams = new StringList();
					int ec = xfer->runScriptOnServer(NULL,"uname",noparams,false,false,newctx);
					if (ec == 0) {
						// It ran successfully
						uname = strdup(output.buffer());
						bsp = output.size();
						// store the uname for future reference
						s->setUname(uname);
					}
				} else {
					// We know the uname for this Unix server
					uname = strdup(s->uname());
				}
				// printf("uname is %s\n",uname);
				if (stricmp(uname,"aix")==0) {
					// AIX MD5 syntax
					md5cmd="csum";
					params->add("-h");
					params->add("MD5");
				}
				else
				if (stricmp(uname,"sunos")==0) {
					// Solaris MD5 syntax
					md5cmd="digest";
					params->add("-a");
					params->add("md5");
					params->add("-v");
					SolarisFormat=true;
				}
			}
			if (uname) free(uname);
			params->add(targetpath);
			int exitcode = xfer->runScriptOnServer(NULL,md5cmd,params,false,false,newctx);
			// printf("exitcode=%d\n",exitcode);
			if (exitcode == 0) {
				// md5sum executed okay - let's store it
				char *tmp = (char *)output.buffer();
				char *calcMD5 = &(tmp[bsp]); // Buffer Start Point may be non-zero if we've run "uname"
				if (SolarisFormat) {
					// output will be md5 (filename) = 0c4627e70d168f7f78257e6dd01fdb60
					// printf("calcMD5=[%s]\n",calcMD5);
					char *p = calcMD5;
					while (*p && *p != '=') p++;
					if (*p=='=') calcMD5=(p+2);
					// printf("calcMD5=[%s]\n",calcMD5);
				} else {
					// output with be XXXXXXXXXXXXXXXX  filename
					// where XXXX is the MD5 sum
					// printf("calcMD5=[%s]\n",calcMD5);
					// Check MD5 is valid. If not, it must be an error message
					bool valid=true;
					if (calcMD5[32]!=' ') valid=false;
					if (valid) {
						// MD5 should only contain hex characters
						for (int i=0;i<32;i++) {
							if (!((tolower(calcMD5[i])>='a' && tolower(calcMD5[i])<='f') || (calcMD5[i]>='0' && calcMD5[i]<='9'))) {
								valid=false;
								break;
							}
						}
					}
					if (valid) calcMD5[32]='\0';
				}
				printf("Calc MD5: %s\n",calcMD5);
				if (strcmp(calcMD5,origmd5)) {
					// Calculated MD5 and Original (deployed) MD5 differ - file has changed.
					// Check the dm_discovery table and check the last detectedmd5. If that's the same
					// as we've just calculated then we've already seen this discrepancy and we don't
					// want to report it again.
					//
					char detectedmd5[128];
					SQLLEN ni_detectedmd5;
					bool mismatch=false;
					sql7->PrepareStatement("SELECT detectedmd5 FROM dm_discovery WHERE serverid=? AND targetfilename=?");
					sql7->BindParameter(1, SQL_INTEGER, sizeof(serverid),   &serverid,  sizeof(serverid));
					sql7->BindParameter(2, SQL_C_CHAR,  sizeof(targetpath), targetpath, strlen(targetpath));
					sql7->BindColumn(1, SQL_C_CHAR, detectedmd5, sizeof(detectedmd5), &ni_detectedmd5);
					SQLRETURN res = sql7->Execute();
					if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) res = sql7->FetchRow();
					if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
						// Got a row from the dm_discovery table - must have previously been checked. Does the last-checked
						// MD5 match the one we've just calculated?
						if (strcmp(detectedmd5,calcMD5)) mismatch=true;	// No, they don't match
					} else if (res == SQL_NO_DATA) mismatch=true;		// No row yet in dm_discovery - first time detected and mismatched
					sql7->CloseSQL();
					if (mismatch) {
						if (!discrepancyList) {
							discrepancyList = (char *)malloc(strlen(targetpath)+5);
							discrepancyList[0]='\0';
						} else {
							discrepancyList = (char *)realloc(discrepancyList,strlen(discrepancyList)+strlen(targetpath)+5);
						}
						strcat(discrepancyList,targetpath);
						strcat(discrepancyList,"\n");
					}
				}
				
				// Try updating the row based on serverid and targetfilename. If no rows, insert it
				time_t now = time(NULL);
				sql4->PrepareStatement("UPDATE dm_discovery SET deployedmd5=?, detectedmd5=?, discovery_time=? WHERE serverid=? AND targetfilename=?");
				sql4->BindParameter(1, SQL_C_CHAR,  sizeof(origmd5),    origmd5,    strlen(origmd5));
				sql4->BindParameter(2, SQL_C_CHAR,  sizeof(calcMD5),    calcMD5,    strlen(calcMD5));
				sql4->BindParameter(3, SQL_INTEGER, sizeof(now),        &now,		sizeof(now));
				sql4->BindParameter(4, SQL_INTEGER, sizeof(serverid),   &serverid,	sizeof(serverid));
				sql4->BindParameter(5, SQL_C_CHAR,  sizeof(targetpath), targetpath,	strlen(targetpath));
				sql4->Execute();
				int count = sql4->GetRowCount();
				sql4->CloseSQL();
				if (count==0) {
					// Update didn't find a row - insert
					// printf("Row Count 0 ... inserting\n");
					int id;
					// Get the new row id
					sql5->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
					int res = sql5->ExecuteSQL("SELECT max(id)+1 FROM dm_discovery");
					// printf("res=%d\n",res);
					res = sql5->FetchRow();
					// printf("FetchRow res=%d\n",res);
					if (res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO) {
						// No row, must be no enties in dm_discovery
						id = 1;
					}
					sql5->CloseSQL();
					char *ServerName = (char *)s->name();
					sql6->PrepareStatement("INSERT INTO dm_discovery(id,serverid,servername,targetfilename,deployedmd5,detectedmd5,discovery_time) VALUES(?,?,?,?,?,?,?)");
					sql6->BindParameter(1, SQL_INTEGER, sizeof(id),			&id,		sizeof(id));
					sql6->BindParameter(2, SQL_INTEGER, sizeof(serverid),   &serverid,  sizeof(serverid));
					sql6->BindParameter(3, SQL_C_CHAR,  sizeof(ServerName), ServerName, strlen(ServerName));
					sql6->BindParameter(4, SQL_C_CHAR,  sizeof(targetpath), targetpath, sizeof(targetpath));
					sql6->BindParameter(5, SQL_C_CHAR,  sizeof(origmd5),    origmd5,    strlen(origmd5));
					sql6->BindParameter(6, SQL_C_CHAR,  sizeof(calcMD5),    calcMD5,    strlen(calcMD5));
					sql6->BindParameter(7, SQL_INTEGER, sizeof(now),        &now,       sizeof(now));
					sql6->Execute();
				}
			}

			// printf("exitcode=%d\n",exitcode);
		}
		
	}
	sql1->CloseSQL();
	if (discrepancyList) {
		// Some files have been deleted or their MD5 differs from that calculated during
		// the deployment. Issue the warning
		NotifyTemplate *md5template = s->getMD5Template(ctx);
		if (md5template) {
			// md5 template is set - set the environment variables accordingly
			ctx.stack().setGlobal("SERVER_NAME",s->name());
			ctx.stack().setGlobal("SERVER_HOSTNAME",s->hostname());
			ctx.stack().setGlobal("MD5_FILELIST",discrepancyList);
			// Following must all be okay in order for us to run an MD5 check
			ctx.stack().setGlobal("NAME_RESOLUTION","OK");
			ctx.stack().setGlobal("PING","OK");
			ctx.stack().setGlobal("CONNECTION","OK");
			ctx.stack().setGlobal("BASEDIR","OK");
			ctx.stack().setGlobal("SERVER_STATUS","OK");
			Model *model = ctx.dm().getModel();
			if (model) {
				ctx.dm().setCurrentUser("admin",NULL, NULL);	// Mmmmm .... 
				ctx.dm().internalNotify(ctx,md5template);
			}
		}
	}
}

int DoDeployment(Model &model,DM &dm,const char *homeDir,char **envp,char **nvargv)
{
	setCurrentUser(dm);

	if(getenv("triappname") || getenv("triappid")
			|| getenv("trienvname") || getenv("trienvid")) {
		setTargetAppAndEnvFromParams(dm, true);
	} else {
		validateDialog(dm, model);
		const char *envname = getEnvironmentNameFromDialog(dm);
		if(!envname) {
			dm.exitWithError("Environment name must be specified");
		}
		const char *appname = getApplicationNameFromDialog(dm);
		if(!appname) {
			dm.exitWithError("Application name must be specified");
		}
		if(!dm.setTargetEnvironment(envname)) {
			dm.exitWithError("Invalid environment '%s' specified", envname);
		}
		if(!dm.setTargetApplication(appname)) {
			dm.exitWithError("Invalid application '%s' specified", appname);
		}
	}
	Environment &targetEnv = *dm.getTargetEnvironment();
	if(!targetEnv.hasAccess(dm.getCurrentUser())) {
		dm.exitWithError(
			"You do not have permission to deploy to the environment '%s'",
			targetEnv.name());
	}
	if(!targetEnv.isAvailable()) {
		dm.exitWithError(
			"Environment '%s' is not available at this time",
			targetEnv.name());
	}
	Application &targetApp = *dm.getTargetApplication();
	if(!targetApp.hasAccess(dm.getCurrentUser())) {
		dm.exitWithError(
			"You do not have permission to deploy the %s '%s'",
			targetApp.isRelease()?"release":"application",
			targetApp.name());
	}
	if(!targetApp.isAvailable(targetEnv)) {
		dm.exitWithError(
			"%s '%s' cannot be deployed into environment '%s' at this time",
			targetApp.isRelease()?"Release":"Application",
			targetApp.name(), targetEnv.name());
	}
	ensurePassphraseFile(dm, homeDir);
	// Create encrypted dialog credentials that we can pass around
	Credentials *dialogCreds = NULL;
	const char *username = getUserNameFromDialog(dm);
	const char *password = getPasswordFromDialog(dm);
	if(username || password) {
		try {
			ConstCharPtr encuser = encryptValue(username, strlen(username));
			ConstCharPtr encpass = encryptValue(password, strlen(password));
			dialogCreds = new Credentials(model,0, "DialogCredentials", CREDENTIALS_ENCRYPTED, encuser, encpass);
		} catch(DMException &e) {
			e.print(dm);
			dm.exitWithError("FATAL ERROR: Credentials encryption failed");
		}
	}
	dm.setDialogCredentials(dialogCreds);
	int ret = 0;

	
	
	try {
		dm.startAudit();	// Startup auditing system
		
		dm.writeToStdOut("INFO: Starting deployment #%d", dm.deployId());
		clock_t start = clock();
		if (dm.getRemoveApp()) {
			dm.writeToStdOut("INFO: Removing Application \"%s\" from Environment \"%s\"",targetApp.name(), targetEnv.name());
		} else {
			dm.writeToStdOut("INFO: Deploying %s \"%s\" into Environment \"%s\"",
				targetApp.isRelease()?"Release":"Application",
				targetApp.name(), targetEnv.name());
		}
		ret = runScript(dm, homeDir, nvargv, envp);
		if (dm.getRemoveApp()) {
			targetApp.recordDeployedToEnv(dm,targetEnv,false);
		}
		
		clock_t finish = clock();
		clock_t elapsed = finish - start;
		dm.writeToStdOut("INFO: Time taken %f seconds", ((float) elapsed)/CLOCKS_PER_SEC);
		
	} catch(DMException &e) {
		debug1("Unhandled DMException in main - exit code -1");
		e.print(dm);
		dm.finishAudit(-1);
		dm.deleteTemporaryFilesAndFolders();
		SAFE_DELETE(dialogCreds);
		return -1;
	} catch(...) {
		debug1("Unhandled exception in main - exit code -1");
		dm.finishAudit(-1);
		dm.deleteTemporaryFilesAndFolders();
		SAFE_DELETE(dialogCreds);
		return -1;	//throw;
	}
	debug3("Finished - exit code %d", ret);
	dm.finishAudit(ret);
	dm.deleteTemporaryFilesAndFolders();
	SAFE_DELETE(dialogCreds);
	return ret;
}

void SetEngineHostName(DM &dm,triODBC &odbc)
{
	//
	// Check if we've got a default installation - one row in dm_engine with hostname of "localhost" against domain 1.
	// if so, set this to our hostname.
	//
	int c=0;
	AutoPtr<triSQL> sql1 = odbc.GetSQL();
	sql1->BindColumn(1,SQL_INTEGER,&c,sizeof(c));
	sql1->ExecuteSQL("SELECT count(*) FROM dm_engine");
	sql1->FetchRow();
	sql1->CloseSQL();
	if (c == 1) {
		// Single row in dm_engine table - update it if the hostname is localhost and the domain is GLOBAL
		char *hostname = dm.getHostname();
		AutoPtr<triSQL> sql2 = odbc.GetSQL();
		sql2->PrepareStatement("UPDATE dm_engine SET hostname=? WHERE domainid=1");
		sql2->BindParameter(1,SQL_CHAR,strlen(hostname),hostname,strlen(hostname));
		sql2->Execute();
		sql2->CloseSQL();
		free(hostname);
	}
}


Dropzone &getScriptDropzone(Context &ctx)
{
	// Check to see if the dropzone was created in this session
	Dropzone *dz = ctx.dm().getDropzone("tdm_scripts");
	if(!dz) {
		// No, so create it, making sure it is empty
		dz = ctx.dm().newDropzone("tdm_scripts", false);
		if(!dz->create(true)) {
			// Set flag to prevent us trying to delete the non-empty folder when we exit
			putenv(strdup("trinounlink=Y"));
			throw RuntimeError(ctx.stack(), "Unable to create dropzone 'tdm_scripts'");
		}
	}
	return *dz;
}


extern "C"
#ifdef WIN32
__declspec(dllexport)
#endif /*WIN32*/

int DM_main(int argc, char **argv, char **envp)
{
//	printf(
//#ifdef WIN32
//#ifdef _WIN64
//		"WIN64"
//#else
//		"WIN32"
//#endif /*_WIN64*/
//#else
//		"UNIX"
//#endif /*WIN32*/
//		"; sizeof(short) = %d; sizeof(int) = %d; sizeof(long) = %d; sizeof(DMINT32) = %d\n",
//		sizeof(short), sizeof(int), sizeof(long), sizeof(DMINT32));
//	exit(0);
 
	try {
	int n = ScanOptions(options, sizeof(options)/sizeof(options[0]), argc, argv);

	if(getenv("tridebuglevel")) {
		setdebuglevel(atoi(getenv("tridebuglevel")));
	}

	if(getenv("trihelp")) {
		printHelpAndExit();
	}
	
	// Start of name/value and positional args
	char **nvargv = &argv[n];

	const char *triReason = getenv("TRIREASON");

	TriReason reason = triReason ? triReasonFromString(triReason) : REASON_UNSET;
	if(reason == REASON_NONE) {
		fprintf(stderr, "Not a good TRIREASON!\n");
		return 1;
	}


	// Now safe to create a DM object and set our homedir
	DM dm;
	ConstCharPtr homeDir = getHomeDirectory(argv[0]);
#ifdef DEV_VERSION
	if(getenv("tridmhome")) {
		homeDir = strdup(getenv("tridmhome"));
		struct stat sb;
		if(stat(homeDir, &sb) == -1) {
			if(errno == ENOENT) {
				// Doesn't exist - so create it
				dm.exitWithError("FATAL ERROR: Directory '%s' does not exist", (const char*) homeDir);
			} else {
				dm.exitWithError("FATAL ERROR: Unable to stat directory '%s'", (const char*) homeDir);
			}
		}
	}
#endif /*DEV_VERSION*/
	dm.setDmHome(homeDir);

	// Reset path to include bin dir for ssl libraries
#ifdef WIN32
	const char *path = getenv("PATH");
	if(path) {
		char *temp = (char*) malloc(strlen(path) + 5 + strlen(homeDir) + 10);
		sprintf(temp, "PATH=%s%sbin%s%s", homeDir, DIR_SEP_STR, PATH_SEP_STR, path);
		putenv(temp);
	} else {
		char *temp = (char*) malloc(strlen(homeDir) + 10);
		sprintf(temp, "PATH=%s%sbin", homeDir, DIR_SEP_STR);
		putenv(temp);
	}
#endif /*WIN32*/

	// Everything from here on needs a database connection
	AutoPtr<triODBC> odbc;

	try {
		if(!(odbc = connectToDatabase(dm, homeDir))) {
			dm.exitWithError("FATAL ERROR: Failed to connect to database");
		}
	} catch(DMException &e) {
		dm.exitWithError("%s\nFATAL ERROR: Failed to connect to database", e.getMessage());
	}

	SetEngineHostName(dm,*odbc);	// Default installation has engine host set to "localhost"

	

	if (getenv("triloadusers")) {
		Model model(*odbc, dm.getHostname());
		loadUsersAndExit(model,*odbc,getenv("triloadusers"));
	}

	if (getenv("triloadenvs")) {
		Model model(*odbc, dm.getHostname());
		loadEnvironmentsAndExit(model,*odbc,getenv("triloadenvs"));
	}

	if (getenv("triloadservs")) {
		Model model(*odbc, dm.getHostname());
		loadServersAndExit(model,*odbc,getenv("triloadservs"));
	}

	if (getenv("tridumpdb")) {
		dumpDBCreds(dm,(const char *)homeDir);
	}

	// If -install specified, just install plugins and exit
	if(getenv("triinstall")) {
		Model model(*odbc, dm.getHostname());
		dm.setModel(&model);
		dm.installPlugin(getenv("triinstall"));
		// repositories
		// filesystem_PluginInstall(dm);
		// svn_PluginInstall(dm);
		// dm.installPlugin("harvest");
		// dm.installPlugin("http");
		// transfers
		// rti_PluginInstall(dm);
		// notifies
		// smtpemail_PluginInstall(dm);
		// txtlocal_PluginInstall(dm);
		// datasources
#ifdef HAVE_ODBC
		//odbcdatasource_PluginInstall(dm);
#endif /*HAVE_ODBC*/
		// modifiers
		// xmlmodify_PluginInstall(dm);
		// textmodify_PluginInstall(dm);
		return 0;
	}

	const char *deplog = getenv("trideplog");
	if(deplog) {
		int logtoshow = atoi(deplog);
		if(!logtoshow) {
			dm.exitWithError("Invalid deploymentid for -showlog");
		}
		return showDeploymentLog(*odbc, logtoshow);
	}

	const char *fileToParse = NULL;
	if((fileToParse = getenv("triparse")) != NULL) {
		int ret = 0;
		try {
			ret = dm.parse(fileToParse);
			if(ret == 0) {
				printf("\"%s\" parsed ok.\n", fileToParse);
			}
		} catch(DMException &e) {
			e.print(dm);
			ret = 4;
		}
		return ret;
	}


	if(getenv("tridmscript")) {
		// Generate a script from the graphical data stored in the database
		char buf[1024];
		int actionid = atol(getenv("tridmscript"));
		Model model(*odbc, dm.getHostname());
		dm.setModel(&model);
		setCurrentUser(dm);
		dm.initialize(homeDir, nvargv, envp);
		List<Server> serverSet;
		ScopeStack stack(dm);
		stack.setGlobal("DMHOME", dm.getBaseDir());
		stack.setGlobal("TRIDM_PLATFORM", DM_PLATFORM);
		Context ctx(dm,serverSet,stack);
		Dropzone &dz = getScriptDropzone(ctx);
		Action *action = model.getAction(actionid);
		if (!action) {
			throw RuntimeError("actionid invalid or action with that id could not be found",actionid);
		}
		AutoPtr<GraphicalScriptGenerator> gen = model.createGraphicalScriptGenerator(dz, *action);
		if(!gen->generate()) {
			throw RuntimeError("Error generating script for action '%s'", action->name());
		}
		//printf("parse generated '%s'\n", (const char*) gen->filename());
		FILE *in = fopen(gen->filename(),"r");
		while (fgets(buf,sizeof(buf),in)) {
			printf(buf);
		}
		unlink(gen->filename());
		return 0;
	}

	if(getenv("triencrypt")) {
		Model model(*odbc, dm.getHostname());
		dm.setModel(&model);
		setCurrentUser(dm);
		ensurePassphraseFile(dm, homeDir);

		// Read first line of input from stdin
		char line[1024];
		if(!fgets(line, sizeof(line), stdin)) {
			dm.exitWithError("No input given");
		}

		// Trim trailing newline
		for(char *x = &line[strlen(line) - 1];
				(x > line) && ((*x == '\r') || (*x == '\n')); x--) {
			*x = '\0';
		}

		//dumpbuffer(line, strlen(line));
		
		try {
			ConstCharPtr encLine = encryptValue(line, strlen(line));
			printf("%s\n", (const char*) encLine);
		} catch(DMException &e) {
			e.print(dm);
			dm.exitWithError("Error during encryption");
		}

		return 0;
	}

	const char *providerTest = NULL;
	if((providerTest = getenv("triprovidertest")) != NULL) {
		OBJECT_KIND kind = OBJ_KIND_NONE;
		if(strcmp(providerTest, "repository") == 0) {
			kind = OBJ_KIND_REPOSITORY;
		} else if(strcmp(providerTest, "notify") == 0) {
			kind = OBJ_KIND_NOTIFY;
		} else if(strcmp(providerTest, "datasource") == 0) {
			kind = OBJ_KIND_DATASOURCE;
		} else {
			dm.exitWithError("Unrecognised provider type '%s'", providerTest);
		}
		int ret = -1;
		if(nvargv && nvargv[0]) {
			int id = atoi(nvargv[0]);
			if(id > 0) {
#ifdef DEV_VERSION
				if(getenv("tridmhome")) {
					debug1("DMHOME = '%s'", (const char*) homeDir);
				}
#endif /*DEV_VERSION*/
				Model model(*odbc, dm.getHostname());
				dm.setModel(&model);
				setCurrentUser(dm);
				dm.initialize(homeDir, nvargv, envp);
				if (argc-n > 1) {
					ret = dm.providerTest(kind, id, nvargv[1]);
				} else {
					ret = dm.providerTest(kind, id);
				}
				dm.deleteTemporaryFilesAndFolders();
			} else {
				dm.exitWithError("Bad provider id '%s'", nvargv[0]);
			}
		} else {
			dm.exitWithError("No provider id specified");
		}
		return ret;
	}

	const char *notifier = NULL;
	if((notifier = getenv("trinotify")) != NULL) {
		int ret = -1;
		int nfyid = atoi(notifier);
		if(nfyid > 0) {
			const char *notifyFrom = getenv("trifrom");
			if(!notifyFrom) {
				notifyFrom = "re@openmakesoftware.com";
			}
			const char *notifySubject = getenv("trisubject");
			if(!notifySubject) {
				 notifySubject = "Notification from Release Engineer";
			}
			int templateid = 0;
			const char *notifyTemplate = getenv("tritemplate");
			if(notifyTemplate) {
				templateid = atoi(notifyTemplate);
				if(templateid == 0) {
					dm.exitWithError("Bad template id '%s'", notifyTemplate);
				}
			}
			if(nvargv && nvargv[0]) {
#ifdef DEV_VERSION
				if(getenv("tridmhome")) {
					debug1("DMHOME = '%s'", (const char*) homeDir);
				}
#endif /*DEV_VERSION*/
				Model model(*odbc, dm.getHostname());
				dm.setModel(&model);
				setCurrentUser(dm);
				setTargetAppAndEnvFromParams(dm, false);
				dm.initialize(homeDir, nvargv, envp);
				ret = dm.doNotify(nfyid, notifyFrom, notifySubject, templateid);
				dm.deleteTemporaryFilesAndFolders();
			} else {
				dm.exitWithError("No recipients specified");
			}
		} else {
			dm.exitWithError("Bad notifier id '%s'", notifier);
		}
		return ret;
	}

	const char *runtask = NULL;
	if((runtask = getenv("triruntask")) != NULL) {
		int ret = -1;
		int taskid = atoi(runtask);
		if(taskid > 0) {
			Model model(*odbc, dm.getHostname());
			dm.setModel(&model);
			setCurrentUser(dm);
			setTargetAppAndEnvFromParams(dm, false);
			ret = runTask(dm, taskid, homeDir, nvargv, envp);
			dm.deleteTemporaryFilesAndFolders();
		} else {
			dm.exitWithError("Bad task id '%s'", runtask);
		}
		return ret;
	}

	switch(reason)
	{
	case REASON_UNSET:
#if 0
		// TODO: Remove this - it is here so we can run from debugger
		// Have we been invoked with a web-interface deployment flag
		if(!getenv("triappname") && !getenv("triappid")
				&& !getenv("trienvname") && !getenv("trienvid")) {
			// Run script with some test data set - -usr toggles between demo and testsuite
			const char *user = getenv("TRIDM_USER");
			if(user && (strcmp(user, "demo") == 0)) {
				putenv(strdup("TRIFIELD1=Demo Environment (SSH)"));
				putenv(strdup("TRIFIELD2="));
				putenv(strdup("TRIFIELD3="));
				//putenv(strdup("TRIFIELD4=DMDemo AppVer 2.0"));
				putenv(strdup("TRIFIELD4=DMDemo P4 Components"));

				nvargv = (char**) malloc(5 * sizeof(char*));
				nvargv[0] = (char*) "cmdln_haruser=harvest";
				nvargv[1] = (char*) "cmdln_harpass=harvest";
				nvargv[2] = (char*) "cmdln_harproject=DMDemo";
				nvargv[3] = (char*) "cmdln_harstate=Test";
				//nvargv[4] = (char*) "cmdln_harpackages='DMDemo-2.0'";
				//nvargv[5] = NULL;
				//nvargv[4] = (char*) "DMDemo-1.0";
				nvargv[4] = (char*) "DMDemo-2.0";
				nvargv[5] = NULL;
			} else {
				putenv(strdup("TRIFIELD1=envA"));
				putenv(strdup("TRIFIELD2=harvest"));
				putenv(strdup("TRIFIELD3=harvest"));
				putenv(strdup("TRIFIELD4=app2"));
				//putenv(strdup("TRIFIELD4=CreditScore"));
				nvargv = (char**) malloc(3 * sizeof(char*));
				nvargv[0] = (char*) "cmdln_haruser=harvest";
				nvargv[1] = (char*) "cmdln_harpass=harvest";
				nvargv[2] = NULL;
			}

			if(!user) {
				putenv(strdup("TRIDM_USER=admin"));
			}
		}
		// fall thru...
#endif /*0*/
	case REASON_SCRIPT:
	case REASON_TIMED: {
#ifdef DEV_VERSION
		if(getenv("tridmhome")) {
			debug1("DMHOME = '%s'", (const char*) homeDir);
		}
#endif /*DEV_VERSION*/
		if(getenv("tritestmode")) {
			return testThings(*odbc);
		}

		/*
		 * PAG MOD - remove license key check here. Licensing will be done through the
		 * web UI
		License lic;
		lic.OpenLicenseFile(homeDir, "dm.lic");
		LicenseError licerror = lic.LoadLicenseFile();
		if(licerror == CouldNotOpenLicenseFile) {
			dm.exitWithError("Could not find or open license file \"%s\"", "dm.lic");
		} else if(licerror == CorruptLicenseFile) {
			dm.exitWithError("Invalid or corrupt license file \"%s\"", "dm.lic");
		}
		if(lic.CheckForExpiredLicense()) {
			dm.exitWithError("License key has expired.\n"
				"Please contact OpenMake Software for a new license key");
		}
		if(lic.CheckForInvalidHostName()) {
			dm.exitWithError("License key is not valid for this host.\n"
				"Please contact OpenMake Software for a new license key");
		}
		*/

		Model model(*odbc, dm.getHostname());
		dm.setModel(&model);

		if (reason == REASON_TIMED) {
			// If we've been invoked on the timer, we need to find any jobs that
			// are scheduled to run now. The "user" should be set to the owner
			// of that job.
			printf("Start timer activity\n");
			List<TimedJob> *a = model.getTimedJobs();
			ListIterator<TimedJob> iter(*a);
			for (TimedJob *t=iter.first();t;t=iter.next()) {
				printf("Event ID %d\n",t->getEventId());
				char appstr[128];
				char envstr[128];
				char usrstr[300];
				sprintf(appstr,"triappid=%d",t->getAppId());
				sprintf(envstr,"trienvid=%d",t->getEnvId());
				User *user = model.getUserById(t->getUsrId());
				sprintf(usrstr,"TRIDM_USER=%s",user->name());
				putenv(appstr);
				putenv(envstr);
				putenv(usrstr);
				dm.setEventId(t->getEventId());
				DoDeployment(model,dm,(const char *)homeDir,envp,nvargv);
			}
			// Check for servers to be pinged/checked periodically
			printf("Checking servers\n");
			dm.initialize(homeDir,nvargv,envp);
			List <Server> *sl = model.getServersToCheck();
			ListIterator<Server> iter2(*sl);
			printf("sl=%d\n",sl->size());
			for (Server *s=iter2.first();s;s=iter2.next()) {
				printf("Checking server id %d\n",s->id());
				int status = CheckServerConnectivity(model,dm,*odbc,s->id());
				printf("Status=0x%lx\n",status);
				if (status & CHECK_CONNECTION_OKAY) {
					//
					// We connected okay to this server. If the server has received
					// deployments and we need to check the MD5 checksums then do
					// this now.
					//
					if (model.checkMD5(s->id())) {
						ScanServer(model,dm,*odbc,s->id());
					}
				}
			}
			dm.cleanup();	// cleanup here as DoDeployment will not do this if on timer.
			printf("end of timer activiity\n");
		} else {
			//
			// Invoked as a user-invoked deployment
			if (getenv("triscanserver")) {
				// Scan server and calculate MD5 for all deployed applications
				int serverid = atol(getenv("triscanserver"));
				ScanServer(model,dm,*odbc,serverid);
			} else
			if (getenv("tricheckserver")) {
				// Check connectivity to server
				int serverid = atol(getenv("tricheckserver"));
				dm.initialize(homeDir,nvargv,envp);
				// dm.writeToLogFile("After dm.initialize, stack is:\n");
				CheckServerConnectivity(model,dm,*odbc,serverid);
			} else
			if (getenv("tridumpscript")) {
				// Dump out the script content specified by the given id
				int actionid = atol(getenv("tridumpscript"));
				dm.initialize(homeDir,nvargv,envp);
				dm.DumpScript(actionid);
			} else 
			if (getenv("triimpscript")) {
				dm.initialize(homeDir,nvargv,envp);
				dm.importScript();
			} else {
				DoDeployment(model,dm,(const char *)homeDir,envp,nvargv);
			}
		}
		return 0; // TIMED or SCRIPT
		}

	case REASON_VALIDATION: {
		Model model(*odbc, dm.getHostname());
		dm.setModel(&model);
		dm.getDummyAudit();
		return validateDialog(dm, model);
		}

#if 0
	case REASON_UNSET:
		// Run script with some test data set
		putenv(strdup("trifields=environments,username,password,applications,combo1,combo2,combo3"));
		//putenv(strdup("TRICURRENTFIELD=1"));
		putenv(strdup("TRICURRENTFIELD=7"));
		putenv(strdup("TRIFIELD1=envA"));
		putenv(strdup("TRIDM_USER=robert"));
		// fall thru...
#endif /*0*/

	case REASON_POPULATE: {
		Model model(*odbc, dm.getHostname());
		dm.setModel(&model);
		dm.getDummyAudit();
		setCurrentUser(dm);
		dm.initialize(homeDir, nvargv, envp);
		int res = handleFieldPopulation(dm, model);
		dm.cleanup();
		dm.deleteTemporaryFilesAndFolders();
		return res;
		}

#if 0
	case REASON_UNSET:
		putenv(strdup("TRIDM_USER=robert"));
		// fall thru...
#endif /*0*/

	case REASON_LISTBOX: {
		Model model(*odbc, dm.getHostname());
		dm.setModel(&model);
		dm.getDummyAudit();
		setCurrentUser(dm);
		ensurePassphraseFile(dm, homeDir);
		int res = runListboxScript(dm, homeDir, nvargv, envp);
		dm.cleanup();
		dm.deleteTemporaryFilesAndFolders();
		return res;
		}

#ifdef HAVE_TRILOGYAPI
	case REASON_PRELOAD: {
		dm.writeToLogFile("PRELOAD");
		OnPopulateAnyField(PreloadPopulateField);
		int ret = WaitForTrilogy(60, argc, argv);
		dm.writeToLogFile("EXITING - ret = %d", ret);
		return 0;
		}
		break;
#endif /*HAVE_TRILOGYAPI*/
	}

	} catch(DMException &e) {
		fprintf(stderr, "Unhandled exception: %s\n", e.getMessage());
		// fprintf(stdout, "Unhandled exception: %s\n", e.getMessage());
		return 1;
	} catch(...) {
		fprintf(stderr, "Unknown exception\n");
		// fprintf(stdout, "Unknown exception\n");
		return 1;
	}
	return 0;
}


#ifdef WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}
#endif /*WIN32*/
