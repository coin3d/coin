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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <Inventor/misc/SoAudioDevice.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/SbBasic.h>
#include <../tidbits.h>

#ifdef HAVE_SOUND
#include <AL/al.h>
#include <AL/alc.h>
#endif // HAVE_SOUND

class SoAudioDeviceP {
public:
  SoAudioDeviceP(SoAudioDevice * master) : master(master) {};
  SoAudioDevice *master;

  static SoAudioDevice *singleton;

  static void prerendercb(void * userdata, SoGLRenderAction * action);
  
  static SoAudioDevice *private_instance();

  void *context;
#ifdef HAVE_SOUND
  ALCdevice *device;
#endif // HAVE_SOUND
  SoGLRenderAction *glRenderAction;
  SoAudioRenderAction *audioRenderAction;
  SoNode *root;

  SbBool enabled;
  SbBool initOK;
};

#undef PRIVATE
#define PRIVATE(p) ((p)->pimpl)
#undef PUBLIC
#define PUBLIC(p) ((p)->master)

SoAudioDevice *SoAudioDeviceP::singleton = NULL;

SoAudioDevice *
SoAudioDevice::instance()
{
  if (SoAudioDeviceP::singleton == NULL) {
    SoAudioDeviceP::singleton = new SoAudioDevice();
  }
  return SoAudioDeviceP::singleton;
}

SoAudioDevice::SoAudioDevice()
{
  PRIVATE(this) = new SoAudioDeviceP(this);
  PRIVATE(this)->context = NULL;
#ifdef HAVE_SOUND
  PRIVATE(this)->device = NULL;
#endif
  PRIVATE(this)->glRenderAction = NULL;
  PRIVATE(this)->audioRenderAction = NULL;
  PRIVATE(this)->enabled = FALSE;
  PRIVATE(this)->root = NULL;
  PRIVATE(this)->initOK = FALSE;

  PRIVATE(this)->audioRenderAction = new SoAudioRenderAction();
}

SoAudioDevice::~SoAudioDevice()
{
  this->cleanup();

  delete PRIVATE(this);
}

/* Return value of COIN_DEBUG_AUDIO environment variable. */
static int
coin_debug_audio(void)
{
  static int d = -1;
  if (d == -1) {
    const char * val = coin_getenv("COIN_DEBUG_AUDIO");
    d = val ? atoi(val) : 0;
  }
  return (d > 0) ? 1 : 0;
}


SbBool SoAudioDevice::init(const SbString &devicetype, 
                           const SbString &devicename)
{
#ifdef HAVE_SOUND
  if (devicetype != "OpenAL") {
    SoDebugError::postWarning("SoAudioDevice::init",
                              "devicetype != OpenAL - currently OpenAL is "
                              "the only supported device type for audio "
                              "rendering");
    return FALSE;
  }

  PRIVATE(this)->device = alcOpenDevice((ALubyte*)devicename.getString());

  if (PRIVATE(this)->device == NULL) {
    SoDebugError::postWarning("SoAudioDevice::init",
                              "Failed to initialize OpenAL");
    return FALSE;
  }

  if (coin_debug_audio()) {
    const ALubyte * str = alGetString(AL_VENDOR);
    SoDebugError::postInfo("SoAudioDevice::init", "AL_VENDOR=='%s'", str);
    str = alGetString(AL_VERSION);
    SoDebugError::postInfo("SoAudioDevice::init", "AL_VERSION=='%s'", str);
    str = alGetString(AL_RENDERER);
    SoDebugError::postInfo("SoAudioDevice::init", "AL_RENDERER=='%s'", str);
    str = alGetString(AL_EXTENSIONS);
    SoDebugError::postInfo("SoAudioDevice::init", "AL_EXTENSIONS=='%s'", str);
  }

  PRIVATE(this)->context = alcCreateContext(PRIVATE(this)->device,NULL);
  alcMakeContextCurrent((ALCcontext *)PRIVATE(this)->context);

  // Clear Error Code
  alGetError();

  PRIVATE(this)->initOK = TRUE;
#endif // HAVE_SOUND
  return PRIVATE(this)->initOK;
}

void SoAudioDevice::cleanup()
{
  this->disable();

  if (PRIVATE(this)->audioRenderAction != NULL)
    delete PRIVATE(this)->audioRenderAction;
  PRIVATE(this)->audioRenderAction = NULL;

#ifdef HAVE_SOUND
  if (PRIVATE(this)->context != NULL)
    alcDestroyContext((ALCcontext *)PRIVATE(this)->context);
  PRIVATE(this)->context = NULL;

  //Close device
  if (PRIVATE(this)->device != NULL)
    alcCloseDevice(PRIVATE(this)->device);
  PRIVATE(this)->device = NULL;
#endif // HAVE_SOUND

}
  
SbBool SoAudioDevice::haveSound()
{
  return PRIVATE(this)->initOK;
}

void SoAudioDevice::setSceneGraph(SoNode *root)
{
  PRIVATE(this)->root = root;
}

void SoAudioDevice::setGLRenderAction(SoGLRenderAction *ra)
{
  PRIVATE(this)->glRenderAction = ra;
}

SbBool SoAudioDevice::enable()
{
  if (PRIVATE(this)->enabled)
    return TRUE; // allready enabled

  assert( (PRIVATE(this)->initOK) 
          && (PRIVATE(this)->root != NULL) && (PRIVATE(this)->prerendercb != NULL) );
  if ( !((PRIVATE(this)->initOK) 
         && (PRIVATE(this)->root != NULL) && (PRIVATE(this)->prerendercb != NULL)) ) {
    return FALSE;
  }

  PRIVATE(this)->enabled = TRUE;

  if (PRIVATE(this)->glRenderAction)
    PRIVATE(this)->glRenderAction->addPreRenderCallback(PRIVATE(this)->prerendercb, PRIVATE(this));
  return TRUE;
}

void SoAudioDevice::disable()
{
  if (!PRIVATE(this)->enabled)
    return; // allready disabled
  
  PRIVATE(this)->enabled = FALSE;

  if (PRIVATE(this)->glRenderAction)
    PRIVATE(this)->glRenderAction->removePreRenderCallback(PRIVATE(this)->prerendercb, PRIVATE(this));
}

void SoAudioDeviceP::prerendercb(void * userdata, SoGLRenderAction * action)
{
  SoAudioDeviceP *thisp = (SoAudioDeviceP *) userdata;
  thisp->audioRenderAction->apply(thisp->root);
}

