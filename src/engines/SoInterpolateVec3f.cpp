/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
