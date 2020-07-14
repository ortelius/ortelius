#ifndef __datasources_odbc_h

#define __datasources_odbc_h


#include "../../dmapi/model.h"
#include "../../dmapi/datasource.h"


#ifdef HAVE_ODBC
class OdbcDatasourceImpl : public virtual DatasourceProviderImpl
{
private:
	char *m_dsn;
	class Property *m_username;
	class Property *m_password;

public:
	OdbcDatasourceImpl(
		DatasourceProviderImplFactory &factory, DMINT32 implId, Datasource &datasource,
		const char *dsn, class Property *username, class Property *password);

	virtual ~OdbcDatasourceImpl();

	bool testProvider(class Context &ctx);

	void query(class Model &model, class ExtendedStmt &stmt, class Context &ctx);

	class Expr *getAttribute(const char *name, class Context &ctx);
};

class LdapDatasourceImpl : public virtual DatasourceProviderImpl
{
private:
	char *m_ldapserver;
	int m_port;
	class Property *m_username;
	class Property *m_password;

	LDAP *ConnectToLDAP(class Context &ctx,class ExtendedStmt *stmt);

public:
	LdapDatasourceImpl(
		DatasourceProviderImplFactory &factory, DMINT32 implId, Datasource &datasource,
		const char *ldapServer, const char *ldapPort, class Property *username, class Property *password);

	virtual ~LdapDatasourceImpl();

	bool testProvider(class Context &ctx);

	void query(class Model &model, class ExtendedStmt &stmt, class Context &ctx);

	class Expr *getAttribute(const char *name, class Context &ctx);
};


class OdbcDatasourceImplFactory : public virtual DatasourceProviderImplFactory
{
public:
	OdbcDatasourceImplFactory();

	DatasourceProviderImpl *create(
		DMINT32 implId, class Datasource &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};

class LdapDatasourceImplFactory : public virtual DatasourceProviderImplFactory
{
public:
	LdapDatasourceImplFactory();

	DatasourceProviderImpl *create(
		DMINT32 implId, class Datasource &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};
#endif /*HAVE_ODBC*/


#endif /*__datasources_odbc_h*/
