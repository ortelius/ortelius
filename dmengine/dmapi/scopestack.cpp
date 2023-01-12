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

#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <malloc.h>
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

#include "scopestack.h"
#include "dm.h"
#include "modelbase.h"
#include "exceptions.h"
#include "platform.h"
#include "node.h"				// for stacktrace
#include "filenamecache.h"		// for stacktrace
#include "expr.h"				// for objectreference
#include "context.h"			// for stacktrace
#include "datetime.h"


// Stack size params - start with 10 and grow up to 50
#define SCOPE_STACK_MIN_SIZE 10
#define SCOPE_STACK_MAX_SIZE 50
#define SCOPE_STACK_INCREMENT 10

/*
typedef struct varset_tag{
char			*Name;
char			*Value;
struct varset_tag	*next;
} VARSET;
*/

Variable::Variable(const char *name, bool value)
	: m_name(DUP_NULL(name)), m_type(vartype_bool),
	  m_ival(value?true:false)
{}

Variable::Variable(const char *name, int value)
	: m_name(DUP_NULL(name)), m_type(vartype_integer),
	  m_ival(value)
{}


Variable::Variable(const char *name, const char *value, bool nocase /*= false*/)
	: m_name(DUP_NULL(name)), m_type(vartype_string), m_nocase(nocase),
	  m_value(DUP_NULL(value))
{}


Variable::Variable(const char *name, DMArray *arr)
	: m_name(DUP_NULL(name)), m_type(vartype_array), m_nocase(false),
	  m_array(arr)
{
	// Increment reference count on the array
	if(arr) {
		arr->addRef();
	}
}


Variable::Variable(const char *name, class IObject *obj)
	: m_name(DUP_NULL(name)), m_type(vartype_object), m_nocase(false),
	  m_objref(obj ? new ObjectReference(obj) : NULL)
{}


Variable::Variable(const char *name, class DMException &ex)
	: m_name(DUP_NULL(name)), m_type(vartype_object), m_nocase(false),
	  m_objref(new ObjectReference(ex))
{}


Variable::Variable(const char *name, class ObjectReference *objref)
	: m_name(DUP_NULL(name)), m_type(vartype_object), m_nocase(false),
	  m_objref(objref ? new ObjectReference(*objref) : NULL)
{}


Variable::Variable(const char *name, class OutputStream *stream)
	: m_name(DUP_NULL(name)), m_type(vartype_stream), m_nocase(false),
	  m_stream(stream)
{
	// Increment reference count on the stream
	if(stream) {
		stream->addRef();
	}
}


Variable::Variable(const char *name, class DateTime *date)
	: m_name(DUP_NULL(name)), m_type(vartype_date), m_nocase(false),
	  m_date(date ? new DateTime(*date) : NULL)
{}


Variable::Variable(const char *name, class Expr *e)
	: m_name(DUP_NULL(name)), m_nocase(false)
{
	if (e) {
		switch(e->kind()) {
		case KIND_BOOL:
			m_type = vartype_bool;
			m_ival = e->toInt();
			break;
		case KIND_INT:
			m_type = vartype_integer;
			m_ival = e->toInt();
			break;
		case KIND_STR:
			m_type = vartype_string;
			m_value = e->toString();
			break;
		case KIND_ARRAY:
			m_type = vartype_array;
			m_array = e->toArray();
			if(m_array) {
				m_array->addRef();
			}
			break;
		case KIND_OBJECT: {
			m_type = vartype_object;
			ObjectReference *objref = e->toObjectReference();
			m_objref = objref ? new ObjectReference(*objref) : NULL;
			}
			break;
		case KIND_STREAM:
			m_type = vartype_stream;
			m_stream = e->toStream();
			if(m_stream) {
				m_stream->addRef();
			}
			break;
		case KIND_DATE: {
			m_type = vartype_date;
			DateTime *date = e->toDate();
			m_date = date ? new DateTime(*date) : NULL;
			}
			break;
		}
	} else {
		debug1("Trying to create variable '%s' with null expr", NULL_CHECK(name));
		m_type = vartype_string;
		m_value = strdup("");
	}
}


Variable::Variable(const Variable& copy)
	: m_name(DUP_NULL(copy.m_name)), m_type(copy.m_type), m_nocase(copy.m_nocase)
{
	switch(m_type) {
	case vartype_bool:
	case vartype_integer:
		m_ival = copy.m_ival;
		break;
	case vartype_string:
		m_value = DUP_NULL(copy.m_value);
		break;
	case vartype_array:
		m_array = copy.m_array;
		if(m_array) { m_array->addRef(); }
		break;
	case vartype_object:
		m_objref = copy.m_objref ? new ObjectReference(*copy.m_objref) : NULL;
		break;
	case vartype_stream:
		m_stream = copy.m_stream;
		if(m_stream) { m_stream->addRef(); }
		break;
	case vartype_date:
		m_date = copy.m_date;
		break;
	}
}


Variable::~Variable()
{
	SAFE_FREE(m_name);
	freeOldValue();
}

void Variable::setValue(bool value)
{
	freeOldValue();
	m_type  = vartype_bool;
	m_ival = value;
}

void Variable::setValue(int value)
{
	freeOldValue();
	m_type  = vartype_integer;
	m_ival = value;
}


void Variable::setValue(const char *value)
{
	freeOldValue();
	m_type  = vartype_string;
	m_value = strdup(value);
}


void Variable::setArray(DMArray *arr)
{
	freeOldValue();
	m_type  = vartype_array;
	m_array = arr;

	// Increment reference count on the array
	if(arr) {
		arr->addRef();
	}
}


void Variable::setObjectReference(class IObject *obj)
{
	freeOldValue();
	m_type    = vartype_object;
	m_objref  = obj ? new ObjectReference(obj) : NULL;
}


void Variable::setObjectReference(class DMException &ex)
{
	freeOldValue();
	m_type    = vartype_object;
	m_objref  = new ObjectReference(ex);
}

void Variable::setObjectReference(class ObjectReference *objref)
{
	freeOldValue();
	m_type    = vartype_object;
	m_objref  = objref ? new ObjectReference(*objref) : NULL;
}


void Variable::setStream(OutputStream *stream)
{
	freeOldValue();
	m_type  = vartype_stream;
	m_stream = stream;

	// Increment reference count on the stream
	if(stream) {
		stream->addRef();
	}
}


void Variable::setDate(class DateTime *date)
{
	freeOldValue();
	m_type = vartype_date;
	m_date = date ? new DateTime(*date) : NULL;
}


int Variable::getInt()
{
	if(m_type == vartype_integer || m_type == vartype_bool) {
		return m_ival;
	}
	return 0;
}


const char *Variable::getString()
{
	if(m_type == vartype_string) {
		return m_value;
	}
	return NULL;
}


DMArray *Variable::getArray()
{
	if(m_type == vartype_array) {
		return m_array;
	}
	return NULL;
}


class ObjectReference *Variable::getObjectReference()
{
	if(m_type == vartype_object) {
		return m_objref;
	}
	return NULL;
}


class OutputStream *Variable::getStream()
{
	if(m_type == vartype_stream) {
		return m_stream;
	}
	return NULL;
}


class DateTime *Variable::getDate()
{
	if(m_type == vartype_date) {
		return m_date;
	}
	return 0;
}


char *Variable::toString()
{
	switch(m_type) {
	case vartype_bool:
		{
		char ibuf[32];
		sprintf(ibuf,"%s",m_ival?"true":"false");
		return strdup(ibuf);
		}
	case vartype_integer:
		{
		char ibuf[32];
		sprintf(ibuf,"%d",m_ival);
		return strdup(ibuf);
		}
	case vartype_string:
		return DUP_NULL(m_value);
	default:
		return NULL;
	}
}


