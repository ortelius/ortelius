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
