#ifndef COIN_SOTABBOXMANIP_H
#define COIN_SOTABBOXMANIP_H

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

class SoTabBoxManipP;

class COIN_DLL_API SoTabBoxManip : public SoTransformManip {
  typedef SoTransformManip inherited;

  SO_NODE_HEADER(SoTabBoxManip);

public:
  static void initClass(void);
  SoTabBoxManip(void);

protected:
  virtual ~SoTabBoxManip(void);

private:
  SbLazyPimplPtr<SoTabBoxManipP> pimpl;

  // NOT IMPLEMENTED:
  SoTabBoxManip(const SoTabBoxManip & rhs);
  SoTabBoxManip & operator = (const SoTabBoxManip & rhs);
}; // SoTabBoxManip

#endif // !COIN_SOTABBOXMANIP_H
