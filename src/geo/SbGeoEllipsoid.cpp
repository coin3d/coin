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

#include "SbGeoEllipsoid.h"
#include <Inventor/SbString.h>
#include <stdio.h>

SbGeoEllipsoid::SbGeoEllipsoid(void)
{
  this->a = 0.0;
  this->e = 0.0;
}
  // constructor
SbGeoEllipsoid::SbGeoEllipsoid(const SbString & desc, char hemisphere)
{
  if (desc == "WGS_1984" || desc == "WGS84" || desc == "WGS1984UTM") {
    this->a = 6378137.0;
    this->e = 0.081819191;
    this->eccsquared = 0.00669438;
  }
  else {
    fprintf(stderr,"Unsupported ellipsoid: %s\n",
            desc.getString());
    this->a = 6378137.0;
    this->e = 0.081819191;
    this->eccsquared = 0.00669438;
  }

  assert(hemisphere == 'N' || hemisphere == 'S');
  this->hemisphere = hemisphere;

  // polar stereographic
  if (this->hemisphere == 'N') {
    this->lambda0.setDegree(0);   // Longitude of origin
    this->phiF.setDegree(75);     // Latitude of standard parallel
  }
  else {
    this->lambda0.setDegree(180);   // Longitude of origin
    this->phiF.setDegree(-75);      // Latitude of standard parallel
  }
}

  // copy constructor
SbGeoEllipsoid::SbGeoEllipsoid(const SbGeoEllipsoid & e)
{
  *this = e;
}

SbGeoEllipsoid &
SbGeoEllipsoid::operator=(const SbGeoEllipsoid & e)
{
  this->e = e.e;
  this->a = e.a;
  this->eccsquared = e.eccsquared;
  this->hemisphere = e.hemisphere;
  this->lambda0 = e.lambda0;
  this->phiF = e.phiF;
  return *this;
}

double
SbGeoEllipsoid::getA(void) const
{
  return this->a;
}

double
SbGeoEllipsoid::getE(void) const
{
  return this->e;
}

double
SbGeoEllipsoid::getEccentricitySquared(void) const
{
  return this->eccsquared;
}

char
SbGeoEllipsoid::getHemisphere(void) const
{
  return this->hemisphere;
}

const SbGeoAngle &
SbGeoEllipsoid::getLatStdParallel(void) const
{
  return this->phiF;
}

const SbGeoAngle &
SbGeoEllipsoid::getLongOrigin(void) const
{
  return this->lambda0;
}
