#ifndef COIN_SCXMLCONTENTELT_H
#define COIN_SCXMLCONTENTELT_H

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

#include <Inventor/scxml/ScXMLElt.h>

#include <Inventor/tools/SbLazyPimplPtr.h>

class COIN_DLL_API ScXMLContentElt : public ScXMLElt {
  typedef ScXMLElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLContentElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLContentElt(void);
  virtual ~ScXMLContentElt(void);

  virtual void copyContents(const ScXMLElt * rhs);

private:
  ScXMLContentElt(const ScXMLContentElt & rhs); // N/A
  ScXMLContentElt & operator = (const ScXMLContentElt & rhs); // N/A

  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;
}; // ScXMLContentElt

#endif // !COIN_SCXMLCONTENTELT_H

