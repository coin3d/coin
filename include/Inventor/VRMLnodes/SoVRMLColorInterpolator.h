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

#ifndef COIN_SOVRMLCOLORINTERPOLATOR_H
#define COIN_SOVRMLCOLORINTERPOLATOR_H

#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFFloat.h>

class COIN_DLL_API SoVRMLColorInterpolator : public SoVRMLInterpolator
{
  typedef SoVRMLInterpolator inherited;
  SO_NODEENGINE_HEADER(SoVRMLColorInterpolator);

public:
  static void initClass(void);
  SoVRMLColorInterpolator(void);

  SoMFColor keyValue;
  SoEngineOutput value_changed; // (SoSFColor)

protected:
  virtual ~SoVRMLColorInterpolator();

private:
  virtual void evaluate(void);
}; // class SoVRMLColorInterpolator

#endif // ! COIN_SOVRMLCOLORINTERPOLATOR_H
