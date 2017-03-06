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
#ifndef __credentials_h

#define __credentials_h


#include "hashtable.h"
#include "dmapiexport.h"


class DMAPI_API CredentialsProvider
{
protected:
	class CredentialsProviderFactory &m_factory;
	class Credentials &m_credentials;

	CredentialsProvider(class CredentialsProviderFactory &factory, Credentials &credentials);

public:
	virtual ~CredentialsProvider();

	virtual bool canTransform() = 0;

	virtual const char *transform(class Object &auth, class Context &ctx) = 0;
};


class DMAPI_API CredentialsProviderFactory
{
public:
	CredentialsProviderFactory();
	virtual ~CredentialsProviderFactory();

	virtual CredentialsProvider *create(class Credentials &credentials) = 0;
};


class DMAPI_API CredentialsProviderRegistry
{
private:
	Hashtable<CredentialsProviderFactory> m_factories;

	static CredentialsProviderRegistry *s_instance;

public:
	CredentialsProviderRegistry();

	static CredentialsProviderRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, CredentialsProviderFactory *factory);
	void registerBuiltIns(class DM &dm);

	CredentialsProviderFactory *getFactory(const char *provider);
};




#endif /*__credentials_h*/
