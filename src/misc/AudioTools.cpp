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

#ifdef HAVE_SOUND

char *GetALErrorString(char *text, ALint errorcode)
{
  // text should be at least char[255]
  switch (errorcode) {
    case AL_INVALID_NAME:
      sprintf(text, "AL_INVALID_NAME - Illegal name passed as an argument to an AL call");
      break;
#ifdef _WIN32
    case AL_INVALID_ENUM:
#else
    case AL_ILLEGAL_ENUM:
#endif
      sprintf(text, "AL_INVALID_ENUM - Illegal enum passed as an argument to an AL call");
      break;
    case AL_INVALID_VALUE:
      sprintf(text, "AL_INVALID_VALUE - Illegal value passed as an argument to an AL call");
      break;
#ifdef _WIN32
    case AL_INVALID_OPERATION:
#else
    case AL_ILLEGAL_COMMAND:
#endif
      sprintf(text, "AL_INVALID_OPERATION - A function was called at an inappropriate time or in an inappropriate way, causing an illegal state. This can be an incompatible ALenum, object ID, and/or function");
      break;
    case AL_OUT_OF_MEMORY:
      sprintf(text, "AL_OUT_OF_MEMORY - A function could not be completed, because there is not enough memory available.");
      break;
    default:
      sprintf(text, "UNDEFINED ERROR");
      break;
  }
  return text;
}

#endif // HAVE_SOUND
