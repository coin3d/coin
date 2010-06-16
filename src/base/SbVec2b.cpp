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

#include <Inventor/SbVec2b.h>

#include <limits>

#include <Inventor/SbVec2ub.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2i32.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2d.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \class SbVec2b Inventor/SbVec2b.h
  \brief a vector class for containing two byte integers.

  \sa SbVec2ub
  \since Coin 2.5
*/

SbVec2b &
SbVec2b::setValue(const SbVec2ub & v)
{
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  return *this;
}

SbVec2b &
SbVec2b::setValue(const SbVec2s & v)
{
#if COIN_DEBUG
  if (v[0] > std::numeric_limits<int8_t>::max() || v[0] < -std::numeric_limits<int8_t>::max() ||
      v[1] > std::numeric_limits<int8_t>::max() || v[1] < -std::numeric_limits<int8_t>::max()) {
    SoDebugError::post("SbVec2b::setValue", "SbVec2s argument out of range for SbVec2b");
  }
#endif // COIN_DEBUG
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  return *this;
}

SbVec2b &
SbVec2b::setValue(const SbVec2i32 & v)
{
#if COIN_DEBUG
  if (v[0] > std::numeric_limits<int8_t>::max() || v[0] < -std::numeric_limits<int8_t>::max() ||
      v[1] > std::numeric_limits<int8_t>::max() || v[1] < -std::numeric_limits<int8_t>::max()) {
    SoDebugError::post("SbVec2b::setValue", "SbVec2i32 argument out of range for SbVec2b");
  }
#endif // COIN_DEBUG
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  return *this;
}

SbVec2b &
SbVec2b::setValue(const SbVec2f & v)
{
#if COIN_DEBUG
  if (v[0] > std::numeric_limits<int8_t>::max() || v[0] < -std::numeric_limits<int8_t>::max() ||
      v[1] > std::numeric_limits<int8_t>::max() || v[1] < -std::numeric_limits<int8_t>::max()) {
    SoDebugError::post("SbVec2b::setValue", "SbVec2f argument out of range for SbVec2b");
  }
#endif // COIN_DEBUG
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  return *this;
}

SbVec2b &
SbVec2b::setValue(const SbVec2d & v)
{
#if COIN_DEBUG
  if (v[0] > std::numeric_limits<int8_t>::max() || v[0] < -std::numeric_limits<int8_t>::max() ||
      v[1] > std::numeric_limits<int8_t>::max() || v[1] < -std::numeric_limits<int8_t>::max()) {
    SoDebugError::post("SbVec2b::setValue", "SbVec2d argument out of range for SbVec2b");
  }
#endif // COIN_DEBUG
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  return *this;
}

SbVec2b &
SbVec2b::operator *= (double d)
{
  vec[0] = static_cast<int8_t>(vec[0] * d);
  vec[1] = static_cast<int8_t>(vec[1] * d);
  return *this;
}
