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
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
// For HANDLE in thread.h
#include <windows.h>
#else
#include <pthread.h>
#endif /*WIN32*/

#include "node.h"
#include "dm.h"
#include "expr.h"
#include "charptr.h"
#include "context.h"
#include "model.h"
#include "dropzone.h"
#include "scopestack.h"
#include "exceptions.h"
#include "extended.h"
#include "function.h"
#include "lexer.h"
#include "dm.tab.hpp"
#include "thread.h"
#include "autoptr.h"
#include "filenamecache.h"
#include "properties.h"
#include "engineconfig.h"


extern int yyparse(void *param);

int yyparse_threadsafe_wrapper(yyparse_param *param)
{
	int ret;
	Thread::lock(__LINE__,__FILE__);
	try {
		ret = yyparse(param);
	} catch(...) {
		Thread::unlock(__LINE__,__FILE__);
		throw;
	}
	Thread::unlock(__LINE__,__FILE__);
	return ret;
}


/**
 * Returns the first occurence of any character in the set of characters
 * represented by ifs.  Any sections that start with a double- or single-
 * quote are only inspected for ifs characters after the closing quote.
 * For example:
 * 1 "2 3" 4 returns three tokens - "1" "2 3" and "4".
 */
char *getFirstIFS(const char *str, const char *ifs)
{
	char *ret = NULL;

	// If we start with a double or single-quote, find the matching quote and
	// start looking for a separator from there.
	if((str[0] == '"') || (str[0] == '\'')) {
		for(int m = 1; str[m]; m++) {
			if(str[m] == str[0]) {
				str = &str[m];
				break;
			}
		}
	}

	// Check each separator in turn and find the nearest to the start.
	for(int n = 0; ifs[n]; n++) {
		char *pos = (char*) strchr(str, ifs[n]);

		if(!ret || (ret < pos)) { ret = pos; }
	}

	return ret;
}


///////////////////////////////////////////////////////////////////////////////
// Node
///////////////////////////////////////////////////////////////////////////////

Node::Node(bool bval)
	: m_kind(NODE_BOOL), m_bval(bval), m_expandable(false),
	  m_left(NULL), m_right(NULL), m_list(NULL)
{}


Node::Node(int ival)
	: m_kind(NODE_INT), m_ival(ival), m_expandable(false),
	  m_left(NULL), m_right(NULL), m_list(NULL)
{}


Node::Node(IObject *objptr)
	: m_kind(NODE_OBJPTR), m_obj(objptr), m_expandable(false),
	  m_left(NULL), m_right(NULL), m_list(NULL)
{}


Node::Node(class DMArray *arrptr)
	: m_kind(NODE_ARRPTR), m_arr(arrptr), m_expandable(false),
	  m_left(NULL), m_right(NULL), m_list(NULL)
{}


Node::Node(int kind, char *str, bool expandable /*= false*/)
	: m_kind(kind), m_str(str), m_expandable(expandable),
	  m_left(NULL), m_right(NULL), m_list(NULL)
{}


Node::Node(int kind, Node *left, Node *right /*= NULL*/)
	: m_kind(kind), m_str(NULL), m_expandable(false),
	  m_left(left), m_right(right), m_list(NULL)
{}


Node::Node(int kind, char *str, NodeList *list)
	: m_kind(kind), m_str(str), m_expandable(false),
	  m_left(NULL), m_right(NULL), m_list(list)
{}


Node::Node(int kind, Node *left, char *str, NodeList *list)
	: m_kind(kind), m_str(str), m_expandable(false),
	  m_left(left), m_right(NULL), m_list(list)
{}


Node::Node(const Node &copy)
	: m_kind(copy.m_kind), m_expandable(copy.m_expandable),
	  m_left(NULL), m_right(NULL), m_list(NULL)
{
	switch(copy.m_kind) {
	case NODE_BOOL:
		m_bval = copy.m_bval;
		break;
	case NODE_INT:
		m_ival = copy.m_ival;
		break;
	case NODE_OBJPTR:
		m_obj = copy.m_obj;
		break;
	case NODE_ARRPTR:
		m_arr = copy.m_arr;
		break;
	default:
		m_str = DUP_NULL(copy.m_str);
		break;
	}
}


/*virtual*/ Node::~Node()
{
	SAFE_DELETE(m_left);
	SAFE_DELETE(m_right);
	SAFE_DELETE(m_list);

	// m_str is in a union, so need to be sure of the type before we free it
	switch(m_kind) {
	case NODE_STR:		case NODE_IDOROBJ:
	case NODE_FUNC:		case NODE_OBJFUNC:
	case NODE_LOOKUP:
		SAFE_FREE(m_str);
		break;
	case NODE_OBJPTR:
		m_obj = NULL;
		break;
	case NODE_ARRPTR:
		m_arr = NULL;
		break;
	}
}


Node *Node::deepCopy()
{
	Node *ret = new Node(*this);
	if(m_left) {
		ret->m_left = m_left->deepCopy();
	}
	if(m_right) {
		ret->m_right = m_right->deepCopy();
	}
	if(m_list) {
		ret->m_list = m_list->deepCopy();
	}
	return ret;
}


char *substr(const char *a, const char *b)
{
	if(a > b) {
printf("### substr failed\n");
		return strdup("");
	}

	int len = b - a;

	char *ret = (char*) malloc(len + 1);
	if (ret) {
		char *c = ret;
		while((a < b) && *a) {
			*c = *a;
			a++;
			c++;
		}
		*c = '\0';
	}
	return ret;
}


/**
 * Expands a string containing embedded variables.  For any variables in braces
 * we actually use the parser again and make sure we have a completely valid
 * expression.  Note that expansion is not destructive, as it uses the above
 * substr function to pull out substrings to work on.
 */
Expr *expandIt(const char *str, Context &ctx)
{
	// Trap empty strings as code below does not handle them
	if(strlen(str) == 0) {
		return new Expr("");
	}

	Node *tree = NULL;
	const char *x;
	const char *z = str;
	for(x = str; x && *x; x++) {
		if((*x == '\\') && (*(x+1) == '$') && (*(x+2) != '{')) {
			// Escaped dollar - replace with '$' and skip expansion
			if(x != z) {
				Node *temp = new Node(NODE_STR, substr(z, x));
				tree = tree ? new Node(NODE_CONC, tree, temp) : temp;
			}
			x++;
			z = x;
			continue;
		}
		if(*x == '$') {
			// If there is any plain text since the last var or start of string
			if(x != z) {
				Node *temp = new Node(NODE_STR, substr(z, x));
				tree = tree ? new Node(NODE_CONC, tree, temp) : temp;
			}

			const char *v = x;
			v++;

			Node *temp = NULL;

			const char *y = v;
			if(!*v) {
				// end of string - just a safety check
				break;
			} else if(isalpha(*v)) {
				// simple identifier
				for(y = v; y && *y; y++) {
					if(!isalnum(*y) && (*y != '_')) {
						break;
					}
				}
// printf("v1 = '%s'\n", substr(v, y));
				temp = new Node(NODE_LOOKUP, substr(v, y));
			} else if(*v == '{') {
				// expression in braces - find matching brace and evaluate
				int braces = 1;
				v++;
				for(y = v; y && *y; y++) {
					if(*y == '{') { braces++; }
					if(*y == '}') {
						braces--;
						if(braces <= 0) {
							y++;	// take us past the closing brace
							break;
						}
					}
				}
//printf("v2 = '%s'\n", substr(x,y));

				// Use the parser to parse the complete ${...} expression
				ConstCharPtr input = substr(x, y);
				LexerBuffer lb(expr_lexer, input, NULL);

				yyparse_param param;
				param.buffer = &lb;
				int res = yyparse_threadsafe_wrapper(&param);
				if((res == 0) && param.expr) {
					temp = param.expr;
					//temp->dump(); printf("\n");
				} else {
					if(param.ex) {
						param.ex->print(ctx.dm());
					}
					temp = new Node(NODE_STR, substr(x, y));
				}
			} else if(*v == '(') {
				// expression in parenthesis - find matching bracket and evaluate as expression
				int parens = 1;
				v++;
				for(y = v; y && *y; y++) {
					if(*y == '(') { parens++; }
					if(*y == ')') {
						parens--;
						if(parens <= 0) {
							y++;	// take us past the closing brace
							break;
						}
					}
				}
//printf("v2 = '%s'\n", substr(x+2,y-1));

				// Use the parser to parse what is between the brackets as an expression
				ConstCharPtr input = substr(x+2, y-1);
				LexerBuffer lb(expr_lexer, input, NULL);

				yyparse_param param;
				param.buffer = &lb;
				int res = yyparse_threadsafe_wrapper(&param);
				if((res == 0) && param.expr) {
					temp = param.expr;
					//temp->dump(); printf("\n");
				} else {
					if(param.ex) {
						param.ex->print(ctx.dm());
					}
					temp = new Node(NODE_STR, substr(x, y));
				}
			} else if((*v == '$') || (*v == '?') || (*v == '!')) {
				// special variable - $$, $? or $!
				temp = new Node(NODE_LOOKUP, substr(v, v+1));
				y = v+1;
			} else {
				// lone $ - pretend we didn't see it
				continue;
			}
			
			tree = tree ? new Node(NODE_CONC, tree, temp) : temp;
			x = z = y;

			// If the last expansion was at the end of the string, y may
			// pointing to the null, so re-check we haven't reached the
			// end of what we are parsing
			if(!*x) {
				break;
			}

			// Decrement x here, as loop will increment it again immediately
			x--;
		}
	}
	if(x != z) {
		Node *temp = new Node(NODE_STR, strdup(z));
		tree = tree ? new Node(NODE_CONC, tree, temp) : temp;
	}
	//tree->dump(); printf("\n");
	Expr *result = tree ? tree->evaluate(ctx, true) : NULL;		// TODO: This could raise an exception - need to perform delete so try catch(...) throw???
	SAFE_DELETE(tree);
	return result;
}


Expr *Node::evaluate(Context &ctx, bool stringify /*= false*/)
{
	switch(m_kind) {
	case NODE_BOOL:
		return new Expr(m_bval);
	case NODE_INT:
		return new Expr(m_ival);
	case NODE_STR:
		if(m_expandable) {
			try {
				return expandIt(m_str, ctx);
			} catch(DMException &e) {
				e.print(ctx);
				return new Expr(m_str);
			}
		}
		return new Expr(m_str);
	case NODE_IDOROBJ:
		return namelist_expr_or_obj(m_str, ctx);
	case NODE_NULL:
		return new Expr((IObject*) 0);
	// binary operators
	case NODE_PLUS:  case NODE_MINUS: case NODE_TIMES:
	case NODE_DIV:   case NODE_MOD:   case NODE_AND:
	case NODE_OR:    case NODE_EQ:    case NODE_NE:
	case NODE_GT:    case NODE_GE:    case NODE_LT:
	case NODE_LE:    case NODE_MATCH: case NODE_ARRAY: {
		ExprPtr l = m_left->evaluate(ctx), r = m_right->evaluate(ctx);
		switch(m_kind) {
		case NODE_PLUS:  return l->operator_plus(*r, ctx);	//*l + *r;
		case NODE_MINUS: return l->operator_minus(*r, ctx);	//*l - *r;
		case NODE_TIMES: return l->operator_times(*r, ctx);	//*l * *r;
		case NODE_DIV:   return l->operator_div(*r, ctx);	//*l / *r;
		case NODE_MOD:   return l->operator_mod(*r, ctx);	//*l % *r;
		case NODE_AND:   return l->operator_and(*r, ctx);	//*l && *r;
		case NODE_OR:    return l->operator_or(*r, ctx);	//*l || *r;
		case NODE_EQ:    return new Expr(l->compare(*r) == 0);
		case NODE_NE:    return new Expr(l->compare(*r) != 0);
		case NODE_GT:    return new Expr(l->compare(*r) == 1);
		case NODE_GE:    return new Expr(l->compare(*r) >= 0);
		case NODE_LT:    return new Expr(l->compare(*r) == -1);
		case NODE_LE:    return new Expr(l->compare(*r) <= 0);
		case NODE_MATCH: return new Expr(l->matches(*r));
		case NODE_ARRAY: return l->array_access(*r);
		default:         return new Expr(KIND_ERROR, "Unhandled binary operator");
		}
		break;
		}	
	case NODE_NEG: {
		ExprPtr l = m_left->evaluate(ctx);
		return l->operator_uminus(ctx);	//-(*l);
		}
	case NODE_NOT: {
		ExprPtr l = m_left->evaluate(ctx);
		return l->operator_not(ctx);
		}
	case NODE_IFSET:
		return check_if_set(m_left->evaluate(ctx, stringify), m_right->evaluate(ctx, stringify));
	case NODE_IFUNSET:
		return check_if_not_set(m_left->evaluate(ctx, stringify), m_right->evaluate(ctx, stringify));
	case NODE_OBJFUNC: {
		// can only ever be a variable if it is on the left - reset to lookup
		if(m_left->m_kind == NODE_IDOROBJ) { m_left->m_kind = NODE_LOOKUP; }
		ExprPtr l = m_left->evaluate(ctx);
		ExprList *args = m_list ? m_list->toExprList(ctx) : new ExprList();
		Expr *ret = l->object_function(m_str, *args, ctx);
		SAFE_DELETE(args);
		return ret;
		}
	case NODE_OBJECT: {
		// can only ever be an object if it is on the left - reset the right-hand kind to a plain ident
		if(m_right->m_kind == NODE_IDOROBJ) { m_right->m_kind = NODE_STR; }
		ExprPtr l = m_left->evaluate(ctx);
		ExprPtr r = m_right->evaluate(ctx);
		ConstCharPtr rs = r->toString();
		return l->object_access(rs, ctx);
		}
	case NODE_CONC: {
		ExprPtr l = m_left->evaluate(ctx, stringify), r = m_right->evaluate(ctx, stringify);
		ConstCharPtr ls = stringify ? l->stringify() : l->toString();
		ConstCharPtr rs = stringify ? r->stringify() : r->toString();
		ConstCharPtr rets = concatenate_string(ls, rs);
		return new Expr((const char*) rets);
		}
	case NODE_LOOKUP:
		return lookup_variable(m_str, true, ctx);
	case NODE_NLVAR:
		// namelist_variable deletes the expr if it would be lost
		return namelist_variable(m_left->evaluate(ctx), ctx);
	case NODE_ARRELE:
		return new Expr(KIND_ERROR, "Array Element not yet implemented");	// TODO: Implement me
	case NODE_LISTELE:
		return new Expr(KIND_ERROR, "List Element not yet implemented");	// TODO: Implement me
	case NODE_ARRINI:
	case NODE_LISTINI: {
		// TODO: Need to record this unnamed array somewhere so we can delete it
		DMArray *arr = new DMArray((m_kind == NODE_LISTINI) ? true : false, false, true);
		m_list->addToArray(ctx, *arr);
		return new Expr(arr);
		}
	case NODE_OBJPTR:
		return new Expr(m_obj);
	case NODE_ARRPTR:
		return new Expr(m_arr);
	case NODE_FUNC:	// polymorphic - should never be called
	default:
		throw RuntimeError(ctx.stack(), "Node of kind %d not yet implemented", m_kind);
	}
}


