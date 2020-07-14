#include <stdio.h>

#include "properties.h"

#include "hashtable.h"
#include "exceptions.h"
#include "node.h"
#include "model.h"
#include "context.h"
#include "crypto.h"


///////////////////////////////////////////////////////////////////////////////
// Property
///////////////////////////////////////////////////////////////////////////////

Property::Property(
		const char *name, const char *value, bool encrypted,
		bool overridable, bool appendable
	)
	: m_name(DUP_NULL(name)), m_value(DUP_NULL(value)), m_encrypted(encrypted),
	  m_overridable(overridable), m_appendable(appendable)
{}


Property::~Property()
{
	SAFE_FREE(m_name);
	SAFE_FREE(m_value);
}


/**
 * This purposfully does not output encrypted values - you must explicitly call
 * decryptedValue() with an object to use for authorization in order to see the
 * value.  This method is used in print() and logging to hide sensitive
 * information from prying eyes.
 */
const char *Property::value()
{
	return m_encrypted ? "********" : m_value;
}


/**
 * Authorization is required in order to decrypt values - if the current user
 * does not have access to the object, the values cannot be decrypted.  The
 * authorization object is typically some kind of provider and we are
 * retrieving its properties.
 */
char *Property::decryptedValue(Object &auth)
{
	if(!m_encrypted) {
		return strdup(m_value);
	}

	if(!auth.hasAccess(NULL)) {
		throw PermissionDeniedException("You do not have permisson to decrypt the property '%s'", m_name);
	}

	// TODO: free the buffer this creates
	return decryptValue(m_value, strlen(m_value));
}


///////////////////////////////////////////////////////////////////////////////
// PropertyDef
///////////////////////////////////////////////////////////////////////////////

PropertyDef::PropertyDef(const char *name, bool required, bool appendable)
	: m_name(DUP_NULL(name)), m_required(required), m_appendable(appendable)
{}


PropertyDef::~PropertyDef()
{
	SAFE_FREE(m_name);
}


///////////////////////////////////////////////////////////////////////////////
// PropertyChecker
///////////////////////////////////////////////////////////////////////////////

PropertyChecker::PropertyChecker(List<PropertyDef> *propdefs, const char *msg)
	: m_propdefs(propdefs), m_msg(DUP_NULL(msg))
{}


PropertyChecker::~PropertyChecker()
{
	// TEMP REMOVED: SAFE_DELETE(m_propdefs);
	SAFE_FREE(m_msg);
}


void PropertyChecker::checkProps(Context &ctx, Hashtable<Property> *props)
{
	// First pass - check that each specified property is allowed
	if(props) {
		StringList *keys = props->keys();
		StringListIterator iter(*keys);
		for(const char *key = iter.first(); key; key = iter.next()) {
			bool found = false;
			ListIterator<PropertyDef> itdefs(*m_propdefs);
			for(PropertyDef *def = itdefs.first(); def; def = itdefs.next()) {
				if(strcmp(def->name(), key) == 0) {
					// Found - check the def to make sure appendable ties up with definition
					Property *prop = props->get(key);
					if(prop && prop->appendable() && !def->appendable()) {
						ctx.writeToStdErr("WARNING: Property '%s' for %s should not be specified as appendable in database",
							key, (m_msg ? m_msg : "(null"));
					}
					found = true;
					break;
				}
			}
			if(!found) {
				ctx.writeToStdErr("WARNING: Unrecognised property '%s' for %s specified in database",
					key, (m_msg ? m_msg : "(null"));
			}
		}
		delete keys;
	}

	// Second pass - check that all mandatory properties are present
	ListIterator<PropertyDef> itdefs(*m_propdefs);
	for(PropertyDef *def = itdefs.first(); def; def = itdefs.next()) {
		if(def->required() && (!props || !props->get(def->m_name))) {
			throw RuntimeError("Mandatory property '%s' for %s not specified in database",
				def->name(), (m_msg ? m_msg : "(null"));
		}
	}
}


void PropertyChecker::checkArgs(Context &ctx, StmtList *args, Hashtable<Property> *props)
{
	// Check for args which attempt to override non-overrideable properties
	if(args && m_propdefs && props) {
		ListIterator<Stmt> iter(*args);
		for(Stmt *arg = iter.first(); arg; arg = iter.next()) {
			if(arg->kind() == STMT_ARG) {
				const char *name = arg->name();
				Property *prop = props->get(name);
				if(prop && !prop->overridable()) {
					ListIterator<PropertyDef> itdefs(*m_propdefs);
					bool isAppendable = false;
					for(PropertyDef *def = itdefs.first(); def; def = itdefs.next()) {
						if(strcmp(def->m_name, name) == 0) {
							isAppendable = def->appendable();
							break;
						}
					}
					
					if(!isAppendable) {
						// Property is never appendable, so report on override only
						if(arg->lineno()) {
							ctx.writeToStdOut("WARNING: Property '%s' is non-overridable for %s at line %d", name, m_msg, arg->lineno());
						} else {
							ctx.writeToStdOut("WARNING: Property '%s' is non-overridable for %s", name, m_msg);
						}
					} else if(!prop->appendable()) {
						// Property can be appendable, so but flag on property says no
						if(arg->lineno()) {
							ctx.writeToStdOut("WARNING: Property '%s' is neither overridable nor appendable for %s at line %d", name, m_msg, arg->lineno());
						} else {
							ctx.writeToStdOut("WARNING: Property '%s' is neither overridable nor appendable for %s", name, m_msg);
						}
					}
				}
			}
		}
	}
}
