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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSURROUNDSCALE_H
#define COIN_SOSURROUNDSCALE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/SbVec3f.h>

class SbMatrix;


class COIN_DLL_EXPORT SoSurroundScale : public SoTransformation {
  typedef SoTransformation inherited;

  SO_NODE_HEADER(SoSurroundScale);

public:
  static void initClass(void);
  SoSurroundScale(void);

  SoSFInt32 numNodesUpToContainer;
  SoSFInt32 numNodesUpToReset;

  void invalidate(void);
  void doAction(SoAction * action);
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
