#ifndef COIN_SOTRANSFORMERMANIP_H
#define COIN_SOTRANSFORMERMANIP_H

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

class SoTransformerManipP;

class COIN_DLL_API SoTransformerManip : public SoTransformManip {
  typedef SoTransformManip inherited;

  SO_NODE_HEADER(SoTransformerManip);

public:
  static void initClass(void);
  SoTransformerManip(void);

  SbBool isLocateHighlighting(void);
  void setLocateHighlighting(SbBool onoff);

  void unsquishKnobs(void);

protected:
  virtual ~SoTransformerManip(void);

private:
  SbLazyPimplPtr<SoTransformerManipP> pimpl;

  // NOT IMPLEMENTED:
  SoTransformerManip(const SoTransformerManip & rhs);
  SoTransformerManip & operator = (const SoTransformerManip & rhs);
}; // SoTransformerManip

#endif // !COIN_SOTRANSFORMERMANIP_H
