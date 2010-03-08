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

#include <Inventor/scxml/ScXMLParamElt.h>

/*!
  \class ScXMLParamElt ScXMLParamElt.h Inventor/scxml/ScXMLParamElt.h
  \brief implements the &lt;param&gt; SCXML element.

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

class ScXMLParamEltReader : public ScXMLEltReader {
public:
  ScXMLParamEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLParamEltReader::ScXMLParamEltReader(void)
: ScXMLEltReader("param")
{
}

ScXMLElt *
ScXMLParamEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * COIN_UNUSED_ARG(doc), ScXMLStateMachine * COIN_UNUSED_ARG(sm))
{
  assert(container && xmlelt);
  ScXMLParamElt * param = new ScXMLParamElt;
  param->setContainer(container);
  this->setXMLAttributes(param, xmlelt);

  // handle XML attributes
  if (unlikely(!param->handleXMLAttributes())) {
    delete param;
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

    SoDebugError::post("ScXMLParamEltReader::read",
                       "<param> contains unexpected <%s> element", elementtype);
    delete param;
    return NULL;
   }

  return param;
}

// *************************************************************************

class ScXMLParamElt::PImpl {
};

#define PRIVATE

SCXML_ELEMENT_SOURCE(ScXMLParamElt);

void
ScXMLParamElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLParamElt, ScXMLElt, "ScXMLElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLParamElt, "param", ScXMLParamEltReader);
}

void
ScXMLParamElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLParamElt);
  ScXMLParamElt::classTypeId = SoType::badType();
}

ScXMLParamElt::ScXMLParamElt(void)
: name(NULL),
  expr(NULL)
{
}

ScXMLParamElt::~ScXMLParamElt(void)
{
  this->setNameAttribute(NULL);
  this->setExprAttribute(NULL);
}

void
ScXMLParamElt::setNameAttribute(const char * namestr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->name, "name", namestr);
}

// const char * ScXMLParamElt::getNameAttribute(void) const { return this->name; }

void
ScXMLParamElt::setExprAttribute(const char * exprstr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->expr, "expr", exprstr);
}

// const char * ScXMLParamElt::getExprAttribute(void) const { return this->expr; }

SbBool
ScXMLParamElt::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) {
    return FALSE;
  }

  this->setNameAttribute(this->getXMLAttribute("name"));
  this->setExprAttribute(this->getXMLAttribute("expr"));

  return TRUE;
}

void
ScXMLParamElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  const ScXMLParamElt * orig = coin_assert_cast<const ScXMLParamElt *>(rhs);
  this->setNameAttribute(orig->getNameAttribute());
  this->setExprAttribute(orig->getExprAttribute());
}

const ScXMLElt *
ScXMLParamElt::search(const char * attrname, const char * attrvalue) const
{
  const ScXMLElt * hit = inherited::search(attrname, attrvalue);
  if (hit) {
    return hit;
  }
  if (strcmp(attrname, "name") == 0) {
    if (this->name && strcmp(attrvalue, this->name) == 0) {
      return this;
    }
  }
  if (strcmp(attrname, "expr") == 0) {
    if (this->expr && strcmp(attrvalue, this->expr) == 0) {
      return this;
    }
  }
  return NULL;
}

#undef PRIVATE
