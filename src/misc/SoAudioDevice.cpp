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

/*!
  \class SoAudioDevice Inventor/misc/SoAudioDevice.h
  \brief The SoAudioDevice class is used to control an audio device.
  \ingroup general

  The SoAudioDevice class is responsible for initialization of an 
  audio device, as well as enabling and disabling sound. It is a singleton
  class.

  The application programmer does not need to use this class directly, as
  audio support is enabled by default, and the default settings are 
  reasonable.

  Coin uses OpenAL (http://www.openal.org/,
  http://developer.soundblaster.com [Games section]) to render audio.
  OpenAL should work with any soundcard, and on most modern operating
  systems (including Unix, Linux, IRIX, *BSD, Mac OS X and Microsoft
  Windows). 2 speaker output is allways available, and on some OS and
  soundcard combinations, more advanced speaker configurations are
  supported. On Microsoft Windows, OpenAL can use DirectSound3D to
  render audio, thus supporting any speaker configuration the current
  DirectSound3D driver supports. Configuring speakers are done through
  the soundcard driver, and is transparent to both Coin and OpenAL.  
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <Inventor/misc/SoAudioDevice.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/tidbits.h>

#include <Inventor/SbBasic.h>
#include "AudioTools.h"

#ifdef HAVE_SOUND
#include <../glue/openal_wrapper.h>
#endif // HAVE_SOUND

class SoAudioDeviceP {
public:
  SoAudioDeviceP(SoAudioDevice * master) : master(master) {};
  SoAudioDevice *master;

  static SoAudioDevice *singleton;

  static SoAudioDevice *private_instance();

  void *context;
  void *device;

  SbBool enabled;
  SbBool initOK;
  float lastGain;

};

#undef PRIVATE
#define PRIVATE(p) ((p)->pimpl)
#undef PUBLIC
#define PUBLIC(p) ((p)->master)

SoAudioDevice *SoAudioDeviceP::singleton = NULL;

/*!
  Returns a pointer to the SoAudioDevice class, which is a singleton.
 */

SoAudioDevice *
SoAudioDevice::instance()
{
  if (SoAudioDeviceP::singleton == NULL) {
    SoAudioDeviceP::singleton = new SoAudioDevice();
  }
  return SoAudioDeviceP::singleton;
}

/*!
  Constructor
 */

SoAudioDevice::SoAudioDevice()
{
  PRIVATE(this) = new SoAudioDeviceP(this);
  PRIVATE(this)->context = NULL;
  PRIVATE(this)->device = NULL;
  PRIVATE(this)->enabled = FALSE;
  PRIVATE(this)->initOK = FALSE;
  PRIVATE(this)->lastGain = 1.0f;
}

/*!
  Destructor
 */

SoAudioDevice::~SoAudioDevice()
{
  this->cleanup();

  delete PRIVATE(this);
}

/*!
  Initializes the audio device. Currently, the only supported \a devicetype
  is "OpenAL". The supported \a devicename depends on the OS and on installed
  soundcards and drivers. On Microsoft Windows, supported device names are
  "DirectSound3D", "DirectSound", and "MMSYSTEM". See OpenAL documentation
  (available from http://www.openal.org/) for further information.

  This function is called by SoDB::init(). The user can control which
  \a devicename SoDB::init() uses by setting the COIN_SOUND_DRIVER_NAME
  environment variable. On Microsoft Windows, the default driver name
  is "DirectSound3D", which should normally be what the user wants.
 */

