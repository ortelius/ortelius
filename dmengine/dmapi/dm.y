/* Deployment Manager Scripting Language.  */

%{
#include <math.h>
#include <stdio.h>
#include <string.h>
// declarations for Hashtable template on Linux
#include <stdlib.h>
#include <ctype.h>
// end
#include "node.h"
#define YYLEX_PARAM ((yyparse_param*) yyparam)->buffer
#define YYLTYPE yylloc_param
int yylex(class LexerBuffer *buffer);
void yyerror(void *, char const *);
#define yytnamerr yytnamerr
char *concatenate_string(const char *str1, const char *str2);		// Temp
static size_t yytnamerr(char *yyres, const char *yystr);
%}


%union value {
	class ActionNode     *actn;
	class ActionNodeList *alist;
	class Stmt	     *stmt;
	class StmtList	     *slist;
	class CaseStmt       *cstmt;
	class CaseStmtList   *clist;
	class Node 	     *node;
	class NodeList 	     *nlist;
	bool		      bval;
	int 		      ival;
	char 		     *str;
}

/* Bison declarations.  */
// %define parse.lac "full"
%parse-param {void *yyparam}
%error-verbose

%defines
%token START_ACTION START_EXPR START_STMTLIST
%token NE    "!="
%token GE    ">="
%token LE    "<="
%token PE    "+="
%token ME    "-="
%token TE    "*="
%token DOLBR "${"
%token CLPL  ":+"
%token CLMI  ":-"
%token MAP   "=>"
%token REDIR ">>"
%token NOWS		/* no whitespace */
%token <bval> T_BOOL	"boolean value"
%token <ival> NUM	"integer value"
%token <str> STR STR2	"string literal"
%token <str> IDENT	"identifier"
%token <str> DOLIDENT	"$identifier"
%token T_ACTION 	"\"action\""
%token T_BREAK 		"\"break\""
%token T_CASE		"\"case\""
%token T_CATCH		"\"catch\""
%token T_CONTINUE 	"\"continue\""
%token T_DECR		"\"decr\""
%token T_DEFAULT 	"\"default\""
%token T_ECHO 		"\"echo\""
%token T_ELSE		"\"else\""
%token T_EVAL		"\"eval\""
%token T_FINALLY 	"\"finally\""
%token T_FOR 		"\"for\""
%token T_FOREACH 	"\"foreach\""
%token T_FUNC 		"\"function\""
%token T_IF 		"\"if\""
%token T_IN 		"\"in\""
%token T_INCR		"\"incr\""
%token T_ITERATE	"\"iterate\""
%token T_NULL		"\"null\""
%token T_PARALLEL 	"\"parallel\""
%token T_POST 		"\"post\""
%token T_PRE		"\"pre\""
%token T_PSLOOP 	"\"psloop\""
%token T_RETURN 	"\"return\""
%token T_SEQUENTIAL 	"\"sequential\""
%token T_SET 		"\"set\""
%token T_SWITCH 	"\"switch\""
%token T_TRY 		"\"try\""
%token T_USING 		"\"using\""
%token T_WHILE		"\"while\""

/* Operator precedence - lowest first */
%left '&' '|'				/* lexer converts -a and -o */
%left '=' "!=" '>' '<' '~' GE LE	/* lexer converts -eq, -ne, -gt and -lt */
%left '-' '+' NOWS
%left '*' '/' '%'
%left '[' '.'
%left NEG '!'    			/* negation--unary minus and not */

%type <actn>	actn
%type <alist>   actnlist
%type <stmt>	stmt arg
%type <slist>	stmtlist block arglist opt_finally
%type <cstmt>   caseblock
%type <clist>	caselist
%type <node>  	exp simpexp name namelist arrayele listele jsonobjele jsonlistele
%type <nlist> 	opt_fnarglist fnarglist opt_explist explist arrayinit listinit jsonobjinit jsonlistinit
%type <str>	opt_setopt opt_stream

%destructor { delete $$; } exp


%% /* The grammar follows.  Note that the start symbol is injected by the lexer
    * depending upon what mode we start the parser in */

input: START_ACTION actnlist		  { ((yyparse_param*) yyparam)->actnlist = $2; }
     | START_EXPR exp			  { ((yyparse_param*) yyparam)->expr = $2; }
     | START_STMTLIST stmtlist		  { ((yyparse_param*) yyparam)->stmtlist = $2; }
;

actnlist: actn		  { $$ = new ActionNodeList(); $$->add($1); }
     | actnlist actn	  { $$ = $1; $$->add($2); }
;

actn: T_ACTION IDENT block	  { $$ = new ActionNode(@1, $2, $3); }
     | T_FUNC IDENT '(' opt_fnarglist ')' block  { $$ = new ActionNode(@1, $2, $4, $6); }
