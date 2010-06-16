#ifndef COIN_SCXMLSCRIPTELT_H
#define COIN_SCXMLSCRIPTELT_H

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

#include <Inventor/tools/SbLazyPimplPtr.h>

class COIN_DLL_API ScXMLScriptElt : public ScXMLExecutableElt {
  typedef ScXMLExecutableElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLScriptElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLScriptElt(void);
  virtual ~ScXMLScriptElt(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual void execute(ScXMLStateMachine * statemachine) const;

private:
  ScXMLScriptElt(const ScXMLScriptElt & rhs); // N/A
  ScXMLScriptElt & operator = (const ScXMLScriptElt & rhs); // N/A

  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;
}; // ScXMLScriptElt

#endif // !COIN_SCXMLSCRIPTELT_H

