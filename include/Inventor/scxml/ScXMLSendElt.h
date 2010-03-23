#ifndef COIN_SCXMLSENDELT_H
#define COIN_SCXMLSENDELT_H

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

#include <Inventor/scxml/ScXMLExecutableElt.h>

#include <Inventor/tools/SbLazyPimplPtr.h>

class ScXMLEvent;
class ScXMLEventTarget;

class COIN_DLL_API ScXMLSendElt : public ScXMLExecutableElt {
  typedef ScXMLExecutableElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLSendElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLSendElt(void);
  virtual ~ScXMLSendElt(void);

  virtual void setEventAttribute(const char * event);
  virtual const char * getEventAttribute(void) const { return this->event; }

  virtual void setTargetAttribute(const char * event);
  virtual const char * getTargetAttribute(void) const { return this->target; }

  virtual void setTargetTypeAttribute(const char * event);
  virtual const char * getTargetTypeAttribute(void) const { return this->targettype; }

  virtual void setSendIDAttribute(const char * event);
  virtual const char * getSendIDAttribute(void) const { return this->sendid; }

  virtual void setDelayAttribute(const char * event);
  virtual const char * getDelayAttribute(void) const { return this->delay; }

  virtual void setNameListAttribute(const char * event);
  virtual const char * getNameListAttribute(void) const { return this->namelist; }

  virtual void setHintsAttribute(const char * event);
  virtual const char * getHintsAttribute(void) const { return this->hints; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  ScXMLEvent * createEvent(ScXMLEventTarget * host) const;

  virtual void execute(ScXMLStateMachine * statemachine) const;

protected:
  char * event;
  char * target;
  char * targettype;
  char * sendid;
  char * delay;
  char * namelist;
  char * hints;

private:
  ScXMLSendElt(const ScXMLSendElt & rhs); // N/A
  ScXMLSendElt & operator = (const ScXMLSendElt & rhs); // N/A

  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;

}; // ScXMLSendElt

#endif // !COIN_SCXMLSENDELT_H

