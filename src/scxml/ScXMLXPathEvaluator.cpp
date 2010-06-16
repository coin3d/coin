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

#include <Inventor/scxml/ScXMLXPathEvaluator.h>
#include "coindefs.h"

/*!
  \class ScXMLXPathEvaluator ScXMLXPathEvaluator.h Inventor/scxml/ScXMLXPathEvaluator.h
  \brief implements the XPath evaluator.

  \ingroup scxml
*/

#include <cassert>

class ScXMLXPathEvaluator::PImpl {
public:
};

SCXML_OBJECT_SOURCE(ScXMLXPathEvaluator);

void
ScXMLXPathEvaluator::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLXPathEvaluator, ScXMLEvaluator, "ScXMLEvaluator");
}

void
ScXMLXPathEvaluator::cleanClass(void)
{
  ScXMLXPathEvaluator::classTypeId = SoType::badType();
}

ScXMLXPathEvaluator::ScXMLXPathEvaluator(void)
{
}

ScXMLXPathEvaluator::~ScXMLXPathEvaluator(void)
{
}

ScXMLDataObj *
ScXMLXPathEvaluator::evaluate(const char * COIN_UNUSED_ARG(expression)) const
{
  // FIXME: not implemented
  return NULL;
}

SbBool
ScXMLXPathEvaluator::setAtLocation(const char * COIN_UNUSED_ARG(location), ScXMLDataObj * COIN_UNUSED_ARG(obj))
{
  // FIXME: not implemented
  return FALSE;
}

ScXMLDataObj *
ScXMLXPathEvaluator::locate(const char * COIN_UNUSED_ARG(location)) const
{
  // FIXME: not implemented
  return NULL;
}

