/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoInterpolateFloat SoInterpolateFloat.h Inventor/engines/SoInterpolateFloat.h
  \brief The SoInterpolateFloat class is used to interpolate two floating point values.
  \ingroup engines
*/

#include <Inventor/engines/SoInterpolateFloat.h>
#include <Inventor/engines/SoSubEngineP.h>

/*!
  \var SoMFFloat SoInterpolateFloat::input0
  First set of input values.
*/
/*!
  \var SoMFFloat SoInterpolateFloat::input1
  Second set of input values.
*/

SO_INTERPOLATE_INTERNAL_INIT_CLASS(SoInterpolateFloat);

SO_INTERPOLATE_INTERNAL_SOURCE(SoInterpolateFloat,
                               SoMFFloat,
                               float,
                               (0.0f),
                               (1.0f),
                               (v1-v0)*a+v0);
