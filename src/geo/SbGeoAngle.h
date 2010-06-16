#ifndef COIN_SBGEOANGLE_H
#define COIN_SBGEOANGLE_H

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

class SbGeoAngle {
public:

  SbGeoAngle(const double d = 0.0);
  SbGeoAngle(const double deg,
             const double min,
             const double sec,
             const char direction = 'N');

  SbGeoAngle(const SbGeoAngle & ang);

  SbGeoAngle & operator=(const double d);
  SbGeoAngle & operator=(SbGeoAngle a);

  void setDegree(const int d);

  double rad(void) const;
  int    deg(void) const;
  int    minutes(void) const;
  double seconds(void) const;

  operator double() const { return this->rad(); }

  SbGeoAngle & operator+=(const SbGeoAngle & a);
  SbGeoAngle & operator-=(const SbGeoAngle & a);
  SbGeoAngle operator-() const;
  SbGeoAngle operator+(SbGeoAngle a) const;
  SbGeoAngle operator-(SbGeoAngle a) const;
  SbGeoAngle operator+(double d) const;
  SbGeoAngle operator-(double d) const;
  friend SbGeoAngle operator+(double d, SbGeoAngle a);
  friend SbGeoAngle operator-(double d, SbGeoAngle a);

  SbGeoAngle & operator*=(double d);
  SbGeoAngle operator*(double d) const;
  friend SbGeoAngle operator*(double d, SbGeoAngle a);

  SbGeoAngle & operator/=(double d);
  SbGeoAngle operator/(double d) const;

  bool operator==(const SbGeoAngle & p) const;
  bool operator!=(const SbGeoAngle & a) const;

  bool operator<(const SbGeoAngle & v) const;
  bool operator>(const SbGeoAngle & v) const;
  bool operator<=(const SbGeoAngle & v) const;
  bool operator>=(const SbGeoAngle & v) const;

private:

  double degrees(void) const;
  double a;
};

#endif // ANGLE_H