char *Variable::flatten()
{
	switch(m_type) {
	case vartype_bool:
		{
		char ibuf[32];
		sprintf(ibuf,"%s",m_ival?"true":"false");
		return strdup(ibuf);
		}
	case vartype_integer:
		{
		char ibuf[32];
		sprintf(ibuf,"%d",m_ival);
		return strdup(ibuf);
		}
	case vartype_string:
		return strdup(m_value ? m_value : "");
	case vartype_array:
		return strdup(m_array->toJSON());
		/*
		{
		Variable **values = m_array->values();
		int len = 0;
		Variable **v;
		for(v = values; v && *v; v++) {
			char *temp = (*v)->flatten();
			len += strlen(temp) + 3;
			free(temp);
		}
		char *buf = (char*) malloc(len + 1);
		if (buf) {
			*buf = '\0';
			for(v = values; v && *v; v++) {
				if(*v) {
					if(*buf) { strcat(buf, " "); }
					strcat(buf, "\"");
					char *temp = (*v)->flatten();
					strcat(buf, temp);
					free(temp);
					strcat(buf, "\"");
				}
			}
		}
		SAFE_FREE(values);
		return buf;
		}
		*/
	case vartype_object:
	case vartype_stream:
	case vartype_date:
		return strdup("");
	}

	return strdup("");
}

void Variable::freeOldValue()
{
	switch(m_type) {
	case vartype_integer:
	case vartype_bool:
		m_ival=0;
		break;
	case vartype_string:
		SAFE_FREE(m_value);
		break;
	case vartype_array:
		if(m_array) {
			m_array->releaseRef();
		}
		m_array = NULL;
		break;
	case vartype_object:
		SAFE_DELETE(m_objref);
		break;
	case vartype_stream:
		if(m_stream) {
			m_stream->releaseRef();
		}
		m_stream = NULL;
		break;
	case vartype_date:
		SAFE_DELETE(m_date);
		m_date = NULL;
		break;
	}
}


//////////////////////////////////////////////////////////////////////////////
// DMArray
//////////////////////////////////////////////////////////////////////////////

DMArray::DMArray(
		bool isList, bool nocase /*= false*/,
		bool isOwner /*= false*/, int buckets /*= 10*/)
	: Hashtable<Variable>(nocase, isOwner, buckets),
	  m_isList(isList), m_refCount(0)
{}


DMArray::~DMArray()
{}


int DMArray::minIndex() const
{
	int minindex = 9999;
	AutoPtr<StringList> keys = this->keys();
	StringListIterator kit(*keys);
	for(const char *k = kit.first(); k; k = kit.next()) {
		int index = atoi(k);
		if(index < minindex) {
			minindex = index;
		}
	}
	return minindex;
}


int DMArray::maxIndex() const
{
	int maxindex = -1;
	AutoPtr<StringList> keys = this->keys();
	StringListIterator kit(*keys);
	for(const char *k = kit.first(); k; k = kit.next()) {
		int index = atoi(k);
		if(index > maxindex) {
			maxindex = index;
		}
	}
	return maxindex;
}


void DMArray::add(Variable *var)
{
	if(!m_isList) {
		throw RuntimeError("Array is not a list");
	}
	char key[32];
	sprintf(key, "%d", maxIndex()+1);
	put(key, var);
}


void DMArray::addAll(const DMArray &arr)
{
	debug3("DMArray::addAll - %s", (m_isList ? "list" : "array"));
	if(m_isList) {
		// List, copy elements renumbering as we go
		if(arr.m_isList) {
			// Source is also a list, renumber starting at max + 1
			int offset = maxIndex()+1;
			int start = arr.minIndex();
			int end = arr.maxIndex();
			debug3("offest = %d; start = %d; end = %d", offset, start, end);
			for(int n = start; n <= end; n++) {
				char okey[32], nkey[32];
				sprintf(okey, "%d", n);
				debug3("Copying key '%s'", okey);
				Variable *var = arr.get(okey);
				if(var) {
					sprintf(nkey, "%d", offset);
					debug3("Copying key '%s' to key '%s'", okey, nkey);
					this->put(nkey, new Variable(*var));
					offset++;
				}
			}
		} else {
			// Source is an array, just add in hash order
			int offset = maxIndex()+1;
			AutoPtr<StringList> keys = arr.keys();
			StringListIterator kit(*keys);
			for(const char *k = kit.first(); k; k = kit.next()) {
				char nkey[32];
				sprintf(nkey, "%d", offset);
				debug3("Copying key '%s' to key '%s'", k, nkey);
				this->put(nkey, new Variable(*(arr.get(k))));
				offset++;
			}
		}
	} else {
		// Array, copy elements using same keys
		AutoPtr<StringList> keys = arr.keys();
		StringListIterator kit(*keys);
		for(const char *k = kit.first(); k; k = kit.next()) {
			this->put(k, new Variable(*(arr.get(k))));
		}
	}
}


void DMArray::addRef()
{
	m_refCount++;
	debug3("addRef() - count now %d", m_refCount);
}


void DMArray::releaseRef()
{
	m_refCount--;
	debug3("releaseRef() - count now %d", m_refCount);
	if(m_refCount == 0) {
		debug3("ref count zero on array - deleting");
		delete this;
	}
}


DMArray *DMArray::operator +(const DMArray &b) const
{
	DMArray *ret = new DMArray(m_isList, m_nocase, m_isOwner, m_bucketCount);
	ret->addAll(*this);
	ret->addAll(b);
	return ret;
}

bool DMArray::isList()
{
	return m_isList;
}

void DMArray::addstr(char **str,char *strtoadd,bool addquote)
{
	int n=0;
	if (addquote) {
		char *p = strtoadd;
		while (*p) {
			if (*p=='"') n++;
			p++;
		}
		if (n) {
			char *newstring = (char *)malloc(strlen(strtoadd)+1+n);
			p=strtoadd;
			char *q=newstring;
			while (*p) {
				if (*p=='"') *q++='\\';
				*q++=*p++;
			}
			*q='\0';
			strtoadd = newstring;
		}
	}
	if (!*str) {
		if (addquote) {
			*str=(char *)malloc(strlen(strtoadd)+3);
			strcat(*str,"\"");
			strcat(*str,strtoadd);
			strcat(*str,"\"");
		} else {
			*str=strdup(strtoadd);
		}
	} else {
		*str=(char *)realloc(*str,strlen(*str)+strlen(strtoadd)+3);
		if (addquote) strcat(*str,"\"");
		strcat(*str,strtoadd);
		if (addquote) strcat(*str,"\"");
	}
	if (n) free(strtoadd);
}

char *DMArray::toJSON()
{
	char *res = (char *)0;
	if (m_isList) {
		// This is a list
		addstr(&res,"[",false);
		int start = minIndex();
		int end = maxIndex();
		int offset = 0;
		for(int n = start; n <= end; n++) {
			char okey[32];
			sprintf(okey, "%d", n);
			Variable *var = get(okey);
			if(var) {
				switch(var->type()) {
					case vartype_integer:
						addstr(&res,var->toString(),false);
						break;
					case vartype_string:
						addstr(&res,var->toString(),true);
						break;
					case vartype_array:
						addstr(&res,var->getArray()->toJSON(),false);
						break;
					case vartype_object:
						addstr(&res,"<<OBJECT>>",true);
						break;
					case vartype_bool:
						addstr(&res,(char *)(var->getInt()?"true":"false"),false);
						break;
				}
				offset++;
				if (n<end) addstr(&res,",",false);
			}
		}
		addstr(&res,"]",false);
	} else {
		// Associative Array - treat as object
		addstr(&res,"{",false);
		int offset = maxIndex()+1;
		AutoPtr<StringList> keys = this->keys();
		StringListIterator kit(*keys);
		bool firstTimeThrough=true;
		for(const char *k = kit.first(); k; k = kit.next()) {
			if (!firstTimeThrough) addstr(&res,",",false);
			addstr(&res,(char *)k,true);
			addstr(&res,":",false);
			Variable *var = get(k);
			switch(var->type()) {
				case vartype_integer:
					addstr(&res,var->toString(),false);
					break;
				case vartype_string:
					addstr(&res,var->toString(),true);
					break;
				case vartype_array:
					addstr(&res,var->getArray()->toJSON(),false);
					break;
				case vartype_object:
					addstr(&res,"<<OBJECT>>",true);
					break;
				case vartype_bool:
					addstr(&res,(char *)(var->getInt()?"true":"false"),false);
					break;
			}
			firstTimeThrough=false;
		}
		addstr(&res,"}",false);
	}
	return res;
}


