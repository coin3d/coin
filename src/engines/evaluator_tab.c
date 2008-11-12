/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* If NAME_PREFIX is specified substitute the variables and functions
   names.  */
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
     LEX_POW = 267,
     LEX_FMOD = 268,
     LEX_LEN = 269,
     LEX_CROSS = 270,
     LEX_DOT = 271,
     LEX_NORMALIZE = 272,
     LEX_VEC3F = 273,
     LEX_ERROR = 274,
     LEX_OR = 275,
     LEX_AND = 276,
     LEX_NEQ = 277,
     LEX_EQ = 278,
     UNARY = 279
   };
#endif
#define LEX_VALUE 258
#define LEX_TMP_FLT_REG 259
#define LEX_IN_FLT_REG 260
#define LEX_OUT_FLT_REG 261
#define LEX_TMP_VEC_REG 262
#define LEX_IN_VEC_REG 263
#define LEX_OUT_VEC_REG 264
#define LEX_COMPARE 265
#define LEX_FLTFUNC 266
#define LEX_POW 267
#define LEX_FMOD 268
#define LEX_LEN 269
#define LEX_CROSS 270
#define LEX_DOT 271
#define LEX_NORMALIZE 272
#define LEX_VEC3F 273
#define LEX_ERROR 274
#define LEX_OR 275
#define LEX_AND 276
#define LEX_NEQ 277
#define LEX_EQ 278
#define UNARY 279




/* Copy the first part of user declarations.  */


/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
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
#include "evaluator.h"


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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)

typedef union YYSTYPE {
  int id;
  float value;
  char  reg;
  so_eval_node *node;
} YYSTYPE;
/* Line 191 of yacc.c.  */

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


/* Line 214 of yacc.c.  */


