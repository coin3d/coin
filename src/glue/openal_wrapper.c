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
#else /* No config.h? Hmm. Assume the openal library is not available for linking. */
#define OPENALWRAPPER_ASSUME_OPENAL 0
#endif /* !HAVE_CONFIG_H */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <Inventor/C/basic.h>
#include <Inventor/C/threads/threadsutilp.h>
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>

#ifdef HAVE_OPENAL /* In case we're _not_ doing runtime linking. */
#define OPENALWRAPPER_ASSUME_OPENAL 1
#endif /* HAVE_OPENAL */

#if OPENALWRAPPER_ASSUME_OPENAL
#if defined HAVE_AL_AL_H
#include <AL/al.h>
#include <AL/alc.h>
#elif defined HAVE_OPENAL_AL_H
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif 
#endif /* OPENALWRAPPER_ASSUME_OPENAL */

#include <Inventor/C/glue/openal_wrapper.h>

static openal_wrapper_t * openal_instance = NULL;
static cc_libhandle openal_libhandle = NULL;
static int openal_failed_to_load = 0;


/* Cleans up at exit. */
static void
openal_wrapper_cleanup(void)
{
#ifdef OPENAL_RUNTIME_LINKING
  if (openal_libhandle) { cc_dl_close(openal_libhandle); }
#endif /* OPENAL_RUNTIME_LINKING */

  assert(openal_instance);
  free(openal_instance);
}

/* Implemented by using the singleton pattern. */
const openal_wrapper_t *
openal_wrapper(void)
{
  CC_SYNC_BEGIN(openal_wrapper);

  if (!openal_instance && !openal_failed_to_load) {

    /* First invocation, do initializations. */
    openal_wrapper_t * oal = (openal_wrapper_t *)malloc(sizeof(openal_wrapper_t));
    /* FIXME: Verify that atexit works when Coin is used in an ActiveX
       dll also. 2003-01-27 thammer. */
    coin_atexit((coin_atexit_f *)openal_wrapper_cleanup, 0);

    /* Detect recursive calls. */
    {
      static int is_initializing = 0;
      assert(is_initializing == 0);
      is_initializing = 1;
    }

    /* The common case is that simage is either available from the
       linking process or we're successfully going to link it in. */
    oal->available = 1;

#ifdef OPENAL_RUNTIME_LINKING
    {
      const char * possiblelibnames[] = {
        "openal", "openal32", "libopenal.so",
        NULL
      };

      int idx = 0;
      while (!openal_libhandle && possiblelibnames[idx]) {
        openal_libhandle = cc_dl_open(possiblelibnames[idx]);
        idx++;
      }

      if (!openal_libhandle) {
        oal->available = 0;
        openal_failed_to_load = 1;
        cc_debugerror_post("openal_wrapper",
                           "Unable to load OpenAL");
        
      }
      oal->runtime = 1;
    }
    /* Define OPENALWRAPPER_REGISTER_FUNC macro. Casting the type is
       necessary for this file to be compatible with C++ compilers. */
#define OPENALWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    oal->_funcname_ = (_funcsig_)cc_dl_sym(openal_libhandle, SO__QUOTE(_funcname_))

#elif defined(OPENALWRAPPER_ASSUME_OPENAL) /* !OPENAL_RUNTIME_LINKING */

    oal->runtime = 0;
#define OPENALWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    oal->_funcname_ = (_funcsig_)_funcname_

#else /* !OPENALWRAPPER_ASSUME_OPENAL */
    oal->available = 0;
    oal->runtime = 0;
#define OPENALWRAPPER_REGISTER_FUNC(_funcname_, _funcsig_) \
    oal->_funcname_ = NULL

#endif /* !OPENALWRAPPER_ASSUME_OPENAL */

    OPENALWRAPPER_REGISTER_FUNC(alGetString, alGetString_t);

    if (oal->available && !oal->alGetString) {
      /* something is seriously wrong */
      cc_debugerror_post("openal_wrapper",
                         "Loaded OpenAL DLL ok, but couldn't resolve symbol "
                         "alGetString().");

      oal->available = 0;
      openal_failed_to_load = 1;
    }
    else {
      OPENALWRAPPER_REGISTER_FUNC(alGetString, alGetString_t);
      OPENALWRAPPER_REGISTER_FUNC(alGetError, alGetError_t);
      OPENALWRAPPER_REGISTER_FUNC(alListenerfv, alListenerfv_t);
      OPENALWRAPPER_REGISTER_FUNC(alListenerf, alListenerf_t);
      OPENALWRAPPER_REGISTER_FUNC(alDistanceModel, alDistanceModel_t);

      OPENALWRAPPER_REGISTER_FUNC(alcCreateContext, alcCreateContext_t);
      OPENALWRAPPER_REGISTER_FUNC(alcMakeContextCurrent, alcMakeContextCurrent_t);
      OPENALWRAPPER_REGISTER_FUNC(alcProcessContext, alcProcessContext_t);
      OPENALWRAPPER_REGISTER_FUNC(alcSuspendContext, alcSuspendContext_t);
      OPENALWRAPPER_REGISTER_FUNC(alcDestroyContext, alcDestroyContext_t);
      OPENALWRAPPER_REGISTER_FUNC(alcOpenDevice, alcOpenDevice_t);
      OPENALWRAPPER_REGISTER_FUNC(alcCloseDevice, alcCloseDevice_t);

      OPENALWRAPPER_REGISTER_FUNC(alGenSources, alGenSources_t);
      OPENALWRAPPER_REGISTER_FUNC(alDeleteSources, alDeleteSources_t);
      OPENALWRAPPER_REGISTER_FUNC(alSourcePlay, alSourcePlay_t);
      OPENALWRAPPER_REGISTER_FUNC(alSourceStop, alSourceStop_t);
      OPENALWRAPPER_REGISTER_FUNC(alSourceRewind, alSourceRewind_t);
      OPENALWRAPPER_REGISTER_FUNC(alSourcefv, alSourcefv_t);
      OPENALWRAPPER_REGISTER_FUNC(alSourcef, alSourcef_t);
      OPENALWRAPPER_REGISTER_FUNC(alSourcei, alSourcei_t);
      OPENALWRAPPER_REGISTER_FUNC(alGetSourcei, alGetSourcei_t);
      OPENALWRAPPER_REGISTER_FUNC(alSourceQueueBuffers, alSourceQueueBuffers_t);
      OPENALWRAPPER_REGISTER_FUNC(alSourceUnqueueBuffers, alSourceUnqueueBuffers_t);
      OPENALWRAPPER_REGISTER_FUNC(alBufferData, alBufferData_t);
      OPENALWRAPPER_REGISTER_FUNC(alGenBuffers, alGenBuffers_t);
      OPENALWRAPPER_REGISTER_FUNC(alDeleteBuffers, alDeleteBuffers_t);
    }

    /* Do this late, so we can detect recursive calls to this function. */
    openal_instance = oal;
  }
  CC_SYNC_END(openal_wrapper);
  return openal_instance;
}
