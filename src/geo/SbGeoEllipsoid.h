#ifndef COIN_SBGEOELLIPSOID_H
#define COIN_SBGEOELLIPSOID_H

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

#include "SbGeoAngle.h"

class SbString;
class SbGeoAngle;

class SbGeoEllipsoid {
public:
  SbGeoEllipsoid(void);
  SbGeoEllipsoid(const SbString & desc, char hemisphere = 'N');
  SbGeoEllipsoid(const SbGeoEllipsoid & e);

  SbGeoEllipsoid & operator=(const SbGeoEllipsoid & e);

  double getA(void) const;
  double getE(void) const;
  double getEccentricitySquared(void) const;
  char getHemisphere(void) const;
  const SbGeoAngle & getLatStdParallel(void) const;
  const SbGeoAngle & getLongOrigin(void) const;

private:
  double a, e;
  double eccsquared;
  char hemisphere;
  SbGeoAngle lambda0;   // Longitude of origin
  SbGeoAngle phiF;      // Latitude of standard parallel
};

#endif // COIN_SBGEOELLIPSOID_H
