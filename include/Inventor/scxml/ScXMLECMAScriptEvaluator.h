#ifndef COIN_SCXMLECMASCRIPTEVALUATOR_H
#define COIN_SCXMLECMASCRIPTEVALUATOR_H

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

class COIN_DLL_API ScXMLECMAScriptEvaluator : public ScXMLEvaluator {
  typedef ScXMLEvaluator inherited;
  SCXML_OBJECT_HEADER(ScXMLECMAScriptEvaluator)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLECMAScriptEvaluator(void);
  virtual ~ScXMLECMAScriptEvaluator(void);

  virtual ScXMLDataObj * evaluate(const char * expression) const;

  virtual SbBool setAtLocation(const char * location, ScXMLDataObj * obj);
  virtual ScXMLDataObj * locate(const char * location) const;

private:
  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;

}; // ScXMLECMAScriptEvaluator

#endif // !COIN_SCXMLECMASCRIPTEVALUATOR_H
