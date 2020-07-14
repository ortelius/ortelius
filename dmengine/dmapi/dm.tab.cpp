/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 1



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




/* Copy the first part of user declarations.  */
#line 3 "dm.ypp"

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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 187 of yacc.c.  */
#line 236 "dm.tab.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

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


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 261 "dm.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  52
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1171

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  131
/* YYNRULES -- Number of states.  */
#define YYNSTATES  312

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   309

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,     2,     2,    75,    64,    54,     2,
      69,    70,    62,    61,    71,    60,    66,    63,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    76,    72,
      58,    56,    57,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    65,     2,    77,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    73,    55,    74,    59,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    68
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    14,    17,    21,    28,
      29,    31,    33,    37,    39,    42,    46,    48,    51,    57,
      65,    71,    85,    91,    97,   103,   109,   116,   123,   130,
     137,   141,   145,   150,   159,   167,   170,   173,   176,   179,
     182,   187,   190,   194,   200,   203,   209,   218,   227,   238,
     244,   252,   253,   257,   258,   262,   263,   266,   268,   271,
     276,   280,   282,   286,   290,   291,   293,   297,   301,   303,
     307,   309,   310,   312,   316,   320,   324,   328,   332,   338,
     344,   345,   347,   351,   353,   355,   359,   363,   367,   371,
     373,   375,   377,   379,   381,   384,   386,   390,   396,   402,
     406,   410,   414,   418,   422,   426,   430,   434,   437,   440,
     444,   448,   452,   456,   460,   464,   468,   472,   477,   482,
     484,   487,   489,   493,   495,   499,   504,   508,   515,   516,
     518,   520
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      79,     0,    -1,     3,    80,    -1,     4,   102,    -1,     5,
      85,    -1,    81,    -1,    80,    81,    -1,    24,    22,    84,
      -1,    37,    22,    69,    82,    70,    84,    -1,    -1,    83,
      -1,    22,    -1,    83,    71,    22,    -1,    72,    -1,    73,
      74,    -1,    73,    85,    74,    -1,    86,    -1,    85,    86,
      -1,    38,    69,   102,    70,    84,    -1,    38,    69,   102,
      70,    84,    32,    84,    -1,    53,    69,   102,    70,    84,
      -1,    35,    69,   105,    56,   102,    72,   102,    72,   105,
      56,   102,    70,    84,    -1,    36,    69,    93,    70,    84,
      -1,    36,    22,    39,   102,    84,    -1,    41,    69,    93,
      70,    84,    -1,    41,    22,    39,   102,    84,    -1,    49,
      87,   105,    56,   102,    72,    -1,    49,    87,   105,     9,
     102,    72,    -1,    49,    87,   105,    10,   102,    72,    -1,
      49,    87,   105,    11,   102,    72,    -1,    40,   105,    72,
      -1,    29,   105,    72,    -1,    31,   102,    88,    72,    -1,
      51,    84,    27,    69,    22,    70,    84,    89,    -1,    50,
      69,   102,    70,    73,    90,    74,    -1,    25,    72,    -1,
      28,    72,    -1,    46,    84,    -1,    43,    84,    -1,    48,
      84,    -1,    52,    22,   103,    84,    -1,    47,    72,    -1,
      47,   102,    72,    -1,    33,    69,   102,    70,    72,    -1,
      22,    84,    -1,    22,    69,    92,    70,    84,    -1,    22,
      69,    92,    70,    73,    45,    84,    74,    -1,    22,    69,
      92,    70,    73,    44,    84,    74,    -1,    22,    69,    92,
      70,    73,    45,    84,    44,    84,    74,    -1,    22,    69,
     106,    70,    72,    -1,   105,    66,    22,    69,   106,    70,
      72,    -1,    -1,    87,    60,    22,    -1,    -1,    16,    75,
      22,    -1,    -1,    34,    84,    -1,    91,    -1,    90,    91,
      -1,    26,   102,    76,    85,    -1,    30,    76,    85,    -1,
      93,    -1,    92,    71,    93,    -1,    22,    76,   102,    -1,
      -1,    95,    -1,    94,    71,    95,    -1,   103,    15,   102,
      -1,    97,    -1,    96,    71,    97,    -1,   102,    -1,    -1,
      99,    -1,    98,    71,    99,    -1,    21,    76,    19,    -1,
      21,    76,    21,    -1,    21,    76,    18,    -1,    21,    76,
      42,    -1,    21,    76,    73,    98,    74,    -1,    21,    76,
      65,   100,    77,    -1,    -1,   101,    -1,   100,    71,   101,
      -1,   102,    -1,   103,    -1,    73,    94,    74,    -1,    73,
      96,    74,    -1,    73,    98,    74,    -1,    65,   100,    77,
      -1,    18,    -1,    42,    -1,    19,    -1,    20,    -1,    21,
      -1,    75,    22,    -1,    23,    -1,    12,   105,    74,    -1,
      12,   105,    13,   102,    74,    -1,    12,   105,    14,   102,
      74,    -1,   102,    17,   102,    -1,   102,    61,   102,    -1,
     102,    60,   102,    -1,   102,    62,   102,    -1,   102,    63,
     102,    -1,   102,    64,   102,    -1,   102,    54,   102,    -1,
     102,    55,   102,    -1,    60,   102,    -1,    67,   102,    -1,
      69,   102,    70,    -1,   102,    56,   102,    -1,   102,     6,
     102,    -1,   102,    57,   102,    -1,   102,     7,   102,    -1,
     102,    58,   102,    -1,   102,     8,   102,    -1,   102,    59,
     102,    -1,    22,    69,   106,    70,    -1,   102,    65,   102,
      77,    -1,    22,    -1,    75,    22,    -1,    23,    -1,    12,
     105,    74,    -1,   104,    -1,   105,    17,   104,    -1,   105,
      65,   102,    77,    -1,   105,    66,   104,    -1,   105,    66,
      22,    69,   106,    70,    -1,    -1,   107,    -1,   102,    -1,
     107,    71,   102,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   115,   115,   116,   117,   120,   121,   124,   125,   128,
     129,   131,   132,   135,   136,   137,   140,   141,   144,   145,
     146,   147,   149,   150,   151,   152,   153,   154,   156,   158,
     160,   163,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   182,   184,   186,
     187,   190,   191,   199,   200,   203,   204,   207,   208,   211,
     212,   215,   216,   219,   222,   223,   224,   227,   230,   231,
     234,   237,   238,   239,   243,   244,   245,   246,   247,   248,
     251,   252,   253,   256,   259,   260,   261,   262,   263,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   298,
     299,   300,   301,   304,   305,   306,   307,   308,   312,   313,
     316,   317
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "START_ACTION", "START_EXPR",
  "START_STMTLIST", "\"!=\"", "\">=\"", "\"<=\"", "\"+=\"", "\"-=\"",
  "\"*=\"", "\"${\"", "\":+\"", "\":-\"", "\"=>\"", "\">>\"", "NOWS",
  "\"boolean value\"", "\"integer value\"", "STR", "\"string literal\"",
  "\"identifier\"", "\"$identifier\"", "\"\\\"action\\\"\"",
  "\"\\\"break\\\"\"", "\"\\\"case\\\"\"", "\"\\\"catch\\\"\"",
  "\"\\\"continue\\\"\"", "\"\\\"decr\\\"\"", "\"\\\"default\\\"\"",
  "\"\\\"echo\\\"\"", "\"\\\"else\\\"\"", "\"\\\"eval\\\"\"",
  "\"\\\"finally\\\"\"", "\"\\\"for\\\"\"", "\"\\\"foreach\\\"\"",
  "\"\\\"function\\\"\"", "\"\\\"if\\\"\"", "\"\\\"in\\\"\"",
  "\"\\\"incr\\\"\"", "\"\\\"iterate\\\"\"", "\"\\\"null\\\"\"",
  "\"\\\"parallel\\\"\"", "\"\\\"post\\\"\"", "\"\\\"pre\\\"\"",
  "\"\\\"psloop\\\"\"", "\"\\\"return\\\"\"", "\"\\\"sequential\\\"\"",
  "\"\\\"set\\\"\"", "\"\\\"switch\\\"\"", "\"\\\"try\\\"\"",
  "\"\\\"using\\\"\"", "\"\\\"while\\\"\"", "'&'", "'|'", "'='", "'>'",
  "'<'", "'~'", "'-'", "'+'", "'*'", "'/'", "'%'", "'['", "'.'", "'!'",
  "NEG", "'('", "')'", "','", "';'", "'{'", "'}'", "'$'", "':'", "']'",
  "$accept", "input", "actnlist", "actn", "opt_fnarglist", "fnarglist",
  "block", "stmtlist", "stmt", "opt_setopt", "opt_stream", "opt_finally",
  "caselist", "caseblock", "arglist", "arg", "arrayinit", "arrayele",
  "listinit", "listele", "jsonobjinit", "jsonobjele", "jsonlistinit",
  "jsonlistele", "exp", "simpexp", "name", "namelist", "opt_explist",
  "explist", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,    38,   124,    61,    62,    60,   126,
      45,    43,    42,    47,    37,    91,    46,    33,   309,    40,
      41,    44,    59,   123,   125,    36,    58,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    78,    79,    79,    79,    80,    80,    81,    81,    82,
      82,    83,    83,    84,    84,    84,    85,    85,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    87,    87,    88,    88,    89,    89,    90,    90,    91,
      91,    92,    92,    93,    94,    94,    94,    95,    96,    96,
      97,    98,    98,    98,    99,    99,    99,    99,    99,    99,
     100,   100,   100,   101,   102,   102,   102,   102,   102,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   104,
     104,   104,   104,   105,   105,   105,   105,   105,   106,   106,
     107,   107
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     1,     2,     3,     6,     0,
       1,     1,     3,     1,     2,     3,     1,     2,     5,     7,
       5,    13,     5,     5,     5,     5,     6,     6,     6,     6,
       3,     3,     4,     8,     7,     2,     2,     2,     2,     2,
       4,     2,     3,     5,     2,     5,     8,     8,    10,     5,
       7,     0,     3,     0,     3,     0,     2,     1,     2,     4,
       3,     1,     3,     3,     0,     1,     3,     3,     1,     3,
       1,     0,     1,     3,     3,     3,     3,     3,     5,     5,
       0,     1,     3,     1,     1,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     2,     1,     3,     5,     5,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     1,
       2,     1,     3,     1,     3,     4,     3,     6,     0,     1,
       1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     2,     5,     0,
      89,    91,    92,    93,     0,    95,    90,     0,    80,     0,
       0,    64,     0,     3,    84,     0,   119,   121,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     4,    16,
     123,     0,     1,     0,     0,     6,   119,     0,   128,   107,
       0,    81,    83,   108,     0,    93,     0,    65,     0,    68,
       0,    72,    70,    84,    94,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   128,    13,     0,    44,    35,    36,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,    38,    37,
      41,     0,    39,     0,     0,     0,     0,     0,   120,    17,
       0,     0,     0,     7,     9,     0,     0,     0,    96,   130,
       0,   129,     0,    88,   109,     0,     0,    85,     0,    86,
       0,    87,     0,   111,   113,   115,    99,   105,   106,   110,
     112,   114,   116,   101,   100,   102,   103,   104,     0,   122,
       0,     0,    61,     0,    14,     0,    31,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,     0,    42,     0,
       0,     0,     0,     0,    84,     0,   124,     0,   119,   126,
      11,     0,    10,     0,     0,   119,   117,     0,    82,    76,
      74,    75,    77,    80,    71,    66,    69,     0,    73,    67,
     118,     0,     0,     0,     0,    15,     0,    32,     0,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,    40,     0,   125,   128,     0,     0,    97,    98,
     128,   131,     0,     0,    63,     0,    45,    62,    49,    54,
      43,     0,    23,    22,    18,    25,    24,     0,     0,     0,
       0,     0,     0,    20,     0,     8,    12,     0,    79,    78,
       0,     0,     0,     0,    27,    28,    29,    26,     0,     0,
       0,    57,     0,   127,   127,     0,     0,     0,    19,     0,
       0,    34,    58,    55,    50,    47,     0,    46,     0,     0,
      60,     0,    33,     0,     0,    59,    56,    48,     0,     0,
       0,    21
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     7,     8,   191,   192,    95,   165,    49,   113,
     168,   302,   280,   281,   161,   162,    66,    67,    68,    69,
      70,    71,    60,    61,   129,    24,    50,    51,   130,   131
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -98
static const yytype_int16 yypact[] =
{
     241,    11,   145,  1033,     7,     0,     2,    11,   -98,    14,
     -98,   -98,   -98,   -98,   -37,   -98,   -98,   145,   145,   145,
     145,   532,    17,  1094,   -98,    14,    72,   -98,    19,    41,
      14,   145,     3,    52,    -9,    64,    14,    79,   107,   107,
     453,   107,   -98,    66,   107,   115,    84,   120,  1033,   -98,
     -98,    90,   -98,   107,   103,   -98,   -98,   112,   145,   -98,
     -44,   -98,  1094,   -98,   760,   100,   110,   -98,   114,   -98,
     126,   -98,  1094,   174,   -98,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,    73,   611,   -98,   928,   -98,   -98,   -98,   166,  1034,
     145,    14,   159,   180,   145,   182,   168,   180,   -98,   -98,
     -98,   504,   -98,    83,   145,   186,   145,   145,   -98,   -98,
      14,   145,    22,   -98,   197,   145,   145,    87,   -98,  1094,
     158,   178,   145,   -98,   -98,   131,   145,   -98,   145,   -98,
     209,   -98,   145,   239,   239,   239,   171,  1106,  1106,   239,
     239,   239,   239,   171,   171,   172,   172,   172,   255,   -98,
     -48,    89,   -98,   169,   -98,   985,   -98,   167,   179,   777,
      38,   145,   181,   185,   837,   -98,   145,   189,   -98,   228,
     212,   854,   191,  1094,   107,   871,   -98,   279,   195,   -98,
     -98,   199,   194,   363,   423,   198,   -98,   145,   -98,   -98,
     -98,   -98,   -98,   145,   209,   -98,   -98,   100,   -98,  1094,
     -98,   145,   143,   180,   201,   -98,   252,   -98,   203,   145,
     444,   107,   107,   444,   107,   -98,   145,   145,   145,   145,
     206,   259,   -98,   107,   -98,   145,   107,   260,   -98,   -98,
     145,  1094,    40,   137,  1094,    18,   -98,   -98,   -98,   -98,
     -98,   523,   -98,   -98,   251,   -98,   -98,   583,   602,   662,
     681,   102,   214,   -98,   218,   -98,   -98,   220,   -98,   -98,
     107,   107,   145,   107,   -98,   -98,   -98,   -98,   145,   215,
     101,   -98,   107,   222,   -98,   221,   -32,   741,   -98,   340,
    1033,   -98,   -98,   258,   -98,   -98,   107,   -98,    14,  1033,
    1033,   107,   -98,   223,   105,  1033,   -98,   -98,   145,   931,
     107,   -98
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -98,   -98,   -98,   298,   -98,   -98,   -30,    -1,   -47,   -98,
     -98,   -98,   -98,    27,   -98,   -97,   -98,   187,   -98,   170,
     117,   184,   119,   193,    -2,   -16,   -70,    -5,   -89,   -98
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      23,   119,    48,   163,    57,    73,   173,    52,   108,   109,
     177,   112,   296,   102,   115,    59,    62,    63,    64,    72,
      91,    58,    53,   123,    54,    98,    25,   132,   211,    99,
      25,   105,    58,   133,    25,     5,    56,    27,   111,    74,
      26,    27,   297,    28,   188,    27,    29,    30,     6,    31,
     186,    32,   189,    33,    34,   120,    35,   189,    36,    37,
     103,    38,   270,   271,    39,    40,    41,    42,    43,    44,
      45,    46,   100,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    47,
     120,    96,   164,    47,   219,    25,   170,    47,   169,    25,
     184,   106,   174,   121,   127,    56,    27,   120,   180,   195,
      27,   132,   181,    97,   183,   185,   247,   268,   119,   187,
      73,   101,   120,   193,   194,   125,   126,   278,   278,   120,
      62,   279,   279,   104,   183,   114,    72,   116,   121,   127,
     209,    92,   118,   179,    93,    94,   264,   159,   107,   199,
     200,   267,   201,   117,   232,   121,   122,     9,    47,   212,
     213,   308,    47,    10,    11,    12,    13,    14,    15,   220,
     121,   127,   124,   202,   223,   291,   135,   121,   127,    93,
      94,   136,   246,   120,   137,   138,   128,    16,   139,   142,
     252,   253,   254,   255,   256,   241,   203,   140,   171,   120,
     141,    62,   172,   263,   204,    17,   265,   176,   140,   244,
      18,   269,    19,   182,    20,    93,   245,   251,    21,   190,
      22,   226,   227,   228,   257,   258,   259,   260,   196,   120,
     207,   121,   127,    87,    88,    89,    90,    90,   166,   214,
     285,   286,   216,   288,     1,     2,     3,   121,   127,   197,
     225,   217,   293,   119,   175,   221,    78,   211,   119,   224,
     231,    75,    76,    77,   235,   237,   303,   240,   229,   236,
     287,   306,    78,   248,   249,   250,   289,   121,   127,   261,
     311,   262,   266,   273,   282,    75,    76,    77,   283,   300,
     284,   290,   301,   304,   294,   295,    78,   307,   305,    85,
      86,    87,    88,    89,    90,    55,   309,   292,   206,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,   243,   242,   205,   208,   198,     0,     0,     0,     0,
       0,     0,   210,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,     0,    75,    76,    77,     0,
       0,     0,     0,     0,     0,     0,   234,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   299,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    75,
      76,    77,     0,     0,     0,     0,     0,   238,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,     0,     9,     0,     0,     0,     0,
       0,    10,    11,    12,    13,    14,    15,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,     0,
       0,     0,     0,     0,     0,    16,     0,   239,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      75,    76,    77,    17,     0,     0,    93,    94,    18,     0,
      19,    78,    20,     0,     0,   110,    21,     0,    22,    75,
      76,    77,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,     0,     9,     0,     0,     0,     0,     0,
      10,    11,    12,    65,    14,    15,     0,     0,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
       0,     0,     0,     0,    16,     0,   178,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    75,
      76,    77,    17,     0,     0,   272,     0,    18,     0,    19,
      78,    20,     0,     0,     0,    21,     0,    22,    75,    76,
      77,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     9,     0,     0,     0,     0,     0,    10,
      11,    12,    13,   160,    15,     0,     0,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,     0,
       0,     0,     0,    16,     0,   274,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    75,    76,
      77,    17,     0,     0,   275,     0,    18,     0,    19,    78,
      20,     0,     0,     0,    21,     0,    22,    75,    76,    77,
       0,     0,     0,     0,     0,     0,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,     0,     0,
       0,     0,     0,     0,   276,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    75,    76,    77,
       0,     0,     0,   277,     0,     0,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,    75,    76,    77,     0,     0,     0,     0,
       0,     0,     0,     0,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,     0,     0,     0,
       0,     0,     0,   298,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,     0,     0,     0,     0,
     134,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    75,    76,    77,     0,   218,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
      75,    76,    77,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,     0,     0,     0,    75,    76,    77,
       0,     0,     0,     0,     0,     0,     0,     0,    78,     0,
       0,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,     0,     0,     0,     0,   222,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
       0,     0,     0,     0,   230,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    75,    76,    77,
      25,   233,     0,     0,     0,     0,     0,     0,    78,     0,
      26,    27,     0,    28,     0,     0,    29,    30,     0,    31,
       0,    32,     0,    33,    34,     0,    35,     0,    36,    37,
       0,    38,     0,     0,    39,    40,    41,    42,    43,    44,
      45,    46,     0,     0,     0,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    25,     0,     0,
       0,   310,   164,    47,     0,     0,     0,    26,    27,     0,
      28,     0,     0,    29,    30,     0,    31,     0,    32,     0,
      33,    34,     0,    35,     0,    36,    37,     0,    38,     0,
       0,    39,    40,    41,    42,    43,    44,    45,    46,     0,
      75,    76,    77,     0,     0,    25,     0,     0,     0,     0,
     167,    78,     0,     0,     0,    26,    27,     0,    28,   215,
      47,    29,    30,     0,    31,     0,    32,     0,    33,    34,
       0,    35,     0,    36,    37,     0,    38,     0,     0,    39,
      40,    41,    42,    43,    44,    45,    46,     0,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      75,    76,    77,     0,     0,     0,     0,     0,    47,     0,
       0,    78,    75,    76,    77,     0,     0,     0,     0,     0,
       0,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
       0,     0,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90
};

