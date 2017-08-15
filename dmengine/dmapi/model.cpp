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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "model.h"
//#include "db.h"
#include "triodbc.h"
#include "scopestack.h"
#include "repository.h"
#include "exceptions.h"
#include "dropzone.h"
#include "context.h"
#include "notify.h"
#include "node.h"
#include "datasource.h"
#include "dm.h"
#include "properties.h"
#include "crypto.h"
#include "autoptr.h"
#include "lexer.h"
#include "expr.h"
#include "charptr.h"
#include "credentials.h"
#include "pathname.h"
#ifndef WIN32
#include <pthread.h>
#endif
#include "thread.h"
#include "rewrap.h"
#include "audit.h"
#include "scriptgen.h"
#include "engineconfig.h"
#include "task.h"
#include "envp.h"
#include "notifytemplate.h"
#include "timedjob.h"

extern int yyparse(void *buffer);
#ifndef WIN32
#define stricmp strcasecmp
#include <unistd.h>
#endif

extern char *base64encode(unsigned char *data, unsigned long datalen);
///////////////////////////////////////////////////////////////////////////////
// SimplePermission
///////////////////////////////////////////////////////////////////////////////

typedef enum tagSimplePermission {
	UNSET,
	ALLOW,
	DENY
} SimplePermission;


///////////////////////////////////////////////////////////////////////////////
// ObjectAccess
///////////////////////////////////////////////////////////////////////////////

class ObjectAccess
{
private:
	SimplePermission m_readAccess;
	SimplePermission m_writeAccess;

	static SimplePermission fromString(const char *str);
	static SimplePermission combine(SimplePermission obj, const char *str);

	void addObjectAccess(const char *read, const char *write);
	void addDomainAccess(const char *read, const char *write);

public:
	ObjectAccess();

	bool isReadable()   { return (m_readAccess  == ALLOW); }
	bool isWriteable()  { return (m_writeAccess == ALLOW); }

	friend class Model;
};


ObjectAccess::ObjectAccess()
	: m_readAccess(UNSET), m_writeAccess(UNSET)
{}


void ObjectAccess::addObjectAccess(const char *read, const char *write)
{
	m_readAccess  = fromString(read);
	m_writeAccess = fromString(write);
}


void ObjectAccess::addDomainAccess(const char *read, const char *write)
{
	m_readAccess  = combine(m_readAccess,  read);
	m_writeAccess = combine(m_writeAccess, write);
}


/*static*/ SimplePermission ObjectAccess::fromString(const char *str)
{
	return (str != NULL) ? ((str[0] == 'Y') ? ALLOW : DENY) : UNSET;
}


/*static*/ SimplePermission ObjectAccess::combine(SimplePermission obj, const char *str)
{
	switch(fromString(str)) {
	case ALLOW: return ((obj == UNSET) ? ALLOW : obj);
	case DENY:  return ((obj == UNSET) ? DENY : obj);
	default:    return obj;
	}
}


///////////////////////////////////////////////////////////////////////////////
// Object
///////////////////////////////////////////////////////////////////////////////


Object::Object(Model &model, int id, const char *name)
	: m_model(model), m_id(id), m_name(DUP_NULL(name)), m_ownerSet(false),
	  m_owner(NULL), m_owngrp(NULL), m_domainSet(false), m_domain(NULL),
	  m_summary(NULL), m_notes(NULL), m_vars(NULL), m_access(NULL),
	  m_cacheFQDomain(NULL), m_cacheFQName(NULL), m_metaDataSet(false),
	  m_creationtime(0),m_modifiedtime(0),m_creator(NULL),m_modifier(NULL)
{}


/*virtual*/ Object::~Object()
{
	SAFE_FREE(m_name);
	m_owner  = NULL;		// Don't own
	m_owngrp = NULL;		// Don't own
	m_domain = NULL;		// Don't own
	SAFE_FREE(m_summary);
	SAFE_FREE(m_notes);
	SAFE_DELETE(m_vars);
	SAFE_DELETE(m_access);
	SAFE_FREE(m_cacheFQDomain);
	SAFE_FREE(m_cacheFQName);
}


Object *Object::getOwner()
{
	if(!m_ownerSet) {
		m_ownerSet = true;
		m_model.getOwnerForObject(*this);
	}

	return m_owner ? (Object*) m_owner : (Object*) m_owngrp;
}

DateTime *Object::getCreationTime()
{
	if (!m_metaDataSet) {
		m_metaDataSet = true;
		m_model.getMetaDataForObject(*this);
	}

	return new DateTime(m_creationtime);
}

DateTime *Object::getModifiedTime()
{
	if (!m_metaDataSet) {
		m_metaDataSet = true;
		m_model.getMetaDataForObject(*this);
	}

	return new DateTime(m_modifiedtime);
}

User *Object::getCreator()
{
	if (!m_metaDataSet) {
		m_metaDataSet = true;
		m_model.getMetaDataForObject(*this);
	}

	return m_creator;
}

User *Object::getModifier()
{
	if (!m_metaDataSet) {
		m_metaDataSet = true;
		m_model.getMetaDataForObject(*this);
	}

	return m_modifier;
}



void Object::setOwner(User *owner)
{
	m_ownerSet = true;
	m_owner    = owner;
}


void Object::setOwner(UserGroup *owngrp)
{
	m_ownerSet = true;
	m_owngrp   = owngrp;
}

void Object::setCreator(User *creator)
{
	m_creator = creator;
}

void Object::setModifier(User *modifier)
{
	m_modifier = modifier;
}

void Object::setCreationTime(int created)
{
	m_creationtime = created;
}

void Object::setModifiedTime(int modified)
{
	m_modifiedtime = modified;
}


//void Object::addAccess(UserGroup &usrgrp, ObjectAccess *oa)
//{
//	if(!m_access) {
//		m_access = new HashtableById<ObjectAccess>(true);
//	}
//	m_access->put(usrgrp.id(), oa);
//}


bool Object::internalHasDomainAccess(User *user)
{
	if(!m_domainSet) {
		getDomain();
	}
	// Only domains don't have to have a parent domain
	if(!m_domain && (kind() != OBJ_KIND_DOMAIN)) {
		debug1("Domain for object with id %d and kind %d not set", id(), kind());
		return false;
	}
	if(m_domain) {
		// Is the user's domain above the level of the object we are accessing?
		Domain *userdom = user->getDomain();
		if(!userdom) {
			debug1("user's domain NOT found - disallow access");
			return false;
		}
		Domain *dom;
		// THIS IS WRONG
		// Loop through all the parent domains of the user
		//for(; userdom; userdom = userdom->getDomain()) {
		//	// Loop through all the parent domains of the object
		//	for(dom = m_domain; dom; dom = dom->getDomain()) {
		//		debug3("Checking '%s'", dom->name());
		//		if(userdom == dom) {
		//			debug3("user's domain found");
		//			break;
		//		}
		//	}
		//}
		// END THIS IS WRONG

		// First check if the User's domain one of the parent domains of the object
		for(dom = m_domain; dom; dom = dom->getDomain()) {
			debug3("Checking '%s'", dom->name());
			if(userdom == dom) {
				debug3("user's domain found");
				break;
			}
		}
		if(!dom) {
			// No, so now check if the object's domain is one of the parent domains of the User
			for(dom = userdom; dom; dom = dom->getDomain()) {
				debug3("Checking '%s'", dom->name());
				if(m_domain == dom) {
					debug3("object's domain found");
					break;
				}
			}
		}
		if(!dom) {
			debug1("user's domain '%s' cannot see object in domain '%s' - disallow access",
				userdom->getFQName(), m_domain->getFQName());
			return false;
		}

		//// Check that the parent domain allows access
		//if(!m_domain->hasAccess(user)) {
		//	// No access to parent domain - disallow access
		//	return false;
		//}
	}

	return true;
}


bool Object::hasAccess(User *user)
{
	// If user is not supplied, then we check the current user
	Thread::lock(__LINE__,__FILE__);
	if(!user) {
		user = m_model.getCurrentUser();
	}
	if(!user) {
		// Failed to get current user - disallow access
		Thread::unlock(__LINE__,__FILE__);
		return false;
	}

	if(!internalHasDomainAccess(user)) {
		Thread::unlock(__LINE__,__FILE__);
		return false;
	}


	// Now check our ACL
	if(!m_access) {
		m_access = m_model.getAccessForObject(*this);
	}

	//// If ACL is empty, then default is to allow access
	//if(m_access->count() == 0) {
	//	return true;
	//}

	List<UserGroup> *groups = user->getUserGroups();
	if(!groups) {
		Thread::unlock(__LINE__,__FILE__);
		return false;
	}

	ListIterator<UserGroup> iter(*groups);
	for(UserGroup *grp = iter.first(); grp; grp = iter.next()) {
		ObjectAccess *access = m_access->get(grp->id());
		if(access && (access->isReadable() || access->isWriteable())) {
			// ACL contains a group that the user is a member of
			Thread::unlock(__LINE__,__FILE__);
			return true;
		}
	}

	Thread::unlock(__LINE__,__FILE__);
	return false;
}


bool Object::hasReadAccess(User *user)
{
	// If user is not supplied, then we check the current user
	if(!user) {
		user = m_model.getCurrentUser();
	}
	if(!user) {
		// Failed to get current user - disallow access
		return false;
	}

	if(!internalHasDomainAccess(user)) {
		return false;
	}

	// Now check our ACL
	if(!m_access) {
		m_access = m_model.getAccessForObject(*this);
	}

	//// If ACL is empty, then default is to allow access
	//if(m_access->count() == 0) {
	//	return true;
	//}

	List<UserGroup> *groups = user->getUserGroups();
	if(!groups) {
		return false;
	}

	ListIterator<UserGroup> iter(*groups);
	for(UserGroup *grp = iter.first(); grp; grp = iter.next()) {
		ObjectAccess *access = m_access->get(grp->id());
		if(access && access->isReadable()) {
			// ACL contains a group with read access that the user is a member of
			return true;
		}
	}

	return false;
}


bool Object::hasWriteAccess(User *user)
{
	// If user is not supplied, then we check the current user
	if(!user) {
		user = m_model.getCurrentUser();
	}
	if(!user) {
		// Failed to get current user - disallow access
		return false;
	}

	if(!internalHasDomainAccess(user)) {
		return false;
	}

	// Now check our ACL
	if(!m_access) {
		m_access = m_model.getAccessForObject(*this);
	}

	//// If ACL is empty, then default is to disallow access - this is different from read access
	//if(m_access->count() == 0) {
	//	return false;
	//}

	List<UserGroup> *groups = user->getUserGroups();
	if(!groups) {
		return false;
	}

	ListIterator<UserGroup> iter(*groups);
	for(UserGroup *grp = iter.first(); grp; grp = iter.next()) {
		ObjectAccess *access = m_access->get(grp->id());
		if(access && access->isWriteable()) {
			// ACL contains a group with read access that the user is a member of
			return true;
		}
	}

	return false;
}

/* temp commented out - will add this later 
bool Object::hasViewAccess(User *user)
{
	// If user is not supplied, then we check the current user
	if(!user) {
		user = m_model.getCurrentUser();
	}
	if(!user) {
		// Failed to get current user - disallow access
		return false;
	}

	if(!internalHasDomainAccess(user)) {
		return false;
	}

	// Now check our ACL
	if(!m_access) {
		m_access = m_model.getAccessForObject(*this);
	}

	//// If ACL is empty, then default is to disallow access - this is different from read access
	//if(m_access->count() == 0) {
	//	return false;
	//}

	List<UserGroup> *groups = user->getUserGroups();
	if(!groups) {
		return false;
	}

	ListIterator<UserGroup> iter(*groups);
	for(UserGroup *grp = iter.first(); grp; grp = iter.next()) {
		ObjectAccess *access = m_access->get(grp->id());
		if(access && access->is()) {
			// ACL contains a group with read access that the user is a member of
			return true;
		}
	}

	return false;
}
*/

Domain *Object::getDomain()
{
	if(!m_domainSet) {
		m_domainSet = true;
		m_model.getDomainForObject(*this);
	}
	return m_domain;
}


void Object::setDomain(Domain *domain)
{
	m_domainSet = true;
	m_domain    = domain;
}


const char *Object::getFQDomain()
{
	if(m_cacheFQDomain) {
		return m_cacheFQDomain;
	}

	Domain *domain = getDomain();
	if(!domain) {
		return NULL;
	}

	const char *mydom = domain->name();
	const char *fqdom = domain->getFQDomain();

	if(!fqdom) {
		m_cacheFQDomain = strdup(mydom);
	} else {
		m_cacheFQDomain = (char*) malloc(strlen(fqdom) + strlen(mydom) + 2);
		sprintf(m_cacheFQDomain, "%s.%s", fqdom, mydom);
	}

	return m_cacheFQDomain;
}


const char *Object::getFQName()
{
	if(m_cacheFQName) {
		return m_cacheFQName;
	}

	const char *fqdom = getFQDomain();

	if(!fqdom) {
		m_cacheFQName = strdup(m_name);
	} else {
		m_cacheFQName = (char*) malloc(strlen(fqdom) + strlen(m_name) + 2);
		sprintf(m_cacheFQName, "%s.%s", fqdom, m_name);
	}

	return m_cacheFQName;
}


const char *Object::getSummary()
{
	if(m_summary) {
		return m_summary;
	}
	m_model.getSummaryForObject(*this);
	return m_summary;
}


void Object::setSummary(const char *summary)
{
	SAFE_FREE(m_summary);
	m_summary = strdup(summary ? summary : "");
}


const char *Object::getNotes()
{
	if(m_notes) {
		return m_notes;
	}
	m_model.getNotesForObject(*this);
	return m_notes;
}


void Object::setNotes(const char *notes)
{
	SAFE_FREE(m_notes);
	m_notes = strdup(notes ? notes : "");
}


void Object::indent(int indent)
{
	for(int n = 0; n < indent; n++) {
		printf("  ");
	}
}


void Object::printObject(int ind)
{
	if(m_access) {
		indent(ind); printf("Access: ");
		AutoPtr<IntList> keys = m_access->keys();
		IntListIterator iter(*keys);
		bool first = true;
		for(int g = iter.first(); iter.more(); g = iter.next()) {
			ObjectAccess *a = m_access->get(g);
			printf("%s%d(%c%c)", (first ? "" : ", "), g,
				(a->isReadable() ? 'r' : '-'), (a->isWriteable() ? 'w' : '-'));
			first = false;
		}
		printf("\n");
	}
	if(m_ownerSet) {
		indent(ind);
		if(m_owner) {
			printf("Owner:\n");
			m_owner->print(ind+1);
		} else if(m_owngrp) {
			printf("Owner:\n");
			m_owngrp->print(ind+1);
		} else {
			printf("Owner: (no owner)\n");
		}
	}
}


/*virtual*/ Expr *Object::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "id") == 0) {
		return new Expr(m_id);
	} else if(strcmp(name, "name") == 0) {
		return new Expr(m_name);
	} else if(strcmp(name, "fqdomain") == 0) {
		const char *fqd = getFQDomain();
		return fqd ? new Expr(fqd) : NULL;
	} else if(strcmp(name, "summary") == 0) {
		const char *summary = getSummary();
		return summary ? new Expr(summary) : NULL;
	} else if(strcmp(name, "owner") == 0) {
		IObject *owner = getOwner();
		return owner ? new Expr(owner) : NULL;
	} else if(strcmp(name, "domain") == 0) {
		Domain *domain = getDomain();
		return domain ? new Expr(domain) : NULL;
	} else if(strcmp(name,"attributes") == 0) {
		Scope *scope = getVars();
		DMArray *arr = scope->getVars();
		return new Expr(arr);
	} else if (strcmp(name,"ctime") == 0) {
		DateTime *res = getCreationTime();
		return new Expr(res);
	} else if (strcmp(name,"mtime") == 0) {
		DateTime *res = getModifiedTime();
		return new Expr(res);
	} else if (strcmp(name,"creator") == 0) {
		IObject *creator = getCreator();
		return creator ? new Expr(creator) : NULL;
	} else if (strcmp(name,"modifier") == 0) {
		IObject *modifier = getModifier();
		return modifier ? new Expr(modifier) : NULL;
	}
	Scope *s = getVars();
	Variable *v = s->get(name);
	return v?new Expr(v):NULL;
}


///*virtual*/ class DMArray *Object::getArrayAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


///*virtual*/ IObject *Object::getObjectAttribute(const char *name, class Context &ctx)
//{
//	if(strcmp(name, "owner") == 0) {
//		return getOwner();
//	}
//	if(strcmp(name, "parent") == 0) {
//		return getDomain();
//	}
//	return NULL;
//}


/*virtual*/ Scope *Object::getVars()
{
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
// ObjectKindAndId
///////////////////////////////////////////////////////////////////////////////

typedef struct tagObjectKindLookup {
	const char *str;
	OBJECT_KIND kind;
} ObjectKindLookup;

ObjectKindLookup objectKindLookupTable[] = {
	{ "en", OBJ_KIND_ENVIRONMENT },
	{ "do", OBJ_KIND_DOMAIN },
	{ "se", OBJ_KIND_SERVER },
	{ "ap", OBJ_KIND_APPLICATION },
	{ "us", OBJ_KIND_USER },
	{ "gr", OBJ_KIND_USERGROUP },
	{ "re", OBJ_KIND_REPOSITORY },
	{ "no", OBJ_KIND_NOTIFY },
	{ "ds", OBJ_KIND_DATASOURCE },
	{ "ac", OBJ_KIND_ACTION },
	//OBJ_KIND_DROPZONE
	{ "co", OBJ_KIND_COMPONENT },
	{ "ci", OBJ_KIND_COMPONENTITEM },
	//OBJ_KIND_DROPZONEFILE
	{ "xf", OBJ_KIND_TRANSFER },
	//OBJ_KIND_APPVERSION
	{ "cr", OBJ_KIND_CREDENTIALS },
	{ NULL, OBJ_KIND_NONE }
};

OBJECT_KIND ObjectKindFromTypeString(const char *str)
{
	for(ObjectKindLookup *ok = objectKindLookupTable; ok && ok->str; ok++) {
		if(strcmp(str, ok->str) == 0) {
			return ok->kind;
		}
	}
	return OBJ_KIND_NONE;
}

const char *ObjectKindToTypeString(OBJECT_KIND val)
{
	for(ObjectKindLookup *ok = objectKindLookupTable; ok && ok->str; ok++) {
		if(val == ok->kind) {
			return ok->str;
		}
	}
	return NULL;
}

ObjectKindAndId::ObjectKindAndId(const char *otid)
{
	if(!otid || (strlen(otid) < 3)) {
		throw RuntimeError("Invalid OTID: '%s'", NULL_CHECK(otid));
	}
	char ot[3];
	sprintf(ot, "%c%c", otid[0], otid[1]);
	const char *oid = &otid[2];
	m_objkind = ObjectKindFromTypeString(ot);

	if(m_objkind == OBJ_KIND_NONE) {
		throw RuntimeError("Invalid Object Type: '%s'", ot);
	}
	
	m_id = atoi(oid);
	if(m_id == 0) {
		throw RuntimeError("Invalid Object Id: '%s'", oid);
	}
}


ObjectKindAndId::ObjectKindAndId(OBJECT_KIND objkind, int id)
	: m_objkind(objkind), m_id(id)
{}


ObjectKindAndId::ObjectKindAndId(Object &obj)
	: m_objkind(obj.kind()), m_id(obj.id())
{}

	
char *ObjectKindAndId::toString()
{
	const char *ot = ObjectKindToTypeString(m_objkind);
	if(!ot) {
		throw RuntimeError("Invalid Object Type: %d", m_objkind);
	}

	char buf[34];
	sprintf(buf, "%s%d", ot, m_id);
	return strdup(buf);
}


///////////////////////////////////////////////////////////////////////////////
// ProviderObject
///////////////////////////////////////////////////////////////////////////////

ProviderObject::ProviderObject(Model &model, int id, const char *name)
	: Object(model, id, name), m_credentials(NULL),		////, m_type(DUP_NULL(type))
	  m_props(NULL), m_def(NULL)
{}


/*virtual*/ ProviderObject::~ProviderObject()
{
	SAFE_DELETE(m_props);
	m_credentials = NULL;	// Owned by cache
	m_def = NULL;			// Owned by cache
}


/*virtual*/ Expr *ProviderObject::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "type") == 0) {
		return new Expr(getDef(ctx).name());
	}

	// TODO: This gives direct access to properties, even if the impl doesn't
	// expose them - this may be useful, but it is also a bit of a security
	// hole - comment out for the time being until we decide if it is useful
	Expr *ret = Object::getAttribute(name, ctx);
	////if(!ret) {
	////	Property *prop = getProperty(name);
	////	ret = prop ? prop->value() : NULL;
	////}

	return ret;
}


Credentials *ProviderObject::getCredentials()
{
	if(!m_credentials) {
		m_model.getCredentialsForProviderObject(*this);
	}

	return m_credentials;
}


void ProviderObject::setCredentials(Credentials *creds)
{
	m_credentials = creds;
}


ProviderObjectDef &ProviderObject::getDef(Context &ctx)
{
	if(!m_def) {
		m_def = m_model.getDefForProviderObject(*this);
	}
	if(!m_def) {
		throw RuntimeError(ctx.stack(), "No definition for %s '%s'", def_kind(), name());
	}

	return *m_def;
}


Property *ProviderObject::getProperty(const char *name)
{
	return m_props ? m_props->get(name) : NULL;
}


void ProviderObject::setProperty(Property *prop)
{
	if(!m_props) {
		m_props = new Hashtable<Property>(false, true);
	}
	m_props->put(prop->name(), prop);
}


Property *ProviderObject::getDefProperty(const char *name)
{
	if(!m_def) {
		throw RuntimeError("Provider object definition not yet resolved for provider '%s'", m_name);
	}
	return m_def->getDefProperty(name);
}


void ProviderObject::setDefProperty(Property *prop)
{
	if(!m_def) {
		throw RuntimeError("Provider object definition not yet resolved for provider '%s'", m_name);
	}
	m_def->setDefProperty(prop);
}


void ProviderObject::printProviderObject(int ind)
{
	indent(ind); printf("Type: %s\n", (m_def ? m_def->name() : "*no def*"));
	printObject(ind);
	if(m_props) {
		indent(ind); printf("Properties:\n");
		AutoPtr<StringList> keys = m_props->keys();
		StringListIterator iter(*keys);
		for(const char *name = iter.first(); name; name = iter.next()) {
			Property *prop =  m_props->get(name);
			indent(ind+1); printf("%s: %s\n", name, (prop ? prop->value() : "(null)"));
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// ProviderObjectDef
///////////////////////////////////////////////////////////////////////////////

ProviderObjectDef::ProviderObjectDef(
		int id, const char *name, const char *kind, Plugin *plugin
	)
	: m_id(id), m_name(DUP_NULL(name)),
	  m_plugin(plugin), m_kind(DUP_NULL(kind)),
	  m_defprops(NULL), m_propdefs(NULL), m_checker(NULL)
{}


ProviderObjectDef::~ProviderObjectDef()
{
	SAFE_FREE(m_name);
	SAFE_FREE(m_kind);
	SAFE_DELETE(m_defprops);
	SAFE_DELETE(m_propdefs);
	SAFE_DELETE(m_checker);
	m_plugin = NULL;	// Owned by cache
}


List<PropertyDef> *ProviderObjectDef::getPropertyDefs()
{
	if(!m_propdefs) {
		m_propdefs = new List<PropertyDef>(true);
		//	m_model.getProperyDefsForProviderObject(*this); - might do it this way at some point
	}
	return m_propdefs;
}


void ProviderObjectDef::add(class PropertyDef *propdef)
{
	if(!m_propdefs) {
		m_propdefs = new List<PropertyDef>(true);
	}
	m_propdefs->add(propdef);
}


Plugin *ProviderObjectDef::getPlugin()
{
	// might do lazy load at some point
	//if(!m_plugin) {
	//	m_model.getPluginForProviderObjectDef(*this);
	//}
	return m_plugin;
}


void ProviderObjectDef::createChecker()
{
	char *temp = (char*) malloc(strlen(m_name) + strlen(m_kind) + 2);
	sprintf(temp, "%s %s", m_name, m_kind);
	m_checker = new PropertyChecker(m_propdefs, temp);
	free(temp);
}


void ProviderObjectDef::verifyProperties(class Context &ctx, Hashtable<class Property> *props)
{
	if(!m_checker) {
		createChecker();
	}
	m_checker->checkProps(ctx, props);
}

void ProviderObjectDef::verifyArgs(class Context &ctx, class StmtList *args, Hashtable<class Property> *props)
{
	if(!m_checker) {
		createChecker();
	}
	m_checker->checkArgs(ctx, args, props);
}


Property *ProviderObjectDef::getDefProperty(const char *name)
{
	return m_defprops ? m_defprops->get(name) : NULL;
}


void ProviderObjectDef::setDefProperty(Property *prop)
{
	if(!m_defprops) {
		m_defprops = new Hashtable<Property>(false, true);
	}
	m_defprops->put(prop->name(), prop);
}


///////////////////////////////////////////////////////////////////////////////
// ProviderObjectImpl
///////////////////////////////////////////////////////////////////////////////

ProviderObjectImpl::ProviderObjectImpl(DMINT32 implId, ProviderObject &prov)
	: m_implId(implId), m_provider(prov), m_vars(NULL)
{}


/*virtual*/ ProviderObjectImpl::~ProviderObjectImpl()
{
	SAFE_DELETE(m_vars);
}


Scope *ProviderObjectImpl::getVars()
{
	if(!m_vars) {
		m_vars = new DelegatingScope(*(m_provider.getVars()), this);
		m_vars->addRef();
	}
	return m_vars;
}


/*virtual*/ Expr *ProviderObjectImpl::getAttribute(
	const char *name, Context &ctx)
{
	return m_provider.getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// Credentials
///////////////////////////////////////////////////////////////////////////////

Credentials::Credentials(Model &model,CREDENTIALS_KIND credkind)
	: Object(model, 0, ""), m_credkind(credkind), m_encusername(NULL),
	  m_encpassword(NULL), m_filename(NULL), m_impls(NULL)
{}


Credentials::Credentials(
		Model &model,int id, const char *name, CREDENTIALS_KIND credkind,
		const char *encusername, const char *encpassword
	)
	: Object(model, id, name), m_credkind(credkind),
	  m_encusername(DUP_NULL(encusername)), m_encpassword(DUP_NULL(encpassword)),
	  m_filename(NULL), m_impls(NULL)
{}


Credentials::Credentials(
		Model &model,int id, const char *name, CREDENTIALS_KIND credkind, const char *filename
	)
	: Object(model, id, name), m_credkind(credkind), m_encusername(NULL),
	  m_encpassword(NULL), m_filename(DUP_NULL(filename)), m_impls(NULL)
{}

Credentials::Credentials(
		Model &model,int id, const char *name,
		const char *username, const char *filename
	)
	: Object(model, id, name), m_credkind(CREDENTIALS_PRIVATE_KEY), m_encusername(DUP_NULL(username)),
	  m_encpassword(NULL), m_filename(DUP_NULL(filename)), m_impls(NULL)		
{}

Credentials::Credentials(Model &model,Property *username, Property *password)
	: Object(model, 0, strdup("From Properties")), m_credkind(CREDENTIALS_ENCRYPTED),
	  m_encusername(NULL), m_encpassword(NULL), m_filename(NULL), m_impls(NULL)
{
	if(username) {
		if(username->m_encrypted) {
			m_encusername = strdup(username->m_value);
		} else {
			m_encusername = encryptValue(username->m_value, strlen(username->m_value));
		}
	}
	if(password) {
		if(password->m_encrypted) {
			m_encpassword = strdup(password->m_value);
		} else {
			m_encpassword = encryptValue(password->m_value, strlen(password->m_value));
		}
	}
}


Credentials::~Credentials()
{
	SAFE_FREE(m_name);
	SAFE_FREE(m_encusername);
	SAFE_FREE(m_encpassword);
	SAFE_FREE(m_filename);
	SAFE_DELETE(m_impls);
}

Expr *Credentials::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "username") == 0) {
		return new Expr(getDecryptedUsername(ctx));
	} else if(strcmp(name, "password") == 0) {
		return new Expr(getDecryptedPassword(ctx));
	} else if (strcmp(name,"kind") == 0) {
		return new Expr(kind());
	} else if (strcmp(name,"b64auth") == 0) {
		char *username = getDecryptedUsername(ctx);
		char *password = getDecryptedPassword(ctx);
		char *t = (char *)malloc(strlen(username)+strlen(password)+2);
		sprintf(t,"%s:%s",username,password);
		char *res = base64encode((unsigned char *)t,strlen(t));
		Expr *ret = new Expr(res);
		free(res);
		free(t);
		return ret;
	}
	return Object::getAttribute(name, ctx);
}

char *Credentials::internalDecryptValue(const char *value, Context &ctx)
{
	if(!hasAccess(NULL)) {
		throw PermissionDeniedException(ctx.stack(), "You do not have permisson to access these credentials");
	}

	if(!value) {
		// throw RuntimeError(ctx.stack(), "Credentials value was not set - unable to decrypt");
		return strdup("");
	}

	return decryptValue(value, strlen(value));
}


char *Credentials::getDecryptedUsername(class Context &ctx)
{
	switch(m_credkind) {
	case CREDENTIALS_ENCRYPTED:
	case CREDENTIALS_IN_DATABASE:
		return internalDecryptValue(m_encusername, ctx);
	case CREDENTIALS_FROM_VARS:
	case CREDENTIALS_PRIVATE_KEY: {
		Node uservar(NODE_STR, strdup(m_encusername), true);
		ExprPtr euser = uservar.evaluate(ctx);
		return (euser ? euser->toString() : NULL);
		}
	}

	return NULL;
}

char *Credentials::filename(class Context &ctx)
{
	if (m_filename) {
		Node keyfilevar(NODE_STR, strdup(m_filename), true);
		ExprPtr ekeyfile = keyfilevar.evaluate(ctx);
		return (ekeyfile ? ekeyfile->toString() : NULL);
	} else {
		return NULL;
	}
}


char *Credentials::getDecryptedPassword(class Context &ctx)
{
	switch(m_credkind) {
	case CREDENTIALS_ENCRYPTED:
	case CREDENTIALS_IN_DATABASE:
		return internalDecryptValue(m_encpassword, ctx);
	case CREDENTIALS_FROM_VARS: {
		Node passvar(NODE_STR, strdup(m_encpassword), true);
		ExprPtr epass = passvar.evaluate(ctx);
		return (epass ? epass->toString() : NULL);
		}
	case CREDENTIALS_PRIVATE_KEY:
		return NULL;
	}

	return NULL;
}


bool Credentials::hasUsername()
{
	switch(m_credkind) {
	case CREDENTIALS_ENCRYPTED:
	case CREDENTIALS_IN_DATABASE:
	case CREDENTIALS_FROM_VARS:
	case CREDENTIALS_PRIVATE_KEY:
		return (m_encusername ? true : false);
	case CREDENTIALS_RTI3_DFO_IN_FILESYSTEM:
	case CREDENTIALS_HARVEST_DFO_IN_FILESYSTEM:
		return true;
	default:
		return false;
	}
}


bool Credentials::hasPassword()
{
	switch(m_credkind) {
	case CREDENTIALS_ENCRYPTED:
	case CREDENTIALS_IN_DATABASE:
	case CREDENTIALS_FROM_VARS:
		return (m_encpassword ? true : false);
	case CREDENTIALS_RTI3_DFO_IN_FILESYSTEM:
	case CREDENTIALS_HARVEST_DFO_IN_FILESYSTEM:
		return true;
	default:
		return false;
	}
}


CredentialsProvider *Credentials::createImpl(const char *provider, Context &ctx)
{
	CredentialsProviderFactory *factory =
		CredentialsProviderRegistry::instance().getFactory(provider);

	if(!factory) {
		throw RuntimeError(ctx.stack(), "Unrecognised credentials provider type '%s'", provider);
	}

	// Create an instance of the impl so that we may later execute it
	CredentialsProvider *impl = factory->create(*this);
	if(!impl) {
		throw RuntimeError(ctx.stack(), "Unexpected error creating instance of credentials provider type '%s'", provider);
	}

	if(!m_impls) {
		m_impls = new Hashtable<CredentialsProvider>(false, true);
	}
	m_impls->put(provider, impl);
	return impl;
}


bool Credentials::canTransform(const char *provider, Context &ctx)
{
	CredentialsProvider *impl = m_impls ? m_impls->get(provider) : NULL;
	if(!impl) {
		impl = createImpl(provider, ctx);
	}

	return impl->canTransform();
}


const char *Credentials::transform(const char *provider, Object &auth, Context &ctx)
{
	CredentialsProvider *impl = m_impls ? m_impls->get(provider) : NULL;
	if(!impl) {
		impl = createImpl(provider, ctx);
	}

	return impl->transform(auth, ctx);
}


void Credentials::print(int indent)
{
	switch(m_credkind) {
	case CREDENTIALS_USE_DIALOG:				printf("Use dialog"); break;
	case CREDENTIALS_ENCRYPTED:					printf("Encrypted"); break;
	case CREDENTIALS_IN_DATABASE:				printf("In database"); break;
	case CREDENTIALS_RTI3_DFO_IN_FILESYSTEM:	printf("RTI DFO"); break;
	case CREDENTIALS_HARVEST_DFO_IN_FILESYSTEM: printf("Harvest DFO"); break;
	case CREDENTIALS_PRIVATE_KEY:				printf("Private Key"); break;
	default: printf("Unknown"); break;
	}
}


///////////////////////////////////////////////////////////////////////////////
// Environment
///////////////////////////////////////////////////////////////////////////////


Environment::Environment(
		Model &model, int id, const char *name, const char *basedir
	)
	: Object(model, id, name), m_basedir(DUP_NULL(basedir)), m_servers(NULL),
	  m_apps(NULL), /*m_domain(NULL), m_domainSet(false),*/
	  m_serversArrayCache(NULL), m_credentials(NULL)	  
{}


Environment::~Environment()
{
	SAFE_FREE(m_basedir);
	SAFE_DELETE(m_servers);
	SAFE_DELETE(m_apps);
	//m_domain = NULL;		// Owned by cache
	m_credentials = NULL;	// Owned by cache
	SAFE_DELETE(m_serversArrayCache);
}


Expr *Environment::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "basedir") == 0) {
		return new Expr(m_basedir);
	}
	if(strcmp(name, "notes") == 0) {
		const char *notes = getNotes();
		return notes ? new Expr(notes) : NULL;
	}
	if((strcmp(name, "servers") == 0) || (strcmp(name, "physicals") == 0)) {
		if(m_serversArrayCache) {
			return new Expr(m_serversArrayCache);
		}
		DMArray *ret = new DMArray(false, false, true);
		ret->addRef();	// This prevents the array being deleted
		List<Server> *list = getServers();
		ListIterator<Server> iter(*list);
		for(Server *s = iter.first(); s; s = iter.next()) {
			ret->put(s->name(), new Variable(NULL, s));
		}
		m_serversArrayCache = ret;
		return new Expr(ret);
	}
	if(strcmp(name, "parent") == 0) {
		Domain *domain = getDomain();
		return domain ? new Expr(domain) : NULL;
	} else if (strcmp(name,"applications") == 0) {
		List<Application> *res = m_model.getApplications(this);
		DMArray *apps = new DMArray(true, false, false);
		apps->addRef();	// This prevents the array being deleted
		ListIterator<Application> iter(*res);
		for(Application *a = iter.first(); a; a = iter.next()) {
			apps->add(new Variable(NULL, a));
		}
		return new Expr(apps);
	}
	return Object::getAttribute(name, ctx);
}


//DMArray *Environment::getArrayAttribute(const char *name, class Context &ctx)
//{
//	if((strcmp(name, "servers") == 0) || (strcmp(name, "physicals") == 0)) {
//		if(m_serversArrayCache) {
//			return m_serversArrayCache;
//		}
//		DMArray *ret = new DMArray(false, false, true);
//		ret->addRef();	// This prevents the array being deleted
//		List<Server> *list = getServers();
//		ListIterator<Server> iter(*list);
//		for(Server *s = iter.first(); s; s = iter.next()) {
//			ret->put(s->name(), new Variable(NULL, s));
//		}
//		m_serversArrayCache = ret;
//		return ret;
//	}
//	return NULL;
//}


//IObject *Environment::getObjectAttribute(const char *name, class Context &ctx)
//{
//	if(strcmp(name, "parent") == 0) {
//		return getDomain();
//	}
//	return Object::getObjectAttribute(name, ctx);
//}


Scope *Environment::getVars()
{
	if(!m_vars) {
		m_vars = new Scope(ENVIRONMENT_SCOPE, this);
		m_vars->addRef();
		m_model.getVariablesForObject(*this, *m_vars);
	}

	return m_vars;
}

void Environment::setVar(const char *attname,Expr *attval)
{
	getVars()->set(attname,attval->toString());
	m_model.setVariableForObject(this,attname,attval);
}

//Domain *Environment::getDomain()
//{
//	if(!m_domainSet) {
//		m_domainSet = true;
//		m_model.getDomainForEnvironment(*this);
//	}
//	return m_domain;
//}


//void Environment::setDomain(Domain *domain)
//{
//	m_domainSet = true;
//	m_domain    = domain;
//}


List<Server> *Environment::getServers()
{
	if(!m_servers) {
		m_servers = new List<Server>();
		m_model.getServersForEnvironment(*this);
	}

	return m_servers;
}


List<Application> *Environment::getAllowedApplications()
{
	if(!m_apps) {
		m_apps = new List<Application>();
		m_model.getApplicationsAllowedInEnvironment(*this);
	}

	return m_apps;
}

Application *Environment::getDeployedApplication(Application &app)
{
	return m_model.getApplicationDeployedToEnvironment(*this, app);
}


Credentials *Environment::getCredentials()
{
	if(!m_credentials) {
		m_model.getCredentialsForEnvironment(*this);
	}

	return m_credentials;
}


void Environment::setCredentials(Credentials *creds)
{
	m_credentials = creds;
}


void Environment::add(Server *server)
{
	if(!m_servers) {
		m_servers = new List<Server>();
	}
	m_servers->add(server);
}


void Environment::add(Application *app)
{
	if(!m_apps) {
		m_apps = new List<Application>(true);	// TODO: This should not own the applications
	}
	m_apps->add(app);
}


//bool Environment::hasAccess(User *user)
//{
//	// Check that the parent domain allows access first
//	if(!m_domainSet) {
//		getDomain();
//	}
//	if(m_domain) {
//		if(!m_domain->hasAccess(user)) {
//			// No access to domain - disallow access
//			return false;
//		}
//	}
//
//	return Object::hasAccess(user);
//}


Server *Environment::server(const char *name)
{
	if(!m_servers) { getServers(); }
	ListIterator<Server> iter(*m_servers);
	for(Server *s = iter.first(); s; s = iter.next()) {
		if(strcmp(s->name(), name) == 0) {
			return s;
		}
	}
	return NULL;
}


class Application *Environment::application(const char *name)
{
	if(!m_apps) { getAllowedApplications(); }
	ListIterator<Application> iter(*m_apps);
	for(Application *a = iter.first(); a; a = iter.next()) {
		if(strcmp(a->name(), name) == 0) {
			return a;
		}
	}
	return NULL;
}


bool Environment::isAvailable()
{
	return m_model.isEnvironmentAvailable(*this);
}


void Environment::print(int ind)
{
	indent(ind);   printf("Environment: %s\n", m_name);
	indent(ind+1); printf("Credentials: ");
	if(m_credentials) {
		m_credentials->print(0);
	} else {
		printf("(none)");
	}
	printf("\n");
	printObject(ind+1);
	if(m_servers) {
		//m_servers->print(ind+1);
		ListIterator<Server> iter(*m_servers);
		for(Server *s = iter.first(); s; s = iter.next()) {
			s->print(ind+1);
		}
	}
	if(m_apps) {
		//m_apps->print(ind+1);
		ListIterator<Application> iter(*m_apps);
		for(Application *a = iter.first(); a; a = iter.next()) {
			a->print(ind+1);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// Domain
///////////////////////////////////////////////////////////////////////////////

Domain::Domain(Model &model, int id, const char *name)
	: Object(model, id, name), m_doms(NULL), m_envs(NULL), m_apps(NULL),
	  m_reps(NULL), m_nfys(NULL), m_dats(NULL), m_childrenCache(NULL), m_pos(0),
	  m_lifecycle(false)
{}


Domain::~Domain()
{
	SAFE_DELETE(m_doms);
	SAFE_DELETE(m_envs);
	SAFE_DELETE(m_apps);
	SAFE_DELETE(m_reps);
	SAFE_DELETE(m_nfys);
	SAFE_DELETE(m_dats);
	SAFE_DELETE(m_childrenCache);
}


List<Domain> *Domain::getSubdomains()
{
	if(!m_doms) {
		m_doms = new List<Domain>();
		m_model.getSubdomainsForDomain(this);
	}

	return m_doms;
}


List<Environment> *Domain::getEnvironments()
{
	if(!m_envs) {
		m_envs = new List<Environment>();
		m_model.getEnvironmentsForDomain(this);
	}

	return m_envs;
}

void Domain::setPosition(int pos)
{
	m_pos = pos;
}

void Domain::setLifecycle(bool lc)
{
	m_lifecycle = lc;
}

int Domain::getPosition()
{
	return m_pos;
}

bool Domain::getLifecycle()
{
	return m_lifecycle;
}


List<Application> *Domain::getApplications()
{
	if(!m_apps) {
		m_apps = new List<Application>();
		m_model.getApplicationsForDomain(this);
	}

	return m_apps;
}


List<Repository> *Domain::getRepositories()
{
	if(!m_reps) {
		m_reps = new List<Repository>();
		m_model.getRepositoriesForDomain(this);
	}

	return m_reps;
}


List<Notify> *Domain::getNotifys()
{
	if(!m_nfys) {
		m_nfys = new List<Notify>();
		m_model.getNotifysForDomain(this);
	}

	return m_nfys;
}


List<Datasource> *Domain::getDatasources()
{
	if(!m_dats) {
		m_dats = new List<Datasource>();
		m_model.getDatasourcesForDomain(this);
	}

	return m_dats;
}


void Domain::add(Domain *domain)
{
	if(!m_doms) {
		m_doms = new List<Domain>();
	}
	m_doms->add(domain);
}


void Domain::add(Environment *env)
{
	if(!m_envs) {
		m_envs = new List<Environment>();
	}
	m_envs->add(env);
}


void Domain::add(Application *app)
{
	if(!m_apps) {
		m_apps = new List<Application>();
	}
	m_apps->add(app);
}


void Domain::add(Repository *rep)
{
	if(!m_reps) {
		m_reps = new List<Repository>();
	}
	m_reps->add(rep);
}


void Domain::add(Notify *nfy)
{
	if(!m_nfys) {
		m_nfys = new List<Notify>();
	}
	m_nfys->add(nfy);
}


void Domain::add(Datasource *dat)
{
	if(!m_dats) {
		m_dats = new List<Datasource>();
	}
	m_dats->add(dat);
}


/*virtual*/ Expr *Domain::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "children") == 0 || strcmp(name,"subdomains") == 0) {
		if(!m_childrenCache) {
			m_childrenCache = new DMArray(false);
			m_childrenCache->addRef();	// This prevents the array being deleted
			List<Domain> *children = getSubdomains();
			ListIterator<Domain> iter(*children);
			for(Domain *dom = iter.first(); dom; dom = iter.next()) {
				m_childrenCache->put(dom->name(), new Variable(NULL, dom));
			}
		}
		return new Expr(m_childrenCache);
	} else if (strcmp(name,"applications")==0) {
		List<Application> *apps = getApplications();
		DMArray *applist = new DMArray(true,false,false);
		ListIterator<Application> iter(*apps);
		for(Application *app = iter.first(); app; app = iter.next()) {
			applist->add(new Variable(NULL,app));
		}
		return new Expr(applist);
	} else if (strcmp(name,"environments")==0) {
		List<Environment> *envs = getEnvironments();
		DMArray *envlist = new DMArray(true,false,false);
		ListIterator<Environment> iter(*envs);
		for(Environment *env = iter.first(); env; env = iter.next()) {
			envlist->add(new Variable(NULL,env));
		}
		return new Expr(envlist);
	} else if (strcmp(name,"lifecycle")==0) {
		return new Expr(m_lifecycle);
	}
	return Object::getAttribute(name, ctx);
}


void Domain::print(int ind)
{
	indent(ind); printf("Domain: %s\n", m_name);
	Object::printObject(ind+1);
	if(m_doms) {
		//m_domains->print(ind+1);
		ListIterator<Domain> iter(*m_doms);
		for(Domain *d = iter.first(); d; d = iter.next()) {
			d->print(ind+1);
		}
	}
	if(m_envs) {
		//m_envs->print(ind+1);
		ListIterator<Environment> iter(*m_envs);
		for(Environment *e = iter.first(); e; e = iter.next()) {
			e->print(ind+1);
		}
	}
	if(m_apps) {
		//m_apps->print(ind+1);
		ListIterator<Application> iter(*m_apps);
		for(Application *a = iter.first(); a; a = iter.next()) {
			a->print(ind+1);
		}
	}
	if(m_reps) {
		//m_reps->print(ind+1);
		ListIterator<Repository> iter(*m_reps);
		for(Repository *r = iter.first(); r; r = iter.next()) {
			r->print(ind+1);
		}
	}
	if(m_nfys) {
		//m_nfys->print(ind+1);
		ListIterator<Notify> iter(*m_nfys);
		for(Notify *n = iter.first(); n; n = iter.next()) {
			n->print(ind+1);
		}
	}
	if(m_dats) {
		//m_dats->print(ind+1);
		ListIterator<Datasource> iter(*m_dats);
		for(Datasource *d = iter.first(); d; d = iter.next()) {
			d->print(ind+1);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// ServerType
///////////////////////////////////////////////////////////////////////////////

ServerType::ServerType(
		int id, const char *hosttype,
		LINE_END_FORMAT lineends, const char *pathformat, const char *scriptext)
	: m_id(id), m_hosttype(DUP_NULL(hosttype)), m_lineends(lineends),
	  m_pathformat(DUP_NULL(pathformat)), m_scriptext(DUP_NULL(scriptext))
{}


ServerType::~ServerType()
{
	SAFE_FREE(m_hosttype);
	SAFE_FREE(m_pathformat);
	SAFE_FREE(m_scriptext);
}


PathNameImpl *ServerType::createPath(const char* path)
{
	if(!m_pathformat) {
		return NULL;
	}
	PathNameImplFactory *fact = PathNameImplRegistry::instance().getFactory(m_pathformat);
	if(!fact) {
		throw RuntimeError("Unknown path format provider '%s'", m_pathformat);
	}
	return fact->create(path);
}


///////////////////////////////////////////////////////////////////////////////
// Server
///////////////////////////////////////////////////////////////////////////////

Server::Server(
		Model &model, int id, const char *name, const char *hostname,
		const char *protocol, const char *basedir, ServerType *servertype, const char *uname)
	: Object(model, id, name), m_hostname(DUP_NULL(hostname)),
	  m_protocol(DUP_NULL(protocol)), m_basedir(DUP_NULL(basedir)),
	  m_credentials(NULL), m_servertype(servertype), m_uname(DUP_NULL(uname))
{
	m_PingTemplate = (NotifyTemplate *)0;
	m_MD5Template = (NotifyTemplate *)0;
	m_sshport = 0;
}


Server::~Server()
{
	SAFE_FREE(m_hostname);
	SAFE_FREE(m_protocol);
	SAFE_FREE(m_basedir);
	m_credentials = NULL;		// Owned by cache
	m_servertype = NULL;		// Owned by cache
}


/*virtual*/ const char *Server::hostname()
{
	return m_hostname ? m_hostname : m_name;
}


LINE_END_FORMAT Server::lineends()
{
	return m_servertype ? m_servertype->lineends() : LINE_ENDS_OFF;
}


PathNameImpl *Server::createPath(const char* path)
{
	return m_servertype ? m_servertype->createPath(path) : NULL;
}

void Server::setSSHPort(int portnum)
{
	m_sshport = portnum;
}

int Server::getSSHPort()
{
	return m_sshport?m_sshport:22;	// default
}


/*virtual*/ Expr *Server::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "hostname") == 0) {
		return new Expr(hostname());
	}
	else
	if(strcmp(name, "basedir") == 0) {
		return m_basedir ? new Expr(m_basedir) : NULL;
	}
	else
	if(strcmp(name, "type") == 0) {
		return m_servertype ? new Expr(m_servertype->hosttype()) : NULL;
	}
	else
	if(strcmp(name, "notes") == 0) {
		const char *notes = getNotes();
		return notes ? new Expr(notes) : NULL;
	}
	else
	if (strcmp(name,"credential") == 0) {
		IObject *cred = getCredentials();
		return cred ? new Expr(cred) : NULL;
	}
	else
	if (strcmp(name,"components") == 0) {
		List<Component> *res = m_model.getComponentsOnServer(this);
		DMArray *comps = new DMArray(true, false, false);
		comps->addRef();	// This prevents the array being deleted
		ListIterator<Component> iter(*res);
		for(Component *c = iter.first(); c; c = iter.next()) {
			comps->add(new Variable(NULL, c));
		}
		return new Expr(comps);
	}
	return Object::getAttribute(name, ctx);
}


//IObject *Server::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return Object::getObjectAttribute(name, ctx);
//}


Scope *Server::getVars()
{
	if(!m_vars) {
		m_vars = new Scope(SERVER_SCOPE, this);
		m_vars->set("RE_PHYSICAL_SERVER", name());
		m_vars->set("RE_PHYSICAL_HOSTNAME", hostname());
		m_vars->set("EXT",serverType()->scriptext());
		m_vars->addRef();
		m_model.getVariablesForObject(*this, *m_vars);
	}

	return m_vars;
}

void Server::setVar(const char *attname,Expr *attval)
{
	getVars()->set(attname,attval->toString());
	m_model.setVariableForObject(this,attname,attval);
}


Credentials *Server::getCredentials()
{
	if(!m_credentials) {
		m_model.getCredentialsForServer(*this);
	}

	return m_credentials;
}


void Server::setCredentials(Credentials *creds)
{
	m_credentials = creds;
}

void Server::setUname(const char *uname)
{
	m_model.setUname(*this,uname);
}


Component *Server::getDeployedComponent(Component &comp)
{
	return m_model.getComponentDeployedToServer(*this, comp);
}


void Server::print(int ind)
{
	indent(ind);   printf("Server: %s\n", m_name);
	indent(ind+1); printf("Hostname: %s\n", (m_hostname ? m_hostname : "(no hostname)"));
	indent(ind+1); printf("Credentials: ");
	if(m_credentials) {
		m_credentials->print(0);
	} else {
		printf("(none)");
	}
	printf("\n");
	printObject(ind+1);
}


///////////////////////////////////////////////////////////////////////////////
// Application
///////////////////////////////////////////////////////////////////////////////


Application::Application(Model &model, int id, const char *name)
	: Object(model, id, name), m_action(NULL), m_preAction(NULL),
	  m_postAction(NULL), m_actionsSet(false), m_components(NULL),
	  m_componentGraph(NULL), m_versionsCache(NULL),
	  m_successTemplate(NULL), m_failureTemplate(NULL), m_isRelease(false),
	  m_children(false), m_parents(false), m_completed(false), m_applicationGraph(NULL)
{}


Application::~Application()
{
	m_action = NULL;		// Owned by cache
	m_preAction = NULL;		// Owned by cache
	m_postAction = NULL;	// Owned by cache
	SAFE_DELETE(m_components);	// List only - objects owned by cache
	SAFE_DELETE(m_componentGraph);
}

class ApplicationVersion *Application::toApplicationVersion()
{
	return (ApplicationVersion *)this;
}


/*virtual*/ Expr *Application::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "branch") == 0) {
		char *BranchName = getBranchName();
		Expr *ret = new Expr(BranchName);
		SAFE_FREE(BranchName);
		return ret;
	} else if (strcmp(name,"components") == 0) {
		List<Component> *comps = getComponents();
		DMArray *res = new DMArray(true,false,false);
		ListIterator<Component> iter(*comps);
		for(Component *c = iter.first(); c; c = iter.next()) {
			res->add(new Variable(NULL,c));
		}
		return new Expr(res);
	} else if (strcmp(name,"defects") == 0 || strcmp(name,"requests") == 0) {
		List<Defect> *defects = getDefects();
		DMArray *res = new DMArray(true,false,false);
		ListIterator<Defect> iter(*defects);
		for(Defect *d = iter.first(); d; d = iter.next()) {
			res->add(new Variable(NULL,d));
		}
		return new Expr(res);
	} else if (strcmp(name,"release") == 0) {
		return new Expr(m_isRelease);
	} else if (strcmp(name,"applications") == 0) {
		if (m_isRelease) {
			ReleaseApplicationGraph *graph = getReleaseApplicationGraph();
			List<Application> *apps = graph->getApplications();
			DMArray *res = new DMArray(true,false,false);
			ListIterator<Application> iter(*apps);
			for(Application *a = iter.first(); a; a = iter.next()) {
				res->add(new Variable(NULL,a));
			}
			return new Expr(res);
		} else return new Expr((DMArray *)0);
	}
	
	
	return Object::getAttribute(name, ctx);
}


//IObject *Application::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return Object::getObjectAttribute(name, ctx);
//}

List<Defect> *Application::getDefects(bool onlyAppDefects /* = false */)
{
	return m_model.getDefectsForApp(*this,onlyAppDefects);
}

List<Domain> *Application::getApprovalDomains(bool Approved /*=true*/)
{
	return m_model.getDomainsApprovedForApp(*this,Approved);
}

List<Application> *Application::getChildren(bool AllDescendents /*=false*/)
{
	return m_model.getChildrenOfApplication(*this,AllDescendents);
}

bool Application::isRelease()
{
	return m_isRelease;
}

char *Application::getBranchName()
{
	return m_model.getBranchName(m_id);
}

void Application::setRelease(bool rel)
{
	m_isRelease = rel;
}


void Application::addChild(Application *av)
{
	m_children.add(av);
}

void Application::addParent(Application *av)
{
	m_parents.add(av);
}

void Application::setThread(class DMThread *t)
{
	m_thread = t;
}

void Application::markParentCompleted(Application *parent)
{
	debug1("Application %s marking parent %s as completed", name(), parent->name());
	m_parents.remove(parent);
	m_completed.add(parent);
	//dump();
}

void Application::markCompleted(Context &ctx)
{
	debug1("Marking application %s as completed", name());
	ListIterator<Application> iter(m_children);
	for(Application *a = iter.first(); a; a = iter.next()) {
		a->markParentCompleted(this);
		if(a->canDeploy()) {
			a->deploy(ctx);
		}
	}
}

bool Application::canDeploy()
{
	return (m_parents.size() == 0) ? true : false;
}

void Application::deploy(Context &ctx)
{
	if(m_thread) {
		debug1("Starting thread for application %s", name());
		ctx.writeToStdOut("INFO: Deploying application '%s'", name());
		m_thread->start();
	} else {
		debug1("ERROR: Application %s had no thread", name());
	}
}

void Application::dump()
{
	printf("  %s:\n", name());

	printf("    PARENTS:");
	ListIterator<Application> iter(m_parents);
	for(Application *a = iter.first(); a; a = iter.next()) {
		printf(" %s", a->name());
	}
	printf("\n");

	printf("    CHILDREN:");
	ListIterator<Application> iter2(m_children);
	for(Application *a = iter2.first(); a; a = iter2.next()) {
		printf(" %s", a->name());
	}
	printf("\n");

	printf("    COMPLETED:");
	ListIterator<Application> iter3(m_completed);
	for(Application *a = iter3.first(); a; a = iter3.next()) {
		printf(" %s", a->name());
	}
	printf("\n");

	for(Application *a = iter2.first(); a; a = iter2.next()) {
		a->dump();
	}
}


Scope *Application::getVars()
{
	if(!m_vars) {
		m_vars = new Scope(APPLICATION_SCOPE, this);
		m_vars->addRef();
		m_model.getVariablesForObject(*this, *m_vars);
	}

	return m_vars;
}

void Application::setVar(const char *attname,Expr *attval)
{
	getVars()->set(attname,attval->toString());
	m_model.setVariableForObject(this,attname,attval);
}


Action *Application::getCustomAction()
{
	if(!m_actionsSet) {
		m_model.getActionsForApplication(*this);
	}
	return m_action;
}


Action *Application::getPreAction()
{
	if(!m_actionsSet) {
		m_model.getActionsForApplication(*this);
	}
	return m_preAction;
}


Action *Application::getPostAction()
{
	if(!m_actionsSet) {
		m_model.getActionsForApplication(*this);
	}
	return m_postAction;
}

NotifyTemplate *Application::getSuccessTemplate(Context &ctx)
{
	if(!m_successTemplate) {
		m_successTemplate = m_model.getApplicationTemplate(ctx,*this,false);
	}
	return m_successTemplate;
}

NotifyTemplate *Application::getFailureTemplate(Context &ctx)
{
	if(!m_failureTemplate) {
		m_failureTemplate = m_model.getApplicationTemplate(ctx,*this,true);
	}
	return m_failureTemplate;
}

NotifyTemplate *Server::getPingTemplate(Context &ctx)
{
	if(!m_PingTemplate) {
		m_PingTemplate = m_model.getPingTemplate(ctx,*this);
	}
	return m_PingTemplate;
}

NotifyTemplate *Server::getMD5Template(Context &ctx)
{
	if(!m_MD5Template) {
		m_MD5Template = m_model.getMD5Template(ctx,*this);
	}
	return m_MD5Template;
}


/*private*/ void Application::setActions(Action *action, Action *preAction, Action *postAction)
{
	m_action = action;
	m_preAction = preAction;
	m_postAction = postAction;
	m_actionsSet = true;
}


List<Component> *Application::getComponents()
{
	if(!m_components) {
		m_components = new List<Component>(false);
		m_model.getComponentsForApplication(*this);
	}
	return m_components;
}


ApplicationComponentGraph *Application::getApplicationComponentGraph()
{
	if(!m_componentGraph) {
		m_componentGraph = m_model.getApplicationComponentGraph(*this);
	}
	return m_componentGraph;
}

ReleaseApplicationGraph *Application::getReleaseApplicationGraph()
{
	if(!m_applicationGraph) {
		m_applicationGraph = m_model.getReleaseApplicationGraph(*this);
	}
	return m_applicationGraph;
}


void Application::addComponent(Component *comp)
{
	if(!m_components) {
		m_components = new List<Component>(false);
	}
	m_components->add(comp);
}


List<Application> *Application::getParentList()
{
	List<Application> *ret = new List<Application>(false);
	ret->add(this);
	return ret;
}


ApplicationVersion *Application::getVersion(const char *name)
{
	ApplicationVersion *av = m_versionsCache ? m_versionsCache->get(name) : NULL;
	if(av) {
		return av;
	}
	av = m_model.getVersionOfApplication(*this, name);
	if(av) {
		if(!m_versionsCache) {
			m_versionsCache = new Hashtable<ApplicationVersion>();
		}
		m_versionsCache->put(name, av);
	}
	return av;
}


bool Application::isAvailable(Environment &env)
{
	return m_model.isApplicationAvailable(*this, env);
}


bool Application::approve(class Domain &tgtDomain, bool approve, const char *note)
{
	return m_model.approveApplication(*this, tgtDomain, approve, note);
}


bool Application::move(class Domain &tgtDomain, const char *note)
{
	return m_model.moveApplication(*this, tgtDomain, note);
}

bool Application::request(class Task &linkedTask, const char *note)
{
	return m_model.request(*this, linkedTask, note);
}

void Application::recordDeployedToEnv(class DM &dm, class Environment &env, bool success)
{
	m_model.recordAppInEnv(dm, *this, env, success);
}


ApplicationVersion *Application::newVersion(Domain &tgtDomain, Application *predecessor)
{
	return m_model.newVersionOfApplication(*this, tgtDomain, predecessor);
}


ApplicationVersion *Application::getLatestVersion(char *branch /* = (char *)0 */)
{
	return m_model.getLatestVersionOfApplication(*this,branch);
}


void Application::print(int ind)
{
	indent(ind); printf("Application: %s\n", m_name);
	printObject(ind+1);
}


///////////////////////////////////////////////////////////////////////////////
// ApplicationVersion
///////////////////////////////////////////////////////////////////////////////

ApplicationVersion::ApplicationVersion(
		class Model &model, int id, const char *name,
		const char *version, Application *parent)
	: Application(model, id, name), m_version(DUP_NULL(version)),
	  m_parent(parent), m_predecessor(NULL), m_predecessorSet(false),
	  m_successors(NULL), m_approvalsArrayCache(NULL)
{}


ApplicationVersion::~ApplicationVersion()
{
	SAFE_FREE(m_version);
	m_parent = NULL;		// Owned by cache
	m_predecessor = NULL;	// Owned by cache
	SAFE_DELETE(m_successors);
}


ApplicationVersion *ApplicationVersion::getPredecessor()
{
	if (kind() == OBJ_KIND_APPVERSION) {
		if(!m_predecessorSet) {
			m_model.getPredecessorForApplicationVersion(*this);
		}
		return m_predecessor;
	} else return NULL;	// OBJ_KIND_APPLICATION has no predecessors
}


/*private*/ void ApplicationVersion::setPredecessor(ApplicationVersion *predecessor)
{
	m_predecessor = predecessor;
	m_predecessorSet = true;
}


List<ApplicationVersion> *ApplicationVersion::getSuccessors()
{
	if(!m_successors) {
		m_successors = new List<ApplicationVersion>(false);
		m_model.getSuccessorsForApplicationVersion(*this);
	}
	return m_successors;
}


/*private*/ void ApplicationVersion::addSuccessor(ApplicationVersion *successor)
{
	if(!m_successors) {
		m_successors = new List<ApplicationVersion>(false);
	}
	m_successors->add(successor);
}


List<Application> *ApplicationVersion::getParentList()
{
	List<Application> *ret =  m_parent ? m_parent->getParentList() : NULL;
	if(!ret) {
		ret = new List<Application>(false);
	}
	ret->add(this);
	return ret;
}


DMArray *ApplicationVersion::getApprovals()
{
	if(!m_approvalsArrayCache) {
		m_approvalsArrayCache =  m_model.getApprovalsForApplicationVersion(*this);
		m_approvalsArrayCache->addRef();		// This prevents the array being deleted
	}
	return m_approvalsArrayCache;
}


Expr *ApplicationVersion::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "version") == 0) {
		return new Expr(m_version);
	} else if(strcmp(name, "parent") == 0) {
		return new Expr(m_parent);
	} else if(strcmp(name, "predecessor") == 0) {
		return new Expr(getPredecessor());
	} else if(strcmp(name, "approvals") == 0) {
		return new Expr(getApprovals());
	} else if (strcmp(name, "components") == 0) {
		DMArray *ret = new DMArray(false, false, true);
		ret->addRef();	// This prevents the array being deleted
		List<Component> *list = getComponents();
		ListIterator<Component> iter(*list);
		for(Component *c = iter.first(); c; c = iter.next()) {
			ret->put(c->name(), new Variable(NULL, c));
		}
		return new Expr(ret);
	} else if (strcmp(name,"defects") == 0 || strcmp(name,"requests") == 0) {
		List<Defect> *defects = getDefects();
		DMArray *res = new DMArray(true,false,false);
		ListIterator<Defect> iter(*defects);
		for(Defect *d = iter.first(); d; d = iter.next()) {
			res->add(new Variable(NULL,d));
		}
		return new Expr(res);
	}
	return Application::getAttribute(name, ctx);
}


//IObject *ApplicationVersion::getObjectAttribute(const char *name, class Context &ctx)
//{
//	if(strcmp(name, "parent") == 0) {
//		return m_parent;
//	}
//	return Application::getObjectAttribute(name, ctx);
//}


ApplicationVersion *ApplicationVersion::operator_plus(int incr, Context &ctx) const
{
	Scope *avloopScope = ctx.stack().getScope(AVLOOP_SCOPE);
	IObject *avloop = avloopScope ? avloopScope->getScopeObject() : NULL;

	if(avloop) {
		debug1("ApplicationVersion %d + %d (in avloop)", m_id, incr);
		ExprPtr elast = avloop->getAttribute("last", ctx);
		ObjectReference *oref = elast ? elast->toObjectReference() : NULL;
		Object *o = oref ? oref->toObject() : NULL;
		if(o && (o->kind() == OBJ_KIND_APPVERSION)) {
			ApplicationVersion *last = (ApplicationVersion*) o;
			List<ApplicationVersion> path(false);
			bool found = false;
			for(ApplicationVersion *av = last; av; av = av->getPredecessor()) {
				path.prepend(av);
				if(av == this) {
					found = true;
					break;
				}
			}
			if(!found) {
				debug1("Application version %d not on current avloop path", m_id);
				throw RuntimeError(ctx.stack(), "Application version %d not on current avloop path", m_id);
			}
			ListIterator<ApplicationVersion> iter(path);
			for(ApplicationVersion *av = iter.first(); av; incr--) {
				if(incr <= 0) {
					debug1("ApplicationVersion operator+ returning %d", av->id());
					return av;
				}
				av = iter.next();
			}
		}
	}

	debug1("ApplicationVersion %d + %d", m_id, incr);
	if(incr > 0) {
		for(ApplicationVersion *av = (ApplicationVersion*) this; av; incr--) {
			if(incr <= 0) {
				debug1("ApplicationVersion operator+ returning %d", av->id());
				return av;
			}
			List<ApplicationVersion> *succs = av->getSuccessors();
			if(!succs || (succs->size() == 0)) break;
			if(succs->size() > 1) {
				debug1("Successor to application version %d is ambiguous", av->id());
				throw RuntimeError(ctx.stack(), "Successor to application version %d is ambiguous", av->id());
			}
			ListIterator<ApplicationVersion> iter(*succs);
			av = iter.first();
		}
	}
	return NULL;
}


ApplicationVersion *ApplicationVersion::operator_minus(int decr, Context &ctx) const
{
	debug1("ApplicationVersion %d - %d", m_id, decr);
	if(decr > 0) {
		for(ApplicationVersion *av = (ApplicationVersion*) this; av; decr--) {
			if(decr <= 0) {
				debug1("ApplicationVersion operator- returning %d", av->id());
				return av;
			}
			av = av->getPredecessor();
		}
	}
	return NULL;
}



/*
void ApplicationVersion::alterVars(class DMArray *attrs)
{
	m_model.alterApplicationVersionVars(*this, attrs);
}
*/


///////////////////////////////////////////////////////////////////////////////
// ComponentItem
///////////////////////////////////////////////////////////////////////////////

ComponentItem::ComponentItem(
		Model &model, int id, const char *name, Component &parent, int repoid,
		const char *target, ComponentFilter rollup, ComponentFilter rollback
	)
	: /*m_model(model), m_id(id),*/ Object(model, id, name), m_parent(parent), m_repoid(repoid),
	  m_target(DUP_NULL(target)), m_rollup(rollup), m_rollback(rollback),
	  m_repo(NULL), m_props(NULL)
{}


ComponentItem::~ComponentItem()
{
	SAFE_FREE(m_target);
	m_repo = NULL;	// Owned by cache
	SAFE_DELETE(m_props);
}


/*private*/ void ComponentItem::setRepository(Repository *repo)
{
	m_repo = repo;
}


Repository *ComponentItem::getRepository()
{
	if(!m_repo) {
		m_model.getRepositoryForComponentItem(*this);
	}
	return m_repo;
}


Property *ComponentItem::getProperty(const char *name)
{
	return m_props ? m_props->get(name) : NULL;
}


void ComponentItem::setProperty(Property *prop)
{
	if(!m_props) {
		m_props = new Hashtable<Property>(false, true);
	}
	m_props->put(prop->name(), prop);
}


StmtList *ComponentItem::getPropertiesAsArgs()
{
	StmtList *ret = new StmtList();
	if(m_props) {
		AutoPtr<StringList> keys = m_props->keys();
		StringListIterator iter(*keys);
		for(const char *name = iter.first(); name; name = iter.next()) {
			Property *p = m_props->get(name);
			if(p) {
				ret->add(new Stmt(strdup(name), new Node(NODE_STR, DUP_NULL(p->value()))));
			}
		}
	}

	return ret;
}


Expr *ComponentItem::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "repository") == 0) {
		Repository *repo = getRepository();
		return (repo ? new Expr(repo->name()) : NULL);
	} else if(strcmp(name, "target") == 0) {
		return new Expr(m_target);
	} else if(strcmp(name, "parent") == 0) {
		return new Expr(m_parent.toObject());
	} else if(strcmp(name, "rollup") == 0) {
		return new Expr(m_rollup != OFF);
	} else if(strcmp(name, "rollback") == 0) {
		return new Expr(m_rollback != OFF);
	} else if (strcmp(name,"properties") == 0) {
		StringList *keys = m_props->keys();
		DMArray *ret = new DMArray(false);
		StringListIterator iter(*keys);
		for(const char *k = iter.first(); k; k = iter.next()) {
			Property *p = m_props->get(k);
			Variable *V = new Variable(NULL,p?p->value():"");
			ret->put(k,V);
		}
		return new Expr(ret);

	}
	return Object::getAttribute(name, ctx);
}


//DMArray *ComponentItem::getArrayAttribute(const char *name, Context &ctx)
//{
//	return NULL;
//}


//IObject *ComponentItem::getObjectAttribute(const char *name, Context &ctx)
//{
//	return NULL;
//}


void ComponentItem::print(int ind)
{
	indent(ind); printf("ComponentItem: %s\n", m_name);
	printObject(ind+1);
}


///////////////////////////////////////////////////////////////////////////////
// Defect
///////////////////////////////////////////////////////////////////////////////
Defect::Defect(Model &model,char *bugid,char *title,char *status,char *api_url,char *html_url)
	: Object(model,0,bugid), m_title(DUP_NULL(title)),m_status(DUP_NULL(status)), m_api_url(DUP_NULL(api_url)), m_html_url(DUP_NULL(html_url))
{}

Defect::~Defect()
{
	SAFE_FREE(m_title);
	SAFE_FREE(m_status);
	SAFE_FREE(m_api_url);
	SAFE_FREE(m_html_url);
}

void Defect::print(int indent)
{
	printf("Defect bugid=%s title=%s status=%s\n",Object::name(),m_title,m_status);
}

Expr *Defect::getAttribute(const char *name, class Context &ctx)
{
	if (strcmp(name, "name") == 0) {
		return new Expr(m_title);
	} else if (strcmp(name,"status") == 0) {
		return new Expr(m_status);
	} else if (strcmp(name,"id") == 0) {
		return Object::getAttribute("name", ctx);
	} else if(strcmp(name, "html_url") == 0) {
		return new Expr(m_html_url);
	} else if(strcmp(name, "api_url") == 0) {
		return new Expr(m_api_url);
	} else if (strcmp(name,"server") == 0) {
		// fetch the bug server name from the api_url
		// Should be https://blah/ or http://blah/ so
		// break at the 3rd /
		Expr *ret = new Expr("");
		char *server = (char *)0;
		if (m_api_url) server = strdup(m_api_url);
		else
		if (m_html_url) server = strdup(m_html_url);
		if (server) {
			int c=3;
			char *p=server;
			char *sp=server;
			while (*p) {
				if (*p=='/') {
					c--;
					if (c==1) sp=(p+1);
					if (c==0) break;
				}
				p++;
			}
			*p='\0';
			ret = new Expr(sp);
			free(server);
		} else {
			ret = new Expr("");
		}
		return ret;
	}
	return Object::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// Component
///////////////////////////////////////////////////////////////////////////////

Component::Component(
		Model &model, int id, const char *name, const char *basedir,
		ComponentFilter rollup, ComponentFilter rollback,
		bool filterItems, bool deployAlways, bool deploySequentially, 
		int parentid, int predecessorid, int datasourceid, int buildid
	)
	: Object(model, id, name), m_basedir(DUP_NULL(basedir)),
	  m_rollup(rollup), m_rollback(rollback), m_filterItems(filterItems),
	  m_deployAlways(deployAlways), m_items(NULL), m_itemsArrayCache(NULL),
	  m_serversArrayCache(NULL), m_action(NULL), m_preAction(NULL),
	  m_postAction(NULL), m_actionsSet(false), m_parentid(parentid),
	  m_predecessorid(predecessorid), m_deploySequentially(deploySequentially),
	  m_datasourceid(datasourceid), m_buildid(buildid)
{}


Component::~Component()
{
	SAFE_DELETE(m_items);
	SAFE_FREE(m_basedir);
	SAFE_DELETE(m_itemsArrayCache);
	SAFE_DELETE(m_serversArrayCache);
	m_action = NULL;			// owned by cache
	m_preAction = NULL;			// owned by cache
	m_postAction = NULL;		// owned by cache
}


Expr *Component::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "items") == 0) {
		if(!m_itemsArrayCache) {
			getItems();
			m_itemsArrayCache = new DMArray(true, false, true);
			m_itemsArrayCache->addRef();	// This prevents the array being deleted
			ListIterator<ComponentItem> iter(*m_items);
			for(ComponentItem *ci = iter.first(); ci; ci = iter.next()) {
				m_itemsArrayCache->add(new Variable(NULL, ci));
			}
		}
		return new Expr(m_itemsArrayCache);
	} else if(strcmp(name, "servers") == 0) {
		if(!m_serversArrayCache) {
			m_serversArrayCache = new DMArray(false, false, true);
			m_serversArrayCache->addRef();	// This prevents the array being deleted
			Environment *env = ctx.dm().getTargetEnvironment();
			List<Server> *servers = m_model.getAllServersForComponent(*this, env);
			ListIterator<Server> iter(*servers);
			for(Server *s = iter.first(); s; s = iter.next()) {
				m_serversArrayCache->put(s->name(), new Variable(NULL, s));
			}
			SAFE_DELETE(servers);
		}
		return new Expr(m_serversArrayCache);
	} else if (strcmp(name,"defects") == 0 || strcmp(name,"requests") == 0) {
		List<Defect> *defects = getDefects();
		DMArray *res = new DMArray(true,false,false);
		ListIterator<Defect> iter(*defects);
		for(Defect *d = iter.first(); d; d = iter.next()) {
			res->add(new Variable(NULL,d));
		}
		return new Expr(res);
	} else if(strcmp(name, "rollup") == 0) {
		return new Expr(m_rollup != OFF);
	} else if(strcmp(name, "rollback") == 0) {
		return new Expr(m_rollback != OFF);
	} else if (strcmp(name,"lastbuild") == 0) {
		return new Expr(m_buildid);
	}
	return Object::getAttribute(name, ctx);
}


//DMArray *Component::getArrayAttribute(const char *name, class Context &ctx)
//{
//	if(strcmp(name, "items") == 0) {
//		if(!m_itemsArrayCache) {
//			getItems();
//			m_itemsArrayCache = new DMArray(true, false, true);
//			m_itemsArrayCache->addRef();	// This prevents the array being deleted
//			ListIterator<ComponentItem> iter(*m_items);
//			for(ComponentItem *ci = iter.first(); ci; ci = iter.next()) {
//				m_itemsArrayCache->add(new Variable(NULL, ci));
//			}
//		}
//		return m_itemsArrayCache;
//	} else if(strcmp(name, "servers") == 0) {
//		if(!m_serversArrayCache) {
//			List<Server> *servers = m_model.getAllServersForComponent(*this);
//			m_serversArrayCache = new DMArray(false, false, true);
//			m_serversArrayCache->addRef();	// This prevents the array being deleted
//			ListIterator<Server> iter(*servers);
//			for(Server *s = iter.first(); s; s = iter.next()) {
//				m_serversArrayCache->put(s->name(), new Variable(NULL, s));
//			}
//			SAFE_DELETE(servers);
//		}
//		return m_serversArrayCache;
//	}
//	return Object::getArrayAttribute(name, ctx);
//}


//IObject *Component::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return Object::getObjectAttribute(name, ctx);
//}

List<Defect> *Component::getDefects()
{
	return m_model.getDefectsForComp(*this);
}

Scope *Component::getVars()
{
	if(!m_vars) {
		m_vars = new Scope(COMPONENT_SCOPE, this);
		m_vars->addRef();
		m_model.getVariablesForObject(*this, *m_vars);
	}

	return m_vars;
}


List<Server> *Component::getServerSubset(class Context &ctx)
{
	return m_model.getServerSubsetForComponent(*this, ctx.servers());
}


List<ComponentItem> *Component::getItems()
{
	if(!m_items) {
		m_items = new List<ComponentItem>(true);
		m_model.getItemsForComponent(*this);
	}
	return m_items;
}


void Component::add(ComponentItem *item)
{
	if(!m_items) {
		m_items = new List<ComponentItem>(true);
	}
	m_items->add(item);
}


Action *Component::getCustomAction()
{
	if(!m_actionsSet) {
		m_model.getActionsForComponent(*this);
	}
	return m_action;
}


Action *Component::getPreAction()
{
	if(!m_actionsSet) {
		m_model.getActionsForComponent(*this);
	}
	return m_preAction;
}


Action *Component::getPostAction()
{
	if(!m_actionsSet) {
		m_model.getActionsForComponent(*this);
	}
	return m_postAction;
}


/*private*/ void Component::setActions(Action *action, Action *preAction, Action *postAction)
{
	m_action = action;
	m_preAction = preAction;
	m_postAction = postAction;
	m_actionsSet = true;
}


void Component::recordDeployedToServer(class DM &dm, class Server &server, bool success)
{
	m_model.recordCompOnServ(dm, *this, server, success);
}


void Component::print(int ind)
{
	indent(ind); printf("Component: %s\n", m_name);
	printObject(ind+1);
}

StringList *Component::getFilesForLastDeployment(class Server &s,int deployid)
{
	return m_model.getFilesForLastDeployment(*this,s,deployid);
}

///////////////////////////////////////////////////////////////////////////////
// ApplicationComponent
///////////////////////////////////////////////////////////////////////////////

ApplicationComponent::ApplicationComponent(class Component &comp)
	: m_comp(comp), m_children(false), m_parents(false),
	  m_completed(false), m_thread(NULL)
{}


/*virtual*/ ApplicationComponent::~ApplicationComponent()
{
	m_thread = NULL;		// owned by thread list
}


void ApplicationComponent::addChild(ApplicationComponent *ac)
{
	m_children.add(ac);
}


void ApplicationComponent::addParent(ApplicationComponent *ac)
{
	m_parents.add(ac);
}


void ApplicationComponent::setThread(class DMThread *t)
{
	m_thread = t;
}


void ApplicationComponent::markCompleted(Context &ctx,int retcode)
{
	debug1("Marking component %s as completed", m_comp.name());
	ListIterator<ApplicationComponent> iter(m_children);
	for(ApplicationComponent *ac = iter.first(); ac; ac = iter.next()) {
		ac->markParentCompleted(this);
		if (retcode == 0) {
			if(ac->canDeploy()) {
				ac->deploy(ctx);
			}
		} else {
			// Need to terminate child threads
			ac->stop(ctx);
		}
	}
}


void ApplicationComponent::markParentCompleted(ApplicationComponent *parent)
{
	debug1("Component %s marking parent %s as completed", m_comp.name(), parent->m_comp.name());
	m_parents.remove(parent);
	m_completed.add(parent);
	dump();
}


bool ApplicationComponent::canDeploy()
{
	return (m_parents.size() == 0) ? true : false;
}

void ApplicationComponent::stop(Context &ctx)
{
	printf("ApplicationComponent::stop\n");
	// Loop through children recursively and terminate
	ListIterator<ApplicationComponent> iter(m_children);
	for(ApplicationComponent *ac = iter.first(); ac; ac = iter.next()) {
		ac->stop(ctx);
	}
	// drop to here when no more children
	m_thread->stop();
}

void ApplicationComponent::deploy(Context &ctx)
{
	if(m_thread) {
		debug1("Starting thread for component %s", m_comp.name());
		m_thread->start();
	} else {
		debug1("ERROR: Component %s had no thread", m_comp.name());
	}
}


void ApplicationComponent::dump()
{
	printf("  %s:\n", m_comp.name());

	printf("    PARENTS:");
	ListIterator<ApplicationComponent> iter(m_parents);
	for(ApplicationComponent *ac = iter.first(); ac; ac = iter.next()) {
		printf(" %s", ac->m_comp.name());
	}
	printf("\n");

	printf("    CHILDREN:");
	ListIterator<ApplicationComponent> iter2(m_children);
	for(ApplicationComponent *ac = iter2.first(); ac; ac = iter2.next()) {
		printf(" %s", ac->m_comp.name());
	}
	printf("\n");

	printf("    COMPLETED:");
	ListIterator<ApplicationComponent> iter3(m_completed);
	for(ApplicationComponent *ac = iter3.first(); ac; ac = iter3.next()) {
		printf(" %s", ac->m_comp.name());
	}
	printf("\n");

	for(ApplicationComponent *ac = iter2.first(); ac; ac = iter2.next()) {
		ac->dump();
	}
}

void ApplicationComponent::dump(Context &ctx)
{
	ctx.dm().writeToStdOut("  %s:\n", m_comp.name());

	ctx.dm().writeToStdOut("    PARENTS:");
	ListIterator<ApplicationComponent> iter(m_parents);
	for(ApplicationComponent *ac = iter.first(); ac; ac = iter.next()) {
		ctx.dm().writeToStdOut(" %s", ac->m_comp.name());
	}
	ctx.dm().writeToStdOut("\n");

	ctx.dm().writeToStdOut("    CHILDREN:");
	ListIterator<ApplicationComponent> iter2(m_children);
	for(ApplicationComponent *ac = iter2.first(); ac; ac = iter2.next()) {
		ctx.dm().writeToStdOut(" %s", ac->m_comp.name());
	}
	ctx.dm().writeToStdOut("\n");

	ctx.dm().writeToStdOut("    COMPLETED:");
	ListIterator<ApplicationComponent> iter3(m_completed);
	for(ApplicationComponent *ac = iter3.first(); ac; ac = iter3.next()) {
		ctx.dm().writeToStdOut(" %s", ac->m_comp.name());
	}
	ctx.dm().writeToStdOut("\n");

	for(ApplicationComponent *ac = iter2.first(); ac; ac = iter2.next()) {
		ac->dump(ctx);
	}
}


///////////////////////////////////////////////////////////////////////////////
// ApplicationComponentThread
///////////////////////////////////////////////////////////////////////////////

class ApplicationComponentThread : public DMThread
{
private:
	ApplicationComponent &m_appComp;
	ScopeStack *m_stack;

public:
	ApplicationComponentThread(
		DMThreadList &threadList, ApplicationComponent &appComp,
		Context &ctx, ScopeStack *stack);
	~ApplicationComponentThread();

	int execute(class Context &ctx);
};


ApplicationComponentThread::ApplicationComponentThread(
		DMThreadList &threadList, ApplicationComponent &appComp,
		Context &ctx, ScopeStack *stack
	)
	: DMThread(threadList, ctx.newThreadCopy(*stack)),
	  m_appComp(appComp), m_stack(stack)
{
	debug1("CONSTRUCT of ApplicationComponentThread ctx.isRollback=%s",ctx.isRollback()?"true":"false");
}


ApplicationComponentThread::~ApplicationComponentThread()
{
	debug3("~ApplicationComponentThread for thread %d", m_id);
	SAFE_DELETE(m_ctx);
	SAFE_DELETE(m_stack);
}


int ApplicationComponentThread::execute(Context &ctx)
{
	debug1("ApplicationComponentThread::execute()");
	Component &comp = m_appComp.component();
	// ctx.writeToStdOut("INFO: Deploying component '%s'", comp.name());

	ctx.stack().push(comp.getVars());

	debug3("in ApplicationComponentThread::execute - ctx.isRollback()=%s",ctx.isRollback()?"true":"false");

	int ret = 0;

	//m_stack.dump();

	// Build a pretend deploy statement - the component will come from the stack
	yylloc_param loc;
	loc.file_num     = loc.frag_num    = 0;
	loc.first_column = loc.last_column = 0;
	loc.first_line   = loc.last_line   = 0;

	StmtList *args = new StmtList();
	ExtendedStmt deployComp(loc, strdup("deploy"), args, NULL, NULL);

	try {
		// ctx.dm().writeToLogFile("Deploying component '%s'", comp.name());
		AutoPtr<List<Server> > servers = comp.getServerSubset(ctx);

		Context newctx(ctx, *servers);
		deployComp.execute(newctx);
	} catch(ReturnException &/*e*/) {
		debug3("RETURN_EXCEPTION");
		// Normal return via return statement
	} catch(DMException &e) {
		debug3("DM_EXCEPTION");
		ctx.dm().writeToLogFile("Caught exception");
		e.print(ctx);
		if(e.getStacktrace()) {
			ctx.writeToStdErr(e.getStacktrace());
		}
		ctx.dm().getAudit().setStatus(e.getMessage());
		ret = 2;
	} catch(...) {
		debug3("UNCAUGHT EXCEPTION");
		ctx.dm().writeToLogFile("Unhandled exception");
		ctx.writeToStdErr("FATAL ERROR: Unhandled exception");
		ctx.dm().getAudit().setStatus("Unhandled exception");
		ret = 2;
	}

	try {
		ctx.stack().pop(COMPONENT_SCOPE);
	} catch(DMException &e) {
		ctx.dm().writeToLogFile("Caught exception (2)");
		e.print(ctx);
		ret = 3;
	} catch(...) {
		ctx.writeToStdErr("FATAL ERROR: Unhandled exception");
		ctx.dm().writeToLogFile("Unhandled exception (2)");
		ret = 3;
	}

	lock(__LINE__,__FILE__,"graph"); // lock on the global mutex for access to the graph
	m_appComp.markCompleted(ctx,ret);
	unlock(__LINE__,__FILE__,"graph"); // unlock the graph
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseApplicationThread
///////////////////////////////////////////////////////////////////////////////

class ReleaseApplicationThread : public DMThread
{
private:
	Application &m_app;
	ScopeStack *m_stack;

public:
	ReleaseApplicationThread(DMThreadList &threadList, Application &app, Context &ctx, ScopeStack *stack);
	~ReleaseApplicationThread();

	int execute(class Context &ctx);
};


ReleaseApplicationThread::ReleaseApplicationThread(DMThreadList &threadList, Application &app, Context &ctx, ScopeStack *stack)
	: DMThread(threadList, ctx.newThreadCopy(*stack)),
	  m_app(app), m_stack(stack)
{}


ReleaseApplicationThread::~ReleaseApplicationThread()
{
	debug3("~ReleaseApplicationThread for thread %d", m_id);
	//SAFE_DELETE(m_ctx);
	//SAFE_DELETE(m_stack);
}


int ReleaseApplicationThread::execute(Context &ctx)
{
	try {
		ctx.dm().doDeployment(m_app,&ctx);
	}
	catch(...) {
		debug3("ReleaseApplicationThread caught exception from doDeployment");
		lock(__LINE__,__FILE__); // lock on the global mutex for access to the graph
		m_app.markCompleted(ctx);
		unlock(__LINE__,__FILE__); // unlock the graph
		debug3("Rethrowing");
		throw;
	}
	lock(__LINE__,__FILE__); // lock on the global mutex for access to the graph
	m_app.markCompleted(ctx);
	unlock(__LINE__,__FILE__); // unlock the graph
//	printf("RELEASE : returning from ReleaseApplicationThread::execute()\n");
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// ApplicationComponentGraph
///////////////////////////////////////////////////////////////////////////////

ApplicationComponentGraph::ApplicationComponentGraph()
	: m_appComps(true), m_starts(false)
{}


/*virtual*/ ApplicationComponentGraph::~ApplicationComponentGraph()
{}


void ApplicationComponentGraph::addComponent(Component &comp)
{
	debug1("Add component %d",comp.id());
	m_appComps.put(comp.id(), new ApplicationComponent(comp));
}


// ===
void ApplicationComponentGraph::addFlow(int compfromid, int comptoid)
{
	debug1("Add flow %d to %d",compfromid,comptoid);
	ApplicationComponent *compto = m_appComps.get(comptoid);
	if(!compto) {
		debug1("WARNING: Failed to find component %d", comptoid);
		return;
	}

	if(compfromid == 0) {
		// Flow from start
		m_starts.add(compto);
	} else {
		// Flow between components
		ApplicationComponent *compfrom = m_appComps.get(compfromid);
		compfrom->addChild(compto);
		compto->addParent(compfrom);
	}
}


int ApplicationComponentGraph::deploy(Context &ctx)
{
	AutoPtr<DMThreadList> threads = new DMThreadList(
		ctx.dm().getEngineConfig().getThreadLimit());

	AutoPtr<IntList> keys = m_appComps.keys();
	IntListIterator alliter(*keys);
	for(int compid = alliter.first(); alliter.more(); compid = alliter.next()) {
		ApplicationComponent *ac = m_appComps.get(compid);
		if(ac) {
			debug1("Creating thread for component %s", ac->component().name());
			ScopeStack *threadStack = new ScopeStack(ctx.stack());
			ApplicationComponentThread *t = new ApplicationComponentThread(*threads, *ac, ctx, threadStack);
			ac->setThread(t);
		}
	}

	debug1("m_starts size=%d",m_starts.size());
	ListIterator<ApplicationComponent> iter(m_starts);
	for(ApplicationComponent *ac = iter.first(); ac; ac = iter.next()) {
		debug1("Calling deploy for application component");
		ac->deploy(ctx);
	}
	ctx.dm().writeToLogFile("Waiting for parallel threads to finish");
	int exitCode = threads->waitForAll();
	ctx.stack().setGlobal("?", exitCode);
	return exitCode;
}


void ApplicationComponentGraph::dump()
{
	printf("START:\n");
	ListIterator<ApplicationComponent> iter(m_starts);
	for(ApplicationComponent *ac = iter.first(); ac; ac = iter.next()) {
		ac->dump();
	}
}

void ApplicationComponentGraph::dump(Context &ctx)
{
	ctx.dm().writeToStdOut("START:\n");
	ListIterator<ApplicationComponent> iter(m_starts);
	for(ApplicationComponent *ac = iter.first(); ac; ac = iter.next()) {
		ac->dump(ctx);
	}
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseApplicationGraph
///////////////////////////////////////////////////////////////////////////////

ReleaseApplicationGraph::ReleaseApplicationGraph()
	: m_apps(true), m_starts(false)
{
	m_apps.clear();
}


/*virtual*/ ReleaseApplicationGraph::~ReleaseApplicationGraph()
{}


void ReleaseApplicationGraph::addApplication(Application *app)
{
	m_apps.put(app->id(),app);
}

List<Application> *ReleaseApplicationGraph::getApplications()
{
	List<Application> *ret = new List<Application>(false);
	AutoPtr<IntList> keys = m_apps.keys();
	IntListIterator alliter(*keys);
	for(int appid = alliter.first(); alliter.more(); appid = alliter.next()) {
		ret->add(m_apps.get(appid));
	}
	return ret;
}


// ===
void ReleaseApplicationGraph::addFlow(int appfromid, int apptoid)
{
	Application *appto = m_apps.get(apptoid);
	if(!appto) {
		debug1("WARNING: Failed to find application %d", apptoid);
		return;
	}

	if(appfromid == 0) {
		// Flow from start
		m_starts.add(appto);
	} else {
		// Flow between applications
		Application *appfrom = m_apps.get(appfromid);
		appfrom->addChild(appto);
		appto->addParent(appfrom);
	}
}


int ReleaseApplicationGraph::deploy(Context &ctx)
{
	AutoPtr<DMThreadList> threads = new DMThreadList(
		ctx.dm().getEngineConfig().getThreadLimit());

	AutoPtr<IntList> keys = m_apps.keys();
	IntListIterator alliter(*keys);
	for(int appid = alliter.first(); alliter.more(); appid = alliter.next()) {
		Application *a = m_apps.get(appid);
		if(a) {
			ScopeStack *threadStack = new ScopeStack(ctx.stack());
			ReleaseApplicationThread *t = new ReleaseApplicationThread(*threads, *a, ctx, threadStack);
			a->setThread(t);
		}
	}

	ListIterator<Application> iter(m_starts);
	for(Application *a = iter.first(); a; a = iter.next()) {
		a->deploy(ctx);
	}

	int exitCode = threads->waitForAll();
	//ctx.stack().setGlobal("?", exitCode);
	return exitCode;
}


void ReleaseApplicationGraph::dump()
{
	printf("START:\n");
	ListIterator<Application> iter(m_starts);
	for(Application *a = iter.first(); a; a = iter.next()) {
		// a->dump();
	}
}


///////////////////////////////////////////////////////////////////////////////
// User
///////////////////////////////////////////////////////////////////////////////

User::User(
		Model &model, int id, const char *name, const char *email,
		const char *realname, const char *phone
	)
	: Object(model, id, name), m_email(DUP_NULL(email)),
	  m_realname(DUP_NULL(realname)), m_phone(DUP_NULL(phone)),
	  m_groups(NULL), m_groupsArrayCache(NULL),
	  m_accessibleDomainsCache(NULL)
{}


User::~User()
{
	SAFE_FREE(m_email);
	SAFE_FREE(m_realname);
	SAFE_FREE(m_phone);
	SAFE_DELETE(m_groups);
	SAFE_DELETE(m_groupsArrayCache);
	SAFE_FREE(m_accessibleDomainsCache)
}


/*virtual*/ Expr *User::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "kind") == 0) {
		return new Expr("user");
	} else if(strcmp(name, "email") == 0) {
		return new Expr(m_email);
	} else if(strcmp(name, "realname") == 0) {
		return new Expr(m_realname);
	} else if(strcmp(name, "phone") == 0) {
		return new Expr(m_phone);
	} else if(strcmp(name, "groups") == 0) {
		if(!m_groupsArrayCache) {
			if(!m_groups) {
				getUserGroups();
			}
			DMArray *ret = new DMArray(true, false, false);
			ret->addRef();	// This prevents the array being deleted
			if(m_groups) {
				ListIterator<UserGroup> iter(*m_groups);
				for(UserGroup *group = iter.first(); group; group = iter.next()) {
					ret->add(new Variable(NULL, group));
				}
			}
			m_groupsArrayCache = ret;
		}
		return new Expr(m_groupsArrayCache);
	} else if (strcmp(name,"lastlogin")==0) {
		DateTime *res = m_model.getLastLogin(id());
		return new Expr(res);
	}
	return Object::getAttribute(name, ctx);
}


//DMArray *User::getArrayAttribute(const char *name, class Context &ctx)
//{
//	if(strcmp(name, "groups") == 0) {
//		if(!m_groupsArrayCache) {
//			if(!m_groups) {
//				getUserGroups();
//			}
//			DMArray *ret = new DMArray(false, false, true);
//			ret->addRef();	// This prevents the array being deleted
//			if(m_groups) {
//				ListIterator<UserGroup> iter(*m_groups);
//				for(UserGroup *group = iter.first(); group; group = iter.next()) {
//					ret->put(group->name(), new Variable(NULL, group));
//				}
//			}
//			m_groupsArrayCache = ret;
//		}
//		return m_groupsArrayCache;
//	}
//	return Object::getArrayAttribute(name, ctx);
//}


List<UserGroup> *User::getUserGroups()
{
	if(!m_groups) {
		add(UserGroup::getEveryoneGroup(m_model));
		m_model.getUserGroupsForUser(*this);
	}
	return m_groups;
}


void User::add(class UserGroup *group)
{
	if(!m_groups) {
		m_groups = new List<UserGroup>();
	}
	m_groups->add(group);
}


bool User::validateHashedPassword(const char *passhash)
{
	return m_model.validateHashedPassword(*this, passhash);
}


const char *User::getAccessibleDomains()
{
	if(m_accessibleDomainsCache) {
		return m_accessibleDomainsCache;
	}

	Domain *domain = getDomain();
	if(domain) {
		m_accessibleDomainsCache = m_model.getAccessibleDomains(*domain);
	}

	debug1("User::getAccessibleDomains() returns '%s'", NULL_CHECK(m_accessibleDomainsCache));
	return m_accessibleDomainsCache;
}


void User::print(int ind)
{
	indent(ind);   printf("User: %s\n", m_name);
	indent(ind+1); printf("Email: %s\n", (m_email ? m_email : "(no email)"));
}


///////////////////////////////////////////////////////////////////////////////
// UserGroup
///////////////////////////////////////////////////////////////////////////////

/*static*/ UserGroup *UserGroup::s_everyone = NULL;


UserGroup::UserGroup(
		class Model &model, int id, const char *name, const char *email
	)
	: Object(model, id, name), m_email(DUP_NULL(email)), m_users(NULL),
	  m_usersArrayCache(NULL)
{}


UserGroup::~UserGroup()
{
	SAFE_FREE(m_email);
	SAFE_DELETE(m_users);
	SAFE_DELETE(m_usersArrayCache);
}


Expr *UserGroup::getAttribute(const char *name, class Context &ctx)
{
	if(strcmp(name, "kind") == 0) {
		return new Expr("group");
	} else if(strcmp(name, "email") == 0) {
		return m_email ? new Expr(m_email) : NULL;
	} else if(strcmp(name, "users") == 0) {
		if(!m_usersArrayCache) {
			if(!m_users) {
				getUsers();
			}
			DMArray *ret = new DMArray(false, false, true);
			ret->addRef();	// This prevents the array being deleted
			if(m_users) {
				ListIterator<User> iter(*m_users);
				for(User *user = iter.first(); user; user = iter.next()) {
					ret->put(user->name(), new Variable(NULL, user));
				}
			}
			m_usersArrayCache = ret;
		}
		return new Expr(m_usersArrayCache);
	}
	return Object::getAttribute(name, ctx);
}


//DMArray *UserGroup::getArrayAttribute(const char *name, class Context &ctx)
//{
//	if(strcmp(name, "users") == 0) {
//		if(!m_usersArrayCache) {
//			if(!m_users) {
//				getUsers();
//			}
//			DMArray *ret = new DMArray(false, false, true);
//			ret->addRef();	// This prevents the array being deleted
//			if(m_users) {
//				ListIterator<User> iter(*m_users);
//				for(User *user = iter.first(); user; user = iter.next()) {
//					ret->put(user->name(), new Variable(NULL, user));
//				}
//			}
//			m_usersArrayCache = ret;
//		}
//		return m_usersArrayCache;
//	}
//	return Object::getArrayAttribute(name, ctx);
//}


List<User> *UserGroup::getUsers()
{
	if(!m_users) {
		m_model.getUsersForUserGroup(*this);
	}
	return m_users;
}


void UserGroup::add(User *user)
{
	if(!m_users) {
		m_users = new List<User>();
	}
	m_users->add(user);
}


void UserGroup::print(int ind)
{
	indent(ind);   printf("UserGroup: %s\n", m_name);
	indent(ind+1); printf("Email: %s\n", (m_email ? m_email : "(no email)"));
}


/*static*/ UserGroup *UserGroup::getEveryoneGroup(class Model &model)
{
	if(!s_everyone) {
		s_everyone = new UserGroup(model, EVERYONE_ID, "EVERYONE", NULL);
	}
	return s_everyone;
}


///////////////////////////////////////////////////////////////////////////////
// Repository
///////////////////////////////////////////////////////////////////////////////

Repository::Repository(class Model &model, int id, const char *name)
	: ProviderObject(model, id, name), m_impls(NULL)
{}


Repository::~Repository()
{
	SAFE_DELETE(m_impls);
}


RepositoryImpl *Repository::createImpl(
	DMINT32 implId, ExtendedStmt &stmt, AuditEntry *entry, Context &ctx)
{
	if(!hasAccess(ctx.dm().getCurrentUser())) {
		throw PermissionDeniedException(ctx.stack(), "No access to repository '%s'", m_name);
	}

	ProviderObjectDef &def = getDef(ctx);

	Plugin *plugin = def.getPlugin();
	if(plugin && !plugin->loadPlugin(ctx.dm())) {
		throw RuntimeError(ctx.stack(), "Unable to load plugin for repository type '%s'", def.name());
	}

	RepositoryImplFactory *factory = RepositoryImplRegistry::instance().getFactory(def);

	if(!factory) {
		throw RuntimeError(ctx.stack(), "Unrecognised repository type '%s'", def.name());
	}

	// Verify that all mandatory properties are present and warn of any we won't use
	def.verifyProperties(ctx, m_props);

	// Create an instance of the impl so that we may later execute it - we push
	// the repository's vars so that they can be used during the evaluation of
	// the args/props.
	RepositoryImpl *impl = NULL;
	ctx.stack().push(getVars());
	try {
		impl = factory->create(implId, *this, stmt, ctx);
	} catch(...) {
		ctx.stack().pop(REPOSITORY_SCOPE);
		throw;
	}
	ctx.stack().pop(REPOSITORY_SCOPE);

	if(!impl) {
		throw RuntimeError(ctx.stack(), "Unexpected error creating instance of repository type '%s'", def.name());
	}
	if(!m_impls) {
		m_impls = new HashtableById<RepositoryImpl>(true);
	}
	m_impls->put(implId, impl);

	if(entry) {
		entry->recordInstance(*impl, ctx);
	}

	return impl;
}


Scope *Repository::getVars()
{
	if(!m_vars) {
		m_vars = new Scope(REPOSITORY_SCOPE, this);
		m_vars->addRef();
		m_model.getVariablesForObject(*this, *m_vars);
	}

	return m_vars;
}


Scope *Repository::getVars(
	DMINT32 implId, class ExtendedStmt &stmt, AuditEntry *entry, Context &ctx)
{
	RepositoryImpl *impl = m_impls ? m_impls->get(implId) : NULL;
	if(!impl) {
		impl = createImpl(implId, stmt, entry, ctx);
	}
	return impl->getVars();
}


char *Repository::dirname(const char *filename)
{
	// TODO: Move this to the factory
	return (m_impls && m_impls->get(0)) ? m_impls->get(0)->dirname(filename) : NULL;
}


bool Repository::testProvider(class ExtendedStmt &stmt, class Context &ctx)
{
	if(!hasAccess(ctx.dm().getCurrentUser())) {
		throw PermissionDeniedException(ctx.stack(), "No access to test repository '%s'", m_name);
	}

	RepositoryImpl *impl = createImpl(1, stmt, NULL, ctx);
	getDef(ctx).verifyArgs(ctx, stmt.getArgs(), m_props);
	return impl->testProvider(ctx);
}


void Repository::checkout(
	const char *dzbase, const char *dzpath, class ExtendedStmt &stmt,
	AuditEntry *entry, DropzoneCallback &cb, Context &ctx)
{
	if(!hasReadAccess(ctx.dm().getCurrentUser())) {
		throw PermissionDeniedException(ctx.stack(), "No read access for checkout from repository '%s'", m_name);
	}

	RepositoryImpl *impl = m_impls ? m_impls->get(cb.repoImplId()) : NULL;
	if(!impl) {
		impl = createImpl(cb.repoImplId(), stmt, entry, ctx);
	}

	bool recursive = stmt.getOverridableArgAsBoolean("recursive", *this, false, ctx);
	debug1("recursive = %s", recursive ? "true" : "false");

	// TODO: Can use either getArg() or getOverridableArg() if we want patterns as properties
	debug3("Getting pattern");
	AutoPtr<StringList> pattern;
	// ExprPtr epattern = stmt.getArg("pattern", ctx);
	ExprPtr epattern = stmt.getOverridableArg("pattern", *this, false, ctx);
//	debug3("epattern=0x%lx",epattern);
	if(epattern) {
		pattern = new StringList();
		if(epattern->kind() == KIND_ARRAY) {
			debug3("KIND ARRAY");
			DMArray *arr = epattern->toArray();
			AutoPtr<StringList> keys = arr->keys();
			StringListIterator iter(*keys);
			for(const char *key = iter.first(); key; key = iter.next()) {
				Variable *v = arr->get(key);
				if(v) {
					ConstCharPtr value = v->toString();
					debug1("adding pattern '%s'", (const char *)value);
					pattern->add(value);
				}
			}
		} else {
			debug3("Not KIND_ARRAY");
			ConstCharPtr patt = epattern->toString();
//			debug3("patt=0x%lx",patt);
			if(patt) {
				debug1("setting pattern to '%s'", (const char*) patt);
				pattern->add(patt);
			}
		}
	}

	if(pattern && (pattern->size() == 0)) {
		throw RuntimeError(ctx.stack(), "Unable to evaluate pattern");
	}

	getDef(ctx).verifyArgs(ctx, stmt.getArgs(), m_props);
	cb.checkout_from_repository(this);
//	debug3("Calling impl->checkout(pattern=0x%lx)",pattern);
	impl->checkout(dzbase, dzpath, recursive, pattern, stmt, cb, ctx);
}


void Repository::checkin(
	const char *dzbase, const char *dzpath, class ExtendedStmt &stmt,
	AuditEntry *entry, DropzoneCallback &cb, Context &ctx)
{
	if(!hasWriteAccess(ctx.dm().getCurrentUser())) {
		throw PermissionDeniedException(ctx.stack(), "No write access for checkin to repository '%s'", m_name);
	}

	RepositoryImpl *impl = m_impls ? m_impls->get(cb.repoImplId()) : NULL;
	if(!impl) {
		impl = createImpl(cb.repoImplId(), stmt, entry, ctx);
	}

	getDef(ctx).verifyArgs(ctx, stmt.getArgs(), m_props);
	cb.checkin_to_repository(this);
	impl->checkin(dzbase, dzpath, stmt, cb, ctx);
}


List<RepositoryTextPattern> *Repository::getTextPatternsForPath(const char *path)
{
	return m_model.getTextPatternsForRepositoryPath(*this, path);
}


List<RepositoryIgnorePattern> *Repository::getIgnorePatterns()
{
	if(!m_def) {
		throw RuntimeError("Provider object definition not yet resolved for repository provider '%s'", m_name);
	}
	return m_model.getIgnorePatternsForRepositoryDef(*m_def);
}


void Repository::print(int ind)
{
	indent(ind); printf("Repository: %s\n", m_name);
	printProviderObject(ind+1);
}


///////////////////////////////////////////////////////////////////////////////
// Notify
///////////////////////////////////////////////////////////////////////////////

Notify::Notify(class Model &model, int id, const char *name)
	: ProviderObject(model, id, name), m_impls(NULL)
{}


Notify::~Notify()
{
	SAFE_DELETE(m_impls);
}


NotifyProviderImpl *Notify::createImpl(
	DMINT32 implId, ExtendedStmt &stmt, AuditEntry *entry, Context &ctx)
{
	if(!hasAccess(ctx.dm().getCurrentUser())) {
		throw PermissionDeniedException(ctx.stack(), "No access to notify '%s'", m_name);
	}

	ProviderObjectDef &def = getDef(ctx);

	Plugin *plugin = def.getPlugin();
	if(plugin && !plugin->loadPlugin(ctx.dm())) {
		throw RuntimeError(ctx.stack(), "Unable to load pluging for notify type '%s'", def.name());
	}

	NotifyProviderImplFactory *factory = NotifyProviderImplRegistry::instance().getFactory(def);
	if(!factory) {
		throw RuntimeError(ctx.stack(), "Unrecognised notify provider type '%s'", def.name());
	}

	// Verify that all mandatory properties are present and warn of any we won't use
	def.verifyProperties(ctx, m_props);

	// Create an instance of the impl so that we may later execute it - we push
	// the notify's vars so that they can be used during the evaluation of
	// the args/props.
	NotifyProviderImpl *impl = NULL;
	ctx.stack().push(getVars());
	try {
		impl = factory->create(implId, *this, stmt, ctx);
	} catch(...) {
		ctx.stack().pop(NOTIFY_SCOPE);
		throw;
	}
	ctx.stack().pop(NOTIFY_SCOPE);

	if(!impl) {
		throw RuntimeError(ctx.stack(), "Unexpected error creating instance of notify provider '%s'", def.name());
	}
	if(!m_impls) {
		m_impls = new HashtableById<NotifyProviderImpl>(true);
	}
	m_impls->put(implId, impl);

	if(entry) {
		entry->recordInstance(*impl, ctx);
	}
	return impl;
}


class Scope *Notify::getVars()
{
	if(!m_vars) {
		m_vars = new Scope(NOTIFY_SCOPE, this);
		m_vars->addRef();
		m_model.getVariablesForObject(*this, *m_vars);
	}

	return m_vars;
}


Scope *Notify::getVars(
	DMINT32 implId, class ExtendedStmt &stmt, AuditEntry *entry, Context &ctx)
{
	NotifyProviderImpl *impl = m_impls ? m_impls->get(implId) : NULL;
	if(!impl) {
		impl = createImpl(implId, stmt, entry, ctx);
	}
	return impl->getVars();
}


bool Notify::testProvider(class ExtendedStmt &stmt, class Context &ctx)
{
	if(!hasAccess(ctx.dm().getCurrentUser())) {
		throw PermissionDeniedException(ctx.stack(), "No access to test notify '%s'", m_name);
	}

	NotifyProviderImpl *impl = createImpl(1, stmt, NULL, ctx);
	getDef(ctx).verifyArgs(ctx, stmt.getArgs(), m_props);
	return impl->testProvider(ctx);
}


void Notify::notify(
	DMINT32 implId, ExtendedStmt &stmt, AuditEntry *entry, OutputStream &body,
	List<Attachment> *atts, Context &ctx)
{
	NotifyProviderImpl *impl = m_impls ? m_impls->get(implId) : NULL;
	if(!impl) {
		impl = createImpl(implId, stmt, entry, ctx);
	}

	getDef(ctx).verifyArgs(ctx, stmt.getArgs(), m_props);
	impl->notify(stmt, body, atts, ctx);
}


void Notify::print(int ind)
{
	indent(ind); printf("Notify: %s\n", m_name);
	printProviderObject(ind+1);
}


///////////////////////////////////////////////////////////////////////////////
// Datasource
///////////////////////////////////////////////////////////////////////////////

Datasource::Datasource(class Model &model, int id, const char *name)
	: ProviderObject(model, id, name), m_impls(NULL)
{}


Datasource::~Datasource()
{
	SAFE_DELETE(m_impls);
}


DatasourceProviderImpl *Datasource::createImpl(
	DMINT32 implId, ExtendedStmt &stmt, AuditEntry *entry, Context &ctx)
{
	if(!hasAccess(ctx.dm().getCurrentUser())) {
		throw PermissionDeniedException(ctx.stack(), "No access to datasource '%s'", m_name);
	}

	ProviderObjectDef &def = getDef(ctx);

	Plugin *plugin = def.getPlugin();
	if(plugin && !plugin->loadPlugin(ctx.dm())) {
		throw RuntimeError(ctx.stack(), "Unable to load pluging for datasource type '%s'", def.name());
	}

	DatasourceProviderImplFactory *factory = DatasourceProviderImplRegistry::instance().getFactory(def);

	if(!factory) {
		throw RuntimeError(ctx.stack(), "Unrecognised datasource type '%s'", def.name());
	}

	// Verify that all mandatory properties are present and warn of any we won't use
	def.verifyProperties(ctx, m_props);

	// Create an instance of the impl so that we may later execute it
	DatasourceProviderImpl *impl = factory->create(implId, *this, stmt, ctx);
	if(!impl) {
		throw RuntimeError(ctx.stack(), "Unexpected error creating instance of datasource '%s'", def.name());
	}
	if(!m_impls) {
		m_impls = new HashtableById<DatasourceProviderImpl>(true);
	}
	m_impls->put(implId, impl);

	if(entry) {
		entry->recordInstance(*impl, ctx);
	}
	return impl;
}


class Scope *Datasource::getVars()
{
	if(!m_vars) {
		m_vars = new Scope(DATASOURCE_SCOPE, this);
		m_vars->addRef();
		m_model.getVariablesForObject(*this, *m_vars);
	}

	return m_vars;
}


Scope *Datasource::getVars(
	DMINT32 implId, class ExtendedStmt &stmt, AuditEntry *entry, Context &ctx)
{
	DatasourceProviderImpl *impl = m_impls ? m_impls->get(implId) : NULL;
	if(!impl) {
		impl = createImpl(implId, stmt, entry, ctx);
	}
	return impl->getVars();
}


bool Datasource::testProvider(class ExtendedStmt &stmt, class Context &ctx)
{
	if(!hasAccess(ctx.dm().getCurrentUser())) {
		throw PermissionDeniedException(ctx.stack(), "No access to test notify '%s'", m_name);
	}

	DatasourceProviderImpl *impl = createImpl(1, stmt, NULL, ctx);
	getDef(ctx).verifyArgs(ctx, stmt.getArgs(), m_props);
	return impl->testProvider(ctx);
}


void Datasource::query(DMINT32 implId, ExtendedStmt &stmt, AuditEntry *entry, Context &ctx)
{
	DatasourceProviderImpl *impl = m_impls ? m_impls->get(implId) : NULL;
	if(!impl) {
		impl = createImpl(implId, stmt, entry, ctx);
	}

	getDef(ctx).verifyArgs(ctx, stmt.getArgs(), m_props);
	impl->query(m_model,stmt, ctx);
}


void Datasource::print(int ind)
{
	indent(ind); printf("Datasource: %s\n", m_name);
	printProviderObject(ind+1);
}


///////////////////////////////////////////////////////////////////////////////
// Task
///////////////////////////////////////////////////////////////////////////////

Task::Task(class Model &model, int id, const char *name, const char *kind)
	: Object(model, id, name), m_taskType(DUP_NULL(kind)), m_impls(NULL),
	  m_preAction(NULL), m_postAction(NULL), m_actionsSet(false)
{}


Task::~Task()
{
	SAFE_FREE(m_taskType);
	SAFE_DELETE(m_impls);
	m_preAction = NULL;		// don't own
	m_postAction = NULL;	// don't own
}


void Task::setActions(Action *preAction, Action *postAction)
{
	m_preAction = preAction;
	m_postAction = postAction;
	m_actionsSet = true;
}


Action *Task::getPreAction()
{
	if(!m_actionsSet) {
		m_model.getActionsForTask(*this);
	}
	return m_preAction;
}


Action *Task::getPostAction()
{
	if(!m_actionsSet) {
		m_model.getActionsForTask(*this);
	}
	return m_postAction;
}


TaskImpl *Task::createImpl(
	DMINT32 implId, ExtendedStmt &stmt, class AuditEntry *entry, Context &ctx)
{
	//if(!hasAccess(ctx.dm().getCurrentUser())) {
	//	throw PermissionDeniedException(ctx.stack(), "No access to task '%s'", m_name);
	//}

	TaskImplFactory *factory = TaskImplRegistry::instance().getFactory(m_taskType);
	if(!factory) {
		throw RuntimeError(ctx.stack(), "Unrecognised task type '%s'", m_taskType);
	}

	// Create an instance of the impl so that we may later execute it
	TaskImpl *impl = factory->create(implId, *this, stmt, ctx);
	if(!impl) {
		throw RuntimeError(ctx.stack(), "Unexpected error creating instance of task '%s'", m_taskType);
	}
	if(!m_impls) {
		m_impls = new HashtableById<TaskImpl>(true);
	}
	m_impls->put(implId, impl);
	return impl;
}


class Scope *Task::getVars()
{
	if(!m_vars) {
		m_vars = new Scope(TASK_SCOPE, this);
		m_vars->addRef();
		//m_model.getVariablesForObject(*this, *m_vars);
	}

	return m_vars;
}


Scope *Task::getVars(
	DMINT32 implId, class ExtendedStmt &stmt, AuditEntry *entry, Context &ctx)
{
	TaskImpl *impl = m_impls ? m_impls->get(implId) : NULL;
	if(!impl) {
		impl = createImpl(implId, stmt, entry, ctx);
	}
	return impl->getVars();
}

void Model::removeRequest(long taskid,Context &ctx)
{
	Scope *appScope = ctx.stack().getScope(APPLICATION_SCOPE);
	Application *app = appScope ? (Application*) appScope->getScopeObject() : NULL;
	if (app) {
		// There's an application on the stack
		long completed   = time(NULL);
		long completedby = m_currentUser->id();
		long appid = app->id();

		AutoPtr<triSQL> sql = m_odbc.GetSQL();
		SQLRETURN res = sql->PrepareStatement("UPDATE dm_request SET status='C', completed=?, completedby=? WHERE taskid=? AND appid=?");
		if(IS_NOT_SQL_SUCCESS(res)) {
			throw RuntimeError("Failed to prepare removeRequest statement");
		}

		// printf("Updating request table setting complete taskid=%d appid=%d",taskid,appid);

		debug1("completed=%ld completedby=%ld taskid=%ld appid=%ld",completed,completedby,taskid,appid);
		sql->BindParameter(1, SQL_INTEGER, sizeof(completed),   &completed,   sizeof(completed));
		sql->BindParameter(2, SQL_INTEGER, sizeof(completedby), &completedby, sizeof(completedby));
		sql->BindParameter(3, SQL_INTEGER, sizeof(taskid),      &taskid,      sizeof(taskid));
		sql->BindParameter(4, SQL_INTEGER, sizeof(appid),       &appid,       sizeof(appid));
		res = sql->Execute();
		debug1("res=%d",res);
		if(res != SQL_SUCCESS && res != SQL_NO_DATA) {
			char *Message = (char *)0;
			SQLINTEGER ErrNum;
			sql->GetLastError(&Message,&ErrNum);
			sql->CloseSQL();
			throw RuntimeError("Failed to update removeRequest");
		}
		sql->CloseSQL();
	} // else ctx.dm().writeToStdOut("No app found on stack");
}

DateTime *Model::getLastLogin(int userid)
{
	long ret = 0;
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLRETURN res = sql->ExecuteSQL("select lastlogin from dm.dm_user where id=%d",userid);
	if(IS_NOT_SQL_SUCCESS(res)) {
		throw RuntimeError("Failed to create lastlogin query");
	}
	SQL_TIMESTAMP_STRUCT ts;
	SQLLEN ni_ts = 0;
	sql->BindColumn(1,SQL_TIMESTAMP,&ts,sizeof(ts),&ni_ts);
	res = sql->Execute();
	res = sql->FetchRow();
	if(res == SQL_SUCCESS || res == SQL_NO_DATA) {
		struct tm logintime;
		logintime.tm_hour = ts.hour;
		logintime.tm_min = ts.minute;
		logintime.tm_sec = ts.second;
		logintime.tm_year = ts.year - 1900;
		logintime.tm_mday = ts.day;
		logintime.tm_mon = ts.month;
		logintime.tm_isdst = -1;
		ret = mktime(&logintime);
		if (ret<0) ret=0;
	}
	sql->CloseSQL();
	return new DateTime(ret);
}

bool Model::DomainBelongsToEngine(int domainid)
{
	// returns true if the domain is under control of this engine,
	// false otherwise. Works by checking the domain to see if there's
	// an engine associated with it and - if so - if it is "us". If
	// there is an engine but it's not us, then we return false. If
	// there is an engine and it's us then we return true. If there
	// is no engine we recurse up to the parent domain.
	//

	char enginehost[257];
	char hostname[257];
	gethostname(hostname,sizeof(hostname));
	long parentdomainid;
	SQLLEN ni_parentdomainid,ni_enginehost;
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	const char *sql1 = "SELECT b.hostname,a.domainid FROM dm.dm_domain a LEFT OUTER JOIN dm.dm_engine b ON b.domainid = a.id WHERE a.id=?";
	SQLRETURN res = sql->PrepareStatement(sql1);
	if(IS_NOT_SQL_SUCCESS(res)) {
		throw RuntimeError("Failed to prepare domain engine statement");
	}
	sql->BindParameter(1, SQL_INTEGER, sizeof(domainid), &domainid, sizeof(domainid));
	sql->BindColumn(1,SQL_CHAR,enginehost,sizeof(enginehost),&ni_enginehost);
	sql->BindColumn(2,SQL_INTEGER,&parentdomainid,sizeof(parentdomainid),&ni_parentdomainid);
	res = sql->Execute();
	if(res != SQL_SUCCESS) {
		sql->CloseSQL();
		throw RuntimeError("Failed to get domain engine data");
	}
	res = sql->FetchRow();
	sql->CloseSQL();

	if (res==SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
		// Found the domain - does it have an engine associated?
		if (ni_enginehost == -1 || enginehost[0]=='\0') {
			// No engine - can't tell - recurse upward
			return DomainBelongsToEngine(parentdomainid);
		}
		// we have a engine - does it match our hostname?
		return (stricmp(hostname,enginehost)==0);
	}
	return false;
}

bool Model::checkMD5(int serverid)
{
	// returns true if the specified server has
	// (a) had deployments and
	// (b) is set to be automatically scanned
	//
	bool ret = false;
	int c=0;
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	const char *sql1 =	"SELECT	count(*) FROM dm_compsonserv WHERE serverid=?";
	SQLRETURN res = sql->PrepareStatement(sql1);
	if(IS_NOT_SQL_SUCCESS(res)) {
		throw RuntimeError("Failed to prepare checkMD5 statement");
	}
	sql->BindParameter(1, SQL_INTEGER, sizeof(serverid), &serverid, sizeof(serverid));
	sql->BindColumn(1, SQL_INTEGER, &c,sizeof(c));
	res = sql->Execute();
	if(res != SQL_SUCCESS) {
		sql->CloseSQL();
		throw RuntimeError("Failed to get checkMD5 data");
	}
	res = sql->FetchRow();
	if ((res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) && c > 0) {
		// This server has some components on it - are we set to auto check?
		char automd5[2];
		AutoPtr<triSQL> sqlmd5 = m_odbc.GetSQL();
		const char *sql2 =	"SELECT automd5 FROM dm_serverstatus WHERE serverid=?";
		SQLRETURN res = sqlmd5->PrepareStatement(sql2);
		if(IS_NOT_SQL_SUCCESS(res)) {
			throw RuntimeError("Failed to prepare autoMD5 statement");
		}
		sqlmd5->BindParameter(1, SQL_INTEGER, sizeof(serverid), &serverid, sizeof(serverid));
		sqlmd5->BindColumn(1, SQL_CHAR, automd5,sizeof(automd5));
		res = sqlmd5->Execute();
		if(res != SQL_SUCCESS) {
			sqlmd5->CloseSQL();
			throw RuntimeError("Failed to get autoMD5 data");
		}
		res = sqlmd5->FetchRow();
		if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
			// Got a row - is it Y?
			ret = (automd5[0]=='Y');
		}
	}
	return ret;
}

List<Server> *Model::getServersToCheck()
{
	//
	// Retrieve a list of servers that need "pinging" to make sure they're alive
	//
	debug1("getServersToCheck");
	time_t now = time(NULL);
	long secsSinceMidnight = now % 86400;
	List<Server> *ret = new List<Server>();
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	int serverid;
	int domainid;
	int interval;
	int starttime;
	int endtime;
	SQLLEN ni_interval;
	const char *sql1 =	"SELECT	a.id,a.domainid,b.starttime,b.endtime,b.checkinterval		"
						"FROM	dm_server		a,	"
						"		dm_serverstatus	b	"
						"WHERE	b.starttime <= ?	"
						"AND	b.endtime >= ?		"
						"AND	a.id = b.serverid	"
						"AND	b.autoping='Y'		"
						"UNION	"
						"SELECT	a.id,a.domainid,0,86400,b.checkinterval		"
						"FROM	dm_server		a,	"
						"		dm_serverstatus	b	"
						"WHERE	b.starttime is null	"
						"AND	b.endtime is null	"
						"AND	a.id = b.serverid	"
						"AND	b.autoping='Y'		"
						"ORDER BY 1";
	SQLRETURN res = sql->PrepareStatement(sql1);
	if(IS_NOT_SQL_SUCCESS(res)) {
		throw RuntimeError("Failed to prepare scan server statement");
	}
	sql->BindParameter(1, SQL_INTEGER, sizeof(secsSinceMidnight), &secsSinceMidnight, sizeof(secsSinceMidnight));
	sql->BindParameter(2, SQL_INTEGER, sizeof(secsSinceMidnight), &secsSinceMidnight, sizeof(secsSinceMidnight));
	sql->BindColumn(1,SQL_INTEGER,&serverid,sizeof(serverid));
	sql->BindColumn(2,SQL_INTEGER,&domainid,sizeof(domainid));
	sql->BindColumn(3,SQL_INTEGER,&starttime,sizeof(starttime));
	sql->BindColumn(4,SQL_INTEGER,&endtime,sizeof(endtime));
	sql->BindColumn(5,SQL_INTEGER,&interval,sizeof(interval),&ni_interval);
	res = sql->Execute();
	if(res != SQL_SUCCESS) {
		sql->CloseSQL();
		printf("SQL failed to execute res=%d\n",res);
		throw RuntimeError("Failed to get scan server data");
	}
	res = sql->FetchRow();
	while (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
		if (ni_interval == -1) interval=15;	// default
		//
		// We have a server that (potentially) needs scanning.
		// Ensure that this belongs to one of this engine's
		// domains (so we don't get more than one engine trying
		// to scan the server)
		//
		if (DomainBelongsToEngine(domainid)) {
			//
			// This server is one of ours - loop from start time to end time
			// using the specified interval and see if we need to check the server
			//
			for (int i=starttime;i<=endtime;i=i+(interval*60)) {
				if (i >= secsSinceMidnight && i < secsSinceMidnight + 900) {
					// This server needs to be checked
					Server *s = getServerById(serverid);
					ret->add(s);
					break;
				}
			}
		}
		res = sql->FetchRow();
	}
	if (res != SQL_NO_DATA) {
		char *MsgPtr;
		SQLINTEGER errnum;
		sql->GetLastError(&MsgPtr,&errnum);
		printf("MsgPtr=[%s]\n",MsgPtr);
	}
	sql->CloseSQL();
	return ret;
}

List<TimedJob> *Model::getTimedJobs()
{
	//
	// Retrieve a list of jobs that are in the calendar that belong to this engine
	// and are ready to execute now.
	//
	long eventid;
	long creatorid;
	long appid;
	long envid;
	long domainid;
	time_t now = time(NULL);
	List<TimedJob> *ret = new List<TimedJob>();
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	const char *sql1 =	"SELECT	a.id,a.creatorid,a.appid,a.envid,b.domainid	"
						"FROM	dm_calendar a,				"
						"		dm_environment b			"
						"WHERE	a.eventtype='Auto'			"
						"AND	a.status='N'					"
						"AND	a.envid = b.id				"
						"AND	a.starttime<=?				"
						"AND	a.endtime>=?				"
						"AND NOT EXISTS (					"
						"	SELECT	x.eventid				"
						"	FROM	dm_deployment x			"
						"	WHERE	x.eventid = a.id		"
						")";
	SQLRETURN res = sql->PrepareStatement(sql1);
	if(IS_NOT_SQL_SUCCESS(res)) {
		throw RuntimeError("Failed to prepare calendar statement");
	}
	sql->BindParameter(1, SQL_INTEGER, sizeof(now), &now, sizeof(now));
	sql->BindParameter(2, SQL_INTEGER, sizeof(now), &now, sizeof(now));
	sql->BindColumn(1,SQL_INTEGER,&eventid,sizeof(eventid));
	sql->BindColumn(2,SQL_INTEGER,&creatorid,sizeof(creatorid));
	sql->BindColumn(3,SQL_INTEGER,&appid,sizeof(appid));
	sql->BindColumn(4,SQL_INTEGER,&envid,sizeof(envid));
	sql->BindColumn(5,SQL_INTEGER,&domainid,sizeof(domainid));
	
	res = sql->Execute();
	if(res != SQL_SUCCESS) {
		sql->CloseSQL();
		throw RuntimeError("Failed to get calendar data");
	}
	res = sql->FetchRow();
	while (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
		//
		// We have a timed job that's ready to go. Ensure that this belongs
		// to one of this engine's domains (so we don't get more than one
		// engine trying to run the same job)
		//
		if (DomainBelongsToEngine(domainid)) {
			// This is one of ours - add to the list
			// Which user shall we use?
			// See if the calendar event has been approved by a user. If it has, then
			// we'll use that user. Otherwise use the creating user.
			long deployuser=0;
			AutoPtr<triSQL> sqla = m_odbc.GetSQL();
			long completedby;
			const char *sql2 = "SELECT completedby FROM dm_request WHERE status='C' AND calendarid=? ORDER BY completed DESC";	// latest first
			res = sqla->PrepareStatement(sql2);
			if(IS_NOT_SQL_SUCCESS(res)) {
				throw RuntimeError("Failed to prepare request statement");
			}
			sqla->BindParameter(1, SQL_INTEGER, sizeof(eventid), &eventid, sizeof(eventid));
			sqla->BindColumn(1,SQL_INTEGER,&completedby,sizeof(completedby));
			res = sqla->Execute();
			if(res != SQL_SUCCESS) {
				sqla->CloseSQL();
				throw RuntimeError("Failed to get request data");
			}
			res = sqla->FetchRow();
			if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
				// This event was approved
				deployuser = completedby;
			} else {
				// event was not approved. Since this is a "real" event and not pending
				// the creating user must have had access to the environment. Use that
				// user
				deployuser = creatorid;
			}
			sqla->CloseSQL();

			TimedJob *job = new TimedJob(eventid,appid,envid,creatorid);
			ret->add(job);
		}
		res = sql->FetchRow();
	}
	sql->CloseSQL();
	return ret;
}

NotifyTemplate *Model::internalGetTemplate(const char *query,Context &ctx,long id)
{
	long templateid;
	long notifierid;
	char szName[129];
	char szSubject[257];
	char szBody[2049];
	
	szSubject[0]='\0';
	szBody[0]='\0';
	szName[0]='\0';

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLRETURN res = sql->PrepareStatement(query);
	if(IS_NOT_SQL_SUCCESS(res)) {
		throw RuntimeError("Failed to prepare template statement");
	}
	sql->BindParameter(1, SQL_INTEGER, sizeof(id), &id, sizeof(id));
	sql->BindColumn(1,SQL_INTEGER,&templateid,sizeof(templateid));
	sql->BindColumn(2,SQL_CHAR,szName,sizeof(szName));
	sql->BindColumn(3,SQL_INTEGER,&notifierid,sizeof(notifierid));
	sql->BindColumn(4,SQL_CHAR,szSubject,sizeof(szSubject));
	sql->BindColumn(5,SQL_CHAR,szBody,sizeof(szBody));
	res = sql->Execute();
	if(res != SQL_SUCCESS) {
		sql->CloseSQL();
		throw RuntimeError("Failed to get notify template");
	}
	res = sql->FetchRow();
	sql->CloseSQL();

	if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
		Notify *notify =  getNotifier(notifierid);
		NotifyTemplate *ret = new NotifyTemplate(*notify,szName,szSubject,szBody);
		//
		// Now get the recipient list and turn them into OTIDs
		//
		long usrgrpid;
		long userid;
		long ownertype;
		SQLLEN ni_usrgrpid=0;
		SQLLEN ni_userid=0;
		SQLLEN ni_ownertype=0;
		char rec[128];
		AutoPtr<triSQL> sqla = m_odbc.GetSQL();
		SQLRETURN res2 = sqla->PrepareStatement("SELECT usrgrpid,userid,ownertype FROM dm_templaterecipients WHERE templateid=?");
		sqla->BindParameter(1, SQL_INTEGER, sizeof(templateid), &templateid, sizeof(templateid));
		sqla->BindColumn(1,SQL_INTEGER,&usrgrpid,sizeof(usrgrpid),&ni_usrgrpid);
		sqla->BindColumn(2,SQL_INTEGER,&userid,sizeof(userid),&ni_userid);
		sqla->BindColumn(3,SQL_INTEGER,&ownertype,sizeof(ownertype),&ni_ownertype);
		res2 = sqla->Execute();
		if (res2 != SQL_SUCCESS) {
			sqla->CloseSQL();
			throw RuntimeError("Failed to get notify recipients");
		}
		char **reclist = (char **)malloc(sizeof(char **));
		reclist[0]=(char *)0;
		int n=1;
		while (sqla->FetchRow() != SQL_NO_DATA) {
			rec[0]='\0';
			if (ni_usrgrpid != -1) {
				// user group is set
				sprintf(rec,"gr%d",usrgrpid);
			}
			if (ni_userid != -1) {
				// user is set
				sprintf(rec,"us%d",userid);
			}
			if (ni_ownertype != -1) {
				debug1("ownertype is set to %d",ownertype);
				// special owner type is set - need to resolve these here
				switch(ownertype) {
					case 1: {
							// ${server.owner}
							Scope *serverScope = ctx.stack().getScope(SERVER_SCOPE);
							Server *server = serverScope ? (Server*) serverScope->getScopeObject() : NULL;
							if (server) {
								Object *owner = server->getOwner();
								if (owner->kind() == OBJ_KIND_USER) {
									// owner is a user
									sprintf(rec,"us%d",owner->id());
								} else {
									// owner is a usergroup
									sprintf(rec,"gr%d",owner->id());
								}
							}
						}
						break;
					case 2: {
							// ${application.owner}
							Scope *appScope = ctx.stack().getScope(APPLICATION_SCOPE);
							Application *app = appScope ? (Application*) appScope->getScopeObject() : NULL;
							if (app) {
								Object *owner = app->getOwner();
								if (owner->kind() == OBJ_KIND_USER) {
									// owner is a user
									sprintf(rec,"us%d",owner->id());
								} else {
									// owner is a usergroup
									sprintf(rec,"gr%d",owner->id());
								}
							}
						}
						break;
					case 3: {
							// ${environment.owner}
							Scope *envScope = ctx.stack().getScope(ENVIRONMENT_SCOPE);
							Environment *env = envScope ? (Environment*) envScope->getScopeObject() : NULL;
							if (env) {
								Object *owner = env->getOwner();
								if (owner->kind() == OBJ_KIND_USER) {
									// owner is a user
									sprintf(rec,"us%d",owner->id());
								} else {
									// owner is a usergroup
									sprintf(rec,"gr%d",owner->id());
								}
							}
						}
						break;
				}

			}
			if (rec[0]) {
				// got a valid recipient
				reclist = (char **)realloc(reclist,(sizeof(char *))*(n+1));
				reclist[n-1]=strdup(rec);
				reclist[n]=(char *)0;
				n++;
			}
		}
		debug1("End of recipient data");
		sqla->CloseSQL();
		ret->setRecipients(reclist);
		return ret;
	} else {
		// No template found
		return NULL;
	}
}

NotifyTemplate *Model::getApplicationTemplate(Context &ctx,Application &app,bool useFailure /* = false */)
{
	const char *sql1 = "SELECT a.id,a.name,a.notifierid,a.subject,a.body FROM dm_template a, dm_application b WHERE a.id=b.successtemplateid AND b.id=?";
	const char *sql2 = "SELECT a.id,a.name,a.notifierid,a.subject,a.body FROM dm_template a, dm_application b WHERE a.id=b.failuretemplateid AND b.id=?";

	return internalGetTemplate(useFailure?sql2:sql1,ctx,app.id());
}

NotifyTemplate *Model::getNotifyTemplate(Context &ctx,Task &task,bool useFailure /* = false */)
{
	const char *sql1 = "SELECT a.id,a.name,a.notifierid,a.subject,a.body FROM dm_template a, dm_task b WHERE a.id=b.successtemplateid AND b.id=?";
	const char *sql2 = "SELECT a.id,a.name,a.notifierid,a.subject,a.body FROM dm_template a, dm_task b WHERE a.id=b.failuretemplateid AND b.id=?";

	return internalGetTemplate(useFailure?sql2:sql1,ctx,task.id());
}

NotifyTemplate *Model::getPingTemplate(Context &ctx,Server &server)
{
	const char *sql = "SELECT a.id,a.name,a.notifierid,a.subject,a.body FROM dm_template a, dm_serverstatus b WHERE a.id=b.pingtemplateid AND b.serverid=?";
	return internalGetTemplate(sql,ctx,server.id());
}

NotifyTemplate *Model::getMD5Template(Context &ctx,Server &server)
{
	const char *sql = "SELECT a.id,a.name,a.notifierid,a.subject,a.body FROM dm_template a, dm_serverstatus b WHERE a.id=b.md5templateid AND b.serverid=?";
	return internalGetTemplate(sql,ctx,server.id());
}

int Task::run(DMINT32 implId, ExtendedStmt &stmt, AuditEntry *entry, Context &ctx)
{
	TaskImpl *impl = m_impls ? m_impls->get(implId) : NULL;
	if(!impl) {
		impl = createImpl(implId, stmt, entry, ctx);
	}

	// Run any pre-linked action
	Action *preAction = getPreAction();
	if(preAction) {
		/*
		ActionNode *act = preAction->getActionNode(ctx);
		if(act) {
			const char *scopeName = act->isFunction() ? FUNCTION_SCOPE : ACTION_SCOPE;
			Scope *actscope = new Scope(scopeName, act, *act, true);
			ctx.stack().push(actscope);
			try {
				act->execute(ctx);
			} catch(ReturnException &e) {
				if(e.expr() && (e.expr()->toInt() != 0)) {
					ctx.dm().writeToStdOut("Pre-linked action returned %d", e.expr()->toInt());
					ctx.stack().pop(scopeName);
					return 1;
				}
			} catch(...) {
				ctx.stack().pop(scopeName);
				throw;
			}
			ctx.stack().pop(scopeName);
		}
		*/
		int ret = ctx.dm().runLinkedAction(*preAction);
		if (ret) {
			ctx.dm().writeToStdOut("Pre-linked action returned %d", ret);
			return ret;	// drop out for non-zero exit codes
		}
	}

	int ret = impl->run(stmt, ctx);

	if (ret == 0) {
		// Successful execution - remove any request entries
		m_model.removeRequest(id(),ctx);
		// Run any post-linked action
		Action *postAction = getPostAction();
		if(postAction) {
			ActionNode *act = postAction->getActionNode(ctx);
			if(act) {
				Scope *actscope = new Scope(ACTION_SCOPE, act, *act, true);
				ctx.stack().push(actscope);
				try {
					act->execute(ctx);
				} catch(...) {
					ctx.stack().pop(ACTION_SCOPE);
					throw;
				}
				ctx.stack().pop(ACTION_SCOPE);
			}
		}
	}

	return ret;
}


Domain *Task::getApprovalDomain()
{
	if(strcmp(m_taskType, "approve") != 0) {
		throw RuntimeError("Task %d '%s' is not an approve task", m_id, m_name);
	}
	return m_model.getApprovalDomainForApproveTask(*this);
}


Domain *Task::getTargetDomain()
{
	if((strcmp(m_taskType, "copy") != 0)
		&& (strcmp(m_taskType, "move") != 0)
		&& (strcmp(m_taskType, "createversion") != 0)) {
		throw RuntimeError("Task %d '%s' is not a copy or move task", m_id, m_name);
	}
	return m_model.getTargetDomainForCopyMoveTask(*this);
}


Task *Task::getLinkedTask()
{
	if(strcmp(m_taskType, "request") != 0) {
		throw RuntimeError("Task %d '%s' is not a request task", m_id, m_name);
	}
	return m_model.getLinkedTaskForRequestTask(*this);
}

NotifyTemplate *Task::getSuccessTemplate(Context &ctx)
{
	return m_model.getNotifyTemplate(ctx,*this);
}

NotifyTemplate *Task::getFailureTemplate(Context &ctx)
{
	return m_model.getNotifyTemplate(ctx,*this,true);
}


/*virtual*/ void Task::print(int indent)
{}


///////////////////////////////////////////////////////////////////////////////
// Script
///////////////////////////////////////////////////////////////////////////////

Script::Script(Model &model, int repoid, const char *filepath)
	: m_model(model), m_repoid(repoid), m_filepath(DUP_NULL(filepath)),
	  m_action(NULL), m_textid(0), m_repo(NULL), m_parsed(false), m_checksum(NULL)
{}

Script::Script(Model &model, Action *action, int textid)
	: m_model(model), m_repoid(0), m_filepath(NULL), m_action(action),
	  m_textid(textid), m_repo(NULL), m_parsed(false), m_checksum(NULL)
{}


Script::~Script()
{
	SAFE_FREE(m_filepath);
	m_repo = NULL;				// Owned by cache
	m_action = NULL;			// Owned by cache
	SAFE_FREE(m_checksum);
}


Repository *Script::getRepository()
{
	if(!m_repo) {
		m_model.getRepositoryForScript(*this);
	}
	return m_repo;
}


/*private*/ void Script::setRepository(Repository *repo)
{
	m_repo = repo;
}


Dropzone &Script::getScriptDropzone(Context &ctx)
{
	// Check to see if the dropzone was created in this session
	Dropzone *dz = ctx.dm().getDropzone("tdm_scripts");
	if(!dz) {
		// No, so create it, making sure it is empty
		dz = ctx.dm().newDropzone("tdm_scripts", false);
		if(!dz->create(true)) {
			// Set flag to prevent us trying to delete the non-empty folder when we exit
			putenv(strdup("trinounlink=Y"));
			throw RuntimeError(ctx.stack(), "Unable to create dropzone 'tdm_scripts'");
		}
	}
	return *dz;
}


char *Script::getFileContent(const char *filename)
{
	struct stat sb;
	if(stat(filename, &sb) != 0) {
		fprintf(stderr, "Input file '%s' not found\n", (const char*) filename);
		return NULL;
	}

	int buflen = sb.st_size;
	char *input = (char*) malloc(buflen + 1);
	if (input) {
		FILE *f = fopen(filename, "r");
		if(!f) {
			fprintf(stderr, "Error opening '%s'\n", (const char*) filename);
			return NULL;
		}

		int read = fread(input, 1, buflen, f);
		fclose(f);

		if(read == 0) {
			return NULL;
		}

		input[read] = '\0';
	}
	return input;
}


int Script::parse(Action &action, Context &ctx)
{
	debug3("Parse IN (%s/%s) - trying for lock...",action.name(),m_action?m_action->name():"null");
	Thread::lock(__LINE__,__FILE__,"parse");
	if(m_parsed) {
		debug1("Script '%s' has already been parsed", (m_action ? m_action->name() : m_filepath));
		Thread::unlock(__LINE__,__FILE__,"parse");
		return 0; //1;
	}

	try {
		CharPtr filename;
		char *input = NULL;

		if(m_action) {
			// We are an action stored in the database
			if((m_action->actionKind() == ACTION_KIND_IN_DB) && !m_textid) {
				throw RuntimeError(ctx.stack(), "Text id for action '%s' missing.", m_action->name());
			}

			if(m_action->actionKind() == ACTION_KIND_IN_DB) {
				// Create an input buffer with the action name wrapped around the text
				// pulled from the db
				ConstCharPtr text = m_model.getActionText(m_textid);
				if(!text) {
					throw RuntimeError(ctx.stack(), "Error fetching text for action '%s'", m_action->name());
				}
				ConstCharPtr sig = m_action->getSignature();
				input = (char*) malloc(strlen(sig) + strlen(text) + 10);
				sprintf(input, "%s {\n%s\n}\n", (const char*) sig, (const char*) text);
				filename = strdup(sig);
			} else if(m_action->actionKind() == ACTION_KIND_GRAPHICAL) {
				// Generate a script from the graphical data stored in the database
				Dropzone &dz = getScriptDropzone(ctx);
				AutoPtr<GraphicalScriptGenerator> gen = m_model.createGraphicalScriptGenerator(dz, *m_action);
				if(!gen->generate()) {
					throw RuntimeError(ctx.stack(), "Error generating script for action '%s'", m_action->name());
				}
				filename = strdup(gen->filename());
				debug3("parse generated '%s'...", (const char*) filename);
				if(!(input = getFileContent(filename))) {
					Thread::unlock(__LINE__,__FILE__,"parse");
					return 1;
				}
			} else {
				throw RuntimeError(ctx.stack(), "Invalid action kind for action '%s'", m_action->name());
			}
		} else {
			// We are a script stored in a repository
			if(!m_repo) {
				getRepository();
			}
			if(!m_repo) {
				throw RuntimeError(ctx.stack(), "Repository for script '%s' not found.", (m_filepath ? m_filepath : "(null)"));
			}

			Dropzone &dz = getScriptDropzone(ctx);
			const char *dropzone = dz.pathname();

			StmtList *args = new StmtList();
			args->add(new Stmt(strdup("pattern"), new Node(KIND_STR, strdup(m_filepath), true)));
			ExtendedStmt dummy(strdup("dummy"), args);

			DropzoneCallback callback(m_repo, 0, dz, ctx);
			m_repo->checkout(dz.pathname(), dropzone, dummy, NULL, callback, ctx);
			debug3("Checkout from repository '%s': total: %d; success %d; failed %d",
				m_repo->name(), callback.total(), callback.success(), callback.failed());

			if(callback.success() != 1) {
				throw RuntimeError(ctx.stack(), "Failed to checkout script '%s' from repository '%s'.",
					(m_filepath ? m_filepath : "(null)"), m_repo->name());
			}

			Node nfilepath(NODE_STR, strdup(m_filepath), true);
			ExprPtr efilepath = nfilepath.evaluate(ctx);
			ConstCharPtr filepath = efilepath->stringify();

			filename = (char*) malloc(strlen(dropzone) + strlen((const char*) filepath) + 10);
			sprintf(filename, "%s%s%s", dropzone, DIR_SEP_STR, (const char*) filepath);

			debug3("parse '%s'...", (const char*) filename);
			if(!(input = getFileContent(filename))) {
				Thread::unlock(__LINE__,__FILE__,"parse");
				return 1;
			}
		}

		// Calculate the checksum of the input text
		m_checksum = digestValue(input, strlen(input), false);
		m_model.getAudit().recordAction(action);

		// printf("input = %s",input);

		LexerBuffer lb(action_lexer, input, filename);

		yyparse_param param;
		param.buffer = &lb;

		int res = 1;
		try {
			res = yyparse(&param);
		} catch(...) {
			SAFE_FREE(input);
			debug3("PARSE ERROR - unlocking");
			Thread::unlock(__LINE__,__FILE__,"parse");		// PAG mod 05/08/2014 - if parse generates an error need to unlock...
			throw;
		}

		SAFE_FREE(input);			// we have finished with the input buffer now

		debug3("PARSE - res=%d",res);

		if(res != 0) {
			SAFE_DELETE(param.actnlist);
			if(param.ex) {
				Thread::unlock(__LINE__,__FILE__);		// PAG mod 05/08/2014 - if syntax error need to unlock...
				throw *((SyntaxError*) param.ex);
			}
			Thread::unlock(__LINE__,__FILE__,"parse");
			return 2;
		}

		if(!param.actnlist) {
			Thread::unlock(__LINE__,__FILE__,"parse");
			return 3;
		}

		if(!getenv("trinosemantic")) {
			try {
				param.actnlist->semanticCheck(ctx.dm());
			} catch(...) {
				SAFE_DELETE(param.actnlist);
				Thread::unlock(__LINE__,__FILE__,"parse");		// PAG mod 05/08/2014 - if symantic error need to unlock...
				throw;
			}
		}
		m_model.setActionNodes(param.actnlist);
	} catch(...) {
		Thread::unlock(__LINE__,__FILE__,"parse");
		throw;
	}

	debug3("Parse OUT - unlock...");
	m_parsed = true;
	Thread::unlock(__LINE__,__FILE__,"parse");
	return 0;
}


void Script::print(int ind)
{
	//indent(ind); printf("Script: %s\n", m_name);
	//printObject(ind+1);
}


///////////////////////////////////////////////////////////////////////////////
// Field
///////////////////////////////////////////////////////////////////////////////

Field::Field(
		class Model &model, int id, const char *name,
		FIELD_KIND kind, const char *querystring
	)
	: m_model(model), m_id(id), m_name(DUP_NULL(name)), m_kind(kind),
	  m_querystring(DUP_NULL(querystring)), m_action(NULL)
{}


Field::~Field()
{
	SAFE_FREE(m_name);
	SAFE_FREE(m_querystring);
	m_action = NULL;	// Owned by cache
}


Action *Field::getAction()
{
	if(!m_action) {
		m_model.getActionForField(*this);
	}
	return m_action;
}


/*private*/ void Field::setAction(Action *action)
{
	m_action = action;
}


void Field::populate(DM &dm)
{
	switch(m_kind) {
	case FIELD_KIND_CHECKBOX: break;
	case FIELD_KIND_COMBO_ARRAY: {
		StringList *values = m_model.getFieldValuesForField(*this);
		if(values) {
			StringListIterator iter(*values);
			for(const char *s = iter.first(); s; s = iter.next()) {
				dm.writeToStdOut("%s", s);
			}
			SAFE_DELETE(values);
		} else {
			dm.writeToStdErr("Failed to get values for field '%s'", (m_name ? m_name : "(null)"));
		}
		}
		break;
	case FIELD_KIND_COMBO_DB: {
		StringList *values = m_model.getFieldValuesForField(m_querystring);
		if(values) {
			StringListIterator iter(*values);
			for(const char *s = iter.first(); s; s = iter.next()) {
				dm.writeToStdOut("%s", s);
			}
			SAFE_DELETE(values);
		} else {
			dm.writeToStdErr("Failed to get values for field '%s'", (m_name ? m_name : "(null)"));
		}
		}
		break;
	case FIELD_KIND_COMBO_ACTION: {
		if(!m_action) {
			getAction();
		}
		if(!m_action) {
			throw RuntimeError("Script for field '%s' not found.", (m_name ? m_name : "(null)"));
		}
		int res = dm.runAction(*m_action);
		}
		break;
	case FIELD_KIND_TEXT: break;
	}
}


///////////////////////////////////////////////////////////////////////////////
// ActionArg
///////////////////////////////////////////////////////////////////////////////

ActionArg::ActionArg(
		const char *name, int pos, bool reqd, bool pad,
		SwitchMode mode, const char *sw, const char *nsw, const char *type
	)
	: m_name(DUP_NULL(name)), m_position(pos), m_required(reqd),
	  m_pad(pad), m_mode(mode), m_switch(DUP_NULL(sw)), m_negswitch(DUP_NULL(nsw)), m_type(DUP_NULL(type))
{}


ActionArg::~ActionArg()
{
	SAFE_FREE(m_name);
	SAFE_FREE(m_switch);
	SAFE_FREE(m_negswitch);
	SAFE_FREE(m_type);
}


/**
 * Appends switches and values to a parameter list.
 *
 * NONE    - value or pad
 * SWITCH  - switch plus value / neg switch / pad
 * PREFIX  - prefix plus value / neg switch / pad
 * ALWAYS  - switch
 * BOOLEAN - switch / negswitch / pad
 */
void ActionArg::process(const char *val, StringList &params, Context &ctx)
{
	switch(m_mode) {
	case SWITCH_MODE_NONE:
		debug3("SWITCH_MODE_NONE");
		if(val) { params.add(val); return; }
		// fall thru for pad
		debug3("end SWITCH_MODE_NONE");
		break;
	case SWITCH_MODE_SWITCH:
		debug3("SWITCH_MODE_SWITCH");
		if(val || m_pad) {
			if(m_switch) {
				ConstCharPtr theswitch = getExpandedSwitch(ctx);
				params.add(theswitch);
			}
			if(val) { params.add(val); return; }
			// fall thru for pad
		} else if(m_negswitch) {
			ConstCharPtr negswitch = getExpandedNegSwitch(ctx);
			params.add(negswitch);
			return;
		}
		debug3("end SWITCH_MODE_SWITCH");
		break;
	case SWITCH_MODE_PREFIX:
		debug3("SWITCH_MODE_PREFIX");
		if(val || m_pad) {
			if(m_switch && val) {
				ConstCharPtr theswitch = getExpandedSwitch(ctx);
				CharPtr temp = (char*) malloc(strlen(val) + strlen(theswitch) + 1);
				sprintf(temp, "%s%s", (const char*) theswitch, val);
				params.add(temp);
				return;
			} else if(val) {
				params.add(val);
				return;
			} else if(m_switch) {
				ConstCharPtr theswitch = getExpandedSwitch(ctx);
				params.add(theswitch);
				return;
			}
			// fall thru for pad
		} else if(m_negswitch) {
			ConstCharPtr negswitch = getExpandedNegSwitch(ctx);
			params.add(negswitch);
			return;
		}
		debug3("end SWITCH_MODE_PREFIX (returning)");
		return;
	case SWITCH_MODE_ALWAYS: {
		debug3("SWITCH_MODE_ALWAYS");
		ConstCharPtr theswitch = getExpandedSwitch(ctx);
		params.add(theswitch ? (const char*) theswitch : "");
		}
		debug3("end SWITCH_MODE_ALWAYS (returning)");
		return;
	case SWITCH_MODE_BOOLEAN:
		debug3("SWITCH_MODE_BOOLEAN");
		if (m_type && STRCASECMP(m_type,"checkbox")==0) {
			debug3("Type is checkbox val is [%s]",val);
			if (val && (tolower(val[0])=='y' || STRCASECMP(val,"true")==0) || (val[0]=='1' && val[1]=='\0')) {
				debug3("val is Y/true/1");
				ConstCharPtr theswitch = getExpandedSwitch(ctx);
				debug3("got the switch (%s), adding...",(const char *)theswitch);
				params.add(theswitch);
				debug3("added - returning");
				return;
			} else {
				debug3("val is not y");
				if (m_negswitch && m_negswitch[0]) {
					debug3("negswitch is set");
					ConstCharPtr negswitch = getExpandedNegSwitch(ctx);
					params.add(negswitch);
					debug3("added negswitch - returning");
					return;
				}
				debug3("val is not y and negswitch not set");
			}
			debug3("end Type is checkbox");
		} else {
			debug3("type is not checkbox");
			if(val) {
				debug3("val is set, expanding");
				Expr e(val);
				debug3("expanded ok");
				if(e.toBool()) {
					debug3("e.toBool returns true");
					if(m_switch) {
						debug3("m_switch is set");
						ConstCharPtr theswitch = getExpandedSwitch(ctx);
						params.add(theswitch);
						debug3("Added the switch, returning");
						return;
					}
					// fall thru for pad
					debug3("m_switch not set");
				} else if(m_negswitch) {
					debug3("m_negswitch is set");
					ConstCharPtr negswitch = getExpandedNegSwitch(ctx);
					params.add(negswitch);
					return;
				}
			}
			debug3("end type is not checkbox");
		}
		// fall thru for pad
		debug3("end SWITCH_MODE_BOOLEAN");
		break;
	}

	if(m_pad) { params.add(""); }
}

char *ActionArg::getExpandedSwitch(Context &ctx)
{
	if(!m_switch) {
		return NULL;
	}
	
	// Expand any embedded variables in m_switch
	Node expandedText(NODE_STR, strdup(m_switch), true);	// will free text on delete
	ExprPtr etext = expandedText.evaluate(ctx);
	return etext->stringify();
}

char *ActionArg::getExpandedNegSwitch(Context &ctx)
{
	if(!m_negswitch) {
		return NULL;
	}
	
	// Expand any embedded variables in m_switch
	Node expandedText(NODE_STR, strdup(m_negswitch), true);	// will free text on delete
	ExprPtr etext = expandedText.evaluate(ctx);
	return etext->stringify();
}


///////////////////////////////////////////////////////////////////////////////
// Action
///////////////////////////////////////////////////////////////////////////////


Action::Action(
		Model &model, int id, ACTION_KIND kind, bool isFunction,
		/*bool isRemote,*/ bool copyToRemote, bool resultIsExpr,
		const char *name, const char *filepath, const char *shell,
		bool useTTY
	)
	: Object(model, id, name), m_actkind(kind),
	  m_filepath(DUP_NULL(filepath)), m_isFunction(isFunction),
	  m_copyToRemote(copyToRemote),
	  m_resultIsExpr(resultIsExpr), m_plugin(NULL),
	  m_args(NULL), m_actionArgMap(NULL), m_functionArgMap(NULL),
	  m_serverTypeFilepaths(NULL), m_parsed(false), m_script(NULL),
	  m_shell(DUP_NULL(shell)), m_useTTY(useTTY)
{}


Action::~Action()
{
	SAFE_FREE(m_filepath);
	m_plugin = NULL;	// Owned by cache
	SAFE_DELETE(m_actionArgMap);
	SAFE_DELETE(m_functionArgMap);
	SAFE_DELETE(m_args);
	SAFE_DELETE(m_serverTypeFilepaths);
	m_script = NULL;	// Owned by cache
}


const char *Action::filepath(ServerType &type)
{
	Thread::lock(__LINE__,__FILE__);
	if(!m_serverTypeFilepaths) {
		m_model.getServerTypeFilepathsForAction(*this);
	}
	Thread::unlock(__LINE__,__FILE__);
	char key[32];
	sprintf(key, "%d", type.id());
	return m_serverTypeFilepaths ? m_serverTypeFilepaths->get(key) : NULL;
}


Hashtable<ActionArg> *Action::getActionArgMap()
{
	Thread::lock(__LINE__,__FILE__,"actionargmap");
	if(m_actionArgMap) {
		Thread::unlock(__LINE__,__FILE__,"actionargmap");
		return m_actionArgMap;
	}
	List<ActionArg> *args = getArgs();
	m_actionArgMap = new Hashtable<ActionArg>(false, false);
	ListIterator<ActionArg> iter(*args);
	for(ActionArg *arg = iter.first(); arg; arg = iter.next()) {
		m_actionArgMap->put(arg->name(), arg);
	}
	Thread::unlock(__LINE__,__FILE__,"actionargmap");
	return m_actionArgMap;
}


HashtableById<ActionArg> *Action::getFunctionArgMap()
{
	Thread::lock(__LINE__,__FILE__,"functionargmap");;
	if(m_functionArgMap) {
		Thread::unlock(__LINE__,__FILE__,"functionargmap");
		return m_functionArgMap;
	}
	List<ActionArg> *args = getArgs();
	m_functionArgMap = new HashtableById<ActionArg>(false);
	ListIterator<ActionArg> iter(*args);
	for(ActionArg *arg = iter.first(); arg; arg = iter.next()) {
		// Only args with an input position go into the map - always is skipped
		if(arg->position() > 0) {
			m_functionArgMap->put(arg->position(), arg);
		}
	}
	Thread::unlock(__LINE__,__FILE__,"functionargmap");
	return m_functionArgMap;
}


List<ActionArg> *Action::getArgs()
{
	Thread::lock(__LINE__,__FILE__);;
	if(m_args) {
		Thread::unlock(__LINE__,__FILE__);
		return m_args;
	}

	m_args = new List<ActionArg>(true);
	m_model.getArgsForAction(*this);
	Thread::unlock(__LINE__,__FILE__);
	return m_args;
}


void Action::add(ActionArg *arg)
{
	//Thread::lock(__LINE__,__FILE__);
	if(!m_args) {
		m_args = new List<ActionArg>(true);
	}
	m_args->add(arg);
	//Thread::unlock(__LINE__,__FILE__);
}


StringList *Action::mapArgsForFunction(class FunctionNode &func, ExprList *args, Context &ctx, Envp *env /*= NULL*/)
{
	List<ActionArg> *ordargs = getArgs();
	HashtableById<ActionArg> *argmap = getFunctionArgMap();
	int argCount = (args ? args->size() : 0);

	// Count the number of required args
	if(ordargs) {
		int requiredCount = 0;
		ListIterator<ActionArg> oit(*ordargs);
		for(ActionArg *lsca = oit.first(); lsca; lsca = oit.next()) {
			if(lsca->required()) {
				requiredCount++;
			}
		}
		if(argCount < requiredCount) {
			throw RuntimeError(func, ctx.stack(),
				"Function '%s' requires at least %d arguments", m_name, requiredCount);			
		}
	} else if(args) {
		throw RuntimeError(func, ctx.stack(),
			"Function '%s' does not accept arguments", m_name);			
	}

	// Go through provided args and make sure that they are all expected
	for(int n = 1; n <= argCount; n++) {
		ActionArg *arg = argmap ? argmap->get(n) : NULL;
		if(!arg) {
			throw RuntimeError(func, ctx.stack(),
				"Argument %d to function '%s' is unexpected", n, m_name);			
		}
	}

	StringList *params = new StringList();

	// Everything seems okay, go through the args in order
	if(ordargs) {
		Expr **argv = args ? args->toArgv() : NULL;
		ListIterator<ActionArg> oit(*ordargs);
		for(ActionArg *lsca = oit.first(); lsca; lsca = oit.next()) {
			ConstCharPtr val;
			if(!lsca->isAlways()) {
				Expr *expr = (argv && (lsca->position() <= argCount)) ? argv[lsca->position()-1] : NULL;
				if (expr && (expr->kind() == KIND_ARRAY)) {
					DMArray *arr = expr->toArray();
					if (arr) {
						Variable **values = arr->values();
						for(Variable **v = values; v && *v; v++) {
							ConstCharPtr arrval = (*v)->toString();
							lsca->process(arrval, *params, ctx);
						}
						if(env) {
							env->putTri(lsca->name(), expr->stringify());
						}
					}
				} else {
					val = expr ? expr->toString() : NULL;
					lsca->process(val, *params, ctx);
					if(env && val) {
						env->putTri(lsca->name(), val);
					}
				}
			} else {
				lsca->process(NULL,*params,ctx);
			}
			
		}
	}

	// DEBUG
	//StringListIterator iter(*params);
	//for(const char *p = iter.first(); p; p = iter.next()) {
	//	printf("[%s] ", p);
	//}
	//printf("\n");
	// END DEBUG

	return params;
}


StringList *Action::mapArgsForAction(ExtendedStmt &stmt, Context &ctx, Envp *env /*= NULL*/)
{
	StmtList *args = stmt.getArgs();
	List<ActionArg> *ordargs = getArgs();
	Hashtable<ActionArg> *argmap = getActionArgMap();

	// Go through arg map and make sure we have all required args
	if(argmap) {
		AutoPtr<StringList> keys = argmap->keys();
		StringListIterator kit(*keys);
		for(const char *key = kit.first(); key; key = kit.next()) {
			ActionArg *lsca = argmap->get(key);
			if(lsca && lsca->required()) {
				bool found = false;
				if(args) {
					ListIterator<Stmt> ait(*args);
					for(Stmt *arg = ait.first(); arg; arg = ait.next()) {
						if(strcmp(arg->name(), lsca->name()) == 0) {
							debug3("found required arg '%s' for action '%s'", arg->name(), m_name);
							found = true;
							break;
						}
					}
				}
				if(!found) {
					throw RuntimeError(stmt, ctx.stack(),
							"Required arg '%s' must be specified for action '%s'",
							lsca->name(), m_name);
				}
			}
		}
	}

	// Go through provided args and make sure that they are all expected
	// env indicates a local script - so we need to handle the optional dropzone arg
	if(args) {
		ListIterator<Stmt> ait(*args);
		for(Stmt *arg = ait.first(); arg; arg = ait.next()) {
			ActionArg *lsca = argmap ? argmap->get(arg->name()) : NULL;
			if((!lsca || lsca->isAlways()) && (!env || (strcmp(arg->name(), "dropzone") != 0))) {
				throw RuntimeError(stmt, ctx.stack(),
					"Arg '%s' is unknown for action '%s'",
					arg->name(), m_name);
			}
			debug3("found arg '%s' for action '%s'", arg->name(), m_name);
		}
	}

	StringList *params = new StringList();

	// Everything seems okay, go through the args in order
	if(ordargs) {
		ListIterator<ActionArg> oit(*ordargs);
		for(ActionArg *lsca = oit.first(); lsca; lsca = oit.next()) {
			ConstCharPtr val;
			if(!lsca->isAlways()) {
				ExprPtr expr = stmt.getArg(lsca->name(), ctx);
				if (expr && (expr->kind() == KIND_ARRAY)) {
					DMArray *arr = expr->toArray();
					if (arr) {
						Variable **values = arr->values();
						for(Variable **v = values; v && *v; v++) {
							ConstCharPtr arrval = (*v)->toString();
							lsca->process(arrval, *params, ctx);
						}
						if(env) {
							env->putTri(lsca->name(), expr->stringify());
						}
					}
				} else {
					val = expr ? expr->toString() : NULL;
					lsca->process(val, *params, ctx);
					if(env && val) {
						env->putTri(lsca->name(), val);
					}
				}
			} else {
				lsca->process(NULL, *params, ctx);
			}
			
		}
	}

	// DEBUG
	// StringListIterator iter(*params);
	// for(const char *p = iter.first(); p; p = iter.next()) {
	//	printf("[%s] ", p);
	//}
	//printf("\n");
	// END DEBUG

	return params;
}


char *Action::getSignature()
{
	char *ret;

	if(m_isFunction) {
		ret = (char*) malloc(strlen(m_name) + 12);
		sprintf(ret, "function %s(", m_name);

		if(!m_args) {
			getArgs();
		}

		ListIterator<ActionArg> iter(*m_args);
		bool first = true;
		for(ActionArg *aa = iter.first(); aa; aa = iter.next()) {
			char *ret2 = (char*) realloc(ret, strlen(ret) + strlen(aa->name()) + 3);
			if (ret2) {
				ret = ret2;
			}
			if(!first) {
				strcat(ret, ",");
			}
			strcat(ret, aa->name());
			first = false;
		}

		strcat(ret, ")");
	} else {
		ret = (char*) malloc(strlen(m_name) + 8);
		sprintf(ret, "action %s", m_name);
	}

	return ret;
}


Script *Action::getScript()
{
	if(!m_script) {
		m_model.getScriptForAction(this);
	}
	return m_script;
}


/*private*/ void Action::setScript(Script *script)
{
	m_script = script;
}


Plugin *Action::getPlugin()
{
	if(!m_plugin) {
		m_model.getPluginForAction(*this);
	}
	return m_plugin;
}


/*private*/ void Action::setPlugin(Plugin *plugin)
{
	m_plugin = plugin;
}


/*private*/ void Action::setServerTypeFilepaths(StringHashtable *filepaths)
{
	m_serverTypeFilepaths = filepaths;
}


int Action::parse(Context &ctx)
{
	switch(m_actkind) {
	case ACTION_KIND_SCRIPT:
	case ACTION_KIND_IN_DB:
	case ACTION_KIND_GRAPHICAL:
		if(!m_parsed) {
			getScript();
			if(!m_script) {
				throw RuntimeError(ctx.stack(), "Failed to find script for action '%s'", m_name);
			}
			return m_script->parse(*this, ctx);
		}
		break;
	case ACTION_KIND_LOCAL_EXTERNAL:
	case ACTION_KIND_REMOTE_EXTERNAL:
	case ACTION_KIND_PLUGIN:
		// Nothing to parse for external script or plugin
		return 0;
	default:
		throw RuntimeError(ctx.stack(), "Unrecognised action kind (%d)", m_actkind);
		break;
	}

	return 0;
}


class ActionNode *Action::getActionNode(Context &ctx)
{
	if(parse(ctx) != 0) {
		return NULL;
	}
	// TODO: Go to the script for the action, rather than the model
	debug3("Getting action Name(%s)",m_name);
	ActionNode *ret = m_model.getActionNode(m_name);
	if(!ret) {
		throw RuntimeError(ctx.stack(), "Failed to find action '%s'", m_name);	
	}
	return ret;
}


Action *Action::getCalledAction(int actionid)
{
	return m_model.getAction(actionid);
}


void Action::print(int indent)
{}


///////////////////////////////////////////////////////////////////////////////
// Plugin
///////////////////////////////////////////////////////////////////////////////

Plugin::Plugin(int id, int version, const char *library)
	: m_id(id), m_version(version), m_library(DUP_NULL(library)), m_loaded(false)
{}


Plugin::~Plugin()
{
	SAFE_FREE(m_library);
}


bool Plugin::loadPlugin(class DM &dm)
{
	if(!m_loaded) {
		m_loaded = dm.loadPlugin(m_library);
	}
	return m_loaded;
}


///////////////////////////////////////////////////////////////////////////////
// PluginObject
///////////////////////////////////////////////////////////////////////////////

PluginObject::PluginObject()
{}


/*virtual*/ PluginObject::~PluginObject()
{}


///////////////////////////////////////////////////////////////////////////////
// RepositoryTextPattern
///////////////////////////////////////////////////////////////////////////////

RepositoryTextPattern::RepositoryTextPattern(
		const char *path, const char *pattern, bool text
	)
	: m_path(DUP_NULL(path)), m_pattern(DUP_NULL(pattern)), m_text(text)
{}


RepositoryTextPattern::~RepositoryTextPattern()
{
	SAFE_FREE(m_path);
	SAFE_FREE(m_pattern);
}


///////////////////////////////////////////////////////////////////////////////
// RepositoryIgnorePattern
///////////////////////////////////////////////////////////////////////////////

RepositoryIgnorePattern::RepositoryIgnorePattern(
		const char *pattern, bool isDir
	)
	: m_pattern(DUP_NULL(pattern)), m_isDir(isDir), m_re(NULL)
{}


RepositoryIgnorePattern::~RepositoryIgnorePattern()
{
	SAFE_FREE(m_pattern);
	SAFE_DELETE(m_re);
}


bool RepositoryIgnorePattern::matches(const char *filepath, bool isDir)
{
	//printf("RepositoryIgnorePattern::matches('%s', %s)\n", filepath, (isDir ? "true" : "false"));
	if(isDir != m_isDir) {
		return false;
	}
	if(!m_re) {
		// may raise and exception
		m_re = new RegExpWrapper(m_pattern, true);
	}
	return m_re->match(filepath) ? true : false;
}


///////////////////////////////////////////////////////////////////////////////
// Model
///////////////////////////////////////////////////////////////////////////////

Model::Model(class triODBC &odbc, const char *engineHostname)
	: m_odbc(odbc), m_engineHostname(DUP_NULL(engineHostname)),
	  m_config(NULL), m_envs(NULL), m_apps(NULL), m_audit(NULL),
	  m_scrCache(false, true), m_pluginObjects(false, true),
	  m_actionNodes(NULL), m_currentUser(NULL), m_DomainList(NULL)
{
	m_deploymentStartTime = time(NULL);
	debug1("Deployment started at %d", m_deploymentStartTime);
	m_engineId = 1;
}


Model::~Model()
{
	SAFE_DELETE(m_envs);		// not set as owner so will just delete list (envCache is owner)
	SAFE_DELETE(m_apps);		// not set as owner so will just delete list (appCache is owner)
	SAFE_DELETE(m_actionNodes);	// ActionList has a destructor
	m_currentUser = NULL;		// Owned by cache
	SAFE_DELETE(m_audit);
}


class EngineConfig &Model::getEngineConfig()
{
	if(m_config) {
		return *m_config;
	}

	m_config = new EngineConfig();

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	char name[DB_NAME_LEN];
	char value[DB_VARVAL_LEN];
	sql->BindColumn(1, SQL_CHAR, name, sizeof(name));
	sql->BindColumn(2, SQL_CHAR, value, sizeof(value));
	SQLRETURN res = sql->ExecuteSQL("SELECT c.name, c.value FROM dm_engineconfig c WHERE c.engineid=%d", m_engineId);
	if(IS_NOT_SQL_SUCCESS(res)) {
		throw RuntimeError("Failed to get engine config");
	}
	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		debug1("Engine config: '%s' -> '%s'", name, value);
		m_config->put(name, value);
	}
	sql->CloseSQL();
	return *m_config;
}


List<Environment> *Model::internalGetEnvironments(const char *whereClause, bool checkDomain)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char envName[DB_NAME_LEN];
	char envBasedir[DB_PATH_LEN];
	SQLLEN ni_envBasedir = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, envName, sizeof(envName));
	sql->BindColumn(3, SQL_CHAR, envBasedir, sizeof(envBasedir), &ni_envBasedir);

	// If checkDomain is set, limit the domain of the environment to those the user can see
	CharPtr domainClause;
	if(checkDomain) {
		const char *domains = m_currentUser ? m_currentUser->getAccessibleDomains() : NULL;
		if(domains) {
			domainClause = (char*) malloc(strlen(domains) + 25);
			sprintf(domainClause, " AND e.domainid in (%s)", domains);
		}
	}

	int res = sql->ExecuteSQL("select e.id, e.name, e.basedir "
		"from dm_environment e where e.status = 'N' "
		"%s%s%s", (whereClause ? "and " : ""), (whereClause ? whereClause : ""),
		(!domainClause.isNull() ? (const char*) domainClause : ""));
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	List<Environment> *ret = new List<Environment>();

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		Environment *env = findOrCreateEnvironment(id, envName, NULL_IND(envBasedir, NULL));
		ret->add(env);
	}

	return ret;
}

void Model::setDomainList(int domainid /* == 0 */)
{
	if (domainid == 0) {
		// first time through
		m_DomainList = (char *)malloc(128);	// Initial size
		sprintf(m_DomainList,"%d",m_currentUser->getDomain()->id());
		setDomainList(m_currentUser->getDomain()->id());	// recurse
	} else {
		int domid;
		AutoPtr<triSQL> sql = m_odbc.GetSQL();
		int res = sql->BindColumn(1,SQL_INTEGER,&domid,sizeof(domid));
		res = sql->BindParameter(1,SQL_INTEGER,sizeof(domainid),&domainid,sizeof(domainid));
		res = sql->ExecuteSQL("SELECT id FROM dm_domain WHERE domainid=?");
		res = sql->FetchRow();
		while (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
			char szDomainId[128];
			sprintf(szDomainId,",%d",domid);
			int newsize = strlen(m_DomainList)+strlen(szDomainId)+5;
			m_DomainList = (char *)realloc(m_DomainList,newsize);
			strcat(m_DomainList,szDomainId);
			setDomainList(domid);
			res = sql->FetchRow();
		}
		sql->CloseSQL();
	}
}

int Model::getDomainID2(char *DomainName,int parent)
{
	//
	// Domain ID can be fully qualified. (DOMAIN.DOMAIN.DOMAIN...)
	// Since individual domain names need not be unique (UAT can exist in
	// more than one parent domain for example) then we need to make sure
	// that we return an error if more than one is found.
	//
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	char *dn = strdup(DomainName);
	char *dot = strchr(dn,'.');
	int domid=DOMAIN_NOT_FOUND;
	if (dot)
	{
		// DomainName is full qualified - chop off everything after the dot ...
		*dot = '\0';
		int p2 = getDomainID2(dn,parent);	// This should return the ID of the first bit
		if (p2>=0)
		{
			// So far, so good...
			if (*(dot+1))
			{
				domid = getDomainID2(dot+1,p2);
				if (domid<0) {
					// There's been an error - bail out
					free(dn);
					return domid;
				}
			}
		}
		else
		{
			free(dn);
			return p2;
		}
	}
	else
	{
		sql->BindColumn(1,SQL_INTEGER,&domid,sizeof(domid));
		sql->BindParameter(1,SQL_CHAR,strlen(DomainName),DomainName,strlen(DomainName));
		if (parent>0) {
			sql->BindParameter(2,SQL_INTEGER,sizeof(parent),&parent,sizeof(parent));
			sql->ExecuteSQL("SELECT id FROM dm_domain WHERE name=? AND domainid=? AND id IN (%s)",m_DomainList);
		} else {
			sql->ExecuteSQL("SELECT id FROM dm_domain where name=? AND id IN (%s)",m_DomainList);
		}
		int res = sql->FetchRow();
		switch (res) {
			case SQL_SUCCESS:
			case SQL_SUCCESS_WITH_INFO:
				// Found a domain with this name - make sure it's unique
				res =sql->FetchRow();
				if (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
					// Another row was returned - this domain is not unique
					domid=DOMAIN_OBJECT_AMBIGUOUS;
					break;
				}
				break;
			case SQL_NO_DATA:
				domid=DOMAIN_NOT_FOUND;
				break;
			default:
				break;
		}
		sql->CloseSQL();
	}
	free(dn);
	return domid;
}

int Model::getDomainID(char *DomainName)
{
	return getDomainID2(DomainName,0);
}

char *Model::ProcessQualifiedName(const char *QualifiedName,int *domain)
{
	char *dn = strdup(QualifiedName);
	char *dot=(char *)0;
	char *res = (char *)QualifiedName;
	*domain=0;
	int p = strlen(dn)-1;
	while (p>=0)
	{
		if (dn[p]=='.')
		{
			dot = &(dn[p]);
			break;
		}
		p--;
	}
	
	if (dot)
	{
		//
		// Qualified name - up to dot is a domain
		//
		*dot='\0';	// split the name at the dot
		res = (char *)&(QualifiedName[p+1]);
		*domain = getDomainID(dn);
	}
	else
	{
		// No domain specified
		*domain = DOMAIN_NOT_SPECIFIED;
	}
	free(dn);
	return res;
}

int Model::GetIDFromQualifiedName(const char *QualifiedName,char *TableName,bool throwRTE)
{
	int id=0;
	int domid=0;
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	sql->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
	sql->BindColumn(2,SQL_INTEGER,&domid,sizeof(domid));
	int domain;
	debug3("GetIDFromQualifiedName(%s,%s,%s)",QualifiedName,TableName,throwRTE?"true":"false");
	char *bn = ProcessQualifiedName(QualifiedName,&domain);
	debug3("bn=[%s] domain=%d",bn,domain);
	switch(domain)
	{
		case DOMAIN_NOT_FOUND:
			if (throwRTE) throw RuntimeError("Could not find \"%s\": Domain not found",QualifiedName);
			return DOMAIN_NOT_FOUND;
			break;
		case DOMAIN_NOT_SPECIFIED:
			sql->ExecuteSQL("SELECT id,domainid FROM %s WHERE upper(name)=upper('%s') AND domainid IN (%s) AND status='N'",TableName,bn,m_DomainList);
			break;
		case DOMAIN_OBJECT_AMBIGUOUS:
			if (throwRTE) throw RuntimeError("Could not find \"%s\": Domain is ambiguous",QualifiedName);
			return DOMAIN_OBJECT_AMBIGUOUS;
			break;
		default:
			debug3("default");
			if (strcmp(TableName,"dm_task")==0) {
				// dm_task does not have a status column
				sql->ExecuteSQL("SELECT id,domainid FROM %s WHERE upper(name)=upper('%s') AND domainid=%d",TableName,bn,domain);
			} else {
				sql->ExecuteSQL("SELECT id,domainid FROM %s WHERE upper(name)=upper('%s') AND domainid=%d AND status='N'",TableName,bn,domain);
			}
			break;
	}
	int res = sql->FetchRow();
	int n=0;
	while (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO) {
		debug3("got a row...");
		if (n>0) {
			// already found a row - must be ambiguous
			if (throwRTE) throw RuntimeError("Could not find \"%s\": Name is Ambiguous",QualifiedName);
			id = DOMAIN_OBJECT_AMBIGUOUS;
			n++;
			break;
		}
		n++;
		res = sql->FetchRow();
	}
	debug3("n=%d id=%d",n,id);
	sql->CloseSQL();
	if (n==0 && throwRTE) throw RuntimeError("Could not find \"%s\": Object not found or no access",QualifiedName);
	return (n==0)?DOMAIN_NOT_FOUND:id;
}

Environment *Model::internalGetEnvironmentByName(const char *envname,bool throwRTE)
{
	// Environment Name can be fully qualified with domain
	Environment *env = (Environment *)0;
	int envid = GetIDFromQualifiedName(envname,"dm_environment",throwRTE);
	if (envid > 0) {
		env = getEnvironment(envid);
	}
	return env;
}

UserGroup *Model::internalGetUserGroupByName(const char *grpname,bool throwRTE)
{
	// User Group Name can be fully qualified with domain
	UserGroup *grp = (UserGroup *)0;
	int grpid = GetIDFromQualifiedName(grpname,"dm_usergroup",throwRTE);
	if (grpid > 0) {
		grp = getUserGroup(grpid);
	}
	return grp;
}

Credentials *Model::internalGetCredentialsByName(const char *credname,bool throwRTE)
{
	// Credential Name can be fully qualified with domain
	Credentials *cred = (Credentials *)0;
	int credid = GetIDFromQualifiedName(credname,"dm_credentials",throwRTE);
	debug3("credid=%d",credid);
	if (credid > 0) {
		 cred = getCredentials(credid);
		 debug3("cred=0x%lx",cred);
		 if (cred) debug3("cred->name=%s",cred->name());
	}
	return cred;
}

Application *Model::internalGetApplicationByName(const char *appname,bool throwRTE)
{
	Application *app = (Application *)0;
	int appid = GetIDFromQualifiedName(appname,"dm_application",throwRTE);
	if (appid > 0) {
		app = getApplication(appid);
	}
	return app;
}

Server *Model::internalGetServerByName(const char *servname,bool throwRTE)
{
	Server *serv = (Server *)0;
	int servid = GetIDFromQualifiedName(servname,"dm_server",throwRTE);
	if (servid > 0) {
		serv = getServer(servid);
	}
	return serv;
}

Component *Model::internalGetComponentByName(const char *compname,bool throwRTE)
{
	Component *comp = (Component *)0;
	int compid = GetIDFromQualifiedName(compname,"dm_component",throwRTE);
	if (compid > 0) {
		comp = getComponentById(compid);
	}
	return comp;
}

User *Model::internalGetUserByName(const char *username,bool throwRTE)
{
	User *user = (User *)0;
	int userid = GetIDFromQualifiedName(username,"dm_user",throwRTE);
	if (userid > 0) {
		user = getUserById(userid);
	}
	return user;
}

Repository *Model::internalGetRepositoryByName(const char *repname,bool throwRTE)
{
	// Repository Name can be fully qualified with domain
	Repository *rep = (Repository *)0;
	int repoid = GetIDFromQualifiedName(repname,"dm_repository",throwRTE);
	if (repoid > 0) {
		rep = getRepository(repoid);
	}
	return rep;
}


List<Server> *Model::internalGetServers(const char *fromClause, const char *whereClause)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char serverName[DB_NAME_LEN];
	char serverHostname[DB_HOSTNAME_LEN];
	char serverProtocol[65];
	char serverBasedir[DB_PATH_LEN];
	char scriptext[DB_SCRIPTEXT_LEN];
	int stid = 0;
	char serverHosttype[DB_NAME_LEN];
	LINE_END_FORMAT lineends = (LINE_END_FORMAT) 0;
	char pathformat[DB_NAME_LEN];
	char uname[257];
	int sshPort;
	SQLLEN ni_serverHostname = 0, ni_serverProtocol = 0,
		ni_serverBasedir = 0, ni_stid = 0, ni_serverHosttype = 0,
		ni_lineends = 0, ni_pathformat = 0, ni_scriptext = 0, ni_uname = 0,
		ni_sshPort = 0, ni_usetty = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, serverName, sizeof(serverName));
	sql->BindColumn(3, SQL_CHAR, serverHostname, sizeof(serverHostname), &ni_serverHostname);
	sql->BindColumn(4, SQL_CHAR, serverProtocol, sizeof(serverProtocol), &ni_serverProtocol);
	sql->BindColumn(5, SQL_CHAR, serverBasedir, sizeof(serverBasedir), &ni_serverBasedir);
	sql->BindColumn(6, SQL_INTEGER, &stid, sizeof(stid), &ni_stid);
	sql->BindColumn(7, SQL_CHAR, serverHosttype, sizeof(serverHosttype), &ni_serverHosttype);
	sql->BindColumn(8, SQL_INTEGER, &lineends, sizeof(lineends), &ni_lineends);
	sql->BindColumn(9, SQL_CHAR, pathformat, sizeof(pathformat), &ni_pathformat);
	sql->BindColumn(10, SQL_CHAR, scriptext, sizeof(scriptext), &ni_scriptext);
	sql->BindColumn(11, SQL_CHAR, uname, sizeof(uname), &ni_uname);
	sql->BindColumn(12, SQL_INTEGER, &sshPort, sizeof(sshPort), &ni_sshPort);

	List<Server> *ret = new List<Server>();

	int res = sql->ExecuteSQL(
			"select distinct s.id, s.name, s.hostname, s.protocol, s.basedir, "
			" st.id, st.name, st.lineends, st.pathformat,st.scriptext, s.uname, s.sshport "
			"from (dm_server s left join dm_servertype st on s.typeid = st.id) %s%s "
			"where s.status = 'N' %s%s",
			(fromClause ? ", " : ""), (fromClause ? fromClause : ""),
			(whereClause ? "and " : ""), (whereClause ? whereClause : ""));
	if(IS_NOT_SQL_SUCCESS(res)) {
		printf("execute did not return success (returned %d)\n",res);
		return ret;	// empty list
	}

	

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		ServerType *st = findOrCreateServerType(
			NULL_IND(stid, 0), NULL_IND(serverHosttype, NULL),
			NULL_IND(lineends, LINE_ENDS_OFF), NULL_IND(pathformat, NULL), NULL_IND(scriptext,NULL));
		Server *srv = findOrCreateServer(
			id, serverName, NULL_IND(serverHostname, NULL),
			NULL_IND(serverProtocol, NULL), NULL_IND(serverBasedir, NULL), st, NULL_IND(uname, NULL));
		if (ni_sshPort != -1) {
			srv->setSSHPort(sshPort);
		}
		ret->add(srv);
	}

	return ret;
}


List<Domain> *Model::internalGetDomains(const char *fromClause, const char *whereClause)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	int pos = 0;
	SQLLEN ni_pos,ni_lifecycle;
	char domainName[DB_NAME_LEN];
	char lifecycle[2];

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, domainName, sizeof(domainName));
	sql->BindColumn(3, SQL_INTEGER, &pos, sizeof(pos),&ni_pos);
	sql->BindColumn(4, SQL_CHAR, lifecycle, sizeof(lifecycle),&ni_lifecycle);

	int res = sql->ExecuteSQL("select d.id, d.name, d.position, d.lifecycle "
			"from dm_domain d %s%s where d.status = 'N' %s%s",
			(fromClause ? ", " : ""), (fromClause ? fromClause : ""),
			(whereClause ? "and " : ""), (whereClause ? whereClause : ""));
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	List<Domain> *ret = new List<Domain>();

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		Domain *domain = findOrCreateDomain(id, domainName);
		domain->setPosition(NULL_IND(pos,0));
		domain->setLifecycle(BOOL_NULL_IND(lifecycle,false));
		ret->add(domain);
	}

	return ret;
}


List<Application> *Model::internalGetApplications(const char *fromClause, const char *whereClause, bool checkDomain)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char appName[DB_NAME_LEN];
	char appVers[DB_NAME_LEN];
	int parentid = 0;
	char isRelease[2];
	SQLLEN ni_appVers = 0, ni_parentid = 0, ni_isRelease = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, appName, sizeof(appName));
	sql->BindColumn(3, SQL_CHAR, appVers, sizeof(appVers), &ni_appVers);
	sql->BindColumn(4, SQL_INTEGER, &parentid, sizeof(parentid), &ni_parentid);
	sql->BindColumn(5, SQL_CHAR, isRelease, sizeof(isRelease), &ni_isRelease);

	// If checkDomain is set, limit the domain of the environment to those the user can see
	CharPtr domainClause;
	if(checkDomain) {
		const char *domains = m_currentUser ? m_currentUser->getAccessibleDomains() : NULL;
		if(domains) {
			domainClause = (char*) malloc(strlen(domains) + 25);
			sprintf(domainClause, " and a.domainid in (%s)", domains);
		}
	}

	debug1("select a.id, a.name, a.version, a.parentid, a.isrelease "
		"from dm_application a %s%s where a.status = 'N' %s%s%s",
		(fromClause ? ", " : ""), (fromClause ? fromClause : ""),
		(whereClause ? "and " : ""), (whereClause ? whereClause : ""),
		(!domainClause.isNull() ? (const char*) domainClause : ""));

	int res = sql->ExecuteSQL("select a.id, a.name, a.version, a.parentid, a.isrelease "
		"from dm_application a %s%s where a.status = 'N' %s%s%s",
		(fromClause ? ", " : ""), (fromClause ? fromClause : ""),
		(whereClause ? "and " : ""), (whereClause ? whereClause : ""),
		(!domainClause.isNull() ? (const char*) domainClause : ""));

	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	List<Application> *ret = new List<Application>(false);

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		Application *app = NULL;
		debug1("parentid=%d",parentid);
		if(NULL_IND(parentid, 0)) {
			debug1("retrieving parent");
			// Retrieve the parent
			char clause[1024];
			sprintf(clause, "a.id = %d", parentid);
			AutoPtr<List<Application> > matches = internalGetApplications(NULL, clause, false);
			if(matches) {
				ListIterator<Application> mit(*matches);
				Application *parent = mit.first();
				if(parent) {
					app = findOrCreateApplicationVersion(id, appName, NULL_IND(appVers, NULL), parent);
				} else {
					debug1("Failed to get parent application with id %d", parentid);
				}
			} else {
				debug1("Parent application query failed");
			}
		} else {
			app = findOrCreateApplication(id, appName);
		}

		if(app) {
			char *rel = NULL_IND(isRelease,NULL);
			app->setRelease(rel && tolower(rel[0])=='y');
			ret->add(app);
		}
	}

	sql->CloseSQL();

	return ret;
}


List<Component> *Model::internalGetComponents(const char *fromClause, const char *whereClause)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char compName[DB_NAME_LEN];
	char baseDir[DB_PATH_LEN];
	int rollup;
	int rollback;
	char filterItems[DB_BOOL_LEN];
	char deployAlways[DB_BOOL_LEN];
	char deploySequentially[DB_BOOL_LEN];
	int parentid;
	int predecessorid;
	int datasourceid;
	int buildid;
	SQLLEN ni_baseDir = 0, ni_rollup = 0, ni_rollback = 0,
		ni_filterItems = 0, ni_deployAlways = 0, ni_deploySequentially,
		ni_parentid = 0, ni_predecessorid, ni_datasourceid, ni_buildid;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, compName, sizeof(compName));
	sql->BindColumn(3, SQL_CHAR, baseDir, sizeof(baseDir),&ni_baseDir);
	sql->BindColumn(4, SQL_INTEGER, &rollup, sizeof(rollup), &ni_rollup);
	sql->BindColumn(5, SQL_INTEGER, &rollback, sizeof(rollback), &ni_rollback);
	sql->BindColumn(6, SQL_CHAR, filterItems, sizeof(filterItems), &ni_filterItems);
	sql->BindColumn(7, SQL_CHAR, deployAlways, sizeof(deployAlways), &ni_deployAlways);
	sql->BindColumn(8, SQL_INTEGER, &parentid, sizeof(parentid), &ni_parentid);
	sql->BindColumn(9, SQL_INTEGER, &predecessorid, sizeof(parentid), &ni_predecessorid);
	sql->BindColumn(10, SQL_CHAR, deploySequentially, sizeof(deploySequentially), &ni_deploySequentially);
	sql->BindColumn(11, SQL_INTEGER, &datasourceid, sizeof(datasourceid), &ni_datasourceid);
	sql->BindColumn(12, SQL_INTEGER, &buildid, sizeof(buildid), &ni_buildid);

	// PAG mod 11/02/2015 - add parentid so we can determine component versions
	// PAG mod 16/03/2016 - add predecessorid so we can set the right component against server when rolling back
	debug1("select c.id, c.name, c.basedir, c.rollup, c.rollback, c.filteritems, c.deployalways, c.parentid, c.predecessorid, c.deploysequentially, c.datasourceid, c.lastbuildnumber "
		"from dm_component c %s%s where c.status = 'N' %s%s",
		(fromClause ? ", " : ""), (fromClause ? fromClause : ""),
		(whereClause ? "and " : ""), (whereClause ? whereClause : ""));


	int res = sql->ExecuteSQL(
		"select c.id, c.name, c.basedir, c.rollup, c.rollback, c.filteritems, c.deployalways, c.parentid, c.predecessorid, c.deploysequentially, c.datasourceid, c.lastbuildnumber "
		"from dm_component c %s%s where c.status = 'N' %s%s",
		(fromClause ? ", " : ""), (fromClause ? fromClause : ""),
		(whereClause ? "and " : ""), (whereClause ? whereClause : ""));
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	List<Component> *ret = new List<Component>(false);

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		Component *comp = findOrCreateComponent(
			id, compName, STR_NULL_IND(baseDir, NULL), (ComponentFilter) NULL_IND(rollup, false),
			(ComponentFilter) NULL_IND(rollback, false),
			BOOL_NULL_IND(filterItems, false), BOOL_NULL_IND(deployAlways, false), 
			BOOL_NULL_IND(deploySequentially, false), NULL_IND(parentid,0),
			NULL_IND(predecessorid,0),NULL_IND(datasourceid,0),NULL_IND(buildid,0));
		ret->add(comp);
	}

	return ret;
}


List<User> *Model::internalGetUsers(
	const char *fromClause, const char *whereClause, bool checkDomain)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char userName[DB_NAME_LEN];
	char userEmail[DB_EMAIL_LEN];
	char realName[DB_NAME_LEN];
	char userPhone[DB_PHONE_LEN];
	SQLLEN ni_userEmail = 0, ni_realName = 0, ni_userPhone = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, userName, sizeof(userName));
	sql->BindColumn(3, SQL_CHAR, userEmail, sizeof(userEmail), &ni_userEmail);
	sql->BindColumn(4, SQL_CHAR, realName, sizeof(realName), &ni_realName);
	sql->BindColumn(5, SQL_CHAR, userPhone, sizeof(userPhone), &ni_userPhone);

	// If checkDomain is set, limit the domain of the environment to those the user can see
	CharPtr domainClause;
	if(checkDomain) {
		const char *domains = m_currentUser ? m_currentUser->getAccessibleDomains() : NULL;
		if(domains) {
			domainClause = (char*) malloc(strlen(domains) + 25);
			sprintf(domainClause, " and u.domainid in (%s)", domains);
		}
	}

	int res = sql->ExecuteSQL("select u.id, u.name, u.email, u.realname, u.phone "
		"from dm_user u %s%s where u.status = 'N' %s%s%s",
		(fromClause ? ", " : ""), (fromClause ? fromClause : ""),
		(whereClause ? "and " : ""), (whereClause ? whereClause : ""),
		(!domainClause.isNull() ? (const char*) domainClause : ""));
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	List<User> *ret = new List<User>(false);

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		User *user = findOrCreateUser(id, userName, NULL_IND(userEmail, NULL),
			NULL_IND(realName, NULL), NULL_IND(userPhone,NULL));
		ret->add(user);
	}

	return ret;
}


List<UserGroup> *Model::internalGetUserGroups(const char *fromClause, const char *whereClause, bool checkDomain)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char groupName[DB_NAME_LEN];
	char groupEmail[DB_EMAIL_LEN];
	SQLLEN ni_groupEmail = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, groupName, sizeof(groupName));
	sql->BindColumn(3, SQL_CHAR, groupEmail, sizeof(groupEmail), &ni_groupEmail);

	// If checkDomain is set, limit the domain of the environment to those the user can see
	CharPtr domainClause;
	if(checkDomain) {
		const char *domains = m_currentUser ? m_currentUser->getAccessibleDomains() : NULL;
		if(domains) {
			domainClause = (char*) malloc(strlen(domains) + 25);
			sprintf(domainClause, " and g.domainid in (%s)", domains);
		}
	}

	int res = sql->ExecuteSQL("select g.id, g.name, g.email "
		"from dm_usergroup g %s%s where g.status = 'N' %s%s%s",
		(fromClause ? ", " : ""), (fromClause ? fromClause : ""),
		(whereClause ? "and " : ""), (whereClause ? whereClause : ""),
		(!domainClause.isNull() ? (const char*) domainClause : ""));
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	List<UserGroup> *ret = new List<UserGroup>(false);

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		UserGroup *group = findOrCreateUserGroup(id, groupName, NULL_IND(groupEmail, NULL));
		ret->add(group);
	}

	return ret;
}


Credentials *Model::internalGetCredentials(const char *fromClause, const char *whereClause)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char name[DB_NAME_LEN];
	CREDENTIALS_KIND kind;
	char encusername[2049];
	char encpassword[2049];
	char filename[DB_PATH_LEN];
	SQLLEN ni_name = 0, ni_encusername = 0, ni_encpassword = 0, ni_filename = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, name, sizeof(name), &ni_name);
	sql->BindColumn(3, SQL_INTEGER, &kind, sizeof(kind));
	sql->BindColumn(4, SQL_CHAR, encusername, sizeof(encusername), &ni_encusername);
	sql->BindColumn(5, SQL_CHAR, encpassword, sizeof(encpassword), &ni_encpassword);
	sql->BindColumn(6, SQL_CHAR, filename, sizeof(filename), &ni_filename);

	int res;
	if (fromClause) {
		res = sql->ExecuteSQL("select c.id, c.name, c.kind, c.encusername, c.encpassword, c.filename "
				"from %s t, dm_credentials c where c.id = t.credid and %s ",
				fromClause, whereClause);
	} else {
		res = sql->ExecuteSQL("select c.id, c.name, c.kind, c.encusername, c.encpassword, c.filename "
				"from dm_credentials c where %s ",
				whereClause);
	}
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	debug3("about to fetch row");

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		debug3("findOrCreateCredentials(id=%d name=[%s])",id,name);
		return findOrCreateCredentials(id, NULL_IND(name, NULL), kind,
			NULL_IND(encusername, NULL), NULL_IND(encpassword, NULL),
			NULL_IND(filename, NULL));
	}

	return NULL;
}


Action *Model::internalGetAction(const char *fromClause, const char *whereClause, bool checkDomain)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char name[DB_NAME_LEN];
	char shell[DB_SHELL_LEN];
	ACTION_KIND kind;
	char filepath[DB_PATH_LEN];
	int textid = 0;
	int repoid = 0;
	char isFunc[DB_BOOL_LEN];
	char useTTY[DB_BOOL_LEN];
	char copyToRemote[DB_BOOL_LEN];
	char resIsExpr[DB_BOOL_LEN];
	int pluginid = 0;
	SQLLEN ni_name = 0, ni_filepath = 0, ni_textid = 0,
		ni_repoid = 0,ni_copyToRemote = 0,
		ni_resIsExpr = 0, ni_pluginid = 0, ni_shell = 0, ni_useTTY = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, name, sizeof(name), &ni_name);
	sql->BindColumn(3, SQL_INTEGER, &kind, sizeof(kind));
	sql->BindColumn(4, SQL_CHAR, filepath, sizeof(filepath), &ni_filepath);
	sql->BindColumn(5, SQL_INTEGER, &textid, sizeof(textid), &ni_textid);
	sql->BindColumn(6, SQL_INTEGER, &repoid, sizeof(repoid), &ni_repoid);
	sql->BindColumn(7, SQL_CHAR, isFunc, sizeof(isFunc));
	sql->BindColumn(8, SQL_CHAR, copyToRemote, sizeof(copyToRemote), &ni_copyToRemote);
	sql->BindColumn(9, SQL_CHAR, resIsExpr, sizeof(resIsExpr), &ni_resIsExpr);
	sql->BindColumn(10, SQL_INTEGER, &pluginid, sizeof(pluginid), &ni_pluginid);
	sql->BindColumn(11, SQL_CHAR, shell, sizeof(shell), &ni_shell);
	sql->BindColumn(12, SQL_CHAR, useTTY, sizeof(useTTY), &ni_useTTY);

	// If checkDomain is set, limit the domain of the environment to those the user can see
	CharPtr domainClause;
	if(checkDomain) {
		const char *domains = m_currentUser ? m_currentUser->getAccessibleDomains() : NULL;
		if(domains) {
			domainClause = (char*) malloc(strlen(domains) + 25);
			sprintf(domainClause, " %s a.domainid in (%s)",
				(whereClause ? "and" : "where"), domains);
		}
	}

	int res = sql->ExecuteSQL(
		"select a.id, a.name, a.kind, a.filepath, a.textid, a.repositoryid, "
		" a.function, a.copy, a.resultisexpr, a.pluginid, a.interpreter, a.usetty "
		"from dm_action a %s%s where a.status = 'N' %s%s%s",
		(fromClause ? ", " : ""), (fromClause ? fromClause : ""),
		(whereClause ? "and " : ""), (whereClause ? whereClause : ""),
		(!domainClause.isNull() ? (const char*) domainClause : ""));
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		return findOrCreateAction(
			id, kind, ((isFunc[0] == 'Y') ? true : false),
			BOOL_NULL_IND(copyToRemote, false),
			BOOL_NULL_IND(resIsExpr, false),
			NULL_IND(name, NULL),
			NULL_IND(filepath, NULL),
			NULL_IND(shell, NULL),
			BOOL_NULL_IND(useTTY,false));
	}

	return NULL;
}


List<Property> *Model::internalGetProperties(const char *table, const char *fk, int id, const char *fk2, int id2)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char propName[DB_NAME_LEN];
	char propValue[DB_VARVAL_LEN];
	char propEnc[DB_BOOL_LEN];
	char propOver[DB_BOOL_LEN];
	char propApnd[DB_BOOL_LEN];
	SQLLEN ni_propValue = 0, ni_propEnc = 0, ni_propOver = 0, ni_propApnd = 0;

	sql->BindColumn(1, SQL_CHAR, propName, sizeof(propName));
	sql->BindColumn(2, SQL_CHAR, propValue, sizeof(propValue), &ni_propValue);
	sql->BindColumn(3, SQL_CHAR, propEnc, sizeof(propEnc), &ni_propEnc);
	sql->BindColumn(4, SQL_CHAR, propOver, sizeof(propOver), &ni_propOver);
	sql->BindColumn(5, SQL_CHAR, propApnd, sizeof(propApnd), &ni_propApnd);

	int res;
	if(fk2) {
		res = sql->ExecuteSQL("select p.name, p.value, p.encrypted, "
				"p.overridable, p.appendable "
				"from %sprops p where p.%s = %d and p.%s = %d ", table, fk, id, fk2, id2);
	} else {
		res = sql->ExecuteSQL("select p.name, p.value, p.encrypted, "
				"p.overridable, p.appendable "
				"from %sprops p where p.%s = %d ", table, fk, id);
	}
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	List<Property> *ret = new List<Property>();

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		ret->add(new Property(propName,
			NULL_IND(propValue, NULL), BOOL_NULL_IND(propEnc, false),
			BOOL_NULL_IND(propOver, false), BOOL_NULL_IND(propApnd, false)));
	}

	return ret;
}


List<Environment> *Model::getEnvironments()
{
	if(m_envs) {
		return m_envs;
	}
	m_envs = internalGetEnvironments(NULL, true);
	return m_envs;
}


List<Application> *Model::getApplications()
{
	if(m_apps) {
		return m_apps;
	}
	m_apps = internalGetApplications(NULL, NULL, true);
	return m_apps;
}

List <Application> *Model::getApplications(Environment *env)
{
	List<Application> *ret = new List<Application>(true);
	int appid;

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	sql->BindColumn(1, SQL_INTEGER, &appid, sizeof(appid));
	printf("select appid from dm.dm_appsinenv where envid=%d",env->id());
	SQLRETURN res = sql->ExecuteSQL("select appid from dm.dm_appsinenv where envid=%d",env->id());
	if(IS_SQL_SUCCESS(res)) {
		for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
			Application *app = getApplication(appid);
			if (app) {
				printf("adding app %d (%s)\n",app->id(),app->name());
				ret->add(app);
			}
		}
	}
	sql->CloseSQL();
	return ret;
}

Environment *Model::getEnvironment(const char *name,bool throwRTE)
{
	Environment *ret = internalGetEnvironmentByName(name,throwRTE);
	if (ret) {
		if (ret->hasReadAccess(getCurrentUser())) {
			m_envCache.put(ret);
			return ret;
		} else {
			throw RuntimeError("Environment \"%s\" cannot be found or no access",name);
		}
	}
	return ret;
}

Repository *Model::getRepository(const char *name,bool throwRTE)
{
	Repository *ret = internalGetRepositoryByName(name,throwRTE);
	if (ret) {
		if (ret->hasReadAccess(getCurrentUser())) {
			m_repCache.put(ret);
			return ret;
		} else {
			throw RuntimeError("Repository \"%s\" cannot be found or no access",name);
		}
	}
	return ret;
}

Credentials *Model::getCredential(const char *name,bool throwRTE)
{	
	Credentials *cred = internalGetCredentialsByName(name,throwRTE);
	if (cred) {
		if (cred->hasReadAccess(getCurrentUser())) {
			m_crdCache.put(cred);
			return cred;
		} else {
			throw RuntimeError("Credential \"%s\" cannot be found or no access",name);
		}
	}
	return cred;
}

Application *Model::getApplication(const char *name,bool throwRTE)
{
	Application *app = internalGetApplicationByName(name,throwRTE);
	if (app) {
		if (app->hasReadAccess(getCurrentUser())) {
			m_appCache.put(app);
			return app;
		} else {
			throw RuntimeError("Application \"%s\" cannot be found or no access",name);
		}
	}
	return app;
}

Server *Model::getServer(const char *name,bool throwRTE)
{
	Server *serv = internalGetServerByName(name,throwRTE);
	return serv;
}


Environment *Model::getEnvironment(int id)
{
	// Have we already got this environment
	Environment *ret = m_envCache.get(id);
	if(ret) { return ret; }

	char whereClause[2100];
	sprintf(whereClause, "e.id = %d", id);

	AutoPtr<List<Environment> > matches = internalGetEnvironments(whereClause, true);
	if(matches && (matches->size() > 0)) {
		ListIterator<Environment> iter(*matches);
		ret = iter.first();
	}

	if (ret) {
		m_envCache.put(ret);
	}
	return ret;
}

Credentials *Model::getCredentials(int id)
{
	Credentials *ret = m_crdCache.get(id);
	if (ret) { return ret; }

	char whereClause[2100];
	sprintf(whereClause, "c.id = %d", id);

	ret = internalGetCredentials(NULL,whereClause);
	if (ret) {
		m_crdCache.put(ret);
	}
	return ret;
}

Application *Model::getApplication(int id)
{
	// Have we already got this application
	Application *ret = m_appCache.get(id);
	if(ret) { return ret; }

	char whereClause[2100];
	sprintf(whereClause, "a.id = %d", id);

	AutoPtr<List<Application> > matches = internalGetApplications(NULL, whereClause, true);
	if(matches && (matches->size() > 0)) {
		ListIterator<Application> iter(*matches);
		ret = iter.first();
	}

	return ret;
}

Server *Model::getServer(int id)
{
	// Have we already got this application
	Server *ret = m_srvCache.get(id);
	if(ret) { return ret; }

	char whereClause[2100];
	sprintf(whereClause, "s.id = %d", id);

	AutoPtr<List<Server> > matches = internalGetServers(NULL, whereClause);
	if(matches && (matches->size() > 0)) {
		ListIterator<Server> iter(*matches);
		ret = iter.first();
	}

	return ret;
}

DateTime *Model::getDeployTime(Component *comp,Server *serv)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int deploymentid = 0;
	SQLLEN ni_deploymentid = 0;
	int modified = 0;
	SQLLEN ni_modified = 0;
	int finished;
	int ret=0;

	sql->BindColumn(1, SQL_INTEGER, &deploymentid, sizeof(deploymentid),&ni_deploymentid);
	sql->BindColumn(2, SQL_INTEGER, &modified, sizeof(modified),&ni_modified);
	
	int res = sql->ExecuteSQL(
		"select deploymentid,modified from dm.dm_compsonserv where compid=%d and serverid=%d",
		comp->id(),serv->id());
	if(IS_NOT_SQL_SUCCESS(res)) return (DateTime *)0;
	res = sql->FetchRow();
	sql->CloseSQL();
	if(IS_NOT_SQL_SUCCESS(res)) return (DateTime *)0;	// no row found, return NULL.
	if (IS_NULL(deploymentid)) {
		// Manually added component to server.
		ret = modified;
	} else {
		// Component reached server due to a deployment, get the deployment time.
		sql->BindColumn(1, SQL_INTEGER, &finished, sizeof(finished));
		int res = sql->ExecuteSQL("select finished from dm.dm_deployment where deploymentid=%d",deploymentid);
		if (IS_NOT_SQL_SUCCESS(res)) return new DateTime(0);
		res = sql->FetchRow();
		sql->CloseSQL();
		if(IS_SQL_SUCCESS(res)) {
			ret = finished;
		}
	}
	return new DateTime(ret);
}

DateTime *Model::getDeployTime(Application *app,Environment *env)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int finished=0;
	SQLLEN ni_finished = 0;
	int ret=0;

	sql->BindColumn(1, SQL_INTEGER, &finished, sizeof(finished),&ni_finished);
	int res = sql->ExecuteSQL(
		"select max(finished) from dm.dm_deployment where appid=%d and envid=%d",
		app->id(),env->id());
	if(IS_NOT_SQL_SUCCESS(res)) return (DateTime *)0;
	res = sql->FetchRow();
	sql->CloseSQL();
	if(IS_NOT_SQL_SUCCESS(res)) return (DateTime *)0;
	if (IS_NULL(finished)) return (DateTime *)0;
	return new DateTime(finished);
}

Component *Model::getComponent(const char *name,bool throwRTE)
{
	Component *comp = internalGetComponentByName(name,throwRTE);
	if (comp) {
		m_comCache.put(comp);
		return comp;
	}
	return comp;

	/*
	// Have we already got this component
	Component *ret = m_comCache.get(name);
	if(ret) { return ret; }

	char *nameLit = triSQL::EscapeLiteral(name);
	char whereClause[2100];
	sprintf(whereClause, "c.name = %s", nameLit);
	SAFE_FREE(nameLit);

	AutoPtr<List<Component> > matches = internalGetComponents(NULL, whereClause);
	if(matches && (matches->size() > 0)) {
		ListIterator<Component> iter(*matches);
		ret = iter.first();
	}

	return ret;
	*/
}

Component *Model::getComponentById(int compid)
{
	Component *ret = NULL;
	char whereClause[128];
	sprintf(whereClause, "c.id = %d", compid);

	AutoPtr<List<Component> > matches = internalGetComponents(NULL, whereClause);
	if(matches && (matches->size() > 0)) {
		ListIterator<Component> iter(*matches);
		ret = iter.first();
	}

	return ret;
}


User *Model::getUser(const char *name, bool throwRTE /*=false*/)
{
	// Have we already got this user
	User *ret = m_usrCache.get(name);
	if(ret) { return ret; }

	if (m_DomainList) {
		User *user = internalGetUserByName(name,throwRTE);
		if (user) {
			m_usrCache.put(user);
			return user;
		}
		return user;
	}
	//
	// If domain list is not yet set drop into here.
	//
	char *nameLit = triSQL::EscapeLiteral(name);
	char whereClause[1024];
	sprintf(whereClause, "u.name = %s", nameLit);
	SAFE_FREE(nameLit);

	AutoPtr<List<User> > matches = internalGetUsers(NULL, whereClause, true);
	if(matches && (matches->size() > 0)) {
		ListIterator<User> iter(*matches);
		ret = iter.first();
	}

	return ret;
}

UserGroup *Model::getUserGroup(const char *name,bool throwRTE)
{
	UserGroup *ret = internalGetUserGroupByName(name,throwRTE);
	if (ret) {
		if (ret->hasReadAccess(getCurrentUser())) {
			return ret;
		} else {
			throw RuntimeError("User Group \"%s\" cannot be found or no access",name);
		}
	}
	return ret;
}


NotifyTemplate *Model::getSuccessTemplate(Context &ctx,int taskid)
{
	Task *task = getTask(taskid);
	return task?task->getSuccessTemplate(ctx):NULL;
}

NotifyTemplate *Model::getFailureTemplate(Context &ctx,int taskid)
{
	Task *task = getTask(taskid);
	return task?task->getFailureTemplate(ctx):NULL;
}

User *Model::getUserById(int id)
{
	// Have we already got this user
	User *ret = m_usrCache.get(id);
	if(ret) { return ret; }

	char whereClause[1024];
	sprintf(whereClause, "u.id = %d", id);

	AutoPtr<List<User> > matches = internalGetUsers(NULL, whereClause, true);
	if(matches && (matches->size() > 0)) {
		ListIterator<User> iter(*matches);
		ret = iter.first();
	}

	return ret;
}


UserGroup *Model::getUserGroup(int id)
{
	// Have we already got this user group
	UserGroup *ret = m_usgCache.get(id);
	if(ret) { return ret; }

	char whereClause[1024];
	sprintf(whereClause, "g.id = %d", id);

	AutoPtr<List<UserGroup> > matches = internalGetUserGroups(NULL, whereClause, true);
	if(matches && (matches->size() > 0)) {
		ListIterator<UserGroup> iter(*matches);
		ret = iter.first();
	}

	return ret;
}

Repository *Model::getRepository(int id)
{
	// Have we already got this repository
	Repository *ret = m_repCache.get(id);
	if(ret) { return ret; }

	char whereClause[1024];
	sprintf(whereClause, "t.id = %d", id);

	AutoPtr<List<Repository> > matches = internalGetProviderObjects<Repository>(
		m_repCache, NULL, whereClause, true);
	if(matches && (matches->size() > 0)) {
		ListIterator<Repository> iter(*matches);
		ret = iter.first();
	}

	return ret;
}


Notify *Model::getNotifier(const char *name)
{
	// Have we already got this notifier
	Notify *ret = m_nfyCache.get(name);
	if(ret) { return ret; }

	char *nameLit = triSQL::EscapeLiteral(name);
	char whereClause[1024];
	sprintf(whereClause, "t.name = %s", nameLit);
	SAFE_FREE(nameLit);

	AutoPtr<List<Notify> > matches = internalGetProviderObjects<Notify>(
		m_nfyCache, NULL, whereClause, true);
	if(matches && (matches->size() > 0)) {
		ListIterator<Notify> iter(*matches);
		ret = iter.first();
	}

	return ret;
}


Notify *Model::getNotifier(int id)
{
	// Have we already got this notifier
	Notify *ret = m_nfyCache.get(id);
	if(ret) { return ret; }

	char whereClause[1024];
	sprintf(whereClause, "t.id = %d", id);

	AutoPtr<List<Notify> > matches = internalGetProviderObjects<Notify>(
		m_nfyCache, NULL, whereClause, true);
	if(matches && (matches->size() > 0)) {
		ListIterator<Notify> iter(*matches);
		ret = iter.first();
	}

	return ret;
}


Datasource *Model::getDatasource(const char *name)
{
	// Have we already got this datasource
	Datasource *ret = m_datCache.get(name);
	if(ret) { return ret; }

	char *nameLit = triSQL::EscapeLiteral(name);
	char whereClause[1024];
	sprintf(whereClause, "t.name = %s", nameLit);
	SAFE_FREE(nameLit);

	AutoPtr<List<Datasource> > matches = internalGetProviderObjects<Datasource>(
		m_datCache, NULL, whereClause, true);
	if(matches && (matches->size() > 0)) {
		ListIterator<Datasource> iter(*matches);
		ret = iter.first();
	}

	return ret;
}


Datasource *Model::getDatasource(int id)
{
	// Have we already got this datasource
	Datasource *ret = m_datCache.get(id);
	if(ret) { return ret; }

	char whereClause[1024];
	sprintf(whereClause, "t.id = %d", id);

	AutoPtr<List<Datasource> > matches = internalGetProviderObjects<Datasource>(
		m_datCache, NULL, whereClause, true);
	if(matches && (matches->size() > 0)) {
		ListIterator<Datasource> iter(*matches);
		ret = iter.first();
	}

	return ret;
}


Field *Model::getField(const char *name)
{
	// Have we already got this field
	Field *ret = m_fldCache.get(name);
	if(ret) { return ret; }

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	char *nameLit = triSQL::EscapeLiteral(name);

	int id = 0;
	char fldName[DB_NAME_LEN];
	FIELD_KIND fldKind = (FIELD_KIND) 0;
	char fldQueryString[DB_QUERY_LEN];
	SQLLEN ni_fldQueryString = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, fldName, sizeof(fldName));
	sql->BindColumn(3, SQL_INTEGER, &fldKind, sizeof(fldKind));
	sql->BindColumn(4, SQL_CHAR, fldQueryString, sizeof(fldQueryString), &ni_fldQueryString);

	int res = sql->ExecuteSQL("select f.id, f.name, f.type, f.querystring "
			"from dm_field f where f.name = %s ", nameLit);
	if(IS_NOT_SQL_SUCCESS(res)) {
		SAFE_FREE(nameLit);
		return NULL;
	}

	SAFE_FREE(nameLit);

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		ret = new Field(*this, id, fldName, fldKind,
			NULL_IND(fldQueryString, NULL));
		m_fldCache.put(ret);
		return ret;
	}

	return NULL;
}


StringList *Model::getFieldValuesForField(Field &field)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char fldValue[129];
	sql->BindColumn(1, SQL_CHAR, fldValue, sizeof(fldValue));

	int res = sql->ExecuteSQL("select fv.fieldvalue "
			"from dm_fieldvalues fv where fv.fieldid = %d "
			"order by fv.positionid ", field.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	StringList *ret = new StringList();

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		ret->add(fldValue);
	}

	return ret;
}


StringList *Model::getFieldValuesForField(const char *querystring)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	// TODO: This may need to be dynamic
	char fldValue[129];
	sql->BindColumn(1, SQL_CHAR, fldValue, sizeof(fldValue));

	int res = sql->ExecuteSQL(querystring);
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	StringList *ret = new StringList();

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		ret->add(fldValue);
	}

	return ret;
}


void Model::getRepositoryForScript(Script &script)
{
	char whereClause[1024];
	sprintf(whereClause, "t.id = %d", script.repoid());

	AutoPtr<List<Repository> > matches = internalGetProviderObjects<Repository>(
		m_repCache, NULL, whereClause, false);
	if(matches && (matches->size() > 0)) {
		ListIterator<Repository> iter(*matches);
		script.setRepository(iter.first());
	}
}


GraphicalScriptGenerator *Model::createGraphicalScriptGenerator(Dropzone &dz, Action &action) {
	return new GraphicalScriptGenerator(m_odbc, dz, action);
}


Action *Model::getAction(const char *name)
{
	Action *ret = m_actCache.get(name);
	if(ret) {
		return ret;
	}

	char *nameLit = triSQL::EscapeLiteral(name);

	char whereClause[1024];
	sprintf(whereClause, "a.name = %s", nameLit);
	ret = internalGetAction(NULL, whereClause, true);
	SAFE_FREE(nameLit);
	return ret;
}

Action *Model::getActionTask(Task &task)
{
	/*
	int taskid = task.id();
	int actionid=0;
	Action *ret = (Action *)0;
	SQLLEN ni_actionid=0;

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	sql->BindColumn(1, SQL_INTEGER, &actionid, sizeof(actionid),&ni_actionid);
	int res = sql->ExecuteSQL("SELECT actionid FROM dm_taskaction WHERE id=%d",taskid);
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}
	res = sql->FetchRow();
	if (IS_SQL_SUCCESS(res)) {
		printf("got a row, actionid=%d\n",NULL_IND(actionid,0));
	}
	sql->CloseSQL();
	if (NULL_IND(actionid,0)>0) {
		ret = getAction(actionid);
	}
	*/

	char whereClause[1024];
	sprintf(whereClause, "a.id = b.actionid and b.id=%d", task.id());
	return internalGetAction("dm_taskaction b", whereClause, true);
}


Action *Model::getAction(int id)
{
	Action *ret = m_actCache.get(id);
	if(ret) {
		return ret;
	}

	char whereClause[1024];
	sprintf(whereClause, "a.id = %d", id);
	ret = internalGetAction(NULL, whereClause, true);
	return ret;
}


SwitchMode decodeSwitchMode(const char *mode)
{
	if(mode) {
		switch(mode[0]) {
		case 'S': return SWITCH_MODE_SWITCH;
		case 'P': return SWITCH_MODE_PREFIX;
		case 'A': return SWITCH_MODE_ALWAYS;
		case 'B': return SWITCH_MODE_BOOLEAN;
		}
	}
	return SWITCH_MODE_NONE;
}


void Model::getArgsForAction(Action &action)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char argName[DB_NAME_LEN];
	int argPos = 0;
	char argReqd[DB_BOOL_LEN];
	char argPad[DB_BOOL_LEN];
	char argMode[DB_BOOL_LEN];
	char argSwitch[DB_NAME_LEN];
	char argNegSwitch[DB_NAME_LEN];
	char argType[DB_NAME_LEN];
	SQLLEN ni_argPos = 0, ni_argReqd = 0, ni_argMode = 0,
		ni_argSwitch = 0, ni_argNegSwitch = 0, ni_argType = 0;

	sql->BindColumn(1, SQL_CHAR, argName, sizeof(argName));
	sql->BindColumn(2, SQL_INTEGER, &argPos, sizeof(argPos), &ni_argPos);
	sql->BindColumn(3, SQL_CHAR, argReqd, sizeof(argReqd), &ni_argReqd);
	sql->BindColumn(4, SQL_CHAR, argPad, sizeof(argPad));
	sql->BindColumn(5, SQL_CHAR, argMode, sizeof(argMode), &ni_argMode);
	sql->BindColumn(6, SQL_CHAR, argSwitch, sizeof(argSwitch), &ni_argSwitch);
	sql->BindColumn(7, SQL_CHAR, argNegSwitch, sizeof(argNegSwitch), &ni_argNegSwitch);
	sql->BindColumn(8, SQL_CHAR, argType, sizeof(argType), &ni_argType);

	int res = sql->ExecuteSQL(
			"select aa.name, aa.inpos, aa.required, aa.pad, "
			"  aa.switchmode, aa.switch, aa.negswitch, aa.type "
			"from dm_actionarg aa "
			"where aa.actionid = %d order by %s", 
			action.id(),
			action.isFunction()?"aa.inpos":"aa.outpos");
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		action.add(new ActionArg(argName, NULL_IND(argPos, -1), BOOL_NULL_IND(argReqd, false),
			((argPad[0] == 'Y') ? true : false), decodeSwitchMode(NULL_IND(argMode, NULL)),
			NULL_IND(argSwitch, NULL), NULL_IND(argNegSwitch, NULL),NULL_IND(argType,"")));
	}	
}


void Model::getScriptForAction(Action *action)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char filepath[DB_PATH_LEN];
	int textid = 0;
	int repoid = 0;
	SQLLEN ni_filepath = 0, ni_textid = 0, ni_repoid = 0;

	sql->BindColumn(1, SQL_CHAR, filepath, sizeof(filepath), &ni_filepath);
	sql->BindColumn(2, SQL_INTEGER, &textid, sizeof(textid), &ni_textid);
	sql->BindColumn(3, SQL_INTEGER, &repoid, sizeof(repoid), &ni_repoid);

	int res = sql->ExecuteSQL("select a.filepath, a.textid, a.repositoryid "
			"from dm_action a where a.id = %d and a.status = 'N' ", action->id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		switch(action->actionKind()) {
		case ACTION_KIND_SCRIPT: {
			Script *script = findOrCreateScript(repoid, NULL_IND(filepath, NULL));
			action->setScript(script);
			}
			break;
		case ACTION_KIND_IN_DB:
		case ACTION_KIND_GRAPHICAL: {
			Script *script = findOrCreateScript(action, NULL_IND(textid, 0));
			action->setScript(script);
			}
			break;
		}
	}
}


char *Model::getActionText(int textid)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id;
	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));	// we don't need this, but we need to call BindColumn

	int res = sql->ExecuteSQL(
		"select at.id, at.data from dm_actiontext at where at.id = %d ", textid);
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	char *ret = NULL;
	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		char *data = NULL;
		SQLLEN ni_data = 0;

		res = sql->GetData(2, SQL_C_BINARY, &data, 0, &ni_data);
		if(IS_SQL_SUCCESS(res)) {
			//debug1("Action text length: %d", ni_data);
			// Get all the data at once.
			data = (char*) malloc(ni_data + 2);
			res = sql->GetData(2, SQL_C_DEFAULT, data, ni_data + 1, &ni_data);
			if(IS_SQL_SUCCESS(res)) {
				ret = data;
				//debug1("Action text is: %s", data);
			}
		}
	}

	sql->CloseSQL();
	return ret;
}


void Model::getPluginForAction(Action &act)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	int version = 0;
	char library[256];
	SQLLEN ni_library = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_INTEGER, &version, sizeof(version));
	sql->BindColumn(3, SQL_CHAR, library, sizeof(library), &ni_library);

	int res = sql->ExecuteSQL(
		"select p.id, p.version, p.library from dm_action a, dm_plugin p "
		"where p.id = a.pluginid and a.id = %d ", act.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		Plugin *plugin = findOrCreatePlugin(id, version,
			NULL_IND(library, NULL));
		act.setPlugin(plugin);
	}
}


void Model::getServerTypeFilepathsForAction(Action &act)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int servertypeid = 0;
	char filepath[DB_PATH_LEN];

	sql->BindColumn(1, SQL_INTEGER, &servertypeid, sizeof(servertypeid));
	sql->BindColumn(2, SQL_CHAR, filepath, sizeof(filepath));

	int res = sql->ExecuteSQL(
		"select af.typeid, af.filepath from dm_actionfilepath af "
		"where af.actionid = %d ", act.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	StringHashtable *filepaths = new StringHashtable();

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		char key[32];
		sprintf(key, "%d", servertypeid);
		filepaths->put(key, filepath);
	}

	act.setServerTypeFilepaths(filepaths);
}


Task *Model::getTask(const char *name)
{
	// Have we already got this task
	Task *ret = m_tskCache.get(name);
	if(ret) { return ret; }

	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char *nameLit = triSQL::EscapeLiteral(name);

	int id;
	char taskName[DB_NAME_LEN];
	char taskKind[DB_NAME_LEN];

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, taskName, sizeof(taskName));
	sql->BindColumn(3, SQL_CHAR, taskKind, sizeof(taskKind));

	int res = sql->ExecuteSQL("select t.id, t.name, lower(tt.name) "
			"from dm_task t, dm_tasktypes tt where t.name = %s AND tt.id = t.typeid", nameLit);
	if(IS_NOT_SQL_SUCCESS(res)) {
		SAFE_FREE(nameLit);
		return NULL;
	}

	SAFE_FREE(nameLit);

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		ret = findOrCreateTask(id, taskName, taskKind);
		return ret;
	}

	return NULL;
}


Task *Model::getTask(int id)
{
	// Have we already got this task
	Task *ret = m_tskCache.get(id);
	if(ret) { return ret; }

	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char taskName[DB_NAME_LEN];
	char taskKind[DB_NAME_LEN];

	sql->BindColumn(1, SQL_CHAR, taskName, sizeof(taskName));
	sql->BindColumn(2, SQL_CHAR, taskKind, sizeof(taskKind));

	int res = sql->ExecuteSQL("select t.name, lower(tt.name) "
			"from dm_task t, dm_tasktypes tt where t.id = %d AND tt.id = t.typeid", id);
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		ret = findOrCreateTask(id, taskName, taskKind);
		return ret;
	}

	return NULL;
}

User *Model::waitForTask(Task &task,Application &app,int deploymentid)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	// Get the starting time for this deployment. When waiting for a task, ensure it's been
	// requested since this deployment started.
	//
	
	int completedby;
	int res;
	do {
		sql->BindColumn(1,SQL_INTEGER,&completedby,sizeof(completedby));
		res = sql->ExecuteSQL(
			"select completedby from dm.dm_request where taskid=%d and appid=%d and completedby is not null and \"WHEN\" >= "
			"(select started from dm.dm_deployment where deploymentid=%d)",
			task.id(),
			app.id(),
			deploymentid
		);
		if(IS_NOT_SQL_SUCCESS(res)) {
			sql->CloseSQL();
			return NULL;
		}
		res = sql->FetchRow();
#ifdef WIN32
		if (res == SQL_NO_DATA) Sleep(5000);	// wait 5 seconds
#else
		if (res == SQL_NO_DATA) sleep(5);
#endif
		sql->CloseSQL();
	} while (res == SQL_NO_DATA);
	User *user = getUserById(completedby);
	return user;
}


void Model::getActionsForTask(Task &task)
{
	char whereClause[1024];

	sprintf(whereClause, "t.id = %d and a.id = t.preactionid", task.id());
	Action *preAction = internalGetAction("dm_task t", whereClause, false);

	sprintf(whereClause, "t.id = %d and a.id = t.postactionid", task.id());
	Action *postAction = internalGetAction("dm_task t", whereClause, false);

	task.setActions(preAction, postAction);
}


Domain *Model::getApprovalDomainForApproveTask(Task &task)
{
	char whereClause[1024];
	sprintf(whereClause, "at.id = %d and d.id = at.approvaldomain", task.id());

	List<Domain> *matches = internalGetDomains("dm_taskapprove at", whereClause);
	if(matches) {
		ListIterator<Domain> iter(*matches);
		return iter.first();
	}

	return NULL;
}

List<Defect> *Model::getDefectsForApp(Application &app,bool onlyAppDefects /* = false */)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	char bugid[128];
	char status[40];
	char title[128];
	char apiurl[1024];
	char htmlurl[1024];
	int  compid;
	SQLLEN	ni_status,ni_apiurl,ni_htmlurl,ni_compid;
	sql->BindColumn(1, SQL_CHAR,	bugid,	sizeof(bugid));
	sql->BindColumn(2, SQL_CHAR,	title,	sizeof(title));
	sql->BindColumn(3, SQL_CHAR,	status,	sizeof(status), &ni_status);
	sql->BindColumn(4, SQL_CHAR,	apiurl,	sizeof(apiurl),	&ni_apiurl);
	sql->BindColumn(5, SQL_CHAR,	htmlurl,sizeof(htmlurl),&ni_htmlurl);
	sql->BindColumn(6, SQL_INTEGER, &compid,sizeof(compid), &ni_compid);
	if (app.isRelease()) {
		sql->ExecuteSQL(
			"select distinct b.bugid,b.title,b.status,b.apiurl,b.htmlurl,b.compid "   
			"from            dm.dm_application       a       "
			"left outer join dm.dm_applicationcomponent c on c.appid=a.id "
			"left outer join dm.dm_defects b on (b.compid=c.compid or b.appid=a.id) "
			"left outer join dm.dm_datasource x on x.id = b.datasourceid "
			"where           a.id in "
			"(SELECT objfrom FROM dm.dm_applicationcomponentflows where objfrom is not null and appid=%d "
			"UNION "
			"SELECT objto FROM dm.dm_applicationcomponentflows where objto is not null and appid=%d) "
			"AND b.bugid IS NOT NULL",
			app.id(),app.id());
	} else {
		if (onlyAppDefects) {
			sql->ExecuteSQL(
				"SELECT bugid,title,status,apiurl,htmlurl,compid FROM dm.dm_defects WHERE appid=%d",
				app.id());
		} else {
			sql->ExecuteSQL(
				"SELECT bugid,title,status,apiurl,htmlurl,null FROM dm.dm_defects WHERE compid IN "
				"(SELECT compid FROM dm.dm_applicationcomponent WHERE appid=%d)"
				"UNION "
				"SELECT bugid,title,status,apiurl,htmlurl,compid FROM dm.dm_defects WHERE appid=%d",
				app.id(),app.id());
		}
	}
	
	List<Defect> *ret = new List<Defect>();
	SQLRETURN res;

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		Defect *defect = new Defect(*this,bugid,title,(char *)(NULL_IND(status,"")),(char *)(NULL_IND(apiurl,"")),(char *)(NULL_IND(htmlurl,"")));
		ret->add(defect);
	}

	return ret;
}

List<Defect> *Model::getDefectsForComp(Component &comp)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	char bugid[128];
	char status[40];
	char title[128];
	char apiurl[1024];
	char htmlurl[1024];
	int  compid;
	SQLLEN	ni_status,ni_apiurl,ni_htmlurl,ni_compid;
	sql->BindColumn(1, SQL_CHAR,	bugid,	sizeof(bugid));
	sql->BindColumn(2, SQL_CHAR,	title,	sizeof(title));
	sql->BindColumn(3, SQL_CHAR,	status,	sizeof(status), &ni_status);
	sql->BindColumn(4, SQL_CHAR,	apiurl,	sizeof(apiurl),	&ni_apiurl);
	sql->BindColumn(5, SQL_CHAR,	htmlurl,sizeof(htmlurl),&ni_htmlurl);
	sql->BindColumn(6, SQL_INTEGER, &compid,sizeof(compid), &ni_compid);
	sql->ExecuteSQL(
		"SELECT bugid,title,status,apiurl,htmlurl,compid FROM dm.dm_defects WHERE compid=%d",
		comp.id(),comp.id());
	
	List<Defect> *ret = new List<Defect>();
	SQLRETURN res;

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		Defect *defect = new Defect(*this,bugid,title,(char *)(NULL_IND(status,"")),(char *)(NULL_IND(apiurl,"")),(char *)(NULL_IND(htmlurl,"")));
		ret->add(defect);
	}

	return ret;
}

List<Domain> *Model::getDomainsApprovedForApp(Application &app,bool approved /*=true*/)
{
	char whereClause[1024];
	sprintf(whereClause, "a.appid = %d and d.id = a.domainid and a.approved='%c' "
		"and not exists (select 'Y' from dm_approval b where b.approved != a.approved and b.appid = a.appid and b.\"WHEN\" > a.\"WHEN\")",
		app.id(),approved?'Y':'N');

	List<Domain> *ret = internalGetDomains("dm_approval a", whereClause);

	return ret;
}

void Model::internalGetChildrenOfApp(List<Application> *applist,Application &app,bool AllDescendents)
{
	debug1("internalGetChildrenOfApp(%d)",app.id());
	char whereClause[128];
	sprintf(whereClause, "(a.predecessorid = %d or a.parentid = %d)",app.id(),app.id());

	List<Application> *ret = internalGetApplications(NULL, whereClause, true);
	ListIterator<Application> iter(*ret);
	for(Application *a = iter.first(); a; a = iter.next()) {
		applist->add(a);
		if (AllDescendents) {
			debug1("iterating for app.id = %d",a->id());
			internalGetChildrenOfApp(applist,*a,true);
		}
	}
}

List <Application> *Model::getChildrenOfApplication(Application &app, bool AllDescendents /*=false*/)
{
	List <Application> *ret = new List<Application>(false);
	internalGetChildrenOfApp(ret,app,AllDescendents);
	return ret;
}

char *Model::getBranchName(int appid)
{
	// Returns the branch name for the specified application. If the app has no branch the code
	// checks the parent and so on up the tree until it finds a branch id.
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLLEN ni_branchName;
	SQLLEN ni_predecessorid;
	char branchName[129];
	int predecessorid;
	sql->BindColumn(1, SQL_CHAR,    branchName,	    sizeof(branchName),    &ni_branchName);
	sql->BindColumn(2, SQL_INTEGER, &predecessorid, sizeof(predecessorid), &ni_predecessorid);
	sql->ExecuteSQL("select branch,predecessorid from dm_application where id=%d",appid);
	int res = sql->FetchRow();
	char *retval = (char *)0;
	if (IS_SQL_SUCCESS(res)) {
		retval = NULL_IND(branchName,NULL);
		predecessorid = NULL_IND(predecessorid,0);
	}
	sql->CloseSQL();
	return retval?strdup(retval):predecessorid?getBranchName(predecessorid):NULL;
}


Domain *Model::getTargetDomainForCopyMoveTask(Task &task)
{
	char fromClause[256], whereClause[1024];
	sprintf(fromClause, "dm_task%s cmt", task.taskType());
	sprintf(whereClause, "cmt.id = %d and d.id = cmt.targetdomain", task.id());

	List<Domain> *matches = internalGetDomains(fromClause, whereClause);
	if(matches) {
		ListIterator<Domain> iter(*matches);
		return iter.first();
	}

	return NULL;
}


Task *Model::getLinkedTaskForRequestTask(Task &task)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int linkedTaskId;
	char linkedTaskName[DB_NAME_LEN];
	char linkedTaskKind[DB_NAME_LEN];

	sql->BindColumn(1, SQL_INTEGER, &linkedTaskId, sizeof(linkedTaskId));
	sql->BindColumn(2, SQL_CHAR, linkedTaskName, sizeof(linkedTaskName));
	sql->BindColumn(3, SQL_CHAR, linkedTaskKind, sizeof(linkedTaskKind));

	int res = sql->ExecuteSQL("select lt.id, lt.name, lower(tt.name) "
			"from dm_taskrequest rt, dm_task lt, dm_tasktypes tt "
			"where rt.id = %d AND lt.id = rt.linkedtaskid AND tt.id = lt.typeid", task.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		return findOrCreateTask(linkedTaskId, linkedTaskName, linkedTaskKind);
	}

	return NULL;
}


bool Model::approveApplication(
	Application &app, Domain &tgtDomain, bool approve, const char *note)
{
	debug1("%s application %d for domain %d",
		(approve ? "Approving" : "Rejecting"), app.id(), tgtDomain.id());

	if(!m_currentUser) {
		debug1("Current user not set");
		return false;
	}

	long id = getNextObjectId("approval");
	if(id == 0) {
		return false;
	}

	debug1("p1");

	long appid = app.id();
	long domid = tgtDomain.id();
	long when = time(NULL);
	long userid = m_currentUser->id();
	const char *approved = approve ? "Y" : "N";
	int notelen = note ? strlen(note) : 0;

	debug1("p2");

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLRETURN res = sql->PrepareStatement(
		"INSERT INTO dm_approval("
		"  id, appid, domainid, \"WHEN\", userid, approved, note) VALUES (?,?,?,?,?,?,?)");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to insert approval entry");
		return false;
	}

	debug1("p3");

	sql->BindParameter(1, SQL_INTEGER, sizeof(id), &id, sizeof(id));
	sql->BindParameter(2, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
	sql->BindParameter(3, SQL_INTEGER, sizeof(domid), &domid, sizeof(domid));
	sql->BindParameter(4, SQL_INTEGER, sizeof(when), &when, sizeof(when));
	sql->BindParameter(5, SQL_INTEGER, sizeof(userid), &userid, sizeof(userid));
	sql->BindParameter(6, SQL_CHAR, 1, (char*) approved, 1);
	sql->BindParameter(7, SQL_CHAR, notelen, (char*) note, notelen);
	res = sql->Execute();
	
	debug1("p4");

	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to insert approval entry (2)");
		sql->CloseSQL();
		return false;
	}
	sql->CloseSQL();

	debug1("p5");

	return true;
}

long Model::nextAutoDeployTime(Application &app,char **penv)
{
	// Returns the date/time (Epoch) of the next auto deployment specified for the
	// specified application in its current domain or 0 if none is pending.
	//
	char envname[2049];
	long timeNow = (long)time(NULL);
	long starttime=0;
	int appid = app.id();
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLRETURN res = sql->PrepareStatement("SELECT a.starttime,b.name FROM dm_calendar a, dm_environment b WHERE a.eventtype='Auto' AND a.appid=? AND a.starttime>=? AND a.status='N' AND a.envid=b.id");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to get nextAutoDeployTime (1)");
		return 0;
	}
	sql->BindParameter(1, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
	sql->BindParameter(2, SQL_INTEGER, sizeof(timeNow), &timeNow, sizeof(timeNow));
	sql->BindColumn(1, SQL_INTEGER, &starttime, sizeof(starttime));
	sql->BindColumn(2, SQL_CHAR, envname, sizeof(envname));
	res = sql->Execute();
	if (IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to get nextAutoDeployTime (2)");
	}
	res = sql->FetchRow();
	sql->CloseSQL();
	if (res == SQL_NO_DATA) {
		*penv = (char *)0;
	} else {
		*penv = strdup(envname);
	}
	return starttime;
}



bool Model::isApproved(Application &app,Domain &domain)
{
	// Returns true if application is approved for the specified domain, false otherwise
	// This requires an approval process to be present that points to the target domain.
	//
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	int appid = app.id();
	int domid = domain.id();
	int c=0;

	debug1("isApproved - application = %s (%d), target domain = %s (%d)",app.name(),app.id(),domain.name(),domain.id());

	SQLRETURN res = sql->PrepareStatement("SELECT count(*) FROM dm_taskapprove WHERE approvaldomain=?");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to get approval status (1)");
		return false;
	}
	sql->BindColumn(1, SQL_INTEGER, &c, sizeof(c));
	sql->BindParameter(1, SQL_INTEGER, sizeof(domid), &domid, sizeof(domid));
	res = sql->Execute();
	if (IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to get approval status (2)");
	}
	sql->FetchRow();
	sql->CloseSQL();

	debug1("c=%d",c);

	if (c>0) {
		//
		// There is a least one approval process pointing at this domain
		//
		AutoPtr<triSQL> sql2 = m_odbc.GetSQL();
		res = sql2->PrepareStatement("SELECT approved FROM dm_approval WHERE appid=? AND domainid=? ORDER BY id DESC");
		if(IS_NOT_SQL_SUCCESS(res)) {
			debug1("Failed to get approval status (3)");
			return false;
		}
		char approved[2];
		sql2->BindColumn(1,SQL_C_CHAR,approved,sizeof(approved));
		sql2->BindParameter(1, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
		sql2->BindParameter(2, SQL_INTEGER, sizeof(domid), &domid, sizeof(domid));
		res = sql2->Execute();
		if (IS_NOT_SQL_SUCCESS(res)) {
			debug1("Failed to get approval status (4)");
		}
		res = sql2->FetchRow();
		sql2->CloseSQL();
		return (res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO)?(approved[0]=='Y' || approved[0]=='y'):false;
	} else {
		//
		// There is no approval process pointing at this domain - just return true.
		//
		return true;
	}
}

bool Model::request(
	Application &app,Task &linkedTask, const char *note)
{
	debug1("Requesting application %d linked task is %d", app.id(), linkedTask.id());

	long taskid = linkedTask.id();
	long now = time(NULL);
	long userid = m_currentUser->id();
	long appid = app.id();

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLRETURN res = sql->PrepareStatement("INSERT INTO dm_request(id,taskid,userid,\"WHEN\",note,appid,status) VALUES(?,?,?,?,?,?,'N')");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to insert into request table");
		return false;
	}

	int notelen = note ? strlen(note) : 0;
	long id = getNextObjectId("request");

	sql->BindParameter(1, SQL_INTEGER, sizeof(id), &id, sizeof(id));
	sql->BindParameter(2, SQL_INTEGER, sizeof(taskid), &taskid, sizeof(taskid));
	sql->BindParameter(3, SQL_INTEGER, sizeof(userid), &userid, sizeof(userid));
	sql->BindParameter(4, SQL_INTEGER, sizeof(now), &now, sizeof(now));
	sql->BindParameter(5, SQL_CHAR,    notelen, (char*) note, notelen);
	sql->BindParameter(6, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));

	res = sql->Execute();
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to insert into request table (2)");
		sql->CloseSQL();
		return false;
	}
	sql->CloseSQL();


	return true;
}



bool Model::moveApplication(
	Application &app, Domain &tgtDomain, const char *note)
{
	debug1("Moving application %d to domain %d", app.id(), tgtDomain.id());

	if(!m_currentUser) {
		debug1("Current user not set");
		return false;
	}

	if (isApproved(app,tgtDomain)) {
		char *adEnv;
		time_t autotime = (time_t)nextAutoDeployTime(app,&adEnv);
		if (autotime == 0) {
			long modified = time(NULL);
			long modifierid = m_currentUser->id();
			long tgtdomid = tgtDomain.id();
			long appid = app.id();
			long srcdomid = 0;
			int srcpos=0;
			int tgtpos=0;
			Domain *appdom = app.getDomain();
			if (appdom) {
				srcdomid = appdom->id();
				srcpos = appdom->getPosition();
			}
			tgtpos=tgtDomain.getPosition();

			AutoPtr<triSQL> sql = m_odbc.GetSQL();
			SQLRETURN res = sql->PrepareStatement(
				"UPDATE dm_application "
				" SET modified = ?, modifierid = ?, domainid = ? "
				" WHERE id = ?");
			if(IS_NOT_SQL_SUCCESS(res)) {
				debug1("Failed to update application entry");
				return false;
			}
			sql->BindParameter(1, SQL_INTEGER, sizeof(modified), &modified, sizeof(modified));
			sql->BindParameter(2, SQL_INTEGER, sizeof(modifierid), &modifierid, sizeof(modifierid));
			sql->BindParameter(3, SQL_INTEGER, sizeof(tgtdomid), &tgtdomid, sizeof(tgtdomid));
			sql->BindParameter(4, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));

			res = sql->Execute();
			if(IS_NOT_SQL_SUCCESS(res)) {
				debug1("Failed to update application entry (2)");
				sql->CloseSQL();
				return false;
			}
			sql->CloseSQL();

			sql = m_odbc.GetSQL();

			// Record an entry to say that we moved the application
			long id = getNextObjectId("historynote");
			if(id == 0) {
				return false;
			}

			//
			// Record the target domain
			//
			int notelen = note ? strlen(note) : 0;
			int domlen = strlen(tgtDomain.name());
			char *finalnote = (char *)malloc(domlen+128+notelen);
			sprintf(finalnote,"Moved to Domain \"%s\"",tgtDomain.name());
			if (notelen > 0) {
				// Add user specified note text
				strcat(finalnote,": ");
				strcat(finalnote,note);
			}
			notelen = strlen(finalnote);
			long kind = OBJ_KIND_APPLICATION;
			const char *icon = (srcpos>=tgtpos)?"movedown":"movever";

			res = sql->PrepareStatement(
				"INSERT INTO dm_historynote(id, objid, kind, \"WHEN\", note, userid, icon) VALUES(?, ?, ?, ?, ?, ?, ?)");
			if(IS_NOT_SQL_SUCCESS(res)) {
				debug1("Failed to insert history note entry");
				return false;
			}
			sql->BindParameter(1, SQL_INTEGER, sizeof(id), &id, sizeof(id));
			sql->BindParameter(2, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
			sql->BindParameter(3, SQL_INTEGER, sizeof(kind), &kind, sizeof(kind));
			sql->BindParameter(4, SQL_INTEGER, sizeof(modified), &modified, sizeof(modified));
			sql->BindParameter(5, SQL_CHAR, notelen, (char*) finalnote, notelen);
			sql->BindParameter(6, SQL_INTEGER, sizeof(modifierid), &modifierid, sizeof(modifierid));
			sql->BindParameter(7, SQL_CHAR, strlen(icon), (char*) icon, strlen(icon));
			res = sql->Execute();
			if(IS_NOT_SQL_SUCCESS(res)) {
				debug1("Failed to insert history note entry (2)");
				sql->CloseSQL();
				return false;
			}
			sql->CloseSQL();
			//
			// Now add the record to the dm_historymove table
			//
			// dm_historymove(
			// id integer,
			// appid integer,
			// "WHEN" integer,
			// userid integer,
			// domainfrom integer,
			// domainto integer
			// )
			//
			res = sql->PrepareStatement(
				"INSERT INTO dm_historymove(id, appid, \"WHEN\", userid, domainfrom, domainto) VALUES(?,?,?,?,?,?)");
			if(IS_NOT_SQL_SUCCESS(res)) {
				debug1("Failed to insert into dm_historymove");
				return false;
			}
			sql->BindParameter(1, SQL_INTEGER, sizeof(id), &id, sizeof(id));
			sql->BindParameter(2, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
			sql->BindParameter(3, SQL_INTEGER, sizeof(modified), &modified, sizeof(modified));
			sql->BindParameter(4, SQL_INTEGER, sizeof(modifierid), &modifierid, sizeof(modifierid));
			sql->BindParameter(5, SQL_INTEGER, sizeof(srcdomid), &srcdomid, sizeof(srcdomid));
			sql->BindParameter(6, SQL_INTEGER, sizeof(tgtdomid), &tgtdomid, sizeof(tgtdomid));
			res = sql->Execute();
			if(IS_NOT_SQL_SUCCESS(res)) {
				debug1("Failed to insert into dm_historymove (2)");
				sql->CloseSQL();
				return false;
			}
			sql->CloseSQL();


			return true;
		} else {
			struct tm *dt = localtime(&autotime);
			printf("Application \"%s\" is due to be auto deployed to Environment \"%s\" on %02d/%02d/%04d at %02d:%02d\n",
				app.name(),
				adEnv,
				dt->tm_mday,
				dt->tm_mon,
				dt->tm_year+1900,
				dt->tm_hour,
				dt->tm_min
				);
			SAFE_FREE(adEnv);
			return false;
		}
	} else {
		printf("Application \"%s\" is not approved for domain \"%s\"\n",app.name(),tgtDomain.name());
		return false;
	}
}


#define BOX_WIDTH  120
#define BOX_HEIGHT 50


ApplicationVersion* Model::newVersionOfApplication(
	Application &app, Domain &tgtDomain, Application *predecessor)
{
	debug1("Creating new version of application %d in domain %d", app.id(), tgtDomain.id());

	if(!m_currentUser) {
		debug1("Current user not set");
		return false;
	}

	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	long appid = app.id();
	long predid = predecessor ? predecessor->id() : appid;

	// Count of children
	long c = 0;
	SQLRETURN res = sql->PrepareStatement(
		"SELECT count(*) FROM dm_application a WHERE a.parentid=? AND status='N'");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to get count of application versions");
		return NULL;
	}
	sql->BindParameter(1, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
	sql->BindColumn(1, SQL_INTEGER, &c, sizeof(c));

	res = sql->Execute();
	if(IS_SQL_SUCCESS(res)) {
		res = sql->FetchRow();
	}
	if(IS_NOT_SQL_SUCCESS(res)) {
		c = 0;
	}
	sql->CloseSQL();

	int xpos = 0, ypos = 0;

	// Coords of predecessor
	SQLLEN ni_xpos = 0, ni_ypos = 0;
	res = sql->PrepareStatement(
		"SELECT a.xpos, a.ypos FROM dm_application a WHERE a.id = ?");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to get coords of application version");
		return NULL;
	}
	sql->BindParameter(1, SQL_INTEGER, sizeof(predid), &predid, sizeof(predid));
	sql->BindColumn(1, SQL_INTEGER, &xpos, sizeof(xpos), &ni_xpos);
	sql->BindColumn(2, SQL_INTEGER, &ypos, sizeof(ypos), &ni_ypos);

	

	res = sql->Execute();
	if(IS_SQL_SUCCESS(res)) {
		res = sql->FetchRow();
	}
	if(IS_NOT_SQL_SUCCESS(res)) {
		xpos = 0;
		ypos = 0;
	}
	sql->CloseSQL();

	debug1("xpos = %d ypos = %d",xpos,ypos);

	// Max xpos of predecessors children
	int maxxpos = 0;
	SQLLEN ni_maxxpos = 0;
	res = sql->PrepareStatement(
		"SELECT max(a.xpos) FROM dm_application a WHERE a.predecessorid=? AND a.status='N'");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to get max xpos of application versions");
		return NULL;
	}
	sql->BindParameter(1, SQL_INTEGER, sizeof(predid), &predid, sizeof(predid));
	sql->BindColumn(1, SQL_INTEGER, &maxxpos, sizeof(maxxpos), &ni_maxxpos);

	res = sql->Execute();
	if(IS_SQL_SUCCESS(res)) {
		res = sql->FetchRow();
	}
	if(IS_NOT_SQL_SUCCESS(res)) {
		maxxpos = -1;
	}
	sql->CloseSQL();

	if(maxxpos > 0) {
		xpos = maxxpos + BOX_WIDTH*1.5;
	}


	ypos += BOX_HEIGHT*2;

	char newName[DB_NAME_LEN];
	sprintf(newName, "%s;%d", app.name(), c+1);

	long newid = getNextObjectId("application");
	long domid = tgtDomain.id();
	long now = time(NULL);
	long userid = m_currentUser->id();

	res = sql->PrepareStatement(
		"INSERT INTO dm_application("
		"  id,name,domainid,ownerid,creatorid,modifierid,created,modified,"
		"  parentid,predecessorid,xpos,ypos,status,successtemplateid,failuretemplateid,isrelease,"
		"  actionid,preactionid,postactionid,datasourceid) "
		"SELECT ?,?,?,?,?,?,?,?,?,?,?,?,'N',successtemplateid,failuretemplateid,isrelease,	"
		"actionid,preactionid,postactionid,datasourceid "
		"FROM dm_application "
		"WHERE id=?");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to insert application version");
		return NULL;
	}
	sql->BindParameter( 1, SQL_INTEGER, sizeof(newid), &newid, sizeof(newid));
	sql->BindParameter( 2, SQL_CHAR,    strlen(newName), newName, strlen(newName));
	sql->BindParameter( 3, SQL_INTEGER, sizeof(domid), &domid, sizeof(domid));
	sql->BindParameter( 4, SQL_INTEGER, sizeof(userid), &userid, sizeof(userid));
	sql->BindParameter( 5, SQL_INTEGER, sizeof(userid), &userid, sizeof(userid));
	sql->BindParameter( 6, SQL_INTEGER, sizeof(userid), &userid, sizeof(userid));
	sql->BindParameter( 7, SQL_INTEGER, sizeof(now), &now, sizeof(now));
	sql->BindParameter( 8, SQL_INTEGER, sizeof(now), &now, sizeof(now));
	sql->BindParameter( 9, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
	sql->BindParameter(10, SQL_INTEGER, sizeof(predid), &predid, sizeof(predid));
	sql->BindParameter(11, SQL_INTEGER, sizeof(xpos), &xpos, sizeof(xpos));
	sql->BindParameter(12, SQL_INTEGER, sizeof(ypos), &ypos, sizeof(ypos));
	sql->BindParameter(13, SQL_INTEGER, sizeof(predid), &predid, sizeof(predid));
	res = sql->Execute();
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to insert application version (2)");
		return NULL;
	}
	sql->CloseSQL();

	//
	// Add the components from the predecessor OR the applications if this is a release
	//
	res = sql->PrepareStatement(
		"INSERT INTO dm_applicationcomponent(appid,compid,childappid,xpos,ypos) "
		"SELECT ?,compid,childappid,xpos,ypos FROM dm_applicationcomponent WHERE appid=?");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to copy components to application version");
	} else {
		sql->BindParameter(1, SQL_INTEGER, sizeof(newid), &newid, sizeof(newid));
		sql->BindParameter(2, SQL_INTEGER, sizeof(predid), &predid, sizeof(predid));
		res = sql->Execute();
		if(IS_NOT_SQL_SUCCESS(res)) {
			debug1("Failed to copy components to application version (2)");
		}
	}
	sql->CloseSQL();

	// Add the component/application flows from the predecessor
	res = sql->PrepareStatement(
		"INSERT INTO dm_applicationcomponentflows(appid,objfrom,objto) "
		"SELECT ?,objfrom,objto FROM dm_applicationcomponentflows WHERE appid=?");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to copy component flows to application version");
	} else {
		sql->BindParameter(1, SQL_INTEGER, sizeof(newid), &newid, sizeof(newid));
		sql->BindParameter(2, SQL_INTEGER, sizeof(predid), &predid, sizeof(predid));
		res = sql->Execute();
		if(IS_NOT_SQL_SUCCESS(res)) {
			debug1("Failed to copy component flows to application version (2)");
		}
	}
	sql->CloseSQL();
	// Now copy all the attributes from the predecessor application
	res = sql->PrepareStatement(
		"INSERT INTO dm_applicationvars(appid,name,value,arrayid,nocase) "
		"SELECT ?,name,value,arrayid,nocase FROM dm_applicationvars WHERE appid=?");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to copy application vars to application version");
	} else {
		sql->BindParameter(1, SQL_INTEGER, sizeof(newid), &newid, sizeof(newid));
		sql->BindParameter(2, SQL_INTEGER, sizeof(predid), &predid, sizeof(predid));
		res = sql->Execute();
		if(IS_NOT_SQL_SUCCESS(res)) {
			debug1("Failed to copy application vars to application version (2)");
		}
	}
	// Now copy any defects associated with the application
	res = sql->PrepareStatement(
		"INSERT INTO dm_defects(appid,bugid,title,status,apiurl,htmlurl) "
		"SELECT ?,bugid,title,status,apiurl,htmlurl FROM dm_defects WHERE appid=?");
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug1("Failed to copy application defects");
	} else {
		sql->BindParameter(1, SQL_INTEGER, sizeof(newid), &newid, sizeof(newid));
		sql->BindParameter(2, SQL_INTEGER, sizeof(predid), &predid, sizeof(predid));
		res = sql->Execute();
		if(IS_NOT_SQL_SUCCESS(res)) {
			debug1("Failed to copy application defects (2)");
		}
	}


	//
	// DO NOT CHANGE THE FORMAT OF THIS MESSAGE - it is used by the API to find the newly created
	// application ID.
	//
	printf("Created new Application Version %s (id %d)\n", newName,newid);
	debug1("New Application Version is %d '%s'", newid, newName);
	return findOrCreateApplicationVersion(newid, newName, newName, (Application*) app.toObject());
}


ApplicationVersion* Model::getLatestVersionOfApplication(Application &app, char *branch /* = (char *)0 */)
{
	char whereClause[256];
	
	if (branch) {
		// Find the application with the branch id
		int id;
		SQLLEN ni_id;
		AutoPtr<triSQL> sql = m_odbc.GetSQL();
		int appid = app.id();
		SQLRETURN res = sql->PrepareStatement("select max(id) from dm_application where parentid=? and branch=? and status='N'");
		if(IS_NOT_SQL_SUCCESS(res)) {
			debug1("Failed to prepare get of branch");
			return NULL;
		}
		sql->BindParameter(1, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
		sql->BindParameter(2, SQL_CHAR,    strlen(branch),branch, strlen(branch));
		sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id), &ni_id);
		res = sql->Execute();
		if(IS_NOT_SQL_SUCCESS(res)) {
			debug1("Failed to ExecuteSQL get of branch");
			return NULL;
		}
		res = sql->FetchRow();
		sql->CloseSQL();
		id = NULL_IND(id,0);
		while (id > 0) {
			// Descend this branch until there's no more versions
			int cid;
			SQLLEN ni_cid;
			sprintf(whereClause,"a.id = %d",id);
			res = sql->PrepareStatement("SELECT max(id) FROM dm_application WHERE predecessorid=? and status='N'");
			sql->BindParameter(1, SQL_INTEGER, sizeof(id), &id, sizeof(id));
			sql->BindColumn(1, SQL_INTEGER, &cid, sizeof(cid), &ni_cid);
			sql->Execute();
			res = sql->FetchRow();
			sql->CloseSQL();
			id = NULL_IND(cid,0);
		}
	} else {	
		sprintf(whereClause,
			"a.id = (select max(a2.id) from dm_application a2 "
			"where a2.parentid = %d and a2.status='N')", app.id());
	}
	AutoPtr<List<Application> > matches = internalGetApplications(NULL, whereClause, true);

	if(matches && (matches->size() > 0)) {
		ListIterator<Application> iter(*matches);
		Application *ret = iter.first();
		return ret ? ret->toApplicationVersion() : NULL;
	}

	return NULL;
}



void Model::recordAppInEnv(class DM &dm, Application &app, Environment &env, bool success)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	long appid = app.id();
	long deployid = dm.deployId();
	long envid = env.id();

	if (success) {
		// Application was successfully deployed
		SQLRETURN res = sql->PrepareStatement(
			"UPDATE dm_appsinenv SET appid = ?, deploymentid = ? "
			"WHERE envid = ? AND appid IN (SELECT a.id FROM dm_application a, dm_application b "
			"WHERE (a.parentid = b.parentid OR a.id = b.parentid OR a.parentid = b.id OR a.id = b.id) AND b.id = ?)");
		if(IS_NOT_SQL_SUCCESS(res)) {
			throw RuntimeError("Failed to update app in env");
		}
		
		sql->BindParameter(1, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
		sql->BindParameter(2, SQL_INTEGER, sizeof(deployid), &deployid, sizeof(deployid));
		sql->BindParameter(3, SQL_INTEGER, sizeof(envid), &envid, sizeof(envid));
		sql->BindParameter(4, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
		debug1("Updating appsinenv (%d, %d, %d)", appid, envid, deployid);
		res = sql->ExecuteIgnoringErrors();
		debug2("res = %d", res);
		sql->CloseSQL();
		if(IS_NOT_SQL_SUCCESS(res)) {
			if((res != SQL_ERROR) && (res != SQL_NO_DATA)) {
				throw RuntimeError("Failed to update app in env (2), %d", res);
			}
			res = sql->PrepareStatement(
				"INSERT INTO dm_appsinenv(envid, appid, deploymentid) VALUES(?,?,?)");
			if(IS_NOT_SQL_SUCCESS(res)) {
				throw RuntimeError("Failed to insert app in env");
			}
			sql->BindParameter(1, SQL_INTEGER, sizeof(envid), &envid, sizeof(envid));
			sql->BindParameter(2, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
			sql->BindParameter(3, SQL_INTEGER, sizeof(deployid), &deployid, sizeof(deployid));
			debug1("Inserting into appsinenv (%d, %d, %d)", appid, envid, deployid);
			res = sql->Execute();
			debug2("res = %d", res);
			sql->CloseSQL();
			if(IS_NOT_SQL_SUCCESS(res)) {
				throw RuntimeError("Failed to insert app in env (2)");
			}
		}
	} else {
		// Application was not successfully deployed. Remove any entry from appsonserv for this
		// application
		SQLRETURN res = sql->PrepareStatement(
			"DELETE FROM dm_appsinenv WHERE envid = ? AND appid IN "
			"(select d.id from dm_application c, dm_application d where c.id = ? and c.parentid=d.parentid	"
			"union	"
			"select a.id from dm_application a where a.parentid=?	"
			"union	"
			"select coalesce(b.parentid,b.id) from dm_application b where b.id=?)");

		if(IS_NOT_SQL_SUCCESS(res)) {
			throw RuntimeError("Failed to update apps in env");
		}
		sql->BindParameter(1, SQL_INTEGER, sizeof(envid), &envid, sizeof(envid));
		sql->BindParameter(2, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
		sql->BindParameter(3, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
		sql->BindParameter(4, SQL_INTEGER, sizeof(appid), &appid, sizeof(appid));
		debug1("Removing application from environment (%d, %d)", appid, envid);
		res = sql->ExecuteIgnoringErrors();
		debug2("res = %d", res);
		sql->CloseSQL();
	}
}


void Model::recordCompOnServ(class DM &dm, Component &comp, Server &server, bool success)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	if (success) {
		// Component was successfully deployed
		long compid = comp.id();
		long buildid = comp.buildid();
		long deployid = dm.deployId();
		long serverid = server.id();
		SQLRETURN res;
		int n=3;
		if (buildid) {
			res = sql->PrepareStatement(
			"UPDATE dm_compsonserv SET compid = ?, deploymentid = ?, buildnumber = ? "
			"WHERE serverid = ? AND compid IN (SELECT a.id FROM dm_component a, dm_component b "
			"WHERE (a.parentid = b.parentid OR a.id = b.parentid OR a.parentid = b.id OR a.id = b.id) AND b.id = ?)");
		} else {
			res = sql->PrepareStatement(
			"UPDATE dm_compsonserv SET compid = ?, deploymentid = ? "
			"WHERE serverid = ? AND compid IN (SELECT a.id FROM dm_component a, dm_component b "
			"WHERE (a.parentid = b.parentid OR a.id = b.parentid OR a.parentid = b.id OR a.id = b.id) AND b.id = ?)");
		}
		if(IS_NOT_SQL_SUCCESS(res)) {
			throw RuntimeError("Failed to update comp on serv");
		}
		sql->BindParameter(1, SQL_INTEGER, sizeof(compid), &compid, sizeof(compid));
		sql->BindParameter(2, SQL_INTEGER, sizeof(deployid), &deployid, sizeof(deployid));
		if (buildid) {
			sql->BindParameter(n++, SQL_INTEGER, sizeof(buildid), &buildid, sizeof(buildid));
		}
		
		sql->BindParameter(n++, SQL_INTEGER, sizeof(serverid), &serverid, sizeof(serverid));
		sql->BindParameter(n++, SQL_INTEGER, sizeof(compid), &compid, sizeof(compid));
		debug1("Updating compsonserv (%d, %d, %d)", compid, serverid, deployid);
		res = sql->ExecuteIgnoringErrors();
		debug2("res = %d", res);
		sql->CloseSQL();
		if(IS_NOT_SQL_SUCCESS(res)) {
			if((res != SQL_ERROR) && (res != SQL_NO_DATA)) {
				throw RuntimeError("Failed to update comp on serv (2), %d", res);
			}
			if (buildid) {
				res = sql->PrepareStatement(
					"INSERT INTO dm_compsonserv(serverid, compid, deploymentid, buildnumber) VALUES(?,?,?,?)");
			} else {
				res = sql->PrepareStatement(
					"INSERT INTO dm_compsonserv(serverid, compid, deploymentid) VALUES(?,?,?)");
			}
			if(IS_NOT_SQL_SUCCESS(res)) {
				throw RuntimeError("Failed to insert comp on serv");
			}
			sql->BindParameter(1, SQL_INTEGER, sizeof(serverid), &serverid, sizeof(serverid));
			sql->BindParameter(2, SQL_INTEGER, sizeof(compid), &compid, sizeof(compid));
			sql->BindParameter(3, SQL_INTEGER, sizeof(deployid), &deployid, sizeof(deployid));
			if (buildid) {
				sql->BindParameter(4, SQL_INTEGER, sizeof(buildid), &buildid, sizeof(buildid));
			}
			debug1("Inserting into compsonserv (%d, %d, %d, %d)", compid, serverid, deployid, buildid);
			res = sql->ExecuteIgnoringErrors();
			debug2("res = %d", res);
			sql->CloseSQL();
//			if(IS_NOT_SQL_SUCCESS(res)) {
//				char msg[1024];
//				sprintf(msg,"Failed to insert comp on serv (2): Inserting into compsonserv (%d, %d, %d, %d,%d)", compid, serverid, deployid, buildid,res);
//				throw RuntimeError(msg);
//			}
		}
	} else {
		// Component was not successfully deployed. Remove any entry from compsonserv for this
		// component
		SQLRETURN res = sql->PrepareStatement(
		"DELETE FROM dm_compsonserv WHERE serverid = ? AND compid IN "
			"(select d.id from dm_component c, dm_component d where c.id = ? and c.parentid=d.parentid	"
			"union	"
			"select a.id from dm_component a where a.parentid=?	"
			"union	"
			"select coalesce(b.parentid,b.id) from dm_component b where b.id=?)");
			

		if(IS_NOT_SQL_SUCCESS(res)) {
			throw RuntimeError("Failed to update comp on serv (3)");
		}
		long compid = comp.id();
		long serverid = server.id();
		sql->BindParameter(1, SQL_INTEGER, sizeof(serverid), &serverid, sizeof(serverid));
		sql->BindParameter(2, SQL_INTEGER, sizeof(compid), &compid, sizeof(compid));
		sql->BindParameter(3, SQL_INTEGER, sizeof(compid), &compid, sizeof(compid));
		sql->BindParameter(4, SQL_INTEGER, sizeof(compid), &compid, sizeof(compid));
		debug1("Removing component from server (%d, %d)", compid, serverid);
		res = sql->ExecuteIgnoringErrors();
		debug2("res = %d", res);
		sql->CloseSQL();
	}
}

StringList *Model::getFilesForLastDeployment(Component &comp,Server &s,int deployid)
{
	StringList *ret = new StringList();
	//
	// Get a list of files that were deployed for this component on this server
	//
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	SQLRETURN res = sql->PrepareStatement(
	"select	a.deploymentid,a.targetfilename	"
	"from	dm.dm_deploymentxfer a	"
	"where	a.componentid = ?	"
	"and	a.serverid = ?	"
	"and	a.deploymentid < ? "
	"order by a.deploymentid desc"
	);
	if(IS_NOT_SQL_SUCCESS(res)) {
		throw RuntimeError("Failed to getFilesForLastDeployment");
	}
	long compid = comp.id();
	long serverid = s.id();
	long deploymentId;
	long lastDeploymentId=-1;
	char pathname[2049];
	sql->BindParameter(1, SQL_INTEGER, sizeof(compid), &compid, sizeof(compid));
	sql->BindParameter(2, SQL_INTEGER, sizeof(serverid), &serverid, sizeof(serverid));
	sql->BindParameter(3, SQL_INTEGER, sizeof(deployid), &deployid, sizeof(deployid));
	sql->BindColumn(1,SQL_INTEGER, &deploymentId, sizeof(deploymentId));
	sql->BindColumn(2,SQL_CHAR, pathname, sizeof(pathname));
	res = sql->Execute();
	if(IS_NOT_SQL_SUCCESS(res)) {
		throw RuntimeError("Failed to getFilesForLastDeployment (execute)");
	}
	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		if (lastDeploymentId == -1) lastDeploymentId = deploymentId;
		if (deploymentId < lastDeploymentId) break;
		ret->add(pathname);
	}
	sql->CloseSQL();
	return ret;
}


/*private*/ void Model::setCurrentUser(User *user)
{
	m_currentUser = user;
}


User *Model::getCurrentUser()
{
	return m_currentUser;
}


void Model::updateUserLastLogin(User &user)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	sql->ExecuteSQL("update dm_user set lastlogin = %s where id = %d ", m_odbc.getNowCol(),user.id());
}


Audit &Model::getAudit()
{
	if(!m_currentUser) {
		throw RuntimeError("Current user not set - can't deploy");
	}

	if(!m_audit) {
		m_audit = new DatabaseAudit(m_currentUser->id(), m_odbc);
	}

	return *m_audit;
}

void Model::addDeploymentAction(Action *action)
{
	if (m_audit && m_audit->deploymentId()>0) {
		AutoPtr<triSQL> sql = m_odbc.GetSQL();
		sql->ExecuteSQL("INSERT INTO dm_deploymentactions(deploymentid,stepid,actionid) VALUES(%d,%d,%d)",
			m_audit->deploymentId(),
			m_audit->currStepId(),
			action->id()
		);
	}
}


Audit &Model::getDummyAudit()
{
	if(!m_audit) {
		m_audit = new Audit();
	}

	return *m_audit;
}


/**
 * Validate the given user's password hash.  To avoid sending the actual hash
 * value across the wire, we query for a match against the hash that we have
 * calculated from the supplied password.  Whilst this might expose how we hash
 * the passwords, it means sniffing the wire whilst attempting a login does not
 * give away the actual password hash, only the hash of what has been entered.
 */
bool Model::validateHashedPassword(User &user, const char *passhash)
{
	char *passhashLit = triSQL::EscapeLiteral(passhash);

	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));

	int res = sql->ExecuteSQL(
			"select u.id from dm_user u "
			"where u.id = %d and u.passhash = %s and u.status = 'N' ",
			user.id(), passhashLit);
	if(IS_NOT_SQL_SUCCESS(res)) {
		SAFE_FREE(passhashLit);
		return false;
	}

	SAFE_FREE(passhashLit);

	sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		return (id == user.id());
	}

	return false;
}


int Model::getNextObjectId(const char *objectType)
{
	// Convert objectType to lowercase
	CharPtr key = strdup(objectType);
	for(char *x = key; *x; x++) { *x = tolower(*x); }

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	sql->SetAutoCommitMode(false);

	long objectId = 0;
	sql->BindColumn(1, SQL_INTEGER, &objectId, sizeof(objectId));

	// Lock the row in our id table so that we can update it
	SQLRETURN res = sql->ExecuteSQL(
		"SELECT id FROM dm_keys WHERE lower(object)='%s' FOR UPDATE",
		(const char*) key);
	if(IS_SQL_SUCCESS(res)) {
		res = sql->FetchRow();
		if(IS_SQL_SUCCESS(res)) {
			objectId++;
			char stmt[256];
			// Increment the id in the table
			AutoPtr<triSQL> upsql = m_odbc.GetSQL();
			sprintf(stmt, "UPDATE dm_keys SET id=%d WHERE lower(object)='%s'",
				objectId, (const char*) key);
			res = upsql->ExecuteSQL(stmt);

			if(IS_SQL_SUCCESS(res)) {
				// Release the lock
				res = sql->EndTransaction(true);
				sql->CloseSQL();
				upsql->CloseSQL();
				sql->SetAutoCommitMode(true);

				if(IS_SQL_SUCCESS(res)) {
					return objectId;
				} else {
					throw RuntimeError("EndTransaction failed");
				}
			} else {
				throw RuntimeError("Update failed");
			}
		} else  {
			// Key does not exist - create it
			AutoPtr<triSQL> maxsql = m_odbc.GetSQL();
			maxsql->BindColumn(1, SQL_INTEGER, &objectId, sizeof(objectId));
			res = maxsql->ExecuteSQL(
				"SELECT coalesce(max(id),0)+1 FROM dm.dm_%s",
				(const char*) key);
			res = maxsql->FetchRow();
			if(IS_SQL_SUCCESS(res)) {
				debug1("Key not found, inserting ('%s', %d)",
					(const char*) key, objectId);
				maxsql->CloseSQL();
			} else {
				objectId = 1;
				debug1("Key not found, max failed, inserting ('%s', %d)",
					(const char*) key, objectId);
			}

			res = sql->EndTransaction(true);
			// Revert to auto-commit mode
			sql->SetAutoCommitMode(true);

			// Insert starting id in the table
			AutoPtr<triSQL> insql = m_odbc.GetSQL();
			res = insql->ExecuteSQL(
				"INSERT INTO dm_keys(object, id) VALUES('%s', %d)",
				(const char*) key, objectId);
			if(IS_SQL_SUCCESS(res)) {
				insql->CloseSQL();
				return objectId;
			} else {
				throw RuntimeError("Insert failed");
			}
		}
	}
	
	sql->SetAutoCommitMode(true);
	throw RuntimeError("Select for update failed");
}


char *Model::internalGetAccessibleChildDomains(int domainid)
{
	AutoPtr<triSQL> sql1 = m_odbc.GetSQL();
	AutoPtr<triSQL> sql2 = m_odbc.GetSQL();

	int rows;

	sql1->BindColumn(1, SQL_INTEGER, &rows, sizeof(rows));
	int res = sql1->ExecuteSQL(
			"select count(*) from dm_domain d where d.domainid =  %d and d.status = 'N' ",
			domainid);
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}
	res = sql1->FetchRow();
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}
	sql1->CloseSQL();

	int id = 0;
	sql2->BindColumn(1, SQL_INTEGER, &id, sizeof(id));

	res = sql2->ExecuteSQL(
			"select d.id from dm_domain d where d.domainid =  %d and d.status = 'N' ",
			domainid);
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	char *ret = NULL;

	debug3("rows=%d",rows);

	if(rows > 0) {
		int *domains = (int*) malloc(rows * sizeof(int));

		int i;

		// Cache the result so that we can start another query
		for(i = 0; i < rows; i++) {
			res = sql2->FetchRow();
			if(IS_NOT_SQL_SUCCESS(res)) {
				return NULL;
			}
			domains[i] = id;
			debug3("domain %d", domains[i]);
		}
		sql2->CloseSQL();

		// Now recurse using the cached results
		for(i = 0; i < rows; i++) {
			char *list = internalGetAccessibleChildDomains(domains[i]);

			char idstr[32];
			sprintf(idstr, "%d", domains[i]);

			if(ret) {
				char *ret2 = (char*) realloc(ret, strlen(idstr) + strlen(ret) + (list ? strlen(list) : 0) + 3);
				if (ret2) {
					ret = ret2;
					strcat(ret, ",");
				}
			} else {
				ret = (char*) malloc(strlen(idstr) + (list ? strlen(list) : 0) + 2);
				if (ret) *ret = '\0';
			}

			if (ret) {
				strcat(ret, idstr);

				if(list) {
					strcat(ret, ",");
					strcat(ret, list);
					SAFE_FREE(list);
				}

				debug3("ret %s", ret);
			} else {
				debug1("ret NULL");
			}
		}

		SAFE_FREE(domains);
	}

	return ret;
}


char *Model::internalGetAccessibleParentDomains(int domainid)
{
	AutoPtr<triSQL> sql1 = m_odbc.GetSQL();
	AutoPtr<triSQL> sql2 = m_odbc.GetSQL();

	int rows = 0;

	sql1->BindColumn(1, SQL_INTEGER, &rows, sizeof(rows));
	int res = sql1->ExecuteSQL(
			"select count(*) from dm_domain c, dm_domain d where c.id = %d and d.id = c.domainid and d.status = 'N' ",
			domainid);
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}
	res=sql1->FetchRow();
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}
	sql1->CloseSQL();

	int id = 0;
	sql2->BindColumn(1, SQL_INTEGER, &id, sizeof(id));

	res = sql2->ExecuteSQL(
			"select d.id from dm_domain c, dm_domain d where c.id = %d and d.id = c.domainid and d.status = 'N' ",
			domainid);
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	char *ret = NULL;
	

	if(rows > 0) {
		int *domains = (int*) malloc(rows * sizeof(int));

		int i;

		// Cache the result so that we can start another query
		for(i = 0; i < rows; i++) {
			res = sql2->FetchRow();
			if(IS_NOT_SQL_SUCCESS(res)) {
				return NULL;
			}
			domains[i] = id;
			debug3("domain %d", domains[i]);
		}
		sql2->CloseSQL();

		// Now recurse using the cached results
		for(i = 0; i < rows; i++) {
			char *list = internalGetAccessibleParentDomains(domains[i]);

			char idstr[32];
			sprintf(idstr, "%d", domains[i]);

			if(ret) {
				char *ret2 = (char*) realloc(ret, strlen(idstr) + strlen(ret) + (list ? strlen(list) : 0) + 3);
				if (ret2) {
					ret = ret2;
					strcat(ret, ",");
				}
			} else {
				ret = (char*) malloc(strlen(idstr) + (list ? strlen(list) : 0) + 2);
				if (ret) *ret = '\0';
			}

			strcat(ret, idstr);

			if(list) {
				strcat(ret, ",");
				strcat(ret, list);
				SAFE_FREE(list);
			}

			debug3("ret %s", ret);
		}

		SAFE_FREE(domains);
	}

	return ret;
}


char *Model::getAccessibleDomains(Domain &domain)
{
	char idstr[32];
	sprintf(idstr, "%d", domain.id());

	char *ret = strdup(idstr);

	debug3("getting child domains for: domain.id=%d",domain.id());

	char *childList = internalGetAccessibleChildDomains(domain.id());
	if(childList) {
		ret = (char*) realloc(ret, strlen(ret) + strlen(childList) + 2);
		strcat(ret, ",");
		strcat(ret, childList);
		SAFE_FREE(childList);
	}

	char *parentList = internalGetAccessibleParentDomains(domain.id());
	if(parentList) {
		ret = (char*) realloc(ret, strlen(ret) + strlen(parentList) + 2);
		strcat(ret, ",");
		strcat(ret, parentList);
		SAFE_FREE(parentList);
	}

	return ret;
}


Environment *Model::findOrCreateEnvironment(
	int id, const char* name, const char *basedir)
{
	Environment *env = NULL;
	if(!(env = m_envCache.get(id))) {
		env = new Environment(*this, id, name, basedir);
		m_envCache.put(env);
	}
	return env;
}


Domain *Model::findOrCreateDomain(int id, const char* name)
{
	Domain *domain = NULL;
	if(!(domain = m_domCache.get(id))) {
		domain = new Domain(*this, id, name);
		m_domCache.put(domain);
	}
	return domain;
}


ServerType *Model::findOrCreateServerType(
	int id, const char *hosttype, LINE_END_FORMAT lineends,
	const char *pathformat,const char *scriptext)
{
	ServerType *st = NULL;
	if(!(st = m_styCache.get(id))) {
		st = new ServerType(id, hosttype, lineends, pathformat,scriptext);
		m_styCache.put(st);
	}
	return st;
}


Server *Model::findOrCreateServer(
	int id, const char *name, const char *hostname, const char *protocol,
	const char *basedir, ServerType *servertype, const char *uname)
{
	Server *server = NULL;
	if(!(server = m_srvCache.get(id))) {
		server = new Server(
			*this, id, name, hostname, protocol, basedir, servertype, uname);
		m_srvCache.put(server);
	}
	return server;
}


Application *Model::findOrCreateApplication(int id, const char *name)
{
	Application *app = NULL;
	if(!(app = m_appCache.get(id))) {
		app = new Application(*this, id, name);
		m_appCache.put(app);
	}
	return app;
}


ApplicationVersion *Model::findOrCreateApplicationVersion(
	int id, const char *name, const char *version, Application *parent)
{
	Application *app = NULL;
	if(!(app = m_appCache.get(id))) {
		app = new ApplicationVersion(*this, id, name, version, parent);
		m_appCache.put(app);
	}
	return app->toApplicationVersion();
}


Component *Model::findOrCreateComponent(
	int id, const char *name, const char *basedir, ComponentFilter rollup,
	ComponentFilter rollback, bool filterItems, bool deployAlways, bool deploySequentially,
	int parentid, int predecessorid, int datasourceid, int buildid)
{
	Component *com = NULL;
	if(!(com = m_comCache.get(id))) {
		com = new Component(
			*this, id, name, basedir, rollup, rollback, filterItems, deployAlways, deploySequentially, parentid, predecessorid, datasourceid, buildid);
		m_comCache.put(com);
	}
	return com;
}


User *Model::findOrCreateUser(
	int id, const char *name, const char* email,
	const char *realname, const char *phone)
{
	User *user = NULL;
	if(name || email) {
		if(!(user = m_usrCache.get(id))) {
			user = new User(*this, id, name, email, realname, phone);
			m_usrCache.put(user);
		}
	}
	return user;
}


UserGroup *Model::findOrCreateUserGroup(
	int id, const char *name, const char* email)
{
	UserGroup *group = NULL;
	if(name || email) {
		if(!(group = m_usgCache.get(id))) {
			group = new UserGroup(*this, id, name, email);
			m_usgCache.put(group);
		}
	}
	return group;
}


ProviderObjectDef *Model::findOrCreateProviderObjectDef(
	int id, const char *name, const char *kind, Plugin *plugin)
{
	ProviderObjectDef *pod = NULL;
	if(!(pod = m_podCache.get(id))) {
		pod = new ProviderObjectDef(id, name, kind, plugin);
		m_podCache.put(pod);
		getPropertiesForProviderObjectDef(*pod);
		getPropertyDefsForProviderObjectDef(*pod);
	}
	return pod;
}


Credentials *Model::findOrCreateCredentials(
	int id, const char *name, CREDENTIALS_KIND kind,
	char *encuser, const char *encpass, const char *filename)
{
	Credentials *cred = NULL;
	if(!(cred = m_crdCache.get(id))) {
		switch(kind) {
		case CREDENTIALS_USE_DIALOG:
		case CREDENTIALS_ENCRYPTED:
			// These should never be set in database
			throw RuntimeError("Credentials kind not allowed in database");
		case CREDENTIALS_IN_DATABASE:
		case CREDENTIALS_FROM_VARS:
			cred = new Credentials(*this,id, name, kind, encuser, encpass);
			break;
		case CREDENTIALS_RTI3_DFO_IN_FILESYSTEM:
		case CREDENTIALS_HARVEST_DFO_IN_FILESYSTEM:
			return new Credentials(*this,id, name, kind, filename);
		case CREDENTIALS_PRIVATE_KEY:
			return new Credentials(*this,id, name, encuser, filename);
		default:
			throw RuntimeError("Unrecognised credentials kind in database (%d)", (int) kind);
		}
		m_crdCache.put(cred);
	}
	return cred;
}


Action *Model::findOrCreateAction(
	int id, ACTION_KIND kind, bool isFunc, /*bool isRemote,*/ bool copyTo,
	bool resIsExpr, const char *name, const char *filepath, const char *shell, bool useTTY)
{
	Action *action = NULL;
	if(!(action = m_actCache.get(id))) {
		action = new Action(
			*this, id, kind, isFunc, /*isRemote,*/ copyTo, resIsExpr, name, filepath, shell, useTTY);
		m_actCache.put(action);
	}
	return action;
}


Script *Model::findOrCreateScript(int repoid, const char *filepath)
{
	char *key = (char*) malloc(strlen(filepath) + 32);
	sprintf(key, "r%d:%s", repoid, filepath);

	Script *script = NULL;
	if(!(script = m_scrCache.get(key))) {
		script = new Script(*this, repoid, filepath);
		m_scrCache.put(key, script);
	}

	SAFE_FREE(key);
	return script;
}


Script *Model::findOrCreateScript(Action *action, int textid)
{
	// TODO: consider whether action name needs to be part of key
	// textid can be null (0) so must put actionid into key
	char key[32];
	sprintf(key, "a%d:%d", action->id(), textid);

	Script *script = NULL;
	if(!(script = m_scrCache.get(key))) {
		script = new Script(*this, action, textid);
		m_scrCache.put(key, script);
	}

	return script;
}


Plugin *Model::findOrCreatePlugin(int id, int version, const char *library)
{
	Plugin *plugin = NULL;
	if(!(plugin = m_plgCache.get(id))) {
		plugin = new Plugin(id, version, library);
		m_plgCache.put(plugin);
	}
	return plugin;
}


Task *Model::findOrCreateTask(int id, const char *name, const char *kind)
{
	Task *task = NULL;
	if(!(task = m_tskCache.get(id))) {
		task = new Task(*this, id, name, kind);
		m_tskCache.put(task);
	}
	return task;
}


void Model::getServersForEnvironment(Environment &env)
{
	char whereClause[1024];
	sprintf(whereClause, "sie.envid = %d and s.id = sie.serverid", env.id());
	AutoPtr<List<Server> > matches = internalGetServers("dm_serversinenv sie", whereClause);
	if(matches) {
		ListIterator<Server> iter(*matches);
		for(Server *s = iter.first(); s; s = iter.next()) {
			env.add(s);
		}
	}
}


/**
 * Gets all the applications and application versions which are allowed in the
 * given environment.
 */
void Model::getApplicationsAllowedInEnvironment(Environment &env)
{
	char whereClause[1024];
	sprintf(whereClause, "aie.envid = %d and a.id = aie.appid OR a.parentid = aie.appid" , env.id());
	AutoPtr<List<Application> > matches = internalGetApplications("dm_appsallowedinenv aie", whereClause, false);
	ListIterator<Application> iter(*matches);
	for(Application *app = iter.first(); app; app = iter.next()) {
		env.add(app);
	}
}


Application *Model::getApplicationDeployedToEnvironment(Environment &env, Application &app)
{
	char whereClause[1024];
	sprintf(whereClause, "aie.envid = %d and a.id = aie.appid and (a.id = %d or a.parentid = %d)",
		env.id(), app.id(), app.id());
	AutoPtr<List<Application> > matches = internalGetApplications("dm_appsinenv aie", whereClause, false);
//	printf("matches = 0x%lx\n",matches);
	ListIterator<Application> iter(*matches);
	Application *depapp = iter.first();

	if(depapp) {
		debug1("getAppDeployedToEnv %d, '%s' - returning %d, '%s'", env.id(), env.name(), depapp->id(), depapp->name());
	} else {
		debug1("getAppDeployedToEnv %d, '%s' - returning NULL", env.id(), env.name());
	}

	return depapp;
}


void Model::getCredentialsForEnvironment(Environment &env)
{
	char whereClause[256];
	sprintf(whereClause, "t.id = %d", env.id());

	Credentials *creds = internalGetCredentials("dm_environment", whereClause);
	env.setCredentials(creds);
}


bool Model::isEnvironmentAvailable(Environment &env)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int unavailstart = 0;
	int unavailend = 0;
	sql->BindColumn(1, SQL_INTEGER, &unavailstart, sizeof(unavailstart));
	sql->BindColumn(2, SQL_INTEGER, &unavailend, sizeof(unavailend));

	int res = sql->ExecuteSQL(
			"select a.unavailstart, a.unavailend from dm_availability a "
			"where a.envid = %d and a.unavailstart < %d and a.unavailend > %d",
			env.id(), m_deploymentStartTime, m_deploymentStartTime);
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	return (sql->FetchRow() == SQL_NO_DATA);
}


void Model::getSubdomainsForDomain(Domain *domain)
{
	char whereClause[256];
	sprintf(whereClause, "d.domainid = %d", domain->id());

	AutoPtr<List<Domain> > matches = internalGetDomains(NULL, whereClause);

	if(matches) {
		ListIterator<Domain> iter(*matches);
		for(Domain *child = iter.first(); child; child = iter.next()) {
			child->setDomain(domain);
			domain->add(child);
		}
	}
}


void Model::getEnvironmentsForDomain(Domain *domain)
{
	char whereClause[256];
	sprintf(whereClause, "e.domainid = %d ", domain->id());
	AutoPtr<List<Environment> > matches = internalGetEnvironments(whereClause, false);

	if(matches) {
		ListIterator<Environment> iter(*matches);
		for(Environment *e = iter.first(); e; e = iter.next()) {
			e->setDomain((Domain*) domain);
			domain->add(e);
		}
	}
}


void Model::getApplicationsForDomain(Domain *domain)
{
	char whereClause[256];
	sprintf(whereClause, "a.domainid = %d", domain->id());

	AutoPtr<List<Application> > matches = internalGetApplications(NULL, whereClause, false);

	if(matches) {
		ListIterator<Application> iter(*matches);
		for(Application *child = iter.first(); child; child = iter.next()) {
			child->setDomain(domain);
			domain->add(child);
		}
	}
}


void Model::getRepositoriesForDomain(Domain *domain)
{
	getProviderObjectsForDomain<Repository>(m_repCache, domain);
}


void Model::getNotifysForDomain(Domain *domain)
{
	getProviderObjectsForDomain<Notify>(m_nfyCache, domain);
}


void Model::getDatasourcesForDomain(Domain *domain)
{
	getProviderObjectsForDomain<Datasource>(m_datCache, domain);
}


void Model::getParentForDomain(Domain &domain)
{
	char whereClause[256];
	sprintf(whereClause, "egc.id = %d and eg.id = egc.domainid", domain.id());

	AutoPtr<List<Domain> > matches = internalGetDomains(
		"dm_domain egc, (dm_domain eg left join dm_user u on eg.ownerid = u.id"
		"left join dm_usergroup g on eg.ogrpid = g.id)", whereClause);

	if(matches && (matches->size() > 0)) {
		ListIterator<Domain> iter(*matches);
		domain.setDomain(iter.first());
		// Note that we don't add an incomplete list of children
		// they will be read from the db only if requested
	}
}


Component *Model::getComponentDeployedToServer(Server &server, Component &comp)
{
	char whereClause[1024];
	//sprintf(whereClause, "cos.serverid = %d and c.id = cos.compid and (c.id = %d or c.parentid = %d)",
	//	server.id(), comp.id(), comp.getParentId());
	int id = comp.getParentId();	// Check Parent ID if a version...
	if (id==0) id = comp.id();		// If not a version, use the actual id
	sprintf(whereClause, "cos.serverid = %d and c.id = cos.compid and (c.id = %d or c.id in "
		"(select x.id from dm_component x where x.parentid=%d))",
		server.id(),id,id);
	AutoPtr<List<Component> > matches = internalGetComponents("dm_compsonserv cos", whereClause);
	ListIterator<Component> iter(*matches);
	Component *depcomp = iter.first();

	if(depcomp) {
		debug1("getCompDeployedToServ %d, '%s' - returning %d, '%s'", server.id(), server.name(), depcomp->id(), depcomp->name());
	} else {
		debug1("getCompDeployedToServ %d, '%s' - returning NULL", server.id(), server.name());
	}

	return depcomp;
}


void Model::getCredentialsForServer(Server &server)
{
	char whereClause[256];
	sprintf(whereClause, "t.id = %d", server.id());

	Credentials *creds = internalGetCredentials(server.table(), whereClause);
	server.setCredentials(creds);
}


void Model::getActionsForApplication(Application &app)
{
	char whereClause[1024];

	sprintf(whereClause, "app.id = %d and a.id = app.actionid", app.id());
	Action *action = internalGetAction("dm_application app", whereClause, false);

	sprintf(whereClause, "app.id = %d and a.id = app.preactionid", app.id());
	Action *preAction = internalGetAction("dm_application app", whereClause, false);

	sprintf(whereClause, "app.id = %d and a.id = app.postactionid", app.id());
	Action *postAction = internalGetAction("dm_application app", whereClause, false);

	app.setActions(action, preAction, postAction);
}


/**
 * Note that this is only used to check if an application has any components
 * and to implement the comploop statement.  For component deployment the
 * getApplicationComponentGraph() method is used instead and this takes into
 * account the parallel deployment of components.
 */

int Model::addCompsForApp2(Application &app,HashtableById< List<Component> > &preds, int compid)
{
	int count2=0;
	List<Component> *ps = preds.get(compid);
	if (ps) {
		ListIterator<Component> iter2(*ps);
		for(Component *p = iter2.first(); p; p = iter2.next()) {
			debug1("Application %d, '%s' - adding next component %d, '%s'", app.id(), app.name(), p->id(), p->name());
			app.addComponent(p);
			count2++;
			count2+=addCompsForApp2(app,preds,p->id());
		}
	}
	return count2;
}

void Model::getComponentsForApplication(Application &app)
{
	//char whereClause[1024];
	//sprintf(whereClause, "ac.appid = %d and c.id = ac.compid", app.id()); // TODO:  was "order by ac.comporder", but comporder replaced with predecessorid
	//AutoPtr<List<Component> > matches = internalGetComponents("dm_applicationcomponent ac", whereClause);
	//if(matches) {
	//	ListIterator<Component> iter(*matches);
	//	for(Component *c = iter.first(); c; c = iter.next()) {
	//		app.addComponent(c);
	//	}
	//}
	//debug1("getComponentsForApplication %d, '%s'", app.id(), app.name());

	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char compName[DB_NAME_LEN];
	char baseDir[DB_PATH_LEN];
	int rollup;
	int rollback;
	char filterItems[DB_BOOL_LEN];
	char deployAlways[DB_BOOL_LEN];
	char deploySequentially[DB_BOOL_LEN];
	int predecessorid = 0;
	int parentid = 0;
	int datasourceid = 0;
	int buildid = 0;
	SQLLEN ni_baseDir = 0, ni_rollup = 0, ni_rollback = 0, ni_filterItems = 0,
		ni_deployAlways = 0, ni_deploySequentially = 0, ni_predecessorid = 0,
		ni_parentid = 0, ni_datasourceid, ni_buildid;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, compName, sizeof(compName));
	sql->BindColumn(3, SQL_CHAR, baseDir, sizeof(baseDir), &ni_baseDir);
	sql->BindColumn(4, SQL_INTEGER, &rollup, sizeof(rollup), &ni_rollup);
	sql->BindColumn(5, SQL_INTEGER, &rollback, sizeof(rollback), &ni_rollback);
	sql->BindColumn(6, SQL_CHAR, filterItems, sizeof(filterItems), &ni_filterItems);
	sql->BindColumn(7, SQL_CHAR, deployAlways, sizeof(deployAlways), &ni_deployAlways);
	sql->BindColumn(8, SQL_INTEGER, &predecessorid, sizeof(predecessorid), &ni_predecessorid);
	sql->BindColumn(9, SQL_INTEGER, &parentid, sizeof(parentid), &ni_parentid);
	sql->BindColumn(10, SQL_CHAR, deploySequentially, sizeof(deploySequentially), &ni_deploySequentially);
	sql->BindColumn(11, SQL_INTEGER, &datasourceid, sizeof(datasourceid), &ni_datasourceid);
	sql->BindColumn(12, SQL_INTEGER, &buildid, sizeof(buildid), &ni_buildid);

	// This query has been altered to use dm_applicationcomponentflows, but it
	// treats them purely to give what it thinks is still a predecessorid.
	//
	// Note: dm_applicationcomponentflows occasionally gets duplicate rows due to a UI bug. Not sure
	// what is causing this but in the meantime, let's stick a distinct on this query (won't hurt)
	debug3("app.id=%d",app.id());
	int res = sql->ExecuteSQL(
		"select distinct c.id, c.name, c.basedir, c.rollup, c.rollback, c.filteritems, c.deployalways, cf.objfrom, c.parentid, c.deploysequentially, c.datasourceid, c.lastbuildnumber "
		"from dm_applicationcomponent ac "
		"left outer join dm_component c on c.id = ac.compid and c.status = 'N' "
		"left outer join dm_applicationcomponentflows cf on cf.appid = ac.appid and cf.objto = ac.compid "
		"where ac.appid = %d and c.id is not null order by 8,1 ", app.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}


	List<Component> roots;			// List of all start components (no predecessor)
	HashtableById< List<Component> > preds;	// Hashtable of non-start components indexed by their predecessor
	List<Component> *predlist;

	int count = 0;
	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		Component *comp = findOrCreateComponent(
			id, compName, STR_NULL_IND(baseDir,NULL), (ComponentFilter) NULL_IND(rollup, false),
			(ComponentFilter) NULL_IND(rollback, false),
			BOOL_NULL_IND(filterItems, false), BOOL_NULL_IND(deployAlways, false), BOOL_NULL_IND(deploySequentially, false),
			NULL_IND(parentid,0), NULL_IND(predecessorid,0), NULL_IND(datasourceid,0), NULL_IND(buildid,0));
		count++;
		if(NULL_IND(predecessorid, 0) != 0) {
			debug1("Component %d, '%s' with predecessor %d", id, compName, predecessorid);
			predlist = preds.get(predecessorid);
			if (!predlist) predlist = new List<Component>;
			predlist->add(comp);
			preds.put(predecessorid, predlist);
		} else {
			debug1("Component %d, '%s' with no predecessor", id, compName);
			roots.add(comp);
		}
	}

	// Go through the roots one at a time and add them to the list followed
	// immediatedly by any components that hang from them
	int count2 = 0;
	ListIterator<Component> iter(roots);
	for(Component *c = iter.first(); c; c = iter.next()) {
		debug1("Application %d, '%s' - adding root component %d, '%s'", app.id(), app.name(), c->id(), c->name());
		app.addComponent(c);
		count2+=addCompsForApp2(app,preds,c->id());
	}

	// This is just a sanity check to make sure we didn't lose an AV
	if(count != count2) {
		debug1("WARNING: Component counts differ %d != %d", count, count2);
	}

}


// ===
ApplicationComponentGraph *Model::getApplicationComponentGraph(Application &app)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char compName[DB_NAME_LEN];
	char baseDir[DB_PATH_LEN];
	int rollup;
	int rollback;
	char filterItems[DB_BOOL_LEN];
	char deployAlways[DB_BOOL_LEN];
	char deploySequentially[DB_BOOL_LEN];
	int parentid;
	int predecessorid;
	int datasourceid;
	int buildid;
	SQLLEN ni_baseDir = 0, ni_rollup = 0, ni_rollback = 0,
		ni_filterItems = 0, ni_deployAlways = 0, ni_deploySequentially = 0,
		ni_parentid = 0, ni_predecessorid = 0, ni_datasourceid = 0, ni_buildid;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, compName, sizeof(compName));
	sql->BindColumn(3, SQL_CHAR, baseDir, sizeof(baseDir), &ni_baseDir);
	sql->BindColumn(4, SQL_INTEGER, &rollup, sizeof(rollup), &ni_rollup);
	sql->BindColumn(5, SQL_INTEGER, &rollback, sizeof(rollback), &ni_rollback);
	sql->BindColumn(6, SQL_CHAR, filterItems, sizeof(filterItems), &ni_filterItems);
	sql->BindColumn(7, SQL_CHAR, deployAlways, sizeof(deployAlways), &ni_deployAlways);
	sql->BindColumn(8, SQL_INTEGER, &parentid, sizeof(parentid), &ni_parentid);
	sql->BindColumn(9, SQL_INTEGER, &predecessorid, sizeof(predecessorid), &ni_predecessorid);
	sql->BindColumn(10, SQL_CHAR, deploySequentially, sizeof(deploySequentially), &ni_deploySequentially);
	sql->BindColumn(11, SQL_INTEGER, &datasourceid, sizeof(datasourceid), &ni_datasourceid);
	sql->BindColumn(12, SQL_INTEGER, &buildid, sizeof(buildid), &ni_buildid);

	int res = sql->ExecuteSQL(
		"select c.id, c.name, c.basedir, c.rollup, c.rollback, c.filteritems, c.deployalways, c.parentid, c.predecessorid, c.deploysequentially, c.datasourceid, c.lastbuildnumber "
		"from dm_applicationcomponent ac, dm_component c "
		"where ac.appid = %d AND c.id = ac.compid and c.status = 'N'", app.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		sql->CloseSQL();
		return NULL;
	}

	ApplicationComponentGraph *graph = new ApplicationComponentGraph();

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		Component *comp = findOrCreateComponent(
			id, compName, STR_NULL_IND(baseDir, NULL), (ComponentFilter) NULL_IND(rollup, false),
			(ComponentFilter) NULL_IND(rollback, false),
			BOOL_NULL_IND(filterItems, false), BOOL_NULL_IND(deployAlways, false), BOOL_NULL_IND(deploySequentially, false),
			NULL_IND(parentid,0), NULL_IND(predecessorid,0), NULL_IND(datasourceid,0), NULL_IND(buildid,0));
		graph->addComponent(*comp);
	}

	sql->CloseSQL();

	int compfromid;
	int comptoid;
	SQLLEN ni_compfromid = 0;
	SQLLEN ni_comptoid = 0;

	sql->BindColumn(1, SQL_INTEGER, &compfromid, sizeof(compfromid), &ni_compfromid);
	sql->BindColumn(2, SQL_INTEGER, &comptoid, sizeof(comptoid), &ni_comptoid);

	res = sql->ExecuteSQL(
		"select distinct acf.objfrom, acf.objto "
		"from dm_applicationcomponentflows acf "
		"where acf.appid = %d	"
		"union	"
		"select null,a.compid	"
		"from dm.dm_applicationcomponent a	"
		"where a.appid = %d	"
		"and a.compid not in (select b.objto from dm.dm_applicationcomponentflows b where b.appid=a.appid)",
		app.id(),app.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		sql->CloseSQL();
		return NULL;
	}

	// Get flows
	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		if (NULL_IND(comptoid,0) != 0) {
			graph->addFlow(NULL_IND(compfromid,0), NULL_IND(comptoid,0));
		}
	}
	sql->CloseSQL();

	return graph;
}

ReleaseApplicationGraph *Model::getReleaseApplicationGraph(Application &app)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	if (!app.isRelease()) return (ReleaseApplicationGraph *)0;	// Not a release

	ReleaseApplicationGraph *graph = new ReleaseApplicationGraph();

	char WhereClause[256];
	sprintf(WhereClause,"ac.appid=%d AND a.id=ac.childappid",app.id());
	AutoPtr< List<Application> > apps = internalGetApplications("dm_applicationcomponent ac",WhereClause,false);

	ListIterator<Application> iter(*apps);
	for (Application *appx=iter.first();appx;appx=iter.next()) {
		graph->addApplication(appx);
	}


	int res;
	int appfromid;
	int apptoid;
	SQLLEN ni_appfromid = 0;

	sql->BindColumn(1, SQL_INTEGER, &appfromid, sizeof(appfromid), &ni_appfromid);
	sql->BindColumn(2, SQL_INTEGER, &apptoid, sizeof(apptoid));

	res = sql->ExecuteSQL(
		"select acf.objfrom, acf.objto "
		"from dm_applicationcomponentflows acf "
		"where acf.appid = %d "
		"union	"
		"select null,a.childappid	"
		"from dm.dm_applicationcomponent a	"
		"where a.appid = %d	"
		"and a.childappid not in (select b.objto from dm.dm_applicationcomponentflows b where b.appid=a.appid)",
		app.id(),app.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		sql->CloseSQL();
		return NULL;
	}

	// Get flows
	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		graph->addFlow(NULL_IND(appfromid,0), apptoid);
	}
	sql->CloseSQL();

	return graph;
}

ApplicationVersion *Model::getVersionOfApplication(Application &app, const char *name)
{
	char *nameLit = triSQL::EscapeLiteral(name);
	char whereClause[1024];
	sprintf(whereClause, "a.parentid = %d AND a.name = %s", app.id(), nameLit);
	SAFE_FREE(nameLit);

	ApplicationVersion *version = NULL;
	AutoPtr<List<Application> > matches = internalGetApplications(NULL, whereClause, false);
	if(matches) {
		ListIterator<Application> iter(*matches);
		Application *app = iter.first();
		if(app && (app->kind() == OBJ_KIND_APPVERSION)) {
			return (ApplicationVersion*) app;
		}
	}
	return NULL;
}


bool Model::isApplicationAvailable(Application &app, Environment &env)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int appid = 0;
	int starttime = 0;
	int endtime = 0;
	sql->BindColumn(1, SQL_INTEGER, &appid, sizeof(appid));
	sql->BindColumn(2, SQL_INTEGER, &starttime, sizeof(starttime));
	sql->BindColumn(3, SQL_INTEGER, &endtime, sizeof(endtime));

	int res = sql->ExecuteSQL(
			"select c.appid, c.starttime, c.endtime from dm_calendar c "
			"where c.status='N' and c.envid = %d and c.starttime < %d and c.endtime > %d",
			env.id(), m_deploymentStartTime, m_deploymentStartTime);
	if(IS_NOT_SQL_SUCCESS(res)) {
		sql->CloseSQL();
		return false;
	}

	bool okay = true;
	int rows=0;
	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		rows++;
		if(appid == app.id()) {
			// reserved for this application, so okay
			sql->CloseSQL();
			return true;
		}
		// at least one reservation which is not this application
		okay = false;
	}
	sql->CloseSQL();
	if (rows==0) {
		// No calendar reservations at all for this app. Is the environment "open for business"?
		char calusage[2];
		SQLLEN ni_calusage = 0;
		sql->BindColumn(1, SQL_CHAR, calusage,sizeof(calusage),&ni_calusage);
		res = sql->ExecuteSQL("select calusage from dm_environment where id=%d",env.id());
		if(IS_NOT_SQL_SUCCESS(res)) {
			sql->CloseSQL();
			return NULL;
		}
		res = sql->FetchRow();
		if (IS_SQL_SUCCESS(res)) {
			if (!IS_NULL(calusage)) {
				// calusage is either "E" (always available (E)xcept when denied by calendar
				// or "O" ((O)nly available when allowed by calendar.
				// Default (if calusage is null) is "E". So only deny if the setting is "O"
				if (toupper(calusage[0])=='O') okay=false;
			}
		}
		sql->CloseSQL();
	}
	return okay;
}


void Model::getPredecessorForApplicationVersion(ApplicationVersion &av)
{
	debug3("** getting predecessor for %s",av.name());
	char whereClause[1024];
	sprintf(whereClause, "a.id = b.predecessorid and b.id = %d", av.id());

	ApplicationVersion *predecessor = NULL;
	AutoPtr<List<Application> > matches = internalGetApplications("dm_application b", whereClause, false);
	if(matches) {
		ListIterator<Application> iter(*matches);
		Application *app = iter.first();
		if(app && (app->kind() == OBJ_KIND_APPVERSION || app->kind() == OBJ_KIND_APPLICATION)) {
			predecessor = (ApplicationVersion*) app;
		}
	}
	av.setPredecessor(predecessor);
}


void Model::getSuccessorsForApplicationVersion(ApplicationVersion &av)
{
	char whereClause[1024];
	sprintf(whereClause, "a.predecessorid = %d", av.id());

	ApplicationVersion *predecessor = NULL;
	AutoPtr<List<Application> > matches = internalGetApplications(NULL, whereClause, false);
	if(matches) {
		ListIterator<Application> iter(*matches);
		for(Application *app = iter.first(); app; app = iter.next()) {
			if(app->kind() == OBJ_KIND_APPVERSION) {
				av.addSuccessor((ApplicationVersion*) app);
			}
		}
	}
}


DMArray *Model::getApprovalsForApplicationVersion(ApplicationVersion &av)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	char approved[DB_BOOL_LEN];
	int domainid;
	char domainName[DB_NAME_LEN];

	SQLRETURN res = sql->PrepareStatement(
		"select a.approved, a.domainid, d.name from dm_approval a, dm_domain d "
		"where a.id in (select max(b.id) from dm_approval b "
		"where b.appid = ? AND b.domainid = a.domainid) and d.id = a.domainid");
	if(IS_NOT_SQL_SUCCESS(res)) {
		throw RuntimeError("Failed to get approvals for application");
	}
	int avid = av.id();
	sql->BindParameter(1, SQL_INTEGER, sizeof(avid), &avid, sizeof(avid));
	sql->BindColumn(1, SQL_CHAR, approved, sizeof(approved));
	sql->BindColumn(2, SQL_INTEGER, &domainid, sizeof(domainid));
	sql->BindColumn(3, SQL_CHAR, domainName, sizeof(domainName));
	res = sql->Execute();
	DMArray *ret =  new DMArray(false);
	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		debug1("Approval %s for domain %d '%s'", approved, domainid, domainName);
		ret->put(domainName, new Variable(NULL, (approved[0] == 'Y' ? "approved" : "rejected")));
	}
	sql->CloseSQL();
	return ret;
}


void Model::alterObjectVars(Object *obj, class DMArray *attrs)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	AutoPtr<StringList> keys = attrs->keys();
	StringListIterator iter(*keys);
	for(const char *key = iter.first(); key; key = iter.next()) {
		Variable  *var = attrs->get(key);
		CharPtr val = (var ? var->toString() : NULL);
		if(!val.isNull()) {
			debug1("SET '%s' = '%s'", key, (const char *)val);

			int objid = obj->id();
			SQLRETURN res = sql->PrepareStatement(
				"update %svars set value = ? where %s = ? and name = ?",
				obj->table(), obj->fk());
			if(IS_NOT_SQL_SUCCESS(res)) {
				throw RuntimeError("Failed to update object vars");
			}
			sql->BindParameter(1, SQL_CHAR, strlen(val), (char*) val, strlen(val));
			sql->BindParameter(2, SQL_INTEGER, sizeof(objid), &objid, sizeof(objid));
			sql->BindParameter(3, SQL_CHAR, strlen(key), (char*) key, strlen(key));
			res = sql->ExecuteIgnoringErrors();
			sql->CloseSQL();
			if(IS_NOT_SQL_SUCCESS(res)) {
				if((res != SQL_ERROR) && (res != SQL_NO_DATA)) {
					throw RuntimeError("Failed to update object vars (2), %d", res);
				}
				res = sql->PrepareStatement(
					"insert into %svars(%s,name,value,nocase) values(?,?,?,'N')",
					obj->table(), obj->fk());
				if(IS_NOT_SQL_SUCCESS(res)) {
					throw RuntimeError("Failed to insert object vars");
				}
				sql->BindParameter(1, SQL_INTEGER, sizeof(objid), &objid, sizeof(objid));
				sql->BindParameter(2, SQL_CHAR, strlen(key), (char*) key, strlen(key));
				sql->BindParameter(3, SQL_CHAR, strlen(val), (char*) val, strlen(val));
				res = sql->Execute();
				sql->CloseSQL();
				if(IS_NOT_SQL_SUCCESS(res)) {
					throw RuntimeError("Failed to insert object vars (2)");
				}
			}
		}
	}
}


List<Server> *Model::getAllServersForComponent(Component &comp, Environment *env)
{
	char whereClause[2048];
	// Modified to use dm_servercomptype
	if (env) {
		sprintf(whereClause,
			"c.id = %d and sct.comptypeid = c.comptypeid and sct.serverid = s.id and sie.envid=%d and sie.serverid = s.id",
			comp.id(),env->id());
		return internalGetServers("dm_component c, dm_servercomptype sct, dm_serversinenv sie", whereClause);
	} else {
		sprintf(whereClause,
			"c.id = %d and cos.compid = c.id and cos.serverid = s.id",
			comp.id());
		return internalGetServers("dm_component c, dm_compsonserv cos", whereClause);
	}
	
}


List<Server> *Model::getServerSubsetForComponent(
		Component &comp, List<Server> &servers)
{
	debug3("Getting server subset for component %s",comp.name());
	// This won't work if the server set is empty
	if(servers.size() == 0) {
		// This can happen if, for example, we're running a "comploop" inside a stand-alone
		// action when there will be no environment on the stack and thus no server set.
		// printf("WARNING: Server set is empty when processing component '%s'\n", comp.name());
		return new List<Server>();
	}

	char numbuf[128];
	int initialsize = 256;
	char *whereClause = (char *)malloc(initialsize);
	int cursize = initialsize;
	sprintf(whereClause,
		"c.id = %d and sct.comptypeid = c.comptypeid and sct.serverid = s.id and s.id in (",comp.id());
	bool first = true;
	ListIterator<Server> iter(servers);
	for(Server *s = iter.first(); s; s = iter.next()) {
		sprintf(numbuf,(first ? "%d" : ",%d"), s->id());
		strcat(whereClause,numbuf);
		debug3("strlen(whereClause)=%d cursize=%d",strlen(whereClause),cursize);
		first = false;
		if (strlen(whereClause)>(cursize-20)) {
			cursize += 128;
			debug3("reallocating whereClause to %d",cursize);
			whereClause = (char *)realloc(whereClause,cursize);
		}
	}
	strcat(whereClause,")");
	//return internalGetServers("dm_component c, dm_compsallowedonserv caos", whereClause);
	debug3("Calling internalGetServers");
	if (servers.size()>0) {
		return internalGetServers("dm_component c, dm_servercomptype sct", whereClause);
	} else {
		return new List<Server>();
	}
}

Server *Model::getServerById(int id)
{
	char whereClause[2048];
	sprintf(whereClause,"s.id=%d",id);
	List<Server> *sl = internalGetServers(NULL, whereClause);
	ListIterator<Server> iter(*sl);
	return iter.first();
}

void Model::setUname(Server &s,const char *uname)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	int id = s.id();
	sql->PrepareStatement("UPDATE dm_server SET uname=? WHERE serverid=?");
	sql->BindParameter(1,SQL_CHAR,strlen(uname),(char *)uname,strlen(uname));
	sql->BindParameter(2,SQL_INTEGER,sizeof(id),&id,sizeof(id));
	sql->Execute();
	sql->CloseSQL();
}




void Model::getItemsForComponent(Component &comp)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char ciName[DB_NAME_LEN];
	int ciRepoId = 0;
	char ciTarget[DB_PATH_LEN];
	int rollup;
	int rollback;
	int predecessorid = 0;
	SQLLEN ni_ciRepoId = 0, ni_ciTarget = 0,
		ni_rollup = 0, ni_rollback = 0,
		ni_predecessorid = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, ciName, sizeof(ciName));
	sql->BindColumn(3, SQL_INTEGER, &ciRepoId, sizeof(ciRepoId), &ni_ciRepoId);
	sql->BindColumn(4, SQL_CHAR, ciTarget, sizeof(ciTarget), &ni_ciTarget);
	sql->BindColumn(5, SQL_INTEGER, &rollup, sizeof(rollup), &ni_rollup);
	sql->BindColumn(6, SQL_INTEGER, &rollback, sizeof(rollback), &ni_rollback);
	sql->BindColumn(7, SQL_INTEGER, &predecessorid, sizeof(predecessorid), &ni_predecessorid);

	int res = sql->ExecuteSQL(
			"select ci.id, ci.name, ci.repositoryid, ci.target, ci.rollup, ci.rollback, ci.predecessorid "
			"from dm_componentitem ci "
			"where ci.compid = %d and ci.status = 'N' "
			"order by 1 ", comp.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	List<ComponentItem> roots;			// List of all start items (no predecessor)
	HashtableById<ComponentItem> preds;	// Hashtable of non-start items indexed by their predecessor

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		ComponentItem *ci = new ComponentItem(
			*this, id, ciName, comp, NULL_IND(ciRepoId, 0), NULL_IND(ciTarget, NULL),
			(ComponentFilter) NULL_IND(rollup, 0), (ComponentFilter) NULL_IND(rollback, 0));
		if(NULL_IND(predecessorid, 0) != 0) {
			//debug1("ComponentItem %d, '%s' with predecessor %d", id, ciName, predecessorid);
			preds.put(predecessorid, ci);
		} else {
			//debug1("ComponentItem %d, '%s' with no predecessor", id, ciName);
			roots.add(ci);
		}
		getPropertiesForComponentItem(*ci);
	}	

	// Go through the roots one at a time and add them to the list followed
	// immediatedly by any items that hang from them
	ListIterator<ComponentItem> iter(roots);
	for(ComponentItem *ci = iter.first(); ci; ci = iter.next()) {
		//debug1("Component %d, '%s' - adding root component item %d, '%s'", comp.id(), comp.name(), ci->id(), ci->name());
		comp.add(ci);
		for(ComponentItem *pi = preds.get(ci->id()); pi; pi = preds.get(pi->id())) {
			//debug1("Component %d, '%s' - adding next component item %d, '%s'", comp.id(), comp.name(), pi->id(), pi->name());
			comp.add(pi);
		}
	}
}


void Model::getActionsForComponent(Component &comp)
{
	char whereClause[1024];

	sprintf(whereClause, "c.id = %d and a.id = c.actionid", comp.id());
	Action *action = internalGetAction("dm_component c", whereClause, false);

	sprintf(whereClause, "c.id = %d and a.id = c.preactionid", comp.id());
	Action *preAction = internalGetAction("dm_component c", whereClause, false);

	sprintf(whereClause, "c.id = %d and a.id = c.postactionid", comp.id());
	Action *postAction = internalGetAction("dm_component c", whereClause, false);

	comp.setActions(action, preAction, postAction);
}


void Model::getRepositoryForComponentItem(ComponentItem &ci)
{
	char whereClause[1024];
	sprintf(whereClause, "t.id = %d", ci.repoid());

	AutoPtr<List<Repository> > matches = internalGetProviderObjects<Repository>(
		m_repCache, NULL, whereClause, false);
	if(matches && (matches->size() > 0)) {
		ListIterator<Repository> iter(*matches);
		ci.setRepository(iter.first());
	}
}


void Model::getPropertiesForComponentItem(ComponentItem &ci)
{
	AutoPtr<List<Property> > matches = internalGetProperties(
		"dm_compitem", "compitemid", ci.id());
	if(matches) {
		ListIterator<Property> iter(*matches);
		for(Property *prop = iter.first(); prop; prop = iter.next()) {
			ci.setProperty(prop);
		}
	}
}


void Model::getUsersForUserGroup(UserGroup &group)
{
	char whereClause[1024];
	sprintf(whereClause, "u.id = uig.userid and uig.groupid = %d", group.id());

	AutoPtr<List<User> > matches = internalGetUsers(
		"dm_usersingroup uig", whereClause, false);

	if(matches) {
		ListIterator<User> iter(*matches);
		for(User *user = iter.first(); user; user = iter.next()) {
			group.add(user);
		}
	}
}


void Model::getUserGroupsForUser(User &user)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char groupName[DB_NAME_LEN];
	char groupEmail[DB_EMAIL_LEN];
	SQLLEN ni_groupEmail = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, groupName, sizeof(groupName));
	sql->BindColumn(3, SQL_CHAR, groupEmail, sizeof(groupEmail), &ni_groupEmail);

	int res = sql->ExecuteSQL("select g.id, g.name, g.email "
			"from dm_usergroup g, dm_usersingroup uig "
			"where g.id = uig.groupid and uig.userid = %d and g.status = 'N'", user.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		UserGroup *group = findOrCreateUserGroup(id, groupName, NULL_IND(groupEmail, NULL));
		user.add(group);
	}
}


void Model::setActionNodes(class ActionNodeList *actions)
{
	if(m_actionNodes) {
		m_actionNodes->merge(*actions);
		SAFE_DELETE(actions);
	} else {
		m_actionNodes = actions;
	}
}


class ActionNode *Model::firstActionNode()
{
	return m_actionNodes ? m_actionNodes->firstActionNode() : NULL;
}


class ActionNode *Model::getActionNode(const char *name)
{
	debug3("name=[%s] m_actionNodes = 0x%lx\n",name,m_actionNodes);
	return m_actionNodes ? m_actionNodes->getActionNode(name) : NULL;
}


void Model::getOwnerForObject(Object &obj)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int ownerid = 0;
	int ogrpid = 0;
	char userName[DB_NAME_LEN];
	char userEmail[DB_EMAIL_LEN];
	char userReal[DB_NAME_LEN];
	char userPhone[DB_PHONE_LEN];
	char groupName[DB_NAME_LEN];
	char groupEmail[DB_EMAIL_LEN];
	SQLLEN ni_ownerid = 0, ni_ogrpid = 0, ni_userName = 0, ni_userEmail = 0,
		ni_userReal = 0, ni_userPhone = 0, ni_groupName = 0, ni_groupEmail = 0;

	sql->BindColumn(1, SQL_INTEGER, &ownerid, sizeof(ownerid), &ni_ownerid);
	sql->BindColumn(2, SQL_INTEGER, &ogrpid, sizeof(ogrpid), &ni_ogrpid);
	sql->BindColumn(3, SQL_CHAR, userName, sizeof(userName), &ni_userName);
	sql->BindColumn(4, SQL_CHAR, userEmail, sizeof(userEmail), &ni_userEmail);
	sql->BindColumn(5, SQL_CHAR, userReal, sizeof(userReal), &ni_userReal);
	sql->BindColumn(6, SQL_CHAR, userPhone, sizeof(userPhone), &ni_userPhone);
	sql->BindColumn(7, SQL_CHAR, groupName, sizeof(groupName), &ni_groupName);
	sql->BindColumn(8, SQL_CHAR, groupEmail, sizeof(groupEmail), &ni_groupEmail);

	int res = sql->ExecuteSQL(
			"select o.ownerid, o.ogrpid, u.name, u.email, "
			" u.realname, u.phone, g.name, g.email "
			"from (%s o left join dm_user u on o.ownerid = u.id and u.status = 'N' "
			"left join dm_usergroup g on o.ogrpid = g.id and g.status = 'N') "
			"where o.id = %d ", obj.table(), obj.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		User *owner = findOrCreateUser(NULL_IND(ownerid, 0),
			NULL_IND(userName, NULL),
			NULL_IND(userEmail, NULL),
			NULL_IND(userReal, NULL),
			NULL_IND(userPhone, NULL));
		UserGroup *owngrp = findOrCreateUserGroup(NULL_IND(ogrpid, 0),
			NULL_IND(groupName, NULL),
			NULL_IND(groupEmail, NULL));

		if(owner) {
			obj.setOwner(owner);
		} else if(owngrp) {
			obj.setOwner(owngrp);
		}
	}
}

void Model::getMetaDataForObject(Object &obj)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int creatorid = 0;
	int modifierid = 0;
	int created = 0;
	int modified = 0;
	char creatorUserName[DB_NAME_LEN];
	char creatorUserEmail[DB_EMAIL_LEN];
	char creatorUserReal[DB_NAME_LEN];
	char creatorUserPhone[DB_PHONE_LEN];
	char modifierUserName[DB_NAME_LEN];
	char modifierUserEmail[DB_EMAIL_LEN];
	char modifierUserReal[DB_NAME_LEN];
	char modifierUserPhone[DB_PHONE_LEN];
	
	SQLLEN ni_creatorid = 0, ni_modifierid = 0, ni_created = 0, ni_modified = 0,
		ni_creatorUserName = 0, ni_creatorUserEmail = 0, ni_creatorUserReal = 0, ni_creatorUserPhone = 0,
		ni_modifierUserName = 0, ni_modifierUserEmail = 0, ni_modifierUserReal = 0, ni_modifierUserPhone = 0;

	sql->BindColumn(1,  SQL_INTEGER, &creatorid, sizeof(creatorid), &ni_creatorid);
	sql->BindColumn(2,  SQL_INTEGER, &modifierid, sizeof(modifierid), &ni_modifierid);
	sql->BindColumn(3,  SQL_INTEGER, &created, sizeof(created), &ni_created);
	sql->BindColumn(4,  SQL_INTEGER, &modified, sizeof(modified), &ni_modified);
	sql->BindColumn(5,  SQL_CHAR, creatorUserName, sizeof(creatorUserName), &ni_creatorUserName);
	sql->BindColumn(6,  SQL_CHAR, creatorUserEmail, sizeof(creatorUserEmail), &ni_creatorUserEmail);
	sql->BindColumn(7,  SQL_CHAR, creatorUserReal, sizeof(creatorUserReal), &ni_creatorUserReal);
	sql->BindColumn(8,  SQL_CHAR, creatorUserPhone, sizeof(creatorUserPhone), &ni_creatorUserPhone);
	sql->BindColumn(9,  SQL_CHAR, modifierUserName, sizeof(modifierUserName), &ni_modifierUserName);
	sql->BindColumn(10,  SQL_CHAR, modifierUserEmail, sizeof(modifierUserEmail), &ni_modifierUserEmail);
	sql->BindColumn(11,  SQL_CHAR, modifierUserReal, sizeof(modifierUserReal), &ni_modifierUserReal);
	sql->BindColumn(12, SQL_CHAR, modifierUserPhone, sizeof(modifierUserPhone), &ni_modifierUserPhone);
	

	int res = sql->ExecuteSQL(
			"select o.creatorid, o.modifierid, o.created, o.modified, "
			" c.name, c.email, c.realname, c.phone, "
			" m.name, m.email, m.realname, m.phone "
			"from (%s o left join dm_user c on o.creatorid = c.id and c.status = 'N' "
			"left join dm_user m on o.modifierid = m.id and m.status = 'N') "
			"where o.id = %d ", obj.table(), obj.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		User *creator = findOrCreateUser(NULL_IND(creatorid, 0),
			NULL_IND(creatorUserName, NULL),
			NULL_IND(creatorUserEmail, NULL),
			NULL_IND(creatorUserReal, NULL),
			NULL_IND(creatorUserPhone, NULL));
		User *modifier = findOrCreateUser(NULL_IND(modifierid, 0),
			NULL_IND(modifierUserName, NULL),
			NULL_IND(modifierUserEmail, NULL),
			NULL_IND(modifierUserReal, NULL),
			NULL_IND(modifierUserPhone, NULL));

		obj.setCreator(creator);
		obj.setModifier(modifier);
		obj.setCreationTime(created);
		obj.setModifiedTime(modified);
	}
}

bool stringIsNumber(const char *str)
{
	for (const char *x = str; *x; x++) {
		if (!isdigit(*x)) return false;
	}
	return true;
}

void Model::getVariablesForObject(Object &obj, Scope &vars)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char varName[DB_NAME_LEN];
	char varValue[DB_VARVAL_LEN];
	int arrayid = 0;
	char nocase[DB_BOOL_LEN];
	SQLLEN ni_varValue = 0, ni_arrayid = 0, ni_nocase = 0;

	sql->BindColumn(1, SQL_CHAR, varName, sizeof(varName));
	sql->BindColumn(2, SQL_CHAR, varValue, sizeof(varValue), &ni_varValue);
	sql->BindColumn(3, SQL_INTEGER, &arrayid, sizeof(arrayid), &ni_arrayid);
	sql->BindColumn(4, SQL_CHAR, nocase, sizeof(nocase), &ni_nocase);

	int res = sql->ExecuteSQL("select v.name, v.value, v.arrayid, v.nocase "
			"from %svars v where v.%s = %d ",
			obj.table(), obj.fk(), obj.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	int deferred_arrayid[1000];
	DMArray *deferred_array[1000];
	int deferred_arrays = 0;

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		if(NULL_IND(arrayid, 0) > 0) {
			// Create the new array, but defer the filling till later
			DMArray *ht = vars.newArray(varName, false,
				BOOL_NULL_IND(nocase, false));
			deferred_arrayid[deferred_arrays] = NULL_IND(arrayid, 0);
			deferred_array[deferred_arrays] = ht;
			deferred_arrays++;
		} else {
			char *value = NULL_IND(varValue, NULL);
			bool nc = BOOL_NULL_IND(nocase, false);
			if (!nc && value && stringIsNumber(value)) {
				vars.set(varName, atoi(value));
			} else {
				vars.set(varName, value, nc);
			}
		}
	}

	sql->CloseSQL();

	// Now run queries for each array after we have finished with the variables query
	for(int a = 0; a < deferred_arrays; a++) {
		char arrName[DB_ARRAYKEY_LEN];
		char arrValue[DB_VARVAL_LEN];
		SQLLEN ni_arrValue = 0;

		sql->BindColumn(1, SQL_CHAR, arrName, sizeof(varName));
		sql->BindColumn(2, SQL_CHAR, arrValue, sizeof(arrValue), &ni_arrValue);

		res = sql->ExecuteSQL("select a.name, a.value "
				"from dm_arrayvalues a where a.id = %d ",
				deferred_arrayid[a]);
		if(IS_SQL_SUCCESS(res)) {
			for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
				deferred_array[a]->put(arrName, new Variable(NULL, NULL_IND(arrValue, NULL)));
			}
		}

		sql->CloseSQL();
	}
}

void Model::setVariableForObject(Object *obj,const char *attname,Expr *attval)
{
	// Probably need to check access rights to object
	if (obj->hasAccess(NULL)) {
		AutoPtr<triSQL> sql = m_odbc.GetSQL();
		int res = sql->ExecuteSQL("UPDATE %svars SET value='%s' WHERE name='%s' AND %s=%d",
			obj->table(),
			attval->toString(),
			attname,
			obj->fk(),
			obj->id());
		if (sql->GetRowCount() == 0) {
			// Update failed - do insert
			res = sql->ExecuteSQL("INSERT INTO %svars(%s,name,value) VALUES(%d,'%s','%s')",
				obj->table(),
				obj->fk(),
				obj->id(),
				attname,
				attval->toString());
		}
		sql->CloseSQL();
	} else {
		throw RuntimeError("No \"Change\" access to \"%s\"",obj->name());
	}
}

List <Component> *Model::getComponentsOnServer(Server *server)
{
	List<Component> *ret = new List<Component>(true);
	int compid;

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	sql->BindColumn(1, SQL_INTEGER, &compid, sizeof(compid));
	SQLRETURN res = sql->ExecuteSQL("select compid from dm.dm_compsonserv where serverid=%d",server->id());
	if(IS_SQL_SUCCESS(res)) {
		for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
			Component *comp = getComponentById(compid);
			ret->add(comp);
		}
	}
	sql->CloseSQL();
	return ret;
}

void Model::addAccessForDomain(Domain &dom, HashtableById<ObjectAccess> &ia)
{
	//debug1("addAccessForDomain('%s')", dom.name());

	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int usrgrpid = 0;
	char readAccess[DB_BOOL_LEN];
	char writeAccess[DB_BOOL_LEN];
	SQLLEN ni_readAccess = 0, ni_writeAccess = 0;

	sql->BindColumn(1, SQL_INTEGER, &usrgrpid, sizeof(usrgrpid));
	sql->BindColumn(2, SQL_CHAR, readAccess, sizeof(readAccess), &ni_readAccess);
	sql->BindColumn(3, SQL_CHAR, writeAccess, sizeof(writeAccess), &ni_writeAccess);

	int res = sql->ExecuteSQL("select a.usrgrpid, a.readaccess, a.writeaccess "
			"from dm_domaininherit a where a.domainid = %d ", dom.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		ObjectAccess *oa = ia.get(usrgrpid);
		if(!oa) {
			oa = new ObjectAccess();
			ia.put(usrgrpid, oa);
		}
		//debug1("Domain '%s' adds access %d, %s, %s", dom.name(), usrgrpid,
		//	NULL_IND(readAccess, "(null)"), NULL_IND(writeAccess, "(null)"));
		oa->addDomainAccess(NULL_IND(readAccess, NULL), NULL_IND(writeAccess, NULL));
	}

	Domain *pdom = dom.getDomain();
	if(pdom) {
		addAccessForDomain(*pdom, ia);
	}
}


HashtableById<ObjectAccess> *Model::getAccessForObject(Object &obj)
{
	//debug1("getAccessForObject('%s')", obj.name());

	HashtableById<ObjectAccess> *ret = new HashtableById<ObjectAccess>(true);
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int usrgrpid = 0;
	char readAccess[DB_BOOL_LEN];
	char writeAccess[DB_BOOL_LEN];
	char viewAccess[DB_BOOL_LEN];
	char updateAccess[DB_BOOL_LEN];
	SQLLEN ni_readAccess = 0, ni_writeAccess = 0, ni_viewAccess = 0, ni_updateAccess = 0;

	sql->BindColumn(1, SQL_INTEGER, &usrgrpid, sizeof(usrgrpid));
	sql->BindColumn(2, SQL_CHAR, readAccess, sizeof(readAccess), &ni_readAccess);
	sql->BindColumn(3, SQL_CHAR, writeAccess, sizeof(writeAccess), &ni_writeAccess);
	sql->BindColumn(4, SQL_CHAR, viewAccess, sizeof(viewAccess), &ni_viewAccess);
	sql->BindColumn(5, SQL_CHAR, updateAccess, sizeof(updateAccess), &ni_updateAccess);

	int res = sql->ExecuteSQL("select a.usrgrpid, a.readaccess, a.writeaccess, a.viewaccess, a.updateaccess "
			"from %saccess a where a.%s = %d ", obj.table(), obj.fk(), obj.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		debug3("SQL failed");
		return ret;
	}

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		ObjectAccess *oa = new ObjectAccess();
		oa->addObjectAccess(NULL_IND(readAccess, NULL), NULL_IND(writeAccess, NULL));
		ret->put(usrgrpid, oa);
	}

	Domain *dom = obj.getDomain();
	if(dom) {
		addAccessForDomain(*dom, *ret);
	}

	return ret;
}

		
void Model::getDomainForObject(Object &obj)
{
	char fromClause[256], whereClause[256];
	sprintf(fromClause, "%s t", obj.table());
	sprintf(whereClause, "t.id = %d and d.id = t.domainid", obj.id());
	AutoPtr<List<Domain> > matches = internalGetDomains(fromClause, whereClause);
	if(matches && (matches->size() > 0)) {
		ListIterator<Domain> iter(*matches);
		obj.setDomain(iter.first());
		// Note that we don't add an incomplete list of children
		// they will be read from the db only if requested
	}
}


void Model::getSummaryForObject(Object &obj)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char summary[DB_SUMMARY_LEN];
	SQLLEN ni_summary = 0;
	sql->BindColumn(1, SQL_CHAR, summary, sizeof(summary), &ni_summary);

	int res = sql->ExecuteSQL("select t.summary "
			"from %s t where t.id = %d ",
			obj.table(), obj.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		obj.setSummary(NULL_IND(summary, ""));
	}
}


void Model::getNotesForObject(Object &obj)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char notes[DB_NOTES_LEN];
	SQLLEN ni_notes = 0;
	sql->BindColumn(1, SQL_CHAR, notes, sizeof(notes), &ni_notes);

	int res = sql->ExecuteSQL("select t.notes "
			"from %s t where t.id = %d ",
			obj.table(), obj.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		obj.setNotes(NULL_IND(notes, ""));
	}
}


void Model::getCredentialsForProviderObject(ProviderObject &po)
{
	char whereClause[256];
	sprintf(whereClause, "t.id = %d", po.id());

	Credentials *creds = internalGetCredentials(po.table(), whereClause);
	po.setCredentials(creds);
}


void Model::getPropertiesForProviderObject(ProviderObject &obj)
{
	AutoPtr<List<Property> > matches = internalGetProperties(
		obj.table(), obj.fk(), obj.id());
	if(matches) {
		ListIterator<Property> iter(*matches);
		for(Property *prop = iter.first(); prop; prop = iter.next()) {
			obj.setProperty(prop);
		}
	}
}


List<RepositoryIgnorePattern> *Model::getIgnorePatternsForRepositoryDef(ProviderObjectDef &def)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char pattern[257];
	char isdir[DB_BOOL_LEN];

	sql->BindColumn(1, SQL_CHAR, pattern, sizeof(pattern));
	sql->BindColumn(2, SQL_CHAR, isdir, sizeof(isdir));

	int res = sql->ExecuteSQL("select i.pattern, i.isdir "
			"from dm_repositoryignore i where i.defid = %d ", def.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	List<RepositoryIgnorePattern> *ret = new List<RepositoryIgnorePattern>(true);

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		ret->add(new RepositoryIgnorePattern(pattern,
			((isdir[0] == 'Y') ? true : false)));
	}

	return ret;
}


ProviderObjectDef *Model::getDefForProviderObject(ProviderObject &obj)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int id = 0;
	char defName[DB_NAME_LEN];
	int pluginId = 0;
	int pluginVer = 0;
	char pluginLib[257];
	SQLLEN ni_pluginId = 0, ni_pluginVer = 0, ni_pluginLib = 0;

	sql->BindColumn(1, SQL_INTEGER, &id, sizeof(id));
	sql->BindColumn(2, SQL_CHAR, defName, sizeof(defName));
	sql->BindColumn(3, SQL_INTEGER, &pluginId, sizeof(pluginId), &ni_pluginId);
	sql->BindColumn(4, SQL_INTEGER, &pluginVer, sizeof(pluginVer), &ni_pluginVer);
	sql->BindColumn(5, SQL_CHAR, pluginLib, sizeof(pluginLib), &ni_pluginLib);

	int res = sql->ExecuteSQL("select d.id, d.name, d.pluginid, p.version, p.library "
			"from %s o, (dm_providerdef d left join dm_plugin p on p.id = d.pluginid) "
			"where o.id = %d and d.id = o.defid ",
			obj.table(), obj.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		Plugin *plugin = NULL;
		if(NULL_IND(pluginId, 0)) {
			plugin = findOrCreatePlugin(pluginId, NULL_IND(pluginVer, 0), NULL_IND(pluginLib, NULL));
		}
		return findOrCreateProviderObjectDef(id, defName, obj.def_kind(), plugin);
	}

	debug1("No def for %s with id %d found - have you set it up correctly?", obj.def_kind(), obj.id());
	return NULL;
}


void Model::getPropertyDefsForProviderObjectDef(ProviderObjectDef &pod)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char pdName[DB_NAME_LEN];
	char pdReqd[DB_BOOL_LEN];
	char pdApnd[DB_BOOL_LEN];

	sql->BindColumn(1, SQL_CHAR, pdName, sizeof(pdName));
	sql->BindColumn(2, SQL_CHAR, pdReqd, sizeof(pdReqd));
	sql->BindColumn(3, SQL_CHAR, pdApnd, sizeof(pdApnd));

	int res = sql->ExecuteSQL("select pd.name, pd.required, pd.appendable "
			"from dm_propertydef pd where pd.defid = %d ", pod.id());
	if(IS_NOT_SQL_SUCCESS(res)) {
		return;
	}

	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		pod.add(new PropertyDef(pdName,
			((pdReqd[0] == 'Y') ? true : false),
			((pdApnd[0] == 'Y') ? true : false)));
	}
}


void Model::getPropertiesForProviderObjectDef(ProviderObjectDef &def)
{
	AutoPtr<List<Property> > matches = internalGetProperties(
		"dm_providerdef", "defid", def.id(), "engineid", m_engineId);
	if(matches) {
		ListIterator<Property> iter(*matches);
		for(Property *prop = iter.first(); prop; prop = iter.next()) {
			def.setDefProperty(prop);
		}
	}
}


List<RepositoryTextPattern> *Model::getTextPatternsForRepositoryPath(
		Repository &repo, const char *path)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char rtpPath[DB_PATH_LEN];
	char rtpPattern[257];
	char rtpText[DB_BOOL_LEN];

	sql->BindColumn(1, SQL_CHAR, rtpPath, sizeof(rtpPath));
	sql->BindColumn(2, SQL_CHAR, rtpPattern, sizeof(rtpPattern));
	sql->BindColumn(3, SQL_CHAR, rtpText, sizeof(rtpText));

	int res = sql->ExecuteSQL("select rtp.path, rtp.pattern, rtp.istext "
			"from dm_repositorytextpattern rtp "
			"where rtp.repositoryid = %d and (rtp.path = '%s' or rtp.path = '/%s')", repo.id(), path, path);
	if(IS_NOT_SQL_SUCCESS(res))
	{
		return NULL;
	}

	List<RepositoryTextPattern> *ret = new List<RepositoryTextPattern>(true);
	for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
		ret->add(new RepositoryTextPattern(rtpPath, rtpPattern,
			((rtpText[0] == 'Y') ? true : false)));
	}
	if (ret->size() == 0) {
		// Nothing found - not sure if we need to try anything else
	}

	debug3("returning ret->size=%d",ret->size());
	return ret;
}


char *Model::getNotifyText(int textid)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char ntName[DB_NAME_LEN];
	sql->BindColumn(1, SQL_CHAR, ntName, sizeof(ntName));

	int res = sql->ExecuteSQL(
		"select nt.name, nt.data from dm_notifytext nt where nt.id = %d ", textid);
	if(IS_NOT_SQL_SUCCESS(res)) {
		return NULL;
	}

	char *ret = NULL;
	res = sql->FetchRow();
	if(IS_SQL_SUCCESS(res)) {
		//debug1("Template name: %s", ntName);

		char *data = NULL;
		SQLLEN ni_data = 0;

		res = sql->GetData(2, SQL_C_BINARY, &data, 0, &ni_data);
		if(IS_SQL_SUCCESS(res)) {
			//debug1("Text size: %ld", ni_data);

			// Get all the data at once.
			data = (char*) malloc(ni_data);
			res = sql->GetData(2, SQL_C_DEFAULT, data, ni_data, &ni_data);
			if(IS_SQL_SUCCESS(res)) {
				ret = data;
				//debug1("Text is: %s", data);
			}
		}
	}

	sql->CloseSQL();
	return ret;
}


void Model::getActionForField(Field &field)
{
	char whereClause[1024];
	sprintf(whereClause, "f.id = %d and a.id = f.actionid", field.id());

	Action *action = internalGetAction("dm_field f", whereClause, false);
	field.setAction(action);
}


void Model::setPluginObject(const char *name, PluginObject *obj)
{
	m_pluginObjects.put(name, obj);
}


PluginObject *Model::getPluginObject(const char *name)
{
	return m_pluginObjects.get(name);
}


bool Model::checkPropertyDefs(const char *name, List<class PropertyDef> *propdefs, int pdId)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	char propName[DB_NAME_LEN];
	char propReqd[DB_BOOL_LEN];
	/*bool*/ int propApnd = 0 /*false*/;
	sql->BindColumn(1, SQL_CHAR, propName, sizeof(propName));
	sql->BindColumn(2, SQL_CHAR, propReqd, sizeof(propReqd));
	sql->BindColumn(3, SQL_INTEGER, &propApnd, sizeof(propApnd));

	int res = sql->ExecuteSQL("select p.name, p.required, p.appendable from dm_propertydef p where p.defid = %d", pdId);
	if(IS_SQL_SUCCESS(res)) {
		List<PropertyDef> foundProps;
		for(res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
			ListIterator<PropertyDef> iter(*propdefs);
			bool found = false;
			for(PropertyDef *def = iter.first(); def; def = iter.next()) {
				if(strcmp(propName, def->name()) == 0) {
					if(((propReqd[0] == 'Y') ? true : false) != def->required()) {
						printf("ERROR: Property '%s' of statement '%s' is already registered with a different required setting\n", propName, name);
						return false;
					}
					//if((propApnd ? true : false) != def->appendable()) {
					//	printf("ERROR: Property '%s' of statement '%s' is already registered with a different appendable setting\n", propName, name);
					//	return false;
					//}
					found = true;
					foundProps.add(def);
					break;
				}
			}

			if(!found) {
				printf("ERROR: Statement '%s' is already registered, but property '%s' is unknown\n", name, propName);
				return false;
			}
		}

		// Now look for missing propertydefs
		if(foundProps.size() != propdefs->size()) {
			debug2("Statement '%s' is already registered, but property counts differ", name);
			ListIterator<PropertyDef> pit(*propdefs);
			for(PropertyDef *pdef = pit.first(); pdef; pdef = pit.next()) {
				ListIterator<PropertyDef> fit(foundProps);
				bool found = false;
				for(PropertyDef *fdef = fit.first(); fdef; fdef = fit.next()) {
					if(fdef == pdef) {
						found = true;
						break;
					}
				}
				if(!found) {
					printf("ERROR: Statement '%s' is already registered, but property '%s' is missing\n", name, pdef->name());
				}
			}
			return false;
		}
	}

	return true;
}

void Model::addProvider(int kind,const char *name,List<class PropertyDef> *propdefs,int pluginid /* = 0 */)
{
	int pid;
	AutoPtr<triSQL> sql3 = m_odbc.GetSQL();
	sql3->BindColumn(1,SQL_INTEGER,&pid,sizeof(pid));
	int res = sql3->ExecuteSQL("SELECT MAX(id)+1 FROM dm_providerdef");
	if (res == SQL_SUCCESS) {
		sql3->FetchRow();
	}
	sql3->CloseSQL();
	char *kindstr = "";
	switch(kind) {
		case OBJ_KIND_REPOSITORY:
			kindstr = "repository";
	}
	int kindstrlen = strlen(kindstr);
	int namelen = strlen(name);
	// id integer NOT NULL,
	// name character varying(256),
	// kind integer,
	// kindstr character varying(256),
	// pluginid integer
	AutoPtr<triSQL> sql4 = m_odbc.GetSQL();
	sql4->BindParameter(1, SQL_INTEGER, sizeof(pid),  &pid,         sizeof(pid));
	sql4->BindParameter(2, SQL_CHAR,    namelen,      (char *)name, namelen);
	sql4->BindParameter(3, SQL_INTEGER, sizeof(kind), &kind,        sizeof(kind));   
	sql4->BindParameter(4, SQL_CHAR,    kindstrlen,   kindstr,      kindstrlen);
	if (pluginid > 0) {
		sql4->BindParameter(5, SQL_INTEGER, sizeof(pluginid), &pluginid, sizeof(pluginid));
		res = sql4->ExecuteSQL("INSERT INTO dm_providerdef(id,name,kind,kindstr,pluginid) VALUES(?,?,?,?,?)");
	} else {
		res = sql4->ExecuteSQL("INSERT INTO dm_providerdef(id,name,kind,kindstr) VALUES(?,?,?,?)");
	}
	if (res == SQL_SUCCESS) {
		//
		// Now insert each supplied propertydef
		//
		// defid integer NOT NULL,
		// name character varying(256) NOT NULL,
		// required character(1),
		// appendable character(1),
		AutoPtr<triSQL> sql5 = m_odbc.GetSQL();
		ListIterator<PropertyDef> iter(*propdefs);
		for(PropertyDef *def = iter.first(); def; def = iter.next()) {
			char *pname = (char *)def->name();
			char *required = (char *)(def->required()?"Y":"N");
			char *appendable = (char *)(def->appendable()?"Y":"N");
			int rlen=1;
			int alen=1;
			int pnamelen = strlen(pname);
			sql5->BindParameter(1, SQL_INTEGER, sizeof(pid),  &pid,         sizeof(pid));
			sql5->BindParameter(2, SQL_CHAR,    pnamelen,     pname,        pnamelen);
			sql5->BindParameter(3, SQL_CHAR,    rlen,         required,     rlen);
			sql5->BindParameter(4, SQL_CHAR,    alen,         appendable,   alen);
			sql5->ExecuteSQL("INSERT INTO dm_propertydef(defid,name,required,appendable) VALUES(?,?,?,?)");
		}
		sql5->CloseSQL();
	}
	sql4->CloseSQL();
}

bool Model::installProviderImpl(
	const char *name, const char *plugin,
	int kind, List<class PropertyDef> *propdefs)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	int res;
	if (STRCASECMP(m_odbc.GetDatabaseVendor(),"oracle")!=0) {
		// We don't set schema for Oracle instances
		res = sql->ExecuteSQL("set schema 'dm'");
		if(IS_NOT_SQL_SUCCESS(res)) {
			return 1;
		}
	}

	// Check to see if we have a plugin of this name, and if so, check the definition is the same
	if (plugin) {
		int pluginId = 0;
		sql->BindColumn(1, SQL_INTEGER, &pluginId, sizeof(pluginId));
		res = sql->ExecuteSQL("select p.id from dm_plugin p where p.library = '%s'", plugin);
		if (!(IS_SQL_SUCCESS(res))) return false;
		res = sql->FetchRow();
		if (IS_SQL_SUCCESS(res)) {
			// Found it, now check the definition is the same
			int pdId = 0;
			char pdName[DB_NAME_LEN];
			int pdKind = 0;
			sql->BindColumn(1, SQL_INTEGER, &pdId, sizeof(pdId));
			sql->BindColumn(2, SQL_CHAR, pdName, sizeof(pdName));
			sql->BindColumn(3, SQL_INTEGER, &pdKind, sizeof(pdKind));
			res = sql->ExecuteSQL("select d.id, d.name, d.kind from dm_providerdef d where d.pluginid = %d", pluginId);
			if (!(IS_SQL_SUCCESS(res))) return false;
			bool found = false;
			for (res = sql->FetchRow(); IS_SQL_SUCCESS(res); res = sql->FetchRow()) {
				if(STRCASECMP(pdName, name) == 0) {
					if(pdKind == kind) {
						// Statement of same kind found, check properties
						if(!checkPropertyDefs(name, propdefs, pdId)) {
							return false;
						}
					} else {
						printf("ERROR: Statement '%s' of plugin '%s' is already registered with a different kind\n", name, plugin);
						return false;
					}
					found = true;
					break;
				}
			}
			if(!found) {
				printf("ERROR: Plugin '%s' is already registered, but statement '%s' is missing\n", plugin, name);
				return false;
			}

			printf("Plugin '%s' is already registered, definition is the same\n", plugin);
			return true;
		} else {
			int id;
			int version = 10000;

			AutoPtr<triSQL> sql2 = m_odbc.GetSQL();
			sql2->BindColumn(1,SQL_INTEGER,&id,sizeof(id));
			res = sql2->ExecuteSQL("SELECT MAX(id)+1 FROM dm_plugin");
			if (res == SQL_SUCCESS) {
				sql2->FetchRow();
			}
			sql2->CloseSQL();

			sql2->BindParameter(1, SQL_INTEGER, sizeof(id), &id, sizeof(id));
			sql2->BindParameter(2, SQL_INTEGER, sizeof(version), &version, sizeof(version));
			int pluginlen = strlen(plugin);
			sql2->BindParameter(3, SQL_CHAR, pluginlen, (char*) plugin, pluginlen);
			res = sql2->ExecuteSQL("INSERT INTO dm_plugin(id,version,library) VALUES (?,?,?)");
			if (res == SQL_SUCCESS) {
				// plugin inserted - now add the propertydefs entry
				addProvider(kind,name,propdefs,id);
			}
			sql2->CloseSQL();
		}
		sql->CloseSQL();
	}

	// No plugin, check for statement with this name
	int pdId = 0;
	int pdKind = 0;
	sql->BindColumn(1, SQL_INTEGER, &pdId, sizeof(pdId));
	sql->BindColumn(2, SQL_INTEGER, &pdKind, sizeof(pdKind));
	res = sql->ExecuteSQL("select d.id, d.kind from dm_providerdef d where d.name = '%s'", name);
	if(IS_SQL_SUCCESS(res)) {
		res = sql->FetchRow();
		if(IS_SQL_SUCCESS(res)) {
			sql->CloseSQL();

			debug2("Statement '%s' is already registered", name);
			if(pdKind == kind) {
				// Statement of same kind found, check properties
				if(!checkPropertyDefs(name, propdefs, pdId)) {
					return false;
				}
			} else {
				printf("ERROR: Statement '%s' is already registered with a different kind\n", name);
				return false;
			}

			printf("Statement '%s' is already registered, definition is the same\n", name);
			return true;
		} else {
			printf("Statement is not registered - creating\n");
			addProvider(kind,name,propdefs);
		}
	}

	return false;
}
