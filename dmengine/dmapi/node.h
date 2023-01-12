/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __node_h

#define __node_h


#include "list.h"
#include "hashtable.h"
#include "dmapiexport.h"
#include "modelbase.h"


typedef struct tag_yyparse_param {
	class LexerBuffer		*buffer;
	union {
		class ActionNodeList	*actnlist;
		class Node				*expr;
		class StmtList			*stmtlist;
	};
	class DMException		*ex;

	tag_yyparse_param()
		: buffer((class LexerBuffer*) 0), actnlist((class ActionNodeList*) 0),
		  ex((class DMException*) 0)
	{}
} yyparse_param;


typedef struct tag_yylloc_param {
	int first_line;
	int first_column;
	int last_line;
	int last_column;
	int file_num;
	int frag_num;
} yylloc_param;

#define YYLTYPE yylloc_param


int yyparse_threadsafe_wrapper(yyparse_param *param);


#define NODE_NONE     0
#define NODE_BOOL     1
#define NODE_INT      2
#define NODE_STR      3
#define NODE_IDOROBJ  4
#define NODE_NULL     5
#define NODE_NOT     10
#define NODE_PLUS    11
#define NODE_MINUS   12
#define NODE_TIMES   13
#define NODE_DIV     14
#define NODE_MOD     15
#define NODE_AND     16
#define NODE_OR      17
#define NODE_NEG     18
#define NODE_EQ      19
#define NODE_NE      20
#define NODE_GT      21
#define NODE_GE      22
#define NODE_LT      23
#define NODE_LE      24
#define NODE_MATCH   25
#define NODE_IFSET   26
#define NODE_IFUNSET 27
#define NODE_FUNC    31
#define NODE_OBJFUNC 32
#define NODE_ARRAY   33
#define NODE_OBJECT  34
#define NODE_CONC    35
#define NODE_LOOKUP  36
#define NODE_NLVAR   37
#define NODE_ARRELE  38
#define NODE_LISTELE 39
#define NODE_ARRINI  40
#define NODE_LISTINI 41
#define NODE_OBJPTR  254
#define NODE_ARRPTR  255

/**
 * Parse tree node - represents a single operation for part of a parsed expression.
 */
class DMAPI_API Node
{
protected:
	int m_kind;
	union {
		bool m_bval;
		int m_ival;
		char *m_str;
		class IObject *m_obj;
		class DMArray *m_arr;
	};
	bool m_expandable;
	class Node *m_left;
	class Node *m_right;
	class NodeList *m_list;

public:
	Node(bool bval);
	Node(int ival);
	Node(class IObject *objptr);
	Node(class DMArray *arrptr);
	Node(int kind, char *str, bool expandable = false);
	Node(int kind, Node *left, Node *right = (Node*) 0);
	Node(int kind, char *str, NodeList *list);
	Node(int kind, Node *left, char *str, NodeList *list);
	Node(const Node &copy);
	virtual ~Node();

	int kind()  { return m_kind; }

	Node *deepCopy();

	virtual class Expr *evaluate(class Context &ctx, bool stringify = false);
	char *evaluateAsString(class Context &ctx);

	char *evalNamelist(Context &ctx);
	void setVariable(
		Node *val, bool append, bool appendAll, bool isGlobal,
		bool noCase, class Context &ctx);
	void dump();

	friend class NodeList;
	friend class CallFunctionNodeImpl;
	friend class SetStmt;
};


class DMAPI_API NodeList : public List<Node>
{
public:
	NodeList();

	NodeList *deepCopy();

	class ExprList *toExprList(Context &ctx);
	int nodeType(Context &ctx);
	void addToArray(Context &ctx, class DMArray &arr);
};


class DMAPI_API FunctionNode : public Node, public ILocatable
{
private:
	int m_fileno;
	int m_lineno;
	class FunctionNodeImpl *m_impl;

	void createImpl(class Context &ctx);

public:
	FunctionNode(YYLTYPE &loc, char *str, NodeList *list);
	~FunctionNode();

	// ILocatable
	int lineno()		{ return m_lineno; }
	int fileno()		{ return m_fileno; }
	int fragno()		{ return 0; }

	const char *name()  { return m_str; }

	class Expr *evaluate(class Context &ctx, bool stringify = false);
	class Expr *execute_function(class ActionNode *actnode, class ExprList *args, class Context &ctx);
};


#define STMT_NONE     0
#define STMT_IF       1
#define STMT_FOR      2
#define STMT_FOREACH  3
#define STMT_WHILE    4
#define STMT_SWITCH   5
#define STMT_PSLOOP   6
#define STMT_PKGLOOP  7
#define STMT_PARALLEL 8
#define STMT_USING    9
#define STMT_TRYCATCH 10
#define STMT_BREAK    11
#define STMT_CONTINUE 12
#define STMT_CASE     13
#define STMT_DEFAULT  14
#define STMT_RETURN   15
#define STMT_SET      20
#define STMT_ECHO     21
#define STMT_EVAL	  22
#define STMT_AVLOOP   23
#define STMT_EXTENDED 30
#define STMT_ARG      31


