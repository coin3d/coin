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

/*!
  \class SoVRMLPositionInterpolator SoVRMLPositionInterpolator.h Inventor/VRMLnodes/SoVRMLPositionInterpolator.h
  \brief The SoVRMLPositionInterpolator class is used to interpolate 3D points.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  PositionInterpolator {
    eventIn      SFFloat set_fraction        # (-,)
    exposedField MFFloat key           []    # (-,)
    exposedField MFVec3f keyValue      []    # (-,)
    eventOut     SFVec3f value_changed
  }
  \endverbatim
  
  The PositionInterpolator node linearly interpolates among a list of
  3D vectors. The keyValue field shall contain exactly as many values
  as in the key field.  4.6.8, Interpolator nodes
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.8>),
  contains a more detailed discussion of interpolators.

*/

/*!
  \var SoMFVec3f SoVRMLPositionInterpolator::keyValue
  The keyValue vector.
*/

/*!
  \var SoEngineOutput SoVRMLPositionInterpolator::value_changed
  The eventOut which is sent every time the interpolator has calculated a new value.
*/

#include <Inventor/VRMLnodes/SoVRMLPositionInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/engines/SoSubNodeEngineP.h>

SO_NODEENGINE_SOURCE(SoVRMLPositionInterpolator);

// Doc in parent
void
SoVRMLPositionInterpolator::initClass(void) // static
{
  SO_NODEENGINE_INTERNAL_INIT_CLASS(SoVRMLPositionInterpolator);
}

/*!
  Constructor.
*/
SoVRMLPositionInterpolator::SoVRMLPositionInterpolator(void)
{
  SO_NODEENGINE_INTERNAL_CONSTRUCTOR(SoVRMLPositionInterpolator);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(keyValue);
  SO_NODEENGINE_ADD_OUTPUT(value_changed, SoSFVec3f);
}

/*!
  Destructor.
*/
SoVRMLPositionInterpolator::~SoVRMLPositionInterpolator()
{
}

// Doc in parent
void
SoVRMLPositionInterpolator::evaluate(void)
{
  float interp;
  int idx = this->getKeyValueIndex(interp);
  if (idx < 0) return;

  const SbVec3f * v = this->keyValue.getValues(0);

  SbVec3f v0 = v[idx];
  if (interp > 0.0f) {
    SbVec3f v1 = v[idx+1];
    v0 = v0 + (v1-v0)*interp;
  }
  SO_ENGINE_OUTPUT(value_changed, SoSFVec3f, setValue(v0));
}
