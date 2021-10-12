// Copyright (c) 2021 Linux Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>

#include "credentials.h"
#include "model.h"


///////////////////////////////////////////////////////////////////////////////
// CredentialsProvider
///////////////////////////////////////////////////////////////////////////////

CredentialsProvider::CredentialsProvider(
		CredentialsProviderFactory &factory, Credentials &credentials
	)
	: m_factory(factory), m_credentials(credentials)
{}


/*virtual*/ CredentialsProvider::~CredentialsProvider()
{}


///////////////////////////////////////////////////////////////////////////////
// CredentialsProviderFactory
///////////////////////////////////////////////////////////////////////////////

CredentialsProviderFactory::CredentialsProviderFactory()
{}


/*virtual*/ CredentialsProviderFactory::~CredentialsProviderFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// CredentialsProviderRegistry
///////////////////////////////////////////////////////////////////////////////

CredentialsProviderRegistry *CredentialsProviderRegistry::s_instance = NULL;


CredentialsProviderRegistry::CredentialsProviderRegistry()
	: m_factories(false, true)
{}


/*static*/ CredentialsProviderRegistry &CredentialsProviderRegistry::instance()
{
	if(!s_instance) {
		s_instance = new CredentialsProviderRegistry();
	}
	return *s_instance;
}


/*static*/ void CredentialsProviderRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void CredentialsProviderRegistry::registerFactory(const char *name, CredentialsProviderFactory *factory)
{
	m_factories.put(name, factory);
}

void CredentialsProviderRegistry::registerBuiltIns(DM &dm)
{}


CredentialsProviderFactory *CredentialsProviderRegistry::getFactory(const char *provider)
{
	return m_factories.get(provider);
}