static const yytype_int16 yycheck[] =
{
       2,    48,     3,    92,     9,    21,   103,     0,    38,    39,
     107,    41,    44,    22,    44,    17,    18,    19,    20,    21,
      25,    69,    22,    53,    22,    30,    12,    71,    76,    31,
      12,    36,    69,    77,    12,    24,    22,    23,    40,    22,
      22,    23,    74,    25,    22,    23,    28,    29,    37,    31,
     120,    33,   122,    35,    36,    17,    38,   127,    40,    41,
      69,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    69,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    75,
      17,    72,    74,    75,    56,    12,   101,    75,   100,    12,
     116,    22,   104,    65,    66,    22,    23,    17,   113,    22,
      23,    71,   114,    72,   116,   117,   213,    77,   165,   121,
     136,    69,    17,   125,   126,    13,    14,    26,    26,    17,
     132,    30,    30,    69,   136,    69,   138,    22,    65,    66,
     142,    69,    22,    60,    72,    73,   235,    74,    69,    18,
      19,   240,    21,    69,   184,    65,    66,    12,    75,    70,
      71,    56,    75,    18,    19,    20,    21,    22,    23,   171,
      65,    66,    69,    42,   176,    74,    76,    65,    66,    72,
      73,    71,   212,    17,    74,    71,    74,    42,    74,    15,
     220,   221,   222,   223,   224,   197,    65,    71,    39,    17,
      74,   203,    22,   233,    73,    60,   236,    39,    71,   211,
      65,    74,    67,    27,    69,    72,    73,   219,    73,    22,
      75,     9,    10,    11,   226,   227,   228,   229,    70,    17,
      21,    65,    66,    62,    63,    64,    65,    65,    72,    70,
     270,   271,    75,   273,     3,     4,     5,    65,    66,    71,
      22,    72,   282,   300,    72,    70,    17,    76,   305,    70,
      69,     6,     7,     8,    69,    71,   296,    69,    56,    70,
     272,   301,    17,    72,    22,    72,   278,    65,    66,    73,
     310,    22,    22,    32,    70,     6,     7,     8,    70,   290,
      70,    76,    34,   298,    72,    74,    17,    74,   299,    60,
      61,    62,    63,    64,    65,     7,   308,   280,   138,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,   204,   203,   136,   140,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    74,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
       6,     7,     8,    60,    -1,    -1,    72,    73,    65,    -1,
      67,    17,    69,    -1,    -1,    72,    73,    -1,    75,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    -1,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    -1,    -1,    -1,    42,    -1,    72,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,     6,
       7,     8,    60,    -1,    -1,    72,    -1,    65,    -1,    67,
      17,    69,    -1,    -1,    -1,    73,    -1,    75,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    21,    22,    23,    -1,    -1,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    42,    -1,    72,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,     6,     7,
       8,    60,    -1,    -1,    72,    -1,    65,    -1,    67,    17,
      69,    -1,    -1,    -1,    73,    -1,    75,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,     6,     7,     8,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,
      70,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,     6,     7,     8,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    70,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    -1,    -1,    -1,    70,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,     6,     7,     8,
      12,    70,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      22,    23,    -1,    25,    -1,    -1,    28,    29,    -1,    31,
      -1,    33,    -1,    35,    36,    -1,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    -1,    -1,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    12,    -1,    -1,
      -1,    70,    74,    75,    -1,    -1,    -1,    22,    23,    -1,
      25,    -1,    -1,    28,    29,    -1,    31,    -1,    33,    -1,
      35,    36,    -1,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
       6,     7,     8,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      16,    17,    -1,    -1,    -1,    22,    23,    -1,    25,    74,
      75,    28,    29,    -1,    31,    -1,    33,    -1,    35,    36,
      -1,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    17,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    -1,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,    79,    24,    37,    80,    81,    12,
      18,    19,    20,    21,    22,    23,    42,    60,    65,    67,
      69,    73,    75,   102,   103,    12,    22,    23,    25,    28,
      29,    31,    33,    35,    36,    38,    40,    41,    43,    46,
      47,    48,    49,    50,    51,    52,    53,    75,    85,    86,
     104,   105,     0,    22,    22,    81,    22,   105,    69,   102,
     100,   101,   102,   102,   102,    21,    94,    95,    96,    97,
      98,    99,   102,   103,    22,     6,     7,     8,    17,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,   105,    69,    72,    73,    84,    72,    72,   105,   102,
      69,    69,    22,    69,    69,   105,    22,    69,    84,    84,
      72,   102,    84,    87,    69,    84,    22,    69,    22,    86,
      17,    65,    66,    84,    69,    13,    14,    66,    74,   102,
     106,   107,    71,    77,    70,    76,    71,    74,    71,    74,
      71,    74,    15,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,    74,
      22,    92,    93,   106,    74,    85,    72,    16,    88,   102,
     105,    39,    22,    93,   102,    72,    39,    93,    72,    60,
     105,   102,    27,   102,   103,   102,   104,   102,    22,   104,
      22,    82,    83,   102,   102,    22,    70,    71,   101,    18,
      19,    21,    42,    65,    73,    95,    97,    21,    99,   102,
      77,    76,    70,    71,    70,    74,    75,    72,    70,    56,
     102,    70,    70,   102,    70,    22,     9,    10,    11,    56,
      70,    69,    84,    70,    77,    69,    70,    71,    74,    74,
      69,   102,   100,    98,   102,    73,    84,    93,    72,    22,
      72,   102,    84,    84,    84,    84,    84,   102,   102,   102,
     102,    73,    22,    84,   106,    84,    22,   106,    77,    74,
      44,    45,    72,    32,    72,    72,    72,    72,    26,    30,
      90,    91,    70,    70,    70,    84,    84,   102,    84,   102,
      76,    74,    91,    84,    72,    74,    44,    74,    72,    76,
      85,    34,    89,    84,   105,    85,    84,    74,    56,   102,
      70,    84
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (yyparam, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, yyparam); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *yyparam)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, yyparam)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    void *yyparam;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (yyparam);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *yyparam)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, yyparam)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    void *yyparam;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, yyparam);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void *yyparam)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, yyparam)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    void *yyparam;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , yyparam);
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, yyparam); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void *yyparam)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, yyparam)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    void *yyparam;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (yyparam);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 102: /* "exp" */
#line 109 "dm.ypp"
	{ delete (yyvaluep->node); };
