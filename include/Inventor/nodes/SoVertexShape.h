#ifndef COIN_SOVERTEXSHAPE_H
#define COIN_SOVERTEXSHAPE_H

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
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoSFNode.h>
#ifndef COIN_INTERNAL
// For Open Inventor compatibility.
#include <Inventor/nodes/SoVertexProperty.h>
#endif // !COIN_INTERNAL

class SoNormalBundle;
class SoNormalCache;
class SbVec3f;
class SoTextureCoordinateElement;
class SoCoordinateElement;
class SoVertexShapeP;

class COIN_DLL_API SoVertexShape : public SoShape {
  typedef SoShape inherited;

  SO_NODE_ABSTRACT_HEADER(SoVertexShape);

public:
  static void initClass(void);

  SoSFNode vertexProperty;

  virtual void notify(SoNotList * nl);
  virtual SbBool generateDefaultNormals(SoState * state,
                                        SoNormalBundle * bundle);
  virtual SbBool generateDefaultNormals(SoState * state,
                                        SoNormalCache * cache);
  virtual void write(SoWriteAction * action);

protected:
  SoVertexShape(void);
  virtual ~SoVertexShape();

  virtual SbBool shouldGLRender(SoGLRenderAction * action);

  void setNormalCache(SoState * const state,
                      const int num, const SbVec3f * normals);
  SoNormalCache * getNormalCache(void) const;

  SoNormalCache * generateAndReadLockNormalCache(SoState * const state);
  void getVertexData(SoState * state,
                     const SoCoordinateElement *& coords,
                     const SbVec3f *& normals,
                     const SbBool neednormals);

  void readLockNormalCache(void);
  void readUnlockNormalCache(void);

private:
  void writeLockNormalCache(void);
  void writeUnlockNormalCache(void);
  SoVertexShapeP * pimpl;
};

#endif // !COIN_SOVERTEXSHAPE_H
