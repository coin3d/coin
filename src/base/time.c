/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif /* HAVE_WINDOWS_H */

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h> /* gettimeofday() */
#endif /* HAVE_SYS_TIME_H */

/* On Mac OS X / Darwin, timeb.h uses time_t from time.h, so the order
   of these two includes needs to be preserved. */
#ifdef HAVE_SYS_TIMEB_H
#include <sys/timeb.h> /* struct _timeb */
#endif /* HAVE_SYS_TIMEB_H */

#ifdef HAVE_UNISTD_H
#include <unistd.h> /* gettimeofday() */
#endif /* HAVE_UNISTD_H */

#include <assert.h>

#include <Inventor/C/base/time.h>

/* ********************************************************************** */

#ifdef HAVE_QUERYPERFORMANCECOUNTER
static int highperf_available = -1;
static double highperf_start = -1;
static double highperf_tick = -1;
#endif /* HAVE_QUERYPERFORMANCECOUNTER */

/* The Win32 QueryPerformanceCounter() strategy is based on code
   submitted by Jan Peciva (aka PCJohn). */
static SbBool
cc_internal_queryperformancecounter(cc_time * t)
{
#ifdef HAVE_QUERYPERFORMANCECOUNTER
  if (highperf_available == -1) {
    LARGE_INTEGER frequency;
    highperf_available = (QueryPerformanceFrequency(&frequency) != 0);
    if (highperf_available) {
      highperf_tick = 1.0 / frequency.QuadPart;

      {
        time_t tt = time(NULL);
        LARGE_INTEGER counter;
        (void)QueryPerformanceCounter(&counter);
        highperf_start = tt - ((double)counter.QuadPart * highperf_tick);
      }
    }
  }

  if (highperf_available) {
    LARGE_INTEGER counter;
    BOOL b = QueryPerformanceCounter(&counter);
    assert(b && "QueryPerformanceCounter() failed even though QueryPerformanceFrequency() worked");
    *t = (double)counter.QuadPart * highperf_tick + highperf_start;
    return TRUE;
  }
  return FALSE;
#else /* !HAVE_QUERYPERFORMANCECOUNTER */
  return FALSE;
#endif /* !HAVE_QUERYPERFORMANCECOUNTER */
}

static SbBool
cc_internal_gettimeofday(cc_time * t)
{
#ifdef HAVE_GETTIMEOFDAY
  struct timeval tv;
  int result = gettimeofday(&tv, NULL);
#if 0 /* FIXME: tmp disabled, SoDebugError not yet ported to C. 20011218 mortene. */
  if (COIN_DEBUG && (result < 0)) {
    SoDebugError::postWarning("SbTime_gettimeofday",
                              "Something went wrong (invalid timezone "
                              "setting?). Result is undefined.");
  }
#endif /* tmp disabled */
  *t = tv.tv_sec;
  *t += ((double)(tv.tv_usec))/1000000.0;
  return TRUE;
#else /* !HAVE_GETTIMEOFDAY */
  return FALSE;
#endif /* !HAVE_GETTIMEOFDAY */
}

static SbBool
cc_internal_ftime(cc_time * t)
{
#ifdef HAVE__FTIME
  struct _timeb timebuffer;
  _ftime(&timebuffer);
  /* FIXME: should use timezone field of struct _timeb aswell. 20011023 mortene. */
  *t = (double)timebuffer.time + (double)timebuffer.millitm / 1000.0;
  return TRUE;
#else /* !HAVE__FTIME */
  return FALSE;
#endif /* !HAVE__FTIME */
}

/* ********************************************************************** */

cc_time
cc_time_interval(int secs, int msecs)
{
  return (cc_time) secs + ((cc_time) msecs / (cc_time) 1000000);
} /* cc_time_interval() */

/* ********************************************************************** */

/* FIXME: doc available in SbTime::getTimeOfDay(). 20011218 mortene. */
cc_time
cc_time_gettimeofday(void)
{
  cc_time t;
  if (cc_internal_queryperformancecounter(&t)) { return t; }
  if (cc_internal_gettimeofday(&t)) { return t; }
  if (cc_internal_ftime(&t)) { return t; }
#if 0 /* FIXME: waiting for SoDebugError ported to C. 20011218 mortene. */
  /* FIXME: write better debug output. 20011218 mortene. */
  SoDebugError::post("cc_time_gettimeofday", "unable to find current time");
#endif /* tmp disabled */
  return 0.0;
}

/* ********************************************************************** */
