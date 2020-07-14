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
