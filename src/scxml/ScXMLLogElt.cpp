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

#include <Inventor/scxml/ScXMLLogElt.h>

/*!
  \class ScXMLLogElt ScXMLLogElt.h Inventor/scxml/ScXMLLogElt.h
  \brief implements the &lt;log&gt; SCXML element.

  \ingroup scxml
*/

#include <cassert>
#include <cstring>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/scxml/ScXMLEvent.h>
#include <Inventor/scxml/ScXMLStateMachine.h>
#include <Inventor/scxml/ScXMLCoinEvaluator.h>

#include "scxml/ScXMLCommonP.h"
#include "SbBasicP.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::strcmp;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

// *************************************************************************

class ScXMLLogEltReader : public ScXMLEltReader {
public:
  ScXMLLogEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLLogEltReader::ScXMLLogEltReader(void)
: ScXMLEltReader("log")
{
}

ScXMLElt *
ScXMLLogEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * COIN_UNUSED_ARG(doc), ScXMLStateMachine * COIN_UNUSED_ARG(sm))
{
  assert(container && xmlelt);
  ScXMLLogElt * log = new ScXMLLogElt;
  log->setContainer(container);
  this->setXMLAttributes(log, xmlelt);

  // handle XML attributes
  if (unlikely(!log->handleXMLAttributes())) {
    delete log;
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
                       "<log> contains unexpected <%s> element", elementtype);
    delete log;
    return NULL;
  }

  return log;
}

// *************************************************************************

class ScXMLLogElt::PImpl {
};

#define PRIVATE

SCXML_ELEMENT_SOURCE(ScXMLLogElt);

void
ScXMLLogElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLLogElt, ScXMLExecutableElt, "ScXMLExecutableElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLLogElt, "log", ScXMLLogEltReader);
}

void
ScXMLLogElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLLogElt);
  ScXMLLogElt::classTypeId = SoType::badType();
}

ScXMLLogElt::ScXMLLogElt(void)
: label(NULL),
  expr(NULL),
  level(NULL)
{
}

ScXMLLogElt::~ScXMLLogElt(void)
{
  this->setLabelAttribute(NULL);
  this->setExprAttribute(NULL);
  this->setLevelAttribute(NULL);
}

void
ScXMLLogElt::setLabelAttribute(const char * labelstr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->label, "label", labelstr);
}

// const char * getLabelAttribute(void) const { return this->label; }

void
ScXMLLogElt::setExprAttribute(const char * exprstr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->expr, "expr", exprstr);
}

// const char * getExprAttribute(void) const { return this->expr; }

void
ScXMLLogElt::setLevelAttribute(const char * levelstr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->level, "level", levelstr);
}

// const char * getLevelAttribute(void) const { return this->level; }

SbBool
ScXMLLogElt::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) {
    return FALSE;
  }

  this->setLabelAttribute(this->getXMLAttribute("label"));
  this->setExprAttribute(this->getXMLAttribute("expr"));
  this->setLevelAttribute(this->getXMLAttribute("level"));

  return TRUE;
}

void
ScXMLLogElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  const ScXMLLogElt * orig = coin_assert_cast<const ScXMLLogElt *>(rhs);
  this->setLabelAttribute(orig->getLabelAttribute());
  this->setExprAttribute(orig->getExprAttribute());
  this->setLevelAttribute(orig->getLevelAttribute());
}

const ScXMLElt *
ScXMLLogElt::search(const char * attrname, const char * attrvalue) const
{
  const ScXMLElt * hit = inherited::search(attrname, attrvalue);
  if (hit) {
    return hit;
  }
  if (strcmp(attrname, "label") == 0) {
    if (this->label && strcmp(attrvalue, this->label) == 0) {
      return this;
    }
  }
  else if (strcmp(attrname, "expr") == 0) {
    if (this->expr && strcmp(attrvalue, this->expr) == 0) {
      return this;
    }
  }
  else if (strcmp(attrname, "level") == 0) {
    if (this->level && strcmp(attrvalue, this->level) == 0) {
      return this;
    }
  }
  return NULL;
}

void
ScXMLLogElt::execute(ScXMLStateMachine * statemachine) const
{
  inherited::execute(statemachine);

  int elementloglevel = 1;
  if (this->level) {
    elementloglevel = atoi(this->level);
  }
  if (elementloglevel < statemachine->getLogLevel()) {
    return;
  }

  // FIXME: log through another mechanism than printf()

  printf("SCXML (%s): %s\n", this->label ? this->label : "Log", this->expr ? this->expr : "<no message>");
  const ScXMLEvent * event = statemachine->getCurrentEvent();
  printf("> EVENT '%s'\n", event ? event->getEventName().getString() : "<no event>");
  if (event->getNumAssociations() > 0) {
    SbList<const char *> keylist;
    size_t numkeys = event->getAssociationKeys(keylist);
    for (size_t i = 0; i < numkeys; ++i) {
      const char * value = event->getAssociation(keylist[i]);
      printf("> - key '%s' carries value '%s'\n", keylist[i], value);
    }
  }
  for (int i = 0; i < statemachine->getNumActiveStates(); ++i) {
    const ScXMLElt * state = statemachine->getActiveState(i);
    const char * id = state->getXMLAttribute("id");
    printf("> SCXML active state: '%s' (type '%s')\n", id ? id : "<no id>",
      state->getTypeId().getName().getString());
  }
  ScXMLEvaluator * evaluator = statemachine->getEvaluator();
  if (evaluator && evaluator->isOfType(ScXMLCoinEvaluator::getClassTypeId())) {
    ScXMLCoinEvaluator * coinev = static_cast<ScXMLCoinEvaluator *>(evaluator);
    coinev->dumpTemporaries();
  }
}

#undef PRIVATE
