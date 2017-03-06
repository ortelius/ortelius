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
#ifndef _SCOPESTACK_H

#define _SCOPESTACK_H

#include "dmapiexport.h"
#include "hashtable.h"

#define GLOBAL_SCOPE		"global"
#define ENVIRONMENT_SCOPE	"environment"
#define OLD_LOGICAL_SCOPE	"logical"
#define APPLICATION_SCOPE	"application"
#define SERVER_SCOPE		"server"
#define OLD_PHYSICAL_SCOPE	"physical"
#define ACTION_SCOPE		"action"
#define FUNCTION_SCOPE		"function"
#define DEPLOY_SCOPE		"deploy"
#define CHECKIN_SCOPE		"checkin"
#define REMOTESCRIPT_SCOPE	"remotescript"
#define TRILOGY_SCOPE		"trilogy"
#define POST_SCOPE			"post"

#define DROPZONE_SCOPE		"dropzone"
#define COMPONENT_SCOPE     "component"
#define TEST_SCOPE			"test"
#define REPOSITORY_SCOPE	"repository"
#define NOTIFY_SCOPE		"notify"
#define DATASOURCE_SCOPE	"datasource"
#define MODIFY_SCOPE		"modify"
#define AVLOOP_SCOPE		"avloop"
#define TASK_SCOPE			"task"


// ### moved
typedef enum _vartype {
	vartype_integer,
	vartype_string,
	vartype_array,
	vartype_object,
	vartype_stream,
	vartype_date,
	vartype_bool
} VarType;


// ### moved
class DMAPI_API Variable
{
private:
	char	*m_name;
	VarType  m_type;
	bool     m_nocase;
	union {
		int						m_ival;
		char					*m_value;
		class DMArray			*m_array;
		class ObjectReference	*m_objref;
		class OutputStream		*m_stream;
		class DateTime			*m_date;
	};

public:
	Variable(const char *name, bool value);
	Variable(const char *name, int value);
	Variable(const char *name, const char *value, bool nocase = false);
	Variable(const char *name, class DMArray *arr);
	Variable(const char *name, class IObject *obj);
	Variable(const char *name, class DMException &ex);
	Variable(const char *name, class ObjectReference *objref);
	Variable(const char *name, class OutputStream *stream);
	Variable(const char *name, class DateTime *date);
	Variable(const char *name, class Expr *expr);
	Variable(const Variable& copy);
	~Variable();

	void setValue(bool value);
	void setValue(int value);
	void setValue(const char *value);
	void setArray(class DMArray *arr);
	void setObjectReference(class IObject *obj);
	void setObjectReference(class DMException &ex);
	void setObjectReference(class ObjectReference *objref);
	void setStream(class OutputStream *stream);
	void setDate(class DateTime *date);
	int type()	{ return m_type; }
	int getInt();
	const char *getString();
	class DMArray *getArray();
	class ObjectReference *getObjectReference();
	class OutputStream *getStream();
	class DateTime *getDate();
	bool getNoCase()   { return m_nocase; }
	bool isSimple()    { return (m_type == vartype_integer) || (m_type == vartype_string); }
	char *toString();
	char *flatten();

private:
	void freeOldValue();

	friend class Scope;
	friend class ScopeStack;
};


class DMAPI_API DMArray : public Hashtable<class Variable>
{
private:
	bool m_isList;
	int m_refCount;
	void addstr(char **str,char *strtoadd,bool addquote);

public:
	DMArray(bool isList, bool nocase = false, bool isOwner = false, int buckets = 10);
	virtual ~DMArray();

	int minIndex() const;
	int maxIndex() const;

	void add(Variable *var);
	void addAll(const DMArray &arr);

	bool isList();
	char *toJSON();

	void addRef();
	void releaseRef();

	DMArray *operator +(const DMArray &b) const;
};


class DMAPI_API Scope
{
private:
	const char				*m_name;
	Hashtable<Variable>		*m_vars;
	class IObject			*m_obj;
	class ActionNode		*m_action;
	bool					 m_open;			// true if created with object constructor
	bool					 m_closed;			// true if a function or action
	int						 m_fileno;
	int						 m_lineno;
	int						 m_fragno;
	bool					 m_autoDestruct;
	int						 m_refCount;

public:
	Scope(const char *name, class IObject *obj = 0);
	Scope(
		const char *name, class ActionNode *m_action,
		class ILocatable &loc, bool closed = false);
	virtual ~Scope();

	virtual DMArray *getVars() const;
	virtual const char* name() const;
	virtual int fileno() const;
	virtual int lineno() const;
	virtual int fragno() const;
	virtual class ActionNode *action() const;
	virtual bool isOpen() const;
	void setOpen(bool open);
	virtual bool isClosed() const;

	virtual Variable *get(const char *name);
	virtual Variable *getXML(const char *name, class Context &ctx);
	virtual DMArray *newArray(const char *name, bool isList = false, bool nocase = false);
	virtual bool isArray(const char *name);
	virtual class OutputStream *newStream(const char *name);
	virtual class IObject *getScopeObject();
	virtual class ExtendedStmtImpl *getStatementObject();
	virtual void setStatementObject(class ExtendedStmtImpl *stmt);
	virtual void unsetScopeObject();

	// Overloaded - just pass the a string or an int
	virtual void set(const char *name, const char *svalue, bool nocase = false);
	virtual void set(const char *name, bool value);
	virtual void set(const char *name, int ivalue);
	virtual void set(const char *name, DMArray *arr);
	virtual void set(const char *name, class IObject* obj);
	virtual void set(const char *name, class DMException& ex);
	virtual void set(const char *name, class ObjectReference* objref);
	virtual void set(const char *name, class OutputStream* ostr);
	virtual void set(const char *name, class DateTime* date);
	virtual void append(const char *name, const char *svalue, const char *sep);

