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

#ifndef COIN_SOTRANSFORM_H
#define COIN_SOTRANSFORM_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>


class SoTransform : public SoTransformation {
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

  void pointAt(const SbVec3f & fromPoint, const SbVec3f & toPoint);
  void getScaleSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const;
  void getRotationSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const;
  void getTranslationSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const;
  void multLeft(const SbMatrix & mat);
  void multRight(const SbMatrix & mat);
  void combineLeft(SoTransformation * nodeOnRight);
  void combineRight(SoTransformation * nodeOnLeft);
  void setMatrix(const SbMatrix & mat);
  void recenter(const SbVec3f & newCenter);

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
