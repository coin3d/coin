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

/*!
  \class SoVRMLScalarInterpolator SoVRMLScalarInterpolator.h Inventor/VRMLnodes/SoVRMLScalarInterpolator.h
  \brief The SoVRMLScalarInterpolator class is used to interpolate scalar values.
*/

/*!
  \var SoMFFloat SoVRMLScalarInterpolator::keyValue
  The keyValue vector.
*/

/*!
  \var SoEngineOutput SoVRMLScalarInterpolator::value_changed
  The eventOut which is sent every time the interpolator has calculated a new value.
*/

#include <Inventor/VRMLnodes/SoVRMLScalarInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/engines/SoSubNodeEngineP.h>

SO_NODEENGINE_SOURCE(SoVRMLScalarInterpolator);

// Doc in parent
void
SoVRMLScalarInterpolator::initClass(void) // static
{
  SO_NODEENGINE_INTERNAL_INIT_CLASS(SoVRMLScalarInterpolator);
}

/*!
  Constructor.
*/
SoVRMLScalarInterpolator::SoVRMLScalarInterpolator(void)
{
  SO_NODEENGINE_INTERNAL_CONSTRUCTOR(SoVRMLScalarInterpolator);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(keyValue);
  SO_NODEENGINE_ADD_OUTPUT(value_changed, SoSFFloat);
}

/*!
  Destructor.
*/
SoVRMLScalarInterpolator::~SoVRMLScalarInterpolator()
{
}

// Doc in parent
void
SoVRMLScalarInterpolator::evaluate(void)
{
  float interp;
  int idx = this->getKeyValueIndex(interp);
  if (idx < 0) return;

  const float * v = this->keyValue.getValues(0);

  float v0 = v[idx];
  if (interp > 0.0f) {
    float v1 = v[idx+1];
    v0 = v0 + (v1-v0)*interp;
  }
  SO_ENGINE_OUTPUT(value_changed, SoSFFloat, setValue(v0));
}