//////////////////////////////////////////////////////////////////////////////
// Scope
//////////////////////////////////////////////////////////////////////////////

Scope::Scope(const char *name, IObject *obj /*= NULL*/)
	: m_name(name), m_vars(new Hashtable<Variable>(false, true)),
	  m_obj(obj), m_action(NULL), m_fileno(0), m_lineno(0), m_fragno(0),
	  m_open(true), m_closed(false), m_autoDestruct(false), m_refCount(0)
{
	// Set default variables
	set("dep.total",    0);
	set("dep.success",  0);
	set("dep.failed",   0);
	set("dep.not_proc", 0);
}


Scope::Scope(
		const char *name, class ActionNode *action,
		ILocatable &loc, bool closed /*= false*/
	)
	: m_name(name), m_vars(new Hashtable<Variable>(false, true)),
	  m_obj(NULL), m_action(action), m_fileno(loc.fileno()),
	  m_lineno(loc.lineno()), m_fragno(loc.fragno()), m_open(false),
	  m_closed(closed), m_autoDestruct(false), m_refCount(0)
{
	// Set default variables
	set("dep.total",    0);
	set("dep.success",  0);
	set("dep.failed",   0);
	set("dep.not_proc", 0);
}


Scope::~Scope()
{
	debug3("~Scope('%s')", m_name);
	SAFE_DELETE(m_vars);	// set as owner so will delete variables
	m_name = NULL;			// don't own
	m_obj = NULL;			// don't own
}


const char* Scope::name() const
{
	return m_name;
}

DMArray *Scope::getVars() const
{
	DMArray *ret = new DMArray(false);
	AutoPtr<StringList> keys = m_vars->keys();
	StringListIterator iter(*keys);
	for(const char *k = iter.first(); k; k = iter.next()) {
		// Each Scope will have dep.success etc. That could be very confusing. Don't return them.
		if (strcmp(k,"dep.success") && strcmp(k,"dep.failed") && strcmp(k,"dep.total") && strcmp(k,"dep.not_proc")) {
			Variable *v = m_vars->get(k);
			ret->put(k,v);
		}
	}
	return ret;
}

DMArray *Scope::getVars(class Context &ctx) const
{
	DMArray *ret = new DMArray(false);
	AutoPtr<StringList> keys = m_vars->keys();
	StringListIterator iter(*keys);
	for(const char *k = iter.first(); k; k = iter.next()) {
		// Each Scope will have dep.success etc. That could be very confusing. Don't return them.
		if (strcmp(k,"dep.success") && strcmp(k,"dep.failed") && strcmp(k,"dep.total") && strcmp(k,"dep.not_proc"))
		{
			Variable *v = m_vars->get(k);
			if (v->m_type == vartype_string && v->getString() != NULL)
			{
			 char *stext1;
			 char *val = (char *)strdup(v->getString());
			 Node expandedText1(NODE_STR, val, true);
			 ExprPtr etext1 = expandedText1.evaluate(ctx);
			 stext1 = etext1->stringify();
			 v = new Variable(v->m_name,stext1);
			}
			ret->put(k,v);
		}
	}
	return ret;
}

int Scope::fileno() const
{
	return m_fileno;
}


int Scope::lineno() const
{
	return m_lineno;
}

int Scope::fragno() const
{
	return m_fragno;
}


class ActionNode *Scope::action() const
{
	return m_action;
}


bool Scope::isOpen() const
{
	return m_open;
}


void Scope::setOpen(bool open)
{
	OBJECT_KIND ok = m_obj->kind();
	if(!m_obj || (ok != OBJ_KIND_APPVERSION && ok != OBJ_KIND_APPLICATION)) {
		throw RuntimeError("Only Application Version scopes may have their visibility changed");
	}
	m_open = open;
}


bool Scope::isClosed() const
{
	return m_closed;
}


Variable *Scope::get(const char *name)
{
	//while(*name == '$') { name++; }
	if((*name == '$') && (name[1])) {
		debug1("##### variable with leading $ - %s", name);
	}
	return m_vars->get(name);
}


Variable *Scope::getXML(const char *name, class Context &ctx)
{
	if(m_obj) {
		//// TODO: val may need to be free'd
		//const char *val = m_obj->getAttribute(name, ctx);
		//if(val) {
		//	return new Variable(NULL, val);
		//}
		//// Not a simple text attribute, must be a meta-data pseudo-attribute
		//DMArray *ht = m_obj->getArrayAttribute(name, ctx);
		//if(ht) {
		//	return new Variable(NULL, ht);
		//}
		//IObject *obj = m_obj->getObjectAttribute(name, ctx);
		//if(obj) {
		//	return new Variable(NULL, obj);
		//}

		ExprPtr e = m_obj->getAttribute(name, ctx);
		return (e ? new Variable(NULL, (Expr*) e) : NULL);
	}
	return NULL;
}


DMArray *Scope::newArray(const char *name, bool isList /*= false*/, bool nocase /*= false*/)
{
	while(*name == '$') { name++; }

	DMArray *ht = new DMArray(isList, nocase, true);

	Variable *var = m_vars->get(name);
	if(var) {
		var->setArray(ht);
	} else {
		m_vars->put(name, new Variable(name, ht));
	}

	return ht;
}


OutputStream *Scope::newStream(const char *name)
{
	OutputStream *ostr = new OutputStream();

	Variable *var = m_vars->get(name);
	if(var) {
		var->setStream(ostr);
	} else {
		m_vars->put(name, new Variable(name, ostr));
	}

	return ostr;
}


class IObject *Scope::getScopeObject()
{
	return m_obj;
}


class ExtendedStmtImpl *Scope::getStatementObject()
{
	return NULL;
}


void Scope::setStatementObject(class ExtendedStmtImpl *stmt)
{
	throw RuntimeError("Scope type error");
}


void Scope::unsetScopeObject()
{
	m_obj = NULL;
}


bool Scope::isArray(const char *name)
{
	while(*name == '$') { name++; }

	Variable *var = m_vars->get(name);
	return (var && (var->m_type == vartype_array));
}


void Scope::set(const char *name, const char *value, bool nocase /*= false*/)
{
	Variable *var = m_vars->get(name);
	if(var) {
		// TODO: Lock
		var->setValue(value ? value : "");
		// TODO: Unlock
	} else {
		m_vars->put(name, new Variable(name, (value ? value : ""), nocase));
	}
}

void Scope::set(const char *name, bool value)
{
	Variable *var = m_vars->get(name);
	if(var) {
		var->setValue(value);
	} else {
		m_vars->put(name, new Variable(name, value));
	}
}

