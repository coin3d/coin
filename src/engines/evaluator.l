%{
/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*
 * Lexical scanner for SoCalculator. Compile into lexical scanner
 * lex.so_eval.c with
 *
 *             flex -L evaluator.l
 *
 * (Note: flex version should be 2.5.4, since this is what is
 * installed on nfs.sim.no).
 */

#include <Inventor/C/basic.h>
#include <float.h>
#include <stdlib.h>
#include "evaluator.h"
%}

%option noyywrap
%option prefix="so_eval"

Digit                           [0-9]
Space                           [ \t]
Exp                             [eE][+-]?{Digit}+

%%
{Space}+                        ;
"<="                            { yylval.id = ID_LEQ; return LEX_COMPARE; }
">="                            { yylval.id = ID_GEQ; return LEX_COMPARE; }
"<"                             { yylval.id = ID_LT; return LEX_COMPARE; }
">"                             { yylval.id = ID_GT; return LEX_COMPARE; }
"=="                            return LEX_EQ;
"!="                            return LEX_NEQ;
"&&"                            return LEX_AND;
"||"                            return LEX_OR;
"-"                             return '-';
[!+*/=\[\]%?:(),;]              return(yytext[0]);
MAXFLOAT                        { yylval.value = FLT_MAX; return LEX_VALUE; }
MINFLOAT                        { yylval.value = FLT_MIN; return LEX_VALUE; }
M_E                             { yylval.value = (float)M_E; return LEX_VALUE; }
M_LOG2E                         { yylval.value = (float)M_LOG2E; return LEX_VALUE; }
M_LOG10E                        { yylval.value = (float)M_LOG10E; return LEX_VALUE; }
M_LN2                           { yylval.value = (float)M_LN2; return LEX_VALUE; }
M_PI                            { yylval.value = (float)M_PI; return LEX_VALUE; }
M_SQRT2                         { yylval.value = (float)M_SQRT2; return LEX_VALUE; }
M_SQRT1_2                       { yylval.value = (float)M_SQRT1_2; return LEX_VALUE; }
cos                             { yylval.id = ID_COS; return LEX_FLTFUNC; }
sin                             { yylval.id = ID_SIN; return LEX_FLTFUNC; }
tan                             { yylval.id = ID_TAN; return LEX_FLTFUNC; }
acos                            { yylval.id = ID_ACOS; return LEX_FLTFUNC; }
asin                            { yylval.id = ID_ASIN; return LEX_FLTFUNC; }
atan                            { yylval.id = ID_ATAN; return LEX_FLTFUNC; }
atan2                           { yylval.id = ID_ATAN2; return LEX_FLTFUNC; }
cosh                            { yylval.id = ID_COSH; return LEX_FLTFUNC; }
sinh                            { yylval.id = ID_SINH; return LEX_FLTFUNC; }
tanh                            { yylval.id = ID_TANH; return LEX_FLTFUNC; }
sqrt                            { yylval.id = ID_SQRT; return LEX_FLTFUNC; }
pow                             return LEX_POW;
exp                             { yylval.id = ID_EXP; return LEX_FLTFUNC; }
log                             { yylval.id = ID_LOG; return LEX_FLTFUNC; }
log10                           { yylval.id = ID_LOG10; return LEX_FLTFUNC; }
ceil                            { yylval.id = ID_CEIL; return LEX_FLTFUNC; }
floor                           { yylval.id = ID_FLOOR; return LEX_FLTFUNC; }
fabs                            { yylval.id = ID_FABS; return LEX_FLTFUNC; }
fmod                            return LEX_FMOD;
rand                            { yylval.id = ID_RAND; return LEX_FLTFUNC; }
cross                           { return LEX_CROSS; }
dot                             { return LEX_DOT; }
length                          { return LEX_LEN; }
normalize                       { return LEX_NORMALIZE; }
vec3f                           { return LEX_VEC3F; }
{Digit}*"."{Digit}*{Exp}?  { yylval.value = (float)atof(yytext); return LEX_VALUE; }
{Digit}+{Exp}?             { yylval.value = (float)atof(yytext); return LEX_VALUE; }
t[a-h]                          { yylval.reg = yytext[1]; return LEX_TMP_FLT_REG; }
o[a-d]                          { yylval.reg = yytext[1]; return LEX_OUT_FLT_REG; }
[a-h]                           { yylval.reg = yytext[0]; return LEX_IN_FLT_REG; }
t[A-H]                          { yylval.reg = yytext[1]; return LEX_TMP_VEC_REG; }
o[A-D]                          { yylval.reg = yytext[1]; return LEX_OUT_VEC_REG; }
[A-H]                           { yylval.reg = yytext[0]; return LEX_IN_VEC_REG; }
.                               { return LEX_ERROR; }
%%
