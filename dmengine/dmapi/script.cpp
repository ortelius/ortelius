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
//#include <stdlib.h>
//#include <string.h>
//#include <sys/stat.h>

#include "script.h"
#include "model.h"
#include "cmdline.h"
#include "exceptions.h"
//#include "expr.h"
//#include "node.h"
#include "context.h"
#include "dm.h"
#include "properties.h"
#include "execute.h"
#include "scopestack.h"


class RemoteScriptImpl : public virtual ScriptProviderImpl
{
private:
	Property *m_url;

public:
	RemoteScriptImpl(ScriptProviderImplFactory &factory, Script &script, Property *url);

	void run(class ExtendedStmt &stmt, class Context &ctx);
};


class RemoteScriptImplFactory : public virtual ScriptProviderImplFactory
{
public:
	RemoteScriptImplFactory();

	ScriptProviderImpl *create(class Script &parent);
};


///////////////////////////////////////////////////////////////////////////////
// ScriptProviderImpl
///////////////////////////////////////////////////////////////////////////////

ScriptProviderImpl::ScriptProviderImpl(ScriptProviderImplFactory &factory, Script &script)
	: m_factory(factory), m_script(script)
{}


/*virtual*/ ScriptProviderImpl::~ScriptProviderImpl()
{}


///////////////////////////////////////////////////////////////////////////////
// ScriptProviderImplFactory
///////////////////////////////////////////////////////////////////////////////

ScriptProviderImplFactory::ScriptProviderImplFactory()
{}


/*virtual*/ ScriptProviderImplFactory::~ScriptProviderImplFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// ScriptProviderImplRegistry
///////////////////////////////////////////////////////////////////////////////

ScriptProviderImplRegistry *ScriptProviderImplRegistry::s_instance = NULL;


ScriptProviderImplRegistry::ScriptProviderImplRegistry()
	: m_factories(false, true)
{}


/*static*/ ScriptProviderImplRegistry &ScriptProviderImplRegistry::instance()
{
	if(!s_instance) {
		s_instance = new ScriptProviderImplRegistry();
	}
	return *s_instance;
}


/*static*/ void ScriptProviderImplRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void ScriptProviderImplRegistry::registerFactory(const char *name, ScriptProviderImplFactory *factory)
{
	m_factories.put(name, factory);
}


void ScriptProviderImplRegistry::registerBuiltIns()
{
	registerFactory("remote", new RemoteScriptImplFactory());
}


ScriptProviderImplFactory *ScriptProviderImplRegistry::getFactory(ProviderObjectDef &def)
{
	return m_factories.get(def.name());
}


///////////////////////////////////////////////////////////////////////////////
// RemoteScriptImpl
///////////////////////////////////////////////////////////////////////////////

RemoteScriptImpl::RemoteScriptImpl(
		ScriptProviderImplFactory &factory, Script &script, Property *url
	)
	: ScriptProviderImpl(factory, script), m_url(url)
{}


void RemoteScriptImpl::run(ExtendedStmt &stmt, Context &ctx)
{
	Environment *targetEnv = ctx.dm().getTargetEnvironment();
	if(!targetEnv) {
		throw RuntimeError("Target environment not set - unable to deploy");
	}

	ListIterator<Server> iter(ctx.servers());
	for(Server *s = iter.first(); s; s = iter.next()) {

		printf("DEBUG: Getting Server '%s' credentials\n", s->name());
		Credentials *creds = s->getCredentials();
		if(!creds) {
			printf("DEBUG: Getting Environment '%s' credentials\n", targetEnv->name());
			creds = targetEnv->getCredentials();
		}
		if(!creds) {
			printf("DEBUG: Getting dialog credentials\n");
			creds = ctx.dm().getDialogCredentials();
		}
		if(!creds) {
			// No credentials for server - abort
			//throw RuntimeError("No credentials for Server '%s' - unable to deploy", s->name());
			printf("DEBUG: SKIPPING SERVER '%s'\n", s->name());
			continue;
		}
		printf("DEBUG: Credentials: "); creds->print(0); printf("\n");

		// push the server onto the stack (if we are not in a psloop)
		bool dopush = !ctx.stack().contains(PHYSICAL_SCOPE);

		if(dopush) {
			ctx.stack().push(s->getVars());
		}

		// TODO: Change this to the real program we will use
#ifdef WIN32
		CmdLine cmd("C:\\Shared\\RTO_Server\\Windows\\Release\\rtoexecp2.exe");
#else
		CmdLine cmd("/mnt/hgfs/Shared/RTO_Server/Linux/release/lib/rtoexecp2");
#endif /*WIN32*/

		cmd.add("-m").add("linux-gnh2" /*s->hostname()*/).add("-syn").add("-force")
		   .add("-usr").add("harvest").add("-pw").add("harvest")
		   .add("ls");

		printf("%s\n", cmd.toCommandString(false));

		bool bShowOutput = true;

		CapturedData *cd = NULL;
		int tempExitStatus;
		int ret = executeAndCapture(ctx.stream(), cmd, bShowOutput, &tempExitStatus, &cd, NULL);
		if(ret) {
			ctx.dm().writeToStdOut("script failed to execute");
		}

		if(dopush) {
			ctx.stack().pop(PHYSICAL_SCOPE);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// RemoteScriptImplFactory
///////////////////////////////////////////////////////////////////////////////

RemoteScriptImplFactory::RemoteScriptImplFactory()
{
	//List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	//propdefs->add(new PropertyDef("url", true, false));

	//m_checker = new PropertyChecker(propdefs, "svn repository");
}


ScriptProviderImpl *RemoteScriptImplFactory::create(Script &parent)
{
	Property *url = parent.getProperty("url");

	return new RemoteScriptImpl(*this, parent, url);
}
