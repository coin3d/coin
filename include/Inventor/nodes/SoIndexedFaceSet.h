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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOINDEXEDFACESET_H
#define COIN_SOINDEXEDFACESET_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoIndexedShape.h>

#define SO_END_FACE_INDEX (-1)


class SoConvexDataCache;

class COIN_DLL_EXPORT SoIndexedFaceSet : public SoIndexedShape {
  typedef SoIndexedShape inherited;

  SO_NODE_HEADER(SoIndexedFaceSet);

public:
  static void initClass(void);
  SoIndexedFaceSet(void);

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual SbBool generateDefaultNormals(SoState * state,
                                        SoNormalBundle * bundle);
  virtual SbBool generateDefaultNormals(SoState * state,
                                        SoNormalCache * cache);


protected:
  virtual ~SoIndexedFaceSet();

  virtual void generatePrimitives(SoAction * action);

private:
  enum Binding {
    OVERALL = 0,
    PER_FACE,
    PER_FACE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED,
    NONE = OVERALL
  };

  SbBool useConvexCache(SoAction * action);
  Binding findMaterialBinding(SoState * const state) const;
  Binding findNormalBinding(SoState * const state) const;
  virtual void notify(SoNotList * list);
  SoConvexDataCache * convexCache;
  int concavestatus;
};

#endif // !COIN_SOINDEXEDFACESET_H
