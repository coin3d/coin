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

#ifndef COIN_SOVRMLCOLLISION_H
#define COIN_SOVRMLCOLLISION_H

#include <Inventor/VRMLnodes/SoVRMLGroup.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFTime.h>

class COIN_DLL_API SoVRMLCollision : public SoVRMLGroup
{
  typedef SoVRMLGroup inherited;
  SO_NODE_HEADER(SoVRMLCollision);

public:
  static void initClass(void);
  SoVRMLCollision(void);
  SoVRMLCollision(int numchildren);

  SoSFBool collide;
  SoSFNode proxy;
  SoSFTime collideTime;

  virtual void GLRender(SoGLRenderAction * action);

  virtual void notify(SoNotList * list);

protected:
  virtual ~SoVRMLCollision();

private:
  void commonConstructor(void);

}; // class SoVRMLCollision

#endif // ! COIN_SOVRMLCOLLISION_H
