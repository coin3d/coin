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
  \class SoVRMLCoordinateInterpolator SoVRMLCoordinateInterpolator.h Inventor/VRMLnodes/SoVRMLCoorinateInterpolator.h
  \brief The SoVRMLCoordinateInterpolator class is used to interpolate 3D coordinates.
  \ingroup VRMLnodes
  
  \WEB3DCOPYRIGHT

  \verbatim
  CoordinateInterpolator {
    eventIn      SFFloat set_fraction        # (-inf, inf)
    exposedField MFFloat key           []    # (-inf, inf)
    exposedField MFVec3f keyValue      []    # (-inf, inf)
    eventOut     MFVec3f value_changed
  }
  \endverbatim

  This node linearly interpolates among a list of MFVec3f values. The
  number of coordinates in the keyValue field shall be an integer
  multiple of the number of keyframes in the key field. That integer
  multiple defines how many coordinates will be contained in the
  value_changed events.  4.6.8, Interpolator nodes
  (<http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-VRML97/part1/concepts.html#4.6.8>),
  contains a more detailed discussion of interpolators.

*/
#include <Inventor/VRMLnodes/SoVRMLCoordinateInterpolator.h>

#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/lists/SbList.h>

#include "engines/SoSubNodeEngineP.h"

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLCoordinateInterpolatorP {
public:
  SbList <SbVec3f> tmplist;
};

#endif // DOXYGEN_SKIP_THIS


SO_NODEENGINE_SOURCE(SoVRMLCoordinateInterpolator);

void
SoVRMLCoordinateInterpolator::initClass(void) // static
{
  SO_NODEENGINE_INTERNAL_INIT_CLASS(SoVRMLCoordinateInterpolator);
}

#define PRIVATE(obj) ((obj)->pimpl)

SoVRMLCoordinateInterpolator::SoVRMLCoordinateInterpolator(void)
{
  PRIVATE(this) = new SoVRMLCoordinateInterpolatorP;

  SO_NODEENGINE_INTERNAL_CONSTRUCTOR(SoVRMLCoordinateInterpolator);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(keyValue);
  SO_NODEENGINE_ADD_OUTPUT(value_changed, SoMFVec3f);
}

SoVRMLCoordinateInterpolator::~SoVRMLCoordinateInterpolator()
{
  delete PRIVATE(this);
}

void
SoVRMLCoordinateInterpolator::evaluate(void)
{
  if (!this->value_changed.isEnabled()) return;

  float interp;
  int i, idx = this->getKeyValueIndex(interp, this->keyValue.getNum());
  if (idx < 0) return;

  PRIVATE(this)->tmplist.truncate(0);
  const int numkeys = this->key.getNum();
  const int numcoords = this->keyValue.getNum() / numkeys;

  const SbVec3f * c0 = this->keyValue.getValues(idx*numcoords);
  const SbVec3f * c1 = c0;
  if (interp > 0.0f) c1 = this->keyValue.getValues((idx+1)*numcoords);

  for (i = 0; i < numcoords; i++) {
    PRIVATE(this)->tmplist.append(c0[i] + (c1[i]-c0[i]) * interp);
  }

  const SbVec3f * coords = PRIVATE(this)->tmplist.getArrayPtr();

  SO_ENGINE_OUTPUT(value_changed, SoMFVec3f, setNum(numcoords));
  SO_ENGINE_OUTPUT(value_changed, SoMFVec3f, setValues(0, numcoords, coords));
}

#undef PRIVATE

#endif // HAVE_VRML97
