
/*  A Bison parser, made from evaluator.y
 by  GNU Bison version 1.27
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse so_evalparse
#define yylex so_evallex
#define yyerror so_evalerror
#define yylval so_evallval
#define yychar so_evalchar
#define yydebug so_evaldebug
#define yynerrs so_evalnerrs
#define	LEX_VALUE	257
#define	LEX_TMP_FLT_REG	258
#define	LEX_IN_FLT_REG	259
#define	LEX_OUT_FLT_REG	260
#define	LEX_TMP_VEC_REG	261
#define	LEX_IN_VEC_REG	262
#define	LEX_OUT_VEC_REG	263
#define	LEX_COMPARE	264
#define	LEX_FLTFUNC	265
#define	LEX_POW	266
#define	LEX_FMOD	267
#define	LEX_LEN	268
#define	LEX_CROSS	269
#define	LEX_DOT	270
#define	LEX_NORMALIZE	271
#define	LEX_VEC3F	272
#define	LEX_ERROR	273
#define	LEX_OR	274
#define	LEX_AND	275
#define	LEX_EQ	276
#define	LEX_NEQ	277
#define	UNARY	278


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

#include <Inventor/C/basic.h>
#include "evaluator.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef union
{
  int id;
  float value;
  char  reg;
  so_eval_node *node;
} YYSTYPE;

  static char * get_regname(char reg, int regtype);
  enum { REGTYPE_IN, REGTYPE_OUT, REGTYPE_TMP };
  so_eval_node *root_node;
  int so_evalerror(char *);
  int so_evallex(void);
#include <stdio.h>


#define	YYFINAL		149
#define	YYFLAG		-32768
#define	YYNTBASE	40

#define YYTRANSLATE(x) ((unsigned)(x) <= 278 ? yytranslate[x] : 47)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    38,     2,     2,     2,    37,     2,     2,    22,
    23,    35,    33,    19,    34,     2,    36,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    28,    24,     2,
    26,     2,    27,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    20,     2,    21,     2,     2,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    25,    29,    30,    31,    32,    39
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     4,     6,    10,    14,    16,    18,    23,    28,    30,
    32,    38,    44,    50,    54,    58,    62,    66,    70,    77,
    84,    87,    91,    96,   101,   106,   108,   110,   112,   117,
   122,   127,   129,   135,   141,   147,   151,   155,   159,   163,
   167,   174,   177,   181,   186,   188,   190,   192,   201,   205,
   209,   213,   217,   221,   225,   229,   232
};

static const short yyrhs[] = {    40,
    24,    41,     0,    41,     0,    42,    26,    44,     0,    43,
    26,    45,     0,     4,     0,     6,     0,     7,    20,     3,
    21,     0,     9,    20,     3,    21,     0,     7,     0,     9,
     0,    46,    27,    44,    28,    44,     0,    44,    27,    44,
    28,    44,     0,    45,    27,    44,    28,    44,     0,    44,
    33,    44,     0,    44,    34,    44,     0,    44,    36,    44,
     0,    44,    35,    44,     0,    44,    37,    44,     0,    12,
    22,    44,    19,    44,    23,     0,    13,    22,    44,    19,
    44,    23,     0,    34,    44,     0,    22,    44,    23,     0,
    11,    22,    44,    23,     0,    14,    22,    45,    23,     0,
    16,    22,    45,    23,     0,     4,     0,     6,     0,     5,
     0,     7,    20,     3,    21,     0,     8,    20,     3,    21,
     0,     9,    20,     3,    21,     0,     3,     0,    46,    27,
    45,    28,    45,     0,    44,    27,    45,    28,    45,     0,
    45,    27,    45,    28,    45,     0,    45,    33,    45,     0,
    45,    34,    45,     0,    45,    35,    44,     0,    45,    36,
    44,     0,    44,    35,    45,     0,    15,    22,    45,    19,
    45,    23,     0,    34,    45,     0,    22,    45,    23,     0,
    17,    22,    45,    23,     0,     7,     0,     9,     0,     8,
     0,    18,    22,    44,    19,    44,    19,    44,    23,     0,
    44,    31,    44,     0,    44,    32,    44,     0,    45,    31,
    45,     0,    45,    32,    45,     0,    44,    10,    44,     0,
    46,    30,    46,     0,    46,    29,    46,     0,    38,    46,
     0,    22,    46,    23,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    82,    84,    87,    88,    91,    92,    93,    95,    99,   100,
   103,   105,   107,   110,   111,   112,   113,   114,   115,   117,
   120,   121,   122,   123,   124,   126,   127,   128,   129,   131,
   133,   135,   139,   141,   143,   146,   147,   148,   149,   150,
   151,   154,   155,   156,   159,   160,   161,   162,   166,   167,
   168,   169,   170,   171,   172,   173,   174
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","LEX_VALUE",
"LEX_TMP_FLT_REG","LEX_IN_FLT_REG","LEX_OUT_FLT_REG","LEX_TMP_VEC_REG","LEX_IN_VEC_REG",
"LEX_OUT_VEC_REG","LEX_COMPARE","LEX_FLTFUNC","LEX_POW","LEX_FMOD","LEX_LEN",
"LEX_CROSS","LEX_DOT","LEX_NORMALIZE","LEX_VEC3F","','","'['","']'","'('","')'",
"';'","LEX_ERROR","'='","'?'","':'","LEX_OR","LEX_AND","LEX_EQ","LEX_NEQ","'+'",
"'-'","'*'","'/'","'%'","'!'","UNARY","expression","subexpression","fltlhs",
"veclhs","fltstatement","vecstatement","boolstatement", NULL
};
#endif

static const short yyr1[] = {     0,
    40,    40,    41,    41,    42,    42,    42,    42,    43,    43,
    44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
    44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
    44,    44,    45,    45,    45,    45,    45,    45,    45,    45,
    45,    45,    45,    45,    45,    45,    45,    45,    46,    46,
    46,    46,    46,    46,    46,    46,    46
};

static const short yyr2[] = {     0,
     3,     1,     3,     3,     1,     1,     4,     4,     1,     1,
     5,     5,     5,     3,     3,     3,     3,     3,     6,     6,
     2,     3,     4,     4,     4,     1,     1,     1,     4,     4,
     4,     1,     5,     5,     5,     3,     3,     3,     3,     3,
     6,     2,     3,     4,     1,     1,     1,     8,     3,     3,
     3,     3,     3,     3,     3,     2,     3
};

static const short yydefact[] = {     0,
     5,     6,     9,    10,     0,     2,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     1,    32,    26,    28,    27,
    45,    47,    46,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     3,     0,     0,     0,     4,     7,
     8,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    21,    42,    56,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    22,    43,
    57,    53,     0,     0,    49,    50,    14,    15,    17,    40,
    16,    18,     0,     0,    51,    52,    36,    37,    38,    39,
     0,     0,    55,    54,    29,    30,    31,    23,     0,     0,
    24,     0,    25,    44,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    12,    34,    13,    35,    11,
    33,    19,    20,    41,     0,     0,    48,     0,     0
};

static const short yydefgoto[] = {     5,
     6,     7,     8,    38,    36,    37
};

static const short yypact[] = {   128,
-32768,-32768,    12,    16,     6,-32768,    11,    13,    35,    40,
   128,    83,    83,    23,    44,-32768,-32768,-32768,-32768,-32768,
    47,    52,    62,    61,    63,    71,    81,    82,    84,    87,
    88,    83,    83,    83,   273,   -22,    46,   273,   -22,-32768,
-32768,   108,   109,   117,    83,    83,    83,    83,    83,    83,
    83,    83,    -8,   312,    41,-32768,-32768,-32768,    83,    83,
    83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
    83,    83,    83,    83,    83,    83,    83,    45,   101,   121,
    92,   120,   131,   326,   298,   340,   354,   150,-32768,-32768,
-32768,   287,   161,   378,   287,   287,   -19,   -19,-32768,-32768,
-32768,-32768,   180,   388,   280,   280,   -28,   -28,    -6,    -6,
   191,   398,   113,-32768,-32768,-32768,-32768,-32768,    83,    83,
-32768,    83,-32768,-32768,    83,    83,    83,    83,    83,    83,
    83,   210,   225,   368,   244,   273,   -22,   273,   -22,   273,
   -22,-32768,-32768,-32768,    83,   259,-32768,   144,-32768
};

static const short yypgoto[] = {-32768,
   134,-32768,-32768,   -12,     9,   -31
};


#define	YYLAST		434


static const short yytable[] = {    35,
    55,    59,    58,    59,    68,   148,    73,    74,    69,    70,
    71,    72,    73,    74,    89,    65,    66,    67,    60,    53,
    56,    39,    61,    62,    63,    64,    65,    66,    67,    11,
    67,     9,    81,    82,    83,    10,    12,    14,    13,    88,
    54,    57,    15,    40,   113,   114,    92,    93,    95,    96,
    97,    98,    99,   101,   102,   103,    84,    85,    86,    87,
   109,   110,   111,    91,    41,   115,    42,    75,    94,    76,
    77,    43,    75,   100,    76,    77,   104,   105,   106,   107,
   108,    44,    45,   112,    46,    17,    18,    19,    20,    21,
    22,    23,    47,    24,    25,    26,    27,    28,    29,    30,
    31,    59,    48,    49,    32,    50,   132,   133,    51,    52,
    78,    79,   135,   136,   118,   138,    33,   140,    60,    80,
    34,   116,    61,    62,    63,    64,    65,    66,    67,    59,
   134,     1,   146,     2,     3,   137,     4,   139,   119,   141,
    59,   117,    77,   149,    16,     0,    60,     0,     0,   120,
    61,    62,    63,    64,    65,    66,    67,    60,     0,    59,
     0,    61,    62,    63,    64,    65,    66,    67,   125,     0,
    59,     0,     0,     0,     0,     0,    60,     0,     0,     0,
    61,    62,    63,    64,    65,    66,    67,    60,   126,    59,
     0,    61,    62,    63,    64,    65,    66,    67,     0,     0,
    59,     0,     0,     0,     0,     0,    60,   128,     0,     0,
    61,    62,    63,    64,    65,    66,    67,    60,   130,    59,
     0,    61,    62,    63,    64,    65,    66,    67,     0,     0,
     0,     0,   142,     0,    59,     0,    60,     0,     0,     0,
    61,    62,    63,    64,    65,    66,    67,   143,     0,     0,
     0,    60,     0,    59,     0,    61,    62,    63,    64,    65,
    66,    67,   145,     0,     0,     0,     0,     0,    59,     0,
    60,     0,     0,     0,    61,    62,    63,    64,    65,    66,
    67,   147,    59,     0,     0,    60,     0,     0,     0,    61,
    62,    63,    64,    65,    66,    67,    59,     0,     0,    60,
     0,     0,     0,    61,    62,    63,    64,    65,    66,    67,
    69,    70,    71,    72,    73,    74,   122,    61,    62,    63,
    64,    65,    66,    67,    68,     0,     0,     0,    69,    70,
    71,    72,    73,    74,    90,     0,     0,     0,    68,     0,
     0,     0,    69,    70,    71,    72,    73,    74,   121,     0,
     0,     0,    68,     0,     0,     0,    69,    70,    71,    72,
    73,    74,   123,     0,     0,     0,    68,     0,     0,     0,
    69,    70,    71,    72,    73,    74,   124,     0,     0,     0,
    68,     0,     0,     0,    69,    70,    71,    72,    73,    74,
   144,     0,     0,     0,    68,     0,     0,     0,    69,    70,
    71,    72,    73,    74,    68,   127,     0,     0,    69,    70,
    71,    72,    73,    74,    68,   129,     0,     0,    69,    70,
    71,    72,    73,    74,    68,   131,     0,     0,    69,    70,
    71,    72,    73,    74
};

static const short yycheck[] = {    12,
    32,    10,    34,    10,    27,     0,    35,    36,    31,    32,
    33,    34,    35,    36,    23,    35,    36,    37,    27,    32,
    33,    13,    31,    32,    33,    34,    35,    36,    37,    24,
    37,    20,    45,    46,    47,    20,    26,     3,    26,    52,
    32,    33,     3,    21,    76,    77,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    48,    49,    50,    51,
    73,    74,    75,    23,    21,    21,    20,    27,    60,    29,
    30,    20,    27,    65,    29,    30,    68,    69,    70,    71,
    72,    20,    22,    75,    22,     3,     4,     5,     6,     7,
     8,     9,    22,    11,    12,    13,    14,    15,    16,    17,
    18,    10,    22,    22,    22,    22,   119,   120,    22,    22,
     3,     3,   125,   126,    23,   128,    34,   130,    27,     3,
    38,    21,    31,    32,    33,    34,    35,    36,    37,    10,
   122,     4,   145,     6,     7,   127,     9,   129,    19,   131,
    10,    21,    30,     0,    11,    -1,    27,    -1,    -1,    19,
    31,    32,    33,    34,    35,    36,    37,    27,    -1,    10,
    -1,    31,    32,    33,    34,    35,    36,    37,    19,    -1,
    10,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
    31,    32,    33,    34,    35,    36,    37,    27,    28,    10,
    -1,    31,    32,    33,    34,    35,    36,    37,    -1,    -1,
    10,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
    31,    32,    33,    34,    35,    36,    37,    27,    28,    10,
    -1,    31,    32,    33,    34,    35,    36,    37,    -1,    -1,
    -1,    -1,    23,    -1,    10,    -1,    27,    -1,    -1,    -1,
    31,    32,    33,    34,    35,    36,    37,    23,    -1,    -1,
    -1,    27,    -1,    10,    -1,    31,    32,    33,    34,    35,
    36,    37,    19,    -1,    -1,    -1,    -1,    -1,    10,    -1,
    27,    -1,    -1,    -1,    31,    32,    33,    34,    35,    36,
    37,    23,    10,    -1,    -1,    27,    -1,    -1,    -1,    31,
    32,    33,    34,    35,    36,    37,    10,    -1,    -1,    27,
    -1,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
    31,    32,    33,    34,    35,    36,    19,    31,    32,    33,
    34,    35,    36,    37,    27,    -1,    -1,    -1,    31,    32,
    33,    34,    35,    36,    23,    -1,    -1,    -1,    27,    -1,
    -1,    -1,    31,    32,    33,    34,    35,    36,    23,    -1,
    -1,    -1,    27,    -1,    -1,    -1,    31,    32,    33,    34,
    35,    36,    23,    -1,    -1,    -1,    27,    -1,    -1,    -1,
    31,    32,    33,    34,    35,    36,    23,    -1,    -1,    -1,
    27,    -1,    -1,    -1,    31,    32,    33,    34,    35,    36,
    23,    -1,    -1,    -1,    27,    -1,    -1,    -1,    31,    32,
    33,    34,    35,    36,    27,    28,    -1,    -1,    31,    32,
    33,    34,    35,    36,    27,    28,    -1,    -1,    31,    32,
    33,    34,    35,    36,    27,    28,    -1,    -1,    31,    32,
    33,    34,    35,    36
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

/* This file comes from bison-1.27.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
{ root_node = so_eval_create_binary(ID_SEPARATOR, yyvsp[-2].node, yyvsp[0].node); yyval.node = root_node; ;
    break;}
case 2:
{ root_node = yyvsp[0].node; yyval.node = yyvsp[0].node; ;
    break;}
case 3:
{ yyval.node = so_eval_create_binary(ID_ASSIGN_FLT, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 4:
{ yyval.node = so_eval_create_binary(ID_ASSIGN_VEC, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 5:
{ yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_TMP)); ;
    break;}
case 6:
{ yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_OUT)); ;
    break;}
case 7:
{ yyval.node = so_eval_create_reg_comp(get_regname(yyvsp[-3].reg, REGTYPE_TMP), (int) yyvsp[-1].value); ;
    break;}
case 8:
{ yyval.node = so_eval_create_reg_comp(get_regname(yyvsp[-3].reg, REGTYPE_OUT), (int) yyvsp[-1].value); ;
    break;}
case 9:
{ yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_TMP));;
    break;}
case 10:
{ yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_OUT));;
    break;}
case 11:
{ yyval.node = so_eval_create_ternary(ID_FLT_COND, yyvsp[-4].node, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 12:
{ yyval.node = so_eval_create_ternary(ID_FLT_COND, so_eval_create_unary(ID_TEST_FLT, yyvsp[-4].node), yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 13:
{ yyval.node = so_eval_create_ternary(ID_FLT_COND, so_eval_create_unary(ID_TEST_VEC, yyvsp[-4].node), yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 14:
{ yyval.node = so_eval_create_binary(ID_ADD, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 15:
{ yyval.node = so_eval_create_binary(ID_SUB, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 16:
{ yyval.node = so_eval_create_binary(ID_DIV, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 17:
{ yyval.node = so_eval_create_binary(ID_MUL, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 18:
{ yyval.node = so_eval_create_binary(ID_FMOD, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 19:
{ yyval.node = so_eval_create_binary(ID_POW, yyvsp[-3].node, yyvsp[-1].node); ;
    break;}
case 20:
{ yyval.node = so_eval_create_binary(ID_FMOD, yyvsp[-3].node, yyvsp[-1].node); ;
    break;}
case 21:
{ yyval.node = so_eval_create_unary(ID_NEG, yyvsp[0].node); ;
    break;}
case 22:
{ yyval.node = yyvsp[-1].node; ;
    break;}
case 23:
{ yyval.node = so_eval_create_unary(yyvsp[-3].id, yyvsp[-1].node);;
    break;}
case 24:
{ yyval.node = so_eval_create_unary(ID_LEN, yyvsp[-1].node);;
    break;}
case 25:
{ yyval.node = so_eval_create_unary(ID_DOT, yyvsp[-1].node); ;
    break;}
case 26:
{ yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_TMP));;
    break;}
case 27:
{ yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_OUT));;
    break;}
case 28:
{ yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_IN));;
    break;}
case 29:
{ yyval.node = so_eval_create_reg_comp(get_regname(yyvsp[-3].reg, REGTYPE_TMP), (int) yyvsp[-1].value);;
    break;}
case 30:
{ yyval.node = so_eval_create_reg_comp(get_regname(yyvsp[-3].reg, REGTYPE_IN), (int) yyvsp[-1].value);;
    break;}
case 31:
{ yyval.node = so_eval_create_reg_comp(get_regname(yyvsp[-3].reg, REGTYPE_OUT), (int) yyvsp[-1].value);;
    break;}
case 32:
{ yyval.node = so_eval_create_flt_val(yyvsp[0].value); ;
    break;}
case 33:
{ yyval.node = so_eval_create_ternary(ID_VEC_COND, yyvsp[-4].node, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 34:
{ yyval.node = so_eval_create_ternary(ID_VEC_COND, so_eval_create_unary(ID_TEST_FLT, yyvsp[-4].node), yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 35:
{ yyval.node = so_eval_create_ternary(ID_VEC_COND, so_eval_create_unary(ID_TEST_VEC, yyvsp[-4].node), yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 36:
{ yyval.node = so_eval_create_binary(ID_ADD_VEC, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 37:
{ yyval.node = so_eval_create_binary(ID_SUB_VEC, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 38:
{ yyval.node = so_eval_create_binary(ID_MUL_VEC_FLT, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 39:
{ yyval.node = so_eval_create_binary(ID_DIV_VEC_FLT, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 40:
{ yyval.node = so_eval_create_binary(ID_MUL_VEC_FLT, yyvsp[0].node, yyvsp[-2].node); ;
    break;}
case 41:
{ yyval.node = so_eval_create_binary(ID_CROSS, yyvsp[-3].node, yyvsp[-1].node); ;
    break;}
case 42:
{ yyval.node = so_eval_create_unary(ID_NEG_VEC, yyvsp[0].node); ;
    break;}
case 43:
{ yyval.node = yyvsp[-1].node; ;
    break;}
case 44:
{ yyval.node = so_eval_create_unary(ID_NORMALIZE, yyvsp[-1].node); ;
    break;}
case 45:
{ yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_TMP));;
    break;}
case 46:
{ yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_OUT));;
    break;}
case 47:
{ yyval.node = so_eval_create_reg(get_regname(yyvsp[0].reg, REGTYPE_IN));;
    break;}
case 48:
{ yyval.node = so_eval_create_ternary(ID_VEC3F, yyvsp[-5].node, yyvsp[-3].node, yyvsp[-1].node); ;
    break;}
case 49:
{ yyval.node = so_eval_create_binary(ID_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 50:
{ yyval.node = so_eval_create_binary(ID_NEQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 51:
{ yyval.node = so_eval_create_binary(ID_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 52:
{ yyval.node = so_eval_create_binary(ID_NEQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 53:
{ yyval.node = so_eval_create_binary(yyvsp[-1].id, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 54:
{ yyval.node = so_eval_create_binary(ID_AND, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 55:
{ yyval.node = so_eval_create_binary(ID_OR, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 56:
{ yyval.node = so_eval_create_unary(ID_NOT, yyvsp[0].node); ;
    break;}
case 57:
{ yyval.node = yyvsp[-1].node; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
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



#include "lex.so_eval.c" /* our lexical scanner */

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
so_evalerror(char *myerr)
{
  strncpy(myerrorbuf, myerr, 512);
  myerrorbuf[511] = 0; /* just in case string was too long */
  myerrorptr = myerrorbuf; /* signal error */
  so_eval_delete(root_node); /* free memory used */
  root_node = NULL;
  return 0;
}