void Scope::set(const char *name, int value)
{
	// char str[30];
	// sprintf(str, "%d", value);
	// set(name, str);
	Variable *var = m_vars->get(name);
	if(var) {
		// TODO: Lock
		//debug1("Variable %s already exists - setting to %d",name,value);
		var->setValue(value);
		// TODO: Unlock
	} else {
		//debug1("Variable %s is new, setting initial value to %d",name,value);
		m_vars->put(name, new Variable(name, value));
	}
}


void Scope::set(const char *name, DMArray *arr)
{
	Variable *var = m_vars->get(name);
	if(var) {
		// TODO: Lock
		var->setArray(arr);
		// TODO: Unlock
	} else {
		m_vars->put(name, new Variable(name, arr));
	}
}


void Scope::set(const char *name, class IObject *obj)
{
	Variable *var = m_vars->get(name);
	if(var) {
		// TODO: Lock
		var->setObjectReference(obj);
		// TODO: Unlock
	} else {
		m_vars->put(name, new Variable(name, obj));
	}
}


void Scope::set(const char *name, class DMException &ex)
{
	Variable *var = m_vars->get(name);
	if(var) {
		// TODO: Lock
		var->setObjectReference(ex);
		// TODO: Unlock
	} else {
		m_vars->put(name, new Variable(name, ex));
	}
}


void Scope::set(const char *name, class ObjectReference *objref)
{
	Variable *var = m_vars->get(name);
	if(var) {
		// TODO: Lock
		var->setObjectReference(objref);
		// TODO: Unlock
	} else {
		m_vars->put(name, new Variable(name, objref));
	}
}


void Scope::set(const char *name, class OutputStream *ostr)
{
	Variable *var = m_vars->get(name);
	if(var) {
		// TODO: Lock
		var->setStream(ostr);
		// TODO: Unlock
	} else {
		m_vars->put(name, new Variable(name, ostr));
	}
}


void Scope::set(const char *name, class DateTime *date)
{
	Variable *var = m_vars->get(name);
	if(var) {
		// TODO: Lock
		var->setDate(date);
		// TODO: Unlock
	} else {
		m_vars->put(name, new Variable(name, date));
	}
}


// RHT 30/08/2011 - new append function to support append mode
void Scope::append(const char *name, const char *value, const char *sep)
{
	Variable *var = m_vars->get(name);
	if(var && (var->m_type == vartype_string)) {
		if(value) {
			// TODO: Lock
			if(!sep) sep = "";
			char *newval = (char*) malloc(strlen(var->m_value) + strlen(sep) + strlen(value) + 1);
			sprintf(newval, "%s%s%s", var->m_value, sep, value);
			var->setValue(newval);
			free(newval);	// setValue will strdup
			// TODO: Unlock
		}
	} else {
		set(name, value);
	}
}


/**
 * Dump the contents of the variables in this scope.
 */
void Scope::dump(DM &dm)
{
	dm.writeToLogFile("%s(%c%c):", m_name, (m_open ? 'o' : '-'), (m_closed ? 'c' : '-'));

	StringList *vars = m_vars->keys();
	StringListIterator iter(*vars);
	for(const char *name = iter.first(); name; name = iter.next())
	{
		Variable *var = m_vars->get(name);
		switch(var->m_type)
		{
		case vartype_bool:
			dm.writeToLogFile("  %s\t\t= %s", name, var->m_ival?"true":"false");
		case vartype_integer:
			dm.writeToLogFile("  %s\t\t= %d", name, var->m_ival);
			break;
		case vartype_string:
			dm.writeToLogFile("  %s\t\t= %s", name, (var->m_value ? var->m_value : "(null)"));
			break;
		case vartype_array: {
			dm.writeToLogFile("  %s[]\t\t=", name);
			StringList *keys = var->m_array->keys();
			StringListIterator iter(*keys);
			for(const char *k = iter.first(); k; k = iter.next()) {
				//const char *value = var->m_array->get(k);
				Variable *value = var->m_array->get(k);
				if(value) {
				switch(value->m_type) {
				case vartype_bool:
					dm.writeToLogFile("    %s\t\t-> %s", k, value->m_ival?"true":"false");
					break;
				case vartype_integer:
					dm.writeToLogFile("    %s\t\t-> %d", k, value->m_ival);
					break;
				case vartype_string:
					dm.writeToLogFile("    %s\t\t-> %s", k, (value->m_value ? value->m_value : "(empty)"));
					break;
				case vartype_array:
					dm.writeToLogFile("    %s\t\t-> array", k);
					break;
				case vartype_object:
					dm.writeToLogFile("    %s\t\t-> <object>", k);
					break;
				case vartype_stream:
					dm.writeToLogFile("    <stream>");
					break;
				}
				} else {
					dm.writeToLogFile("    %s\t\t-> (null)", k);
				}
			}
			SAFE_DELETE(keys);
			}
			break;
		case vartype_object:
			dm.writeToLogFile("    <object>");
			break;
		case vartype_stream:
			dm.writeToLogFile("    <stream>");
			break;
		}
	}
	SAFE_DELETE(vars);
}

/**
 * Dump the contents of the variables in this scope.
 */
void Scope::dump2File(class Context &ctx, FILE *fp)
{
	// fprintf(fp, "%s(%c%c):\n", m_name, (m_open ? 'o' : '-'), (m_closed ? 'c' : '-'));

	StringList *vars = m_vars->keys();
	StringListIterator iter(*vars);
	for(const char *name = iter.first(); name; name = iter.next())
	{
	    if (strstr(name,".") != NULL)
		  continue;

		Variable *var = m_vars->get(name);
   	    if (var->m_type == vartype_string && var->getString() != NULL)
		{
		 char *stext1;
		 char *val = (char *)strdup(var->getString());
		 Node expandedText1(NODE_STR, val, true);
		 ExprPtr etext1 = expandedText1.evaluate(ctx);
		 stext1 = etext1->stringify();
		 var = new Variable(var->m_name,stext1);
		}

		switch(var->m_type)
		{
		case vartype_bool:
			fprintf(fp,"%s=%s\n", name, var->m_ival?"true":"false");
		case vartype_integer:
			fprintf(fp,"%s=%d\n", name, var->m_ival);
			break;
		case vartype_string:
			fprintf(fp,"%s=%s\n", name, (var->m_value ? var->m_value : ""));
			break;
		case vartype_array:
			break;
		case vartype_object:
			// fprintf(fp,"    <object>");
			break;
		case vartype_stream:
			// fprintf(fp,"    <stream>");
			break;
		}
	}
	SAFE_DELETE(vars);
}

void Scope::addRef()
{
	++m_refCount;
	debug3("+Scope '%s' refcount now %d", m_name, m_refCount);
}


void Scope::releaseRef()
{
	--m_refCount;
	debug3("-Scope '%s' refcount now %d", m_name, m_refCount);
	if(m_refCount == 0) {
		debug3("Scope '%s' refcount now zero", m_name);
		delete this;
	}
}


//////////////////////////////////////////////////////////////////////////////
// DelegatingScope
//////////////////////////////////////////////////////////////////////////////

DelegatingScope::DelegatingScope(Scope &delegate, class IObject *obj)
	: Scope(NULL), m_delegate(delegate), m_obj(obj), m_stmt(NULL)
{
	m_delegate.addRef();
}


DelegatingScope::~DelegatingScope()
{
	m_delegate.releaseRef();
}


class IObject *DelegatingScope::getScopeObject()
{
	return m_obj ? m_obj : m_delegate.getScopeObject();
}


class ExtendedStmtImpl *DelegatingScope::getStatementObject()
{
	return m_stmt;
}


void DelegatingScope::setStatementObject(class ExtendedStmtImpl *stmt)
{
	m_stmt = stmt;
}


