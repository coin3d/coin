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

#ifndef COIN_SOVRMLSPHERESENSOR_H
#define COIN_SOVRMLSPHERESENSOR_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLDragSensor.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>

class SbSphereProjector;

class COIN_DLL_API SoVRMLSphereSensor : public SoVRMLDragSensor
{
  typedef SoVRMLDragSensor inherited;
  SO_NODE_HEADER(SoVRMLSphereSensor);

public:
  static void initClass(void);
  SoVRMLSphereSensor(void);

  SoSFRotation offset;
  SoSFRotation rotation_changed;

protected:

  virtual SbBool dragStart(void);
  virtual void drag(void);
  virtual void dragFinish(void);

  virtual ~SoVRMLSphereSensor();

private:

  SbSphereProjector * sphereproj;
  SbVec3f prevworldhitpt;
  SbRotation prevrotation;
};

#endif // ! COIN_SOVRMLSPHERESENSOR_H
