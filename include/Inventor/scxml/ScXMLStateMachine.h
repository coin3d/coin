#ifndef COIN_SCXMLSTATEMACHINE_H
#define COIN_SCXMLSTATEMACHINE_H

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

#include <Inventor/scxml/ScXMLEventTarget.h>

#include <Inventor/SbName.h>
#include <Inventor/lists/SbList.h>

class ScXMLEvent;
class ScXMLDocument;
class ScXMLStateMachine;
class ScXMLEvaluator;
class ScXMLElt;

typedef void ScXMLStateMachineDeleteCB(void * userdata,
                                       ScXMLStateMachine * statemachine);
typedef void ScXMLStateChangeCB(void * userdata,
                                ScXMLStateMachine * statemachine,
                                const char * stateidentifier,
                                SbBool enterstate,
                                SbBool success);
typedef void ScXMLParallelStateChangeCB(void * userdata,
                                        ScXMLStateMachine * statemachine,
                                        int numstates,
                                        const char ** stateidentifiers,
                                        SbBool enterstate,
                                        SbBool success);

class COIN_DLL_API ScXMLStateMachine : public ScXMLEventTarget {
  typedef ScXMLEventTarget inherited;
  SCXML_OBJECT_HEADER(ScXMLStateMachine)

public:
  static void initClass(void);
  static void cleanClass(void);

 ScXMLStateMachine(void);
  virtual ~ScXMLStateMachine(void);

  virtual void setName(const SbName & name);
  const SbName & getName(void) const;

  virtual void setDescription(ScXMLDocument * document);
  const ScXMLDocument * getDescription(void) const;

  virtual void setSessionId(const SbName & sessionid);
  const SbName & getSessionId(void) const;

  virtual void initialize(void);

  virtual SbBool isActive(void) const;
  virtual SbBool isFinished(void) const;

  virtual int getNumActiveStates(void) const;
  virtual const ScXMLElt * getActiveState(int idx) const;

  virtual void addDeleteCallback(ScXMLStateMachineDeleteCB * callback,
                                 void * userdata);
  virtual void removeDeleteCallback(ScXMLStateMachineDeleteCB * callback,
                                    void * userdata);

  virtual void addStateChangeCallback(ScXMLStateChangeCB * callback,
                                      void * userdata);
  virtual void removeStateChangeCallback(ScXMLStateChangeCB * callback,
                                         void * userdata);

  virtual void setVariable(const char * name, const char * value);
  virtual const char * getVariable(const char * name) const;

  static ScXMLStateMachine * getStateMachineForSessionId(const SbName & sessionid);

  virtual void setLogLevel(int loglevel);
  int getLogLevel(void) const;

  virtual void setEvaluator(ScXMLEvaluator * evaluator);
  ScXMLEvaluator * getEvaluator(void) const;

  SbBool isModuleEnabled(const char * modulename) const;
  int getNumEnabledModules(void) const;
  const char * getEnabledModuleName(int idx) const;
  void setEnabledModulesList(const SbList<const char *> & modulenames);

protected:
  virtual SbBool processOneEvent(const ScXMLEvent * event);

private:
  ScXMLStateMachine(const ScXMLStateMachine & rhs); // N/A
  ScXMLStateMachine & operator = (const ScXMLStateMachine & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLStateMachine

#endif // !COIN_SCXMLSTATEMACHINE_H
