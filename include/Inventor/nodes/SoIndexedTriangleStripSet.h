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

#ifndef COIN_SOINDEXEDTRIANGLESTRIPSET_H
#define COIN_SOINDEXEDTRIANGLESTRIPSET_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoIndexedShape.h>
#ifndef COIN_INTERNAL
 // For Open Inventor compatibility.
 #include <Inventor/nodes/SoVertexProperty.h>
#endif // !COIN_INTERNAL


class SoIndexedTriangleStripSet : public SoIndexedShape {
  typedef SoIndexedShape inherited;

  SO_NODE_HEADER(SoIndexedTriangleStripSet);

public:
  static void initClass(void);
  SoIndexedTriangleStripSet(void);

  enum Binding {
    OVERALL = 0,
    PER_STRIP,
    PER_STRIP_INDEXED,
    PER_TRIANGLE,
    PER_TRIANGLE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED
  };

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual SbBool generateDefaultNormals(SoState * state, SoNormalBundle * nb);

protected:
  virtual ~SoIndexedTriangleStripSet();

  virtual SbBool generateDefaultNormals(SoState * state, SoNormalCache * nc);
  virtual void generatePrimitives(SoAction * action);

private:
  void countPrimitives(int & strips, int & tris);
  Binding findMaterialBinding(SoState * const state) const;
  Binding findNormalBinding(SoState * const state) const;
};

#endif // !COIN_SOINDEXEDTRIANGLESTRIPSET_H
