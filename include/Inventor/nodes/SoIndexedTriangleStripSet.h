/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOINDEXEDTRIANGLESTRIPSET_H
#define COIN_SOINDEXEDTRIANGLESTRIPSET_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoIndexedShape.h>
#ifndef COIN_INTERNAL
 // For Open Inventor compatibility.
 #include <Inventor/nodes/SoVertexProperty.h>
#endif // !COIN_INTERNAL


class COIN_DLL_EXPORT SoIndexedTriangleStripSet : public SoIndexedShape {
  typedef SoIndexedShape inherited;

  SO_NODE_HEADER(SoIndexedTriangleStripSet);

public:
  static void initClass(void);
  SoIndexedTriangleStripSet(void);

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual SbBool generateDefaultNormals(SoState * state, SoNormalBundle * nb);

protected:
  virtual ~SoIndexedTriangleStripSet();

  virtual SbBool willSetShadeModel() const;
  virtual SbBool generateDefaultNormals(SoState * state, SoNormalCache * nc);
  virtual void generatePrimitives(SoAction * action);

private:
  enum Binding {
    OVERALL = 0,
    PER_STRIP,
    PER_STRIP_INDEXED,
    PER_TRIANGLE,
    PER_TRIANGLE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED
  };

  void countPrimitives(int & strips, int & tris);
  Binding findMaterialBinding(SoState * const state) const;
  Binding findNormalBinding(SoState * const state) const;
};

#endif // !COIN_SOINDEXEDTRIANGLESTRIPSET_H
