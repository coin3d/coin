/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_SODATASENSOR_H
#define COIN_SODATASENSOR_H

#include <Inventor/sensors/SoDelayQueueSensor.h>
#include <stdlib.h> // for NULL definition

class SoNode;
class SoField;
class SoPath;
class SoNotList;


class COIN_DLL_API SoDataSensor : public SoDelayQueueSensor {
  typedef SoDelayQueueSensor inherited;

public:
  SoDataSensor(void);
  SoDataSensor(SoSensorCB * func, void * data);
  virtual ~SoDataSensor(void);

  void setDeleteCallback(SoSensorCB * function, void * data = NULL);
  SoNode * getTriggerNode(void) const;
  SoField * getTriggerField(void) const;
  SoPath * getTriggerPath(void) const;
  void setTriggerPathFlag(SbBool flag);
  SbBool getTriggerPathFlag(void) const;

  virtual void trigger(void);
  virtual void notify(SoNotList * l);
  virtual void dyingReference(void) = 0;

protected:
  void invokeDeleteCallback(void);

private:
  SoSensorCB * cbfunc;
  void * cbdata;
  SbBool findpath;
  SoField * triggerfield;
  SoNode * triggernode;
  SoPath * triggerpath;
};

#endif // !COIN_SODATASENSOR_H
