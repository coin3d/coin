/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/scxml/SoScXMLEvent.h>

#include <assert.h>

#include <Inventor/SbString.h>
#include <Inventor/events/SoEvents.h>
#include <Inventor/scxml/ScXML.h>

// *************************************************************************

/*!
  \class SoScXMLEvent SoScXMLEvent.h Inventor/scxml/SoScXMLEvent.h
  \brief Adds an SoEvent holder to the ScXMLEvent base.

  This class is part of integrating the Coin types with the SCXML
  subsystem.  SoScXMLEvent objects are ScXMLEvents that wraps/passes
  an SoEvent to the SCXML state machine.

  \since Coin 3.0
  \ingroup scxml
*/

SCXML_OBJECT_SOURCE(SoScXMLEvent);

void
SoScXMLEvent::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(SoScXMLEvent, ScXMLEvent, SCXML_COIN_NS, "event");
}

SoScXMLEvent::SoScXMLEvent(void)
  : soeventptr(NULL)
{
}

SoScXMLEvent::~SoScXMLEvent(void)
{
  // soevent is intentinally leaked
}

/*!
  This function stores a pointer to the originator SoEvent that
  caused the given event, if any.

  There is no resource management with regards to this pointer - it
  has to be kept alive on the outside for as long as this event may
  live, and be deallocated on the outside as well.

  \sa setUpIdentifiers
*/
void
SoScXMLEvent::setSoEvent(const SoEvent * soevent)
{
  this->soeventptr = soevent;
}

/*!
  Returns the pointer stored by setSoEvent() or NULL if no pointer
  has been stored yet.

  \sa setSoEvent
*/
const SoEvent *
SoScXMLEvent::getSoEvent(void) const
{
  return this->soeventptr;
}

/*!
  This method updates the ScXMLEvent event class and event
  identifier strings based on the set SoEvent object.

  The identifiers will be cleared if no SoEvent object has been
  stored.

  \sa setSoEvent
*/
void
SoScXMLEvent::setUpIdentifier(void)
{
  if (this->soeventptr == NULL) {
    this->setIdentifier("");
    return;
  }

  const char * prefix = "sim.coin3d.coin";
  const char * eventclassname = "";
  SbString eventdata;
  if (this->soeventptr->isOfType(SoKeyboardEvent::getClassTypeId())) {
    eventclassname = "SoKeyboardEvent";
    const SoKeyboardEvent * mbevent = static_cast<const SoKeyboardEvent *>(this->soeventptr);
    SbString statestr;
    SoButtonEvent::enumToString(mbevent->getState(), statestr);
    SbString keystr;
    SoKeyboardEvent::enumToString(mbevent->getKey(), keystr);
    eventdata.sprintf("%s.%s", statestr.getString(), keystr.getString());
  }
  else if (this->soeventptr->isOfType(SoMouseButtonEvent::getClassTypeId())) {
    eventclassname = "SoMouseButtonEvent";
    const SoMouseButtonEvent * mbevent = static_cast<const SoMouseButtonEvent *>(this->soeventptr);
    SbString statestr;
    SoButtonEvent::enumToString(mbevent->getState(), statestr);
    SbString buttonstr;
    SoMouseButtonEvent::enumToString(mbevent->getButton(), buttonstr);
    eventdata.sprintf("%s.%s", statestr.getString(), buttonstr.getString());
  }
  else if (this->soeventptr->isOfType(SoSpaceballButtonEvent::getClassTypeId())) {
    eventclassname = "SoSpaceballButtonEvent";
    const SoSpaceballButtonEvent * sbevent =
      static_cast<const SoSpaceballButtonEvent *>(this->soeventptr);
    SbString statestr;
    SoButtonEvent::enumToString(sbevent->getState(), statestr);
    SbString buttonstr;
    SoSpaceballButtonEvent::enumToString(sbevent->getButton(), buttonstr);
    eventdata.sprintf("%s.%s", statestr.getString(), buttonstr.getString());
  }
  else if (this->soeventptr->isOfType(SoLocation2Event::getClassTypeId())) {
    eventclassname = "SoLocation2Event";
    // no eventdata to present
  }
  else if (this->soeventptr->isOfType(SoMotion3Event::getClassTypeId())) {
    eventclassname = "SoMotion3Event";
    // no eventdata to present
  }
  else {
    // fallback mechanism
    eventclassname = this->soeventptr->getTypeId().getName().getString();
  }

  SbString classidentifierstr;
  classidentifierstr.sprintf("%s.%s", prefix, eventclassname);
  if (eventdata.getLength() > 0) {
    SbString identifierstr;
    identifierstr.sprintf("%s.%s.%s", prefix, eventclassname, eventdata.getString());
    this->setIdentifier(identifierstr.getString());
  } else {
    this->setIdentifier(classidentifierstr.getString());
  }
}

// *************************************************************************
