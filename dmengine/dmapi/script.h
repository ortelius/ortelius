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
#ifndef __script_h

#define __script_h


#include "hashtable.h"
#include "dmapiexport.h"


class DMAPI_API ScriptProviderImpl
{
protected:
	class ScriptProviderImplFactory &m_factory;
	class Script &m_script;

	ScriptProviderImpl(class ScriptProviderImplFactory &factory, Script &script);

public:
	virtual ~ScriptProviderImpl();

	virtual void run(class ExtendedStmt &stmt, class Context &ctx) = 0;
};


class DMAPI_API ScriptProviderImplFactory
{
public:
	ScriptProviderImplFactory();
	virtual ~ScriptProviderImplFactory();

	virtual ScriptProviderImpl *create(class Script &parent) = 0;
};


class DMAPI_API ScriptProviderImplRegistry
{
private:
	Hashtable<ScriptProviderImplFactory> m_factories;

	static ScriptProviderImplRegistry *s_instance;

public:
	ScriptProviderImplRegistry();

	static ScriptProviderImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, ScriptProviderImplFactory *factory);
	void registerBuiltIns();

	ScriptProviderImplFactory *getFactory(class ProviderObjectDef &def);
};


#endif /*__script_h*/