#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
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
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   434

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  8
/* YYNRULES -- Number of rules. */
#define YYNRULES  59
/* YYNRULES -- Number of states. */
#define YYNSTATES  149

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   279

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,     2,     2,    37,     2,     2,
      22,    23,    35,    33,    19,    34,     2,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    28,    24,
       2,    26,     2,    27,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    20,     2,    21,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    25,    29,    30,    31,    32,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     7,     9,    10,    14,    18,    20,    22,
      27,    32,    34,    36,    42,    48,    54,    58,    62,    66,
      70,    74,    81,    88,    91,    95,   100,   105,   110,   112,
     114,   116,   121,   126,   131,   133,   139,   145,   151,   155,
     159,   163,   167,   171,   178,   181,   185,   190,   192,   194,
     196,   205,   209,   213,   217,   221,   225,   229,   233,   236
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      41,     0,    -1,    41,    24,    42,    -1,    42,    -1,    -1,
      43,    26,    45,    -1,    44,    26,    46,    -1,     4,    -1,
       6,    -1,     7,    20,     3,    21,    -1,     9,    20,     3,
      21,    -1,     7,    -1,     9,    -1,    47,    27,    45,    28,
      45,    -1,    45,    27,    45,    28,    45,    -1,    46,    27,
      45,    28,    45,    -1,    45,    33,    45,    -1,    45,    34,
      45,    -1,    45,    36,    45,    -1,    45,    35,    45,    -1,
      45,    37,    45,    -1,    12,    22,    45,    19,    45,    23,
      -1,    13,    22,    45,    19,    45,    23,    -1,    34,    45,
      -1,    22,    45,    23,    -1,    11,    22,    45,    23,    -1,
      14,    22,    46,    23,    -1,    16,    22,    46,    23,    -1,
       4,    -1,     6,    -1,     5,    -1,     7,    20,     3,    21,
      -1,     8,    20,     3,    21,    -1,     9,    20,     3,    21,
      -1,     3,    -1,    47,    27,    46,    28,    46,    -1,    45,
      27,    46,    28,    46,    -1,    46,    27,    46,    28,    46,
      -1,    46,    33,    46,    -1,    46,    34,    46,    -1,    46,
      35,    45,    -1,    46,    36,    45,    -1,    45,    35,    46,
      -1,    15,    22,    46,    19,    46,    23,    -1,    34,    46,
      -1,    22,    46,    23,    -1,    17,    22,    46,    23,    -1,
       7,    -1,     9,    -1,     8,    -1,    18,    22,    45,    19,
      45,    19,    45,    23,    -1,    45,    32,    45,    -1,    45,
      31,    45,    -1,    46,    32,    46,    -1,    46,    31,    46,
      -1,    45,    10,    45,    -1,    47,    30,    47,    -1,    47,
      29,    47,    -1,    38,    47,    -1,    22,    47,    23,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,   105,   105,   107,   110,   111,   112,   115,   116,   117,
     119,   123,   124,   127,   129,   131,   134,   135,   136,   137,
     138,   139,   141,   144,   145,   146,   147,   148,   150,   151,
     152,   153,   155,   157,   159,   163,   165,   167,   170,   171,
     172,   173,   174,   175,   178,   179,   180,   183,   184,   185,
     186,   190,   191,   192,   193,   194,   195,   196,   197,   198
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LEX_VALUE", "LEX_TMP_FLT_REG", 
  "LEX_IN_FLT_REG", "LEX_OUT_FLT_REG", "LEX_TMP_VEC_REG", 
  "LEX_IN_VEC_REG", "LEX_OUT_VEC_REG", "LEX_COMPARE", "LEX_FLTFUNC", 
  "LEX_POW", "LEX_FMOD", "LEX_LEN", "LEX_CROSS", "LEX_DOT", 
  "LEX_NORMALIZE", "LEX_VEC3F", "','", "'['", "']'", "'('", "')'", "';'", 
  "LEX_ERROR", "'='", "'?'", "':'", "LEX_OR", "LEX_AND", "LEX_NEQ", 
  "LEX_EQ", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "UNARY", "$accept", 
  "expression", "subexpression", "fltlhs", "veclhs", "fltstatement", 
  "vecstatement", "boolstatement", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,    44,
      91,    93,    40,    41,    59,   274,    61,    63,    58,   275,
     276,   277,   278,    43,    45,    42,    47,    37,    33,   279
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    40,    41,    41,    42,    42,    42,    43,    43,    43,
      43,    44,    44,    45,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    47,    47,    47,    47,    47,    47,    47,    47,    47
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     3,     1,     0,     3,     3,     1,     1,     4,
       4,     1,     1,     5,     5,     5,     3,     3,     3,     3,
       3,     6,     6,     2,     3,     4,     4,     4,     1,     1,
       1,     4,     4,     4,     1,     5,     5,     5,     3,     3,
       3,     3,     3,     6,     2,     3,     4,     1,     1,     1,
       8,     3,     3,     3,     3,     3,     3,     3,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       4,     7,     8,    11,    12,     0,     3,     0,     0,     0,
       0,     1,     4,     0,     0,     0,     0,     2,    34,    28,
      30,    29,    47,    49,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     0,     0,     0,
       6,     9,    10,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,    44,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    45,    59,    55,     0,     0,    52,    51,    16,    17,
      19,    42,    18,    20,     0,     0,    54,    53,    38,    39,
      40,    41,     0,     0,    57,    56,    31,    32,    33,    25,
       0,     0,    26,     0,    27,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    14,    36,    15,
      37,    13,    35,    21,    22,    43,     0,     0,    50
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     5,     6,     7,     8,    39,    37,    38
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -33
static const short yypact[] =
{
      61,   -33,   -33,    17,    23,     6,   -33,    18,    38,    63,
      68,   -33,    61,    83,    83,    51,    62,   -33,   -33,   -33,
     -33,   -33,    65,    73,    84,    60,    81,    87,    88,    89,
      90,    98,   100,    83,    83,    83,   273,   -22,    46,   273,
     -22,   -33,   -33,   103,   129,   132,    83,    83,    83,    83,
      83,    83,    83,    83,    -8,   312,     9,   -33,   -33,   -33,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,   113,
     116,   121,    92,   120,   131,   326,   298,   340,   354,   150,
     -33,   -33,   -33,   287,   161,   378,   287,   287,   -19,   -19,
     -33,   -33,   -33,   -33,   180,   388,   280,   280,   -28,   -28,
      -6,    -6,   191,   398,   114,   -33,   -33,   -33,   -33,   -33,
      83,    83,   -33,    83,   -33,   -33,    83,    83,    83,    83,
      83,    83,    83,   210,   225,   368,   244,   273,   -22,   273,
     -22,   273,   -22,   -33,   -33,   -33,    83,   259,   -33
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
     -33,   -33,   133,   -33,   -33,   -13,     8,   -32
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      36,    56,    60,    59,    60,    69,    11,    74,    75,    70,
      71,    72,    73,    74,    75,    90,    66,    67,    68,    61,
      54,    57,    40,    62,    63,    64,    65,    66,    67,    68,
      12,    68,    92,    82,    83,    84,    76,     9,    77,    78,
      89,    55,    58,    10,    13,   114,   115,    93,    94,    96,
      97,    98,    99,   100,   102,   103,   104,    85,    86,    87,
      88,   110,   111,   112,    14,     1,    15,     2,     3,    95,
       4,    16,    41,    76,   101,    77,    78,   105,   106,   107,
     108,   109,    46,    42,   113,    43,    18,    19,    20,    21,
      22,    23,    24,    44,    25,    26,    27,    28,    29,    30,
      31,    32,    60,    47,    45,    33,    79,   133,   134,    48,
      49,    50,    51,   136,   137,   119,   139,    34,   141,    61,
      52,    35,    53,    62,    63,    64,    65,    66,    67,    68,
      60,   135,    80,   147,   116,    81,   138,   117,   140,   120,
     142,    60,   118,     0,    78,    17,     0,    61,     0,     0,
     121,    62,    63,    64,    65,    66,    67,    68,    61,     0,
      60,     0,    62,    63,    64,    65,    66,    67,    68,   126,
       0,    60,     0,     0,     0,     0,     0,    61,     0,     0,
       0,    62,    63,    64,    65,    66,    67,    68,    61,   127,
      60,     0,    62,    63,    64,    65,    66,    67,    68,     0,
       0,    60,     0,     0,     0,     0,     0,    61,   129,     0,
       0,    62,    63,    64,    65,    66,    67,    68,    61,   131,
      60,     0,    62,    63,    64,    65,    66,    67,    68,     0,
       0,     0,     0,   143,     0,    60,     0,    61,     0,     0,
       0,    62,    63,    64,    65,    66,    67,    68,   144,     0,
       0,     0,    61,     0,    60,     0,    62,    63,    64,    65,
      66,    67,    68,   146,     0,     0,     0,     0,     0,    60,
       0,    61,     0,     0,     0,    62,    63,    64,    65,    66,
      67,    68,   148,    60,     0,     0,    61,     0,     0,     0,
      62,    63,    64,    65,    66,    67,    68,    60,     0,     0,
      61,     0,     0,     0,    62,    63,    64,    65,    66,    67,
      68,    70,    71,    72,    73,    74,    75,   123,    62,    63,
      64,    65,    66,    67,    68,    69,     0,     0,     0,    70,
      71,    72,    73,    74,    75,    91,     0,     0,     0,    69,
       0,     0,     0,    70,    71,    72,    73,    74,    75,   122,
       0,     0,     0,    69,     0,     0,     0,    70,    71,    72,
      73,    74,    75,   124,     0,     0,     0,    69,     0,     0,
       0,    70,    71,    72,    73,    74,    75,   125,     0,     0,
       0,    69,     0,     0,     0,    70,    71,    72,    73,    74,
      75,   145,     0,     0,     0,    69,     0,     0,     0,    70,
      71,    72,    73,    74,    75,    69,   128,     0,     0,    70,
      71,    72,    73,    74,    75,    69,   130,     0,     0,    70,
      71,    72,    73,    74,    75,    69,   132,     0,     0,    70,
      71,    72,    73,    74,    75
};

