/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     START_ACTION = 258,
     START_EXPR = 259,
     START_STMTLIST = 260,
     NE = 261,
     GE = 262,
     LE = 263,
     PE = 264,
     ME = 265,
     TE = 266,
     DOLBR = 267,
     CLPL = 268,
     CLMI = 269,
     MAP = 270,
     REDIR = 271,
     NOWS = 272,
     T_BOOL = 273,
     NUM = 274,
     STR = 275,
     STR2 = 276,
     IDENT = 277,
     DOLIDENT = 278,
     T_ACTION = 279,
     T_BREAK = 280,
     T_CASE = 281,
     T_CATCH = 282,
     T_CONTINUE = 283,
     T_DECR = 284,
     T_DEFAULT = 285,
     T_ECHO = 286,
     T_ELSE = 287,
     T_EVAL = 288,
     T_FINALLY = 289,
     T_FOR = 290,
     T_FOREACH = 291,
     T_FUNC = 292,
     T_IF = 293,
     T_IN = 294,
     T_INCR = 295,
     T_ITERATE = 296,
     T_NULL = 297,
     T_PARALLEL = 298,
     T_POST = 299,
     T_PRE = 300,
     T_PSLOOP = 301,
     T_RETURN = 302,
     T_SEQUENTIAL = 303,
     T_SET = 304,
     T_SWITCH = 305,
     T_TRY = 306,
     T_USING = 307,
     T_WHILE = 308,
     NEG = 309
   };
#endif
/* Tokens.  */
#define START_ACTION 258
#define START_EXPR 259
#define START_STMTLIST 260
#define NE 261
#define GE 262
#define LE 263
#define PE 264
#define ME 265
#define TE 266
#define DOLBR 267
#define CLPL 268
#define CLMI 269
#define MAP 270
#define REDIR 271
#define NOWS 272
#define T_BOOL 273
#define NUM 274
#define STR 275
#define STR2 276
#define IDENT 277
#define DOLIDENT 278
#define T_ACTION 279
#define T_BREAK 280
#define T_CASE 281
#define T_CATCH 282
#define T_CONTINUE 283
#define T_DECR 284
#define T_DEFAULT 285
#define T_ECHO 286
#define T_ELSE 287
#define T_EVAL 288
#define T_FINALLY 289
#define T_FOR 290
#define T_FOREACH 291
#define T_FUNC 292
#define T_IF 293
#define T_IN 294
#define T_INCR 295
#define T_ITERATE 296
#define T_NULL 297
#define T_PARALLEL 298
#define T_POST 299
#define T_PRE 300
#define T_PSLOOP 301
#define T_RETURN 302
#define T_SEQUENTIAL 303
#define T_SET 304
#define T_SWITCH 305
#define T_TRY 306
#define T_USING 307
#define T_WHILE 308
#define NEG 309




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union
#line 22 "dm.ypp"
value {
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
/* Line 1489 of yacc.c.  */
#line 171 "dm.tab.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;
