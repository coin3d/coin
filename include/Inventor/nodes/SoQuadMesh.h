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

#ifndef COIN_SOQUADMESH_H
#define COIN_SOQUADMESH_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNonIndexedShape.h>
#include <Inventor/fields/SoSFInt32.h>


class COIN_DLL_EXPORT SoQuadMesh : public SoNonIndexedShape {
  typedef SoNonIndexedShape inherited;

  SO_NODE_HEADER(SoQuadMesh);

public:
  static void initClass(void);
  SoQuadMesh(void);

  SoSFInt32 verticesPerColumn;
  SoSFInt32 verticesPerRow;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual SbBool generateDefaultNormals(SoState * state, SoNormalBundle * nb);

protected:
  virtual ~SoQuadMesh();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);

  virtual SbBool generateDefaultNormals(SoState *, SoNormalCache *);

private:
  enum Binding {
    OVERALL = 0,
    PER_ROW,
    PER_FACE,
    PER_VERTEX,
    NONE = OVERALL
  };

  Binding findMaterialBinding(SoState * const state) const;
  Binding findNormalBinding(SoState * const state) const;
};

#endif // !COIN_SOQUADMESH_H
