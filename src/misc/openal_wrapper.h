#if 0 /* Just while I'm debugging. 2002-01-27 thammer. */
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

#ifndef COIN_OPENALWRAPPER_H
#define COIN_OPENALWRAPPER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#if HAVE_WINDOWS_H
#include <windows.h>
#endif /* HAVE_WINDOWS_H */

/* Under Win32, we need to make sure we use the correct calling method
   by using the APIENTRY define for the function signature types (or
   else we'll get weird stack errors). On other platforms, just define
   APIENTRY empty. */
#ifndef APIENTRY
#define APIENTRY
#endif /* !APIENTRY */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Typedefinitions of function signatures for openal calls we use. We
   need these for casting from the void-pointer return of dlsym().*/

  // ALAPI const ALubyte* ALAPIENTRY alGetString( ALenum param );
  typedef const unsigned char * (APIENTRY *alGetString_t)(int param);

  // ALAPI void ALAPIENTRY alListenerfv( ALenum pname, ALfloat* param ); 
  typedef void (APIENTRY *alListenerfv_t)(int pname, float *param);
  // ALAPI void ALAPIENTRY alListenerf( ALenum pname, ALfloat param );
  typedef void (APIENTRY *alListenerf_t)(int pname, float param);

  // ALAPI void ALAPIENTRY alDistanceModel( ALenum distanceModel );
  typedef void (APIENTRY *alDistanceModel_t)(int distanceModel);

  // ALCAPI ALCboolean ALCAPIENTRY alcMakeContextCurrent(ALCcontext *context);
  typedef int (APIENTRY *alcMakeContextCurrent_t)(void *context);
  // ALCAPI ALCvoid	  ALCAPIENTRY alcProcessContext(ALCcontext *context);
  typedef void (APIENTRY *alcProcessContext_t)(void *context);
  // ALCAPI ALCvoid	  ALCAPIENTRY alcSuspendContext(ALCcontext *context);
  typedef void (APIENTRY *alcSuspendContext_t)(void *context);
  // ALCAPI ALCvoid    ALCAPIENTRY alcDestroyContext(ALCcontext *context);
  typedef void (APIENTRY *alcDestroyContext_t)(void *context);

  // ALCAPI ALCdevice* ALCAPIENTRY alcOpenDevice(ALCubyte *deviceName);
  typedef void * (APIENTRY *alcOpenDevice_t)(unsigned char *deviceName);
  // ALCAPI ALCvoid    ALCAPIENTRY alcCloseDevice(ALCdevice *device);
  typedef void (APIENTRY *alcCloseDevice_t)(void *device);
  // Fixme: investigate if it's OK to have a different pointertype
  // from the one defined in the openal headers. 2002-01-27 thammer.


  typedef struct {
    /* Is the openal library at all available? */
    int available;

    alcMakeContextCurrent_t alcMakeContextCurrent;
    alcProcessContext_t alcProcessContext;
    alcSuspendContext_t alcSuspendContext;
    alcDestroyContext_t alcDestroyContext;
    alcOpenDevice_t alcOpenDevice;
    alcCloseDevice_t alcCloseDevice;
    alGetString_t alGetString;
    alListenerfv_t alListenerfv;
    alListenerf_t alListenerf;
    alDistanceModel_t alDistanceModel;

  } openal_wrapper_t;

  const openal_wrapper_t * openal_wrapper(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COIN_OPENALWRAPPER_H */

#endif
