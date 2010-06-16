%{
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

/* Run 'flex eval-minimum.l' to generate the compilable source files */

#include <cmath>
#include <cfloat>
#include "scxml/eval-minimum.h"
#ifndef SCXML_MINIMUM_OP_ADD
#include "scxml/eval-minimum-tab.hpp"
#else
#endif // !SCXML_MINIMUM_OP_ADD
%}

%option outfile="eval-minimum.cpp"
%option prefix="scxml_minimum_"
%option case-sensitive
%option never-interactive
%option noyywrap
%option nounput
%option noinput

DIGITSET            0123456789
LETTERSET           aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ
DIGIT               [0123456789]
DIGITS              {DIGIT}+
LETTER              [{LETTERSET}]
LETTERS             {LETTER}+

SIGN                ([+-])?
EXP                 [eE]{SIGN}{DIGITS}
FLOAT               {SIGN}{DIGITS}("."{DIGITS})?({EXP})?
WHITE               [ \t]*
WS                  [ \t]{WHITE}
LINE                [^\n]*

ID                  {LETTER}[{LETTERSET}{DIGITSET}_]+

TRUE                ("TRUE"|"True"|"true")
FALSE               ("FALSE"|"False"|"false")

%%

{WS}                { }
"("                 { return SCXML_MINIMUM_PAREN_OPEN; }
")"                 { return SCXML_MINIMUM_PAREN_CLOSE; }

"In"                { return SCXML_MINIMUM_IN_FUNC; }
{ID}                { scxml_minimum_lval.stringptr = yytext;
                      return SCXML_MINIMUM_IDENTIFIER; }
%%

// "'"[^']*"'"         { scxml_minimum_lval.stringptr = yytext; return SCXML_MINIMUM_STRING; }

int
scxml_minimum_error(const char * error)
{
  printf("\nscxml_minimum error: %s\n", error);
  return 0;
}

ScXMLDataObj *
scxml_minimum_parse(const char * buffer)
{
  YY_BUFFER_STATE yybuf = scxml_minimum__scan_string(buffer);
  scxml_minimum__switch_to_buffer(yybuf);
  scxml_minimum_parse(); // the bison function
  scxml_minimum__delete_buffer(yybuf);
  ScXMLDataObj * dataobj = scxml_minimum_get_root_obj();
  scxml_minimum_clear_root_obj();
  return dataobj;
}