;

opt_fnarglist: /*empty*/	  { $$ = NULL; }
     | fnarglist		  { $$ = $1; }

fnarglist: IDENT		  { $$ = new NodeList(); $$->add(new Node(NODE_STR, $1)); }
     | fnarglist ',' IDENT	  { $$ = $1; $$->add(new Node(NODE_STR, $3)); }
;

block: ';'			  { $$ = NULL; }
     | '{' '}'			  { $$ = new StmtList(); }
     | '{' stmtlist '}'		  { $$ = $2; }
;

stmtlist: stmt			  { $$ = new StmtList(); $$->add($1); }
     | stmtlist stmt		  { $$ = $1; $$->add($2) }
;

stmt: T_IF '(' exp ')' block	 		{ $$ = new IfStmt(@1, $3, $5); }
     | T_IF '(' exp ')' block T_ELSE block	{ $$ = new IfStmt(@1, $3, $5, $7); }
     | T_WHILE '(' exp ')' block		{ $$ = new WhileStmt(@1, $3, $5); }
     | T_FOR '(' namelist '=' exp ';'
         exp ';' namelist '=' exp ')' block	{ $$ = new ForStmt(@1, $3, $5, $7, $9, $11, $13); }
     | T_FOREACH '(' arg ')' block		{ $$ = new ForEachStmt(@1, $3, $5, false); }
     | T_FOREACH IDENT T_IN exp block	        { $$ = new ForEachStmt(@1, new Stmt(@1, $2, $4 ), $5, false); }
     | T_ITERATE '(' arg ')' block		{ $$ = new ForEachStmt(@1, $3, $5, true); }
     | T_ITERATE IDENT T_IN exp block	        { $$ = new ForEachStmt(@1, new Stmt(@1, $2, $4 ), $5, true); }
     | T_SET opt_setopt namelist '=' exp ';'	{ $$ = new SetStmt(@1, $2, $3, $5); }
     | T_SET opt_setopt namelist "+=" exp ';'	{ $$ = new SetStmt(@1, $2, $3, new Node(
     							NODE_PLUS,  new Node(NODE_NLVAR, $3->deepCopy()), $5)); }
     | T_SET opt_setopt namelist "-=" exp ';'	{ $$ = new SetStmt(@1, $2, $3, new Node(
     							NODE_MINUS, new Node(NODE_NLVAR, $3->deepCopy()), $5)); }
     | T_SET opt_setopt namelist "*=" exp ';'	{ $$ = new SetStmt(@1, $2, $3, new Node(
     							NODE_TIMES, new Node(NODE_NLVAR, $3->deepCopy()), $5)); }
     | T_INCR namelist ';'			{ $$ = new SetStmt(@1, NULL, $2, new Node(
     							NODE_PLUS, new Node(NODE_NLVAR,
     								$2->deepCopy()), new Node(1))); }
     | T_DECR namelist ';'			{ $$ = new SetStmt(@1, NULL, $2, new Node(
     							NODE_MINUS, new Node(NODE_NLVAR,
     								$2->deepCopy()), new Node(1))); }
     | T_ECHO exp opt_stream ';'		{ $$ = new EchoStmt(@1, $2, $3); }
     | T_TRY block T_CATCH '(' IDENT ')' block opt_finally  { $$ = new TryCatchStmt(@1, $2, $5, $7, $8); }
     | T_SWITCH '(' exp ')' '{' caselist '}'	{ $$ = new SwitchStmt(@1, $3, $6 ); }
     | T_BREAK ';'				{ $$ = new Stmt(@1, STMT_BREAK); }
     | T_CONTINUE ';'				{ $$ = new Stmt(@1, STMT_CONTINUE); }
     | T_PSLOOP block				{ $$ = new PsLoopStmt(@1, $2); }
     | T_PARALLEL block				{ $$ = new ParallelStmt(@1, $2); }
     | T_SEQUENTIAL block			{ $$ = new SequentialStmt(@1, $2); }
     | T_USING IDENT simpexp block		{ $$ = new UsingStmt(@1, $2, $3, $4); }
     | T_RETURN	';'				{ $$ = new ReturnStmt(@1); }
     | T_RETURN	exp ';'				{ $$ = new ReturnStmt(@1, $2); }
     | T_EVAL '(' exp ')' ';'			{ $$ = new EvalStmt(@1, $3); }
     | IDENT block				{ $$ = new ExtendedStmt(@1, $1, $2); }
     | IDENT '(' arglist ')' block		{ $$ = new ExtendedStmt(@1, $1, $3, $5); }
     | IDENT '(' arglist ')'
       '{' T_PRE block '}'			{ $$ = new ExtendedStmt(@1, $1, $3, $7, NULL); }
     | IDENT '(' arglist ')'
       '{' T_POST block '}'			{ $$ = new ExtendedStmt(@1, $1, $3, NULL, $7); }
     | IDENT '(' arglist ')'
       '{' T_PRE block T_POST block '}'		{ $$ = new ExtendedStmt(@1, $1, $3, $7, $9); }
     | IDENT '(' opt_explist ')' ';'		{ $$ = new ExtendedStmt(new FunctionNode(@1, $1, $3)); }
     | namelist '.' IDENT '(' opt_explist ')' ';' { $$ = new ExtendedStmt(new Node(NODE_OBJFUNC, $1, $3, $5)); }
