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

#include <Inventor/SbVec2ui32.h>

#include <Inventor/SbVec2i32.h>
#include <Inventor/SbVec2ub.h>
#include <Inventor/SbVec2us.h>

/*!
  \class SbVec2ui32 Inventor/SbVec2ui32.h

  \since Coin 2.5
*/

SbVec2ui32 &
SbVec2ui32::setValue(const SbVec2i32 & v)
{
  vec[0] = static_cast<uint32_t>(v[0]);
  vec[1] = static_cast<uint32_t>(v[1]);
  return *this;
}

SbVec2ui32 &
SbVec2ui32::setValue(const SbVec2ub & v)
{
  vec[0] = static_cast<uint32_t>(v[0]);
  vec[1] = static_cast<uint32_t>(v[1]);
  return *this;
}

SbVec2ui32 &
SbVec2ui32::setValue(const SbVec2us & v)
{
  vec[0] = static_cast<uint32_t>(v[0]);
  vec[1] = static_cast<uint32_t>(v[1]);
  return *this;
}

void
SbVec2ui32::negate(void)
{
  vec[0] = static_cast<uint32_t>(0-vec[0]);
  vec[1] = static_cast<uint32_t>(0-vec[1]);
}

SbVec2ui32 &
SbVec2ui32::operator *= (double d)
{
  vec[0] = static_cast<uint32_t>(vec[0] * d);
  vec[1] = static_cast<uint32_t>(vec[1] * d);
  return *this;
}
