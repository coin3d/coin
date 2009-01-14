#ifndef COIN_SCXMLSTATE_H
#define COIN_SCXMLSTATE_H

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

#include <Inventor/scxml/ScXMLObject.h>
#include <Inventor/tools/SbPimplPtr.h>

class ScXMLOnEntry;
class ScXMLOnExit;
class ScXMLTransition;
class ScXMLState;
class ScXMLInitial;
class ScXMLFinal;
class ScXMLInvoke;
class ScXMLHistory;
class ScXMLAnchor;
class ScXMLEvent;
class ScXMLStateMachine;

class ScXMLState : public ScXMLObject {
  typedef ScXMLObject inherited;
  SCXML_OBJECT_HEADER(ScXMLState);

public:
  static void initClass(void);

  ScXMLState(void);
  virtual ~ScXMLState(void);

  // pre-validation states
  void setIsParallel(SbBool parallel);
  SbBool isParallel(void) const;

  // XML attributes
  virtual void setIdAttribute(const char * id);
  const char * getIdAttribute(void) const { return this->id; }

  virtual void setSrcAttribute(const char * src);
  const char * getSrcAttribute(void) const { return this->src; }
  
  virtual void setTaskAttribute(const char * task);
  const char * getTaskAttribute(void) const { return this->task; }

  virtual SbBool handleXMLAttributes(void);

  // children...
  virtual void setOnEntry(ScXMLOnEntry * onentry);
  virtual ScXMLOnEntry * getOnEntry(void) const;

  virtual void setOnExit(ScXMLOnExit * onexit);
  virtual ScXMLOnExit * getOnExit(void) const;

  virtual int getNumTransitions(void) const;
  virtual ScXMLTransition * getTransition(int idx) const;
  virtual void addTransition(ScXMLTransition * transition);
  virtual void removeTransition(ScXMLTransition * transition);
  virtual void clearAllTransitions(void);

  virtual void setInitial(ScXMLInitial * initial);
  virtual ScXMLInitial * getInitial(void) const;

  virtual int getNumStates(void) const;
  virtual ScXMLState * getState(int idx) const;
  virtual void addState(ScXMLState * state);
  virtual void removeState(ScXMLState * state);
  virtual void clearAllStates(void);

  virtual int getNumParallels(void) const;
  virtual ScXMLState * getParallel(int idx) const;
  virtual void addParallel(ScXMLState * state);
  virtual void removeParallel(ScXMLState * state);
  virtual void clearAllParallels(void);

  virtual int getNumFinals(void) const;
  virtual ScXMLFinal * getFinal(int idx) const;
  virtual void addFinal(ScXMLFinal * state);
  virtual void removeFinal(ScXMLFinal * state);
  virtual void clearAllFinals(void);

  virtual int getNumHistories(void) const;
  virtual ScXMLHistory * getHistory(int idx) const;
  virtual void addHistory(ScXMLHistory * history);
  virtual void removeHistory(ScXMLHistory * history);
  virtual void clearAllHistories(void);

  virtual int getNumAnchors(void) const;
  virtual ScXMLAnchor * getAnchor(int idx) const;
  virtual void addAnchor(ScXMLAnchor * anchor);
  virtual void removeAnchor(ScXMLAnchor * anchor);
  virtual void clearAllAnchors(void);

  // no datamodel support yet

  virtual void setInvoke(ScXMLInvoke * invoke);
  virtual ScXMLInvoke * getInvoke(void) const;

  virtual void invoke(ScXMLStateMachine * statemachine);

  SbBool isAtomicState(void) const;
  SbBool isTask(void) const;

protected:
  SbBool isparallel;
  SbBool istask;

  char * id;
  char * src;
  char * task;

private:
  ScXMLState(const ScXMLState & rhs); // N/A
  ScXMLState & operator = (const ScXMLState & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLState

#endif // !COIN_SCXMLSTATE_H
