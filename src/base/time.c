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
#include <Inventor/C/errors/debugerror.h>

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
  if (COIN_DEBUG && (result < 0)) {
    cc_debugerror_postwarning("SbTime_gettimeofday",
                              "Something went wrong (invalid timezone "
                              "setting?). Result is undefined.");
  }
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
cc_time_gettimeofday(void)
{
  cc_time t;
  if (cc_internal_queryperformancecounter(&t)) { return t; }
  if (cc_internal_gettimeofday(&t)) { return t; }
  if (cc_internal_ftime(&t)) { return t; }
  /* FIXME: write better debug output. 20011218 mortene. */
  cc_debugerror_post("cc_time_gettimeofday", "unable to find current time");
  return 0.0;
}

/* ********************************************************************** */
