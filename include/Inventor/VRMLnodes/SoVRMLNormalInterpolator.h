/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOVRMLNORMALINTERPOLATOR_H
#define COIN_SOVRMLNORMALINTERPOLATOR_H

#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFVec3f.h>

class COIN_DLL_API SoVRMLNormalInterpolator : public SoVRMLInterpolator
{
  typedef SoVRMLInterpolator inherited;
  SO_NODEENGINE_HEADER(SoVRMLNormalInterpolator);

public:
  static void initClass(void);
  SoVRMLNormalInterpolator(void);

  SoMFVec3f keyValue;
  SoEngineOutput value_changed; // (SoMFVec3f)

protected:
  virtual ~SoVRMLNormalInterpolator();
private:
  virtual void evaluate(void);

  class SoVRMLNormalInterpolatorP * pimpl;
};

#endif // ! COIN_SOVRMLNORMALINTERPOLATOR_H
