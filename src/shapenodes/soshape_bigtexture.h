#ifndef COIN_SOSHAPE_BIGTEXTURE_H
#define COIN_SOSHAPE_BIGTEXTURE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <Inventor/lists/SbList.h>
#include <Inventor/elements/SoMultiTextureImageElement.h>
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

  void beginShape(SoGLBigImage * image,
                  const float quality);
  void triangle(SoState * state,
                const SoPrimitiveVertex * v1,
                const SoPrimitiveVertex * v2,
                const SoPrimitiveVertex * v3);
  SbBool endShape(SoState * state, SoShape * shape,
                  SoMaterialBundle & mb);


private:
  void clip_triangles(SoState * state);
  void handle_triangle(SoState * state,
                       SoPrimitiveVertex * v1,
                       SoPrimitiveVertex * v2,
                       SoPrimitiveVertex * v3,
                       const SoMultiTextureImageElement::Wrap wrap[2],
                       const int transs, 
                       const int transt);
  
  SbList <SoPrimitiveVertex*> vertexlist;
  
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