SbBool SoAudioDevice::init(const SbString &devicetype, 
                           const SbString &devicename)
{
#ifdef HAVE_SOUND
  const char * env;
  env = coin_getenv("COIN_SOUND_DISABLE");
  if (env && atoi(env)) {
    if (coin_debug_audio()) {
      SoDebugError::postInfo("SoAudioDevice::init", 
                             "Sound has been disabled because the "
                             "environment variable COIN_SOUND_DISABLE=1");
    }
    return FALSE;
  }

  if (devicetype != "OpenAL") {
    SoDebugError::postWarning("SoAudioDevice::init",
                              "devicetype != OpenAL - currently OpenAL is "
                              "the only supported device type for audio "
                              "rendering");
    return FALSE;
  }

  if (!openal_wrapper()->available) {
    SoDebugError::postWarning("SoAudioDevice::init", 
                           "OpenAL linking failed. "
                           "Attempted to use %s linking. "
                           "Sound will not be available.", 
                           openal_wrapper()->runtime ? "run-time" : 
                           "link-time");
    if (openal_wrapper()->runtime) {
      SoDebugError::postInfo("SoAudioDevice::init",
                             "To get more debug information, "
                             "set the environment variable "
                             "COIN_DEBUG_DL=1 and run the "
                             "application again");
    }
    PRIVATE(this)->enabled = FALSE;
    PRIVATE(this)->initOK = FALSE;
    return FALSE;
  }

  this->cleanup();

  PRIVATE(this)->device = 
    openal_wrapper()->alcOpenDevice((unsigned char*)devicename.getString());

  if (PRIVATE(this)->device == NULL) {
    SoDebugError::postWarning("SoAudioDevice::init",
                              "Failed to initialize OpenAL. "
                              "Sound will not be available.");
    return FALSE;
  }

  if (coin_debug_audio()) {
    SoDebugError::postInfo("SoAudioDevice::init", 
                           "OpenAL linking OK. Using %s linking.", 
                           openal_wrapper()->runtime ? "run-time" :
                           "link-time");
    const unsigned char * str;
    str = openal_wrapper()->alGetString(AL_VENDOR);
    SoDebugError::postInfo("SoAudioDevice::init", "AL_VENDOR=='%s'", str);
    str = openal_wrapper()->alGetString(AL_VERSION);
    SoDebugError::postInfo("SoAudioDevice::init", "AL_VERSION=='%s'", str);
    str = openal_wrapper()->alGetString(AL_RENDERER);
    SoDebugError::postInfo("SoAudioDevice::init", "AL_RENDERER=='%s'", str);
    str = openal_wrapper()->alGetString(AL_EXTENSIONS);
    SoDebugError::postInfo("SoAudioDevice::init", "AL_EXTENSIONS=='%s'", str);
  }
  // FIXME: the version string should be checked against the minimum
  // version we demand. A standard "Debian testing" distribution as of
  // now comes with version 0.0.6, for instance, and that one has
  // problems with thammer's code. It is unknown if the problems are
  // caused by our code, or by old bugs in OpenAL, though.
  //
  // const ALubyte * str = alGetString(AL_VERSION);
  //
  // 20021029 mortene.

  PRIVATE(this)->context = openal_wrapper()->alcCreateContext(PRIVATE(this)->device, NULL);
  openal_wrapper()->alcMakeContextCurrent(PRIVATE(this)->context);

  // Clear Error Code
  openal_wrapper()->alGetError();

  // Set listener parameters (position, orientation, velocity, gain)
  // These will never change, since we're simulating listener movement by
  // moving sounds instead of moving the listener. 2002-11-13 thammer.
  int error;
  float alfloat3[3] = { 0.0f, 0.0f, 0.0f };
  float alfloat6[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
  float gain = 1.0f;

  openal_wrapper()->alListenerfv(AL_POSITION, alfloat3);
  if ((error = openal_wrapper()->alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoAudioDevice::init",
                              "alListenerfv(AL_POSITION,) failed. %s."
                              "Sound will not be available.",
                              coin_get_openal_error(error));
    return FALSE;
  }

  openal_wrapper()->alListenerfv(AL_VELOCITY, alfloat3);
  if ((error = openal_wrapper()->alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoAudioDevice::init",
                              "alListenerfv(AL_VELOCITY,) failed. %s."
                              "Sound will not be available.",
                              coin_get_openal_error(error));
    return FALSE;
  }
  
  openal_wrapper()->alListenerfv(AL_ORIENTATION, alfloat6);
  if ((error = openal_wrapper()->alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoAudioDevice::init",
                              "alListenerfv(AL_ORIENTATION,) failed. %s."
                              "Sound will not be available.",
                              coin_get_openal_error(error));
    return FALSE;
  }

  openal_wrapper()->alListenerf(AL_GAIN, gain);
  if ((error = openal_wrapper()->alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoAudioDevice::init",
                              "alListenerf(AL_GAIN,) failed. %s."
                              "Sound will not be available.",
                              coin_get_openal_error(error));
    return FALSE;
  }

  openal_wrapper()->alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

  PRIVATE(this)->enabled = TRUE;
  PRIVATE(this)->initOK = TRUE;
#endif // HAVE_SOUND
  if (coin_debug_audio() && PRIVATE(this)->initOK) {
    SoDebugError::postInfo("SoAudioDevice::init",
                              "Initialization succeeded");
  }
  return PRIVATE(this)->initOK;
}

/*!
  Cleans up any allocated resources.
 */

void SoAudioDevice::cleanup()
{
  if (coin_debug_audio()) {
    SoDebugError::postInfo("SoAudioDevice::cleanup", "Closing audio device");
  }

  if (!this->haveSound())
    return;

  this->disable();

#ifdef HAVE_SOUND
  if (PRIVATE(this)->context != NULL)
    openal_wrapper()->alcDestroyContext(PRIVATE(this)->context);
  PRIVATE(this)->context = NULL;

  //Close device
  if (PRIVATE(this)->device != NULL)
    openal_wrapper()->alcCloseDevice(PRIVATE(this)->device);
  PRIVATE(this)->device = NULL;
#endif // HAVE_SOUND

  PRIVATE(this)->initOK = FALSE;
}
  
/*!
  returns true if the audio device has been initialized successfully.
 */

SbBool SoAudioDevice::haveSound()
{
  return PRIVATE(this)->initOK;
}

/*!
  Enables sound
 */

SbBool SoAudioDevice::enable()
{
  if (!this->haveSound())
    return FALSE;

  if (PRIVATE(this)->enabled)
    return TRUE; // allready enabled

  PRIVATE(this)->enabled = TRUE;

#ifdef HAVE_SOUND
  openal_wrapper()->alcProcessContext(PRIVATE(this)->context);
#endif

  return TRUE;
}

/*!
  Disables sound. Effectively silencing all audio output.
 */

void SoAudioDevice::disable()
{
  if (!this->haveSound())
    return;

  if (!PRIVATE(this)->enabled)
    return; // allready disabled
  
  PRIVATE(this)->enabled = FALSE;

#ifdef HAVE_SOUND
  openal_wrapper()->alcSuspendContext(PRIVATE(this)->context);
#endif
}

/*!
  Returns TRUE if audio is enabled.
 */

SbBool 
SoAudioDevice::isEnabled()
{
  return PRIVATE(this)->enabled;
}

void 
SoAudioDevice::setGain(float gain)
{
  if (!this->haveSound())
    return;

  gain = (gain < 0.0f) ? 0.0f : gain;
#ifdef HAVE_SOUND
  int error;
  openal_wrapper()->alListenerf(AL_GAIN, gain);
  if ((error = openal_wrapper()->alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoAudioDevice::setGain",
                              "alListenerf(AL_GAIN,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }
#endif // HAVE_SOUND
  PRIVATE(this)->lastGain = gain;
}

void 
SoAudioDevice::mute(SbBool mute)
{
  if (mute) {
    float lastgain = PRIVATE(this)->lastGain;
    this->setGain(0.0f);
    PRIVATE(this)->lastGain = lastgain;
  } else {
    this->setGain(PRIVATE(this)->lastGain);
  }
}

