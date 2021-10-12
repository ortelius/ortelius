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
