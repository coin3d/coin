/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOVRMLTRANSFORM_H
#define COIN_SOVRMLTRANSFORM_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLGroup.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>

class COIN_DLL_API SoVRMLTransform : public SoVRMLGroup
{
  typedef SoVRMLGroup inherited;
  SO_NODE_HEADER(SoVRMLTransform);

public:
  static void initClass(void);
  SoVRMLTransform(void);
  SoVRMLTransform(int children);

  SoSFVec3f translation;
  SoSFRotation rotation;
  SoSFVec3f scale;
  SoSFRotation scaleOrientation;
  SoSFVec3f center;

  void pointAt(const SbVec3f & from, const SbVec3f & to);
  void getScaleSpaceMatrix(SbMatrix & matrix, SbMatrix & inverse) const;
  void getRotationSpaceMatrix(SbMatrix & matrix, SbMatrix & inverse) const;
  void getTranslationSpaceMatrix(SbMatrix & matrix, SbMatrix & inverse) const;

  void multLeft(const SbMatrix & matrix);
  void multRight(const SbMatrix & matrix);

  void combineLeft(SoVRMLTransform * leftnode);
  void combineRight(SoVRMLTransform * rightnode);

  void setMatrix(const SbMatrix & matrix);

  void recenter(const SbVec3f & newcenter);

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);

  virtual void notify(SoNotList * list);

protected:
  virtual ~SoVRMLTransform();

private:
  void commonConstructor(void);
  void applyMatrix(SoState * state);
}; // class SoVRMLTransform

#endif // ! COIN_SOVRMLTRANSFORM_H
