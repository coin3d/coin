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

#ifndef COIN_SOVRMLNORMAL_H
#define COIN_SOVRMLNORMAL_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoMFVec3f.h>

class COIN_DLL_API SoVRMLNormal : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLNormal);

public:
  static void initClass(void);
  SoVRMLNormal(void);

  SoMFVec3f vector;

  virtual void doAction( SoAction * action );
  virtual void GLRender( SoGLRenderAction * action );
  virtual void callback( SoCallbackAction * action );
  virtual void pick( SoRayPickAction * action );
  virtual void getPrimitiveCount( SoGetPrimitiveCountAction * action );

protected:
  virtual ~SoVRMLNormal();

}; // class SoVRMLNormal

#endif // ! COIN_SOVRMLNORMAL_H
