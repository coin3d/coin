#ifndef COIN_SBTIME_H
#define COIN_SBTIME_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

// FIXME: the following system testing and conditional header file
// inclusion is a mess. Sort it out properly (with configure checks,
// probably). 20011019 mortene.

// Usually you get all you need from time.h
#include <time.h>
#ifdef _WIN32
#include <sys/timeb.h>

struct timeval;
#else
// Sometimes (linux) sys/time.h is also needed
#include <sys/time.h>
#endif // ! WIN32

#include <stdio.h>

#include <Inventor/system/inttypes.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>


class COIN_DLL_API SbTime {
public:
  SbTime(void);
  SbTime(const double sec);
  SbTime(const int32_t sec, const long usec);
  SbTime(const struct timeval * const tv);
  static SbTime getTimeOfDay(void);
  void setToTimeOfDay(void);
  static SbTime zero(void);

  // "max" is a #define somewhere in the Win32 include hierarchy mess.
  // Believe it or not. Is there no end to the stupidity?
#ifndef _WIN32 // FIXME: #ifdef'ing on system is bad design. 20011019 mortene.
  static SbTime max(void);
#endif // _WIN32

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
#ifndef _WIN32 // FIXME: #ifdef'ing on system is bad design. 20011019 mortene.
  SbString formatDate(const char * const fmt = "%A, %D %r") const;
#else // _WIN32
  SbString formatDate(const char * const fmt = "%#c") const;
#endif // _WIN32
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

#endif // !COIN_SBTIME_H
