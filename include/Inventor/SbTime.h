/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SBTIME_H
#define COIN_SBTIME_H

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


class COIN_DLL_EXPORT SbTime {
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
#ifndef _WIN32
  static SbTime max(void);
#else // _WIN32
  static SbTime maxTime(void);
#endif // !_WIN32
  void setValue(const double sec);
  void setValue(const int32_t sec, const long usec);
  void setValue(const struct timeval * const tv);
  void setMsecValue(const unsigned long msec);
  double getValue(void) const;
  void getValue(int32_t& sec, long& usec) const;
  void getValue(struct timeval *tv) const;
  unsigned long getMsecValue(void) const;
  SbString format(const char * const fmt = "%S.%i") const;
  SbString formatDate(const char * const fmt = "%A, %D %r") const;
  SbBool parsedate(const char * const date);
  friend SbTime operator +(const SbTime& t0, const SbTime& t1);
  friend SbTime operator -(const SbTime& t0, const SbTime& t1);
  SbTime& operator +=(const SbTime& tm);
  SbTime& operator -=(const SbTime& tm);
  SbTime operator -(void) const;
  friend SbTime operator *(const double s, const SbTime& tm);
  friend SbTime operator *(const SbTime& tm, const double s);
  friend SbTime operator /(const SbTime& tm, const double s);
  SbTime& operator *=(const double s);
  SbTime& operator /=(const double s);
  double operator /(const SbTime& tm) const;
  SbTime operator %(const SbTime& tm) const;
  int operator ==(const SbTime& tm) const;
  int operator !=(const SbTime& tm) const;
  SbBool operator <(const SbTime& tm) const;
  SbBool operator >(const SbTime& tm) const;
  SbBool operator <=(const SbTime& tm) const;
  SbBool operator >=(const SbTime& tm) const;

  void print(FILE * fp) const;

private:
  double dtime;
  void addToString(SbString& str, const double val) const;
};

#endif // !COIN_SBTIME_H