static const short yycheck[] =
{
      13,    33,    10,    35,    10,    27,     0,    35,    36,    31,
      32,    33,    34,    35,    36,    23,    35,    36,    37,    27,
      33,    34,    14,    31,    32,    33,    34,    35,    36,    37,
      24,    37,    23,    46,    47,    48,    27,    20,    29,    30,
      53,    33,    34,    20,    26,    77,    78,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    49,    50,    51,
      52,    74,    75,    76,    26,     4,     3,     6,     7,    61,
       9,     3,    21,    27,    66,    29,    30,    69,    70,    71,
      72,    73,    22,    21,    76,    20,     3,     4,     5,     6,
       7,     8,     9,    20,    11,    12,    13,    14,    15,    16,
      17,    18,    10,    22,    20,    22,     3,   120,   121,    22,
      22,    22,    22,   126,   127,    23,   129,    34,   131,    27,
      22,    38,    22,    31,    32,    33,    34,    35,    36,    37,
      10,   123,     3,   146,    21,     3,   128,    21,   130,    19,
     132,    10,    21,    -1,    30,    12,    -1,    27,    -1,    -1,
      19,    31,    32,    33,    34,    35,    36,    37,    27,    -1,
      10,    -1,    31,    32,    33,    34,    35,    36,    37,    19,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    31,    32,    33,    34,    35,    36,    37,    27,    28,
      10,    -1,    31,    32,    33,    34,    35,    36,    37,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    31,    32,    33,    34,    35,    36,    37,    27,    28,
      10,    -1,    31,    32,    33,    34,    35,    36,    37,    -1,
      -1,    -1,    -1,    23,    -1,    10,    -1,    27,    -1,    -1,
      -1,    31,    32,    33,    34,    35,    36,    37,    23,    -1,
      -1,    -1,    27,    -1,    10,    -1,    31,    32,    33,    34,
      35,    36,    37,    19,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    27,    -1,    -1,    -1,    31,    32,    33,    34,    35,
      36,    37,    23,    10,    -1,    -1,    27,    -1,    -1,    -1,
      31,    32,    33,    34,    35,    36,    37,    10,    -1,    -1,
      27,    -1,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    31,    32,    33,    34,    35,    36,    19,    31,    32,
      33,    34,    35,    36,    37,    27,    -1,    -1,    -1,    31,
      32,    33,    34,    35,    36,    23,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    31,    32,    33,    34,    35,    36,    23,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    36,    23,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    31,    32,    33,    34,    35,    36,    23,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    31,    32,    33,    34,    35,
      36,    23,    -1,    -1,    -1,    27,    -1,    -1,    -1,    31,
      32,    33,    34,    35,    36,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    36,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    36,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    36
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     4,     6,     7,     9,    41,    42,    43,    44,    20,
      20,     0,    24,    26,    26,     3,     3,    42,     3,     4,
       5,     6,     7,     8,     9,    11,    12,    13,    14,    15,
      16,    17,    18,    22,    34,    38,    45,    46,    47,    45,
      46,    21,    21,    20,    20,    20,    22,    22,    22,    22,
      22,    22,    22,    22,    45,    46,    47,    45,    46,    47,
      10,    27,    31,    32,    33,    34,    35,    36,    37,    27,
      31,    32,    33,    34,    35,    36,    27,    29,    30,     3,
       3,     3,    45,    45,    45,    46,    46,    46,    46,    45,
      23,    23,    23,    45,    45,    46,    45,    45,    45,    45,
      45,    46,    45,    45,    45,    46,    46,    46,    46,    46,
      45,    45,    45,    46,    47,    47,    21,    21,    21,    23,
      19,    19,    23,    19,    23,    23,    19,    28,    28,    28,
      28,    28,    28,    45,    45,    46,    45,    45,    46,    45,
      46,    45,    46,    23,    23,    23,    19,    45,    23
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
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
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
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
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
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

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
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

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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

    { root_node = so_eval_create_binary(ID_SEPARATOR, yyvsp[-2].node, yyvsp[0].node); yyval.node = root_node; ;}
    break;

  case 3:

    { root_node = yyvsp[0].node; yyval.node = yyvsp[0].node; ;}
    break;

  case 4:

    { yyval.node = NULL ;}
    break;

  case 5:

    { yyval.node = so_eval_create_binary(ID_ASSIGN_FLT, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 6:

    { yyval.node = so_eval_create_binary(ID_ASSIGN_VEC, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 7:

    { yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_TMP)); ;}
    break;

  case 8:

    { yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_OUT)); ;}
    break;

  case 9:

    { yyval.node = so_eval_create_reg_comp(get_regname(yyvsp[-3].reg, REGTYPE_TMP), (int) yyvsp[-1].value); ;}
    break;

  case 10:

    { yyval.node = so_eval_create_reg_comp(get_regname(yyvsp[-3].reg, REGTYPE_OUT), (int) yyvsp[-1].value); ;}
    break;

  case 11:

    { yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_TMP));;}
    break;

  case 12:

    { yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_OUT));;}
    break;

  case 13:

    { yyval.node = so_eval_create_ternary(ID_FLT_COND, yyvsp[-4].node, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 14:

    { yyval.node = so_eval_create_ternary(ID_FLT_COND, so_eval_create_unary(ID_TEST_FLT, yyvsp[-4].node), yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 15:

    { yyval.node = so_eval_create_ternary(ID_FLT_COND, so_eval_create_unary(ID_TEST_VEC, yyvsp[-4].node), yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 16:

    { yyval.node = so_eval_create_binary(ID_ADD, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 17:

    { yyval.node = so_eval_create_binary(ID_SUB, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 18:

    { yyval.node = so_eval_create_binary(ID_DIV, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 19:

    { yyval.node = so_eval_create_binary(ID_MUL, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 20:

    { yyval.node = so_eval_create_binary(ID_FMOD, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 21:

    { yyval.node = so_eval_create_binary(ID_POW, yyvsp[-3].node, yyvsp[-1].node); ;}
    break;

  case 22:

    { yyval.node = so_eval_create_binary(ID_FMOD, yyvsp[-3].node, yyvsp[-1].node); ;}
    break;

  case 23:

    { yyval.node = so_eval_create_unary(ID_NEG, yyvsp[0].node); ;}
    break;

  case 24:

    { yyval.node = yyvsp[-1].node; ;}
    break;

  case 25:

    { yyval.node = so_eval_create_unary(yyvsp[-3].id, yyvsp[-1].node);;}
    break;

  case 26:

    { yyval.node = so_eval_create_unary(ID_LEN, yyvsp[-1].node);;}
    break;

  case 27:

    { yyval.node = so_eval_create_unary(ID_DOT, yyvsp[-1].node); ;}
    break;

  case 28:

    { yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_TMP));;}
    break;

  case 29:

    { yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_OUT));;}
    break;

  case 30:

    { yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_IN));;}
    break;

  case 31:

    { yyval.node = so_eval_create_reg_comp(get_regname(yyvsp[-3].reg, REGTYPE_TMP), (int) yyvsp[-1].value);;}
    break;

  case 32:

    { yyval.node = so_eval_create_reg_comp(get_regname(yyvsp[-3].reg, REGTYPE_IN), (int) yyvsp[-1].value);;}
    break;

  case 33:

    { yyval.node = so_eval_create_reg_comp(get_regname(yyvsp[-3].reg, REGTYPE_OUT), (int) yyvsp[-1].value);;}
    break;

  case 34:

    { yyval.node = so_eval_create_flt_val(yyvsp[0].value); ;}
    break;

  case 35:

    { yyval.node = so_eval_create_ternary(ID_VEC_COND, yyvsp[-4].node, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 36:

    { yyval.node = so_eval_create_ternary(ID_VEC_COND, so_eval_create_unary(ID_TEST_FLT, yyvsp[-4].node), yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 37:

    { yyval.node = so_eval_create_ternary(ID_VEC_COND, so_eval_create_unary(ID_TEST_VEC, yyvsp[-4].node), yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 38:

    { yyval.node = so_eval_create_binary(ID_ADD_VEC, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 39:

    { yyval.node = so_eval_create_binary(ID_SUB_VEC, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 40:

    { yyval.node = so_eval_create_binary(ID_MUL_VEC_FLT, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 41:

    { yyval.node = so_eval_create_binary(ID_DIV_VEC_FLT, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 42:

    { yyval.node = so_eval_create_binary(ID_MUL_VEC_FLT, yyvsp[0].node, yyvsp[-2].node); ;}
    break;

  case 43:

    { yyval.node = so_eval_create_binary(ID_CROSS, yyvsp[-3].node, yyvsp[-1].node); ;}
    break;

  case 44:

    { yyval.node = so_eval_create_unary(ID_NEG_VEC, yyvsp[0].node); ;}
    break;

  case 45:

    { yyval.node = yyvsp[-1].node; ;}
    break;

  case 46:

    { yyval.node = so_eval_create_unary(ID_NORMALIZE, yyvsp[-1].node); ;}
    break;

  case 47:

    { yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_TMP));;}
    break;

  case 48:

    { yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_OUT));;}
    break;

  case 49:

    { yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_IN));;}
    break;

  case 50:

    { yyval.node = so_eval_create_ternary(ID_VEC3F, yyvsp[-5].node, yyvsp[-3].node, yyvsp[-1].node); ;}
    break;

  case 51:

    { yyval.node = so_eval_create_binary(ID_EQ, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 52:

    { yyval.node = so_eval_create_binary(ID_NEQ, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 53:

    { yyval.node = so_eval_create_binary(ID_EQ, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 54:

    { yyval.node = so_eval_create_binary(ID_NEQ, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 55:

    { yyval.node = so_eval_create_binary(yyvsp[-1].id, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 56:

    { yyval.node = so_eval_create_binary(ID_AND, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 57:

    { yyval.node = so_eval_create_binary(ID_OR, yyvsp[-2].node, yyvsp[0].node); ;}
    break;

  case 58:

    { yyval.node = so_eval_create_unary(ID_NOT, yyvsp[0].node); ;}
    break;

  case 59:

    { yyval.node = yyvsp[-1].node; ;}
    break;


    }

/* Line 999 of yacc.c.  */


  yyvsp -= yylen;
  yyssp -= yylen;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
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
