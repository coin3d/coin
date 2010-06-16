#ifndef COIN_SOTRIANGLESTRIPSET_H
#define COIN_SOTRIANGLESTRIPSET_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNonIndexedShape.h>
#include <Inventor/fields/SoMFInt32.h>

class COIN_DLL_API SoTriangleStripSet : public SoNonIndexedShape {
  typedef SoNonIndexedShape inherited;

  SO_NODE_HEADER(SoTriangleStripSet);

public:
  static void initClass(void);
  SoTriangleStripSet(void);

  SoMFInt32 numVertices;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual SbBool generateDefaultNormals(SoState * state, SoNormalBundle * nb);

protected:
  virtual ~SoTriangleStripSet();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);

  virtual SbBool generateDefaultNormals(SoState *, SoNormalCache *);

private:
  enum Binding {
    OVERALL = 0,
    PER_STRIP,
    PER_FACE,
    PER_VERTEX
  };

  Binding findMaterialBinding(SoState * const state) const;
  Binding findNormalBinding(SoState * const state) const;
};

#endif // !COIN_SOTRIANGLESTRIPSET_H
