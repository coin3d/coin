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

#include <Inventor/scxml/ScXMLScriptElt.h>

/*!
  \class ScXMLScriptElt ScXMLScriptElt.h Inventor/scxml/ScXMLScriptElt.h
  \brief implements the &lt;script&gt; SCXML element.

  \ingroup scxml
  \since Coin 3.1
*/

#include <cassert>
#include <cstring>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/element.h>

#include "coindefs.h"
#include "SbBasicP.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::strcmp;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

// *************************************************************************

class ScXMLScriptEltReader : public ScXMLEltReader {
public:
  ScXMLScriptEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLScriptEltReader::ScXMLScriptEltReader(void)
: ScXMLEltReader("script")
{
}

ScXMLElt *
ScXMLScriptEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * COIN_UNUSED_ARG(doc), ScXMLStateMachine * COIN_UNUSED_ARG(sm))
{
  assert(container && xmlelt);
  ScXMLScriptElt * script = new ScXMLScriptElt;
  script->setContainer(container);
  this->setXMLAttributes(script, xmlelt);

  // handle XML attributes
  if (unlikely(!script->handleXMLAttributes())) {
    delete script;
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

    SoDebugError::post("ScXMLScriptEltReader::read",
                       "<script> contains unexpected <%s> element", elementtype);
    delete script;
    return NULL;
   }

  return script;
}

// *************************************************************************

class ScXMLScriptElt::PImpl {
public:
};

#define PRIVATE

SCXML_ELEMENT_SOURCE(ScXMLScriptElt);

void
ScXMLScriptElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLScriptElt, ScXMLExecutableElt, "ScXMLExecutableElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLScriptElt, "script", ScXMLScriptEltReader);
}

void
ScXMLScriptElt::cleanClass(void)
{
}

ScXMLScriptElt::ScXMLScriptElt(void)
{
}

ScXMLScriptElt::~ScXMLScriptElt(void)
{
}

void
ScXMLScriptElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  /*const ScXMLScriptElt * orig = */coin_assert_cast<const ScXMLScriptElt *>(rhs);
}

void
ScXMLScriptElt::execute(ScXMLStateMachine * COIN_UNUSED_ARG(statemachine)) const
{
  // get evaluator (profile-determined) from statemachine
  // evaluate script
}

#undef PRIVATE
