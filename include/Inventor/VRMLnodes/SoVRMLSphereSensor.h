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
