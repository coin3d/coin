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

#ifndef COIN_SOVRMLTOUCHSENSOR_H
#define COIN_SOVRMLTOUCHSENSOR_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFTime.h>

class COIN_DLL_API SoVRMLTouchSensor : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLTouchSensor);

public:
  static void initClass(void);
  SoVRMLTouchSensor(void);

  SoSFBool enabled;
  SoSFVec3f hitNormal_changed;
  SoSFVec3f hitPoint_changed;
  SoSFVec2f hitTexCoord_changed;
  SoSFBool isActive;
  SoSFBool isOver;
  SoSFTime touchTime;

  virtual SbBool affectsState(void) const;
  virtual void handleEvent(SoHandleEventAction * action);

protected:
  virtual ~SoVRMLTouchSensor();

  virtual void notify(SoNotList * list);

private:
  SbBool isactive;
}; // class SoVRMLTouchSensor

#endif // ! COIN_SOVRMLTOUCHSENSOR_H
