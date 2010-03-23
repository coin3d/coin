#ifndef SCXML_EVENTTARGET_H
#define SCXML_EVENTTARGET_H

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

#include <Inventor/scxml/ScXMLObject.h>
#include <Inventor/tools/SbPimplPtr.h>

class ScXMLSendElt;
class ScXMLEventElt;
class ScXMLEvent;

class COIN_DLL_API ScXMLEventTarget : public ScXMLObject {
  typedef ScXMLObject inherited;
  SCXML_OBJECT_ABSTRACT_HEADER(ScXMLEventTarget)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLEventTarget(void);
  virtual ~ScXMLEventTarget(void);

  // event-target identification
  virtual void setEventTargetType(const char * targettype);
  const char * getEventTargetType(void) const { return this->targettype; }

  virtual void setEventTargetName(const char * targetname);
  const char * getEventTargetName(void) const { return this->targetname; }

  // event processing
  virtual const ScXMLEvent * getCurrentEvent(void) const { return this->currentevent; }

  virtual void queueEvent(const ScXMLEvent * event);
  virtual void queueEvent(const SbName & eventid);

  virtual SbBool processEventQueue(void);

  // inter-system communication
  virtual SbBool sendExternalEvent(const ScXMLSendElt * sendelt);
  virtual SbBool sendInternalEvent(const ScXMLEventElt * sendelt);

  virtual void queueInternalEvent(const ScXMLEvent * event);
  virtual void queueInternalEvent(const SbName & eventid);

protected:
  char * targetname;
  char * targettype;

  const ScXMLEvent * currentevent;
  SbBool isprocessingqueue;

  virtual void setCurrentEvent(const ScXMLEvent * event);
  virtual const ScXMLEvent * getNextEvent(void);
  virtual const ScXMLEvent * getNextInternalEvent(void);
  virtual const ScXMLEvent * getNextExternalEvent(void);

  virtual SbBool processOneEvent(const ScXMLEvent * event);

  static void registerEventTarget(ScXMLEventTarget * target, const char * sessionid = NULL);
  static void unregisterEventTarget(ScXMLEventTarget * target, const char * sessionid = NULL);

  static ScXMLEventTarget * getEventTarget(const char * targettype, const char * targetname, const char * sessionid = NULL);

private:
  ScXMLEventTarget(const ScXMLEventTarget & rhs); // N/A
  ScXMLEventTarget & operator = (const ScXMLEventTarget & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLEventTarget

#endif // !SCXML_EVENTTARGET_H