Variable *DelegatingScope::getXML(const char *name, class Context &ctx)
{
	//// TODO: Add array and object attributes
	//const char *val = m_obj->getAttribute(name, ctx);
	//if(val) {
	//	return new Variable(NULL, val);
	//}
	//return NULL;
	ExprPtr e = m_obj->getAttribute(name, ctx);
	return (e? new Variable(NULL, (Expr*) e) : NULL);
}


//////////////////////////////////////////////////////////////////////////////
// ScopeStack
//////////////////////////////////////////////////////////////////////////////

/**
 * Constructor.  Creates a new stack with a single "global" scope on it.
 */
ScopeStack::ScopeStack(DM &dm)
	: m_dm(dm), m_isOwner(true)
{
	m_top = 0;

	m_size = SCOPE_STACK_MIN_SIZE;
	m_stack = (Scope**) malloc(m_size * sizeof(Scope*));

	if (m_stack) {
		// Zero the stack
		for(int n = 0; n < m_size; n++) {
			m_stack[n] = NULL;
		}
	}

	// Create global scope
	Scope *global = new Scope("global");
	global->addRef();
	push(global);
}


/**
 * Copy constructor.  Performs a shallow copy of the stack.  Scopes are still
 * owned by the stack we are copying from.
 */
ScopeStack::ScopeStack(ScopeStack &other)
	: m_dm(other.m_dm), m_isOwner(false)
{
	m_top = other.m_top;

	m_size = other.m_size;
	m_stack = (Scope**) malloc(m_size * sizeof(Scope*));
	if (m_stack) {
		// Zero the stack
		int n;
		for(n = 0; n < m_size; n++) {
			m_stack[n] = NULL;
		}

		// Copy the other stack's elements across
		for(n = 0; n < m_top; n++) {
			m_stack[n] = other.m_stack[n];
			m_stack[n]->addRef();
		}
	}

	//m_dm.writeToLogFile("%d %s: copy", m_top-1, m_stack[m_top-1]->name());
}


ScopeStack::~ScopeStack()
{
	// Release all the scopes
	for(int n = 0; n < m_top; n++) {
		m_stack[n]->releaseRef();
	}

	// stack owns global scope
	if(m_isOwner && (m_top > 0) && m_stack[0]) {
		delete m_stack[0];
		m_stack[0] = NULL;
	}

	SAFE_FREE(m_stack);
}


/**
 * Pushes the given scope onto the top of the stack.
 */
void ScopeStack::push(Scope *scope)
{
	if(!scope) {
		throw RuntimeError(*this, "push failed: scope was null");
	}

	if(m_top >= (m_size-1)) {
		if(m_size >= SCOPE_STACK_MAX_SIZE) {
			throw RuntimeError(*this, "push failed: scope stack size exceeded");
		}
		m_size += SCOPE_STACK_INCREMENT;
		Scope **newStack;
		newStack = (Scope**) realloc(m_stack, m_size * sizeof(Scope**));
		if (newStack) {
			m_stack = newStack;
			debug2("Stack resized to %d slots", m_size);
		} else {
			throw RuntimeError(*this, "push failed: Out of Memory");
		}
	}

	m_stack[m_top] = scope;
	scope->addRef();

	//m_dm.writeToLogFile("%d %s: push", m_top, m_stack[m_top]->name());

	m_top++;
}


/**
 * Pops the topmost scope from the stack and returns it.
 */
/*Scope **/ void ScopeStack::pop(const char *expect /*= 0*/)
{
	if(m_top < 1) {
		throw RuntimeError("pop failed: scope stack empty");
	}

	--m_top;
	Scope *ret = m_stack[m_top];
	m_stack[m_top] = NULL;

	if(ret) {
		//m_dm.writeToLogFile("%d %s: pop", m_top, ret->name());
		if(expect && ret->name() && (strcmp(ret->name(), expect) != 0)) {
			throw RuntimeError(*this, "Stack out of sync - expecting '%s' got '%s'", expect, ret->name());
		}
		ret->releaseRef();
	}

	//return ret;
}


// ### direct access to scopes for new expr stuff
Scope *ScopeStack::getScope(const char *name)
{
	for(int n = m_top-1; n >= 0; n--) {
		if(strcmp(name, m_stack[n]->name()) == 0) {
			return m_stack[n];
		}
	}

	// If not a current scope name, check old names for environment and server
	if(strcmp(name, OLD_LOGICAL_SCOPE) == 0) {
		return getScope(ENVIRONMENT_SCOPE);
	} else if(strcmp(name, OLD_PHYSICAL_SCOPE) == 0) {
		return getScope(SERVER_SCOPE);
	}

	return NULL;
}

Variable *ScopeStack::expand(Variable *v)
{
 int addTicks = 0;
 char value[4096] = {""};

 if (v->getString() == NULL)
  return v;

 strcpy(value,v->getString());
 debug1("RESET2 [%s]", value);

 if (*value == '\'' && *(value+1) == '$')
 {
  strcpy(value,v->getString()+1);
  *(value + strlen(value)-1) = '\0';
  addTicks = 1;
 }

 if (*value == '$')
 {
  char workVar[4096] = {""};

  if (*value == '$' && *(value+1) == '{')
  {
   strcpy(workVar,value+2);
	*(workVar + strlen(workVar)-1) = '\0';
  }
  else
    strcpy(workVar,value+1);

  Variable *ret = NULL;

  debug1("RESET3 [%s]", workVar);

  for(int n = m_top-1; n >= 0; n--)
  {
   if ((ret = m_stack[n]->get(workVar)) != NULL)
	break;
  }

  if (ret != NULL)
  {
   debug1("RESET %s [%s]", value, ret->getString());
   if (addTicks)
   {
	sprintf(workVar,"'%s'", ret->getString());
	return new Variable(ret->m_name,workVar);
   }
   return new Variable(ret->m_name,ret->getString());
  }
 }
 return v;
}

/**
 * Navigates the stack from top to bottom looking for a variable with
 * the given name in each Scope, returning the first one it finds, unless
 * the variable name contains a '.' in which case we first look for a scope
 * with a name matching that part of the variable name before the '.' - if
 * found then only this scope is checked, otherwise a variable with the full
 * name is searched for in the normal manner.
 */
Variable *ScopeStack::get(const char *name)
{
	Variable *ret = NULL;
	bool first = true;

	for(int n = m_top-1; n >= 0; n--) {
		if((first || m_stack[n]->isOpen()) && ((ret = m_stack[n]->get(name)) != NULL)) {
			// debug1("RESET1 %d %s: %s=[%s]", n, m_stack[n]->name(), name, ret->getString());
			break;
		}
		if(m_stack[n]->isClosed()) {
			first = false;
		}
	}
	if (!ret && name[0]=='?' && name[1]=='\0') {
		// Request for $? but it's not set anywhere
		ret = new Variable("?",0);
	}

  return ret;
}


/**
 * Navigates the stack from top to bottom looking in the XML element
 * associated with each Scope (if any) for an attribute named by name.
 */
Variable *ScopeStack::getXML(const char *name, class Context &ctx)
{
	Variable *ret = NULL;

	for(int n = m_top-1; n >= 0; n--) {
		if((ret = m_stack[n]->getXML(name, ctx)) != NULL) {
			return ret;
		}
	}

	return ret;
}


/**
 * Sets a string variable in the topmost scope of the stack.
 */
//void ScopeStack::set(const char *name, const char *value, bool nocase /*= false*/)
//{
//	if(m_top < 1) {
//		throw RuntimeError("set failed: scope stack empty");
//	}
//
//	//m_dm.writeToLogFile("%d %s: set %s=[%s]", m_top-1, m_stack[m_top-1]->name(), name, value);
//	m_stack[m_top-1]->set(name, value, nocase);
//}


