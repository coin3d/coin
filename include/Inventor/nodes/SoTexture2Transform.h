/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOTEXTURE2TRANSFORM_H
#define COIN_SOTEXTURE2TRANSFORM_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFFloat.h>

class SbMatrix;


class COIN_DLL_API SoTexture2Transform : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoTexture2Transform);

public:
  static void initClass(void);
  SoTexture2Transform(void);

  SoSFVec2f translation;
  SoSFFloat rotation;
  SoSFVec2f scaleFactor;
  SoSFVec2f center;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void pick(SoPickAction * action);

protected:
  virtual ~SoTexture2Transform();

private:
  void makeMatrix(SbMatrix & mat);
};



#endif // !COIN_SOTEXTURE2TRANSFORM_H
