#ifndef COIN_SOINDEXEDPOINTSET_H
#define COIN_SOINDEXEDPOINTSET_H

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
#include <Inventor/nodes/SoIndexedShape.h>

class SoVertexArrayIndexer;
class SoGLCoordinateElement;
class SoTextureCoordinateBundle;

class COIN_DLL_API SoIndexedPointSet : public SoIndexedShape {
  typedef SoIndexedShape inherited;

  SO_NODE_HEADER(SoIndexedPointSet);

public:
  static void initClass(void);
  SoIndexedPointSet(void);

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual void notify(SoNotList * list);

protected:
  virtual ~SoIndexedPointSet();

  virtual void generatePrimitives(SoAction * action);

  virtual SbBool generateDefaultNormals(SoState *, SoNormalCache * nc);
  virtual SbBool generateDefaultNormals(SoState * state,
                                        SoNormalBundle * bundle);
  SoVertexArrayIndexer * vaindexer;

  enum Binding {
    OVERALL,
    PER_VERTEX,
    PER_VERTEX_INDEXED
  };

  Binding findNormalBinding(SoState * const state) const;
  Binding findMaterialBinding(SoState * const state) const;
  Binding findTextureBinding(SoState * const state) const;

};

#endif // !COIN_SOINDEXEDPOINTSET_H
