#ifndef __datasource_h

#define __datasource_h


#include "hashtable.h"
#include "model.h"


class DatasourceProviderImpl : public ProviderObjectImpl
{
protected:
	class DatasourceProviderImplFactory &m_factory;
	class Datasource &m_datasource;

	DatasourceProviderImpl(
		class DatasourceProviderImplFactory &factory, DMINT32 implId,
		class Datasource &datasource);

public:
	virtual ~DatasourceProviderImpl();

	virtual void query(class Model &model,class ExtendedStmt &stmt, class Context &ctx) = 0;

	// ProviderObjectImpl
	OBJECT_KIND kind()  { return OBJ_KIND_DATASOURCE_IMPL; }
	virtual class Expr *getAttribute(const char *name, class Context &ctx);
	//virtual class DMArray *getArrayAttribute(const char *name, class Context &ctx)  { return NULL; }
	//virtual class Object *getObjectAttribute(const char *name, class Context &ctx)  { return NULL; }
	class Object *toObject() { return NULL; }
};


class DatasourceProviderImplFactory
{
public:
	DatasourceProviderImplFactory();
	virtual ~DatasourceProviderImplFactory();

	virtual DatasourceProviderImpl *create(
		DMINT32 implId, class Datasource &parent, class ExtendedStmt &stmt,
		class Context &ctx) = 0;
};


class DatasourceProviderImplRegistry
{
private:
	Hashtable<DatasourceProviderImplFactory> m_factories;

	static DatasourceProviderImplRegistry *s_instance;

public:
	DatasourceProviderImplRegistry();

	static DatasourceProviderImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, DatasourceProviderImplFactory *factory);
	void registerBuiltIns(class DM &dm);

	DatasourceProviderImplFactory *getFactory(class ProviderObjectDef &def);
};


#endif /*__datasource_h*/