#line 1619 "dm.tab.cpp"
	break;

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void *yyparam);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *yyparam)
#else
int
yyparse (yyparam)
    void *yyparam;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 115 "dm.ypp"
    { ((yyparse_param*) yyparam)->actnlist = (yyvsp[(2) - (2)].alist); ;}
    break;

  case 3:
#line 116 "dm.ypp"
    { ((yyparse_param*) yyparam)->expr = (yyvsp[(2) - (2)].node); ;}
    break;

  case 4:
#line 117 "dm.ypp"
    { ((yyparse_param*) yyparam)->stmtlist = (yyvsp[(2) - (2)].slist); ;}
    break;

  case 5:
#line 120 "dm.ypp"
    { (yyval.alist) = new ActionNodeList(); (yyval.alist)->add((yyvsp[(1) - (1)].actn)); ;}
    break;

  case 6:
#line 121 "dm.ypp"
    { (yyval.alist) = (yyvsp[(1) - (2)].alist); (yyval.alist)->add((yyvsp[(2) - (2)].actn)); ;}
    break;

  case 7:
#line 124 "dm.ypp"
    { (yyval.actn) = new ActionNode((yylsp[(1) - (3)]), (yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].slist)); ;}
    break;

  case 8:
#line 125 "dm.ypp"
    { (yyval.actn) = new ActionNode((yylsp[(1) - (6)]), (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].nlist), (yyvsp[(6) - (6)].slist)); ;}
    break;

  case 9:
