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

#ifndef COIN_SOVRMLVISIBILITYSENSOR_H
#define COIN_SOVRMLVISIBILITYSENSOR_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>

class COIN_DLL_API SoVRMLVisibilitySensor : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLVisibilitySensor);

public:
  static void initClass(void);
  SoVRMLVisibilitySensor(void);

  SoSFVec3f center;
  SoSFVec3f size;
  SoSFBool enabled;

  virtual void GLRender( SoGLRenderAction * action );

protected:
  virtual ~SoVRMLVisibilitySensor();

  SoSFTime enterTime;
  SoSFTime exitTime;
  SoSFBool isActive;
};

#endif // ! COIN_SOVRMLVISIBILITYSENSOR_H
