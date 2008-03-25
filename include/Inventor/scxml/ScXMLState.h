#ifndef COIN_SCXMLSTATE_H
#define COIN_SCXMLSTATE_H

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

#include <Inventor/scxml/ScXMLStateBase.h>

#include <vector>

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

class COIN_DLL_API ScXMLState : public ScXMLStateBase {
  typedef ScXMLStateBase inherited;
  SCXML_OBJECT_HEADER(ScXMLState);

public:
  static void initClass(void);

  ScXMLState(void);
  virtual ~ScXMLState(void);

  // pre-validation states
  void setIsParallel(SbBool parallel);
  SbBool isParallel(void) const;

  // XML attributes
  virtual void setSrcXMLAttr(const char * src);
  const char * getSrcXMLAttr(void) const { return this->src; }
  
  virtual void setTaskXMLAttr(const char * task);
  const char * getTaskXMLAttr(void) const { return this->task; }

  virtual SbBool handleXMLAttributes(void);

  //
  virtual void setOnEntry(const ScXMLOnEntry * onentry);
  virtual const ScXMLOnEntry * getOnEntry(void) const;

  virtual void setOnExit(const ScXMLOnExit * onexit);
  virtual const ScXMLOnExit * getOnExit(void) const;

  virtual int getNumTransitions(void) const;
  virtual const ScXMLTransition * getTransition(int idx) const;
  virtual void addTransition(const ScXMLTransition * transition);
  virtual void removeTransition(const ScXMLTransition * transition);
  virtual void clearAllTransitions(void);

  virtual void setInitial(const ScXMLInitial * initial);
  virtual const ScXMLInitial * getInitial(void) const;

  virtual int getNumStates(void) const;
  virtual const ScXMLState * getState(int idx) const;
  virtual void addState(const ScXMLState * state);
  virtual void removeState(const ScXMLState * state);
  virtual void clearAllStates(void);

  virtual int getNumParallels(void) const;
  virtual const ScXMLState * getParallel(int idx) const;
  virtual void addParallel(const ScXMLState * state);
  virtual void removeParallel(const ScXMLState * state);
  virtual void clearAllParallels(void);

  virtual int getNumFinals(void) const;
  virtual const ScXMLFinal * getFinal(int idx) const;
  virtual void addFinal(const ScXMLFinal * state);
  virtual void removeFinal(const ScXMLFinal * state);
  virtual void clearAllFinals(void);

  virtual int getNumHistories(void) const;
  virtual const ScXMLHistory * getHistory(int idx) const;
  virtual void addHistory(const ScXMLHistory * history);
  virtual void removeHistory(const ScXMLHistory * history);
  virtual void clearAllHistories(void);

  virtual int getNumAnchors(void) const;
  virtual const ScXMLAnchor * getAnchor(int idx) const;
  virtual void addAnchor(const ScXMLAnchor * anchor);
  virtual void removeAnchor(const ScXMLAnchor * anchor);
  virtual void clearAllAnchors(void);

  // datamodel

  virtual void setInvoke(const ScXMLInvoke * invoke);
  virtual const ScXMLInvoke * getInvoke(void) const;

  virtual void invoke(const ScXMLStateMachine * statemachine);

  SbBool isAtomicState(void) const;

protected:
  SbBool parallel;

  const char * src;
  const char * task;

  const ScXMLOnEntry * onentryptr;
  const ScXMLOnExit * onexitptr;
  std::vector<const ScXMLTransition *> transitionlist;
  const ScXMLInitial * initialptr;
  std::vector<const ScXMLState *> statelist;
  std::vector<const ScXMLState *> parallellist;
  std::vector<const ScXMLFinal *> finallist;
  std::vector<const ScXMLHistory *> historylist;
  std::vector<const ScXMLAnchor *> anchorlist;
  // datamodel
  const ScXMLInvoke * invokeptr;

}; // ScXMLState

#endif // !COIN_SCXMLSTATE_H
