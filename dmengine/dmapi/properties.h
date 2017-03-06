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
#ifndef __properties_h

#define __properties_h


#include "list.h"
#include "hashtable.h"
#include "dmapiexport.h"


/**
 * Represents an instance of a property, which is a name/value pair, with the
 * value optionally encrypted.  The administrator can set the property as
 * either overridable or appendable to indicate whether the user can override
 * or append to the value.  All properties can be overridden, but not all
 * properties understand being appended, so only those properties that define
 * themselves to be appendable in their PropertyDef will make use of this flag.
 */
class DMAPI_API Property
{
private:
	char *m_name;
	char *m_value;
	bool m_encrypted;
	bool m_overridable;
	bool m_appendable;

public:
	Property(const char *name, const char *value, bool encrypted,
		bool overridable, bool appendable);
	~Property();

	const char *name()  { return m_name; }
	const char *value();

	bool overridable()	{ return m_overridable; }
	bool appendable()	{ return m_appendable; }

	char *decryptedValue(class Object &auth);

	friend class Credentials;
};


/**
 * Represents the definition of a property.  All properties have a name and can
 * be set as required or appendable.  Note that it is up to the defining plugin
 * to set up this data based upon what it assumes in its implementation.  This
 * data is inserted and checked with the -install option.
 */
class DMAPI_API PropertyDef
{
private:
	char *m_name;
	bool m_required;
	bool m_appendable;

public:
	PropertyDef(
		const char *name, bool required, bool appendable);
	~PropertyDef();

	const char *name()  { return m_name; }
	bool required()		{ return m_required; }
	bool appendable()	{ return m_appendable; }

	friend class PropertyChecker;
};


/**
 * This is a utility class to aid checking that properties and supplied
 * arguments confirm to their required and overridable rules as set above.
 */
class DMAPI_API PropertyChecker
{
private:
	List<PropertyDef> *m_propdefs;
	char *m_msg;

public:
	PropertyChecker(List<PropertyDef> *propdefs, const char *msg);
	~PropertyChecker();

	void checkProps(class Context &ctx, Hashtable<Property> *props);
	void checkArgs(class Context &ctx, class StmtList *args, Hashtable<Property> *props);
};


#endif /*__properties_h*/
