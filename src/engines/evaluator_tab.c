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
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse so_evalparse
#define yylex   so_evallex
#define yyerror so_evalerror
#define yylval  so_evallval
#define yychar  so_evalchar
#define yydebug so_evaldebug
#define yynerrs so_evalnerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LEX_VALUE = 258,
     LEX_TMP_FLT_REG = 259,
     LEX_IN_FLT_REG = 260,
     LEX_OUT_FLT_REG = 261,
     LEX_TMP_VEC_REG = 262,
     LEX_IN_VEC_REG = 263,
     LEX_OUT_VEC_REG = 264,
     LEX_COMPARE = 265,
     LEX_FLTFUNC = 266,
     LEX_ATAN2 = 267,
     LEX_POW = 268,
     LEX_FMOD = 269,
     LEX_LEN = 270,
     LEX_CROSS = 271,
     LEX_DOT = 272,
     LEX_NORMALIZE = 273,
     LEX_VEC3F = 274,
     LEX_ERROR = 275,
     LEX_OR = 276,
     LEX_AND = 277,
     LEX_NEQ = 278,
     LEX_EQ = 279,
     UNARY = 280
   };
#endif
/* Tokens.  */
#define LEX_VALUE 258
#define LEX_TMP_FLT_REG 259
#define LEX_IN_FLT_REG 260
#define LEX_OUT_FLT_REG 261
#define LEX_TMP_VEC_REG 262
#define LEX_IN_VEC_REG 263
#define LEX_OUT_VEC_REG 264
#define LEX_COMPARE 265
#define LEX_FLTFUNC 266
#define LEX_ATAN2 267
#define LEX_POW 268
#define LEX_FMOD 269
#define LEX_LEN 270
#define LEX_CROSS 271
#define LEX_DOT 272
#define LEX_NORMALIZE 273
#define LEX_VEC3F 274
#define LEX_ERROR 275
#define LEX_OR 276
#define LEX_AND 277
#define LEX_NEQ 278
#define LEX_EQ 279
#define UNARY 280




/* Copy the first part of user declarations.  */


/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*
 * Syntax analyzer for SoCalculator expressions.
 *
 * Compile with
 *
 *         bison -p so_eval -o evaluator_tab.c -l evaluator.y
 *
 * ..with GNU bison version 1.27 (which is what we have on nfs.sim.no)
 * then patch the resulting evaluator_tab.c file with
 *
 *         patch -p0 < evaluator_tab.diff
 *
 * The patch is explained at the top of the diff file.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_IO_H
/* isatty() on windows */
#include <io.h>
#endif /* HAVE_IO_H */
#include <Inventor/C/basic.h>
#include "engines/evaluator.h"


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE

{
  int id;
  float value;
  char  reg;
  so_eval_node *node;
}
/* Line 187 of yacc.c.  */

	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


  static char * get_regname(char reg, int regtype);
  enum { REGTYPE_IN, REGTYPE_OUT, REGTYPE_TMP };
  static so_eval_node *root_node;
  static int so_evalerror(const char *);
  static int so_evallex(void);


