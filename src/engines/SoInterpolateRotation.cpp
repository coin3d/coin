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
  \class SoInterpolateRotation SoInterpolateRotation.h Inventor/engines/SoInterpolateRotation.h
  \brief The SoInterpolateRotation class is used to interpolate between two rotations.
  \ingroup engines
*/

#include <Inventor/engines/SoInterpolateRotation.h>
#include <Inventor/engines/SoSubEngineP.h>
#include <Inventor/SbVec3f.h>

/*!
  \var SoMFRotation SoInterpolateRotation::input0
  First set of input values.
*/
/*!
  \var SoMFRotation SoInterpolateRotation::input1
  Second set of input values.
*/

SO_INTERPOLATE_INTERNAL_INIT_CLASS(SoInterpolateRotation);

SO_INTERPOLATE_INTERNAL_SOURCE(SoInterpolateRotation,
                               SoMFRotation,
                               SbRotation,
                               (SbVec3f(0.0f,0.0f,1.0f),0.0f),
                               (SbVec3f(0.0f,0.0f,1.0f),0.0f),
                               SbRotation::slerp(v0,v1,a));
