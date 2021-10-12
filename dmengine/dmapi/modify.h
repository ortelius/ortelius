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

#ifndef __modify_h

#define __modify_h


#include "hashtable.h"
#include "extended.h"


class DMAPI_API ModifyProviderImpl
{
protected:
	class ExtendedStmt &m_parent;

public:
	ModifyProviderImpl(class ExtendedStmt &parent);
	virtual ~ModifyProviderImpl();

	virtual void loadFile(const char *infile, class Context &ctx) = 0; 
	virtual void saveFile(const char *outfile, class Context &ctx) = 0;

	virtual void executeSubStmt(class ExtendedStmt &stmt, class Context &ctx) = 0;
};


class DMAPI_API ModifyProviderImplFactory
{
public:
	virtual ~ModifyProviderImplFactory();

	virtual ModifyProviderImpl *create(
		class ExtendedStmt &parent) = 0;
};


class DMAPI_API ModifyProviderImplRegistry
{
private:
	Hashtable<ModifyProviderImplFactory> m_factories;

	static ModifyProviderImplRegistry *s_instance;

public:
	ModifyProviderImplRegistry();

	static ModifyProviderImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, ModifyProviderImplFactory *factory);
	void registerBuiltIns(class DM &dm);

	ModifyProviderImplFactory *getFactory(const char *name);
};


class DMAPI_API ModifyProviderStmtImpl : public ExtendedStmtImpl
{
protected:
	ExtendedStmt &m_parent;

public:
	ModifyProviderStmtImpl(class ExtendedStmt &parent);
	virtual ~ModifyProviderStmtImpl();

	void execute(class Context &ctx); 
};


class DMAPI_API ModifyProviderStmtImplFactory : public ExtendedStmtImplFactory
{
public:
	virtual ~ModifyProviderStmtImplFactory();

	bool allowsBody()     { return false; }
	bool allowsPrePost()  { return false; }
	bool isThreaded()     { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


#endif /*__modify_h*/