typedef enum tag_control_path_return {
	control_path_normal,
	control_path_return,
	control_path_exception
} ControlPathReturn;


class DMAPI_API Stmt : public ILocatable
{
protected:
	int m_kind;
	int m_fileno;
	int m_lineno;
	int m_fragno;
	Node *m_value;
	char *m_extended;

public:
	Stmt(YYLTYPE &loc, int kind);
	Stmt(YYLTYPE &loc, char *name, Node *value);
	Stmt(int kind);
	Stmt(char *name, Node *value);
	Stmt(const Stmt &copy);
	virtual ~Stmt();

	int kind()	  { return m_kind; }
	const char *name()  { return m_extended; }

	// ILocatable
	int lineno()  { return m_lineno; }
	int fileno()  { return m_fileno; }
	int fragno()  { return m_fragno; }

	virtual ControlPathReturn checkControlPaths(class DM &dm);
	virtual void execute(class Context &ctx);
	virtual void execute(class DMThreadList &threads, Context &ctx);
	virtual bool isThreaded()  { return false; }

	void indent(int ind);
	virtual void print(int indent);

	friend class StmtList;
	friend class ForEachStmt;
	friend class ExtendedStmt;
};


class DMAPI_API StmtList : public List<Stmt>
{
public:
	StmtList();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(Context &ctx);
	Stmt *getArg(const char *name);
};



class DMAPI_API IfStmt : public Stmt
{
private:
	Node *m_cond;
	StmtList *m_then;
	StmtList *m_else;

public:
	IfStmt(YYLTYPE &loc, Node *cond, StmtList *then_part, StmtList *else_part = (StmtList*) 0);
	~IfStmt();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(Context &ctx);
};


class DMAPI_API ForStmt : public Stmt
{
private:
	Node *m_var;
	Node *m_value;
	Node *m_cond;
	Node *m_var2;
	Node *m_value2;
	StmtList *m_body;

public:
	ForStmt(YYLTYPE &loc, Node *var, Node *value, Node *cond, Node *var2, Node *value2, StmtList *body);
	~ForStmt();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(Context &ctx);
};


class DMAPI_API ForEachStmt : public Stmt
{
private:
	Stmt *m_loop;
	StmtList *m_body;
	bool m_iterate;

public:
	ForEachStmt(YYLTYPE &loc, Stmt *loop, StmtList *body, bool iterate);
	~ForEachStmt();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(Context &ctx);
};


class DMAPI_API WhileStmt : public Stmt
{
private:
	Node *m_cond;
	StmtList *m_body;

public:
	WhileStmt(YYLTYPE &loc, Node *cond, StmtList *body);
	~WhileStmt();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(Context &ctx);
};


class DMAPI_API CaseStmt : public Stmt
{
private:
	Node *m_value;
	StmtList *m_body;

public:
	CaseStmt(YYLTYPE &loc, Node *value, StmtList *body);
	CaseStmt(YYLTYPE &loc, StmtList *body);
	~CaseStmt();

	void execute(Context &ctx);
	bool isEqual(Expr &e, Context &ctx);
};


class DMAPI_API CaseStmtList : public List<CaseStmt>
{
public:
	CaseStmtList();
};


class DMAPI_API SwitchStmt : public Stmt
{
private:
	Node *m_cond;
	CaseStmtList *m_cases;

public:
	SwitchStmt(YYLTYPE &loc, Node *cond, CaseStmtList *cases);
	~SwitchStmt();

	void execute(Context &ctx);
};


class DMAPI_API SetStmt : public Stmt
{
private:
	char *m_options;
	Node *m_var;
	Node *m_value;

public:
	SetStmt(YYLTYPE &loc, char *options, Node *var, Node *value);
	~SetStmt();

	void execute(Context &ctx);
};


class DMAPI_API EchoStmt : public Stmt
{
private:
	Node *m_exp;
	char *m_stream;

public:
	EchoStmt(YYLTYPE &loc, Node *exp, char *stream = NULL);
	~EchoStmt();

	void execute(Context &ctx);
};


class DMAPI_API TryCatchStmt : public Stmt
{
private:
	StmtList *m_body;
	char *m_var;
	StmtList *m_catch;
	StmtList *m_finally;

public:
	TryCatchStmt(YYLTYPE &loc, StmtList *body, char *var, StmtList *catch_part, StmtList *finally_part);
	~TryCatchStmt();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(Context &ctx);
};


class DMAPI_API PsLoopStmt : public Stmt
{
private:
	StmtList *m_body;

public:
	PsLoopStmt(YYLTYPE &loc, StmtList *body);
	~PsLoopStmt();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(Context &ctx);
};


class DMAPI_API ParallelStmt : public Stmt
{
private:
	StmtList *m_body;

public:
	ParallelStmt(YYLTYPE &loc, StmtList *body);
	~ParallelStmt();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(Context &ctx);
};