	virtual void dump(class DM &dm);

	virtual void addRef();
	virtual void releaseRef();

	friend class Envp;
	//friend class ScopeStack;
};


class DMAPI_API DelegatingScope : public Scope
{
private:
	Scope &m_delegate;
	class IObject *m_obj;
	class ExtendedStmtImpl *m_stmt;

public:
	DelegatingScope(Scope &delegate, class IObject *obj);
	~DelegatingScope();

	// Scope
	const char* name() const		  { return m_delegate.name(); }
	int fileno() const				  { return m_delegate.fileno(); }
	int lineno() const				  { return m_delegate.lineno(); }
	int fragno() const				  { return m_delegate.fragno(); }
	class ActionNode *action() const  { return m_delegate.action(); }
	bool isOpen() const				  { return m_delegate.isOpen(); }
	bool isClosed() const			  { return m_delegate.isClosed(); }

	Variable *get(const char *name)	  { return m_delegate.get(name); }
	Variable *getXML(const char *name, class Context &ctx);
	DMArray *newArray(const char *name, bool isList = false, bool nocase = false)  { return m_delegate.newArray(name, isList, nocase); }
	bool isArray(const char *name)	  { return m_delegate.isArray(name); }
	class OutputStream *newStream(const char *name)  { return m_delegate.newStream(name); }
	class IObject *getScopeObject();
	class ExtendedStmtImpl *getStatementObject();
	void setStatementObject(class ExtendedStmtImpl *stmt);
	void unsetScopeObject()			  { m_delegate.unsetScopeObject(); }

	void set(const char *name, const char *svalue, bool nocase = false)	{ m_delegate.set(name, svalue, nocase); }
	void set(const char *name, int ivalue)								{ m_delegate.set(name, ivalue); }
	void set(const char *name, DMArray *arr)							{ m_delegate.set(name, arr); }
	void set(const char *name, class IObject* obj)						{ m_delegate.set(name, obj); }
	void set(const char *name, class DMException& ex)					{ m_delegate.set(name, ex); }
	void set(const char *name, class ObjectReference* objref)			{ m_delegate.set(name, objref); }
	void set(const char *name, class OutputStream* ostr)				{ m_delegate.set(name, ostr); }
	void set(const char *name, class DateTime* date)					{ m_delegate.set(name, date); }
	void append(const char *name, const char *svalue, const char *sep)  { m_delegate.append(name, svalue, sep); }

	void dump(class DM &dm)  { m_delegate.dump(dm); }

	void addRef()  { m_delegate.addRef(); }
	void releaseRef()  { m_delegate.releaseRef(); }
};


class DMAPI_API ScopeStack
{
private:
	class DM	&m_dm;
	int			 m_size;
	Scope		**m_stack;
	int			 m_top;
	bool		 m_isOwner;

public:
	ScopeStack(class DM &dm);
	ScopeStack(ScopeStack &other);
	~ScopeStack();

	void push(Scope *scope);
	/*Scope **/ void pop(const char *expect = 0);
	Scope *getScope(const char *name);

	//const char *get(const char *name);
	Variable *get(const char *name);
	Variable *getXML(const char *name, class Context &ctx);

	// Overloaded - just pass a string or an int
	// Old versions - these just set variables in the topmost scope
	//void set(const char *name, const char *svalue, bool nocase = false);
	//void set(const char *name, int ivalue);
	void set(const char *name, DMArray* arr);
	////void set(const char *name, class Object* obj);
	//void set(const char *name, class DMException& ex);
	//void set(const char *name, class ObjectReference* objref);
	//void set(const char *name, class OutputStream* ostr);
	//void append(const char *name, const char *svalue, const char *sep);		// RHT 22/02/2012 - new non-global append fn

	// Overloaded - just pass a string or an int
	// New versions - these set variables in the topmost user-writable scope
	void setLocal(const char *name, const char *svalue, bool nocase = false);
	void setLocal(const char *name, bool value);
	void setLocal(const char *name, int ivalue);
	void setLocal(const char *name, DMArray* arr);
	//void setLocal(const char *name, class Object* obj);
	void setLocal(const char *name, class DMException& ex);
	void setLocal(const char *name, class ObjectReference* objref);
	void setLocal(const char *name, class OutputStream* ostr);
	void setLocal(const char *name, class DateTime* date);
	void appendLocal(const char *name, const char *svalue, const char *sep);

	// Overloaded - just pass a string or an int
	void setGlobal(const char *name, const char *svalue, bool nocase = false);
	void setGlobal(const char *name, bool value);
	void setGlobal(const char *name, int ivalue);
	void setGlobal(const char *name, DMArray* arr);
	//void setGlobal(const char *name, class Object* obj);
	void setGlobal(const char *name, class ObjectReference* objref);
	void setGlobal(const char *name, class OutputStream* ostr);
	void setGlobal(const char *name, class DateTime* date);
	void appendGlobal(const char *name, const char *svalue, const char *sep);

	// Operations which apply to all scopes on the stack
	void appendAll(const char *name, const char *list);
	void incrementAll(const char *name, int inc);
	bool contains(const char *name);
	bool isRestricted();
	bool isFunction();
	bool isAction();

	//DMArray *newArray(const char *name, bool isList = false, bool nocase = false);
	DMArray *newLocalArray(const char *name, bool isList = false, bool nocase = false);
	DMArray *newGlobalArray(const char *name, bool isList = false, bool nocase = false);
	class OutputStream *newStream(const char *name);

	void dump();
	void trace(class IOutputStream &out, class ILocatable &loc);
	void trace();
};

#endif /*_SCOPESTACK_H*/
