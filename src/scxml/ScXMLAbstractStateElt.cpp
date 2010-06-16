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

#include <Inventor/scxml/ScXMLAbstractStateElt.h>

/*!
  \class ScXMLAbstractStateElt ScXMLAbstractStateElt.h Inventor/scxml/ScXMLAbstractStateElt
  \brief abstract base class for the SCXML 'state' elements.

  \ingroup scxml
  \since Coin 3.1
*/

#include <cassert>

#include <Inventor/errors/SoDebugError.h>
#include "scxml/ScXMLCommonP.h"
#include "SbBasicP.h"

// *************************************************************************

class ScXMLAbstractStateElt::PImpl {
public:
};

#define PRIVATE

SCXML_ELEMENT_ABSTRACT_SOURCE(ScXMLAbstractStateElt);


void
ScXMLAbstractStateElt::initClass(void)
{
  SCXML_OBJECT_INIT_ABSTRACT_CLASS(ScXMLAbstractStateElt, ScXMLElt, "ScXMLElt");
}

void
ScXMLAbstractStateElt::cleanClass(void)
{
  ScXMLAbstractStateElt::classTypeId = SoType::badType();
}

ScXMLAbstractStateElt::ScXMLAbstractStateElt(void)
: id(NULL)
{
}

ScXMLAbstractStateElt::~ScXMLAbstractStateElt(void)
{
  this->setIdAttribute(NULL);
}

void
ScXMLAbstractStateElt::setIdAttribute(const char * idstr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->id, "id", idstr);
  // FIXME: validate id for lexcal conformance?
}

SbBool
ScXMLAbstractStateElt::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) {
    return FALSE;
  }

  this->setIdAttribute(this->getXMLAttribute("id"));

  // validate
  if ((this->id == NULL) || (std::strcmp(this->id, "") == 0)) {
    SoDebugError::postInfo("ScXMLAbstractStateElt::handleXMLAttributes",
                           "state element must have 'id' attribute");
    return FALSE;
  }

  return TRUE;
}

void
ScXMLAbstractStateElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  const ScXMLAbstractStateElt * orig = coin_assert_cast<const ScXMLAbstractStateElt *>(rhs);
  this->setIdAttribute(orig->getIdAttribute());
}

const ScXMLElt *
ScXMLAbstractStateElt::search(const char * attrname, const char * attrvalue) const
{
  const ScXMLElt * hit = inherited::search(attrname, attrvalue);
  if (hit) {
    return hit;
  }
  if (strcmp(attrname, "id") == 0) {
    if (this->id && strcmp(attrvalue, this->id) == 0) {
      return this;
    }
  }
  return NULL;
}

#undef PRIVATE
