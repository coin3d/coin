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

#ifndef COIN_SOTRANSFORMVEC3F_H
#define COIN_SOTRANSFORMVEC3F_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFMatrix.h>

class COIN_DLL_API SoTransformVec3f : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoTransformVec3f);

public:
  static void initClass(void);
  SoTransformVec3f(void);

  SoMFVec3f vector;
  SoMFMatrix matrix;

  SoEngineOutput point;           // SoMFVec3f
  SoEngineOutput direction;       // SoMFVec3f
  SoEngineOutput normalDirection; // SoMFVec3f

protected:
  virtual ~SoTransformVec3f();

private:
  virtual void evaluate(void);
};

#endif // !COIN_SOTRANSFORMVEC3F_H
