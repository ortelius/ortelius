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
