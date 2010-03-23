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

#include <Inventor/scxml/ScXMLElseIfElt.h>

/*!
  \class ScXMLElseIfElt ScXMLElseIfElt.h Inventor/scxml/ScXMLElseIfElt.h
  \brief implements the &lt;elseif&gt; SCXML element.

  The &lt;elseif&gt; element can have the attribute "cond" and no children.
  The &lt;elseif&gt; element is a separator element for the enclosing &lt;if&gt;
  element. It can only be used inside &lt;if&gt; elements.

  \ingroup scxml
  \since Coin 3.1
*/

#include <cassert>
#include <cstring>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/scxml/ScXMLIfElt.h>

#include "scxml/ScXMLCommonP.h"
#include "SbBasicP.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::strcmp;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

// *************************************************************************

class ScXMLElseIfEltReader : public ScXMLEltReader {
public:
  ScXMLElseIfEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLElseIfEltReader::ScXMLElseIfEltReader(void)
: ScXMLEltReader("elseif")
{
}

ScXMLElt *
ScXMLElseIfEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * COIN_UNUSED_ARG(doc), ScXMLStateMachine * COIN_UNUSED_ARG(sm))
{
  assert(container && xmlelt);
  ScXMLElseIfElt * elseifelt = new ScXMLElseIfElt;
  elseifelt->setContainer(container);
  this->setXMLAttributes(elseifelt, xmlelt);

  // handle XML attributes
  if (unlikely(!elseifelt->handleXMLAttributes())) {
    delete elseifelt;
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

    SoDebugError::post("ScXMLElseIfEltReader::read",
                       "<elseif> contains unexpected <%s> element", elementtype);
    delete elseifelt;
    return NULL;
  }

  return elseifelt;
}

// *************************************************************************

class ScXMLElseIfElt::PImpl {
public:
};

#define PRIVATE(obj) ((obj)->pimpl)

SCXML_ELEMENT_SOURCE(ScXMLElseIfElt);

void
ScXMLElseIfElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLElseIfElt, ScXMLExecutableElt, "ScXMLExecutableElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLElseIfElt, "elseif", ScXMLElseIfEltReader);
}

void
ScXMLElseIfElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLElseIfElt);
  ScXMLElseIfElt::classTypeId = SoType::badType();
}

ScXMLElseIfElt::ScXMLElseIfElt(void)
: cond(NULL)
{
}

ScXMLElseIfElt::~ScXMLElseIfElt(void)
{
  this->setCondAttribute(NULL);
}

void
ScXMLElseIfElt::setCondAttribute(const char * condstr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->cond, "cond", condstr);
}

// const char * ScXMLElseIfElt::getCondAttribute(void) const

SbBool
ScXMLElseIfElt::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) {
    return FALSE;
  }

  this->setCondAttribute(this->getXMLAttribute("cond"));

  return TRUE;
}

void
ScXMLElseIfElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  const ScXMLElseIfElt * orig = coin_assert_cast<const ScXMLElseIfElt *>(rhs);
  this->setCondAttribute(orig->getCondAttribute());
}

const ScXMLElt *
ScXMLElseIfElt::search(const char * attrname, const char * attrvalue) const
{
  const ScXMLElt * hit = inherited::search(attrname, attrvalue);
  if (hit) {
    return hit;
  }
  if (strcmp(attrname, "cond") == 0) {
    if (this->cond && strcmp(attrvalue, this->cond) == 0) {
      return this;
    }
  }
  return NULL;
}

#undef PRIVATE
