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
  \class SoVRMLInterpolator SoVRMLInterpolator.h Inventor/VRMLnodes/SoVRMLInterpolator.h
  \brief The SoVRMLInterpolator class is an internal abstract class.

  This class collects the two fields that are common for all
  interpolator nodes, plus common code that operates on these
  fields. Since this is an abstract "helper" class, it does not
  represent an actual node from the VRML97 specification, so don't use
  it as such.

  For more information, a detailed discussion of interpolators is
  available in section 4.6.8 of the VRML97 specification:

  <http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-VRML97/part1/concepts.html#4.6.8>
*/

/*!
  \var SoMFFloat SoVRMLInterpolator::key

  This field contains a set of floating point values which the
  interpolation will run over. The key values should be monotonically
  non-decreasing.

  The field is inherited from it's declaration in the abstract
  SoVRMLInterpolator class into a range of different VRML interpolator
  nodes.

  See the class documentation of the \e non-abstract VRML interpolator
  node you want to use for information on what the key values
  represent in that specific context.
*/
/*!
  \var SoSFFloat SoVRMLInterpolator::set_fraction

  The set_fraction field gets an input signal that triggers a
  calculation of the next value_changed eventOut value.

  The field is inherited from it's declaration in the abstract
  SoVRMLInterpolator class into a range of different VRML interpolator
  nodes.
*/

#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>

#include <Inventor/VRMLnodes/SoVRMLMacros.h>

#include "engines/SoSubNodeEngineP.h"

SO_NODEENGINE_ABSTRACT_SOURCE(SoVRMLInterpolator);

void
SoVRMLInterpolator::initClass(void) // static
{
  SO_NODEENGINE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLInterpolator);
}

SoVRMLInterpolator::SoVRMLInterpolator(void) // protected
{
  SO_NODEENGINE_CONSTRUCTOR(SoVRMLInterpolator);

  SO_VRMLNODE_ADD_EVENT_IN(set_fraction);

  // initialize set_fraction to some value, since if set_fraction is
  // never set, we'll attempt to read an unitialized value when the
  // interpolator is destructed (all engines evaluates when
  // destructed)
  this->set_fraction.enableNotify(FALSE);
  this->set_fraction = 0.0f;
  this->set_fraction.enableNotify(TRUE);
  
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(key);
}

SoVRMLInterpolator::~SoVRMLInterpolator() // virtual, protected
{
}

/*!
  \COININTERNAL
*/
int 
SoVRMLInterpolator::getKeyValueIndex(float & interp, int numvalues)
{
  float fraction = this->set_fraction.getValue();
  const int n = this->key.getNum();
  if (n == 0 || numvalues == 0) return -1;

  const float * t = this->key.getValues(0); 
  for (int i = 0; i < SbMin(n, numvalues); i++) {
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
  return SbMin(numvalues,n)-1;
}

#endif // HAVE_VRML97