/* Line 216 of yacc.c.  */


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
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   500

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  8
/* YYNRULES -- Number of rules.  */
#define YYNRULES  60
/* YYNRULES -- Number of states.  */
#define YYNSTATES  157

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   280

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    39,     2,     2,     2,    38,     2,     2,
      23,    24,    36,    34,    20,    35,     2,    37,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    29,    25,
       2,    27,     2,    28,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    21,     2,    22,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    26,    30,    31,    32,    33,
      40
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     7,     9,    10,    14,    18,    20,    22,
      27,    32,    34,    36,    42,    48,    54,    58,    62,    66,
      70,    74,    81,    88,    95,    98,   102,   107,   112,   119,
     121,   123,   125,   130,   135,   140,   142,   148,   154,   160,
     164,   168,   172,   176,   180,   187,   190,   194,   199,   201,
     203,   205,   214,   218,   222,   226,   230,   234,   238,   242,
     245
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      42,     0,    -1,    42,    25,    43,    -1,    43,    -1,    -1,
      44,    27,    46,    -1,    45,    27,    47,    -1,     4,    -1,
       6,    -1,     7,    21,     3,    22,    -1,     9,    21,     3,
      22,    -1,     7,    -1,     9,    -1,    48,    28,    46,    29,
      46,    -1,    46,    28,    46,    29,    46,    -1,    47,    28,
      46,    29,    46,    -1,    46,    34,    46,    -1,    46,    35,
      46,    -1,    46,    37,    46,    -1,    46,    36,    46,    -1,
      46,    38,    46,    -1,    12,    23,    46,    20,    46,    24,
      -1,    13,    23,    46,    20,    46,    24,    -1,    14,    23,
      46,    20,    46,    24,    -1,    35,    46,    -1,    23,    46,
      24,    -1,    11,    23,    46,    24,    -1,    15,    23,    47,
      24,    -1,    17,    23,    47,    20,    47,    24,    -1,     4,
      -1,     6,    -1,     5,    -1,     7,    21,     3,    22,    -1,
       8,    21,     3,    22,    -1,     9,    21,     3,    22,    -1,
       3,    -1,    48,    28,    47,    29,    47,    -1,    46,    28,
      47,    29,    47,    -1,    47,    28,    47,    29,    47,    -1,
      47,    34,    47,    -1,    47,    35,    47,    -1,    47,    36,
      46,    -1,    47,    37,    46,    -1,    46,    36,    47,    -1,
      16,    23,    47,    20,    47,    24,    -1,    35,    47,    -1,
      23,    47,    24,    -1,    18,    23,    47,    24,    -1,     7,
      -1,     9,    -1,     8,    -1,    19,    23,    46,    20,    46,
      20,    46,    24,    -1,    46,    33,    46,    -1,    46,    32,
      46,    -1,    47,    33,    47,    -1,    47,    32,    47,    -1,
      46,    10,    46,    -1,    48,    31,    48,    -1,    48,    30,
      48,    -1,    39,    48,    -1,    23,    48,    24,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   105,   105,   107,   110,   111,   112,   115,   116,   117,
     119,   123,   124,   127,   129,   131,   134,   135,   136,   137,
     138,   139,   141,   143,   146,   147,   148,   149,   150,   152,
     153,   154,   155,   157,   159,   161,   165,   167,   169,   172,
     173,   174,   175,   176,   177,   180,   181,   182,   185,   186,
     187,   188,   192,   193,   194,   195,   196,   197,   198,   199,
     200
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LEX_VALUE", "LEX_TMP_FLT_REG",
  "LEX_IN_FLT_REG", "LEX_OUT_FLT_REG", "LEX_TMP_VEC_REG", "LEX_IN_VEC_REG",
  "LEX_OUT_VEC_REG", "LEX_COMPARE", "LEX_FLTFUNC", "LEX_ATAN2", "LEX_POW",
  "LEX_FMOD", "LEX_LEN", "LEX_CROSS", "LEX_DOT", "LEX_NORMALIZE",
  "LEX_VEC3F", "','", "'['", "']'", "'('", "')'", "';'", "LEX_ERROR",
  "'='", "'?'", "':'", "LEX_OR", "LEX_AND", "LEX_NEQ", "LEX_EQ", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'!'", "UNARY", "$accept", "expression",
  "subexpression", "fltlhs", "veclhs", "fltstatement", "vecstatement",
  "boolstatement", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      44,    91,    93,    40,    41,    59,   275,    61,    63,    58,
     276,   277,   278,   279,    43,    45,    42,    47,    37,    33,
     280
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    42,    42,    43,    43,    43,    44,    44,    44,
      44,    45,    45,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    47,    48,    48,    48,    48,    48,    48,    48,    48,
      48
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     1,     0,     3,     3,     1,     1,     4,
       4,     1,     1,     5,     5,     5,     3,     3,     3,     3,
       3,     6,     6,     6,     2,     3,     4,     4,     6,     1,
       1,     1,     4,     4,     4,     1,     5,     5,     5,     3,
       3,     3,     3,     3,     6,     2,     3,     4,     1,     1,
       1,     8,     3,     3,     3,     3,     3,     3,     3,     2,
       3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     7,     8,    11,    12,     0,     3,     0,     0,     0,
       0,     1,     4,     0,     0,     0,     0,     2,    35,    29,
      31,    30,    48,    50,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     0,     0,
       0,     6,     9,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
      45,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    25,    46,    60,    56,     0,     0,    53,
      52,    16,    17,    19,    43,    18,    20,     0,     0,    55,
      54,    39,    40,    41,    42,     0,     0,    58,    57,    32,
      33,    34,    26,     0,     0,     0,    27,     0,     0,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,    37,    15,    38,    13,    36,    21,
      22,    23,    44,    28,     0,     0,    51
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     5,     6,     7,     8,    40,    38,    39
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -34
static const yytype_int16 yypact[] =
{
      85,   -34,   -34,     2,    12,     6,   -34,   -12,    16,    42,
      45,   -34,    85,   121,   121,    39,    51,   -34,   -34,   -34,
     -34,   -34,    53,    54,    60,    59,    62,    67,    78,    79,
      90,    91,    92,    93,   121,   121,   121,    34,   -23,    10,
      34,   -23,   -34,   -34,    80,   116,   118,   121,   121,   121,
     121,   121,   121,   121,   121,   121,    -8,   377,    56,   -34,
     -34,   -34,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,    73,   101,   109,   138,   157,   168,   187,   391,   353,
     363,   405,   198,   -34,   -34,   -34,   342,   217,   443,   342,
     342,   -19,   -19,   -34,   -34,   -34,   -34,   228,   453,    72,
      72,   -29,   -29,    -6,    -6,   248,   463,   111,   -34,   -34,
     -34,   -34,   -34,   121,   121,   121,   -34,   121,   121,   -34,
     121,   121,   121,   121,   121,   121,   121,   263,   278,   293,
     419,   433,   312,    34,   -23,    34,   -23,    34,   -23,   -34,
     -34,   -34,   -34,   -34,   121,   327,   -34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -34,   -34,   131,   -34,   -34,   -13,    25,   -33
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      37,    58,    62,    61,    62,    71,    11,    76,    77,    72,
      73,    74,    75,    76,    77,    13,    93,    68,    69,    70,
      63,    56,    59,     9,    64,    65,    66,    67,    68,    69,
      70,    12,    70,    10,    84,    85,    86,    87,    78,    41,
      79,    80,    92,    14,    62,    15,   117,   118,    16,    96,
      97,    99,   100,   101,   102,   103,   105,   106,   107,    57,
      60,    42,    63,   113,   114,   115,    64,    65,    66,    67,
      68,    69,    70,    43,    44,    45,    88,    89,    90,    91,
      95,    46,    47,    81,    78,    48,    79,    80,    98,     1,
      49,     2,     3,   104,     4,   119,   108,   109,   110,   111,
     112,    50,    51,   116,    72,    73,    74,    75,    76,    77,
     137,   138,   139,    52,    53,    54,    55,   142,   143,    82,
     145,    83,   147,   120,    18,    19,    20,    21,    22,    23,
      24,   121,    25,    26,    27,    28,    29,    30,    31,    32,
      33,   155,    80,    17,    34,     0,     0,     0,    62,     0,
       0,     0,   140,   141,     0,     0,    35,   144,     0,   146,
      36,   148,   122,     0,     0,     0,    63,    62,     0,     0,
      64,    65,    66,    67,    68,    69,    70,   123,    62,     0,
       0,     0,     0,     0,     0,    63,     0,     0,   124,    64,
      65,    66,    67,    68,    69,    70,    63,    62,     0,     0,
      64,    65,    66,    67,    68,    69,    70,   125,    62,     0,
       0,     0,     0,     0,     0,    63,     0,     0,   130,    64,
      65,    66,    67,    68,    69,    70,    63,    62,     0,     0,
      64,    65,    66,    67,    68,    69,    70,     0,    62,     0,
       0,     0,     0,     0,     0,    63,   131,     0,     0,    64,
      65,    66,    67,    68,    69,    70,    63,   133,    62,     0,
      64,    65,    66,    67,    68,    69,    70,     0,     0,     0,
       0,     0,     0,    62,     0,     0,    63,   135,     0,     0,
      64,    65,    66,    67,    68,    69,    70,   149,    62,     0,
       0,    63,     0,     0,     0,    64,    65,    66,    67,    68,
      69,    70,   150,    62,     0,     0,    63,     0,     0,     0,
      64,    65,    66,    67,    68,    69,    70,   151,     0,     0,
       0,    63,    62,     0,     0,    64,    65,    66,    67,    68,
      69,    70,   154,     0,     0,     0,     0,    62,     0,     0,
      63,     0,     0,     0,    64,    65,    66,    67,    68,    69,
      70,   156,    62,     0,     0,    63,     0,     0,     0,    64,
      65,    66,    67,    68,    69,    70,     0,     0,     0,     0,
       0,     0,     0,   127,    64,    65,    66,    67,    68,    69,
      70,    71,     0,   128,     0,    72,    73,    74,    75,    76,
      77,    71,     0,     0,     0,    72,    73,    74,    75,    76,
      77,    94,     0,     0,     0,    71,     0,     0,     0,    72,
      73,    74,    75,    76,    77,   126,     0,     0,     0,    71,
       0,     0,     0,    72,    73,    74,    75,    76,    77,   129,
       0,     0,     0,    71,     0,     0,     0,    72,    73,    74,
      75,    76,    77,   152,     0,     0,     0,    71,     0,     0,
       0,    72,    73,    74,    75,    76,    77,   153,     0,     0,
       0,    71,     0,     0,     0,    72,    73,    74,    75,    76,
      77,    71,   132,     0,     0,    72,    73,    74,    75,    76,
      77,    71,   134,     0,     0,    72,    73,    74,    75,    76,
      77,    71,   136,     0,     0,    72,    73,    74,    75,    76,
      77
};

