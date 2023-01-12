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

#ifdef WIN32
#include "windows.h"
#define ODBCDATASOURCE_EXPORT __declspec(dllexport)
#else
#define ODBCDATASOURCE_EXPORT
#endif /*WIN32*/

#include <stdio.h>

#ifdef WIN32
#include <winldap.h>
#else
#include <ldap.h>
typedef char *PCHAR;
#endif

#include "odbcdatasource.h"

#include "../../dmapi/datasource.h"
#include "../../dmapi/charptr.h"
#include "../../dmapi/node.h"
//#include "../../dmapi/model.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/context.h"
//#include "../../dmapi/scopestack.h"
#include "../../dmapi/properties.h"
//#include "../../dmapi/triodbc.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/expr.h"



#define ODBCDATASOURCE_PLUGIN_VERSION "1.00"

#ifdef HAVE_ODBC
///////////////////////////////////////////////////////////////////////////////
// OdbcDatasourceImpl
///////////////////////////////////////////////////////////////////////////////

OdbcDatasourceImpl::OdbcDatasourceImpl(
		DatasourceProviderImplFactory &factory, DMINT32 implId,
		Datasource &datasource, const char *dsn,
		Property *username, Property *password
	)
	: DatasourceProviderImpl(factory, implId, datasource), m_dsn(DUP_NULL(dsn)),
	  m_username(username), m_password(password)
{
	debug1("OdbcDatasourceImpl('%s')", NULL_CHECK(m_dsn));
}


OdbcDatasourceImpl::~OdbcDatasourceImpl()
{
	SAFE_FREE(m_dsn);
}


void replace_element(char *res, const char *seq, int val)
{
	char *p = strstr(res, seq);
	if(p) {
		if(strlen(seq) == 2) {
			p[0] = (val / 10) + '0';
			p[1] = (val % 10) + '0';
		} else if(strlen(seq) == 4) {
			p[0] = (val / 1000) + '0';
			p[1] = ((val % 1000)/100) + '0';
			p[2] = ((val % 100)/10) + '0';
			p[3] = (val % 10) + '0';
		}
	}
}

char *decodetime(SQL_TIMESTAMP_STRUCT c, char *buf, int buflen)
{
	const char *format = getenv("TRIDISPDATEFORMAT");
	if(!format) format = "DD/MM/YYYY HH:MI:SS";
	strncpy(buf, format, buflen);
	buf[buflen-1] = '\0';
	replace_element(buf, "DD", c.day);
	replace_element(buf, "MM", c.month);
	replace_element(buf, "YYYY", c.year);
	replace_element(buf, "YY", (c.year % 100));
	replace_element(buf, "HH", c.hour);
	replace_element(buf, "MI", c.minute);
	replace_element(buf, "SS", c.second);
	return buf;
}




/**
 * Very simple test to make sure that the repository is correctly configured
 * with all the mandatory properties.  Just tests to make sure that a dsn has
 * been specified.
 */
bool OdbcDatasourceImpl::testProvider(class Context &ctx)
{
	if(!m_dsn) {
		ctx.writeToStdErr("\"data source name\" (dsn) property must be specified");
		return false;
	}
	triODBC odbc;
	char *user = (char *)0;
	char *pass = (char *)0;
	Credentials *creds = m_datasource.getCredentials();
	if (creds) {
		user = creds->getDecryptedUsername(ctx);
		pass = creds->getDecryptedPassword(ctx);
	} else {
		user = strdup("");
		pass = strdup("");
	}
	SQLRETURN res = odbc.ConnectToDataSource(&(ctx.dm()),m_dsn, user, pass);
	SECURE_FREE(user);
	SECURE_FREE(pass);
	if (res == SQL_ERROR) {
		ctx.writeToStdErr("Failed to connect to datasource '%s'", m_dsn);
		return false;
	} else {
		ctx.writeToStdOut("Connection to datasource ok\n");
	}

	return true;
}


