#ifndef COIN_SOINDEXEDLINESET_H
#define COIN_SOINDEXEDLINESET_H

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

#ifndef SO_END_LINE_INDEX // also defined in SoVRMLIndexedLineSet.h
#define SO_END_LINE_INDEX (-1)
#endif // !SO_END_LINE_INDEX

class SoIndexedLineSetP;

class COIN_DLL_API SoIndexedLineSet : public SoIndexedShape {
  typedef SoIndexedShape inherited;

  SO_NODE_HEADER(SoIndexedLineSet);

public:
  static void initClass(void);
  SoIndexedLineSet(void);

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoIndexedLineSet();
  virtual void notify(SoNotList * list);

private:
  virtual void generatePrimitives(SoAction * action);

  virtual SbBool generateDefaultNormals(SoState * state, SoNormalBundle * bundle);
  virtual SbBool generateDefaultNormals(SoState * state, SoNormalCache * nc);

  enum Binding {
    OVERALL = 0,
    PER_SEGMENT,
    PER_SEGMENT_INDEXED,
    PER_LINE,
    PER_LINE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED
  };

  Binding findNormalBinding(SoState * state);
  Binding findMaterialBinding(SoState * state);

  SoIndexedLineSetP * pimpl;
};

#endif // !COIN_SOINDEXEDLINESET_H
