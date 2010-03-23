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

#include <Inventor/scxml/ScXMLFinalizeElt.h>

/*!
  \class ScXMLFinalizeElt ScXMLFinalizeElt.h Inventor/scxml/ScXMLFinalizeElt.h
  \brief implements the &lt;finalize&gt; SCXML element.

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

class ScXMLFinalizeEltReader : public ScXMLEltReader {
public:
  ScXMLFinalizeEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLFinalizeEltReader::ScXMLFinalizeEltReader(void)
: ScXMLEltReader("finalize")
{
}

ScXMLElt *
ScXMLFinalizeEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * COIN_UNUSED_ARG(doc), ScXMLStateMachine * COIN_UNUSED_ARG(sm))
{
  assert(container && xmlelt);
  ScXMLFinalizeElt * finalize = new ScXMLFinalizeElt;
  finalize->setContainer(container);
  this->setXMLAttributes(finalize, xmlelt);

  // handle XML attributes
  if (unlikely(!finalize->handleXMLAttributes())) {
    delete finalize;
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

    SoDebugError::post("ScXMLFinalizeEltReader::read",
                       "<finalize> contains unexpected <%s> element", elementtype);
    delete finalize;
    return NULL;
  }

  return finalize;
}

// *************************************************************************

class ScXMLFinalizeElt::PImpl {
public:
};

#define PRIVATE

SCXML_ELEMENT_SOURCE(ScXMLFinalizeElt);

void
ScXMLFinalizeElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLFinalizeElt, ScXMLElt, "ScXMLElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLFinalizeElt, "finalize", ScXMLFinalizeEltReader);
}

void
ScXMLFinalizeElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLFinalizeElt);
  ScXMLFinalizeElt::classTypeId = SoType::badType();
}

ScXMLFinalizeElt::ScXMLFinalizeElt(void)
{
}

ScXMLFinalizeElt::~ScXMLFinalizeElt(void)
{
}

void
ScXMLFinalizeElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  /*const ScXMLFinalizeElt * orig = */coin_assert_cast<const ScXMLFinalizeElt *>(rhs);
}

#undef PRIVATE
