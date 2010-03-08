#ifndef COIN_SCXMLASSIGNELT_H
#define COIN_SCXMLASSIGNELT_H

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

class COIN_DLL_API ScXMLAssignElt : public ScXMLExecutableElt {
  typedef ScXMLExecutableElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLAssignElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLAssignElt(void);
  virtual ~ScXMLAssignElt(void);

  virtual void setLocationAttribute(const char * location);
  virtual const char * getLocationAttribute(void) const { return this->location; }

  virtual void setDataIDAttribute(const char * dataID);
  virtual const char * getDataIDAttribute(void) const { return this->dataID; }

  virtual void setExprAttribute(const char * expr);
  virtual const char * getExprAttribute(void) const { return this->expr; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  virtual void execute(ScXMLStateMachine * statemachine) const;

protected:
  char * location;
  char * dataID;
  char * expr;

private:
  ScXMLAssignElt(const ScXMLAssignElt & rhs); // N/A
  ScXMLAssignElt & operator = (const ScXMLAssignElt & rhs); // N/A

  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;

}; // ScXMLAssignElt

#endif // !COIN_SCXMLASSIGNELT_H