char *Node::evaluateAsString(class Context &ctx)
{
	ExprPtr e = evaluate(ctx);
	return e ? e->toString() : NULL;
}


char *Node::evalNamelist(Context &ctx)
{
	switch(m_kind) {
	case NODE_IDOROBJ:
		//printf("evalNamelist: namelist_expr_or_obj('%s')\n", m_str);
		// Is this an object?
		if(ctx.stack().getScope(m_str)) {
			throw RuntimeError(ctx.stack(), "Objects not valid in namelists");
		}
		return strdup(m_str);
	case NODE_LOOKUP:
		// Added for UsingStmt
		return strdup(m_str);
	case NODE_NLVAR:
		//printf("evalNamelist: namelist_variable\n");
		{
		ExprPtr e = m_left->evaluate(ctx);
		if(e->isSimple() && (e->kind() == KIND_STR)) {
			return e->toString();
		}
		throw RuntimeError(ctx.stack(), "Not a valid nlvar namelist result (kind %d)", e->kind());
		}
		break;
	case NODE_CONC: {
		//printf("evalNamelist: concatenate_string\n");
		ExprPtr l = m_left->evaluate(ctx), r = m_right->evaluate(ctx);
		ConstCharPtr ls = l->toString(), rs = r->toString();
		return concatenate_string(ls, rs);
		}
	default:
		throw RuntimeError(ctx.stack(), "Not a valid namelist (kind %d)", m_kind);
	}

	return NULL;
}


typedef enum tagSetType {
	SET_ERROR = 0,
	SET_STRING,
	SET_ARRAY_VAL,
	SET_ARRAY_REF,
	SET_OBJECT_REF,
	SET_STREAM_REF,
	SET_DATE,
	SET_INTEGER,
	SET_BOOLEAN
} SetType;


void Node::setVariable(
	Node *value, bool append, bool appendAll, bool isGlobal,
	bool noCase, class Context &ctx)
{
	ExprPtr eval;
	ConstCharPtr val;
	int ival;
	NodeList *arrVal = NULL;
	ObjectReference *objRef = NULL;
	class DMArray *arrRef = NULL;
	class OutputStream *strRef = NULL;
	class DateTime *dateVal = 0;

	SetType kind = SET_ERROR;

	// TODO: This is only temporary until we find a way to set nocase and
	// append for the evaluate of these
	if((value->kind() == NODE_ARRINI) || (value->kind() == NODE_LISTINI)) {
		kind = SET_ARRAY_VAL;
		arrVal = value->m_list;
	} else {
		eval = value->evaluate(ctx);
		if (eval) {
			switch(eval->kind()) {
			case KIND_BOOL:
				kind = SET_BOOLEAN;
				ival = eval->toInt();
				break;
			case KIND_OBJECT:
				debug3("set to object detected");
				kind = SET_OBJECT_REF;
				objRef = eval->toObjectReference();
				break;
			case KIND_ARRAY:
				debug3("set to array detected");
				kind = SET_ARRAY_REF;
				arrRef = eval->toArray();
				break;
			case KIND_STREAM:
				debug3("set to stream detected");
				kind = SET_STREAM_REF;
				strRef = eval->toStream();
				break;
			case KIND_DATE:
				debug3("set to date detected");
				kind = SET_DATE;
				dateVal = eval->toDate();
				break;
			case KIND_INT:
				// debug3("set to integer detected");
				kind = SET_INTEGER;
				ival = eval->toInt();
				break;
			default:
				// debug3("set to string detected");
				kind = SET_STRING;	
				val = eval->toString();
				break;
			}
		}
	}

	ConstCharPtr separator = strdup("");
	if(val && append) {
		// If we are appending text, then check the stack for the SEPARATOR
		Variable *sep = ctx.stack().get("SEPARATOR");
		if(sep) {
			char *newsep = sep->toString();
			if (newsep) {
				separator = newsep;
			}
		}
	}

	switch(m_kind) {
	case NODE_IDOROBJ:
	case NODE_CONC: {
		//printf("setVariable: namelist_expr_or_obj or concatenate\n");
		ConstCharPtr varname = evalNamelist(ctx);
		//printf("varname = '%s'\n", (const char*) varname);

		// Do a simple set on the variable
		switch(kind) {
		case SET_BOOLEAN:
			if(append) {
				char sval[32];
				sprintf(sval,"%s",ival?"true":"false");
				if(isGlobal) {
					ctx.stack().appendGlobal(varname, sval, separator /*, noCase*/);		// TODO: Provide some way of setting no-case on append
				} else {
					ctx.stack().appendLocal(varname, sval, separator /*, noCase*/);		// TODO: Provide some way of setting no-case on append
				}
			} else {
				if(isGlobal) {
					ctx.stack().setGlobal(varname, ival?true:false);
				} else {
					ctx.stack().setLocal(varname, ival?true:false);
				}
			}
			break;
		case SET_INTEGER:
			if(append) {
				char sval[32];
				sprintf(sval,"%d",ival);
				if(isGlobal) {
					ctx.stack().appendGlobal(varname, sval, separator /*, noCase*/);		// TODO: Provide some way of setting no-case on append
				} else {
					ctx.stack().appendLocal(varname, sval, separator /*, noCase*/);		// TODO: Provide some way of setting no-case on append
				}
			} else {
				if(isGlobal) {
					ctx.stack().setGlobal(varname, ival);
				} else {
					ctx.stack().setLocal(varname, ival);
				}
			}
			break;
		case SET_STRING:
			if(val) {
				if(append) {
					if(isGlobal) {
						ctx.stack().appendGlobal(varname, val, separator /*, noCase*/);		// TODO: Provide some way of setting no-case on append
					} else {
						ctx.stack().appendLocal(varname, val, separator /*, noCase*/);		// TODO: Provide some way of setting no-case on append
					}
				} else {
					if(isGlobal) {
						ctx.stack().setGlobal(varname, val, noCase);
					} else {
						ctx.stack().setLocal(varname, val, noCase);
					}
				}
			}
			break;
		case SET_ARRAY_VAL:
			if(arrVal) {
				if(append) {
					// TODO: append global arrays?
					Variable *var = ctx.stack().get(varname);
					if(var && (var->type() == vartype_array)) {
						DMArray *arr = var->getArray();
						if(arr) {
							arrVal->addToArray(ctx, *arr);
						}
					} else {
						throw RuntimeError(ctx.stack(), "Variable '%s' is not an array", (const char*) varname);
					}
				} else {
					int kind = arrVal->nodeType(ctx);
					DMArray *arr = isGlobal
						? ctx.stack().newGlobalArray(varname, (kind == NODE_LISTELE) ? true : false, noCase)
						: ctx.stack().newLocalArray(varname, (kind == NODE_LISTELE) ? true : false, noCase);

					if(arr) {
						arrVal->addToArray(ctx, *arr);
					}
				}
			}
			break;
		case SET_OBJECT_REF:
			// null is allowed here
			if(isGlobal) {
				ctx.stack().setGlobal(varname, objRef);
			} else {
				ctx.stack().setLocal(varname, objRef);
			}
			break;
		case SET_ARRAY_REF:
			if(arrRef) {
				if(append) {
					//TODO: Implement me
					debug1("Array append - not yet implemented");
				}
				if(appendAll) {
					Variable *var = ctx.stack().get(varname);
					if(var && (var->type() == vartype_array)) {
						DMArray *arr = var->getArray();
						if(arr) { arr->addAll(*arrRef); }
					} else {
						throw RuntimeError(ctx.stack(), "Variable '%s' is not an array", (const char*) varname);
					}
				} else {
					if(isGlobal) {
						ctx.stack().setGlobal(varname, arrRef);
					} else {
						ctx.stack().setLocal(varname, arrRef);
					}
				}
			} else {
				if(isGlobal) {
					ctx.stack().setLocal(varname, (ObjectReference *)0);
				} else {
					ctx.stack().setLocal(varname, (ObjectReference *)0);
				}
			}
			break;
		case SET_STREAM_REF:
			if(strRef) {
				if(isGlobal) {
					ctx.stack().setGlobal(varname, strRef);
				} else {
					ctx.stack().setLocal(varname, strRef);
				}
			} else {
				if(isGlobal) {
					ctx.stack().setLocal(varname, (ObjectReference *)0);
				} else {
					ctx.stack().setLocal(varname, (ObjectReference *)0);
				}
			}
			break;
		case SET_DATE:
			if(dateVal) {
				if(isGlobal) {
					ctx.stack().setGlobal(varname, dateVal);
				} else {
					ctx.stack().setLocal(varname, dateVal);
				}
			} else {
				if(isGlobal) {
					ctx.stack().setLocal(varname, (ObjectReference *)0);
				} else {
					ctx.stack().setLocal(varname, (ObjectReference *)0);
				}
			}
			break;
		default:
			throw RuntimeError(ctx.stack(), "Unhandle set kind (%d)", kind);
		}}
		break;
	case NODE_ARRAY: {
		//printf("setVariable: array_access\n");
		ExprPtr sub = m_right->evaluate(ctx);
		ConstCharPtr subs = sub->toString();
		//printf("setVariable: (1) array_access(???, '%s')\n", (const char*) subs);
		if(strlen((const char*) subs) == 0) {
			throw RuntimeError(ctx.stack(), "Empty array subscript");
		}
		ConstCharPtr arrname = m_left->evalNamelist(ctx);
		//printf("setVariable: (2) array_access('%s', '%s')\n", (const char*) arrname, (const char*) subs);
		Variable *var = ctx.stack().get(arrname);
		if(!var) {
			if(isGlobal) {
				ctx.stack().newGlobalArray(arrname, false, noCase);
			} else {
				ctx.stack().newLocalArray(arrname, false, noCase);
			}
			var = ctx.stack().get(arrname);
		}

		if(var->type() != vartype_array) {
			throw RuntimeError(ctx.stack(), "not an array");
		}

		DMArray *arr = var->getArray();
		if(!arr) { throw RuntimeError(ctx.stack(), "array was null"); }
		switch(kind) {
		case SET_STRING:
			if(val) {
				if(append) {
					Variable *oldval = arr->get(subs);
					if(oldval) {
						ConstCharPtr oldvalstr = oldval->toString();
						if(oldvalstr) {
							char *newval = (char*) malloc(strlen(oldvalstr) + strlen(separator) + strlen(val) + 1);
							sprintf(newval, "%s%s%s", (const char*) oldvalstr, (const char*) separator, (const char*) val);
							oldval->setValue(newval);
							free(newval);	// setValue will strdup
						} else {
							oldval->setValue((const char *)val);
						}
					} else {
						arr->put(subs, new Variable(NULL, (const char *)val));
					}
				} else {
					arr->put(subs, new Variable(NULL, (const char *)val));
				}
			}
			break;
		case SET_ARRAY_VAL:
			if(arrVal) {
				int kind = arrVal->nodeType(ctx);
				DMArray *newarr = new DMArray((kind == NODE_LISTELE) ? true : false, noCase, true);
				arrVal->addToArray(ctx, *newarr);
				arr->put(subs, new Variable(NULL, newarr));
			}
			break;
		case SET_OBJECT_REF:
			// null is allowed here
			arr->put(subs, new Variable(NULL, objRef));
			break;
		case SET_INTEGER:
			arr->put(subs, new Variable(NULL, ival));
			break;
		default:
			throw RuntimeError(ctx.stack(), "Unhandle set array kind (%d)", kind);
		}}
		break;

	default:
		throw RuntimeError(ctx.stack(), "Not a valid set namelist (kind %d)", m_kind);
	}
}