#line 128 "dm.ypp"
    { (yyval.nlist) = NULL; ;}
    break;

  case 10:
#line 129 "dm.ypp"
    { (yyval.nlist) = (yyvsp[(1) - (1)].nlist); ;}
    break;

  case 11:
#line 131 "dm.ypp"
    { (yyval.nlist) = new NodeList(); (yyval.nlist)->add(new Node(NODE_STR, (yyvsp[(1) - (1)].str))); ;}
    break;

  case 12:
#line 132 "dm.ypp"
    { (yyval.nlist) = (yyvsp[(1) - (3)].nlist); (yyval.nlist)->add(new Node(NODE_STR, (yyvsp[(3) - (3)].str))); ;}
    break;

  case 13:
#line 135 "dm.ypp"
    { (yyval.slist) = NULL; ;}
    break;

  case 14:
#line 136 "dm.ypp"
    { (yyval.slist) = new StmtList(); ;}
    break;

  case 15:
#line 137 "dm.ypp"
    { (yyval.slist) = (yyvsp[(2) - (3)].slist); ;}
    break;

  case 16:
#line 140 "dm.ypp"
    { (yyval.slist) = new StmtList(); (yyval.slist)->add((yyvsp[(1) - (1)].stmt)); ;}
    break;

  case 17:
