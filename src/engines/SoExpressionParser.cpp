/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/


#include <Inventor/engines/SoExpressionParser.h>

/* ChangeLog / General Messages
 *
 * 990520 larsa    started implementation
 */

/*!
  \class SoExpressionParser Inventor/engines/SoExpressionParser.h
  \brief INTERNAL class used by SoCalculator engine.
*/

static const char * const keywords[] = {
// constants
    "MAXFLOAT",
    "MINFLOAT",
    "M_E",
    "M_LOG2E",
    "M_LOG10E",
    "M_LN2",
    "M_LN10",
    "M_PI",
    "M_SQRT2",
    "M_SQRT1_2",
// math.h functions
    "cos",
    "sin",
    "tan",
    "acos",
    "asin",
    "atan",
    "atan2",
    "cosh",
    "sinh",
    "tanh",
    "sqrt",
    "pow",
    "exp",
    "log",
    "log10",
    "ceil",
    "floor",
    "fabs",
    "fmod",
// other functions
    "rand",
    "cross",
    "dot",
    "length",
    "normalize",
    "vec3f",
}; // keywords[]

/**************************************************************************/

SoExpressionParser::SoExpressionParser(
    SoCalculator * valueContainer )
  : values( valueContainer )
{
}

SoExpressionParser::~SoExpressionParser(
    void )
{
}

/**************************************************************************/

void
SoExpressionParser::parse(
    void )
{
}

/**************************************************************************/

void
SoExpressionParser::evaluate(
    void )
{
}

/**************************************************************************/

