#ifndef COIN_SOINDEXEDFACESET_H
#define COIN_SOINDEXEDFACESET_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoIndexedShape.h>

#define SO_END_FACE_INDEX (-1)

class SoIndexedFaceSetP;

class COIN_DLL_API SoIndexedFaceSet : public SoIndexedShape {
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

  SbBool useConvexCache(SoAction * action,
                        const SbVec3f * normals,
                        const int32_t * nindices,
                        const SbBool normalsfromcache);
  Binding findMaterialBinding(SoState * const state) const;
  Binding findNormalBinding(SoState * const state) const;
  virtual void notify(SoNotList * list);

  SoIndexedFaceSetP * pimpl;
};

#endif // !COIN_SOINDEXEDFACESET_H
