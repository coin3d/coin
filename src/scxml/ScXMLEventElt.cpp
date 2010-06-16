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

#include <Inventor/scxml/ScXMLEventElt.h>

/*!
  \class ScXMLEventElt ScXMLEventElt.h Inventor/scxml/ScXMLEventElt.h
  \brief implements the &lt;event&gt; SCXML element.

  \since Coin 3.1
  \ingroup scxml
*/

#include <cassert>
#include <cstring>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLEvent.h>
#include <Inventor/scxml/ScXMLStateMachine.h>

#include "scxml/ScXMLCommonP.h"
#include "SbBasicP.h"

// *************************************************************************

class ScXMLEventEltReader : public ScXMLEltReader {
public:
  ScXMLEventEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLEventEltReader::ScXMLEventEltReader(void)
: ScXMLEltReader("event")
{
}

ScXMLElt *
ScXMLEventEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * COIN_UNUSED_ARG(doc), ScXMLStateMachine * COIN_UNUSED_ARG(sm))
{
  assert(container && xmlelt);
  ScXMLEventElt * event = new ScXMLEventElt;
  event->setContainer(container);
  this->setXMLAttributes(event, xmlelt);

  // handle XML attributes
  if (unlikely(!event->handleXMLAttributes())) {
    delete event;
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

    SoDebugError::post("ScXMLEventEltReader::read",
                       "<event> contains unexpected <%s> element", elementtype);
    delete event;
    return NULL;
  }

  return event;
}

// *************************************************************************

class ScXMLEventElt::PImpl {
public:
};

#define PRIVATE


SCXML_ELEMENT_SOURCE(ScXMLEventElt);

void
ScXMLEventElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLEventElt, ScXMLExecutableElt, "ScXMLExecutableElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLEventElt, "event", ScXMLEventEltReader);
}

void
ScXMLEventElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLEventElt);
  ScXMLEventElt::classTypeId = SoType::badType();
}

ScXMLEventElt::ScXMLEventElt(void)
: name(NULL)
{
}

ScXMLEventElt::~ScXMLEventElt(void)
{
  this->setNameAttribute(NULL);
}

void
ScXMLEventElt::setNameAttribute(const char * namestr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->name, "name", namestr);
}

// const char * ScXMLEventElt::getNameAttribute(void) const

SbBool
ScXMLEventElt::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) {
    return FALSE;
  }

  this->setNameAttribute(this->getXMLAttribute("name"));

  return TRUE;
}

void
ScXMLEventElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  const ScXMLEventElt * orig = coin_assert_cast<const ScXMLEventElt *>(rhs);
  this->setNameAttribute(orig->getNameAttribute());
}


const ScXMLElt *
ScXMLEventElt::search(const char * attrname, const char * attrvalue) const
{
  const ScXMLElt * hit = inherited::search(attrname, attrvalue);
  if (hit) {
    return hit;
  }
  if (strcmp(attrname, "name") == 0) {
    if (this->name && strcmp(attrvalue, this->name)) {
      return this;
    }
  }
  return NULL;
}

ScXMLEvent *
ScXMLEventElt::createEvent(ScXMLEventTarget * COIN_UNUSED_ARG(host)) const
{
  ScXMLEvent * event = new ScXMLEvent;
  event->setEventName(this->getNameAttribute());
  return event;
}

void
ScXMLEventElt::execute(ScXMLStateMachine * statemachine) const
{
  inherited::execute(statemachine);
  statemachine->sendInternalEvent(this);
}

#undef PRIVATE
