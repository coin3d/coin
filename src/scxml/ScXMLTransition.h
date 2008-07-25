#ifndef COIN_SCXMLTRANSITION_H
#define COIN_SCXMLTRANSITION_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
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

#include <Inventor/scxml/ScXMLObject.h>
#include <Inventor/tools/SbPimplPtr.h>

class ScXMLEvent;
class ScXMLInvoke;
class ScXMLStateMachine;

class ScXMLTransition : public ScXMLObject {
  typedef ScXMLObject inherited;
  SCXML_OBJECT_HEADER(ScXMLTransition);
public:
  static void initClass(void);

  ScXMLTransition(void);
  virtual ~ScXMLTransition(void);

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

  // predicates
  SbBool isConditionLess(void) const;
  SbBool isTargetLess(void) const;
  SbBool isSelfReferencing(void) const;

  // check
  virtual SbBool isEventMatch(const ScXMLEvent * event) const;
  // isConditionMatch() ?

  // executable content
  virtual int getNumInvokes(void) const;
  virtual ScXMLInvoke * getInvoke(int idx) const;
  virtual void addInvoke(ScXMLInvoke * invoke);
  virtual void removeInvoke(ScXMLInvoke * invoke);
  virtual void clearAllInvokes(void);

  // invoke
  virtual SbBool evaluateCondition(ScXMLStateMachine * statemachine);
  virtual void invoke(ScXMLStateMachine * statemachine);

protected:
  char * event;
  char * cond;
  char * target;
  char * anchor;

  SbBool needprefixmatching;
  SbName eventkey;
  SbName targetkey;

private:
  ScXMLTransition(const ScXMLTransition & rhs); // N/A
  ScXMLTransition & operator = (const ScXMLTransition & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLTransition

#endif // !COIN_SCXMLTRANSITION_H
