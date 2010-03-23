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

/*
 * Generate the compilable source files this way:
 *
 * bison eval-minimum-tab.y
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <cstdio>

#include "scxml/eval-minimum.h"
#include <Inventor/scxml/ScXMLMinimumEvaluator.h>

namespace {
static ScXMLDataObj * root = NULL;
}
%}

%name-prefix="scxml_minimum_"
%output="eval-minimum-tab.cpp"

%union {
  char * stringptr;
  ScXMLDataObj * scxmlobj;
}

%token SCXML_MINIMUM_PAREN_OPEN SCXML_MINIMUM_PAREN_CLOSE
// %token <stringptr> SCXML_MINIMUM_STRING
%token <stringptr> SCXML_MINIMUM_IDENTIFIER
%token SCXML_MINIMUM_IN_FUNC

%defines

%type <scxmlobj> rootexpr
%type <scxmlobj> expr
%type <scxmlobj> evalexpr
%type <scxmlobj> infuncexpr

%start rootexpr

%%

rootexpr            : expr { $$ = $1; root = $$; }
                    ;

expr                : evalexpr { $$ = $1; }
                    ;

evalexpr            : infuncexpr { $$ = $1; }
                    ;

infuncexpr          : SCXML_MINIMUM_IN_FUNC SCXML_MINIMUM_PAREN_OPEN SCXML_MINIMUM_IDENTIFIER SCXML_MINIMUM_PAREN_CLOSE
                      { $$ = ScXMLInExprDataObj::createFor($3); }
                    ;

%%

ScXMLDataObj *
scxml_minimum_get_root_obj(void)
{
  return root;
}

void
scxml_minimum_clear_root_obj(void)
{
  root = NULL;
}
