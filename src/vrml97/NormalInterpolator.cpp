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
  \class SoVRMLNormalInterpolator SoVRMLNormalInterpolator.h Inventor/VRMLnodes/SoVRMLNormalInterpolator.h
  \brief The SoVRMLNormalInterpolator class is used to interpolate normals.
  \ingroup VRMLnodes
  
  \WEB3DCOPYRIGHT

  \verbatim
  NormalInterpolator {
    eventIn      SFFloat set_fraction       # (-inf, inf)
    exposedField MFFloat key           []   # (-inf, inf)
    exposedField MFVec3f keyValue      []   # (-inf, inf)
    eventOut     MFVec3f value_changed
  }
  \endverbatim

  The NormalInterpolator node interpolates among a list of normal
  vector sets specified by the keyValue field. The output vector,
  value_changed, shall be a set of normalized vectors.  Values in the
  keyValue field shall be of unit length. The number of normals in the
  keyValue field shall be an integer multiple of the number of
  keyframes in the key field. That integer multiple defines how many
  normals will be contained in the value_changed events.  Normal
  interpolation shall be performed on the surface of the unit sphere.
  That is, the output values for a linear interpolation from a point P
  on the unit sphere to a point Q also on the unit sphere shall lie
  along the shortest arc (on the unit sphere) connecting points P and
  Q. Also, equally spaced input fractions shall result in arcs of
  equal length. The results are undefined if P and Q are diagonally
  opposite.  A more detailed discussion of interpolators is provided
  in 4.6.8, Interpolator nodes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.8).
  
*/

/*!
  \var SoMFVec3f SoVRMLNormalInterpolator::keyValue
  The keyValue vector.
*/

/*!
  \var SoEngineOutput SoVRMLNormalInterpolator::value_changed
  The eventOut which is sent every time the interpolator has calculated a new value.
*/

#include <Inventor/VRMLnodes/SoVRMLNormalInterpolator.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/engines/SoSubNodeEngineP.h>

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLNormalInterpolatorP {
public:
  SbList <SbVec3f> tmplist;
};

#endif // DOXYGEN_SKIP_THIS

SO_NODEENGINE_SOURCE(SoVRMLNormalInterpolator);

// Doc in parent
void
SoVRMLNormalInterpolator::initClass(void)
{
  SO_NODEENGINE_INTERNAL_INIT_CLASS(SoVRMLNormalInterpolator);
}

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoVRMLNormalInterpolator::SoVRMLNormalInterpolator(void)
{
  THIS = new SoVRMLNormalInterpolatorP;

  SO_NODEENGINE_INTERNAL_CONSTRUCTOR(SoVRMLNormalInterpolator);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(keyValue);
  SO_NODEENGINE_ADD_OUTPUT(value_changed, SoMFVec3f);
}

/*!
  Destructor.
*/
SoVRMLNormalInterpolator::~SoVRMLNormalInterpolator()
{
  delete THIS;
}

// Doc in parent
void
SoVRMLNormalInterpolator::evaluate(void)
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
