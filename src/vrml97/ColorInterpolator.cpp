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
  \class SoVRMLColorInterpolator SoVRMLColorInterpolator.h Inventor/VRMLnodes/SoVRMLColorInterpolator.h
  \brief The SoVRMLColorInterpolator class is used to interpolate color values.
*/


/*!
  \var SoMFColor SoVRMLColorInterpolator::keyValue
  The color values.
*/

/*!
  \var SoEngineOutput SoVRMLColorInterpolator::value_changed
  The eventOut color.
*/

#include <Inventor/VRMLnodes/SoVRMLColorInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/engines/SoSubNodeEngineP.h>

SO_NODEENGINE_SOURCE(SoVRMLColorInterpolator);

// doc in parent
void
SoVRMLColorInterpolator::initClass(void) // static
{
  SO_NODEENGINE_INTERNAL_INIT_CLASS(SoVRMLColorInterpolator);
}

/*!
  Constructor.
*/
SoVRMLColorInterpolator::SoVRMLColorInterpolator(void)
{
  SO_NODEENGINE_INTERNAL_CONSTRUCTOR(SoVRMLColorInterpolator);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(keyValue);
  SO_NODEENGINE_ADD_OUTPUT(value_changed, SoSFColor);
}

/*!
  Destructor.
*/
SoVRMLColorInterpolator::~SoVRMLColorInterpolator() // virtual, protected
{
}

// doc in parent
void
SoVRMLColorInterpolator::evaluate(void)
{
  float interp;
  int idx = this->getKeyValueIndex(interp);
  if (idx < 0) return;

  const SbColor * v = this->keyValue.getValues(0);

  SbColor v0 = v[idx];
  if (interp > 0.0f) {
    SbColor v1 = v[idx+1];
    v0 = v0 + (v1-v0)*interp;
  }
  SO_ENGINE_OUTPUT(value_changed, SoSFColor, setValue(v0));
}
