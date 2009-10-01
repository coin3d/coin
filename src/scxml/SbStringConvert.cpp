/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "scxml/SbStringConvert.h"

#include <cstdio>
#include <cstring>

#include <Inventor/SbString.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec3d.h>
#include <Inventor/SbRotation.h>

using std::strcmp;
using std::strncmp;

SbStringConvert::TypeIdentity
SbStringConvert::typeOf(SbString & str)
{
  if (strncmp(str.getString(), "Sb", 2) == 0) {
    if (strncmp(str.getString() + 2, "Vec2s(", 6) == 0) {
      return SBVEC2S;
    }
    else if (strncmp(str.getString() + 2, "Vec2f(", 6) == 0) {
      return SBVEC2F;
    }
    else if (strncmp(str.getString() + 2, "Vec3f(", 6) == 0) {
      return SBVEC3F;
    }
    else if (strncmp(str.getString() + 2, "Vec3d(", 6) == 0) {
      return SBVEC3D;
    }
    else if (strncmp(str.getString() + 2, "Rotation(", 9) == 0) {
      return SBROTATION;
    }
  }
  else {
    if (str[0] >= '0' && str[0] <= '9') {
      return NUMERIC;
    }
    if (str[0] == '-' && str[1] >= '0' && str[1] <= '9') {
      return NUMERIC;
    }
    if (strcmp(str.getString(), "TRUE") == 0 ||
        strcmp(str.getString(), "FALSE") == 0) {
      return BOOLEAN;
    }
  }
  return UNKNOWN;
}