static const yytype_int16 yycheck[] =
{
      13,    34,    10,    36,    10,    28,     0,    36,    37,    32,
      33,    34,    35,    36,    37,    27,    24,    36,    37,    38,
      28,    34,    35,    21,    32,    33,    34,    35,    36,    37,
      38,    25,    38,    21,    47,    48,    49,    50,    28,    14,
      30,    31,    55,    27,    10,     3,    79,    80,     3,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    34,
      35,    22,    28,    76,    77,    78,    32,    33,    34,    35,
      36,    37,    38,    22,    21,    21,    51,    52,    53,    54,
      24,    21,    23,     3,    28,    23,    30,    31,    63,     4,
      23,     6,     7,    68,     9,    22,    71,    72,    73,    74,
      75,    23,    23,    78,    32,    33,    34,    35,    36,    37,
     123,   124,   125,    23,    23,    23,    23,   130,   131,     3,
     133,     3,   135,    22,     3,     4,     5,     6,     7,     8,
       9,    22,    11,    12,    13,    14,    15,    16,    17,    18,
      19,   154,    31,    12,    23,    -1,    -1,    -1,    10,    -1,
      -1,    -1,   127,   128,    -1,    -1,    35,   132,    -1,   134,
      39,   136,    24,    -1,    -1,    -1,    28,    10,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    20,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    20,    32,
      33,    34,    35,    36,    37,    38,    28,    10,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    20,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    20,    32,
      33,    34,    35,    36,    37,    38,    28,    10,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    32,
      33,    34,    35,    36,    37,    38,    28,    29,    10,    -1,
      32,    33,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    28,    29,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    24,    10,    -1,
      -1,    28,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    38,    24,    10,    -1,    -1,    28,    -1,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    24,    -1,    -1,
      -1,    28,    10,    -1,    -1,    32,    33,    34,    35,    36,
      37,    38,    20,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      28,    -1,    -1,    -1,    32,    33,    34,    35,    36,    37,
      38,    24,    10,    -1,    -1,    28,    -1,    -1,    -1,    32,
      33,    34,    35,    36,    37,    38,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    32,    33,    34,    35,    36,    37,
      38,    28,    -1,    20,    -1,    32,    33,    34,    35,    36,
      37,    28,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    24,    -1,    -1,    -1,    28,    -1,    -1,    -1,    32,
      33,    34,    35,    36,    37,    24,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    32,    33,    34,    35,    36,    37,    24,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    32,    33,    34,
      35,    36,    37,    24,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    32,    33,    34,    35,    36,    37,    24,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    28,    29,    -1,    -1,    32,    33,    34,    35,    36,
      37,    28,    29,    -1,    -1,    32,    33,    34,    35,    36,
      37,    28,    29,    -1,    -1,    32,    33,    34,    35,    36,
      37
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     6,     7,     9,    42,    43,    44,    45,    21,
      21,     0,    25,    27,    27,     3,     3,    43,     3,     4,
       5,     6,     7,     8,     9,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    23,    35,    39,    46,    47,    48,
      46,    47,    22,    22,    21,    21,    21,    23,    23,    23,
      23,    23,    23,    23,    23,    23,    46,    47,    48,    46,
      47,    48,    10,    28,    32,    33,    34,    35,    36,    37,
      38,    28,    32,    33,    34,    35,    36,    37,    28,    30,
      31,     3,     3,     3,    46,    46,    46,    46,    47,    47,
      47,    47,    46,    24,    24,    24,    46,    46,    47,    46,
      46,    46,    46,    46,    47,    46,    46,    46,    47,    47,
      47,    47,    47,    46,    46,    46,    47,    48,    48,    22,
      22,    22,    24,    20,    20,    20,    24,    20,    20,    24,
      20,    29,    29,    29,    29,    29,    29,    46,    46,    46,
      47,    47,    46,    46,    47,    46,    47,    46,    47,    24,
      24,    24,    24,    24,    20,    46,    24
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
      yyerror (YY_("syntax error: cannot back up")); \
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
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
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

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
int yyparse (void);
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
yyparse (void)
#else
int
yyparse ()

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



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


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


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

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

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

    { root_node = so_eval_create_binary(ID_SEPARATOR, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); (yyval.node) = root_node; ;}
    break;

  case 3:

    { root_node = (yyvsp[(1) - (1)].node); (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 4:

    { (yyval.node) = NULL ;}
    break;

  case 5:

    { (yyval.node) = so_eval_create_binary(ID_ASSIGN_FLT, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 6:

    { (yyval.node) = so_eval_create_binary(ID_ASSIGN_VEC, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 7:

    { (yyval.node) = so_eval_create_reg(get_regname((yyvsp[(1) - (1)].reg), REGTYPE_TMP)); ;}
    break;

  case 8:

    { (yyval.node) = so_eval_create_reg(get_regname((yyvsp[(1) - (1)].reg), REGTYPE_OUT)); ;}
    break;

  case 9:

    { (yyval.node) = so_eval_create_reg_comp(get_regname((yyvsp[(1) - (4)].reg), REGTYPE_TMP), (int) (yyvsp[(3) - (4)].value)); ;}
    break;

  case 10:

    { (yyval.node) = so_eval_create_reg_comp(get_regname((yyvsp[(1) - (4)].reg), REGTYPE_OUT), (int) (yyvsp[(3) - (4)].value)); ;}
    break;

  case 11:

    { (yyval.node) = so_eval_create_reg(get_regname((yyvsp[(1) - (1)].reg), REGTYPE_TMP));;}
    break;

  case 12:

    { (yyval.node) = so_eval_create_reg(get_regname((yyvsp[(1) - (1)].reg), REGTYPE_OUT));;}
    break;

  case 13:

    { (yyval.node) = so_eval_create_ternary(ID_FLT_COND, (yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 14:

    { (yyval.node) = so_eval_create_ternary(ID_FLT_COND, so_eval_create_unary(ID_TEST_FLT, (yyvsp[(1) - (5)].node)), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 15:

    { (yyval.node) = so_eval_create_ternary(ID_FLT_COND, so_eval_create_unary(ID_TEST_VEC, (yyvsp[(1) - (5)].node)), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 16:

    { (yyval.node) = so_eval_create_binary(ID_ADD, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 17:

    { (yyval.node) = so_eval_create_binary(ID_SUB, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 18:

    { (yyval.node) = so_eval_create_binary(ID_DIV, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 19:

    { (yyval.node) = so_eval_create_binary(ID_MUL, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 20:

    { (yyval.node) = so_eval_create_binary(ID_FMOD, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 21:

    { (yyval.node) = so_eval_create_binary(ID_ATAN2, (yyvsp[(3) - (6)].node), (yyvsp[(5) - (6)].node)); ;}
    break;

  case 22:

    { (yyval.node) = so_eval_create_binary(ID_POW, (yyvsp[(3) - (6)].node), (yyvsp[(5) - (6)].node)); ;}
    break;

  case 23:

    { (yyval.node) = so_eval_create_binary(ID_FMOD, (yyvsp[(3) - (6)].node), (yyvsp[(5) - (6)].node)); ;}
    break;

  case 24:

    { (yyval.node) = so_eval_create_unary(ID_NEG, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 25:

    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 26:

    { (yyval.node) = so_eval_create_unary((yyvsp[(1) - (4)].id), (yyvsp[(3) - (4)].node));;}
    break;

  case 27:

    { (yyval.node) = so_eval_create_unary(ID_LEN, (yyvsp[(3) - (4)].node));;}
    break;

  case 28:

    { (yyval.node) = so_eval_create_binary(ID_DOT, (yyvsp[(3) - (6)].node), (yyvsp[(5) - (6)].node)); ;}
    break;

  case 29:

    { (yyval.node) = so_eval_create_reg(get_regname((yyvsp[(1) - (1)].reg), REGTYPE_TMP));;}
    break;

  case 30:

    { (yyval.node) = so_eval_create_reg(get_regname((yyvsp[(1) - (1)].reg), REGTYPE_OUT));;}
    break;

  case 31:

    { (yyval.node) = so_eval_create_reg(get_regname((yyvsp[(1) - (1)].reg), REGTYPE_IN));;}
    break;

  case 32:

    { (yyval.node) = so_eval_create_reg_comp(get_regname((yyvsp[(1) - (4)].reg), REGTYPE_TMP), (int) (yyvsp[(3) - (4)].value));;}
    break;

  case 33:

    { (yyval.node) = so_eval_create_reg_comp(get_regname((yyvsp[(1) - (4)].reg), REGTYPE_IN), (int) (yyvsp[(3) - (4)].value));;}
    break;

  case 34:

    { (yyval.node) = so_eval_create_reg_comp(get_regname((yyvsp[(1) - (4)].reg), REGTYPE_OUT), (int) (yyvsp[(3) - (4)].value));;}
    break;

  case 35:

    { (yyval.node) = so_eval_create_flt_val((yyvsp[(1) - (1)].value)); ;}
    break;

  case 36:

    { (yyval.node) = so_eval_create_ternary(ID_VEC_COND, (yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 37:

    { (yyval.node) = so_eval_create_ternary(ID_VEC_COND, so_eval_create_unary(ID_TEST_FLT, (yyvsp[(1) - (5)].node)), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 38:

    { (yyval.node) = so_eval_create_ternary(ID_VEC_COND, so_eval_create_unary(ID_TEST_VEC, (yyvsp[(1) - (5)].node)), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 39:

    { (yyval.node) = so_eval_create_binary(ID_ADD_VEC, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 40:

    { (yyval.node) = so_eval_create_binary(ID_SUB_VEC, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 41:

    { (yyval.node) = so_eval_create_binary(ID_MUL_VEC_FLT, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 42:

    { (yyval.node) = so_eval_create_binary(ID_DIV_VEC_FLT, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 43:

    { (yyval.node) = so_eval_create_binary(ID_MUL_VEC_FLT, (yyvsp[(3) - (3)].node), (yyvsp[(1) - (3)].node)); ;}
    break;

  case 44:

    { (yyval.node) = so_eval_create_binary(ID_CROSS, (yyvsp[(3) - (6)].node), (yyvsp[(5) - (6)].node)); ;}
    break;

  case 45:

    { (yyval.node) = so_eval_create_unary(ID_NEG_VEC, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 46:

    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 47:

    { (yyval.node) = so_eval_create_unary(ID_NORMALIZE, (yyvsp[(3) - (4)].node)); ;}
    break;

  case 48:

    { (yyval.node) = so_eval_create_reg(get_regname((yyvsp[(1) - (1)].reg), REGTYPE_TMP));;}
    break;

  case 49:

    { (yyval.node) = so_eval_create_reg(get_regname((yyvsp[(1) - (1)].reg), REGTYPE_OUT));;}
    break;

  case 50:

    { (yyval.node) = so_eval_create_reg(get_regname((yyvsp[(1) - (1)].reg), REGTYPE_IN));;}
    break;

  case 51:

    { (yyval.node) = so_eval_create_ternary(ID_VEC3F, (yyvsp[(3) - (8)].node), (yyvsp[(5) - (8)].node), (yyvsp[(7) - (8)].node)); ;}
    break;

  case 52:

    { (yyval.node) = so_eval_create_binary(ID_EQ, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 53:

    { (yyval.node) = so_eval_create_binary(ID_NEQ, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 54:

    { (yyval.node) = so_eval_create_binary(ID_EQ, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 55:

    { (yyval.node) = so_eval_create_binary(ID_NEQ, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 56:

    { (yyval.node) = so_eval_create_binary((yyvsp[(2) - (3)].id), (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 57:

    { (yyval.node) = so_eval_create_binary(ID_AND, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 58:

    { (yyval.node) = so_eval_create_binary(ID_OR, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 59:

    { (yyval.node) = so_eval_create_unary(ID_NOT, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 60:

    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;


/* Line 1267 of yacc.c.  */

      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


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
      yyerror (YY_("syntax error"));
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
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



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
		      yytoken, &yylval);
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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





/*
 * Creates a register name from the register type and register char.
 *
 * Note: don't "const" the return type, as that will trigger a bug in
 * Microsoft Visual C++ v6.0. 20000606 mortene.
*/
static char *
get_regname(char reg, int regtype)
{
  static char buf[3];
  buf[2] = 0;

  if (regtype != REGTYPE_IN) {
    if (regtype == REGTYPE_TMP) buf[0] = 't';
    else if (regtype == REGTYPE_OUT) buf[0] = 'o';
    buf[1] = reg;
    buf[2] = 0;
  }
  else {
    buf[0] = reg;
    buf[1] = 0;
  }
  return buf;
}



#include "so_eval.ic" /* our lexical scanner */

/* some very simple error handling for now :) */
static char *myerrorptr;
static char myerrorbuf[512];

/*
 * parse the text string into a tree structure.
 */
so_eval_node *
so_eval_parse(const char *buffer)
{
  /* FIXME: better error handling is obviously needed */
  YY_BUFFER_STATE state;
  myerrorptr = NULL;
  root_node = NULL;
  state = so_eval_scan_string(buffer); /* flex routine */
  so_evalparse(); /* start parsing */
  so_eval_delete_buffer(state); /* flex routine */
  if (myerrorptr) return NULL;
  return root_node;
}

/*
 * Returns current error message or NULL if none.
 *
 * Note: don't "const" the return type, as that will trigger a bug in
 * Microsoft Visual C++ v6.0. 20000606 mortene.
 */
char *
so_eval_error(void)
{
  return myerrorptr;
}

/*
 * Called by bison parser upon lexical/syntax error.
 */
int
so_evalerror(const char *myerr)
{
  strncpy(myerrorbuf, myerr, 512);
  myerrorbuf[511] = 0; /* just in case string was too long */
  myerrorptr = myerrorbuf; /* signal error */
  so_eval_delete(root_node); /* free memory used */
  root_node = NULL;
  return 0;
}
