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

/*!
  \class SoInterpolateFloat SoInterpolateFloat.h Inventor/engines/SoInterpolateFloat.h
  \brief The SoInterpolateFloat class is used to interpolate two floating point values.
  \ingroup engines
*/

#include <Inventor/engines/SoInterpolateFloat.h>

#include "engines/SoSubEngineP.h"

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
