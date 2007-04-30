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

#include <Inventor/SbVec4s.h>

#include <Inventor/SbVec4us.h>
#include <Inventor/SbVec4b.h>
#include <Inventor/SbVec4i32.h>

/*!
  \class SbVec4s Inventor/SbVec4s.h

  \since 2007-04-28
*/

SbVec4s &
SbVec4s::setValue(const SbVec4us & v)
{
  vec[0] = static_cast<short>(v[0]);
  vec[1] = static_cast<short>(v[1]);
  vec[2] = static_cast<short>(v[2]);
  vec[3] = static_cast<short>(v[3]);
  return *this;
}

SbVec4s &
SbVec4s::setValue(const SbVec4b & v)
{
  vec[0] = static_cast<short>(v[0]);
  vec[1] = static_cast<short>(v[1]);
  vec[2] = static_cast<short>(v[2]);
  vec[3] = static_cast<short>(v[3]);
  return *this;
}

SbVec4s &
SbVec4s::setValue(const SbVec4i32 & v)
{
  vec[0] = static_cast<short>(v[0]);
  vec[1] = static_cast<short>(v[1]);
  vec[2] = static_cast<short>(v[2]);
  vec[3] = static_cast<short>(v[3]);
  return *this;
}

SbVec4s &
SbVec4s::operator *= (double d)
{
  vec[0] = static_cast<short>(vec[0] * d);
  vec[1] = static_cast<short>(vec[1] * d);
  vec[2] = static_cast<short>(vec[2] * d);
  vec[3] = static_cast<short>(vec[3] * d); 
  return *this;
}
