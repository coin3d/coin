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
  \class SoInterpolateVec3f SoInterpolateVec3f.h Inventor/engines/SoInterpolateVec3f.h
  \brief The SoInterpolateVec3f class is used to interpolate between pairs of 3D vectors.
  \ingroup engines
*/

#include <Inventor/engines/SoInterpolateVec3f.h>
#include <Inventor/engines/SoSubEngineP.h>

/*!
  \var SoMFVec3f SoInterpolateVec3f::input0
  First set of input values.
*/
/*!
  \var SoMFVec3f SoInterpolateVec3f::input1
  Second set of input values.
*/

SO_INTERPOLATE_INTERNAL_INIT_CLASS(SoInterpolateVec3f);

SO_INTERPOLATE_INTERNAL_SOURCE(SoInterpolateVec3f,
                               SoMFVec3f,
                               SbVec3f,
                               (0.0f,0.0f,0.0f),
                               (0.0f,0.0f,0.0f),
                               (v1-v0)*a+v0);
