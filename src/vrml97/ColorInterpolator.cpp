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
  \ingroup VRMLnodes

  WEB3DCOPYRIGHT

  \verbatim
  ColorInterpolator {
    eventIn      SFFloat set_fraction        # (-inf, inf)
    exposedField MFFloat key           []    # (-inf, inf)
    exposedField MFColor keyValue      []    # [0,1]
    eventOut     SFColor value_changed
  }
  \endverbatim

  This node interpolates among a list of MFColor key values to produce
  an SFColor (RGB) value_changed event. The number of colours in the
  keyValue field shall be equal to the number of keyframes in the key
  field. The keyValue field and value_changed events are defined in
  RGB colour space. A linear interpolation using the value of
  set_fraction as input is performed in HSV space (see 
  http://www.web3d.org/technicalinfo/specifications/vrml97/part1/bibliography.html#[FOLE]
  for description of RGB and HSV colour spaces). The results are undefined
  when interpolating between two consecutive keys with complementary
  hues.  

  4.6.8, Interpolator nodes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.8), 
  contains a detailed discussion of interpolators.

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