/**
 * Sets an integer variable in the topmost scope of the stack.
 */
//void ScopeStack::set(const char *name, int value)
//{
//	if(m_top < 1) {
//		throw RuntimeError("set failed: scope stack empty");
//	}
//
//	//m_dm.writeToLogFile("%d %s: set %s=%d", m_top-1, m_stack[m_top-1]->name(), name, value);
//	m_stack[m_top-1]->set(name, value);
//}


void ScopeStack::set(const char *name, DMArray* arr)
{
	if(m_top < 1) {
		throw RuntimeError("set failed: scope stack empty");
	}

	//m_dm.writeToLogFile("%d %s: set %s=%p", m_top-1, m_stack[m_top-1]->name(), name, arr);
	m_stack[m_top-1]->set(name, arr);
}


/**
 * Sets an exception variable in the topmost scope of the stack.
 */
//void ScopeStack::set(const char *name, class DMException& ex)
//{
//	if(m_top < 1) {
//		throw RuntimeError("set failed: scope stack empty");
//	}
//
//	//m_dm.writeToLogFile("%d %s: set %s=%p", m_top-1, m_stack[m_top-1]->name(), name, ex);
//	m_stack[m_top-1]->set(name, ex);
//}


/**
 * Sets an object variable in the topmost scope of the stack.
 */
//void ScopeStack::set(const char *name, class ObjectReference *objref)
//{
//	if(m_top < 1) {
//		throw RuntimeError("set failed: scope stack empty");
//	}
//
//	//m_dm.writeToLogFile("%d %s: set %s=%p", m_top-1, m_stack[m_top-1]->name(), name, objref);
//	m_stack[m_top-1]->set(name, objref);
//}


/**
 * Sets a stream variable in the topmost scope of the stack.
 */
//void ScopeStack::set(const char *name, class OutputStream *ostr)
//{
//	if(m_top < 1) {
//		throw RuntimeError("set failed: scope stack empty");
//	}
//
//	//m_dm.writeToLogFile("%d %s: set %s=%p", m_top-1, m_stack[m_top-1]->name(), name, ostr);
//	m_stack[m_top-1]->set(name, ostr);
//}


/**
 * Appends a string variable in the bottom-most scope of the stack.
 */
//void ScopeStack::append(const char *name, const char *value, const char *sep)
//{
//	if(m_top < 1) {
//		throw RuntimeError("appendGlobal failed: scope stack empty");
//	}
//
//	//m_dm.writeToLogFile("%d %s: append %s=[%s]", m_top-1, m_stack[m_top-1]->name(), name, value);
//	m_stack[m_top-1]->append(name, value, sep);
//}


/**
 * Sets a string variable in the topmost scope of the stack.
 */
void ScopeStack::setLocal(const char *name, const char *value, bool nocase /*= false*/)
{
	if(m_top < 1) {
		throw RuntimeError("set failed: scope stack empty");
	}

	// Look for first non-Object scope
	for(int n = m_top-1; n >= 0; n--) {
		if(!m_stack[n]->isOpen()) {
			//debug1("%d %s: set %s=[%s]", n, m_stack[n]->name(), name, value);
			m_stack[n]->set(name, value, nocase);
			return;
		}
		if(m_stack[n]->isClosed()) {
			debug1("Encountered closed scope before first open - skip to global");
			break;
		}
	}

	// Didn't find one - use global scope
	debug1("%d %s: set %s=[%s]", 0, m_stack[0]->name(), name, value);
	m_stack[0]->set(name, value, nocase);
}

void ScopeStack::setLocal(const char *name, bool value)
{
	if(m_top < 1) {
		throw RuntimeError("set failed: scope stack empty");
	}

	// Look for first non-Object scope
	for(int n = m_top-1; n >= 0; n--) {
		if(!m_stack[n]->isOpen()) {
			//debug1("%d %s: set %s=[%d]", n, m_stack[n]->name(), name, value);
			m_stack[n]->set(name, value);
			return;
		}
		if(m_stack[n]->isClosed()) {
			debug1("Encountered closed scope before first open - skip to global");
			break;
		}
	}

	// Didn't find one - use global scope
	debug1("%d %s: set %s=[%d]", 0, m_stack[0]->name(), name, value);
	m_stack[0]->set(name, value);
}

/**
 * Sets an integer variable in the topmost scope of the stack.
 */
void ScopeStack::setLocal(const char *name, int value)
{
	if(m_top < 1) {
		throw RuntimeError("set failed: scope stack empty");
	}

	// Look for first non-Object scope
	for(int n = m_top-1; n >= 0; n--) {
		if(!m_stack[n]->isOpen()) {
			//debug1("%d %s: set %s=[%d]", n, m_stack[n]->name(), name, value);
			m_stack[n]->set(name, value);
			return;
		}
		if(m_stack[n]->isClosed()) {
			debug1("Encountered closed scope before first open - skip to global");
			break;
		}
	}

	// Didn't find one - use global scope
	debug1("%d %s: set %s=[%d]", 0, m_stack[0]->name(), name, value);
	m_stack[0]->set(name, value);
}


void ScopeStack::setLocal(const char *name, DMArray* arr)
{
	if(m_top < 1) {
		throw RuntimeError("set failed: scope stack empty");
	}

	// Look for first non-Object scope
	for(int n = m_top-1; n >= 0; n--) {
		if(!m_stack[n]->isOpen()) {
			//debug1("%d %s: set %s=%p", n, m_stack[n]->name(), name, arr);
			m_stack[n]->set(name, arr);
			return;
		}
		if(m_stack[n]->isClosed()) {
			debug1("Encountered closed scope before first open - skip to global");
			break;
		}
	}

	// Didn't find one - use global scope
	debug1("%d %s: set %s=%p", 0, m_stack[0]->name(), name, arr);
	m_stack[0]->set(name, arr);
}


/**
 * Sets an exception variable in the topmost scope of the stack.
 */
void ScopeStack::setLocal(const char *name, class DMException& ex)
{
	if(m_top < 1) {
		throw RuntimeError("set failed: scope stack empty");
	}

	// Look for first non-Object scope
	for(int n = m_top-1; n >= 0; n--) {
		if(!m_stack[n]->isOpen()) {
			//debug1("%d %s: set %s=%p", n, m_stack[n]->name(), name, &ex);
			m_stack[n]->set(name, ex);
			return;
		}
		if(m_stack[n]->isClosed()) {
			debug1("Encountered closed scope before first open - skip to global");
			break;
		}
	}

	// Didn't find one - use global scope
	debug1("%d %s: set %s=%p", 0, m_stack[0]->name(), name, &ex);
	m_stack[0]->set(name, ex);
}


/**
 * Sets an object variable in the topmost scope of the stack.
 */
void ScopeStack::setLocal(const char *name, class ObjectReference *objref)
{
	if(m_top < 1) {
		throw RuntimeError("set failed: scope stack empty");
	}

	// Look for first non-Object scope
	for(int n = m_top-1; n >= 0; n--) {
		if(!m_stack[n]->isOpen()) {
			//debug1("%d %s: set %s=%p", n, m_stack[n]->name(), name, objref);
			m_stack[n]->set(name, objref);
			return;
		}
		if(m_stack[n]->isClosed()) {
			debug1("Encountered closed scope before first open - skip to global");
			break;
		}
	}

	// Didn't find one - use global scope
	debug1("%d %s: set %s=%p", 0, m_stack[0]->name(), name, objref);
	m_stack[0]->set(name, objref);
}


/**
 * Sets a stream variable in the topmost scope of the stack.
 */