void Node::dump()
{
	switch(m_kind) {
	case NODE_INT: printf("%d ", m_ival); break;
	case NODE_STR: printf("'%s' ", m_str); break;
	case NODE_IDOROBJ: printf("namelist_expr_or_obj('%s') ", m_str); break;
	case NODE_NULL: printf("null "); break;
	case NODE_NOT: printf("! "); m_left->dump(); break;
	case NODE_PLUS: m_left->dump(); printf("+ "); m_right->dump(); break;
	case NODE_MINUS: m_left->dump(); printf("- "); m_right->dump(); break;
	case NODE_TIMES: m_left->dump(); printf("* "); m_right->dump(); break;
	case NODE_DIV: m_left->dump(); printf("/ "); m_right->dump(); break;
	case NODE_MOD: m_left->dump(); printf("% "); m_right->dump(); break;
	case NODE_AND: m_left->dump(); printf("& "); m_right->dump(); break;
	case NODE_OR: m_left->dump(); printf("| "); m_right->dump(); break;
	case NODE_NEG: printf("- "); m_left->dump(); break;
	case NODE_EQ: m_left->dump(); printf("= "); m_right->dump(); break;
	case NODE_NE: m_left->dump(); printf("!= "); m_right->dump(); break;
	case NODE_GT: m_left->dump(); printf("> "); m_right->dump(); break;
	case NODE_GE: m_left->dump(); printf(">= "); m_right->dump(); break;
	case NODE_LT: m_left->dump(); printf("< "); m_right->dump(); break;
	case NODE_LE: m_left->dump(); printf("<= "); m_right->dump(); break;
	case NODE_MATCH: m_left->dump(); printf("~ "); m_right->dump(); break;
	case NODE_IFSET: m_left->dump(); printf(":- "); m_right->dump(); break;
	case NODE_IFUNSET: m_left->dump(); printf(":+ "); m_right->dump(); break;
	case NODE_FUNC: printf("%s(...) "); break;
	case NODE_OBJFUNC: m_left->dump(); printf(". %s(...) ", m_str); break;
	case NODE_ARRAY: m_left->dump(); printf("[ "); m_right->dump(); printf("] "); break;
	case NODE_OBJECT: m_left->dump(); printf(". "); m_right->dump(); break;
	case NODE_CONC: printf("concatenate_string("); m_left->dump(); printf(", "); m_right->dump(); printf(") "); break;
	case NODE_LOOKUP: printf("lookup('%s') ", m_str); break;
	case NODE_NLVAR: printf("namelist_variable("); m_left->dump(); printf(") "); break;
	case NODE_ARRELE: m_left->dump(); printf("=> "); m_right->dump(); break;
	case NODE_LISTELE: m_left->dump(); break;
	default: printf("<UNKNOWN> "); break;
	}
}


///////////////////////////////////////////////////////////////////////////////
// NodeList
///////////////////////////////////////////////////////////////////////////////

NodeList::NodeList()
	: List<Node>(true)
{}


NodeList *NodeList::deepCopy()
{
	NodeList *ret = new NodeList();
	ListIterator<Node> iter(*this);
	for(Node *n = iter.first(); n; n = iter.next()) {
		ret->add(n->deepCopy());
	}
	return ret;
}


ExprList *NodeList::toExprList(Context &ctx)
{
	ExprList *ret = new ExprList();
	ListIterator<Node> iter(*this);
	for(Node *n = iter.first(); n; n = iter.next()) {
		ret->add(n->evaluate(ctx));
	}
	return ret;
}


int NodeList::nodeType(Context &ctx)
{
	// Scan the list to determine the type of the elements we are adding
	int kind = 0;
	ListIterator<Node> iter(*this);
	for(Node *n = iter.first(); n; n = iter.next()) {
		if(!kind) {
			kind = n->kind();
		} else if(kind != n->kind()) {
			throw SyntaxError(ctx.stack(),
				"Array/list initializer not allowed to contain mixed array/list elements");
		}
	}
	return kind;
}


