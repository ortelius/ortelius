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
#ifndef __expr_h

#define __expr_h


#include "list.h"
#include "hashtable.h"

#include "model.h"
#include "dmapiexport.h"


typedef enum objref_kind_tag {
	OBJREF_STACK	 = 1,
	OBJREF_OBJECT    = 2,
	OBJREF_EXCEPTION = 3
} OBJREF_KIND;


class DMAPI_API ObjectReference
{
private:
	OBJREF_KIND m_kind;
	union {
		class Scope       *m_stack;
		class IObject     *m_obj;
		class DMException *m_ex;
	};

public:
	ObjectReference(class Scope* stack);
	ObjectReference(class IObject *obj);
	ObjectReference(class DMException &ex);
	ObjectReference(const ObjectReference &copy);
	~ObjectReference();

	class Expr *object_access(const char *str, class Context &ctx) const;
	class IObject *toIObject() const;
	class Object *toObject() const;
	OBJECT_KIND kind() const;
	char *stringify() const;
	int compare(ObjectReference &other) const;
	void print() const;
};


#define KIND_ALL    -1
#define KIND_ERROR  0
#define KIND_BOOL   1
#define KIND_INT    2
#define KIND_STR    3
#define KIND_ARRAY  4
#define KIND_OBJECT 5
#define KIND_STREAM 6
#define KIND_DATE   7


typedef Expr *(*DMExprFnPtr)(int argc, class Expr **argv, class Context &ctx);
typedef Expr *(*DMObjFnPtr)(int argc, class Expr *self, class Expr **argv, class Context &ctx);


class DMAPI_API Expr {
private:
	int m_kind;
	union {
		bool m_bval;
		int m_ival;
		char *m_str;
		class DMArray *m_arr;
		class ObjectReference *m_obj;
		class OutputStream *m_stream;
		class DateTime *m_date;
	};
	bool m_nocase;
	bool m_simple;

	DMObjFnPtr find_object_function(const char *str, int argc, Context &ctx);

public:
	Expr();
	Expr(bool bval);
	Expr(int ival);
	Expr(const char *str, bool nocase = false);
	Expr(class DMArray *arr);
	Expr(class IObject *obj);
	Expr(class Scope *scope);
	Expr(class ObjectReference *objref);
	Expr(class DateTime *date);
	Expr(int kind, const char* str);
	Expr(class Variable *var);
	Expr(const Expr &copy);
	~Expr();
	
	//Expr *operator + (const Expr &b) const;
	//Expr *operator - () const;
	//Expr *operator - (const Expr &b) const;
	//Expr *operator * (const Expr &b) const;
	//Expr *operator / (const Expr &b) const;
	//Expr *operator % (const Expr &b) const;
	//Expr *operator && (const Expr &b) const;
	//Expr *operator || (const Expr &b) const;
	Expr *operator_not(class Context &ctx) const;
	Expr *operator_plus(const Expr &b, class Context &ctx) const;
	Expr *operator_uminus(class Context &ctx) const;
	Expr *operator_minus(const Expr &b, class Context &ctx) const;
	Expr *operator_times(const Expr &b, class Context &ctx) const;
	Expr *operator_div(const Expr &b, class Context &ctx) const;
	Expr *operator_mod(const Expr &b, class Context &ctx) const;
	Expr *operator_and(const Expr &b, class Context &ctx) const;
	Expr *operator_or(const Expr &b, class Context &ctx) const;
	int compare(const Expr &b) const;
	bool matches(const Expr &b) const;
	Expr *toLower() const;
	Expr *toUpper() const;
	Expr *array_access(const Expr &a) const;
	Expr *array_length() const;
	class ExprList *array_keys() const;
	Expr *object_access(const char *str, class Context &ctx) const;
	Expr *object_function(const char *str, const class ExprList &args, class Context &ctx);
	void print() const;
	int kind() const;
	const char* kindStr() const;
	const char* objectKindStr(OBJECT_KIND okind);
	bool isSimple() const;
	const char* error() const;
	char *toString() const;
	char *stringify() const;
	int toInt() const;
	bool toBool() const;
	bool isNumber() const;
	bool isNull() const;
	class OutputStream *toStream() const;
	class DMArray *toArray() const;
	ObjectReference *toObjectReference() const;
	class DateTime *toDate() const;
	
	friend class ExprList;
	friend class SetStmt;
	friend class Expr *evaluate_function(const char *str, const class ExprList *args, class Context &ctx);

};


class DMAPI_API ExprList : public virtual List<Expr>
{	
public:
	ExprList();
	~ExprList();
	Expr **toArgv() const;
};


class DMAPI_API ExprPtr {
private:
	Expr *m_ptr;

public:
	ExprPtr();
	ExprPtr(Expr *ptr);
	~ExprPtr();

	Expr *operator =(Expr *ptr);
	Expr *operator ->();
	operator Expr*();
	Expr &operator *();
	bool operator ==(Expr *ptr) const;
	bool operator !() const;
	operator bool() const;
	bool isNull();
};


Expr *lookup_variable(const char *str, bool allowXml, class Context &ctx);
Expr *namelist_expr_or_obj(const char *str, class Context &ctx);
Expr *namelist_variable(Expr *e, class Context &ctx);
Expr *evaluate_function(const char *str, const ExprList *args, class Context &ctx);

Expr *concatenate_expr(const Expr &e1, const Expr &e2);
char *concatenate_string(const char *str1, const char *str2);

Expr *check_if_set(Expr *e1, Expr *e2);
Expr *check_if_not_set(Expr *e1, Expr *e2);

#endif /*__expr_h*/