#line 141 "dm.ypp"
    { (yyval.slist) = (yyvsp[(1) - (2)].slist); (yyval.slist)->add((yyvsp[(2) - (2)].stmt)) ;}
    break;

  case 18:
#line 144 "dm.ypp"
    { (yyval.stmt) = new IfStmt((yylsp[(1) - (5)]), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].slist)); ;}
    break;

  case 19:
#line 145 "dm.ypp"
    { (yyval.stmt) = new IfStmt((yylsp[(1) - (7)]), (yyvsp[(3) - (7)].node), (yyvsp[(5) - (7)].slist), (yyvsp[(7) - (7)].slist)); ;}
    break;

  case 20:
#line 146 "dm.ypp"
    { (yyval.stmt) = new WhileStmt((yylsp[(1) - (5)]), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].slist)); ;}
    break;

  case 21:
#line 148 "dm.ypp"
    { (yyval.stmt) = new ForStmt((yylsp[(1) - (13)]), (yyvsp[(3) - (13)].node), (yyvsp[(5) - (13)].node), (yyvsp[(7) - (13)].node), (yyvsp[(9) - (13)].node), (yyvsp[(11) - (13)].node), (yyvsp[(13) - (13)].slist)); ;}
    break;

  case 22:
#line 149 "dm.ypp"
    { (yyval.stmt) = new ForEachStmt((yylsp[(1) - (5)]), (yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].slist), false); ;}
    break;

  case 23:
#line 150 "dm.ypp"
    { (yyval.stmt) = new ForEachStmt((yylsp[(1) - (5)]), new Stmt((yylsp[(1) - (5)]), (yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].node) ), (yyvsp[(5) - (5)].slist), false); ;}
    break;

  case 24:
#line 151 "dm.ypp"
    { (yyval.stmt) = new ForEachStmt((yylsp[(1) - (5)]), (yyvsp[(3) - (5)].stmt), (yyvsp[(5) - (5)].slist), true); ;}
    break;

  case 25:
#line 152 "dm.ypp"
    { (yyval.stmt) = new ForEachStmt((yylsp[(1) - (5)]), new Stmt((yylsp[(1) - (5)]), (yyvsp[(2) - (5)].str), (yyvsp[(4) - (5)].node) ), (yyvsp[(5) - (5)].slist), true); ;}
    break;

  case 26:
#line 153 "dm.ypp"
    { (yyval.stmt) = new SetStmt((yylsp[(1) - (6)]), (yyvsp[(2) - (6)].str), (yyvsp[(3) - (6)].node), (yyvsp[(5) - (6)].node)); ;}
    break;

  case 27:
#line 154 "dm.ypp"
    { (yyval.stmt) = new SetStmt((yylsp[(1) - (6)]), (yyvsp[(2) - (6)].str), (yyvsp[(3) - (6)].node), new Node(
     							NODE_PLUS,  new Node(NODE_NLVAR, (yyvsp[(3) - (6)].node)->deepCopy()), (yyvsp[(5) - (6)].node))); ;}
    break;

  case 28:
