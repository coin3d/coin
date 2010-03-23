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

#include <Inventor/scxml/ScXMLHistoryElt.h>

/*!
  \class ScXMLHistoryElt ScXMLHistoryElt.h Inventor/scxml/ScXMLHistoryElt.h
  \brief implements the &lt;history&gt; SCXML element.

  The &lt;history&gt; element is not supported, other than as a dummy
  state.

  \since Coin 3.0
  \ingroup scxml
*/

#include <cassert>
#include <cstring>

#include <boost/scoped_ptr.hpp>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLTransitionElt.h>

#include "scxml/ScXMLCommonP.h"
#include "SbBasicP.h"

// *************************************************************************

class ScXMLHistoryEltReader : public ScXMLEltReader {
public:
  ScXMLHistoryEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLHistoryEltReader::ScXMLHistoryEltReader(void)
: ScXMLEltReader("history")
{
}

ScXMLElt *
ScXMLHistoryEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm)
{
  assert(container && xmlelt);
  ScXMLHistoryElt * history = new ScXMLHistoryElt;
  history->setContainer(container);
  this->setXMLAttributes(history, xmlelt);

  // handle XML attributes
  if (unlikely(!history->handleXMLAttributes())) {
    delete history;
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

    if (strcmp(elementtype, "transition") == 0) {
      // <transition> - zero or one
      if (unlikely(history->getTransition())) {
        SoDebugError::post("ScXMLHistoryEltReader::read",
                           "<history> elements can only have one <transition> element");
        delete history;
        return NULL;
      }
      ScXMLEltReader * transitionreader = ScXMLTransitionElt::getElementReader();
      ScXMLElt * transitionobj = transitionreader->read(history, element, doc, sm);
      if (unlikely(!transitionobj)) {
        delete history;
        return NULL;
      }
      assert(transitionobj->isOfType(ScXMLTransitionElt::getClassTypeId()));
      history->setTransition(static_cast<ScXMLTransitionElt *>(transitionobj));
    }

    else {
      SoDebugError::post("ScXMLHistoryEltReader::read",
                         "unexpected XML element '<%s>' found in <history>",
                         elementtype);
      delete history;
      return NULL;
    }
  }

  return history;
} // readScXMLHistory

// *************************************************************************

class ScXMLHistoryElt::PImpl {
public:
  PImpl(void) : transitionptr(NULL) { }

  boost::scoped_ptr<ScXMLTransitionElt> transitionptr;

};

#define PRIVATE(obj) ((obj)->pimpl)

SCXML_ELEMENT_SOURCE(ScXMLHistoryElt);

void
ScXMLHistoryElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLHistoryElt, ScXMLElt, "ScXMLElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLHistoryElt, "history", ScXMLHistoryEltReader);
}

void
ScXMLHistoryElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLHistoryElt);
  ScXMLHistoryElt::classTypeId = SoType::badType();
}

ScXMLHistoryElt::ScXMLHistoryElt(void)
: id(NULL),
  type(NULL)
{
}

ScXMLHistoryElt::~ScXMLHistoryElt(void)
{
  this->setIdAttribute(NULL);
  this->setTypeAttribute(NULL);
}

void
ScXMLHistoryElt::setIdAttribute(const char * idstr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->id, "id", idstr);
}

void
ScXMLHistoryElt::setTypeAttribute(const char * typestr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->type, "type", typestr);
}

SbBool
ScXMLHistoryElt::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  this->setIdAttribute(this->getXMLAttribute("id"));
  this->setTypeAttribute(this->getXMLAttribute("type"));

  if (!this->id) {
    SoDebugError::post("ScXMLHistoryElt::handleXMLAttributes",
                       "no 'id' attribute");
    return FALSE;
  }

  if (this->type) {
    if ((strcmp(this->type, "deep") != 0) &&
        (strcmp(this->type, "shallow") != 0)) {
      SoDebugError::post("ScXMLHistoryElt::handleXMLAttributes",
                         "'type' attribute must be 'deep' or 'shalow'");
      return FALSE;
    }
  }

  return TRUE;
}

void
ScXMLHistoryElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  const ScXMLHistoryElt * orig = coin_assert_cast<const ScXMLHistoryElt *>(rhs);
  this->setIdAttribute(orig->getIdAttribute());
  this->setTypeAttribute(orig->getTypeAttribute());
}

const ScXMLElt *
ScXMLHistoryElt::search(const char * attrname, const char * attrvalue) const
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
  else if (strcmp(attrname, "type") == 0) {
    if (this->type && strcmp(attrvalue, this->type) == 0) {
      return this;
    }
  }
  return NULL;
}

SCXML_SINGLE_OBJECT_API_IMPL(ScXMLHistoryElt, ScXMLTransitionElt, PRIVATE(this)->transitionptr, Transition);

#undef PRIVATE
