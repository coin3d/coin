#ifndef COIN_SOJACKMANIP_H
#define COIN_SOJACKMANIP_H

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

class SoJackManipP;

class COIN_DLL_API SoJackManip : public SoTransformManip {
  typedef SoTransformManip inherited;

  SO_NODE_HEADER(SoJackManip);

public:
  static void initClass(void);
  SoJackManip(void);

protected:
  virtual ~SoJackManip(void);

private:
  SbLazyPimplPtr<SoJackManipP> pimpl;

  // NOT IMPLEMENTED:
  SoJackManip(const SoJackManip & rhs);
  SoJackManip & operator = (const SoJackManip & rhs);
}; // SoJackManip

#endif // !COIN_SOJACKMANIP_H
