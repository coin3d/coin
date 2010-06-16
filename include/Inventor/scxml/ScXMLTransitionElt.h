#ifndef COIN_SCXMLTRANSITIONELT_H
#define COIN_SCXMLTRANSITIONELT_H

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

class ScXMLEvent;
class ScXMLExecutableElt;
class ScXMLStateMachine;

class COIN_DLL_API ScXMLTransitionElt : public ScXMLElt {
  typedef ScXMLElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLTransitionElt)
public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLTransitionElt(void);
  virtual ~ScXMLTransitionElt(void);

  // XML attributes
  virtual void setEventAttribute(const char * event);
  const char * getEventAttribute(void) const { return this->event; }

  virtual void setCondAttribute(const char * cond);
  const char * getCondAttribute(void) const { return this->cond; }

  virtual void setTargetAttribute(const char * target);
  const char * getTargetAttribute(void) const { return this->target; }

  virtual void setAnchorAttribute(const char * anchor);
  const char * getAnchorAttribute(void) const { return this->anchor; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  // predicates
  SbBool isConditionLess(void) const;
  SbBool isTargetLess(void) const;
  SbBool isSelfReferencing(void) const;

  // check
  virtual SbBool isEventMatch(const ScXMLEvent * event) const;
  // isConditionMatch() ?
  virtual SbBool evaluateCondition(ScXMLStateMachine * statemachine);

  // executable content
  virtual int getNumExecutables(void) const;
  virtual ScXMLExecutableElt * getExecutable(int idx) const;
  virtual void addExecutable(ScXMLExecutableElt * executable);
  virtual void removeExecutable(ScXMLExecutableElt * executable);
  virtual void clearAllExecutables(void);

  virtual void execute(ScXMLStateMachine * statemachine) const;

protected:
  char * event;
  char * cond;
  char * target;
  char * anchor;

  SbBool needprefixmatching;
  SbName eventkey;
  SbName targetkey;

private:
  ScXMLTransitionElt(const ScXMLTransitionElt & rhs); // N/A
  ScXMLTransitionElt & operator = (const ScXMLTransitionElt & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLTransitionElt

#endif // !COIN_SCXMLTRANSITIONELT_H
