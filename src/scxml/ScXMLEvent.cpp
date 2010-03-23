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

#include <Inventor/scxml/ScXMLEvent.h>

/*!
  \class ScXMLEvent ScXMLEvent.h Inventor/scxml/ScXMLEvent.h
  \brief Base class for events sent to SCXML state machines.

  \since Coin 3.0
  \ingroup scxml
*/

#include <cassert>
#include <cstring>
#include <map>

#include <Inventor/scxml/ScXML.h>

// *************************************************************************

class ScXMLEvent::PImpl {
public:
  PImpl(void) { }
  ~PImpl(void)
  {
    // FIXME: delete strings in associations map (values only)
  }

  std::map<const char *, const char *> associations;
};

#define PRIVATE(obj) ((obj)->pimpl)

SCXML_OBJECT_SOURCE(ScXMLEvent);

void
ScXMLEvent::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLEvent, ScXMLObject, "ScXMLObject");
}

void
ScXMLEvent::cleanClass(void)
{
  ScXMLEvent::classTypeId = SoType::badType();
}

ScXMLEvent::ScXMLEvent(void)
: name(SbName::empty())
{
}

ScXMLEvent::~ScXMLEvent(void)
{
}

/*!
  This method is for setting a string that will identifies this particular
  event, having this particular state, and can be used for event matching
  in the SCXML descriptions.

  The string should, according to spec., be a set of tokens consisting
  of alphanumeric characters, separated with periods (.). This limitation
  is not enforced by this implementation.
*/
void
ScXMLEvent::setEventName(const SbName & namearg)
{
  this->name = namearg;
}

void
ScXMLEvent::setAssociation(const char * key, const char * value)
{
  assert(key && value);
  SbName dictentry(key);
  std::map<const char *, const char *>::iterator findit =
    PRIVATE(this)->associations.find(dictentry.getString());
  char * valuecopy = new char [strlen(value) + 1];
  strcpy(valuecopy, value);
  if (findit != PRIVATE(this)->associations.end()) {
    delete [] findit->second;
    findit->second = valuecopy;
  } else {
    std::pair<const char *, const char *> newentry(dictentry.getString(), valuecopy);
    PRIVATE(this)->associations.insert(newentry);
  }
}

const char *
ScXMLEvent::getAssociation(const char * key) const
{
  assert(key);
  SbName dictentry(key);
  std::map<const char *, const char *>::const_iterator findit =
    PRIVATE(this)->associations.find(dictentry.getString());
  if (findit != PRIVATE(this)->associations.end()) {
    return findit->second;
  }
  return NULL;
}

size_t
ScXMLEvent::getNumAssociations(void) const
{
  return PRIVATE(this)->associations.size();
}

size_t
ScXMLEvent::getAssociationKeys(SbList<const char *> & keys) const
{
  std::map<const char *, const char *>::const_iterator it =
    PRIVATE(this)->associations.begin();
  size_t count = 0;
  while (it != PRIVATE(this)->associations.end()) {
    keys.append(it->first);
    ++count;
    ++it;
  }
  return count;
}

ScXMLEvent *
ScXMLEvent::clone(void) const
{
  SoType thistype = this->getTypeId();
  assert(thistype.canCreateInstance());
  ScXMLEvent * newevent = static_cast<ScXMLEvent *>(thistype.createInstance());
  newevent->copyContents(this);
  return newevent;
}

void
ScXMLEvent::copyContents(const ScXMLEvent * rhs)
{
  this->name = rhs->name;
  SbList<const char *> keys;
  size_t numkeys = rhs->getAssociationKeys(keys);
  for (size_t i = 0; i < numkeys; ++i) {
    this->setAssociation(keys[i], rhs->getAssociation(keys[i]));
  }
}

#undef PRIVATE
