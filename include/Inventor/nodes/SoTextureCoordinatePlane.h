/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SOTEXTURECOORDINATEPLANE_H
#define COIN_SOTEXTURECOORDINATEPLANE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTextureCoordinateFunction.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbVec3f.h>

class SoTextureCoordinatePlane : public SoTextureCoordinateFunction {
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