#line 156 "dm.ypp"
    { (yyval.stmt) = new SetStmt((yylsp[(1) - (6)]), (yyvsp[(2) - (6)].str), (yyvsp[(3) - (6)].node), new Node(
     							NODE_MINUS, new Node(NODE_NLVAR, (yyvsp[(3) - (6)].node)->deepCopy()), (yyvsp[(5) - (6)].node))); ;}
    break;

  case 29:
#line 158 "dm.ypp"
    { (yyval.stmt) = new SetStmt((yylsp[(1) - (6)]), (yyvsp[(2) - (6)].str), (yyvsp[(3) - (6)].node), new Node(
     							NODE_TIMES, new Node(NODE_NLVAR, (yyvsp[(3) - (6)].node)->deepCopy()), (yyvsp[(5) - (6)].node))); ;}
    break;

  case 30:
#line 160 "dm.ypp"
    { (yyval.stmt) = new SetStmt((yylsp[(1) - (3)]), NULL, (yyvsp[(2) - (3)].node), new Node(
     							NODE_PLUS, new Node(NODE_NLVAR,
     								(yyvsp[(2) - (3)].node)->deepCopy()), new Node(1))); ;}
    break;

  case 31:
#line 163 "dm.ypp"
    { (yyval.stmt) = new SetStmt((yylsp[(1) - (3)]), NULL, (yyvsp[(2) - (3)].node), new Node(
     							NODE_MINUS, new Node(NODE_NLVAR,
     								(yyvsp[(2) - (3)].node)->deepCopy()), new Node(1))); ;}
    break;

  case 32:
#line 166 "dm.ypp"
    { (yyval.stmt) = new EchoStmt((yylsp[(1) - (4)]), (yyvsp[(2) - (4)].node), (yyvsp[(3) - (4)].str)); ;}
    break;

  case 33:
#line 167 "dm.ypp"
    { (yyval.stmt) = new TryCatchStmt((yylsp[(1) - (8)]), (yyvsp[(2) - (8)].slist), (yyvsp[(5) - (8)].str), (yyvsp[(7) - (8)].slist), (yyvsp[(8) - (8)].slist)); ;}
    break;

  case 34:
#line 168 "dm.ypp"
    { (yyval.stmt) = new SwitchStmt((yylsp[(1) - (7)]), (yyvsp[(3) - (7)].node), (yyvsp[(6) - (7)].clist) ); ;}
    break;

  case 35:
#line 169 "dm.ypp"
    { (yyval.stmt) = new Stmt((yylsp[(1) - (2)]), STMT_BREAK); ;}
    break;

  case 36:
#line 170 "dm.ypp"
    { (yyval.stmt) = new Stmt((yylsp[(1) - (2)]), STMT_CONTINUE); ;}
    break;

  case 37:
#line 171 "dm.ypp"
    { (yyval.stmt) = new PsLoopStmt((yylsp[(1) - (2)]), (yyvsp[(2) - (2)].slist)); ;}
    break;

  case 38:
#line 172 "dm.ypp"
    { (yyval.stmt) = new ParallelStmt((yylsp[(1) - (2)]), (yyvsp[(2) - (2)].slist)); ;}
    break;

  case 39:
#line 173 "dm.ypp"
    { (yyval.stmt) = new SequentialStmt((yylsp[(1) - (2)]), (yyvsp[(2) - (2)].slist)); ;}
    break;

  case 40:
#line 174 "dm.ypp"
    { (yyval.stmt) = new UsingStmt((yylsp[(1) - (4)]), (yyvsp[(2) - (4)].str), (yyvsp[(3) - (4)].node), (yyvsp[(4) - (4)].slist)); ;}
    break;

  case 41:
#line 175 "dm.ypp"
    { (yyval.stmt) = new ReturnStmt((yylsp[(1) - (2)])); ;}
    break;

  case 42:
#line 176 "dm.ypp"
    { (yyval.stmt) = new ReturnStmt((yylsp[(1) - (3)]), (yyvsp[(2) - (3)].node)); ;}
    break;

  case 43:
#line 177 "dm.ypp"
    { (yyval.stmt) = new EvalStmt((yylsp[(1) - (5)]), (yyvsp[(3) - (5)].node)); ;}
    break;

  case 44:
#line 178 "dm.ypp"
    { (yyval.stmt) = new ExtendedStmt((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].slist)); ;}
    break;

  case 45:
#line 179 "dm.ypp"
    { (yyval.stmt) = new ExtendedStmt((yylsp[(1) - (5)]), (yyvsp[(1) - (5)].str), (yyvsp[(3) - (5)].slist), (yyvsp[(5) - (5)].slist)); ;}
    break;

  case 46:
#line 181 "dm.ypp"
    { (yyval.stmt) = new ExtendedStmt((yylsp[(1) - (8)]), (yyvsp[(1) - (8)].str), (yyvsp[(3) - (8)].slist), (yyvsp[(7) - (8)].slist), NULL); ;}
    break;

  case 47:
#line 183 "dm.ypp"
    { (yyval.stmt) = new ExtendedStmt((yylsp[(1) - (8)]), (yyvsp[(1) - (8)].str), (yyvsp[(3) - (8)].slist), NULL, (yyvsp[(7) - (8)].slist)); ;}
    break;

  case 48:
#line 185 "dm.ypp"
    { (yyval.stmt) = new ExtendedStmt((yylsp[(1) - (10)]), (yyvsp[(1) - (10)].str), (yyvsp[(3) - (10)].slist), (yyvsp[(7) - (10)].slist), (yyvsp[(9) - (10)].slist)); ;}
    break;

  case 49:
#line 186 "dm.ypp"
    { (yyval.stmt) = new ExtendedStmt(new FunctionNode((yylsp[(1) - (5)]), (yyvsp[(1) - (5)].str), (yyvsp[(3) - (5)].nlist))); ;}
    break;

  case 50:
#line 187 "dm.ypp"
    { (yyval.stmt) = new ExtendedStmt(new Node(NODE_OBJFUNC, (yyvsp[(1) - (7)].node), (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].nlist))); ;}
    break;

  case 51:
#line 190 "dm.ypp"
    { (yyval.str) = NULL; ;}
    break;

  case 52:
