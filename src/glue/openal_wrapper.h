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

  typedef const unsigned char * (APIENTRY *alGetString_t)(int param);
  typedef int (APIENTRY *alGetError_t)(void);

  typedef void (APIENTRY *alListenerfv_t)(int pname, float *param);
  typedef void (APIENTRY *alListenerf_t)(int pname, float param);

  typedef void (APIENTRY *alDistanceModel_t)(int distanceModel);

  typedef void (APIENTRY *alGenSources_t)(int n, unsigned int *sources);
  typedef void (APIENTRY *alDeleteSources_t)(int n, unsigned int *sources);
  typedef void (APIENTRY *alSourcePlay_t)(unsigned int source);
  typedef void (APIENTRY *alSourceStop_t)(unsigned int source);
  typedef void (APIENTRY *alSourceRewind_t)(unsigned int source);
  typedef void (APIENTRY *alSourcefv_t)(unsigned int source, int param, 
                                        float *values);
  typedef void (APIENTRY *alSourcef_t)(unsigned int source, int param, 
                                       float value);
  typedef void (APIENTRY *alSourcei_t)(unsigned int source, int param, 
                                       int value);
  typedef void (APIENTRY *alGetSourcei_t)(unsigned int source, int param, 
                                          int *value);
  typedef void (APIENTRY *alSourceQueueBuffers_t)(unsigned int source, 
                                                  unsigned int n, 
                                                  unsigned int *buffers);
  typedef void (APIENTRY *alSourceUnqueueBuffers_t)(unsigned int source, 
                                                  unsigned int n, 
                                                  unsigned int *buffers);

  typedef void (APIENTRY *alBufferData_t)(unsigned int buffer, int format,
                                       void *data, unsigned int size,
                                       unsigned int freq);
  typedef void (APIENTRY *alGenBuffers_t)(int n, unsigned int *buffers);
  typedef void (APIENTRY *alDeleteBuffers_t)(int n, unsigned int *buffers);

  typedef void * (APIENTRY *alcCreateContext_t)(void *device, int *attrlist);
  typedef int (APIENTRY *alcMakeContextCurrent_t)(void *context);
  typedef void (APIENTRY *alcProcessContext_t)(void *context);
  typedef void (APIENTRY *alcSuspendContext_t)(void *context);
  typedef void (APIENTRY *alcDestroyContext_t)(void *context);

  typedef void * (APIENTRY *alcOpenDevice_t)(unsigned char *deviceName);
  typedef void (APIENTRY *alcCloseDevice_t)(void *device);
  /* Fixme: investigate if it's OK to have a different pointertype
     from the one defined in the openal headers. 2002-01-27
     thammer. */

#if !OPENALWRAPPER_ASSUME_OPENAL
  enum {
    AL_NO_ERROR=0x0000,
    AL_NONE=0x0000,
    AL_PITCH=0x1003,
    AL_POSITION=0x1004,
    AL_VELOCITY=0x1006,
    AL_LOOPING=0x1007,
    AL_GAIN=0x100a,
    AL_MIN_GAIN=0x100d,
    AL_MAX_GAIN=0x100e,
    AL_ORIENTATION=0x100f,
    AL_BUFFER=0x1009,
    AL_SOURCE_STATE=0x1010,
    AL_INITIAL=0x1011,
    AL_PLAYING=0x1012,
    AL_PAUSED=0x1013,
    AL_STOPPED=0x1014,
    AL_BUFFERS_QUEUED=0x1015,
    AL_BUFFERS_PROCESSED=0x1016,
    AL_ROLLOFF_FACTOR=0x1021,
    AL_FORMAT_MONO8=0x1100,
    AL_FORMAT_MONO16=0x1101,
    AL_FORMAT_STEREO8=0x1102,
    AL_FORMAT_STEREO16=0x1103,
    AL_INVALID_NAME=0xa001,
    AL_INVALID_ENUM=0xa002,
    AL_INVALID_VALUE=0xa003,
    AL_INVALID_OPERATION=0xa004,
    AL_OUT_OF_MEMORY=0xa005,
    AL_VENDOR=0xb001,
    AL_VERSION=0xb002,
    AL_RENDERER=0xb003,
    AL_EXTENSIONS=0xb004,
    AL_INVERSE_DISTANCE_CLAMPED=0xd002
  };
#endif /* !OPENALWRAPPER_ASSUME_OPENAL */

  typedef struct {
    int available;
    int runtime;

    alGetString_t alGetString;
    alGetError_t alGetError;
    alListenerfv_t alListenerfv;
    alListenerf_t alListenerf;
    alDistanceModel_t alDistanceModel;
    alGenSources_t alGenSources;
    alDeleteSources_t alDeleteSources;
    alSourcePlay_t alSourcePlay;
    alSourceStop_t alSourceStop;
    alSourceRewind_t alSourceRewind;
    alSourcefv_t alSourcefv;
    alSourcef_t alSourcef;
    alSourcei_t alSourcei;
    alGetSourcei_t alGetSourcei;
    alSourceQueueBuffers_t alSourceQueueBuffers;
    alSourceUnqueueBuffers_t alSourceUnqueueBuffers;
    alBufferData_t alBufferData;
    alGenBuffers_t alGenBuffers;
    alDeleteBuffers_t alDeleteBuffers;

    alcCreateContext_t alcCreateContext;
    alcMakeContextCurrent_t alcMakeContextCurrent;
    alcProcessContext_t alcProcessContext;
    alcSuspendContext_t alcSuspendContext;
    alcDestroyContext_t alcDestroyContext;
    alcOpenDevice_t alcOpenDevice;
    alcCloseDevice_t alcCloseDevice;

  } openal_wrapper_t;

  const openal_wrapper_t * openal_wrapper(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COIN_OPENALWRAPPER_H */