void OdbcDatasourceImpl::query(class Model &model, class ExtendedStmt &stmt, class Context &ctx)
{
	Credentials *creds = m_datasource.getCredentials();
	AutoPtr<Credentials> cap;
	if(!creds) {
		// Create a credentials object from the username and password properties
		creds = new Credentials(model,m_username, m_password);
		cap = creds; // Delete on return
	}

	ConstCharPtr sql = stmt.getArgAsString("sql", ctx);
	if(!sql) {
		throw RuntimeError(stmt, ctx.stack(),
				"query failed - sql statement must be specified");
	}

	ConstCharPtr result = stmt.getArgAsString("result", ctx);
	if(!result) {
		throw RuntimeError(stmt, ctx.stack(),
				"query failed - result array must be specified");
	}

	triODBC odbc;
	char *msg = NULL;
	SQLINTEGER err = 0;

	char *user = creds->getDecryptedUsername(ctx);
	char *pass = creds->getDecryptedPassword(ctx);
	SQLRETURN res = odbc.ConnectToDataSource(&(ctx.dm()),m_dsn, user, pass);
	SECURE_FREE(user);
	SECURE_FREE(pass);
	if(res == SQL_ERROR) {
		odbc.GetLastError(&msg,&err);
		throw RuntimeError(stmt, ctx.stack(),"Failed to connect to datasource%s%s",msg?": ":".",msg?msg:"");
	}

	AutoPtr<triSQL> tsql = odbc.GetSQL();

	res = tsql->PrepareStatement((const char*) sql);
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		tsql->GetLastError(&msg, &err);
		throw RuntimeError(stmt, ctx.stack(), "%s (%d)", msg, err);
	}

	AutoPtr<COLDATA> cols = tsql->GetColumnInfoForStatement();
	if(cols) {
		debug1("Looks like a SELECT");
		if(!m_datasource.hasReadAccess(ctx.dm().getCurrentUser())) {
			tsql->CloseSQL();
			throw PermissionDeniedException(ctx.stack(),
				"No read access to datasource '%s'", m_datasource.name());
		}
	} else {
		//
		// We may not have any columns because of an error in the
		// SELECT query. So test for that here. If no error, then
		// it's a SELECT or UPDATE.
		//
		tsql->GetLastError(&msg,&err);
		if (err!=0) {
			// Remove odd additional text in case we're talking to a PostreSQL database
			char *p = strstr(msg,";\n");
			if (p) *p='\0';
			throw RuntimeError(stmt, ctx.stack(),"query failed: %s",msg);
		}
		debug1("Looks like a DELETE or UPDATE");
		if(!m_datasource.hasWriteAccess(ctx.dm().getCurrentUser())) {
			tsql->CloseSQL();
			throw PermissionDeniedException(ctx.stack(),
				"No write access to datasource '%s'", m_datasource.name());
		}
	}

	res = tsql->Execute();	//ExecuteSQL((const char*) sql);
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		char *msg = NULL;
		SQLINTEGER err = 0;
		tsql->GetLastError(&msg, &err);
		throw RuntimeError(stmt, ctx.stack(), "%s (%d)", msg, err);
	}

	DMArray *ht = ctx.stack().newLocalArray(result, false, true);	// Case-insensitive so column names can be upper or lowercase

	////AutoPtr<COLDATA> cols = tsql->GetColumnInfoForStatement();
	int n=0;
	// Setup dynamic binding
	for(COLDATA *x = cols; x; x = x->next) {
		n++;
		x->allocDataPtr();
//printf("column %d, name '%s', bindtype %d, colsize %d\n", n, x->ColumnName, x->BindType, x->ColumnSize);
		tsql->BindColumn(n, x->BindType, x->DataPtr, x->ColumnSize, &(x->NullInd));
	}

	char buf[256];

	// Bindings done, pull the data
	res = (SQLRETURN) tsql->FetchRow();
	while(res != SQL_NO_DATA) {
		ht->clear();

		for(COLDATA *x = cols; x; x = x->next) {
			if(x->NullInd == -1) {
				ht->put(x->ColumnName, NULL);
			} else {
				switch(x->BindType)
				{
				case SQL_VARCHAR:
				case SQL_CHAR:
					ht->put(x->ColumnName, new Variable(NULL, (char*) x->DataPtr));
					break;
				case SQL_INTEGER:
				case -16:	// RHT 18/04/2010 - Fix for unknown bindtype
					sprintf(buf, "%d", *((long*) x->DataPtr));
					ht->put(x->ColumnName, new Variable(NULL, buf));
					break;
				case SQL_DOUBLE:
					sprintf(buf, "%ld", *((long*) x->DataPtr));
					ht->put(x->ColumnName, new Variable(NULL, buf));
					break;
				case SQL_SMALLINT:
					sprintf(buf, "%hd", *((short*) x->DataPtr));
					ht->put(x->ColumnName, new Variable(NULL, buf));
					break;
				case SQL_REAL:
					break;
				case SQL_TIMESTAMP:
				case SQL_DATETIME:
					ht->put(x->ColumnName, new Variable(NULL, decodetime(*((SQL_TIMESTAMP_STRUCT*) x->DataPtr), buf, sizeof(buf))));
					break;
				case SQL_DECIMAL:
					break;
				case SQL_NUMERIC:
					sprintf(buf,"%d", *((long*) x->DataPtr));
					ht->put(x->ColumnName, new Variable(NULL, buf));
					break;
				default:
					ctx.writeToStdErr("Unknown bindtype %d for column %s", x->BindType, x->ColumnName);
					break;
				}
			}
		}

		// Now run the body of query tag
		try {
			stmt.executeBody(ctx);
		} catch(...) {
			tsql->CloseSQL();
			throw;
		}

		res = (SQLRETURN) tsql->FetchRow();
	}

	tsql->CloseSQL();
}


