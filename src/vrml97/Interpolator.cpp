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

#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/engines/SoSubNodeEngineP.h>

SO_NODEENGINE_ABSTRACT_SOURCE(SoVRMLInterpolator);

void
SoVRMLInterpolator::initClass(void) // static
{
  SO_NODEENGINE_INIT_ABSTRACT_CLASS(SoVRMLInterpolator, SoNodeEngine, "NodeEngine");
}

SoVRMLInterpolator::SoVRMLInterpolator(void) // protected
{
  SO_NODEENGINE_CONSTRUCTOR(SoVRMLInterpolator);

  SO_VRMLNODE_ADD_EVENT_IN(set_fraction);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(key);
}

SoVRMLInterpolator::~SoVRMLInterpolator() // virtual, protected
{
}

int 
SoVRMLInterpolator::getKeyValueIndex(float & interp)
{
  float fraction = this->set_fraction.getValue();
  const int n = this->key.getNum();
  if (n == 0) return -1;

  const float * t = this->key.getValues(0); 
  for (int i = 0; i < n; i++) {
    if (fraction < t[i]) {
      if (i == 0) {
        interp = 0.0f;
        return 0;
      }
      else {
        float delta = t[i] - t[i-1];
        if (delta > 0.0f) {
          interp = (fraction - t[i-1]) / delta;
        }
        else interp = 0.0f;
      }
      return i-1;
    }
  }
  interp = 0.0f;
  return n-1;
}
