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
#include <stdlib.h>

void
SoGeo::init(void)
{
  SoGeoElement::initClass();
  SoGeoOrigin::initClass();
  SoGeoLocation::initClass();
  SoGeoSeparator::initClass();
  SoGeoCoordinate::initClass();
}

// return zone number stored in string, or 0 if parsing failed
static int find_utm_zone(const SbString & s)
{
  if (s.getLength() < 2) return 0;
  if (s[0] != 'Z' && s[1] != 'z') return 0;
  
  return (int) atoi(s.getString()+1);
}

SbMatrix
SoGeo::calculateTransform(const SbString * originsystem,
                          const int numoriginsys,
                          const SbString & geocoords,
                          const SbString * targetsystem,
                          const int numtargetsys,
                          const SbString & targetcoords)
{

  double originpos[3];
  double targetpos[3];

  int num = sscanf(geocoords.getString(), "%lf%lf%lf",
                   &originpos[0], &originpos[1], &originpos[2]);
  assert(num == 3);

  num = sscanf(targetcoords.getString(), "%lf%lf%lf",
               &targetpos[0], &targetpos[1], &targetpos[2]);


  assert(numoriginsys == 2);
  assert(numtargetsys == 2);

  // only support projecting to UTM for now
  assert(originsystem[0] == "UTM" && "unsupported projection");

  if (targetsystem[0] == "UTM") {
    if (targetsystem[1] != originsystem[1]) { // different UTM zones
      SbUTMProjection targetproj(find_utm_zone(targetsystem[1]), SbGeoEllipsoid("WGS84"));
      SbUTMProjection proj(find_utm_zone(originsystem[1]), SbGeoEllipsoid("WGS84"));
      
      SbGeoAngle lat, lng;

      // unconvert target to geo coordinates
      targetproj.unproject(targetpos[0], targetpos[1], &lat, &lng);

      // project coordinates into the original utm system
      proj.project(lat,lng, &targetpos[0], &targetpos[1]);
    }
  }
  else if (targetsystem[1] == "GD") {
    int zone = find_utm_zone(originsystem[1]);
    SbUTMProjection proj(zone, SbGeoEllipsoid("WGS84"));

    SbGeoAngle lat(targetpos[0], 0.0, 0.0, 'N');
    SbGeoAngle lng(targetpos[1], 0.0, 0.0, 'N');
    
    // just write back into targetpos
    proj.project(lat, lng, &targetpos[0], &targetpos[1]);
  }
  else {
    assert(0 && "unsupported projection");
  }



  SbMatrix mat = SbMatrix::identity();
  mat.setTranslate(SbVec3f((float)(targetpos[0]-originpos[0]),
                           (float)(targetpos[1]-originpos[1]),
                           (float)(targetpos[2]-originpos[2])));

  return mat;
}