;

opt_setopt: /*empty*/				{ $$ = NULL; }
     | opt_setopt '-' IDENT			{ $$ = concatenate_string("-", $3); SAFE_FREE($3);
						  if($1) {
     						      char *temp = concatenate_string($1, $$);
     						      SAFE_FREE($1); SAFE_FREE($$); $$ = temp;
     						  }
     						}
;

opt_stream: /*empty*/				{ $$ = NULL; }
     | ">>" '$' IDENT				{ $$ = $3; }
;

opt_finally: /*empty*/				{ $$ = NULL; }
     | T_FINALLY block				{ $$ = $2; }
;

caselist: caseblock				{ $$ = new CaseStmtList(); $$->add($1); }
     | caselist caseblock			{ $$ = $1; $$->add($2); }
;

caseblock: T_CASE exp ':' stmtlist		{ $$ = new CaseStmt(@1, $2, $4); }
     | T_DEFAULT ':' stmtlist			{ $$ = new CaseStmt(@1, $3); }
;

arglist: arg					{ $$ = new StmtList(); $$->add($1); }
     | arglist ',' arg				{ $$ = $1; $$->add($3); }
;

arg: IDENT ':' exp				{ $$ = new Stmt(@1, $1, $3 ); }
;

arrayinit: /*empty*/				{ $$ = new NodeList(); }
     | arrayele					{ $$ = new NodeList(); $$->add($1); }
     | arrayinit ',' arrayele			{ $$ = $1; $$->add($3); }
;

arrayele: simpexp "=>" exp			{ $$ = new Node(NODE_ARRELE, $1, $3); }
;

listinit: listele				{ $$ = new NodeList(); $$->add($1); }
     | listinit ',' listele			{ $$ = $1; $$->add($3); }
;

listele: exp					{ $$ = new Node(NODE_LISTELE, $1); }
;

jsonobjinit: jsonobjele				{ $$ = new NodeList(); $$->add($1); }
     | jsonobjinit ',' jsonobjele		{ $$ = $1; $$->add($3); }
;

/* Take note that JSON uses STR2 and creates non-expandable STRs */
jsonobjele: STR2 ':' NUM			{ $$ = new Node(NODE_ARRELE, new Node(NODE_STR, $1), new Node($3)); }
     | STR2 ':' STR2				{ $$ = new Node(NODE_ARRELE, new Node(NODE_STR, $1), new Node(NODE_STR, $3,true)); }
     | STR2 ':' T_BOOL				{ $$ = new Node(NODE_ARRELE, new Node(NODE_STR, $1), new Node($3)); }
     | STR2 ':' T_NULL				{ $$ = new Node(NODE_ARRELE, new Node(NODE_STR, $1), new Node(NODE_STR, strdup(""))); }
     | STR2 ':' '{' jsonobjinit '}'		{ $$ = new Node(NODE_ARRELE, new Node(NODE_STR, $1), new Node(NODE_ARRINI, NULL, $4)); }
     | STR2 ':' '[' jsonlistinit ']'		{ $$ = new Node(NODE_ARRELE, new Node(NODE_STR, $1), new Node(NODE_LISTINI, NULL, $4)); }
;

jsonlistinit: /*empty*/				{ $$ = new NodeList(); }
     | jsonlistele				{ $$ = new NodeList(); $$->add($1); }
     | jsonlistinit ',' jsonlistele		{ $$ = $1; $$->add($3); }
;

jsonlistele: exp				{ $$ = new Node(NODE_LISTELE, $1); }
;

exp:  simpexp			  { $$ = $1; }
     | '{' arrayinit '}'	  { $$ = new Node(NODE_ARRINI, NULL, $2); }
     | '{' listinit '}'	  	  { $$ = new Node(NODE_LISTINI, NULL, $2); }
     | '{' jsonobjinit '}'	  { $$ = new Node(NODE_ARRINI, NULL, $2); }
     | '[' jsonlistinit ']'	  { $$ = new Node(NODE_LISTINI, NULL, $2); }
;

