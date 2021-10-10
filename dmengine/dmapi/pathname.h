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

#ifndef __pathname_h

#define __pathname_h


#include "hashtable.h"


class PathNameImpl
{
public:
	virtual ~PathNameImpl();

	virtual const char *path() = 0;
	virtual const char *stdpath() = 0;

	virtual bool isAbsolute() = 0;
	virtual PathNameImpl *basename() = 0;
	virtual PathNameImpl *dirname() = 0;
	virtual PathNameImpl *parent() = 0;
	virtual PathNameImpl *append(const char *path) = 0;
	virtual PathNameImpl *appendstd(const char *stdpath) = 0;
};


class PathNameImplFactory
{
public:
	virtual ~PathNameImplFactory();

	virtual PathNameImpl *create(const char *path) = 0;
};


class PathNameImplRegistry
{
private:
	Hashtable<PathNameImplFactory> m_factories;

	static PathNameImplRegistry *s_instance;

public:
	PathNameImplRegistry();

	static PathNameImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, PathNameImplFactory *factory);
	void registerBuiltIns();

	PathNameImplFactory *getFactory(const char *name);
};


#endif /*__pathname_h*/
