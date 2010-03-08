#ifndef COIN_SCXMLIFELT_H
#define COIN_SCXMLIFELT_H

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

#include <Inventor/tools/SbPimplPtr.h>

class ScXMLElseElt;
class ScXMLElseIfElt;

class COIN_DLL_API ScXMLIfElt : public ScXMLExecutableElt {
  typedef ScXMLExecutableElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLIfElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLIfElt(void);
  virtual ~ScXMLIfElt(void);

  virtual void setCondAttribute(const char * cond);
  virtual const char * getCondAttribute(void) const { return this->cond; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  virtual int getNumElseIfs(void) const;
  virtual ScXMLElseIfElt * getElseIf(int idx) const;
  virtual void addElseIf(ScXMLElseIfElt * elseif);
  virtual void removeElseIf(ScXMLElseIfElt * elseif);
  virtual void clearAllElseIfs(void);

  virtual void setElse(ScXMLElseElt * elseelt);
  virtual ScXMLElseElt * getElse(void) const;

  virtual int getNumExecutables(const ScXMLExecutableElt * conditional) const;
  virtual ScXMLExecutableElt * getExecutable(const ScXMLExecutableElt * conditional, int idx) const;
  virtual void addExecutable(ScXMLExecutableElt * conditional, ScXMLExecutableElt * executable);
  virtual void removeExecutable(ScXMLExecutableElt * conditional, ScXMLExecutableElt * executable);
  virtual void clearAllExecutables(ScXMLExecutableElt * conditional);

  virtual void execute(ScXMLStateMachine * statemachine) const;

protected:
  char * cond;

private:
  ScXMLIfElt(const ScXMLIfElt & rhs); // N/A
  ScXMLIfElt & operator = (const ScXMLIfElt & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLIfElt

#endif // !COIN_SCXMLIFELT_H
