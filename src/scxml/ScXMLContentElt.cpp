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

#include <Inventor/scxml/ScXMLContentElt.h>

/*!
  \class ScXMLContentElt Inventor/scxml/ScXMLContentElt.h
  \brief implements the &lt;content&gt; SCXML element.

  \ingroup scxml
*/

#include <cassert>
#include <cstring>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/element.h>

#include "coindefs.h"
#include "scxml/ScXMLCommonP.h"
#include "SbBasicP.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::strcmp;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

// *************************************************************************

class ScXMLContentEltReader : public ScXMLEltReader {
public:
  ScXMLContentEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLContentEltReader::ScXMLContentEltReader(void)
: ScXMLEltReader("content")
{
}

ScXMLElt *
ScXMLContentEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * COIN_UNUSED_ARG(doc), ScXMLStateMachine * COIN_UNUSED_ARG(sm))
{
  assert(container && xmlelt);
  ScXMLContentElt * content = new ScXMLContentElt;
  content->setContainer(container);
  this->setXMLAttributes(content, xmlelt);

  // handle XML attributes
  if (unlikely(!content->handleXMLAttributes())) {
    delete content;
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

    SoDebugError::post("ScXMLContentEltReader::read",
                       "unexpected XML element '<%s>' found in <content>",
                       elementtype);
    delete content;
    return NULL;
  }


  return content;
}

// *************************************************************************

class ScXMLContentElt::PImpl {
public:
};

#define PRIVATE

SCXML_ELEMENT_SOURCE(ScXMLContentElt);

void
ScXMLContentElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLContentElt, ScXMLElt, "ScXMLElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLContentElt, "content", ScXMLContentEltReader);
}

void
ScXMLContentElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLContentElt);
  ScXMLContentElt::classTypeId = SoType::badType();
}

ScXMLContentElt::ScXMLContentElt(void)
{
}

ScXMLContentElt::~ScXMLContentElt(void)
{
}

void
ScXMLContentElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  /*const ScXMLContentElt * orig = */coin_assert_cast<const ScXMLContentElt *>(rhs);
}

#undef PRIVATE
