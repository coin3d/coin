/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
  void endShape(SoState * state, SoShape * shape,
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
