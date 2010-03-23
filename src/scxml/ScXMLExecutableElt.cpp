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

#include <Inventor/scxml/ScXMLExecutableElt.h>

/*!
  \class ScXMLExecutableElt ScXMLExecutableElt.h Inventor/scxml/ScXMLExecutableElt.h
  \brief base class for all SCXML elements with executable content.

  \ingroup scxml
*/

#include <cassert>

#include <Inventor/errors/SoDebugError.h>

#include "SbBasicP.h"

// *************************************************************************

class ScXMLExecutableElt::PImpl {
public:
};

#define PRIVATE

SCXML_ELEMENT_ABSTRACT_SOURCE(ScXMLExecutableElt);

void
ScXMLExecutableElt::initClass(void)
{
  SCXML_OBJECT_INIT_ABSTRACT_CLASS(ScXMLExecutableElt, ScXMLElt, "ScXMLElt");
}

void
ScXMLExecutableElt::cleanClass(void)
{
  ScXMLExecutableElt::classTypeId = SoType::badType();
}

ScXMLExecutableElt::ScXMLExecutableElt(void)
{
}

ScXMLExecutableElt::~ScXMLExecutableElt(void)
{
}

void
ScXMLExecutableElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  /*const ScXMLExecutableElt * orig = */coin_assert_cast<const ScXMLExecutableElt *>(rhs);
}

void
ScXMLExecutableElt::execute(ScXMLStateMachine * COIN_UNUSED_ARG(statemachine)) const
{
  // nada
  // make abstract?
}

#undef PRIVATE
