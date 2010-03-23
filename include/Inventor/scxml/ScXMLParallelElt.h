#ifndef COIN_SCXMLPARALLELELT_H
#define COIN_SCXMLPARALLELELT_H

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

#include <Inventor/scxml/ScXMLAbstractStateElt.h>
#include <Inventor/tools/SbPimplPtr.h>

class ScXMLEvent;
class ScXMLStateMachine;
class ScXMLOnEntryElt;
class ScXMLOnExitElt;
class ScXMLTransitionElt;
class ScXMLStateElt;
class ScXMLInitialElt;
class ScXMLFinalElt;
class ScXMLHistoryElt;
class ScXMLAnchorElt;
class ScXMLDataModelElt;

class COIN_DLL_API ScXMLParallelElt : public ScXMLAbstractStateElt {
  typedef ScXMLAbstractStateElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLParallelElt(void);
  virtual ~ScXMLParallelElt(void);

  // XML attributes
  virtual void setSrcAttribute(const char * src);
  const char * getSrcAttribute(void) const { return this->src; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  // children...
  virtual void setOnEntry(ScXMLOnEntryElt * onentry);
  virtual ScXMLOnEntryElt * getOnEntry(void) const;

  virtual void setOnExit(ScXMLOnExitElt * onexit);
  virtual ScXMLOnExitElt * getOnExit(void) const;

  virtual int getNumTransitions(void) const;
  virtual ScXMLTransitionElt * getTransition(int idx) const;
  virtual void addTransition(ScXMLTransitionElt * transition);
  virtual void removeTransition(ScXMLTransitionElt * transition);
  virtual void clearAllTransitions(void);

  virtual void setInitial(ScXMLInitialElt * initial);
  virtual ScXMLInitialElt * getInitial(void) const;

  virtual int getNumStates(void) const;
  virtual ScXMLStateElt * getState(int idx) const;
  virtual void addState(ScXMLStateElt * state);
  virtual void removeState(ScXMLStateElt * state);
  virtual void clearAllStates(void);

  virtual int getNumParallels(void) const;
  virtual ScXMLParallelElt * getParallel(int idx) const;
  virtual void addParallel(ScXMLParallelElt * state);
  virtual void removeParallel(ScXMLParallelElt * state);
  virtual void clearAllParallels(void);

  virtual int getNumFinals(void) const;
  virtual ScXMLFinalElt * getFinal(int idx) const;
  virtual void addFinal(ScXMLFinalElt * state);
  virtual void removeFinal(ScXMLFinalElt * state);
  virtual void clearAllFinals(void);

  virtual int getNumHistories(void) const;
  virtual ScXMLHistoryElt * getHistory(int idx) const;
  virtual void addHistory(ScXMLHistoryElt * history);
  virtual void removeHistory(ScXMLHistoryElt * history);
  virtual void clearAllHistories(void);

  virtual int getNumAnchors(void) const;
  virtual ScXMLAnchorElt * getAnchor(int idx) const;
  virtual void addAnchor(ScXMLAnchorElt * anchor);
  virtual void removeAnchor(ScXMLAnchorElt * anchor);
  virtual void clearAllAnchors(void);

  virtual void setDataModel(ScXMLDataModelElt * datamodel);
  virtual ScXMLDataModelElt * getDataModel(void) const;

  SbBool isAtomicState(void) const;

protected:
  char * src;

private:
  ScXMLParallelElt(const ScXMLParallelElt & rhs); // N/A
  ScXMLParallelElt & operator = (const ScXMLParallelElt & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLParallelElt

#endif // !COIN_SCXMLPARALLELELT_H
