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
  virtual void audioRender(SoAudioRenderAction * action);

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
