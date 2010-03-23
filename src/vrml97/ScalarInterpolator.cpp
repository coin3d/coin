/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_VRML97

/*!
  \class SoVRMLScalarInterpolator SoVRMLScalarInterpolator.h Inventor/VRMLnodes/SoVRMLScalarInterpolator.h
  \brief The SoVRMLScalarInterpolator class is used to interpolate scalar values.
  \ingroup VRMLnodes
  
  \verbatim
  ScalarInterpolator {
    eventIn      SFFloat set_fraction         # (-inf, inf)
    exposedField MFFloat key           []     # (-inf, inf)
    exposedField MFFloat keyValue      []     # (-inf, inf)
    eventOut     SFFloat value_changed
  }
  \endverbatim

  This node linearly interpolates among a list of SFFloat values. This
  interpolator is appropriate for any parameter defined using a single
  floating point value. Examples include width, radius, and intensity
  fields. The keyValue field shall contain exactly as many numbers as
  there are keyframes in the key field.  A more detailed discussion of
  interpolators is available in 4.6.8, Interpolator nodes
  (<http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-VRML97/part1/concepts.html#4.6.8>).
   
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

#include "engines/SoSubNodeEngineP.h"

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
  int idx = this->getKeyValueIndex(interp, this->keyValue.getNum());
  if (idx < 0) return;

  const float * v = this->keyValue.getValues(0);

  float v0 = v[idx];
  if (interp > 0.0f) {
    float v1 = v[idx+1];
    v0 = v0 + (v1-v0)*interp;
  }
  SO_ENGINE_OUTPUT(value_changed, SoSFFloat, setValue(v0));
}

#endif // HAVE_VRML97