#line 191 "dm.ypp"
    { (yyval.str) = concatenate_string("-", (yyvsp[(3) - (3)].str)); SAFE_FREE((yyvsp[(3) - (3)].str));
						  if((yyvsp[(1) - (3)].str)) {
     						      char *temp = concatenate_string((yyvsp[(1) - (3)].str), (yyval.str));
     						      SAFE_FREE((yyvsp[(1) - (3)].str)); SAFE_FREE((yyval.str)); (yyval.str) = temp;
     						  }
     						;}
    break;

  case 53:
#line 199 "dm.ypp"
    { (yyval.str) = NULL; ;}
    break;

  case 54:
#line 200 "dm.ypp"
    { (yyval.str) = (yyvsp[(3) - (3)].str); ;}
    break;

  case 55:
#line 203 "dm.ypp"
    { (yyval.slist) = NULL; ;}
    break;

  case 56:
#line 204 "dm.ypp"
    { (yyval.slist) = (yyvsp[(2) - (2)].slist); ;}
    break;

  case 57:
#line 207 "dm.ypp"
    { (yyval.clist) = new CaseStmtList(); (yyval.clist)->add((yyvsp[(1) - (1)].cstmt)); ;}
    break;

  case 58:
#line 208 "dm.ypp"
    { (yyval.clist) = (yyvsp[(1) - (2)].clist); (yyval.clist)->add((yyvsp[(2) - (2)].cstmt)); ;}
    break;

  case 59:
#line 211 "dm.ypp"
    { (yyval.cstmt) = new CaseStmt((yylsp[(1) - (4)]), (yyvsp[(2) - (4)].node), (yyvsp[(4) - (4)].slist)); ;}
    break;

  case 60:
#line 212 "dm.ypp"
    { (yyval.cstmt) = new CaseStmt((yylsp[(1) - (3)]), (yyvsp[(3) - (3)].slist)); ;}
    break;

  case 61:
#line 215 "dm.ypp"
    { (yyval.slist) = new StmtList(); (yyval.slist)->add((yyvsp[(1) - (1)].stmt)); ;}
    break;

  case 62:
#line 216 "dm.ypp"
    { (yyval.slist) = (yyvsp[(1) - (3)].slist); (yyval.slist)->add((yyvsp[(3) - (3)].stmt)); ;}
    break;

  case 63:
#line 219 "dm.ypp"
    { (yyval.stmt) = new Stmt((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].node) ); ;}
    break;

  case 64:
#line 222 "dm.ypp"
    { (yyval.nlist) = new NodeList(); ;}
    break;

  case 65:
#line 223 "dm.ypp"
    { (yyval.nlist) = new NodeList(); (yyval.nlist)->add((yyvsp[(1) - (1)].node)); ;}
    break;

  case 66:
#line 224 "dm.ypp"
    { (yyval.nlist) = (yyvsp[(1) - (3)].nlist); (yyval.nlist)->add((yyvsp[(3) - (3)].node)); ;}
    break;

  case 67:
#line 227 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRELE, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 68:
#line 230 "dm.ypp"
    { (yyval.nlist) = new NodeList(); (yyval.nlist)->add((yyvsp[(1) - (1)].node)); ;}
    break;

  case 69:
#line 231 "dm.ypp"
    { (yyval.nlist) = (yyvsp[(1) - (3)].nlist); (yyval.nlist)->add((yyvsp[(3) - (3)].node)); ;}
    break;

  case 70:
#line 234 "dm.ypp"
    { (yyval.node) = new Node(NODE_LISTELE, (yyvsp[(1) - (1)].node)); ;}
    break;

  case 71:
#line 237 "dm.ypp"
    { (yyval.nlist) = new NodeList(); ;}
    break;

  case 72:
#line 238 "dm.ypp"
    { (yyval.nlist) = new NodeList(); (yyval.nlist)->add((yyvsp[(1) - (1)].node)); ;}
    break;

  case 73:
#line 239 "dm.ypp"
    { (yyval.nlist) = (yyvsp[(1) - (3)].nlist); (yyval.nlist)->add((yyvsp[(3) - (3)].node)); ;}
    break;

  case 74:
#line 243 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRELE, new Node(NODE_STR, (yyvsp[(1) - (3)].str)), new Node((yyvsp[(3) - (3)].ival))); ;}
    break;

  case 75:
#line 244 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRELE, new Node(NODE_STR, (yyvsp[(1) - (3)].str)), new Node(NODE_STR, (yyvsp[(3) - (3)].str),true)); ;}
    break;

  case 76:
#line 245 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRELE, new Node(NODE_STR, (yyvsp[(1) - (3)].str)), new Node((yyvsp[(3) - (3)].bval))); ;}
    break;

  case 77:
#line 246 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRELE, new Node(NODE_STR, (yyvsp[(1) - (3)].str)), new Node(NODE_STR, strdup(""))); ;}
    break;

  case 78:
#line 247 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRELE, new Node(NODE_STR, (yyvsp[(1) - (5)].str)), new Node(NODE_ARRINI, NULL, (yyvsp[(4) - (5)].nlist))); ;}
    break;

  case 79:
#line 248 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRELE, new Node(NODE_STR, (yyvsp[(1) - (5)].str)), new Node(NODE_LISTINI, NULL, (yyvsp[(4) - (5)].nlist))); ;}
    break;

  case 80:
#line 251 "dm.ypp"
    { (yyval.nlist) = new NodeList(); ;}
    break;

  case 81:
#line 252 "dm.ypp"
    { (yyval.nlist) = new NodeList(); (yyval.nlist)->add((yyvsp[(1) - (1)].node)); ;}
    break;

  case 82:
#line 253 "dm.ypp"
    { (yyval.nlist) = (yyvsp[(1) - (3)].nlist); (yyval.nlist)->add((yyvsp[(3) - (3)].node)); ;}
    break;

  case 83:
#line 256 "dm.ypp"
    { (yyval.node) = new Node(NODE_LISTELE, (yyvsp[(1) - (1)].node)); ;}
    break;

  case 84:
#line 259 "dm.ypp"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 85:
#line 260 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRINI, NULL, (yyvsp[(2) - (3)].nlist)); ;}
    break;

  case 86:
#line 261 "dm.ypp"
    { (yyval.node) = new Node(NODE_LISTINI, NULL, (yyvsp[(2) - (3)].nlist)); ;}
    break;

  case 87:
#line 262 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRINI, NULL, (yyvsp[(2) - (3)].nlist)); ;}
    break;

  case 88:
#line 263 "dm.ypp"
    { (yyval.node) = new Node(NODE_LISTINI, NULL, (yyvsp[(2) - (3)].nlist)); ;}
    break;

  case 89:
#line 266 "dm.ypp"
    { (yyval.node) = new Node((yyvsp[(1) - (1)].bval));		;}
    break;

  case 90:
