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

#include "SoGeo.h"
#include <Inventor/nodes/SoGeoOrigin.h>
#include <Inventor/nodes/SoGeoLocation.h>
#include <Inventor/nodes/SoGeoSeparator.h>
#include <Inventor/nodes/SoGeoCoordinate.h>
#include <Inventor/elements/SoGeoElement.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec3f.h>

#include "SbGeoProjection.h"
#include "SbUTMProjection.h"
#include "SbGeoAngle.h"
#include "SbGeoEllipsoid.h"

#include <assert.h>
#include <stdio.h>

void 
SoGeo::init(void)
{
  SoGeoElement::initClass();
  SoGeoOrigin::initClass();
  SoGeoLocation::initClass();
  SoGeoSeparator::initClass();
  SoGeoCoordinate::initClass();
}

SbMatrix
SoGeo::calculateTransform(const SbString * originsystem,
                          const int numoriginsys,
                          const SbString & geocoords,
                          const SbString * targetsystem,
                          const int numtargetsys,
                          const SbString & targetcoords)
{
  
  assert(numoriginsys == 2);
  assert(numtargetsys == 2);

  assert(targetsystem[0] == "UTM");
  assert(originsystem[0] == "UTM");

  double originpos[3];
  double targetpos[3];

  int num = sscanf(geocoords.getString(), "%lf%lf%lf",
                   &originpos[0], &originpos[1], &originpos[2]);
  assert(num == 3);

  num = sscanf(targetcoords.getString(), "%lf%lf%lf",
               &targetpos[0], &targetpos[1], &targetpos[2]);
  
  SbMatrix mat = SbMatrix::identity();
  mat.setTranslate(SbVec3f((float)(targetpos[0]-originpos[0]),
                           (float)(targetpos[1]-originpos[1]),
                           (float)(targetpos[2]-originpos[2])));
  
  return mat;
}
