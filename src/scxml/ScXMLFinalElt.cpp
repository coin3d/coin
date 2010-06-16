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

#include <Inventor/scxml/ScXMLFinalElt.h>

/*!
  \class ScXMLFinalElt ScXMLFinalElt.h Inventor/scxml/ScXMLFinalElt.h
  \brief implements the &lt;final&gt; SCXML element.

  A &lt;final&gt; element takes an "id" attribute (required), and can
  have one child of type &lt;onentry&gt; and &lt;onexit&gt;.

  \since Coin 3.0
  \ingroup scxml
*/

#include <cassert>
#include <cstring>

#include <boost/scoped_ptr.hpp>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLOnEntryElt.h>
#include <Inventor/scxml/ScXMLOnExitElt.h>

#include "scxml/ScXMLCommonP.h"

// *************************************************************************

class ScXMLFinalEltReader : public ScXMLEltReader {
public:
  ScXMLFinalEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);

};

ScXMLFinalEltReader::ScXMLFinalEltReader(void)
: ScXMLEltReader("final")
{
}

ScXMLElt *
ScXMLFinalEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm)
{
  assert(container && xmlelt);

  ScXMLFinalElt * final = new ScXMLFinalElt;
  final->setContainer(container);
  this->setXMLAttributes(final, xmlelt);

  // handle XML attributes
  if (unlikely(!final->handleXMLAttributes())) {
    delete final;
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

    if (strcmp(elementtype, "onentry") == 0) {
      if (unlikely(final->getOnEntry())) {
        SoDebugError::post("ScXMLFinalEltReader::read",
                           "<final> element can only contain one <onentry> element");
        delete final;
        return NULL;
      }

      ScXMLEltReader * onentryreader = ScXMLOnEntryElt::getElementReader();
      assert(onentryreader);
      ScXMLElt * onentryelt = onentryreader->read(final, element, doc, sm);

      if (unlikely(!onentryelt)) {
        delete final;
        return NULL;
      }
      assert(final->isOfType(ScXMLOnEntryElt::getClassTypeId()));
      final->setOnEntry(static_cast<ScXMLOnEntryElt *>(onentryelt));
    }

    else if (strcmp(elementtype, "onexit") == 0) {
      if (unlikely(final->getOnExit())) {
        SoDebugError::post("ScXMLFinalEltReader::read",
                           "<final> element can only contain one <onexit> element");
        delete final;
        return NULL;
      }

      ScXMLEltReader * onexitreader = ScXMLOnExitElt::getElementReader();
      assert(onexitreader);
      ScXMLElt * onexitelt = onexitreader->read(final, element, doc, sm);

      if (unlikely(!onexitelt)) {
        delete final;
        return NULL;
      }
      assert(onexitelt->isOfType(ScXMLOnExitElt::getClassTypeId()));
      final->setOnExit(static_cast<ScXMLOnExitElt *>(onexitelt));
    }

    else {
      SoDebugError::post("ScXMLFinalEltReader::read",
                         "unexpected XML element '<%s>' found in <final>",
                         elementtype);
      delete final;
      return NULL;
    }
  }

  return final;
} // readScXMLFinal

// *************************************************************************

class ScXMLFinalElt::PImpl {
public:
  PImpl(void) : onentry(NULL), onexit(NULL) { }

  boost::scoped_ptr<ScXMLOnEntryElt> onentry;
  boost::scoped_ptr<ScXMLOnExitElt> onexit;

};

#define PRIVATE(obj) ((obj)->pimpl)

SCXML_ELEMENT_SOURCE(ScXMLFinalElt);

void
ScXMLFinalElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLFinalElt, ScXMLAbstractStateElt, "ScXMLAbstractStateElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLFinalElt, "final", ScXMLFinalEltReader);
}

void
ScXMLFinalElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLFinalElt);
  ScXMLFinalElt::classTypeId = SoType::badType();
}

ScXMLFinalElt::ScXMLFinalElt(void)
{
}

ScXMLFinalElt::~ScXMLFinalElt(void)
{
}

void
ScXMLFinalElt::setOnEntry(ScXMLOnEntryElt * onentry)
{
  PRIVATE(this)->onentry.reset(onentry);
}

ScXMLOnEntryElt *
ScXMLFinalElt::getOnEntry(void) const
{
  return PRIVATE(this)->onentry.get();
}

void
ScXMLFinalElt::setOnExit(ScXMLOnExitElt * onexit)
{
  PRIVATE(this)->onexit.reset(onexit);
}

ScXMLOnExitElt *
ScXMLFinalElt::getOnExit(void) const
{
  return PRIVATE(this)->onexit.get();
}

void
ScXMLFinalElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  const ScXMLFinalElt * orig = coin_assert_cast<const ScXMLFinalElt *>(rhs);
  if (orig->getOnEntry()) {
    ScXMLOnEntryElt * onentry =
      coin_assert_cast<ScXMLOnEntryElt *>(orig->getOnEntry()->clone());
    this->setOnEntry(onentry);
  }
  if (orig->getOnExit()) {
    ScXMLOnExitElt * onexit =
      coin_assert_cast<ScXMLOnExitElt *>(orig->getOnExit()->clone());
    this->setOnExit(onexit);
  }
}

const ScXMLElt *
ScXMLFinalElt::search(const char * attrname, const char * attrvalue) const
{
  const ScXMLElt * hit = inherited::search(attrname, attrvalue);
  if (hit) {
    return hit;
  }
  if (PRIVATE(this)->onentry.get()) {
    hit = PRIVATE(this)->onentry->search(attrname, attrvalue);
    if (hit) {
      return hit;
    }
  }
  if (PRIVATE(this)->onexit.get()) {
    hit = PRIVATE(this)->onexit->search(attrname, attrvalue);
    if (hit) {
      return hit;
    }
  }
  return NULL;
}

#undef PRIVATE
