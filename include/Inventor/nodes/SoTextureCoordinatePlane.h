#ifndef COIN_SOTEXTURECOORDINATEPLANE_H
#define COIN_SOTEXTURECOORDINATEPLANE_H

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

  // FIXME: this is to avoid a complaint from Doxygen, which has a bug
  // that makes it spit out a warning on undocumented *private*
  // structs and classes. Bug has been reported to
  // <doxygen-develop@lists.sourceforge.net> at 2002-07-30 by mortene.
#ifndef DOXYGEN_SKIP_THIS
  // Note: Microsoft Visual C++ 6.0 needs to have a type definition
  // and an explicit variable declaration, just using
  // "struct { ... } gencache;" won't do.
  typedef struct {
    SbVec3f s, t;
    float mul_s, mul_t;
    SbVec4f ret;
  } gencache_t;
#endif // !DOXYGEN_SKIP_THIS

  gencache_t gencache;
};

#endif // !COIN_SOTEXTURECOORDINATEPLANE_H