Expr *OdbcDatasourceImpl::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "dsn") == 0) {
		return new Expr(m_dsn);
	}

	return DatasourceProviderImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// OdbcDatasourceImplFactory
///////////////////////////////////////////////////////////////////////////////

OdbcDatasourceImplFactory::OdbcDatasourceImplFactory()
{}


DatasourceProviderImpl *OdbcDatasourceImplFactory::create(
	DMINT32 implId, Datasource &parent, ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr dsn      = stmt.getOverridableArgAsString("dsn", parent, true, ctx);
	Property *username = parent.getProperty("username");
	Property *password = parent.getProperty("password");

	return new OdbcDatasourceImpl(*this, implId, parent, dsn, username, password);
}

///////////////////////////////////////////////////////////////////////////////
// LdapDatasourceImpl
///////////////////////////////////////////////////////////////////////////////

LdapDatasourceImpl::~LdapDatasourceImpl()
{
	SAFE_FREE(m_ldapserver);
}

LDAP *LdapDatasourceImpl::ConnectToLDAP(class Context &ctx,class ExtendedStmt *stmt)
{
	LDAP *ld;
	if (!m_ldapserver) {
		if (stmt) {
			throw RuntimeError(*stmt, ctx.stack(),"\"LDAP Server\" property must be specified");
		} else {
			ctx.writeToStdErr("\"LDAP Server\" property must be specified");
			return (LDAP *)0;
		}
	}
	char *sc = strdup(m_ldapserver);
	char *server = sc;
	// m_ldapserver could be in the format ldap:://ldap.server.com:389
	// If string contains // then strip that off
	char *p = strstr(server,"//");
	if (p) server=p+2;	// skip //
	// now check if port has been specified
	p=strchr(server,':');
	if (p) {
		m_port=atol(p+1);
		*p='\0';
	} else {
		if (m_port==0) m_port=LDAP_PORT;
	}
	ld = ldap_init(server,m_port);
	if (!ld) {
		if (stmt) {
			throw RuntimeError(*stmt,ctx.stack(),"Failed to connect to LDAP Server \"%s\" on port %d",server,m_port);
		} else {
			ctx.writeToStdErr("Failed to connect to LDAP Server \"%s\" on port %d",server,m_port);
			return (LDAP *)0;
		}
	}
	unsigned long res;
	unsigned long v = LDAP_VERSION3;
	res = ldap_set_option(ld,LDAP_OPT_PROTOCOL_VERSION,(void*)&v);
	if (res != LDAP_SUCCESS) {
		if (stmt) {
			throw RuntimeError(*stmt,ctx.stack(),"Failed switching LDAP protocol version to %ld",v);
		} else {
			ctx.writeToStdErr("Failed switching LDAP protocol version to %ld",v);
			return (LDAP *)0;
		}
	}
	//
	// Try connecting with the specified credentials (if given)
	//
	Credentials *cred = m_datasource.getCredentials();
	char *un = (char *)0;
	char *pw = (char *)0;

	if (cred) {
		un = cred->getDecryptedUsername(ctx);
		pw = cred->getDecryptedPassword(ctx);
	} else {
		if (m_username && m_password) {
			un = (char *)m_username->value();
			pw = (char *)m_password->value();
		}
	}
#ifdef WIN32
	if (un == (char *)0 || pw == (char *)0) {
		// No username or password - try anonymous connection
		res = ldap_connect(ld,NULL);
		if (res != LDAP_SUCCESS) {
			if (stmt) {
				throw RuntimeError(*stmt,ctx.stack(),"Failed to connect to LDAP Server \"%s\" on port %d: error %ld",server,m_port,res);
			} else {
				ctx.writeToStdErr("Failed to connect to LDAP Server \"%s\" on port %d: error %ld",server,m_port,res);
				return (LDAP *)0;
			}
		}
	} else {
#endif
		// credentials or username/password combination specified - use
		res = ldap_bind_s(ld,un,pw,LDAP_AUTH_SIMPLE);
		if (res != LDAP_SUCCESS) {
			if (stmt) {
				throw RuntimeError(*stmt,ctx.stack(),"Failed to connect to LDAP Server \"%s\" on port %d: error %ld",server,m_port,res);
			} else {
				ctx.writeToStdErr("Failed to connect to LDAP Server \"%s\" on port %d: error %ld",server,m_port,res);
			}
			return (LDAP *)0;
		}
#ifdef WIN32
	}
#endif
	SAFE_FREE(sc);
	return ld;
}

