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
#include <Inventor/SbBasic.h>
#include <math.h>
#include <assert.h>

#define ANGLE_TOLERANCE 1e-5

/*
  Default constructor radians
*/

SbGeoAngle::SbGeoAngle(const double d)
{
  this->a = d;
}

/*
  Default constructor degrees
*/

SbGeoAngle::SbGeoAngle(const double deg,
                       const double min,
                       const double sec,
                       const char direction)
{
  assert(direction == 'N' ||
         direction == 'S' ||
         direction == 'E' ||
         direction == 'W' &&
         "direction must be either N, S, E or W");

  this->a = deg * M_PI / 180.0;
  this->a += (min / 60.0) * M_PI / 180;
  this->a += (sec / 3600.0) * M_PI / 180;
  if (direction == 'S') this->a = -this->a;
}

/*
  Copy  constructor
*/

SbGeoAngle::SbGeoAngle(const SbGeoAngle & a)
{
  this->a = a.a;
}

void
SbGeoAngle::setDegree(const int deg)
{
  this->a = deg * M_PI / 180.0;
}

/*
  Operators
 */
SbGeoAngle &
SbGeoAngle::operator=(const double d)
{
  this->a = d;
  return *this;
}

SbGeoAngle &
SbGeoAngle::operator=(SbGeoAngle ang)
{
  this->a = ang.a;
  return *this;
}

double
SbGeoAngle::rad(void) const
{
  return this->a;
}

int
SbGeoAngle::deg(void) const
{
  return int(this->degrees());
}

SbGeoAngle &
SbGeoAngle::operator+=(const SbGeoAngle & a)
{
  this->a += a.a;
  return *this;
}

 SbGeoAngle &
SbGeoAngle::operator-=(const SbGeoAngle & a)
{
  this->a -= a.a;
  return *this;
}

 SbGeoAngle
SbGeoAngle::operator-() const
{
  SbGeoAngle p;
  p.a = -this->a;
  return p;
}

 SbGeoAngle
SbGeoAngle::operator+(SbGeoAngle a) const
{
  a += *this;
  return a;
}

 SbGeoAngle
SbGeoAngle::operator-(SbGeoAngle a) const
{
  a -= *this;
  return -a;
}

SbGeoAngle
SbGeoAngle::operator+(double d) const
{
  return *this + SbGeoAngle(d);
}

SbGeoAngle
SbGeoAngle::operator-(double d) const
{
  return *this - SbGeoAngle(d);
}


SbGeoAngle &
SbGeoAngle::operator*=(double d)
{
  this->a *= d;
  return *this;
}

SbGeoAngle
SbGeoAngle::operator*(double d) const
{
  return SbGeoAngle(a*d);
}

SbGeoAngle
operator*(double d, SbGeoAngle a)
{
  a *= d;
  return a;
}

SbGeoAngle
operator+(double d, SbGeoAngle a)
{
  a += d;
  return a;
}

SbGeoAngle
operator-(double d, SbGeoAngle a)
{
  a -= d;
  return a;
}

SbGeoAngle &
SbGeoAngle::operator/=(double d)
{
  d = 1/d;
  *this *= d;
  return *this;
}

 SbGeoAngle
SbGeoAngle::operator/(double d) const
{
  d = 1/d;
  return *this * d;
}

 bool
SbGeoAngle::operator==(const SbGeoAngle & p) const
{
  return fabs(a - p.a) < ANGLE_TOLERANCE;
}

 bool
SbGeoAngle::operator!=(const SbGeoAngle & a) const
{
  return !(*this == a);
}

 bool
SbGeoAngle::operator<(const SbGeoAngle & v) const
{
  return this->a < v.a;
}


 bool
SbGeoAngle::operator>(const SbGeoAngle & v) const
{
  return this->a > v.a;
}

 bool
SbGeoAngle::operator<=(const SbGeoAngle & v) const
{
  return !(*this > v);
}

bool
SbGeoAngle::operator>=(const SbGeoAngle & v) const
{
  return !(*this < v);
}

int
SbGeoAngle::minutes() const
{
  double tmp = this->degrees();
  tmp -= (double) this->deg();

  return (int) (tmp * 60.0);
}

double
SbGeoAngle::seconds() const
{
  double tmp = this->degrees();
  tmp -= this->deg() + this->minutes() / 60.0;

  return tmp * 60.0 * 60.0;
}

double
SbGeoAngle::degrees(void) const
{
  double tmp = fmod(180.0 * (this->a / M_PI), 360.0);
  if (tmp > 180.0) {
    return tmp - 360.0;
  }
  return tmp;
}

#undef ANGLE_TOLERANCE
