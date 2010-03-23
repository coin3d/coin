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

#include <Inventor/scxml/ScXMLElseElt.h>

/*!
  \class ScXMLElseElt ScXMLElseElt.h Inventor/scxml/ScXMLElseElt.h
  \brief implements the &lt;else&gt; SCXML element.

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

class ScXMLElseEltReader : public ScXMLEltReader {
public:
  ScXMLElseEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLElseEltReader::ScXMLElseEltReader(void)
: ScXMLEltReader("else")
{
}

ScXMLElt *
ScXMLElseEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * COIN_UNUSED_ARG(doc), ScXMLStateMachine * COIN_UNUSED_ARG(sm))
{
  assert(container && xmlelt);
  ScXMLElseElt * elseelt = new ScXMLElseElt;
  elseelt->setContainer(container);
  this->setXMLAttributes(elseelt, xmlelt);

  // handle XML attributes
  if (unlikely(!elseelt->handleXMLAttributes())) {
    delete elseelt;
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

    SoDebugError::post("ScXMLElseEltReader::read",
                       "<else> contains unexpected <%s> element", elementtype);
    delete elseelt;
    return NULL;
  }

  return elseelt;
}

// *************************************************************************

class ScXMLElseElt::PImpl {
public:
};

#define PRIVATE

SCXML_ELEMENT_SOURCE(ScXMLElseElt);

void
ScXMLElseElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLElseElt, ScXMLExecutableElt, "ScXMLExecutableElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLElseElt, "else", ScXMLElseEltReader);
}

void
ScXMLElseElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLElseElt);
  ScXMLElseElt::classTypeId = SoType::badType();
}

ScXMLElseElt::ScXMLElseElt(void)
{
}

ScXMLElseElt::~ScXMLElseElt(void)
{
}

void
ScXMLElseElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  /*const ScXMLElseElt * orig = */coin_assert_cast<const ScXMLElseElt *>(rhs);
}

#undef PRIVATE
