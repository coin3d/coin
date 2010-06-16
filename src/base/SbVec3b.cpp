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

#include <Inventor/SbVec3b.h>

#include <limits>

#include <Inventor/SbVec3ub.h>
#include <Inventor/SbVec3s.h>
#include <Inventor/SbVec3i32.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec3d.h>

/*!
  \class SbVec3b Inventor/SbVec3b.h

  \since Coin 2.5
*/

SbVec3b &
SbVec3b::setValue(const SbVec3ub & v)
{
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  vec[2] = static_cast<int8_t>(v[2]);
  return *this;
}

SbVec3b &
SbVec3b::setValue(const SbVec3s & v)
{
#if COIN_DEBUG
  if (v[0] > std::numeric_limits<int8_t>::max() || v[0] < -std::numeric_limits<int8_t>::max() ||
      v[1] > std::numeric_limits<int8_t>::max() || v[1] < -std::numeric_limits<int8_t>::max() ||
      v[2] > std::numeric_limits<int8_t>::max() || v[2] < -std::numeric_limits<int8_t>::max()) {
    SoDebugError::post("SbVec3b::setValue", "SbVec3s argument out of range for SbVec3b");
  }
#endif // COIN_DEBUG
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  vec[2] = static_cast<int8_t>(v[2]);
  return *this;
}

SbVec3b &
SbVec3b::setValue(const SbVec3i32 & v)
{
#if COIN_DEBUG
  if (v[0] > std::numeric_limits<int8_t>::max() || v[0] < -std::numeric_limits<int8_t>::max() ||
      v[1] > std::numeric_limits<int8_t>::max() || v[1] < -std::numeric_limits<int8_t>::max() ||
      v[2] > std::numeric_limits<int8_t>::max() || v[2] < -std::numeric_limits<int8_t>::max()) {
    SoDebugError::post("SbVec3b::setValue", "SbVec3i32 argument out of range for SbVec3b");
  }
#endif // COIN_DEBUG
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  vec[2] = static_cast<int8_t>(v[2]);
  return *this;
}

SbVec3b &
SbVec3b::setValue(const SbVec3f & v)
{
#if COIN_DEBUG
  if (v[0] > std::numeric_limits<int8_t>::max() || v[0] < -std::numeric_limits<int8_t>::max() ||
      v[1] > std::numeric_limits<int8_t>::max() || v[1] < -std::numeric_limits<int8_t>::max() ||
      v[2] > std::numeric_limits<int8_t>::max() || v[2] < -std::numeric_limits<int8_t>::max()) {
    SoDebugError::post("SbVec3b::setValue", "SbVec3f argument out of range for SbVec3b");
  }
#endif // COIN_DEBUG
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  vec[2] = static_cast<int8_t>(v[2]);
  return *this;
}

SbVec3b &
SbVec3b::setValue(const SbVec3d & v)
{
#if COIN_DEBUG
  if (v[0] > std::numeric_limits<int8_t>::max() || v[0] < -std::numeric_limits<int8_t>::max() ||
      v[1] > std::numeric_limits<int8_t>::max() || v[1] < -std::numeric_limits<int8_t>::max() ||
      v[2] > std::numeric_limits<int8_t>::max() || v[2] < -std::numeric_limits<int8_t>::max()) {
    SoDebugError::post("SbVec3b::setValue", "SbVec3d argument out of range for SbVec3b");
  }
#endif // COIN_DEBUG
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  vec[2] = static_cast<int8_t>(v[2]);
  return *this;
}

SbVec3b &
SbVec3b::operator *= (double d)
{
  vec[0] = static_cast<int8_t>(vec[0] * d);
  vec[1] = static_cast<int8_t>(vec[1] * d);
  vec[2] = static_cast<int8_t>(vec[2] * d);
  return *this;
}
