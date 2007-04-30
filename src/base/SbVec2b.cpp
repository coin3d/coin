/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/SbVec2b.h>

#include <Inventor/SbVec2ub.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2i32.h>

/*!
  \class SbVec2b Inventor/SbVec2b.h

  \since 2007-04-28
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
  vec[0] = static_cast<int8_t>(v[0]);
  vec[1] = static_cast<int8_t>(v[1]);
  return *this;
}

SbVec2b &
SbVec2b::setValue(const SbVec2i32 & v)
{
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
