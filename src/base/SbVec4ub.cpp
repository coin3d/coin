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

#include <Inventor/SbVec4ub.h>

#include <Inventor/SbVec4b.h>
#include <Inventor/SbVec4us.h>
#include <Inventor/SbVec4ui32.h>

/*!
  \class SbVec4ub Inventor/SbVec4ub.h

  \since Coin 2.5
*/

SbVec4ub &
SbVec4ub::setValue(const SbVec4b & v)
{
  vec[0] = static_cast<uint8_t>(v[0]);
  vec[1] = static_cast<uint8_t>(v[1]);
  vec[2] = static_cast<uint8_t>(v[2]);
  vec[3] = static_cast<uint8_t>(v[3]);
  return *this;
}

SbVec4ub &
SbVec4ub::setValue(const SbVec4us & v)
{
  vec[0] = static_cast<uint8_t>(v[0]);
  vec[1] = static_cast<uint8_t>(v[1]);
  vec[2] = static_cast<uint8_t>(v[2]);
  vec[3] = static_cast<uint8_t>(v[3]);
  return *this;
}

SbVec4ub &
SbVec4ub::setValue(const SbVec4ui32 & v)
{
  vec[0] = static_cast<uint8_t>(v[0]);
  vec[1] = static_cast<uint8_t>(v[1]);
  vec[2] = static_cast<uint8_t>(v[2]);
  vec[3] = static_cast<uint8_t>(v[3]);
  return *this;
}

void
SbVec4ub::negate(void)
{
  vec[0] = static_cast<uint8_t>(-vec[0]);
  vec[1] = static_cast<uint8_t>(-vec[1]);
  vec[2] = static_cast<uint8_t>(-vec[2]);
  vec[3] = static_cast<uint8_t>(-vec[3]);
}

SbVec4ub &
SbVec4ub::operator *= (double d)
{
  vec[0] = static_cast<uint8_t>(vec[0] * d);
  vec[1] = static_cast<uint8_t>(vec[1] * d);
  vec[2] = static_cast<uint8_t>(vec[2] * d);
  vec[3] = static_cast<uint8_t>(vec[3] * d);
  return *this;
}
