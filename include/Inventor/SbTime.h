#ifndef COIN_SBTIME_H
#define COIN_SBTIME_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <stdio.h>

#include <Inventor/system/inttypes.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>

// Avoid problem with Microsoft Visual C++ Win32 API headers (they
// #define "max" (in 3 different header files, no less)).
#ifdef max
#define SBTIME_UNDEF_MAX
#undef max
#endif // max


class COIN_DLL_API SbTime {
public:
  SbTime(void);
  SbTime(const double sec);
  SbTime(const int32_t sec, const long usec);
  SbTime(const struct timeval * const tv);
  static SbTime getTimeOfDay(void);
  void setToTimeOfDay(void);
  static SbTime zero(void);

  static SbTime max(void);
  static SbTime maxTime(void);
  void setValue(const double sec);
  void setValue(const int32_t sec, const long usec);
  void setValue(const struct timeval * const tv);
  void setMsecValue(const unsigned long msec);
  double getValue(void) const;
  void getValue(time_t & sec, long & usec) const;
  void getValue(struct timeval * tv) const;
  unsigned long getMsecValue(void) const;
  SbString format(const char * const fmt = "%S.%i") const;
  SbString formatDate(const char * const fmt = NULL) const;
  SbBool parsedate(const char * const date);
  friend COIN_DLL_API SbTime operator +(const SbTime & t0, const SbTime & t1);
  friend COIN_DLL_API SbTime operator -(const SbTime & t0, const SbTime & t1);
  SbTime & operator +=(const SbTime & tm);
  SbTime & operator -=(const SbTime & tm);
  SbTime operator-(void) const;
  friend COIN_DLL_API SbTime operator *(const double s, const SbTime & tm);
  friend COIN_DLL_API SbTime operator *(const SbTime & tm, const double s);
  friend COIN_DLL_API SbTime operator /(const SbTime & tm, const double s);
  SbTime & operator *=(const double s);
  SbTime & operator /=(const double s);
  double operator /(const SbTime & tm) const;
  SbTime operator %(const SbTime & tm) const;
  int operator ==(const SbTime & tm) const;
  int operator !=(const SbTime & tm) const;
  SbBool operator <(const SbTime & tm) const;
  SbBool operator >(const SbTime & tm) const;
  SbBool operator <=(const SbTime & tm) const;
  SbBool operator >=(const SbTime & tm) const;

  void print(FILE * fp) const;

private:
  double dtime;
  void addToString(SbString & str, const double val) const;
};

COIN_DLL_API SbTime operator +(const SbTime & t0, const SbTime & t1);
COIN_DLL_API SbTime operator -(const SbTime & t0, const SbTime & t1);
COIN_DLL_API SbTime operator *(const double s, const SbTime & tm);
COIN_DLL_API SbTime operator *(const SbTime & tm, const double s);
COIN_DLL_API SbTime operator /(const SbTime & tm, const double s);

// Avoid problem with Microsoft Win32 API headers (see above).
// Redefine macro max() back to a definition compatible with what it
// is in the MSVC header files.
#ifdef SBTIME_UNDEF_MAX
#define max(a,b) (((a) > (b)) ? (a) : (b))
#undef SBTIME_UNDEF_MAX
#endif // SBTIME_UNDEF_MAX

#endif // !COIN_SBTIME_H
