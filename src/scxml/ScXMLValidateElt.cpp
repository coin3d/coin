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

#include <Inventor/scxml/ScXMLValidateElt.h>

/*!
  \class ScXMLValidateElt ScXMLValidateElt.h Inventor/scxml/ScXMLValidateElt.h
  \brief implements the &lt;validate&gt; SCXML element.

  \since Coin 3.1
  \ingroup scxml
*/

#include <cassert>
#include <cstring>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/element.h>

#include "scxml/ScXMLCommonP.h"
#include "SbBasicP.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::strcmp;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

// *************************************************************************

class ScXMLValidateEltReader : public ScXMLEltReader {
public:
  ScXMLValidateEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLValidateEltReader::ScXMLValidateEltReader(void)
: ScXMLEltReader("validate")
{
}

ScXMLElt *
ScXMLValidateEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * COIN_UNUSED_ARG(doc), ScXMLStateMachine * COIN_UNUSED_ARG(sm))
{
  assert(container && xmlelt);
  ScXMLValidateElt * validate = new ScXMLValidateElt;
  validate->setContainer(container);
  this->setXMLAttributes(validate, xmlelt);

  // handle XML attributes
  if (unlikely(!validate->handleXMLAttributes())) {
    delete validate;
    return NULL;
  }

  const int numchildren = cc_xml_elt_get_num_children(xmlelt);
  for (int c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(xmlelt, c);
    const char * elementtype = cc_xml_elt_get_type(element);

    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) {
      // ignore CDATA
      continue;
    }

    SoDebugError::post("ScXMLValidateEltReader::read",
                       "<validate> contains unexpected <%s> element", elementtype);
    delete validate;
    return NULL;
  }

  return validate;
}

// *************************************************************************

class ScXMLValidateElt::PImpl {
};

#define PRIVATE

SCXML_ELEMENT_SOURCE(ScXMLValidateElt);

void
ScXMLValidateElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLValidateElt, ScXMLElt, "ScXMLElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLValidateElt, "validate", ScXMLValidateEltReader);
}

void
ScXMLValidateElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLValidateElt);
  ScXMLValidateElt::classTypeId = SoType::badType();
}

ScXMLValidateElt::ScXMLValidateElt(void)
: location(NULL),
  schema(NULL)
{
}
ScXMLValidateElt::~ScXMLValidateElt(void)
{
  this->setLocationAttribute(NULL);
  this->setSchemaAttribute(NULL);
}

void
ScXMLValidateElt::setLocationAttribute(const char * locationstr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->location, "location", locationstr);
}

// const char * getLocationAttribute(void) const { return this->location; }

void
ScXMLValidateElt::setSchemaAttribute(const char * schemastr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->schema, "schema", schemastr);
}

// const char * getSchemaAttribute(void) const { return this->schema; }

SbBool
ScXMLValidateElt::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) {
    return FALSE;
  }

  this->setLocationAttribute(this->getXMLAttribute("location"));
  this->setSchemaAttribute(this->getXMLAttribute("schema"));

  return TRUE;
}

void
ScXMLValidateElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  const ScXMLValidateElt * orig = coin_assert_cast<const ScXMLValidateElt *>(rhs);
  this->setLocationAttribute(orig->getLocationAttribute());
  this->setSchemaAttribute(orig->getSchemaAttribute());
}

const ScXMLElt *
ScXMLValidateElt::search(const char * attrname, const char * attrvalue) const
{
  const ScXMLElt * hit = inherited::search(attrname, attrvalue);
  if (hit) {
    return hit;
  }
  if (strcmp(attrname, "location") == 0) {
    if (this->location && strcmp(attrvalue, this->location) == 0) {
      return this;
    }
  }
  else if (strcmp(attrname, "schema") == 0) {
    if (this->schema && strcmp(attrvalue, this->schema) == 0) {
      return this;
    }
  }
  return NULL;
}

void
ScXMLValidateElt::execute(ScXMLStateMachine * statemachine) const
{
  inherited::execute(statemachine);
  // FIXME: validate datamodel
}

#undef PRIVATE