#line 267 "dm.ypp"
    { (yyval.node) = new Node(NODE_NULL, (char*) NULL);     ;}
    break;

  case 91:
#line 268 "dm.ypp"
    { (yyval.node) = new Node((yyvsp[(1) - (1)].ival));		;}
    break;

  case 92:
#line 269 "dm.ypp"
    { (yyval.node) = new Node(NODE_STR, (yyvsp[(1) - (1)].str));	;}
    break;

  case 93:
#line 270 "dm.ypp"
    { (yyval.node) = new Node(NODE_STR, (yyvsp[(1) - (1)].str), true);	;}
    break;

  case 94:
#line 271 "dm.ypp"
    { (yyval.node) = new Node(NODE_LOOKUP, (yyvsp[(2) - (2)].str));	;}
    break;

  case 95:
#line 272 "dm.ypp"
    { (yyval.node) = new Node(NODE_LOOKUP, (yyvsp[(1) - (1)].str));	;}
    break;

  case 96:
#line 273 "dm.ypp"
    { (yyval.node) = new Node(NODE_NLVAR, (yyvsp[(2) - (3)].node));	;}
    break;

  case 97:
#line 274 "dm.ypp"
    { (yyval.node) = new Node(NODE_IFUNSET, new Node(NODE_NLVAR, (yyvsp[(2) - (5)].node)), (yyvsp[(4) - (5)].node));	;}
    break;

  case 98:
#line 275 "dm.ypp"
    { (yyval.node) = new Node(NODE_IFSET, new Node(NODE_NLVAR, (yyvsp[(2) - (5)].node)), (yyvsp[(4) - (5)].node)); ;}
    break;

  case 99:
#line 276 "dm.ypp"
    { (yyval.node) = new Node(NODE_CONC, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 100:
#line 277 "dm.ypp"
    { (yyval.node) = new Node(NODE_PLUS, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));		;}
    break;

  case 101:
#line 278 "dm.ypp"
    { (yyval.node) = new Node(NODE_MINUS, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));		;}
    break;

  case 102:
#line 279 "dm.ypp"
    { (yyval.node) = new Node(NODE_TIMES, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); 		;}
    break;

  case 103:
#line 280 "dm.ypp"
    { (yyval.node) = new Node(NODE_DIV, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));  		;}
    break;

  case 104:
#line 281 "dm.ypp"
    { (yyval.node) = new Node(NODE_MOD, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));   		;}
    break;

  case 105:
#line 282 "dm.ypp"
    { (yyval.node) = new Node(NODE_AND, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));    	;}
    break;

  case 106:
#line 283 "dm.ypp"
    { (yyval.node) = new Node(NODE_OR, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));	    	;}
    break;

  case 107:
#line 284 "dm.ypp"
    { (yyval.node) = new Node(NODE_NEG, (yyvsp[(2) - (2)].node));       		;}
    break;

  case 108:
#line 285 "dm.ypp"
    { (yyval.node) = new Node(NODE_NOT, (yyvsp[(2) - (2)].node));       		;}
    break;

  case 109:
#line 286 "dm.ypp"
    { (yyval.node) = (yyvsp[(2) - (3)].node);			;}
    break;

  case 110:
#line 287 "dm.ypp"
    { (yyval.node) = new Node(NODE_EQ, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 111:
#line 288 "dm.ypp"
    { (yyval.node) = new Node(NODE_NE, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 112:
#line 289 "dm.ypp"
    { (yyval.node) = new Node(NODE_GT, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 113:
#line 290 "dm.ypp"
    { (yyval.node) = new Node(NODE_GE, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 114:
#line 291 "dm.ypp"
    { (yyval.node) = new Node(NODE_LT, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 115:
#line 292 "dm.ypp"
    { (yyval.node) = new Node(NODE_LE, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 116:
#line 293 "dm.ypp"
    { (yyval.node) = new Node(NODE_MATCH, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 117:
#line 294 "dm.ypp"
    { (yyval.node) = new FunctionNode((yylsp[(1) - (4)]), (yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].nlist)); ;}
    break;

  case 118:
#line 295 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRAY, (yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node));	;}
    break;

  case 119:
#line 298 "dm.ypp"
    { (yyval.node) = new Node(NODE_IDOROBJ, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 120:
#line 299 "dm.ypp"
    { (yyval.node) = new Node(NODE_LOOKUP, (yyvsp[(2) - (2)].str));	;}
    break;

  case 121:
#line 300 "dm.ypp"
    { (yyval.node) = new Node(NODE_LOOKUP, (yyvsp[(1) - (1)].str));	;}
    break;

  case 122:
#line 301 "dm.ypp"
    { (yyval.node) = new Node(NODE_NLVAR, (yyvsp[(2) - (3)].node)); ;}
    break;

  case 123:
#line 304 "dm.ypp"
    { (yyval.node)= (yyvsp[(1) - (1)].node); ;}
    break;

  case 124:
#line 305 "dm.ypp"
    { (yyval.node) = new Node(NODE_CONC, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 125:
#line 306 "dm.ypp"
    { (yyval.node) = new Node(NODE_ARRAY, new Node(NODE_NLVAR, (yyvsp[(1) - (4)].node)), (yyvsp[(3) - (4)].node)); ;}
    break;

  case 126:
#line 307 "dm.ypp"
    { (yyval.node) = new Node(NODE_OBJECT, new Node(NODE_NLVAR, (yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 127:
#line 309 "dm.ypp"
    { (yyval.node) = new Node(NODE_OBJFUNC, (yyvsp[(1) - (6)].node), (yyvsp[(3) - (6)].str), (yyvsp[(5) - (6)].nlist)); ;}
    break;

  case 128:
#line 312 "dm.ypp"
    { (yyval.nlist) = NULL; ;}
    break;

  case 129:
#line 313 "dm.ypp"
    { (yyval.nlist) = (yyvsp[(1) - (1)].nlist); ;}
    break;

  case 130:
#line 316 "dm.ypp"
    { (yyval.nlist) = new NodeList(); (yyval.nlist)->add((yyvsp[(1) - (1)].node)); ;}
    break;

  case 131:
#line 317 "dm.ypp"
    { (yyval.nlist) = (yyvsp[(1) - (3)].nlist); (yyval.nlist)->add((yyvsp[(3) - (3)].node));	;}
    break;


/* Line 1267 of yacc.c.  */
#line 2605 "dm.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (yyparam, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yyparam, yymsg);
	  }
	else
	  {
	    yyerror (yyparam, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, yyparam);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, yyparam);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (yyparam, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, yyparam);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, yyparam);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 319 "dm.ypp"



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