void ScopeStack::setLocal(const char *name, class OutputStream *ostr)
{
	if(m_top < 1) {
		throw RuntimeError("set failed: scope stack empty");
	}

	// Look for first non-Object scope
	for(int n = m_top-1; n >= 0; n--) {
		if(!m_stack[n]->isOpen()) {
			//debug1("%d %s: set %s=%p", n, m_stack[n]->name(), name, ostr);
			m_stack[n]->set(name, ostr);
			return;
		}
		if(m_stack[n]->isClosed()) {
			debug1("Encountered closed scope before first open - skip to global");
			break;
		}
	}

	debug1("%d %s: set %s=%p", 0, m_stack[0]->name(), name, ostr);
	m_stack[0]->set(name, ostr);
}


/**
 * Sets a date variable in the topmost scope of the stack.
 */
void ScopeStack::setLocal(const char *name, class DateTime *date)
{
	if(m_top < 1) {
		throw RuntimeError("set failed: scope stack empty");
	}

	// Look for first non-Object scope
	for(int n = m_top-1; n >= 0; n--) {
		if(!m_stack[n]->isOpen()) {
			//debug1("%d %s: set %s=%p", n, m_stack[n]->name(), name, date);
			m_stack[n]->set(name, date);
			return;
		}
		if(m_stack[n]->isClosed()) {
			debug1("Encountered closed scope before first open - skip to global");
			break;
		}
	}

	debug1("%d %s: set %s=%p", 0, m_stack[0]->name(), name, date);
	m_stack[0]->set(name, date);
}

/**
 * Appends a string variable in the bottom-most scope of the stack.
 */
void ScopeStack::appendLocal(const char *name, const char *value, const char *sep)
{
	if(m_top < 1) {
		throw RuntimeError("appendGlobal failed: scope stack empty");
	}

	// Look for first non-Object scope
	for(int n = m_top-1; n >= 0; n--) {
		if(!m_stack[n]->isOpen()) {
			//debug1("%d %s: append %s=[%s]", n, m_stack[n]->name(), name, value);
			m_stack[n]->append(name, value, sep);
			return;
		}
		if(m_stack[n]->isClosed()) {
			debug1("Encountered closed scope before first open - skip to global");
			break;
		}
	}

	debug1("%d %s: append %s=[%s]", 0, m_stack[0]->name(), name, value);
	m_stack[0]->append(name, value, sep);
}


/**
 * Sets a string variable in the bottom-most scope of the stack.
 */
void ScopeStack::setGlobal(const char *name, const char *value, bool nocase /*= false*/)
{
	if(m_top < 1) {
		throw RuntimeError("setGlobal failed: scope stack empty");
	}

	//m_dm.writeToLogFile("0 %s: set %s=[%s]", m_stack[0]->name(), name, value);
	m_stack[0]->set(name, value, nocase);
}


void ScopeStack::setGlobal(const char *name, DMArray* arr)
{
	if(m_top < 1) {
		throw RuntimeError("setGlobal failed: scope stack empty");
	}

	//m_dm.writeToLogFile("0 %s: set %s=%p", m_stack[0]->name(), name, arr);
	m_stack[0]->set(name, arr);
}


/**
 * Sets an object variable in the bottom-most scope of the stack.
 */
void ScopeStack::setGlobal(const char *name, class ObjectReference *objref)
{
	if(m_top < 1) {
		throw RuntimeError("setGlobal failed: scope stack empty");
	}

	//m_dm.writeToLogFile("0 %s: set %s=%p", m_stack[0]->name(), name, objref);
	m_stack[0]->set(name, objref);
}


/**
 * Sets a stream variable in the bottom-most scope of the stack.
 */
void ScopeStack::setGlobal(const char *name, class OutputStream *ostr)
{
	if(m_top < 1) {
		throw RuntimeError("setGlobal failed: scope stack empty");
	}

	//m_dm.writeToLogFile("0 %s: set %s=%p", m_stack[0]->name(), name, ostr);
	m_stack[0]->set(name, ostr);
}


/**
 * Sets a date variable in the bottom-most scope of the stack.
 */
void ScopeStack::setGlobal(const char *name, class DateTime *date)
{
	if(m_top < 1) {
		throw RuntimeError("setGlobal failed: scope stack empty");
	}

	//m_dm.writeToLogFile("0 %s: set %s=%p", m_stack[0]->name(), name, date);
	m_stack[0]->set(name, date);
}


/**
 * Sets an integer variable in the bottom-most scope of the stack.
 */

void ScopeStack::setGlobal(const char *name, bool value)
{
	if(m_top < 1) {
		throw RuntimeError("setGlobal failed: scope stack empty");
	}

	//m_dm.writeToLogFile("0 %s: set %s=%d", m_stack[0]->name(), name, value);
	m_stack[0]->set(name, value);
}

void ScopeStack::setGlobal(const char *name, int value)
{
	if(m_top < 1) {
		throw RuntimeError("setGlobal failed: scope stack empty");
	}

	//m_dm.writeToLogFile("0 %s: set %s=%d", m_stack[0]->name(), name, value);
	m_stack[0]->set(name, value);
}

/**
 * Appends a string variable in the bottom-most scope of the stack.
 */
void ScopeStack::appendGlobal(const char *name, const char *value, const char *sep)
{
	if(m_top < 1) {
		throw RuntimeError("appendGlobal failed: scope stack empty");
	}

	//m_dm.writeToLogFile("0 %s: append %s=[%s]", m_stack[0]->name(), name, value);
	m_stack[0]->append(name, value, sep);
}


/**
 * Appends (or sets) the named variable in all the scopes on the stack.
 */
void ScopeStack::appendAll(const char *name, const char *list)
{
	for(int n = m_top-1; n >= 0; n--) {
		//m_dm.writeToLogFile("%d %s: append %s=%s", n, m_stack[n]->name(), name, list);

		// RHT 30/08/2011 - use new append fn
		m_stack[n]->append(name, list, " ");
	}
}


/**
 * Increments (or sets) the named variable in all the scopes on the stack.
 */
void ScopeStack::incrementAll(const char *name, int inc)
{
	for(int n = m_top-1; n >= 0; n--) {
		//const char *oldvalue = m_stack[n]->get(name);
		Variable *oldvalue = m_stack[n]->get(name);
		if(oldvalue && (oldvalue->m_type == vartype_integer)) {
			m_stack[n]->set(name, oldvalue->m_ival + inc);
		}
		else
		if(oldvalue && (oldvalue->m_type == vartype_string)) {
			// Increment the old value and set it
			int val = atoi(oldvalue->m_value);
			//m_dm.writeToLogFile("%d %s: inc %s=%d (%d + %d)", n, m_stack[n]->name(), name, val + inc, val, inc);
			m_stack[n]->set(name, val + inc);
		} else {
			// Just set the new value
			//m_dm.writeToLogFile("%d %s: inc %s=%d", n, m_stack[n]->name(), name, inc);
			m_stack[n]->set(name, inc);
		}
	}
}


/**
 * Returns true if the stack contains an scope with the given name, false otherwise.
 */
bool ScopeStack::contains(const char *name)
{
	for(int n = m_top-1; n >= 0; n--) {
		if(m_stack[n]->name() && (strcmp(name, m_stack[n]->name()) == 0)) {
			return true;
		}
	}

	return false;
}


/**
 * Returns true if the stack contains any of deploy, remotescript or trilogy.
 */
bool ScopeStack::isRestricted()
{
	for(int n = m_top-1; n >= 0; n--) {
		const char *name = m_stack[n]->name();
		if(name
			&& ((strcmp(DEPLOY_SCOPE, name) == 0)
			 || (strcmp(CHECKIN_SCOPE, name) == 0)
			 || (strcmp(REMOTESCRIPT_SCOPE, name) == 0)
			 || (strcmp(TRILOGY_SCOPE, name) == 0)
			 || (strcmp(POST_SCOPE, name) == 0)))
		{
			return true;
		}
	}

	return false;
}


