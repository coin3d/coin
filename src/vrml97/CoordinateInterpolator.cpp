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
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.8>),
  contains a more detailed discussion of interpolators.

*/
#include <Inventor/VRMLnodes/SoVRMLCoordinateInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/engines/SoSubNodeEngineP.h>
#include <Inventor/lists/SbList.h>

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

#undef THIS
#define THIS this->pimpl

SoVRMLCoordinateInterpolator::SoVRMLCoordinateInterpolator(void)
{
  THIS = new SoVRMLCoordinateInterpolatorP;

  SO_NODEENGINE_INTERNAL_CONSTRUCTOR(SoVRMLCoordinateInterpolator);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(keyValue);
  SO_NODEENGINE_ADD_OUTPUT(value_changed, SoMFVec3f);
}

SoVRMLCoordinateInterpolator::~SoVRMLCoordinateInterpolator()
{
  delete THIS;
}

void
SoVRMLCoordinateInterpolator::evaluate(void)
{
  if (!this->value_changed.isEnabled()) return;

  float interp;
  int i, idx = this->getKeyValueIndex(interp);
  if (idx < 0) return;

  THIS->tmplist.truncate(0);
  const int numkeys = this->key.getNum();
  const int numcoords = this->keyValue.getNum() / numkeys;

  const SbVec3f * c0 = this->keyValue.getValues(idx*numcoords);
  const SbVec3f * c1 = c0;
  if (interp > 0.0f) c1 = this->keyValue.getValues((idx+1)*numcoords);

  for (i = 0; i < numcoords; i++) {
    THIS->tmplist.append(c0[i] + (c1[i]-c0[i]) * interp);
  }

  const SbVec3f * coords = THIS->tmplist.getArrayPtr();

  SO_ENGINE_OUTPUT(value_changed, SoMFVec3f, setNum(numcoords));
  SO_ENGINE_OUTPUT(value_changed, SoMFVec3f, setValues(0, numcoords, coords));
}
