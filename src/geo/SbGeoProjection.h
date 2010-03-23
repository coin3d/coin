#ifndef COIN_SBGEOPROJECTION_H
#define COIN_SBGEOPROJECTION_H

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
#include "SbGeoEllipsoid.h"

#include <Inventor/SbBasic.h>

class SbGeoProjection {
public:
  SbGeoProjection(const SbGeoEllipsoid & ellipsoid,
                  double FE = 0.0, double FN = 0.0);
  virtual ~SbGeoProjection();

  virtual SbBool isUTMProjection(void) const;

  virtual void project(const SbGeoAngle & phi,    // latitude
                       const SbGeoAngle & lambda, // longitude
                       double * easting,
                       double * northing) const = 0;

  virtual void unproject(const double easting,
                         const double northing,
                         SbGeoAngle * phi,
                         SbGeoAngle * lambda) const = 0;

  // convenience methods
  void project(const double latdeg,
               const double latmin,
               const double latsec,
               const double lngdeg,
               const double lngmin,
               const double lngsec,
               double & easting,
               double & northing) {
    SbGeoAngle lat(latdeg, latmin, latsec);
    SbGeoAngle lng(lngdeg, lngmin, lngsec);
    this->project(lat, lng, &easting, & northing);
  }

  void project(const double lat,
               const double lng,
               double & easting,
               double & northing) {
    SbGeoAngle latangle(lat, 0.0, 0.0);
    SbGeoAngle lngangle(lng, 0.0, 0.0);
    this->project(latangle, lngangle, &easting, & northing);
  }

  void unproject(const double easting,
                 const double northing,
                 double & latdeg,
                 double & latmin,
                 double & latsec,
                 double & lngdeg,
                 double & lngmin,
                 double & lngsec) {
    SbGeoAngle lat, lng;
    this->unproject(easting, northing, &lat, &lng);
    latdeg = lat.deg();
    latmin = lat.minutes();
    latsec = lat.seconds();

    lngdeg = lng.deg();
    lngmin = lng.minutes();
    lngsec = lng.seconds();
  }

  void unproject(const double easting,
                 const double northing,
                 double & latdeg,
                 double & lngdeg)
  {
    SbGeoAngle lat, lng;
    this->unproject(easting, northing, &lat, &lng);

    latdeg = lat.deg() + lat.minutes() / 60.0 + lat.seconds() / 3600.0;
    lngdeg = lng.deg() + lng.minutes() / 60.0 + lng.seconds() / 3600.0;
  }
protected:
  SbGeoEllipsoid ellipsoid;
  double FE;       // false easting
  double FN;       // false northing
};

#endif // COIN_SBGEOPROJECTION_H
