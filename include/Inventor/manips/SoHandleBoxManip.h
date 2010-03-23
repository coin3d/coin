#ifndef COIN_SOHANDLEBOXMANIP_H
#define COIN_SOHANDLEBOXMANIP_H

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

#include <Inventor/manips/SoTransformManip.h>
#include <Inventor/tools/SbLazyPimplPtr.h>

class SoHandleBoxManipP;

class COIN_DLL_API SoHandleBoxManip : public SoTransformManip {
  typedef SoTransformManip inherited;

  SO_NODE_HEADER(SoHandleBoxManip);

public:
  static void initClass(void);
  SoHandleBoxManip(void);

protected:
  virtual ~SoHandleBoxManip(void);

private:
  SbLazyPimplPtr<SoHandleBoxManipP> pimpl;

  // NOT IMPLEMENTED:
  SoHandleBoxManip(const SoHandleBoxManip & rhs);
  SoHandleBoxManip & operator = (const SoHandleBoxManip & rhs);
}; // SoHandleBoxManip

#endif // !COIN_SOHANDLEBOXMANIP_H
