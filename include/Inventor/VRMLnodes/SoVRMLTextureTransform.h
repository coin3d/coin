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

#ifndef COIN_SOVRMLTEXTURETRANSFORM_H
#define COIN_SOVRMLTEXTURETRANSFORM_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFFloat.h>

class SbMatrix;

class COIN_DLL_API SoVRMLTextureTransform : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLTextureTransform);

public:
  static void initClass(void);
  SoVRMLTextureTransform(void);

  SoSFVec2f translation;
  SoSFFloat rotation;
  SoSFVec2f scale;
  SoSFVec2f center;

  virtual void doAction( SoAction * action );
  virtual void callback( SoCallbackAction * action );
  virtual void GLRender( SoGLRenderAction * action );
  virtual void getMatrix( SoGetMatrixAction * action );
  virtual void pick( SoPickAction * action );

protected:
  virtual ~SoVRMLTextureTransform();

private:
  void makeMatrix(SbMatrix & mat) const;

}; // class SoVRMLTextureTransform

#endif // ! COIN_SOVRMLTEXTURETRANSFORM_H