void NodeList::addToArray(Context &ctx, DMArray &arr)
{
	// Nothing to append, so no need to do anything
	if(size() == 0) {
		return;
	}

	// Scan the list to determine the type of the elements we are adding
	int kind = nodeType(ctx);

	// Throw out anything we don't understand
	if((kind != NODE_ARRELE) && (kind != NODE_LISTELE)) {
		throw SyntaxError(ctx.stack(),
			"Invalid array/list initializer element of kind %d", kind);
	}

	// For lists, determine the max index currently in the hashtable
	int maxindex = arr.maxIndex();

	// Now add the elements
	ListIterator<Node> iter(*this);
	for(Node *n = iter.first(); n; n = iter.next()) {
		switch(kind) {
		case NODE_ARRELE: {
			ExprPtr nl = n->m_left->evaluate(ctx);
			ConstCharPtr key = nl->toString();
			ExprPtr value = n->m_right->evaluate(ctx);

			if((value->kind() == KIND_STR) || /* (value->kind() == KIND_INT) || (value->kind() == KIND_BOOL)  || */ (value->kind() == KIND_ERROR)) {
				if(value->kind() == KIND_ERROR) {
					debug1("Error is '%s'", value->error());
				}
				ConstCharPtr vs = value->toString();
				arr.put(key, new Variable(NULL, (const char *)vs));
			} else if(value->kind() == KIND_INT) {
				arr.put(key, new Variable(NULL, value->toInt()));
			} else if(value->kind() == KIND_BOOL) {
				arr.put(key, new Variable(NULL, value->toBool()));
			} else if(value->kind() == KIND_ARRAY) {
				arr.put(key, new Variable(NULL, value->toArray()));
			} else if(value->kind() == KIND_OBJECT) {
				arr.put(key, new Variable(NULL, value->toObjectReference()));
			} else if(value->kind() == KIND_DATE) {
				arr.put(key, new Variable(NULL, value->toDate()->toString()));
			} else {
				throw SyntaxError(ctx.stack(),
					"Unable to handle array element of kind %s", value->kindStr());	// TODO: Handle streams and dates
			}
			}
			break;
		case NODE_LISTELE: {
			char key[32];
			sprintf(key, "%d", ++maxindex);
			ExprPtr value = n->m_left->evaluate(ctx);
			if((value->kind() == KIND_STR) ||  /* (value->kind() == KIND_INT) || (value->kind() == KIND_BOOL) || */ (value->kind() == KIND_ERROR)) {
				if(value->kind() == KIND_ERROR) {
					debug1("Error is '%s'", value->error());
				}
				ConstCharPtr vs = value->toString();
				arr.put(key, new Variable(NULL, (const char *)vs));
			} else if(value->kind() == KIND_INT) {
				arr.put(key, new Variable(NULL, value->toInt()));
			} else if(value->kind() == KIND_BOOL) {
				arr.put(key, new Variable(NULL, value->toBool()));
			} else if(value->kind() == KIND_ARRAY) {
				arr.put(key, new Variable(NULL, value->toArray()));
			} else if(value->kind() == KIND_OBJECT) {
				arr.put(key, new Variable(NULL, value->toObjectReference()));
			} else if(value->kind() == KIND_DATE) {
				arr.put(key, new Variable(NULL, value->toDate()->toString()));
			} else throw SyntaxError(ctx.stack(),
					"Unable to handle list element of kind %s", value->kindStr());	// TODO: Handle streams and dates
			}
			break;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// CallFunctionNodeImpl
///////////////////////////////////////////////////////////////////////////////

class CallFunctionNodeImpl : public virtual FunctionNodeImpl
{
private:
	class FunctionNode &m_parent;
	class ActionNode *m_actionNode;

public:
	CallFunctionNodeImpl(class FunctionNode &parent, class ActionNode *action);

	Expr *evaluate(class ExprList *args, class Context &ctx, bool stringify);
};


CallFunctionNodeImpl::CallFunctionNodeImpl(FunctionNode &parent, ActionNode *action)
	: m_parent(parent), m_actionNode(action)
{}


Expr *CallFunctionNodeImpl::evaluate(ExprList *args, Context &ctx, bool stringify)
{
	// Check that we have a function and not an action
	if(!m_actionNode->isFunction()) {
		throw SyntaxError(m_parent,
			"Action '%s' cannot be called as a function", m_parent.name());
	}

	int argc = args ? args->size() : 0;

	// Check we have the correct number of arguments
	NodeList *formalArgs = m_actionNode->getArgs();
	int formalArgc = formalArgs ? formalArgs->size() : 0;
	if(argc != formalArgc) {
		throw SyntaxError(m_parent,
			"Function '%s' expects %d args", m_parent.name(), formalArgc);
	}

	// Create a new scope for the function
	Scope *funcscope = new Scope(FUNCTION_SCOPE, m_actionNode, m_parent, true);

	// If there are any arguments, set them as variables inside the new scope
	if(formalArgc) {
		ListIterator<Node> ait(*formalArgs);
		ListIterator<Expr> eit(*args);
		Expr *e = eit.first();
		for(Node *a = ait.first(); a; a = ait.next()) {
			if(!e) { 
				throw SyntaxError(m_parent,
					"Missing argument '%s' to function '%s'", m_parent.name(), a->m_str);
			}
			switch(e->kind()) {
			case KIND_OBJECT: {
				ObjectReference* oref = e->toObjectReference();
				if(oref) {
					funcscope->set(a->m_str, oref);
				}
				}
				break;
			case KIND_ARRAY:
				funcscope->set(a->m_str, e->toArray());
				break;
			default: {
				ConstCharPtr value = e->toString();
				if(value) {
					debug3("Setting arg '%s' to '%s'", a->m_str, (const char*) value);
					funcscope->set(a->m_str, (const char*) value);
				}
				}
				break;
			}
			e = eit.next();
		}
	}

	ctx.stack().push(funcscope);

	// Execute the function, looking out for exceptions
	try {
		m_actionNode->execute(ctx);
	} catch(ReturnException &e) {
		ctx.stack().pop(FUNCTION_SCOPE);
		if(e.expr()) {
			return new Expr(*e.expr());
		}
		throw RuntimeError(ctx.stack(), "Function '%s' failed to return a value", m_parent.name());
	} catch(...) {
		ctx.stack().pop(FUNCTION_SCOPE);
		throw;
	}
	ctx.stack().pop(FUNCTION_SCOPE);
	throw RuntimeError(ctx.stack(), "Missing return from function '%s'", m_parent.name());
}


///////////////////////////////////////////////////////////////////////////////
// FunctionNode
///////////////////////////////////////////////////////////////////////////////

FunctionNode::FunctionNode(YYLTYPE &loc, char *str, NodeList *list)
	: Node(NODE_FUNC, str, list), m_fileno(loc.file_num),
	  m_lineno(loc.first_line), m_impl(NULL)
{}


FunctionNode::~FunctionNode()
{
	SAFE_DELETE(m_impl);
}


void FunctionNode::createImpl(Context &ctx)
{
	// Check plugin-defined functions
	FunctionNodeImplFactory *factory = FunctionNodeImplRegistry::instance().getFactory(m_str);
	if(factory) {
		m_impl = factory->create(ctx.dm(), *this);
		return;
	}

	// Check user-defined functions
	ActionNode *actnode = ctx.dm().getActionNode(m_str);
	if(actnode) {
		m_impl = new CallFunctionNodeImpl(*this, actnode);
		return;
	}

	// Not built-in or script function, check repository and database functions
	Action *action = ctx.dm().getAction(m_str);
	if(action) {
		switch(action->actionKind()) {
		case ACTION_KIND_SCRIPT:
		case ACTION_KIND_IN_DB: {
			ActionNode *actnode = action->getActionNode(ctx);
			if(!actnode) {
				throw RuntimeError(*this, ctx.stack(),
					"Failed to call function '%s'", action->name());
			}
			m_impl = new CallFunctionNodeImpl(*this, actnode);
			}
			return;
		case ACTION_KIND_LOCAL_EXTERNAL:
			m_impl = new LocalScriptFunctionImpl(*this, *action);
			return;
		case ACTION_KIND_REMOTE_EXTERNAL:
			m_impl = new RemoteScriptFunctionImpl(*this, *action);
			return;
		case ACTION_KIND_PLUGIN: {
			// load the plugin - lookup the function - create the impl using the factory
			Plugin *plugin = action->getPlugin();
			if(!plugin) {
				throw RuntimeError(*this, ctx.stack(),
					"Function \"%s\" defined as plugin, but plugin not defined",
					action->name());
			}
			if(!plugin->loadPlugin(ctx.dm())) {
				throw RuntimeError(*this, ctx.stack(),
					"Failed to load plugin \"%s\"", plugin->library());
			}
			FunctionNodeImplFactory *factory = FunctionNodeImplRegistry::instance()
				.getFactory(action->name());
			if(!factory) {
				throw RuntimeError(*this, ctx.stack(),
					"Loaded plugin \"%s\" and function \"%s\" is still not registered",
					plugin->library(), action->name());
			}
			m_impl = factory->create(ctx.dm(), *this);
			}
			return;
		}
	}

	throw SyntaxError(*this, ctx.stack(), "Unrecognised function '%s'", m_str);
}


Expr *FunctionNode::evaluate(Context &ctx, bool stringify /*= false*/)
{
	AutoPtr<ExprList> args = (m_list ? m_list->toExprList(ctx) : NULL);

	// Check built-in functions first
	Expr *ret = ::evaluate_function(m_str, args, ctx);
	if(ret) {
		return ret;
	}

	if(!m_impl) {
		createImpl(ctx);
	}
	if (m_impl) {
		// Build in function found okay
		return m_impl->evaluate(args, ctx, stringify);
	}
}


///////////////////////////////////////////////////////////////////////////////
// Stmt
///////////////////////////////////////////////////////////////////////////////

Stmt::Stmt(YYLTYPE &loc, int kind)
	: m_fileno(loc.file_num), m_lineno(loc.first_line), m_fragno(loc.frag_num),
	  m_kind(kind), m_value(NULL), m_extended(NULL)
{}


Stmt::Stmt(YYLTYPE &loc, char *name, Node *value)
	: m_fileno(loc.file_num), m_lineno(loc.first_line), m_fragno(loc.frag_num),
	  m_kind(STMT_ARG), m_value(value), m_extended(name)
{}


Stmt::Stmt(int kind)
	: m_fileno(0), m_lineno(0), m_fragno(0), m_kind(kind),
	  m_value(NULL), m_extended(NULL)
{}


Stmt::Stmt(char *name, Node *value)
	: m_fileno(0), m_lineno(0), m_fragno(0), m_kind(STMT_ARG),
	  m_value(value), m_extended(name)
{}


Stmt::Stmt(const Stmt &copy)
	: m_fileno(copy.m_fileno), m_lineno(copy.m_lineno),
	  m_fragno(copy.m_fragno), m_kind(copy.m_kind),
	  m_value(copy.m_value ? new Node(*copy.m_value) : NULL),
	  m_extended(DUP_NULL(copy.m_extended))
{}


/*virtual*/ Stmt::~Stmt()
{
	if(m_kind == STMT_ARG) {
		SAFE_DELETE(m_value);
		SAFE_FREE(m_extended);
	}
}


/*virtual*/ ControlPathReturn Stmt::checkControlPaths(class DM &dm)
{
	debug3("Checking stmt of kind %d", m_kind);
	return control_path_normal;
}


/*virtual*/ void Stmt::execute(Context &ctx)
{
	switch(m_kind) {
	case STMT_BREAK: throw BreakException(*this, ctx.stack());
	case STMT_CONTINUE: throw ContinueException(*this, ctx.stack());
	}

	throw SyntaxError(*this, ctx.stack(),
		"Statement of kind %d not yet implemented", m_kind);
}


void Stmt::execute(DMThreadList &threads, Context &ctx)
{
	// Most statements aren't thread aware, so just run the normal execute
	execute(ctx);
}


void Stmt::indent(int indent)
{
	for(int n = 0; n < indent; n++) {
		printf("  ");
	}
}


/*virtual*/ void Stmt::print(int ind)
{
	switch(m_kind) {
	case STMT_BREAK:    indent(ind); printf("break;\n"); break;
	case STMT_CONTINUE: indent(ind); printf("continue;\n"); break;
	case STMT_ARG:      indent(ind); printf("%s: ", m_extended), m_value->dump(); printf("\n"); break;
	default:            indent(ind); printf("kind(%d)\n", m_kind); break;
	}
}


///////////////////////////////////////////////////////////////////////////////

StmtList::StmtList()
	: List<Stmt>(true)
{}


ControlPathReturn StmtList::checkControlPaths(DM &dm)
{
	debug3("Checking StmtList (%p)", this);
	ListIterator<Stmt> iter(*this);
	for(Stmt *s = iter.first(); s; s = iter.next()) {
		ControlPathReturn ret = s->checkControlPaths(dm);
		if(ret != control_path_normal) {
			// All control paths return, so execution can go no further
			if((s = iter.next()) != NULL) {
				// TODO: lookup filename from m_fileno
				dm.writeToStdErr("WARNING: Unreachable code detected at line %d", s->lineno());
			}
			return ret;
		}
	}

	return control_path_normal;
}


void StmtList::execute(Context &ctx)
{
	ListIterator<Stmt> iter(*this);
	for(Stmt *s = iter.first(); s; s = iter.next()) {
		s->execute(ctx);
	}
}


Stmt *StmtList::getArg(const char *name)
{
	ListIterator<Stmt> iter(*this);
	for(Stmt *s = iter.first(); s; s = iter.next()) {
		if((s->m_kind == STMT_ARG) && (strcmp(s->m_extended, name) == 0)) {
			return s;
		}
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////

IfStmt::IfStmt(YYLTYPE &loc, Node *cond, StmtList *then_part, StmtList *else_part /*= NULL*/)
	: Stmt(loc, STMT_IF), m_cond(cond), m_then(then_part), m_else(else_part)
{}


IfStmt::~IfStmt()
{
	SAFE_DELETE(m_cond);
	SAFE_DELETE(m_then);
	SAFE_DELETE(m_else);
}


/**
 * Core function - reduces the list of servers from the context by applying the
 * given condition to each server in turn and removing any for which the
 * condition is false.
 */
List<Server> *getMatchingServers(Node *cond, Context &ctx, List<Server> **anti = NULL)
{
	List<Server> *ret = new List<Server>();
	if(anti) {
		*anti = new List<Server>();
	}

	// Loop through the current list of servers
	ListIterator<Server> iter(ctx.servers());
	for(Server *s = iter.first(); s; s = iter.next()) {
		// push the server's scope (just so we can evaluate the expression)
		ctx.stack().push(s->getVars());

		try {
			ExprPtr result = cond->evaluate(ctx);
			if(result->toInt() != 0) {
				ret->add(s);
			} else if(anti) {
				(*anti)->add(s);
			}
		} catch(...) {
			ctx.stack().pop(SERVER_SCOPE);
			throw;
		}

		// pop the server's scope
		ctx.stack().pop(SERVER_SCOPE);
	}

	return ret;
}


/**
 * Core function - conditional executes either one or two blocks based on the
 * result of an expression - if the condition is true for any servers, then the
 * first block is execute.  If a second block is supplied then this is executed
 * for any servers where the condition is false.  If the condition contains no
 * references to server variables, the nett result is a normal if/then/else.
 */
bool conditionalExecution(
		Node *cond,
		StmtList *then_part,
		StmtList *else_part,
		bool handleContinue,
		Context &ctx)
{
	bool bMatched = false;

	// Optimization for when physical server is already on the stack
	// or when server-set is empty
	if(ctx.stack().contains(SERVER_SCOPE) || (ctx.servers().size() == 0)) {
		ExprPtr result = cond->evaluate(ctx);
		if(result->toInt() != 0) {
			bMatched = true;
			if(then_part) {
				try {
					then_part->execute(ctx);
				} catch(ContinueException &e) {
					if(!handleContinue) {
						throw e;
					}
				}
			}
		} else if(else_part) {
			else_part->execute(ctx);
		}
		return bMatched;
	}


	List<Server> *anti = NULL;

	//ctx.dm().writeToLogFile("CONDITIONAL EXECUTION");

	// Calculate the matching servers - only request anti-set if else part specified
	AutoPtr< List<Server> > serverSubset = getMatchingServers(cond, ctx, (else_part ? &anti : NULL));
	AutoPtr< List<Server> > antiSubset(anti);

	// If any servers match, execute the then part
	if(serverSubset->size() > 0) {
		//ctx.dm().writeToLogFile("CONDITION IS TRUE FOR THE FOLLOWING SERVERS:");
		//ListIterator<Server> iter(*serverSubset);
		//for(Server *s = iter.first(); s; s = iter.next()) {
		//	ctx.dm().writeToLogFile("%s", s->name());
		//}

		bMatched = true;

		if(then_part) {
			Context newctx(ctx, *serverSubset);
			try {
				then_part->execute(newctx);
			} catch(ContinueException &e) {
				if(!handleContinue) {
					throw e;
				}
			}
		}
	} /*else {
		ctx.dm().writeToLogFile("CONDITION IS FALSE FOR ALL SERVERS - SKIPPING CODE BLOCK");
	}*/

	// Execute the else part if any servers don't match
	if(else_part) {
		if(antiSubset && (antiSubset->size() > 0)) {
			//ctx.dm().writeToLogFile("ANTI CONDITION IS FALSE FOR THE FOLLOWING SERVERS:");
			//ListIterator<Server> iter(*antiSubset);
			//for(Server *s = iter.first(); s; s = iter.next()) {
			//	ctx.dm().writeToLogFile("%s", s->name());
			//}

			Context newctx(ctx, *antiSubset);
			else_part->execute(newctx);
		} /*else {
			ctx.dm().writeToLogFile("ANTI CONDITION IS TRUE FOR ALL SERVERS - SKIPPING CODE BLOCK");
		}*/
	}

	//ctx.dm().writeToLogFile("END CONDITIONAL EXECUTION");
	return bMatched;
}


/**
 * If both then and else have control paths that always return, then the if
 * will always return.  If there is no else part, then there is always a path
 * that will not return (unless the condition is ALWAYS true).
 * TODO: Check whether the condition is always true or always false
 */
ControlPathReturn IfStmt::checkControlPaths(class DM &dm)
{
	debug3("Checking if/then/else");
	ControlPathReturn thenOkay = m_then->checkControlPaths(dm);

	if(!m_else) {
		// Check only used to look for unreachable code, result is always false
		return control_path_normal;
	}

	ControlPathReturn elseOkay = m_else->checkControlPaths(dm);

	// If either path does not return, then whole if does not return
	if((thenOkay == control_path_normal) || (elseOkay == control_path_normal)) {
		return control_path_normal;
	}

	// If they both return for the same reason, then that is the reason
	if(thenOkay == elseOkay) {
		return thenOkay;
	}

	// Not the same, so say exception so that try/catch can deal with it
	return control_path_exception;
}


void IfStmt::execute(Context &ctx)
{
	conditionalExecution(m_cond, m_then, m_else, false, ctx);
}


///////////////////////////////////////////////////////////////////////////////

ForStmt::ForStmt(YYLTYPE &loc, Node *var, Node *value, Node *cond, Node *var2, Node *value2, StmtList *body)
	: Stmt(loc, STMT_FOR), m_var(var), m_value(value), m_cond(cond), m_var2(var2), m_value2(value2), m_body(body)
{}


ForStmt::~ForStmt()
{
	SAFE_DELETE(m_var);
	SAFE_DELETE(m_value);
	SAFE_DELETE(m_cond);
	SAFE_DELETE(m_var2);
	SAFE_DELETE(m_value2);
	SAFE_DELETE(m_body);
}


ControlPathReturn ForStmt::checkControlPaths(class DM &dm)
{
	return m_body ? m_body->checkControlPaths(dm) : control_path_normal;
}


/**
 * C-style for loop:
 *
 * for(n = 0; $n < 10; n = $n +1) {}
 *
 * Supports both break and continue.
 */
void ForStmt::execute(Context &ctx)
{
	ExprPtr evar = m_var->evaluate(ctx);
	ConstCharPtr var = evar->toString();
	ExprPtr estart = m_value->evaluate(ctx);
	if (estart->kind() == KIND_INT) {
		ctx.stack().setLocal(var, estart->toInt());
	} else {
		ConstCharPtr startval = estart->toString();
		ctx.stack().setLocal(var, (const char*) startval);
	}

	try {
		ExprPtr result = m_cond->evaluate(ctx);
		while(result->toInt() != 0) {
			try {
				if(m_body) {
					m_body->execute(ctx);
				}
			} catch(ContinueException& /*e*/) {
				ctx.dm().writeToLogFile("continue for loop");
			}

			ExprPtr evar2 =  m_var2->evaluate(ctx);
			ConstCharPtr var2 = evar2->toString();
			ExprPtr enext = m_value2->evaluate(ctx);
			if (enext->kind() == KIND_INT) {
				ctx.stack().setLocal(var2, enext->toInt());
			} else {
				ConstCharPtr nextval = enext->toString();
				ctx.stack().setLocal(var2, (const char*) nextval);
			}

			result = m_cond->evaluate(ctx);
		}
	} catch(BreakException& /*e*/) {
		ctx.dm().writeToLogFile("break out of for loop");
	}
}


///////////////////////////////////////////////////////////////////////////////

ForEachStmt::ForEachStmt(YYLTYPE &loc, Stmt *loop, StmtList *body, bool iterate)
	: Stmt(loc, STMT_FOREACH), m_loop(loop), m_body(body), m_iterate(iterate)
{}


ForEachStmt::~ForEachStmt()
{
	SAFE_DELETE(m_loop);
	SAFE_DELETE(m_body);
}


ControlPathReturn ForEachStmt::checkControlPaths(class DM &dm)
{
	return m_body ? m_body->checkControlPaths(dm) : control_path_normal;
}


void ForEachStmt::execute(Context &ctx)
{
	const char *var = m_loop->m_extended;
	ExprPtr eexpr = m_loop->m_value->evaluate(ctx);

	if(eexpr->isNull()) return;
	
	if(eexpr->kind() == KIND_STR)
	{
		char *t = eexpr->toString();
		if (t && *t=='\0') return;	// zero length string - could be null object
		if (m_iterate) throw SyntaxError(*this,
				"Invalid type for iterate - must be array/list");
		// The variable 'IFS' is taken from the stack to give the inter-field
		// separator - if not set, we defaut to whitespace
		ConstCharPtr ifs = strdup(" \t");
		Variable *ifsvar = ctx.stack().get("IFS");
		if(ifsvar) {
			char *newifs = ifsvar->toString();
			if (newifs) ifs = newifs;
		}

		CharPtr expr = eexpr->toString();
		char *val = expr;

		// The variable 'DEQUOTE' is taken from the stack and specifies whether
		// the result values should have quotes stripped from them
		bool dequote = false;
		Variable *dqvar = ctx.stack().get("DEQUOTE");
		if(dqvar) {
			ConstCharPtr temp =  dqvar->toString();
			dequote = temp && ((temp[0] == 'Y') || (temp[0] == 'y') || (strcmp(temp, "1") == 0));
		}

		ctx.dm().writeToLogFile("Performing for loop on [%s] for values [%s]", var, val);

		try {
			for(char *pos = getFirstIFS(val, ifs); pos; pos = getFirstIFS(val, ifs)) {
				*pos = '\0';
				if(dequote) {
					if((*val == '"') || (*val == '\'')) { val++; }
					if((*(pos-1) == '"') || (*(pos-1) == '\''))  { *(pos-1) = '\0'; }
				}

				//dm.writeToLogFile("for: setting [%s] to [%s]", var, val);
				ctx.stack().setLocal(var, val);

				// Execute the body of the loop
				try {
					if(m_body) {
						m_body->execute(ctx);
					}
				} catch(ContinueException& /*e*/) {
					ctx.dm().writeToLogFile("continue foreach loop");
				}

				val = pos+1;
			}

			// Last value - pos will be null
			if(dequote) {
				if((*val == '"') || (*val == '\'')) { val++; }
				int len = strlen(val);
				if((val[len-1] == '"') || (val[len-1] == '\''))  { val[len-1] = '\0'; }
			}

			//dm.writeToLogFile("for: setting [%s] to [%s]", var, val);
			ctx.stack().setLocal(var, val);

			// Execute the body of the loop
			try {
				if(m_body) {
					m_body->execute(ctx);
				}
			} catch(ContinueException& /*e*/) {
				ctx.dm().writeToLogFile("continue foreach loop");
			}
		} catch(BreakException& /*e*/) {
			ctx.dm().writeToLogFile("break out of foreach loop");
		}

		ctx.dm().writeToLogFile("End for loop");
	}
	else if(eexpr->kind() == KIND_ARRAY)
	{
		try {
			DMArray *arr = eexpr->toArray();
			if (arr->isList()) {
				// If a list, just loop through the keys in sequence
				for (int i=0;i<arr->count();i++) {
					if (m_iterate) {
						char buf[32];
						sprintf(buf,"%d",i);
						Variable *v = arr->get(buf);
						if (v) {
							switch (v->type()) {
								case vartype_integer:
									ctx.stack().setLocal(m_loop->m_extended,v->getInt());
									break;
								case vartype_string:
									ctx.stack().setLocal(m_loop->m_extended,v->getString());
									break;
								case vartype_array:
									ctx.stack().setLocal(m_loop->m_extended,v->getArray());
									break;
								case vartype_object:
									ctx.stack().setLocal(m_loop->m_extended,v->getObjectReference());
									break;
								case vartype_stream:
									ctx.stack().setLocal(m_loop->m_extended,"");
									break;
								case vartype_date:
									ctx.stack().setLocal(m_loop->m_extended,v->getDate());
									break;
								case vartype_bool:
									ctx.stack().setLocal(m_loop->m_extended,v->getInt()?true:false);
									break;
							}
						} else {
							ctx.stack().setLocal(m_loop->m_extended,"");
						}
					} else {
						ctx.stack().setLocal(m_loop->m_extended,i);
					}
					try {
						if(m_body) {
							m_body->execute(ctx);
						}
					} catch(ContinueException& /*e*/) {
						ctx.dm().writeToLogFile("continue foreach loop");
					}
				}
			} else {
				AutoPtr<ExprList> keys = eexpr->array_keys();
				ListIterator<Expr> iter(*keys);
				for(Expr *k = iter.first(); k; k = iter.next()) { 
					ConstCharPtr str = k->toString();
					if (m_iterate) {
						Variable *v = arr->get(str);
						if (v) {
							switch (v->type()) {
								case vartype_integer:
									ctx.stack().setLocal(m_loop->m_extended,v->getInt());
									break;
								case vartype_string:
									ctx.stack().setLocal(m_loop->m_extended,v->getString());
									break;
								case vartype_array:
									ctx.stack().setLocal(m_loop->m_extended,v->getArray());
									break;
								case vartype_object:
									ctx.stack().setLocal(m_loop->m_extended,v->getObjectReference());
									break;
								case vartype_stream:
									ctx.stack().setLocal(m_loop->m_extended,"");
									break;
								case vartype_date:
									ctx.stack().setLocal(m_loop->m_extended,v->getDate());
									break;
								case vartype_bool:
									ctx.stack().setLocal(m_loop->m_extended,v->getInt()?true:false);
									break;
							}
						} else {
							ctx.stack().setLocal(m_loop->m_extended,"");
						}
					} else {
						ctx.stack().setLocal(m_loop->m_extended, (const char*) str);
					}
					try {
						if(m_body) {
							m_body->execute(ctx);
						}
					} catch(ContinueException& /*e*/) {
						ctx.dm().writeToLogFile("continue foreach loop");
					}
				}
			}
		} catch(BreakException& /*e*/) {
			ctx.dm().writeToLogFile("break out of foreach loop");
		}
	}
	else if(eexpr->kind() == KIND_STREAM)
	{
		if (m_iterate) throw SyntaxError(*this,
				"Invalid type for iterate - must be array/list");
		char *val = eexpr->stringify();
		try {
			char *temp = NULL;
			for(char *line = STRTOK_R(val, "\r\n", &temp);
				line; line = STRTOK_R(NULL, "\r\n", &temp)) {
				ctx.stack().setLocal(m_loop->m_extended, line);
				try {
					if(m_body) {
						m_body->execute(ctx);
					}
				} catch(ContinueException& /*e*/) {
					ctx.dm().writeToLogFile("continue foreach loop");
				}
			}
		} catch(BreakException& /*e*/) {
			ctx.dm().writeToLogFile("break out of foreach loop");
		}
		free(val);
	}
	else
	{
		if(eexpr->kind() == KIND_ERROR) {
			debug1("Error is '%s'", eexpr->error());
		}
		throw SyntaxError(*this,
				"Invalid type for for loop - must be string, array or stream");
	}
}


///////////////////////////////////////////////////////////////////////////////
// WhileStmt
///////////////////////////////////////////////////////////////////////////////

WhileStmt::WhileStmt(YYLTYPE &loc, Node *cond, StmtList *body)
	: Stmt(loc, STMT_WHILE), m_cond(cond), m_body(body)
{}


WhileStmt::~WhileStmt()
{
	SAFE_DELETE(m_cond);
	SAFE_DELETE(m_body);
}


ControlPathReturn WhileStmt::checkControlPaths(class DM &dm)
{
	return m_body ? m_body->checkControlPaths(dm) : control_path_normal;
}


void WhileStmt::execute(Context &ctx)
{
	try {
		while(conditionalExecution(m_cond, m_body, NULL, true, ctx)) { /*loop until condition is false*/ }
	} catch(BreakException& /*e*/) {
		ctx.dm().writeToLogFile("break out of while loop");
	}
}


///////////////////////////////////////////////////////////////////////////////
// CaseStmt
///////////////////////////////////////////////////////////////////////////////

CaseStmt::CaseStmt(YYLTYPE &loc, Node *value, StmtList *body)
	: Stmt(loc, STMT_CASE), m_value(value), m_body(body)
{}


CaseStmt::CaseStmt(YYLTYPE &loc, StmtList *body)
	: Stmt(loc, STMT_DEFAULT), m_value(NULL), m_body(body)
{}


CaseStmt::~CaseStmt()
{
	SAFE_DELETE(m_value);
	SAFE_DELETE(m_body);
}


void CaseStmt::execute(Context &ctx)
{
	if(m_body) {
		m_body->execute(ctx);
	}
}


bool CaseStmt::isEqual(Expr &e, Context &ctx)
{
	// default always matches
	if(m_kind == STMT_DEFAULT) {
		return true;
	}

	ExprPtr result = m_value->evaluate(ctx);
	return (e.compare(*result) == 0) ? true : false;
}


///////////////////////////////////////////////////////////////////////////////
// CaseStmtList
///////////////////////////////////////////////////////////////////////////////

CaseStmtList::CaseStmtList()
	: List<CaseStmt>(true)
{}


///////////////////////////////////////////////////////////////////////////////
// SwitchStmt
///////////////////////////////////////////////////////////////////////////////

SwitchStmt::SwitchStmt(YYLTYPE &loc, Node *cond, CaseStmtList *cases)
	: Stmt(loc, STMT_SWITCH), m_cond(cond), m_cases(cases)
{}


SwitchStmt::~SwitchStmt()
{
	SAFE_DELETE(m_cond);
	SAFE_DELETE(m_cases);
}


/**
 * Conventional switch statement.  The condition is evaluated at the start and
 * then the list of case blocks is traversed looking for a matching value.  If
 * a match is found, then the block is executed, along with an subsequent
 * blocks until a break is encountered (an exception is raised for this).  If
 * no match is found, then the default block is executed, by virtue of the fact
 * that is always matches.
 */
void SwitchStmt::execute(Context &ctx)
{
	ExprPtr result = m_cond->evaluate(ctx);
	ListIterator<CaseStmt> iter(*m_cases);
	bool bMatched = false;
	for(CaseStmt *c = iter.first(); c; c = iter.next()) {
		if(!bMatched && c->isEqual(*result, ctx)) {
			bMatched = true;
		}

		if(bMatched) {
			try {
				c->execute(ctx);
			} catch(BreakException& /*e*/) {
				break;
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// SetStmt
///////////////////////////////////////////////////////////////////////////////


SetStmt::SetStmt(YYLTYPE &loc, char *options, Node *var, Node *value)
	: Stmt(loc, STMT_SET), m_options(options),
	  m_var(var), m_value(value)
{}


SetStmt::~SetStmt()
{
	SAFE_FREE(m_options);
	SAFE_DELETE(m_var);
	SAFE_DELETE(m_value);
}


void SetStmt::execute(Context &ctx)
{
	bool append    = m_options ? (strstr(m_options, "-a") ? true : false) : false;
	bool appendAll = m_options ? (strstr(m_options, "-l") ? true : false) : false;
	bool isGlobal  = m_options ? (strstr(m_options, "-g") ? true : false) : false;
	bool noCase    = m_options ? (strstr(m_options, "-i") ? true : false) : false;

	if((!m_value)) {
		throw SyntaxError(*this, "Invalid set syntax");
	}

	if(append && appendAll) {
		throw SyntaxError(*this,
			"Append (-a) and append all (-l) are mutually exclusive");
	}
	m_var->setVariable(m_value, append, appendAll, isGlobal, noCase, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// EchoStmt
///////////////////////////////////////////////////////////////////////////////


EchoStmt::EchoStmt(YYLTYPE &loc, Node *exp, char *stream /*= NULL*/)
	: Stmt(loc, STMT_ECHO), m_exp(exp), m_stream(stream)
{}


EchoStmt::~EchoStmt()
{
	SAFE_DELETE(m_exp);
	SAFE_FREE(m_stream);
}


void EchoStmt::execute(Context &ctx)
{
	if(m_stream) {
		// Find or create the stream (only set when redirecting via >>)
		OutputStream *ostr = NULL;
		Variable *var = ctx.stack().get(m_stream);
		if(var) {
			if(var->type() != vartype_stream) {
				throw RuntimeError(*this, ctx.stack(),
					"'%s' is already defined and is not a stream", m_stream);
			}
			ostr = var->getStream();
		} else {
			ostr = ctx.stack().newStream(m_stream);
		}
		// Change context to use the new stream for output
		Context newctx(ctx, ostr);
		ExprPtr result = m_exp->evaluate(newctx);
		ConstCharPtr rs = result->stringify();
		newctx.writeToStdOut("%s", (const char*) rs);
	} else {
		ExprPtr result = m_exp->evaluate(ctx);
		ConstCharPtr rs = result ? result->stringify() : strdup("");
		ctx.writeToStdOut("%s", (const char*) rs);
	}
}


///////////////////////////////////////////////////////////////////////////////
// TryCatchStmt
///////////////////////////////////////////////////////////////////////////////

TryCatchStmt::TryCatchStmt(
		YYLTYPE &loc, StmtList *body, char *var,
		StmtList *catch_part, StmtList *finally_part
	)
	: Stmt(loc, STMT_TRYCATCH), m_body(body), m_var(var),
	  m_catch(catch_part), m_finally(finally_part)
{}


TryCatchStmt::~TryCatchStmt()
{
	SAFE_DELETE(m_body);
	SAFE_FREE(m_var);
	SAFE_DELETE(m_catch);
	SAFE_DELETE(m_finally);
}


/**
 * try/catch/finally changes the way exception returns are handled.  Within the
 * body of the try an exception is morphed into a normal return.  Within the
 * catch and finally we take the first non-normal result.  We process all the
 * blocks to look for unreachable code.
 */
ControlPathReturn TryCatchStmt::checkControlPaths(class DM &dm)
{
	ControlPathReturn ret = control_path_normal;
	if(m_body) {
		// Does the body raise an exception?
		ControlPathReturn bodyOkay = m_body->checkControlPaths(dm);
		if(bodyOkay == control_path_exception) {
			debug1("Caught exception path detected");
			ret = control_path_normal;
		} else {
			ret = bodyOkay;
		}
	}
	if(m_catch) {
		// Does the catch always return
		ControlPathReturn catchOkay = m_catch->checkControlPaths(dm);
		if(ret == control_path_normal) {
			ret = catchOkay;
		}
	}
	if(m_finally) {
		// Does the catch always return
		ControlPathReturn finallyOkay = m_catch ? m_catch->checkControlPaths(dm) : control_path_normal ;
		if(ret == control_path_normal) {
			ret = finallyOkay;
		}
	}
	return ret;
}


void TryCatchStmt::execute(Context &ctx)
{
	try {
		if(m_body) {
			m_body->execute(ctx);
		}
	} catch(BreakException &/*e*/) {
		if(m_finally) {
			m_finally->execute(ctx);
		}
		throw;
	} catch(ContinueException &/*e*/) {
		if(m_finally) {
			m_finally->execute(ctx);
		}
		throw;
	} catch(ReturnException &/*e*/) {
		if(m_finally) {
			m_finally->execute(ctx);
		}
		throw;
	} catch(DMException &e) {
		ctx.stack().setLocal(m_var, e);
		if(m_catch) {
			// TODO: This may throw an exception, in which case finally would not be executed - should it?
			m_catch->execute(ctx);
		}
	}
	if(m_finally) {
		m_finally->execute(ctx);
	}	
}


///////////////////////////////////////////////////////////////////////////////
// PsLoopStmt
///////////////////////////////////////////////////////////////////////////////

PsLoopStmt::PsLoopStmt(YYLTYPE &loc, StmtList *body)
	: Stmt(loc, STMT_PSLOOP), m_body(body)
{}


PsLoopStmt::~PsLoopStmt()
{
	SAFE_DELETE(m_body);
}


ControlPathReturn PsLoopStmt::checkControlPaths(class DM &dm)
{
	return m_body ? m_body->checkControlPaths(dm) : control_path_normal;
}


void PsLoopStmt::execute(Context &ctx)
{
	if(ctx.servers().size() == 0) {
		ctx.dm().writeToLogFile("psloop - no physical servers through which to iterate");
		return;
	}

	int dopush = !ctx.stack().contains(SERVER_SCOPE);

	ctx.dm().writeToLogFile("Performing a loop for each physical server (psloop)");

	ListIterator<Server> iter(ctx.servers());
	try {
		for(Server *ps = iter.first(); ps; ps = iter.next())
		{
			// push the physical server's scope (if we are not already in a psloop)
			if(dopush) {
				ctx.stack().push(ps->getVars());
			}

			// Create a new list of servers containing just the current server
			List<Server> pslist;
			pslist.add(ps);

			// Now process anything within the psloop
			ctx.dm().writeToLogFile("TRIDM_PHYSICAL_SERVER=[%s]", ps->name());
			ctx.dm().writeToLogFile("TRIDM_PHYSICAL_HOSTNAME=[%s]", ps->hostname());
			if(m_body) {
				Context newctx(ctx, pslist);
				try {
					m_body->execute(newctx);
				} catch(ContinueException& /*e*/) {
					ctx.dm().writeToLogFile("continue psloop loop");
				} catch(...) {
					// If an exception is raised we still need to do the pop
					if(dopush) { ctx.stack().pop(SERVER_SCOPE); }
					throw;
				}
			}

			// pop the physical server's scope (if we were not already in a psloop)
			if(dopush) {
				ctx.stack().pop(SERVER_SCOPE);
			}
		}
	} catch(BreakException& /*e*/) {
		ctx.dm().writeToLogFile("break out of psloop loop");
	}

	ctx.dm().writeToLogFile("end psloop");
}


///////////////////////////////////////////////////////////////////////////////
// ParallelThread
///////////////////////////////////////////////////////////////////////////////

class ParallelThread : public DMThread
{
private:
	Stmt &m_stmt;
	ScopeStack *m_stack;

public:
	ParallelThread(
		DMThreadList &threadList, Stmt &stmt, Context &ctx, ScopeStack *stack);
	~ParallelThread();

	int execute(class Context &ctx);
};


ParallelThread::ParallelThread(
		DMThreadList &threadList, Stmt &stmt, Context &ctx, ScopeStack *stack
	)
	: DMThread(threadList, ctx.newThreadCopy(*stack)),
	  m_stmt(stmt), m_stack(stack)
{}


ParallelThread::~ParallelThread()
{
	debug3("~ParallelThread for thread %d", m_id);
	SAFE_DELETE(m_ctx);
	SAFE_DELETE(m_stack);
}


int ParallelThread::execute(Context &ctx)
{
	m_stmt.execute(ctx);
	return 0;	// for now
}


///////////////////////////////////////////////////////////////////////////////
// ParallelStmt
///////////////////////////////////////////////////////////////////////////////

ParallelStmt::ParallelStmt(YYLTYPE &loc, StmtList *body)
	: Stmt(loc, STMT_PARALLEL), m_body(body)
{}


ParallelStmt::~ParallelStmt()
{
	SAFE_DELETE(m_body);
}


ControlPathReturn ParallelStmt::checkControlPaths(class DM &dm)
{
	return m_body ? m_body->checkControlPaths(dm) : control_path_normal;
}


/**
 * Executes the body of the parallel block with each statement being executed
 * in its own thread.  For statements like deploy and remotescript which are
 * inherently multithreaded, we don't create a thread for them and let them
 * spawn their worker threads as necessary.  For all other statements, we
 * create a new thread and manage it for them.  We then wait for all the
 * threads (both worker and statement threads) to finish at the end of the
 * parallel block.  When we call execute(), we call the variant of execute
 * that takes a thread list, which allows deploy and remotescript to add their
 * threads to the threadlist.
 */
void ParallelStmt::execute(Context &ctx)
{
	AutoPtr<DMThreadList> threads = new DMThreadList(
		ctx.dm().getEngineConfig().getThreadLimit());

	if(m_body) {
		ListIterator<Stmt> iter(*m_body);
		for(Stmt *s = iter.first(); s; s = iter.next()) {
			if(s->isThreaded()) {
				s->execute(*threads, ctx);
			} else {
				ScopeStack *threadStack = new ScopeStack(ctx.stack());
				ParallelThread *t = new ParallelThread(*threads, *s, ctx, threadStack);
				t->start();
			}
		}
	}

	ctx.dm().writeToLogFile("Waiting for parallel threads to finish");
	int exitCode = threads->waitForAll();
	ctx.stack().setGlobal("?", exitCode);
}


///////////////////////////////////////////////////////////////////////////////
// SequentialStmt
///////////////////////////////////////////////////////////////////////////////

SequentialStmt::SequentialStmt(YYLTYPE &loc, StmtList *body)
	: Stmt(loc, STMT_PARALLEL), m_body(body)
{}


SequentialStmt::~SequentialStmt()
{
	SAFE_DELETE(m_body);
}


ControlPathReturn SequentialStmt::checkControlPaths(class DM &dm)
{
	return m_body ? m_body->checkControlPaths(dm) : control_path_normal;
}


/**
 * This is a bit of a dumb statement, but it allows parallel blocks to revert
 * to running statements in sequence.  It just executes the body, but returns
 * false from isThreaded() to ensure we get our own thread.
 */
void SequentialStmt::execute(Context &ctx)
{
	if(m_body) {
		m_body->execute(ctx);
	}
}


///////////////////////////////////////////////////////////////////////////////
// UsingStmt
///////////////////////////////////////////////////////////////////////////////

UsingStmt::UsingStmt(YYLTYPE &loc, char *usekind, Node *exp, StmtList *body)
	: Stmt(loc, STMT_USING), m_usekind(usekind), m_exp(exp), m_body(body)
{}


UsingStmt::~UsingStmt()
{
	SAFE_FREE(m_usekind);
	SAFE_DELETE(m_exp);
	SAFE_DELETE(m_body);
}


ControlPathReturn UsingStmt::checkControlPaths(class DM &dm)
{
	return m_body ? m_body->checkControlPaths(dm) : control_path_normal;
}


/*private*/ void UsingStmt::executeUsingStream(Context &ctx)
{
	// Find or create the stream
	ExprPtr estream = m_exp->evaluate(ctx);
	OutputStream *ostr = estream ? estream->toStream() : NULL;

	if(!ostr && (m_exp->kind() == NODE_LOOKUP)) {
		ConstCharPtr stream = m_exp->evalNamelist(ctx);
		Variable *var = ctx.stack().get(stream);
		if(var) {
			if(var->type() != vartype_stream) {
				throw RuntimeError(*this, ctx.stack(),
					"'%s' is already defined and is not a stream", (const char*) stream);
			}
			ostr = var->getStream();
		} else {
			ostr = ctx.stack().newStream(stream);
		}
	}

	if(!ostr) {
		throw RuntimeError(*this, ctx.stack(), "Not a valid stream");
	}

	// Change context to use the new stream for output and execute body
	Context newctx(ctx, ostr);
	if(m_body) {
		m_body->execute(newctx);
	}
}


/*private*/ void UsingStmt::executeUsingDropzone(Context &ctx)
{
	ExprPtr edropzone = m_exp->evaluate(ctx);
	ConstCharPtr dropzone = edropzone ? edropzone->toString() : NULL;

	if(!dropzone) {
		throw RuntimeError(*this, ctx.stack(),
			"A dropzone name must be specified");
	}

	Dropzone *dz = ctx.dm().getDropzone(dropzone);
	if(!dz) {
		// Create the dropzone
		dz = ctx.dm().newDropzone(dropzone, false);
		if(!dz->create(true)) {
			throw RuntimeError(*this, ctx.stack(),
				"b) Unable to create dropzone '%s'", (const char*) dropzone);
		}
	}

	if(m_body) {
		ctx.stack().push(dz->getVars());
		try {
			m_body->execute(ctx);
		} catch(...) {
			ctx.stack().pop(DROPZONE_SCOPE);
			throw;
		}
		ctx.stack().pop(DROPZONE_SCOPE);
	}
}


/*private*/ void UsingStmt::executeUsingComponent(Context &ctx)
{
	ExprPtr ecomponent = m_exp->evaluate(ctx);
	ConstCharPtr component = ecomponent ? ecomponent->toString() : NULL;

	if(!component) {
		throw RuntimeError(*this, ctx.stack(),
			"A component name must be specified");
	}

	Component *comp = ctx.dm().getComponent(component,true);	// throws runtime error if component cannot be found

	// Calculate the intersect of the current list of servers and the servers
	// upon which the component is deployed
	AutoPtr<List<Server> > servers = comp->getServerSubset(ctx);

	// Push the component and execute body
	Context newctx(ctx, *servers);
	if(m_body) {
		newctx.stack().push(comp->getVars());
		try {
			m_body->execute(newctx);
		} catch(...) {
			newctx.stack().pop(COMPONENT_SCOPE);
			throw;
		}
		newctx.stack().pop(COMPONENT_SCOPE);
	}
}

/*private*/ void UsingStmt::executeUsingApplication(Context &ctx)
{
	ExprPtr eapplication = m_exp->evaluate(ctx);
	ConstCharPtr application = eapplication ? eapplication->toString() : NULL;

	if(!application) {
		throw RuntimeError(*this, ctx.stack(),
			"An application name must be specified");
	}

	Application *app = ctx.dm().getApplication(application);
	if(!app) {
		throw RuntimeError(*this, ctx.stack(), "Not a valid application");
	}

	ScopeStack *newstack = new ScopeStack(ctx.stack());
	Context newctx(ctx,newstack);

	if(m_body) {
		newctx.stack().push(app->getVars());
		try {
			m_body->execute(newctx);
		} catch(...) {
			newctx.stack().pop(APPLICATION_SCOPE);
			throw;
		}
		newctx.stack().pop(APPLICATION_SCOPE);
	}
}

/*private*/ void UsingStmt::executeUsingEnvironment(Context &ctx)
{
	ExprPtr eenvironment = m_exp->evaluate(ctx);
	ConstCharPtr environment = eenvironment ? eenvironment->toString() : NULL;

	if(!environment) {
		throw RuntimeError(*this, ctx.stack(),
			"An environment name must be specified");
	}

	Environment *env = ctx.dm().getEnvironment((const char *)environment);
	if(!env) {
		throw RuntimeError(*this, ctx.stack(), "Not a valid environment");
	}

	List<Server> *envservers = env->getServers();
	ScopeStack *newstack = new ScopeStack(ctx.stack());
	Context newctx(ctx.dm(),*envservers,*newstack);
	newctx.dm().setTargetEnvironment(env->id());

	if(m_body) {
		newctx.stack().push(env->getVars());
		try {
			m_body->execute(newctx);
		} catch(...) {
			newctx.stack().pop(ENVIRONMENT_SCOPE);
			throw;
		}
		newctx.stack().pop(ENVIRONMENT_SCOPE);
	}
}

void UsingStmt::execute(Context &ctx)
{
	if(!m_usekind) {
		throw RuntimeError(*this, ctx.stack(),
			"No kind specified after \"using\"");
	}
	if(!m_exp) {
		throw RuntimeError(*this, ctx.stack(),
			"No expression specified after \"using\"");
	}

	if(strcmp(m_usekind, "stream") == 0) {
		executeUsingStream(ctx);
	} else if(strcmp(m_usekind, "dropzone") == 0) {
		executeUsingDropzone(ctx);
	} else if(strcmp(m_usekind, "component") == 0) {
		executeUsingComponent(ctx);
	} else if(strcmp(m_usekind, "application") == 0) {
		executeUsingApplication(ctx);
	} else if (strcmp(m_usekind, "environment") == 0) {
		executeUsingEnvironment(ctx);
	} else throw RuntimeError(*this, ctx.stack(),
			"Unknown kind '%s' specified after \"using\"", m_usekind);
}


///////////////////////////////////////////////////////////////////////////////
// ReturnStmt
///////////////////////////////////////////////////////////////////////////////

ReturnStmt::ReturnStmt(YYLTYPE &loc, Node* exp /*= NULL*/)
	: Stmt(loc, STMT_RETURN), m_exp(exp)
{}


ReturnStmt::~ReturnStmt()
{
	SAFE_DELETE(m_exp);
}


/**
 * Slightly special case - this returns true only if a value is being returned.
 */
ControlPathReturn ReturnStmt::checkControlPaths(DM &dm)
{
	if(!m_exp) {
		debug1("Return with no expression");
		throw SyntaxError(*this, "function return must specify a value");
	}
	return control_path_return;
}


void ReturnStmt::execute(Context &ctx)
{
	if(m_exp) {
		if(!ctx.stack().isFunction()) {
			throw SyntaxError(*this, "can only return a value from a function");
		}

		Expr *expr = m_exp->evaluate(ctx);
		throw ReturnException(*this, ctx.stack(), expr);
	}
	
	if(!ctx.stack().isAction()) {
		throw SyntaxError(*this, ctx.stack(), "can only return from an action");
	}
	throw ReturnException(*this, ctx.stack(), NULL);
}


///////////////////////////////////////////////////////////////////////////////
// EvalStmt
///////////////////////////////////////////////////////////////////////////////

EvalStmt::EvalStmt(YYLTYPE &loc, Node *exp)
	: Stmt(loc, STMT_EVAL), m_exp(exp)
{}


EvalStmt::~EvalStmt()
{
	SAFE_DELETE(m_exp);
}


void EvalStmt::execute(Context &ctx)
{
	if(!m_exp) {
		throw SyntaxError(*this, "expression required for eval");
	}

	ExprPtr eexpr = m_exp->evaluate(ctx, true);
	ConstCharPtr expr = eexpr ? eexpr->stringify() : NULL;

	if(!expr) {
		throw RuntimeError(*this, ctx.stack(), "invalid expression for eval");
	}

	LexerBuffer lb(eval_lexer, (const char*) expr, NULL);

	yyparse_param param;
	param.buffer = &lb;
	int res = yyparse_threadsafe_wrapper(&param);
	if((res == 0) && param.stmtlist) {
		param.stmtlist->execute(ctx);
		delete param.stmtlist;
	} else {
		if(param.ex) {
			param.ex->print(ctx.dm());
			throw *(param.ex->clone());
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// CallActionStmtImpl
///////////////////////////////////////////////////////////////////////////////

class CallActionStmtImpl : public virtual ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;
	class ActionNode *m_actionNode;

public:
	CallActionStmtImpl(class ExtendedStmt &parent, class ActionNode *action);

	void execute(Context &ctx);
};


CallActionStmtImpl::CallActionStmtImpl(ExtendedStmt &parent, ActionNode *action)
	: m_parent(parent), m_actionNode(action)
{}


void CallActionStmtImpl::execute(Context &ctx)
{
	if(m_actionNode) {
		// Create a new scope for the action
		Scope *actscope = new Scope(ACTION_SCOPE, m_actionNode, m_parent, true);

		// If there are any arguments, set them as variables inside the new scope
		StmtList *args = m_parent.getArgs();
		if(args) {
			ListIterator<Stmt> iter(*args);
			for(Stmt *s = iter.first(); s; s = iter.next()) {
				const char *name = s->name();
				ExprPtr e = m_parent.getArg(name, ctx);
				if(e) {
					switch(e->kind()) {
					case KIND_OBJECT: {
						ObjectReference* oref = e->toObjectReference();
						if(oref) {
							actscope->set(name, oref);
						}
						}
						break;
					case KIND_ARRAY:
						actscope->set(name, e->toArray());
						break;
					default: {
						ConstCharPtr value = e->toString();
						if(value) {
							debug3("Setting arg '%s' to '%s'", name, (const char*) value);
							actscope->set(name, (const char*) value);
						}
						}
						break;
					}
				}
			}
		}

		ctx.stack().push(actscope);

		// Execute the action, looking out for exceptions
		try {
			m_actionNode->execute(ctx);
		} catch(ReturnException &e) {
			if(e.expr()) {
				throw RuntimeError(m_parent, ctx.stack(),
					"Action '%s' should not return a value", m_actionNode->name());
			}
			// fall thru... scope will be popped before returning
		} catch(...) {
			ctx.stack().pop(ACTION_SCOPE);
			throw;
		}
		ctx.stack().pop(ACTION_SCOPE);
	}
}

///////////////////////////////////////////////////////////////////////////////
// CallFunctionStmtImpl
///////////////////////////////////////////////////////////////////////////////

class CallFunctionStmtImpl : public virtual ExtendedStmtImpl
{
private:
	class ExtendedStmt &m_parent;
	class FunctionNode *m_functionNode;
	class Node *m_node;

public:
	CallFunctionStmtImpl(class ExtendedStmt &parent, class FunctionNode *function);
	CallFunctionStmtImpl(class ExtendedStmt &parent, class Node *node);

	void execute(Context &ctx);
};


CallFunctionStmtImpl::CallFunctionStmtImpl(ExtendedStmt &parent, FunctionNode *function)
	: m_parent(parent), m_functionNode(function), m_node(NULL)
{}

CallFunctionStmtImpl::CallFunctionStmtImpl(ExtendedStmt &parent, Node *node)
	: m_parent(parent), m_functionNode(NULL), m_node(node)
{}


void CallFunctionStmtImpl::execute(Context &ctx)
{
	if (m_functionNode) {
		Expr *d = m_functionNode->evaluate(ctx);
	} else {
		m_node->evaluate(ctx);
	}
}


///////////////////////////////////////////////////////////////////////////////
// ExtendedStmt
///////////////////////////////////////////////////////////////////////////////

ExtendedStmt::ExtendedStmt(YYLTYPE &loc, char *ident, StmtList *body)
	: Stmt(loc, STMT_EXTENDED), m_ident(ident), m_args(NULL),
	  m_body(body), m_pre(NULL), m_post(NULL), m_action(NULL),
	  m_function(NULL), m_node(NULL)
{}


ExtendedStmt::ExtendedStmt(YYLTYPE &loc, char *ident, StmtList *args, StmtList *body)
	: Stmt(loc, STMT_EXTENDED), m_ident(ident), m_args(args),
	  m_body(body), m_pre(NULL), m_post(NULL), m_action(NULL),
	  m_function(NULL), m_node(NULL)
{}


ExtendedStmt::ExtendedStmt(YYLTYPE &loc, char *ident, StmtList *args, StmtList *pre, StmtList *post)
	: Stmt(loc, STMT_EXTENDED), m_ident(ident), m_args(args),
	  m_body(NULL), m_pre(pre), m_post(post), m_action(NULL),
	  m_function(NULL), m_node(NULL)
{}


ExtendedStmt::ExtendedStmt(char *ident, StmtList *args)
	: Stmt(STMT_EXTENDED), m_ident(ident), m_args(args),
	  m_body(NULL), m_pre(NULL), m_post(NULL), m_action(NULL),
	  m_function(NULL), m_node(NULL)
{}


ExtendedStmt::ExtendedStmt(Action *action)
	: Stmt(STMT_EXTENDED), m_ident(DUP_NULL(action ? action->name() : NULL)), m_args(NULL),
	  m_body(NULL), m_pre(NULL), m_post(NULL), m_action(action),
	  m_function(NULL), m_node(NULL)
{}

ExtendedStmt::ExtendedStmt(FunctionNode *fn)
	: Stmt(STMT_EXTENDED), m_ident(NULL), m_args(NULL),
	  m_body(NULL), m_pre(NULL), m_post(NULL), m_action(NULL),
	  m_function(fn), m_node(NULL)
{}

ExtendedStmt::ExtendedStmt(Node *fn)
	: Stmt(STMT_EXTENDED), m_ident(NULL), m_args(NULL),
	  m_body(NULL), m_pre(NULL), m_post(NULL), m_action(NULL),
	  m_function(NULL), m_node(fn)
{}


ExtendedStmt::~ExtendedStmt()
{
	SAFE_FREE(m_ident);
	SAFE_DELETE(m_args);
	SAFE_DELETE(m_body);
	SAFE_DELETE(m_pre);
	SAFE_DELETE(m_post);
	m_action = NULL;		// owned by cache
}


ExtendedStmtImpl *ExtendedStmt::createImplFromFactory(ExtendedStmtImplFactory &factory, Context &ctx)
{
	// Check that we have the right pre/post/body supplied
	if(!factory.allowsBody() && m_body) {
		throw SyntaxError(*this, "'%s' does not allow a body", m_ident);
	}
	if(!factory.allowsPrePost() && (m_pre || m_post)) {
		throw SyntaxError(*this, "'%s' does not allow a pre or post", m_ident);
	}

	// Create an instance of the impl so that we may later execute it
	class ExtendedStmtImpl *impl = factory.create(ctx.dm(), *this);
	if(!impl) {
		throw SyntaxError(*this, "Unexpected error creating instance of '%s'", m_ident);
	}
	return impl;
}


ExtendedStmtImpl *ExtendedStmt::createImpl(Context &ctx)
{
	ExtendedStmtImplFactory *factory = m_ident?ExtendedStmtImplRegistry::instance().getFactory(m_ident):NULL;
	if(factory) {
		return createImplFromFactory(*factory, ctx);
	}

	// Not a built-in, so is it a script action we are calling?
	if(!m_action) {
		ActionNode *actnode = m_ident?ctx.dm().getActionNode(m_ident):NULL;
		if(actnode) {
			if(m_body) {
				throw SyntaxError(*this, ctx.stack(),
					"Action call cannot have a body");
			}
			if(actnode->isFunction()) {
				throw SyntaxError(*this, ctx.stack(),
					"Cannot call a function as an action");
			}
			return new CallActionStmtImpl(*this, actnode);
		}
	}

	
	// Is this a stand-alone function
	if (m_function) {
		return new CallFunctionStmtImpl(*this,m_function);
	} else if (m_node) {
		return new CallFunctionStmtImpl(*this,m_node);
	}

	// Not built-in or script action, check external scripts and database actions
	Action *action = m_action ? m_action : ctx.dm().getAction(m_ident);
	if(action) {
		ctx.dm().addDeploymentAction(action);
		switch(action->actionKind()) {
		case ACTION_KIND_SCRIPT:
		case ACTION_KIND_IN_DB:
		case ACTION_KIND_GRAPHICAL: {
			ActionNode *actnode = action->getActionNode(ctx);
			if(actnode) {
				if(actnode->isFunction()) {
					throw SyntaxError(*this, ctx.stack(),
						"Cannot call a function as an action");
				}
				return new CallActionStmtImpl(*this, actnode);
			} else {
				throw RuntimeError(*this, ctx.stack(),
					"Failed to create call action for '%s'", action->name());
			}
			}
			break;
		case ACTION_KIND_LOCAL_EXTERNAL:
			return new LocalScriptStmtImpl(*this, *action);
		case ACTION_KIND_REMOTE_EXTERNAL:
			return new RemoteScriptActionImpl(*this, *action);
		case ACTION_KIND_PLUGIN: {
			// load the plugin - lookup the action - create the impl using the factory
			Plugin *plugin = action->getPlugin();
			if(!plugin) {
				throw RuntimeError(*this, ctx.stack(),
					"Statement \"%s\" defined as plugin, but no plugin name given", m_ident);
			}
			if(!plugin->loadPlugin(ctx.dm())) {
				throw RuntimeError(*this, ctx.stack(),
					"Failed to load plugin \"%s\"", plugin->library());
			}
			ExtendedStmtImplFactory *factory = ExtendedStmtImplRegistry::instance().getFactory(m_ident);
			if(!factory) {
				throw RuntimeError(*this, ctx.stack(),
					"Loaded plugin \"%s\" and statement \"%s\" is still not registered",
					plugin->library(), m_ident);
			}
			return createImplFromFactory(*factory, ctx);
			}
			break;
		}
	}

	throw SyntaxError(*this, ctx.stack(), "Unrecognised statement '%s'", m_ident);
}


ControlPathReturn ExtendedStmt::checkControlPaths(class DM &dm)
{
	// TODO: This is not a particularly clean way to do this
	if (m_ident && strcmp(m_ident, "abort") == 0) {
		debug1("Exception path detected");
		return control_path_exception;
	}
	return control_path_normal;
}


void ExtendedStmt::execute(Context &ctx)
{
	AutoPtr<ExtendedStmtImpl> impl = createImpl(ctx);
	impl->execute(ctx);
}


void ExtendedStmt::execute(DMThreadList &threads, Context &ctx)
{
	// Extended statment implementations may be thread aware, so delegate to the correct method
	AutoPtr<ExtendedStmtImpl> impl = createImpl(ctx);
	impl->execute(threads, ctx);
}


bool ExtendedStmt::isThreaded()
{
	ExtendedStmtImplFactory *factory = ExtendedStmtImplRegistry::instance().getFactory(m_ident);
	return factory ? factory->isThreaded() : false;
}


void ExtendedStmt::executeBody(Context &ctx)
{
	if(m_body) {
		m_body->execute(ctx);
	}
}


void ExtendedStmt::executePre(Context &ctx)
{
	if(m_pre) {
		m_pre->execute(ctx);
	}
}


void ExtendedStmt::executePost(Context &ctx)
{
	if(m_post) {
		m_post->execute(ctx);
	}
}


void ExtendedStmt::print(int ind)
{
	indent(ind);
	printf("%s(\n", m_ident);
	//m_args->print(ind+1);
	ListIterator<Stmt> iter(*m_args);
	for(Stmt *s = iter.first(); s; s = iter.next()) {
		s->print(ind+1);
	}
	printf(")\n");
}


/*virtual*/ Node *ExtendedStmt::getArgNode(const char *name)
{
	if(m_args) {
		Stmt *arg = m_args->getArg(name);
		if(arg) {
			return arg->m_value;
		}
	}
	return NULL;
}


/*virtual*/ Expr *ExtendedStmt::getArg(const char *name, Context &ctx)
{
	if(m_args) {
		Stmt *arg = m_args->getArg(name);
		if(arg && arg->m_value) {
			return arg->m_value->evaluate(ctx);
		}
	}
	return NULL;
}


/*virtual*/ StmtList *ExtendedStmt::getArgs()
{
	return m_args;
}


Expr *ExtendedStmt::getOverridableArg(
	const char *name, ProviderObject &prov, bool mandatory, Context &ctx)
{
	Property *prop = prov.getProperty(name);
	Expr *arg = getArg(name, ctx);
	Expr *ret = NULL;

	if(prop && arg) {
		// both specified - check overridable - if set take arg, otherwise exception
		if(!prop->overridable()) {
			SAFE_FREE(arg);
			throw RuntimeError(*this, ctx.stack(),
				"\"%s\" cannot be overridden for %s", name, m_ident);
		}
		ret = arg;
	} else if(prop) {
		// Expand variables in property value
		char *propval = prop->decryptedValue(prov);
		Node npropval(NODE_STR, propval, true);
		ret = npropval.evaluate(ctx);
	} else {
		ret = arg;
	}

	if(mandatory && !ret) {
		throw RuntimeError(*this, ctx.stack(),
			"\"%s\" must be specified for %s", name, m_ident);
	}

	debug1("getOverridableArg('%s') -> '%s'", name, ret ? ret->stringify() : "(null)");
	return ret;
}


/**
 * Convenience method, so that the caller doesn't have to do the toString()
 */
char *ExtendedStmt::getArgAsString(const char *name, Context &ctx)
{
	ExprPtr e = getArg(name, ctx);
	return e ? e->toString() : NULL;
}


char *ExtendedStmt::getOverridableArgAsString(
	const char *name, ProviderObject &prov, bool mandatory, Context &ctx)
{
	Property *prop = prov.getProperty(name);
	char *arg = getArgAsString(name, ctx);
	char *ret = NULL;

	if(prop && arg) {
		// both specified - check overridable - if set take arg, otherwise exception
		if(!prop->overridable()) {
			SAFE_FREE(arg);
			throw RuntimeError(*this, ctx.stack(),
				"\"%s\" cannot be overridden for %s", name, m_ident);
		}
		ret = arg;
	} else if(prop) {
		// Expand variables in property value
		char *propval = prop->decryptedValue(prov);
		Node npropval(NODE_STR, propval, true);
		ExprPtr epropval = npropval.evaluate(ctx);
		ret = epropval->stringify();
	} else {
		ret = arg;
	}

	if(mandatory && !ret) {
		throw RuntimeError(*this, ctx.stack(),
			"\"%s\" must be specified for %s", name, m_ident);
	}

	debug1("getOverridableArgAsString('%s') -> '%s'", name, NULL_CHECK(ret));
	return ret;
}


char *ExtendedStmt::getOverridableAppendableArgAsString(
	const char *name, char absolute, char separator,
	ProviderObject &prov, bool mandatory, Context &ctx)
{
	Property *prop = prov.getProperty(name);
	char *arg = getArgAsString(name, ctx);
	char *ret = NULL;

	if(prop && arg) {
		// both specified - work out override/append
		if(arg[0] == absolute) {
			
			// arg is absolute, so user wants to override
			if(!prop->overridable()) {
				SAFE_FREE(arg);
				throw RuntimeError(*this, ctx.stack(),
					"\"%s\" cannot be overridden for %s", name, m_ident);
			}
			ret = arg;
		} else {
			// arg is relative, so user wants to append
			if(!prop->appendable()) {
				throw RuntimeError(*this, ctx.stack(),
					"\"%s\" cannot be appended for %s", name, m_ident);
			}
			// Expand variables in property value
			char *propval = prop->decryptedValue(prov);
			Node npropval(NODE_STR, propval, true);
			ExprPtr epropval = npropval.evaluate(ctx);
			ConstCharPtr root = epropval->stringify();
			// Append the argument to the property
			ret = (char*) malloc(strlen(root) + strlen(arg) + 2);
			sprintf(ret, "%s%c%s", (const char*) root, separator, arg);
			SAFE_FREE(arg);
		}
	} else if(prop) {
		// Expand variables in property value
		char *propval = prop->decryptedValue(prov);
		Node npropval(NODE_STR, propval, true);
		ExprPtr epropval = npropval.evaluate(ctx);
		ret = epropval->stringify();	// TODO: If this can be toString() then we can simplify to use getOverridableArg()
	} else {
		ret = arg;
	}

	if(mandatory && !ret) {
		throw RuntimeError(*this, ctx.stack(),
				"\"%s\" must be specified for %s", name, m_ident);
	}

	debug1("getOverridableAppendableArgAsString('%s') -> '%s'", name, NULL_CHECK(ret));
	return ret;
}


bool ExtendedStmt::getArgAsBoolean(const char *name, Context &ctx)
{
	ExprPtr e = getArg(name, ctx);
	return e ? e->toBool() : false;
}


bool ExtendedStmt::getOverridableArgAsBoolean(
	const char *name, ProviderObject &prov, bool mandatory, Context &ctx)
{
	ExprPtr e = getOverridableArg(name, prov, mandatory, ctx);
	return e ? e->toBool() : false;
}


int ExtendedStmt::getArgAsInt(const char *name, class Context &ctx)
{
	ExprPtr e = getArg(name, ctx);
	return e ? e->toInt() : 0;
}


/**
 * This is guaranteed to return a Dropzone or raise an exception.
 */
Dropzone &ExtendedStmt::pushDropzone(bool &popdz, Context &ctx)
{
	ConstCharPtr dropzone = getArgAsString("dropzone", ctx);
	Scope *dzscope = ctx.stack().getScope(DROPZONE_SCOPE);
	Dropzone *dzscopj = dzscope ? (Dropzone*) dzscope->getScopeObject() : NULL;

	if(!dropzone && !dzscopj) {
		throw RuntimeError(*this, ctx.stack(),
			"'%s' requires a dropzone to be specified", ident());
	}

	Dropzone *dz = NULL;
	if(dropzone) {
		dz = ctx.dm().getDropzone(dropzone);
		if(!dz) {
			throw RuntimeError(*this, ctx.stack(),
				"dropzone '%s' not found for '%s'", (const char*) dropzone, m_ident);
		}
	} else if(dzscopj) {
		dz = dzscopj;
	}

	if (dz) {
		if(!dz->pathname()) {
			throw RuntimeError(*this, ctx.stack(),
				"dropzone '%s' does not exist", dz->name());
		}

		popdz = false;
		if(!dzscope || (dzscope != dz->getVars())) {
			ctx.stack().push(dz->getVars());
			popdz = true;
		}

		return *dz;
	} else {
		throw RuntimeError(*this, ctx.stack(), "internal error: dz not set");
	}
}


/**
 * This returns a Dropzone or NULL of one is not specified.
 * May raise an exception if a dropzone is named but doesn't exist.
 */
class Dropzone *ExtendedStmt::getOptionalDropzone(Context &ctx)
{
	ConstCharPtr dropzone = getArgAsString("dropzone", ctx);
	Scope *dzscope = ctx.stack().getScope(DROPZONE_SCOPE);
	Dropzone *dzscopj = dzscope ? (Dropzone*) dzscope->getScopeObject() : NULL;

	if(!dropzone && !dzscopj) {
		return NULL;
	}

	Dropzone *dz = NULL;
	if(dropzone) {
		dz = ctx.dm().getDropzone(dropzone);
		if(!dz) {
			throw RuntimeError(*this, ctx.stack(),
				"dropzone '%s' not found for '%s'", (const char*) dropzone, m_ident);
		}
	} else if(dzscopj) {
		dz = dzscopj;
	}

	if (dz) {
		if(!dz->pathname()) {
			throw RuntimeError(*this, ctx.stack(),
				"dropzone '%s' does not exist", dz->name());
		}
	}
	return dz;
}


///////////////////////////////////////////////////////////////////////////////
// ActionNode
///////////////////////////////////////////////////////////////////////////////

ActionNode::ActionNode(YYLTYPE &loc, char *name, class StmtList *body)
	: m_fileno(loc.file_num), m_lineno(loc.first_line), m_name(name),
	  m_args(NULL), m_body(body), m_isFunction(false)
{}


ActionNode::ActionNode(YYLTYPE &loc, char *name, NodeList *args, class StmtList *body)
	: m_fileno(loc.file_num), m_lineno(loc.first_line), m_name(name),
	  m_args(args), m_body(body), m_isFunction(true)
{}


ActionNode::~ActionNode()
{
	SAFE_FREE(m_name);
	SAFE_DELETE(m_args);
	SAFE_DELETE(m_body);
}


void ActionNode::semanticCheck(class DM &dm)
{
	if(m_isFunction) {
		debug2("Checking control paths of function '%s'", m_name);
		if(m_body->checkControlPaths(dm) == control_path_normal) {
			debug1("Control paths of function '%s' are NOT okay", m_name);
			throw SyntaxError(*this, "Not all control paths return a value");
		}
		debug1("Control paths of function '%s' are okay", m_name);
	}
}


void ActionNode::execute(Context &ctx)
{
	m_body->execute(ctx);
}


///////////////////////////////////////////////////////////////////////////////
// ActionNodeList
///////////////////////////////////////////////////////////////////////////////

ActionNodeList::ActionNodeList()
	: List<ActionNode>(true)
{}


ActionNode *ActionNodeList::firstActionNode()
{
	ListIterator<ActionNode> iter(*this);
	return iter.first();
}


ActionNode *ActionNodeList::getActionNode(const char *name)
{
	debug3("getActionNode name=[%s]",name);
	ListIterator<ActionNode> iter(*this);
	debug3("About to start iterating");
	for(ActionNode *a = iter.first(); a; a = iter.next()) {
		debug3("In iteration loop, a->m_name=[%s]",a->m_name);
		if(strcmp(a->m_name, name) == 0) {
			return a;
		}
	}
	debug3("End of iteration- returning NULL");
	return NULL;
}


List<ActionNode> *ActionNodeList::add(ActionNode *actnode)
{
	ListIterator<ActionNode> iter(*this);
	for(ActionNode *a = iter.first(); a; a = iter.next()) {
		if(strcmp(a->m_name, actnode->name()) == 0) {
			throw SyntaxError(*actnode,
					"Action with the name '%s' already defined", actnode->name());
		}
	}
	return List<ActionNode>::add(actnode);
}


void ActionNodeList::merge(ActionNodeList &other)
{
	ListIterator<ActionNode> iter(other);
	for(ActionNode *a = iter.first(); a; a = iter.next()) {
		this->add(a);
	}
	// List items now owned by this list - unset owner on other list and delete
	other.setIsOwner(false);
	other.clear();
}


void ActionNodeList::semanticCheck(class DM &dm)
{
	ListIterator<ActionNode> iter(*this);
	for(ActionNode *a = iter.first(); a; a = iter.next()) {
		a->semanticCheck(dm);
	}
}
