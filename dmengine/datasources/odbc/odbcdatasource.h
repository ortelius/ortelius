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
