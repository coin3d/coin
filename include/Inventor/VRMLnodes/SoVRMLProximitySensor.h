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

#ifndef COIN_SOVRMLPROXIMITYSENSOR_H
#define COIN_SOVRMLPROXIMITYSENSOR_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFRotation.h>

class COIN_DLL_API SoVRMLProximitySensor : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLProximitySensor);

public:
  static void initClass(void);
  SoVRMLProximitySensor(void);

  SoSFVec3f center;
  SoSFVec3f size;
  SoSFBool enabled;

  SoSFBool isActive;
  SoSFVec3f position_changed;
  SoSFRotation orientation_changed;
  SoSFTime enterTime;
  SoSFTime exitTime;

  virtual SbBool affectsState(void) const;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);

protected:
  virtual ~SoVRMLProximitySensor();

  virtual void notify(SoNotList * list);
  virtual void updateState(SbVec3f & viewerpos, SbVec3f & viewerorientation,
                            SbBool inside);
}; // class SoVRMLProximitySensor

#endif // ! COIN_SOVRMLPROXIMITYSENSOR_H
