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
