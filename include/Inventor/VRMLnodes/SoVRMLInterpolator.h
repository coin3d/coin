/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOVRMLINTERPOLATOR_H
#define COIN_SOVRMLINTERPOLATOR_H

#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/engines/SoEngineOutput.h>

class COIN_DLL_API SoVRMLInterpolator : public SoNodeEngine {
  typedef SoNodeEngine inherited;

  SO_NODEENGINE_ABSTRACT_HEADER(SoVRMLInterpolator);

public:
  SoMFFloat key;
  SoSFFloat set_fraction;

  static void initClass(void);

protected:
  
  int getKeyValueIndex(float & interp);

  SoVRMLInterpolator(void);
  virtual ~SoVRMLInterpolator();
};

#endif // ! COIN_SOVRMLINTERPOLATOR_H
