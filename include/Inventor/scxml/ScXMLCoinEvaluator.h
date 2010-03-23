#ifndef COIN_SCXMLCOINEVALUATOR_H
#define COIN_SCXMLCOINEVALUATOR_H

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
#include <Inventor/tools/SbPimplPtr.h>

class COIN_DLL_API ScXMLCoinEvaluator : public ScXMLEvaluator {
  typedef ScXMLEvaluator inherited;
  SCXML_OBJECT_HEADER(ScXMLCoinEvaluator)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLCoinEvaluator(void);
  virtual ~ScXMLCoinEvaluator(void);

  virtual void setStateMachine(ScXMLStateMachine *sm);

  virtual ScXMLDataObj * evaluate(const char * expression) const;

  virtual SbBool setAtLocation(const char * location, ScXMLDataObj * obj);
  virtual ScXMLDataObj * locate(const char * location) const;

  virtual void clearTemporaryVariables(void);
  void dumpTemporaries(void);

private:
  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLCoinEvaluator

class COIN_DLL_API ScXMLCoinEqualsOpExprDataObj : public ScXMLEqualsOpExprDataObj {
  SCXML_OBJECT_HEADER(ScXMLCoinEqualsOpExprDataObj)
  typedef ScXMLEqualsOpExprDataObj inherited;
public:
  static void initClass(void);
  static void cleanClass(void);

protected:
  virtual SbBool evaluateNow(ScXMLStateMachine * sm, ScXMLDataObj * & pointer) const;

};

class COIN_DLL_API ScXMLCoinAddOpExprDataObj : public ScXMLAddOpExprDataObj {
  SCXML_OBJECT_HEADER(ScXMLCoinAddOpExprDataObj)
  typedef ScXMLAddOpExprDataObj inherited;
public:
  static void initClass(void);
  static void cleanClass(void);

protected:
  virtual SbBool evaluateNow(ScXMLStateMachine * sm, ScXMLDataObj * & pointer) const;

};

class COIN_DLL_API ScXMLCoinSubtractOpExprDataObj : public ScXMLSubtractOpExprDataObj {
  SCXML_OBJECT_HEADER(ScXMLCoinSubtractOpExprDataObj)
  typedef ScXMLSubtractOpExprDataObj inherited;
public:
  static void initClass(void);
  static void cleanClass(void);

  static ScXMLDataObj * createFor(ScXMLDataObj * lhs, ScXMLDataObj * rhs);

protected:
  virtual SbBool evaluateNow(ScXMLStateMachine * sm, ScXMLDataObj * & pointer) const;

};

class COIN_DLL_API ScXMLCoinMultiplyOpExprDataObj : public ScXMLMultiplyOpExprDataObj {
  SCXML_OBJECT_HEADER(ScXMLCoinMultiplyOpExprDataObj)
  typedef ScXMLMultiplyOpExprDataObj inherited;
public:
  static void initClass(void);
  static void cleanClass(void);

protected:
  virtual SbBool evaluateNow(ScXMLStateMachine * sm, ScXMLDataObj * & pointer) const;

};

class COIN_DLL_API ScXMLCoinDivideOpExprDataObj : public ScXMLDivideOpExprDataObj {
  SCXML_OBJECT_HEADER(ScXMLCoinDivideOpExprDataObj)
  typedef ScXMLDivideOpExprDataObj inherited;
public:
  static void initClass(void);
  static void cleanClass(void);

protected:
  virtual SbBool evaluateNow(ScXMLStateMachine * sm, ScXMLDataObj * & pointer) const;

};

class COIN_DLL_API ScXMLCoinLengthFuncExprDataObj : public ScXMLExprDataObj {
  SCXML_OBJECT_HEADER(ScXMLCoinLengthFuncExprDataObj)
  typedef ScXMLExprDataObj inherited;
public:
  static void initClass(void);
  static void cleanClass(void);

  static ScXMLDataObj * createFor(ScXMLDataObj * obj);

  ScXMLCoinLengthFuncExprDataObj(void);
  ScXMLCoinLengthFuncExprDataObj(ScXMLDataObj * obj);
  virtual ~ScXMLCoinLengthFuncExprDataObj(void);

  void setExpr(ScXMLDataObj * obj);
  ScXMLDataObj * getExpr(void) const { return this->expr; }

protected:
  virtual SbBool evaluateNow(ScXMLStateMachine * sm, ScXMLDataObj * & pointer) const;

  ScXMLDataObj * expr;

};

#endif // !COIN_SCXMLCOINEVALUATOR_H