class DMAPI_API SequentialStmt : public Stmt
{
private:
	StmtList *m_body;

public:
	SequentialStmt(YYLTYPE &loc, StmtList *body);
	~SequentialStmt();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(Context &ctx);
};


class DMAPI_API UsingStmt : public Stmt
{
private:
	char *m_usekind;
	Node *m_exp;
	StmtList *m_body;

	void executeUsingStream(Context &ctx);
	void executeUsingDropzone(Context &ctx);
	void executeUsingComponent(Context &ctx);
	void executeUsingApplication(Context &ctx);
	void executeUsingEnvironment(Context &ctx);

public:
	UsingStmt(YYLTYPE &loc, char *usekind, Node *exp, StmtList *body);
	~UsingStmt();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(Context &ctx);
};


class DMAPI_API ReturnStmt : public Stmt
{
private:
	Node* m_exp;

public:
	ReturnStmt(YYLTYPE &loc, Node* exp = NULL);
	~ReturnStmt();

	ControlPathReturn checkControlPaths(DM &dm);
	void execute(Context &ctx);
};


class DMAPI_API EvalStmt : public Stmt
{
private:
	Node *m_exp;

public:
	EvalStmt(YYLTYPE &loc, Node *exp);
	~EvalStmt();

	void execute(Context &ctx);
};


class DMAPI_API ExtendedStmt : public Stmt
{
protected:
	char *m_ident;
	StmtList *m_args;
	StmtList *m_body;
	StmtList *m_pre;
	StmtList *m_post;
	class Action *m_action;
	class FunctionNode *m_function;
	class Node *m_node;

	class ExtendedStmtImpl *createImplFromFactory(
		class ExtendedStmtImplFactory &factory, class Context &ctx);
	class ExtendedStmtImpl *createImpl(class Context &ctx);

public:
	ExtendedStmt(YYLTYPE &loc, char *ident, StmtList *body);
	ExtendedStmt(YYLTYPE &loc, char *ident, StmtList *args, StmtList *body);
	ExtendedStmt(YYLTYPE &loc, char *ident, StmtList *args, StmtList *pre, StmtList *post);
	ExtendedStmt(char *ident, StmtList *args);
	ExtendedStmt(class Action *action);
	ExtendedStmt(class FunctionNode *fn);
	ExtendedStmt(class Node *fn);
	virtual ~ExtendedStmt();

	const char *ident()  { return m_ident; }
	bool isThreaded();

	ControlPathReturn checkControlPaths(class DM &dm);
	void execute(class Context &ctx);
	void execute(class DMThreadList &threads, Context &ctx);
	void executeBody(class Context &ctx);
	void executePre(class Context &ctx);
	void executePost(class Context &ctx);
	void print(int indent);

	virtual Node *getArgNode(const char *name);
	virtual Expr *getArg(const char *name, class Context &ctx);
	virtual StmtList *getArgs();

	Expr *getOverridableArg(
		const char *name, class ProviderObject &prov,
		bool mandatory, class Context &ctx);

	char *getArgAsString(const char *name, class Context &ctx);
	char *getOverridableArgAsString(
		const char *name, class ProviderObject &prov,
		bool mandatory, class Context &ctx);
	char *getOverridableAppendableArgAsString(
		const char *name, char absolute, char separator,
		class ProviderObject &prov, bool mandatory, class Context &ctx);

	bool getArgAsBoolean(const char *name, class Context &ctx);
	bool getOverridableArgAsBoolean(
		const char *name, class ProviderObject &prov,
		bool mandatory, class Context &ctx);

	int getArgAsInt(const char *name, class Context &ctx);

	class Dropzone &pushDropzone(bool &popdz, class Context &ctx);
	class Dropzone *getOptionalDropzone(Context &ctx);
};


class DMAPI_API ActionNode : public ILocatable
{
private:
	bool m_isFunction;
	int m_fileno;
	int m_lineno;
	char *m_name;
	NodeList *m_args;
	StmtList *m_body;

public:
	ActionNode(YYLTYPE &loc, char *name, StmtList *body);
	ActionNode(YYLTYPE &loc, char *name, NodeList *args, StmtList *body);
	~ActionNode();

	bool isFunction()		{ return m_isFunction; }
	const char *name()		{ return m_name; }

	// ILocatable
	int fileno()			{ return m_fileno; }
	int lineno()			{ return m_lineno; }
	int fragno()			{ return 0; }

	NodeList *getArgs()		{ return m_args; }

	void semanticCheck(class DM &dm);
	void execute(Context &ctx);

	friend class ActionNodeList;
};


class DMAPI_API ActionNodeList : public List<ActionNode>
{
public:
	ActionNodeList();

	ActionNode *firstActionNode();
	ActionNode *getActionNode(const char *name);

	List<ActionNode> *add(ActionNode *actnode);
	void merge(ActionNodeList &other);

	void semanticCheck(class DM &dm);
};


#endif /*__node_h*/