simpexp: T_BOOL			  { $$ = new Node($1);		}
     | T_NULL                     { $$ = new Node(NODE_NULL, (char*) NULL);     }
     | NUM			  { $$ = new Node($1);		}
     | STR			  { $$ = new Node(NODE_STR, $1);	}
     | STR2			  { $$ = new Node(NODE_STR, $1, true);	}
     | '$' IDENT		  { $$ = new Node(NODE_LOOKUP, $2);	}
     | DOLIDENT		  	  { $$ = new Node(NODE_LOOKUP, $1);	}
     | "${" namelist '}'	  { $$ = new Node(NODE_NLVAR, $2);	}
     | "${" namelist ":+" exp '}' { $$ = new Node(NODE_IFUNSET, new Node(NODE_NLVAR, $2), $4);	}
     | "${" namelist ":-" exp '}' { $$ = new Node(NODE_IFSET, new Node(NODE_NLVAR, $2), $4); }
     | exp NOWS exp		  { $$ = new Node(NODE_CONC, $1, $3); }
     | exp '+' exp		  { $$ = new Node(NODE_PLUS, $1, $3);		}
     | exp '-' exp		  { $$ = new Node(NODE_MINUS, $1, $3);		}
     | exp '*' exp		  { $$ = new Node(NODE_TIMES, $1, $3); 		}
     | exp '/' exp		  { $$ = new Node(NODE_DIV, $1, $3);  		}
     | exp '%' exp		  { $$ = new Node(NODE_MOD, $1, $3);   		}
     | exp '&' exp		  { $$ = new Node(NODE_AND, $1, $3);    	}
     | exp '|' exp		  { $$ = new Node(NODE_OR, $1, $3);	    	}
     | '-' exp  %prec NEG	  { $$ = new Node(NODE_NEG, $2);       		}
     | '!' exp  %prec NEG	  { $$ = new Node(NODE_NOT, $2);       		}
     | '(' exp ')'		  { $$ = $2;			}
     | exp '=' exp		  { $$ = new Node(NODE_EQ, $1, $3); }
     | exp "!=" exp		  { $$ = new Node(NODE_NE, $1, $3); }
     | exp '>' exp		  { $$ = new Node(NODE_GT, $1, $3); }
     | exp ">=" exp		  { $$ = new Node(NODE_GE, $1, $3); }
     | exp '<' exp		  { $$ = new Node(NODE_LT, $1, $3); }
     | exp "<=" exp		  { $$ = new Node(NODE_LE, $1, $3); }
     | exp '~' exp		  { $$ = new Node(NODE_MATCH, $1, $3); }
     | IDENT '(' opt_explist ')'  { $$ = new FunctionNode(@1, $1, $3); }
     | exp '[' exp ']'		  { $$ = new Node(NODE_ARRAY, $1, $3);	}
;

name:	IDENT			  { $$ = new Node(NODE_IDOROBJ, $1); }
     | '$' IDENT		  { $$ = new Node(NODE_LOOKUP, $2);	}
     | DOLIDENT			  { $$ = new Node(NODE_LOOKUP, $1);	}
     | "${" namelist '}'	  { $$ = new Node(NODE_NLVAR, $2); }
;

namelist: name			  { $$= $1; }
     | namelist NOWS name	  { $$ = new Node(NODE_CONC, $1, $3); }
     | namelist '[' exp ']'	  { $$ = new Node(NODE_ARRAY, new Node(NODE_NLVAR, $1), $3); }
     | namelist '.' name	  { $$ = new Node(NODE_OBJECT, new Node(NODE_NLVAR, $1), $3); }
     | namelist '.' IDENT 
       '(' opt_explist ')'        { $$ = new Node(NODE_OBJFUNC, $1, $3, $5); }
;

opt_explist: /*empty*/		  { $$ = NULL; }
     | explist			  { $$ = $1; }
;

explist: exp			  { $$ = new NodeList(); $$->add($1); }
     | explist ',' exp     	  { $$ = $1; $$->add($3);	}
;
%%


/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T yytnamerr(char *yyres, const char *yystr)
{
	if (*yystr == '"') {
		YYSIZE_T yyn = 0;
		char const *yyp = yystr;

		for (;;) {
			switch(*++yyp) {
			case '\'':
			case ',':
				goto do_not_strip_quotes;
			case '\\':
				++yyp;
				/* RHT mod - allow double-quotes to be escaped */
				if((*yyp != '\\') && (*yyp != '"')) {
					goto do_not_strip_quotes;
				}
			/* Fall through.  */
			default:
				if(yyres) {
					yyres[yyn] = *yyp;
				}
				yyn++;
				break;
			case '"':
				if(yyres) {
					yyres[yyn] = '\0';
				}
				return yyn;
			}
		}
do_not_strip_quotes: ;
    }

	if(!yyres) {
		return yystrlen(yystr);
	}

	return yystpcpy(yyres, yystr) - yyres;
}
