/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOVRMLCOORDINATEINTERPOLATOR_H
#define COIN_SOVRMLCOORDINATEINTERPOLATOR_H

#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFVec3f.h>

class COIN_DLL_API SoVRMLCoordinateInterpolator : public SoVRMLInterpolator
{
  typedef SoVRMLInterpolator inherited;
  SO_NODEENGINE_HEADER(SoVRMLCoordinateInterpolator);

public:
  static void initClass(void);
  SoVRMLCoordinateInterpolator(void);

  SoMFVec3f keyValue;
  SoEngineOutput value_changed; // (SoMFVec3f)

protected:
  virtual ~SoVRMLCoordinateInterpolator();
private:
  virtual void evaluate(void);
  class SoVRMLCoordinateInterpolatorP * pimpl;

}; // class SoVRMLCoordinateInterpolator

#endif // ! COIN_SOVRMLCOORDINATEINTERPOLATOR_H
