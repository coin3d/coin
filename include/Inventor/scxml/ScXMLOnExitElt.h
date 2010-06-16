#ifndef COIN_SCXMLONEXITELT_H
#define COIN_SCXMLONEXITELT_H

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

#include <Inventor/scxml/ScXMLElt.h>
#include <Inventor/tools/SbPimplPtr.h>

class ScXMLExecutableElt;
class ScXMLEvent;
class ScXMLStateMachine;

class COIN_DLL_API ScXMLOnExitElt : public ScXMLElt {
  typedef ScXMLElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLOnExitElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLOnExitElt(void);
  virtual ~ScXMLOnExitElt(void);

  // executable content
  virtual int getNumExecutables(void) const;
  virtual ScXMLExecutableElt * getExecutable(int idx) const;
  virtual void addExecutable(ScXMLExecutableElt * executable);
  virtual void removeExecutable(ScXMLExecutableElt * executable);
  virtual void clearAllExecutables(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  virtual void execute(ScXMLStateMachine * statemachine) const;

private:
  ScXMLOnExitElt(const ScXMLOnExitElt & rhs); // N/A
  ScXMLOnExitElt & operator = (const ScXMLOnExitElt & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLOnExitElt

#endif // !COIN_SCXMLONEXITELT_H
