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

#ifndef COIN_SOSHAPE_BIGTEXTURE_H
#define COIN_SOSHAPE_BIGTEXTURE_H

#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbPlane.h>

class SoGLBigImage;
class SoState;
class SoPrimitiveVertex;
class SoMaterialBundle;
class SoShape;
class SbClip;
class SbVec3f;

// FIXME: this class will also be part of the global namespace of
// the compiled Coin library (at least when compiled to a UNIX-style
// library). That should be avoided. 20020220 mortene.

class soshape_bigtexture {
public:
  soshape_bigtexture(void);
  ~soshape_bigtexture();

  void beginShape(SoState * state,
                  SoGLBigImage * image,
                  const float quality);
  void triangle(SoState * state,
                const SoPrimitiveVertex * v1,
                const SoPrimitiveVertex * v2,
                const SoPrimitiveVertex * v3);
  SbBool endShape(SoState * state, SoShape * shape,
                  SoMaterialBundle & mb);

private:
  class bt_region {
  public:
    SbVec2f start, end, tcmul;
    SbPlane planes[4];
    SbList <SoPrimitiveVertex*> pvlist;
    SbList <int> facelist;
  };

  SbList <SoPrimitiveVertex*> * pvlist;
  int pvlistcnt;
  SbClip * clipper;
  SoGLBigImage * image;
  float quality;
  bt_region * regions;
  int numallocregions;
  int numregions;

  SoPrimitiveVertex * get_new_pv(void);

  static void * clipcb(const SbVec3f & v0, void * vdata0,
                       const SbVec3f & v1, void * vdata1,
                       const SbVec3f & newvertex,
                       void * userdata);
};

#endif // !COIN_SOSHAPE_BIGTEXTURE_H
