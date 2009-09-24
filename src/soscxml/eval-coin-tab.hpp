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
     SCXML_COIN_PAREN_OPEN = 258,
     SCXML_COIN_PAREN_CLOSE = 259,
     SCXML_COIN_EVENT_SCOPE = 260,
     SCXML_COIN_BOOL_TRUE = 261,
     SCXML_COIN_BOOL_FALSE = 262,
     SCXML_COIN_REAL = 263,
     SCXML_COIN_STRING = 264,
     SCXML_COIN_SBVALUE = 265,
     SCXML_COIN_IDENTIFIER = 266,
     SCXML_COIN_IN_FUNC = 267,
     SCXML_COIN_LENGTH_FUNC = 268,
     SCXML_COIN_OP_NOT = 269,
     SCXML_COIN_OP_AND = 270,
     SCXML_COIN_OP_OR = 271,
     SCXML_COIN_OP_EQUALS = 272,
     SCXML_COIN_OP_NOT_EQUALS = 273,
     SCXML_COIN_OP_ADD = 274,
     SCXML_COIN_OP_SUBTRACT = 275,
     SCXML_COIN_OP_MULTIPLY = 276,
     SCXML_COIN_OP_DIVIDE = 277,
     SCXML_COIN_OP_NEGATE = 278,
     SCXML_COIN_OP_APPEND = 279,
     SCXML_COIN_END = 280
   };
#endif
/* Tokens.  */
#define SCXML_COIN_PAREN_OPEN 258
#define SCXML_COIN_PAREN_CLOSE 259
#define SCXML_COIN_EVENT_SCOPE 260
#define SCXML_COIN_BOOL_TRUE 261
#define SCXML_COIN_BOOL_FALSE 262
#define SCXML_COIN_REAL 263
#define SCXML_COIN_STRING 264
#define SCXML_COIN_SBVALUE 265
#define SCXML_COIN_IDENTIFIER 266
#define SCXML_COIN_IN_FUNC 267
#define SCXML_COIN_LENGTH_FUNC 268
#define SCXML_COIN_OP_NOT 269
#define SCXML_COIN_OP_AND 270
#define SCXML_COIN_OP_OR 271
#define SCXML_COIN_OP_EQUALS 272
#define SCXML_COIN_OP_NOT_EQUALS 273
#define SCXML_COIN_OP_ADD 274
#define SCXML_COIN_OP_SUBTRACT 275
#define SCXML_COIN_OP_MULTIPLY 276
#define SCXML_COIN_OP_DIVIDE 277
#define SCXML_COIN_OP_NEGATE 278
#define SCXML_COIN_OP_APPEND 279
#define SCXML_COIN_END 280




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 50 "eval-coin-tab.y"
{
  double real;
  char * stringptr;
  ScXMLDataObj * scxmlobj;
}
/* Line 1489 of yacc.c.  */
#line 105 "eval-coin-tab.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE scxml_coin_lval;

