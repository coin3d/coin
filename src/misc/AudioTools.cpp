/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include "AudioTools.h"
#include <stdio.h>
#include <assert.h>
#include <AL/altypes.h>
#include <Inventor/SbString.h>

// There are some differences in error defines between different
// versions of OpenAL, and we try to fix them up below.
//
// (It even looks like some of these defines have been set up
// exclusively for MSWindows usage, while others are for other
// platforms. Really great interface design there from the OpenAL
// developers...)
#ifndef AL_ILLEGAL_ENUM
#define AL_ILLEGAL_ENUM AL_INVALID_ENUM
#endif // !AL_ILLEGAL_ENUM
#ifndef AL_ILLEGAL_COMMAND
#define AL_ILLEGAL_COMMAND AL_INVALID_OPERATION
#endif // !AL_ILLEGAL_COMMAND


const char *
coin_get_openal_error(int errcode)
{
#ifndef HAVE_SOUND

  assert(FALSE && "coin_get_openal_error() invoked without sound support");
  return NULL;

#else // HAVE_SOUND

  switch ((ALint)errcode) {
    case AL_INVALID_NAME:
      return "AL_INVALID_NAME - Illegal name passed as an argument to an AL call";

    case AL_ILLEGAL_ENUM:
      return "AL_INVALID_ENUM - Illegal enum passed as an argument to an AL call";

    case AL_INVALID_VALUE:
      return "AL_INVALID_VALUE - Illegal value passed as an argument to an AL call";

    case AL_ILLEGAL_COMMAND:
      return "AL_INVALID_OPERATION - A function was called at an inappropriate time or in an inappropriate way, causing an illegal state. This can be an incompatible ALenum, object ID, and/or function";

    case AL_OUT_OF_MEMORY:
      return "AL_OUT_OF_MEMORY - A function could not be completed, because there is not enough memory available.";

    default:
      return "UNDEFINED ERROR";
  }

#endif // HAVE_SOUND
}
