#ifndef COIN_SCXMLFINALELT_H
#define COIN_SCXMLFINALELT_H

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

class ScXMLOnEntryElt;
class ScXMLOnExitElt;

class COIN_DLL_API ScXMLFinalElt : public ScXMLAbstractStateElt {
  typedef ScXMLAbstractStateElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLFinalElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLFinalElt(void);
  virtual ~ScXMLFinalElt(void);

  virtual void setOnEntry(ScXMLOnEntryElt * onentry);
  virtual ScXMLOnEntryElt * getOnEntry(void) const;

  virtual void setOnExit(ScXMLOnExitElt * onexit);
  virtual ScXMLOnExitElt * getOnExit(void) const;

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

private:
  ScXMLFinalElt(const ScXMLFinalElt & rhs); // N/A
  ScXMLFinalElt & operator = (const ScXMLFinalElt & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLFinalElt

#endif // !COIN_SCXMLFINALELT_H
