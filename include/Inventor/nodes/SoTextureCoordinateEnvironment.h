#ifndef COIN_SOTEXTURECOORDINATEENVIRONMENT_H
#define COIN_SOTEXTURECOORDINATEENVIRONMENT_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTextureCoordinateFunction.h>
#include <Inventor/SbVec4f.h>

class COIN_DLL_API SoTextureCoordinateEnvironment : public SoTextureCoordinateFunction {
  typedef SoTextureCoordinateFunction inherited;

  SO_NODE_HEADER(SoTextureCoordinateEnvironment);

public:
  static void initClass(void);
  SoTextureCoordinateEnvironment(void);

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void pick(SoPickAction * action);
  virtual void callback(SoCallbackAction * action);

protected:
  virtual ~SoTextureCoordinateEnvironment();

private:
  static const SbVec4f &generate(void * userdata,
                                 const SbVec3f & p,
                                 const SbVec3f & n);
  static void handleTexgen(void *data);
};

#endif // !COIN_SOTEXTURECOORDINATEENVIRONMENT_H
