#ifndef COIN_SOSURROUNDSCALE_H
#define COIN_SOSURROUNDSCALE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/SbVec3f.h>

class SbMatrix;

class COIN_DLL_API SoSurroundScale : public SoTransformation {
  typedef SoTransformation inherited;

  SO_NODE_HEADER(SoSurroundScale);

public:
  static void initClass(void);
  SoSurroundScale(void);

  SoSFInt32 numNodesUpToContainer;
  SoSFInt32 numNodesUpToReset;

  void invalidate(void);
  virtual void doAction(SoAction * action);
  void setDoingTranslations(const SbBool val);
  SbBool isDoingTranslations(void);

protected:
  virtual ~SoSurroundScale();

  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void pick(SoPickAction * action);
  void updateMySurroundParams(SoAction * action, const SbMatrix & inv);
  void setIgnoreInBbox(const SbBool val);
  SbBool isIgnoreInBbox(void);

protected:
  SbVec3f cachedScale;
  SbVec3f cachedInvScale;
  SbVec3f cachedTranslation;
  SbBool cacheOK;
  SbBool doTranslations;

private:
  SbBool ignoreinbbox;
};

#endif // !COIN_SOSURROUNDSCALE_H
