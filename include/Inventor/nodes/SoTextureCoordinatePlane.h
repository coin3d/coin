/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOTEXTURECOORDINATEPLANE_H
#define COIN_SOTEXTURECOORDINATEPLANE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTextureCoordinateFunction.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbVec3f.h>

class COIN_DLL_API SoTextureCoordinatePlane : public SoTextureCoordinateFunction {
  typedef SoTextureCoordinateFunction inherited;

  SO_NODE_HEADER(SoTextureCoordinatePlane);

public:
  static void initClass(void);
  SoTextureCoordinatePlane(void);

  SoSFVec3f directionS;
  SoSFVec3f directionT;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void pick(SoPickAction * action);
  virtual void callback(SoCallbackAction * action);

protected:
  virtual ~SoTextureCoordinatePlane();

private:
  static const SbVec4f &generate(void *userdata,
                                 const SbVec3f &p,
                                 const SbVec3f &n);
  static void handleTexgen(void *data);

  // Note: Microsoft Visual C++ 6.0 needs to have a type definition
  // and an explicit variable declaration, just using
  // "struct { ... } gencache;" won't do.
  typedef struct {
    SbVec3f s, t;
    float mul_s, mul_t;
    SbVec4f ret;
  } gencache_t;
  gencache_t gencache;
};

#endif // !COIN_SOTEXTURECOORDINATEPLANE_H