/**
 * Used by 'return' statement to determine whether we are able to return from a
 * function.  Returns true of the top level stack element is a FUNCTION_SCOPE.
 */
bool ScopeStack::isFunction()
{
	//return ((m_top > 0) && (strcmp(m_stack[m_top-1]->name(), FUNCTION_SCOPE) == 0));

	for(int n = m_top-1; n >= 0; n--) {
		if((strcmp(m_stack[n]->name(), FUNCTION_SCOPE) == 0)
				|| (strcmp(m_stack[n]->name(), ACTION_SCOPE) == 0)) {
			return (strcmp(m_stack[n]->name(), FUNCTION_SCOPE) == 0);
		}
	}
	return false;
}


/**
 * Used by 'return' statement to determine whether we are able to return from
 * an action.  Returns true of the top level stack element is an ACTION_SCOPE.
 */
bool ScopeStack::isAction()
{
	//return ((m_top > 0) && (strcmp(m_stack[m_top-1]->name(), ACTION_SCOPE) == 0));

	for(int n = m_top-1; n >= 0; n--) {
		if((strcmp(m_stack[n]->name(), FUNCTION_SCOPE) == 0)
				|| (strcmp(m_stack[n]->name(), ACTION_SCOPE) == 0)) {
			return (strcmp(m_stack[n]->name(), ACTION_SCOPE) == 0);
		}
	}
	return false;
}


//Hashtable *ScopeStack::getArray(const char *name)
//{
//	Hashtable *ret = NULL;
//
//	for(int n = m_top-1; n >= 0; n--)
//	{
//		if( (ret = m_stack[n]->getArray(name)) != NULL ) {
//			//m_dm.writeToLogFile("%d %s: %s=[%s]", n, m_stack[n]->name(), name, ret);
//			break;
//		}
//	}
//
//	//m_dm.writeToLogFile("0 %s: set %s=%d", m_stack[0]->name(), name, value);
//	return ret;
//}


//DMArray *ScopeStack::newArray(const char *name, bool isList /*= false*/, bool nocase /*= false*/)
//{
//	if(m_top < 1) {
//		throw RuntimeError("newArray failed: scope stack empty");
//	}
//
//	//m_dm.writeToLogFile("%d %s: newArray %s", m_top-1, m_stack[m_top-1]->name(), name);
//	return m_stack[m_top-1]->newArray(name, isList, nocase);
//}


DMArray *ScopeStack::newLocalArray(const char *name, bool isList /*= false*/, bool nocase /*= false*/)
{
	if(m_top < 1) {
		throw RuntimeError("newArray failed: scope stack empty");
	}

	// Look for first non-Object scope
	for(int n = m_top-1; n >= 0; n--) {
		if(!m_stack[n]->isOpen()) {
			//debug1("%d %s: newArray %s", n, m_stack[n]->name(), name);
			return m_stack[n]->newArray(name, isList, nocase);
		}
		if(m_stack[n]->isClosed()) {
			debug1("Encountered closed scope before first open - skip to global");
			break;
		}
	}

	// Didn't find one - use global scope
	debug1("%d %s: newArray %s", 0, m_stack[0]->name(), name);
	return m_stack[0]->newArray(name, isList, nocase);
}


DMArray *ScopeStack::newGlobalArray(const char *name, bool isList /*= false*/, bool nocase /*= false*/)
{
	if(m_top < 1) {
		throw RuntimeError("newGlobalArray failed: scope stack empty");
	}

	//m_dm.writeToLogFile("0 %s: newGlobalArray %s", m_stack[0]->name(), name);
	return m_stack[0]->newArray(name, isList, nocase);
}


/**
 * Creates a new stream object - this is created in the local scope and will be
 * destroyed when the variable that refers to it goes out of scope.  If the
 * variable is assigned to another, then reference counting should take care of
 * the destruction.
 */
OutputStream *ScopeStack::newStream(const char *name)
{
	if(m_top < 1) {
		throw RuntimeError("newGlobalArray failed: scope stack empty");
	}

	//m_dm.writeToLogFile("0 %s: newStream %s", m_stack[0]->name(), name);
	//return m_stack[0]->newStream(name);
	// return m_stack[m_top-1]->newStream(name);
	return m_stack[0]->newStream(name);				// Always declare streams at global level.
}



/**
 * Dumps the stack so that we can see the current state of it.
 */
void ScopeStack::dump()
{
	for(int n = m_top-1; n >= 0; n--) {
		m_stack[n]->dump(m_dm);
	}
}

/**
 * Dumps the stack so that we can see the current state of it.
 */
void ScopeStack::dump2File(class Context &ctx)
{
	FILE *fp;
    char name[1024] = {""};

	if ((fp=fopen("/tmp/remote.env","w")) != NULL)
	{
	 for(int n = 1; n < m_top; n++)  {
		m_stack[n]->dump2File(ctx,fp);
	 }
	 fclose(fp);
	}
}


/**
 * Prints a stacktrace.
 */

void ScopeStack::trace(IOutputStream &out, ILocatable &loc)
{
	int fileno = loc.fileno(), lineno = loc.lineno(), fragno = loc.fragno();

	out.writeToStdOut("Stacktrace:");

	for(int n = m_top-1; n >= 0; n--) {
		const char *filename = fileno ? FilenameCache::getFilename(fileno) : NULL;
		if(m_stack[n]->action()) {
			const char *fa = m_stack[n]->action()->isFunction() ? "function" : "action";
			if(fragno) {
				// TODO: show fragment name and line number of generated file
				out.writeToStdOut("\t%s %s at fragment %d", fa, m_stack[n]->action()->name(), fragno);
			} else if(lineno) {
				if(filename) {
					out.writeToStdOut("\t%s %s at line %d of \"%s\"", fa, m_stack[n]->action()->name(), lineno, filename);
				} else {
					out.writeToStdOut("\t%s %s at line %d", fa, m_stack[n]->action()->name(), lineno);
				}
			} else {
				out.writeToStdOut("\t%s %s", fa, m_stack[n]->action()->name());
			}
		} else if(m_stack[n]->getScopeObject()) {
			List<Server> emptyList;
			Context ctx(m_dm, emptyList, *this);
			//const char *objname = m_stack[n]->getScopeObject()->getAttribute("name", ctx);
			ExprPtr eobjname = m_stack[n]->getScopeObject()->getAttribute("name", ctx);
			ConstCharPtr objname = (eobjname ? eobjname->toString() : NULL);
			if(!objname) {
				objname = strdup("");
			}
			if(lineno) {
				if(filename) {
					out.writeToStdOut("\t%s %s at line %d of \"%s\"", m_stack[n]->name(), (const char*) objname, lineno, filename);
				} else {
					out.writeToStdOut("\t%s %s at line %d", m_stack[n]->name(), (const char*) objname, lineno);
				}
			} else {
				out.writeToStdOut("\t%s %s", m_stack[n]->name(), (const char*) objname);
			}
		} else {
			if(lineno) {
				if(filename) {
					out.writeToStdOut("\t%s at line %d of \"%s\"", m_stack[n]->name(), lineno, filename);
				} else {
					out.writeToStdOut("\t%s at line %d", m_stack[n]->name(), lineno);
				}
			} else {
				out.writeToStdOut("\t%s", m_stack[n]->name());
			}
		}

		fileno = m_stack[n]->fileno();
		lineno = m_stack[n]->lineno();
		fragno = m_stack[n]->fragno();
	}
}