bool LdapDatasourceImpl::testProvider(class Context &ctx)
{
	LDAP *ld = ConnectToLDAP(ctx,(ExtendedStmt *)0);
	if (ld) {
		ldap_unbind(ld);
		ctx.writeToStdOut("Connection to LDAP datasource ok\n");
		return true;
	} else {
		return false;
	}
}

void LdapDatasourceImpl::query(class Model &model, class ExtendedStmt &stmt, class Context &ctx)
{
	// printf("running query on LDAP\n");
	unsigned long res;
	BerElement* pBer = NULL;
    PCHAR pAttribute = NULL;
    PCHAR* ppValue = NULL;
    ULONG iValue = 0;
	char *sb = stmt.getArgAsString("searchbase", ctx);
	if(!sb) {
		throw RuntimeError(stmt, ctx.stack(),"query failed - searchbase must be specified for LDAP datasources");
	}
	char *searchbase=sb;

	char *f = stmt.getArgAsString("filter", ctx);
	if(!f) {
		throw RuntimeError(stmt, ctx.stack(),"query failed - filter must be specified for LDAP datasources");
	}
	char *filter=f;

	char *result = stmt.getArgAsString("result", ctx);
	if(!result) {
		throw RuntimeError(stmt, ctx.stack(),"query failed - result array must be specified");
	}

	char **attrs = (char **)0;
	Expr *exprAtts = stmt.getArg("attributes",ctx);
	if (exprAtts) {
		// User has supplied an "attributes" array
		if (exprAtts->kind() != KIND_ARRAY) {
			throw RuntimeError(stmt, ctx.stack(),"attributes parameter should be an array");
		}
		DMArray *atts = exprAtts->toArray();
		// printf("array size = %d\n",atts->count());
		attrs = (char **)malloc(sizeof(char *)*(atts->count()+1));
		if (atts) {
			AutoPtr<StringList> keys = atts->keys();
			StringListIterator iter(*keys);
			int n=0;
			for(const char *key = iter.first(); key; key = iter.next()) {
				Variable *v = atts->get(key);
				if (v) {
					attrs[n++] = strdup(v->toString());
				}
			}
			attrs[n]=(char *)0;
			// debug - check
			/*
			n=0;
			while (attrs[n]) {
				printf("attribute: %s\n",attrs[n]);
				n++;
			}
			*/
		}
	}


	/*
	printf("searchbase=[%s]\n",searchbase);
	printf("filter=[%s]\n",filter);
	printf("result=[%s]\n",result);
	*/

	LDAP *ld = ConnectToLDAP(ctx,&stmt);
	if (ld) {
		// printf("Connected to LDAP okay\n");
		// Successful connection
		LDAPMessage *pMsg = NULL;
		res = ldap_search_s(ld,searchbase,LDAP_SCOPE_ONELEVEL,filter,attrs,0,&pMsg);
		if (res == LDAP_SUCCESS) {
			// Process results of the search
			// Create result array
			DMArray *ht = ctx.stack().newLocalArray(result, false, true);

			ULONG numberOfEntries = ldap_count_entries(ld,pMsg);
			LDAPMessage *pEntry = NULL;
			// printf("numberOfEntries=%d\n",numberOfEntries);
			for(int c=0;c<numberOfEntries;c++) {
				ht->clear();
				// printf("**** %d ****\n",c);
				// Get the first/next entry.
				if(c==0) {
					pEntry = ldap_first_entry(ld,pMsg);
				} else {
					pEntry = ldap_next_entry(ld,pEntry);
				}
				pAttribute = ldap_first_attribute(ld,pEntry,&pBer);
				while (pAttribute != NULL) {
					//printf("     ATTR: %s",pAttribute);
					// Get the string values.
					ppValue = ldap_get_values(ld,pEntry,pAttribute);
					if(ppValue == NULL) {
						// printf(": [NO ATTRIBUTE VALUE RETURNED]");
						ht->put(pAttribute,NULL);
					} else {
						iValue = ldap_count_values(ppValue);
						if(iValue) {
							//printf(": %s", *ppValue);
							if (iValue>1) {
								// More than one attribute value
								DMArray *ta = ctx.stack().newLocalArray(pAttribute,true,true);
								for(ULONG z=1; z<iValue; z++) {
									//printf(", %s", ppValue[z]);
									ta->add(new Variable(NULL, (char*) ppValue[z]));
								}
								// not sure how to do this.
								// ht->put(pAttribute,ta);
							} else {
								ht->put(pAttribute,new Variable(NULL, (char*) *ppValue));
							}
						}
					}
					// Free memory.
					if (ppValue != NULL) ldap_value_free(ppValue);
					ppValue = NULL;
					ldap_memfree(pAttribute);
					// Get next attribute name.
					pAttribute = ldap_next_attribute(ld,pEntry,pBer);
					//printf("\n");
				}
				// Now execute the body of the query
				try {
					stmt.executeBody(ctx);
				} catch(...) {
					if (pMsg != NULL) {
						ldap_msgfree(pMsg);
					}
					ldap_unbind(ld);
					throw;
				}
			}

		} else {
			if (pMsg != NULL) {
				ldap_msgfree(pMsg);
			}
			ldap_unbind(ld);
			throw RuntimeError(stmt, ctx.stack(),"search failed: error %ld",res);
		}


		if (pMsg != NULL) {
			ldap_msgfree(pMsg);
		}
		ldap_unbind(ld);
	} else printf("didn't connect to LDAP\n");
}

