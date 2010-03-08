#ifndef COIN_SCXMLEVENTELT_H
#define COIN_SCXMLEVENTELT_H

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

class COIN_DLL_API ScXMLEventElt : public ScXMLExecutableElt {
  typedef ScXMLExecutableElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLEventElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLEventElt(void);
  virtual ~ScXMLEventElt(void);

  virtual void setNameAttribute(const char * name);
  virtual const char * getNameAttribute(void) const { return this->name; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  ScXMLEvent * createEvent(ScXMLEventTarget * host) const;

  virtual void execute(ScXMLStateMachine * statemachine) const;

protected:
  char * name;

private:
  ScXMLEventElt(const ScXMLEventElt & rhs); // N/A
  ScXMLEventElt & operator = (const ScXMLEventElt & rhs); // N/A

  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;

}; // ScXMLEventElt

#endif // !COIN_SCXMLEVENTELT_H
