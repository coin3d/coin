#ifndef COIN_SCXMLMINIMUMEVALUATOR_H
#define COIN_SCXMLMINIMUMEVALUATOR_H

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

#include <Inventor/scxml/ScXMLEvaluator.h>
#include <Inventor/tools/SbLazyPimplPtr.h>

class COIN_DLL_API ScXMLMinimumEvaluator : public ScXMLEvaluator {
  typedef ScXMLEvaluator inherited;
  SCXML_OBJECT_HEADER(ScXMLMinimumEvaluator)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLMinimumEvaluator(void);
  virtual ~ScXMLMinimumEvaluator(void);

  virtual void setStateMachine(ScXMLStateMachine *);

  virtual ScXMLDataObj * evaluate(const char * expression) const;

  virtual SbBool setAtLocation(const char * location, ScXMLDataObj * obj);
  virtual ScXMLDataObj * locate(const char * location) const;

  virtual void clearTemporaryVariables(void);

private:
  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;

}; // ScXMLMinimumEvaluator


class COIN_DLL_API ScXMLMinimumExprDataObj : public ScXMLExprDataObj {
  SCXML_OBJECT_ABSTRACT_HEADER(ScXMLMinimumExprDataObj)
  typedef ScXMLExprDataObj inherited;
public:
  static void initClass(void);
  static void cleanClass(void);

}; // ScXMLMinimumDataObj


class COIN_DLL_API ScXMLInExprDataObj : public ScXMLMinimumExprDataObj {
  SCXML_OBJECT_HEADER(ScXMLMinimumExprDataObj)
  typedef ScXMLExprDataObj inherited;
public:
  static void initClass(void);
  static void cleanClass(void);

  static ScXMLDataObj * createFor(const char * stateid);

  ScXMLInExprDataObj(void);
  ScXMLInExprDataObj(const char * stateid);
  virtual ~ScXMLInExprDataObj(void);

  void setStateId(const char * stateid);
  const char * getStateId(void) const { return this->stateid; }

protected:
  char * stateid;

  virtual SbBool evaluateNow(ScXMLStateMachine * sm, ScXMLDataObj *& pointer) const;

}; // ScXMLMinimumDataObj


class COIN_DLL_API ScXMLAppendOpExprDataObj : public ScXMLExprDataObj {
  SCXML_OBJECT_HEADER(ScXMLAppendOpExprDataObj)
  typedef ScXMLExprDataObj inherited;
public:
  static void initClass(void);
  static void cleanClass(void);

  static ScXMLDataObj * createFor(ScXMLDataObj * lhs, ScXMLDataObj * rhs);

  ScXMLAppendOpExprDataObj(void);
  ScXMLAppendOpExprDataObj(ScXMLDataObj * lhs, ScXMLDataObj * rhs);
  virtual ~ScXMLAppendOpExprDataObj(void);

  void setLHS(ScXMLDataObj * lhs);
  const ScXMLDataObj * getLHS(void) const { return this->lhs; }
  void setRHS(ScXMLDataObj * rhs);
  const ScXMLDataObj * getRHS(void) const { return this->rhs; }

protected:
  virtual SbBool evaluateNow(ScXMLStateMachine * sm, ScXMLDataObj *& pointer) const;

private:
  ScXMLDataObj * lhs, * rhs;

};

#endif // !COIN_SCXMLMINIMUMEVALUATOR_H