Expr *LdapDatasourceImpl::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "ldapserver") == 0) {
		return new Expr(m_ldapserver);
	}
	return DatasourceProviderImpl::getAttribute(name, ctx);
}

LdapDatasourceImpl::LdapDatasourceImpl(
		DatasourceProviderImplFactory &factory, DMINT32 implId,
		Datasource &datasource, const char *ldapServer, const char *ldapPort,
		Property *username, Property *password
	)
	: DatasourceProviderImpl(factory, implId, datasource), m_ldapserver(DUP_NULL(ldapServer)),
	m_username(username), m_password(password), m_port(ldapPort?atol(ldapPort):0)
{
	debug1("LdapDatasourceImpl('%s')", NULL_CHECK(m_ldapserver));
}


///////////////////////////////////////////////////////////////////////////////
// LdapDatasourceImplFactory
///////////////////////////////////////////////////////////////////////////////

LdapDatasourceImplFactory::LdapDatasourceImplFactory()
{}


DatasourceProviderImpl *LdapDatasourceImplFactory::create(
	DMINT32 implId, Datasource &parent, ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr ldapServer = stmt.getOverridableArgAsString("LDAP Server", parent, true, ctx);
	ConstCharPtr ldapPort = stmt.getOverridableArgAsString("Port Number", parent, false, ctx);
	Property *username = parent.getProperty("username");
	Property *password = parent.getProperty("password");

	return new LdapDatasourceImpl(*this, implId, parent, ldapServer, ldapPort, username, password);
}

///////////////////////////////////////////////////////////////////////////////

// TODO: remove "odbcdatasouce_" prefix when moving to a DLL

extern "C" ODBCDATASOURCE_EXPORT int odbcdatasource_PluginStart(DM &dm)
{
	dm.writeToLogFile("ODBC Datasource plugin V" ODBCDATASOURCE_PLUGIN_VERSION);

	DatasourceProviderImplRegistry::instance().registerFactory("odbc", new OdbcDatasourceImplFactory());
	DatasourceProviderImplRegistry::instance().registerFactory("ldap", new LdapDatasourceImplFactory());
	return 0;
}


extern "C" int odbcdatasource_PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("dsn",      true,  false));
	propdefs->add(new PropertyDef("username", false, false));
	propdefs->add(new PropertyDef("password", false, false));

	dm.installProviderImpl("odbc",  NULL, OBJ_KIND_DATASOURCE, propdefs);
	return 0;
}

#endif /*HAVE_ODBC*/
