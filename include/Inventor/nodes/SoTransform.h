#ifndef COIN_SOTRANSFORM_H
#define COIN_SOTRANSFORM_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>

class COIN_DLL_API SoTransform : public SoTransformation {
  typedef SoTransformation inherited;

  SO_NODE_HEADER(SoTransform);

public:
  static void initClass(void);
  SoTransform(void);

  SoSFVec3f translation;
  SoSFRotation rotation;
  SoSFVec3f scaleFactor;
  SoSFRotation scaleOrientation;
  SoSFVec3f center;

  void pointAt(const SbVec3f & frompoint, const SbVec3f & topoint);
  void getScaleSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const;
  void getRotationSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const;
  void getTranslationSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const;
  void multLeft(const SbMatrix & mat);
  void multRight(const SbMatrix & mat);
  void combineLeft(SoTransformation * nodeonright);
  void combineRight(SoTransformation * nodeonleft);
  void setMatrix(const SbMatrix & mat);
  void recenter(const SbVec3f & newcenter);

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoTransform();
};

#endif // !COIN_SOTRANSFORM_H
