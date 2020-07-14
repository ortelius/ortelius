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
