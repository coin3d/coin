/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

// Hack used to circumvent the fact that the API interface for the public
// class SoNormalCache does not supply the number of coordinates as an argument

#include <assert.h>
#include <Inventor/C/tidbitsp.h>
#include <Inventor/SbDict.h>
#include <Inventor/C/threads/threadsutilp.h>
#include "normalcache_numcoords_hack.h"

#if COIN_MINOR_VERSION > 3
// Catch bump to 2.4.0, as there's a clean-up task to do:
//
// Get rid of the below hack by fixing the SoNormalCache interface
// (overload generate*() functions, but with an extra "numcoords"
// argument, obsolete the current generate*() functions).
#error clean up interface for 2.4 (and 3.0)
#endif

static SbDict * numcoordshash_hack = NULL;

static void
numcoords_hack_cleanup(void)
{
  delete numcoordshash_hack;
}

void
normalcache_set_num_coords_hack(const SoNormalCache * n, unsigned int nr)
{
  CC_GLOBAL_LOCK;
  if (!numcoordshash_hack) {
    numcoordshash_hack = new SbDict;
    coin_atexit((coin_atexit_f*) numcoords_hack_cleanup, 0);
  }
  numcoordshash_hack->enter((unsigned long)n, (void *)nr);
  CC_GLOBAL_UNLOCK;
}

unsigned int
normalcache_get_num_coords_hack(const SoNormalCache * n)
{
  assert(numcoordshash_hack);
  void * val;
  CC_GLOBAL_LOCK;
  const SbBool ok = numcoordshash_hack->find((unsigned long)n, val);
  assert(ok);
  CC_GLOBAL_UNLOCK;

  return (unsigned int)val;
}
