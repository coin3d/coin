#ifndef COIN_SBUTMPROJECTION_H
#define COIN_SBUTMPROJECTION_H
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

#include "SbGeoProjection.h"

class SbGeoAngle;
class SbGeoEllipsoid;

class SbUTMProjection : public SbGeoProjection {
  typedef SbGeoProjection inherited;
public:
  SbUTMProjection(const int utmzone,
                  const SbGeoEllipsoid & e,
                  double FE = 0.0, double FN = 0.0);

  virtual SbBool isUTMProjection(void) const;

  void setUTMZone(const int utmzone);
  int getUTMZone(void) const;

  virtual void project(const SbGeoAngle & phi,
                       const SbGeoAngle & lambda,
                       double * easting,
                       double * northing) const;

  virtual void unproject(const double easting,
                         const double northing,
                         SbGeoAngle * phi,
                         SbGeoAngle * lambda) const;

private:
  int forcedutmzone;
};

#endif // UTMPROJECTION_H
