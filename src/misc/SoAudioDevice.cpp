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

#ifdef HAVE_SOUND
#include <AL/al.h>
#include <AL/alc.h>
#endif // HAVE_SOUND

class SoAudioDeviceP {
public:
  SoAudioDeviceP(SoAudioDevice * interfaceptr) : ifacep(interfaceptr) {};
  SoAudioDevice *ifacep;

  static void prerendercb(void * userdata, SoGLRenderAction * action);

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

#undef THIS
#define THIS this->soaudiodevice_impl

#undef ITHIS
#define ITHIS this->ifacep

SoAudioDevice::SoAudioDevice()
{
  THIS = new SoAudioDeviceP(this);

  THIS->context = NULL;
#ifdef HAVE_SOUND
  THIS->device = NULL;
#endif
  THIS->glRenderAction = NULL;
  THIS->audioRenderAction = NULL;
  THIS->enabled = FALSE;
  THIS->root = NULL;
  THIS->initOK = FALSE;

  THIS->audioRenderAction = new SoAudioRenderAction();
}

SoAudioDevice::~SoAudioDevice()
{
  this->disable();

  if (THIS->audioRenderAction != NULL)
    delete THIS->audioRenderAction;

#ifdef HAVE_SOUND
  alcDestroyContext((ALCcontext *)THIS->context);

  //Close device
  alcCloseDevice(THIS->device);
#endif // HAVE_SOUND

  delete THIS;
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

  THIS->device = alcOpenDevice((ALubyte*)devicename.getString());

  if (THIS->device == NULL) {
    SoDebugError::postWarning("SoAudioDevice::init",
                              "Failed to initialize OpenAL");
    return FALSE;
  }

  THIS->context=alcCreateContext(THIS->device,NULL);
  alcMakeContextCurrent((ALCcontext *)THIS->context);

  // Clear Error Code
  alGetError();

  THIS->initOK = TRUE;
  return TRUE;
#else
  return FALSE;
#endif // HAVE_SOUND
}
  
void SoAudioDevice::setSceneGraph(SoNode *root)
{
  THIS->root = root;
}

void SoAudioDevice::setGLRenderAction(SoGLRenderAction *ra)
{
  THIS->glRenderAction = ra;
}

SbBool SoAudioDevice::enable()
{
  if (THIS->enabled)
    return TRUE; // allready enabled

  assert( (THIS->initOK) 
          && (THIS->root != NULL) && (THIS->prerendercb != NULL) );
  if ( !((THIS->initOK) 
         && (THIS->root != NULL) && (THIS->prerendercb != NULL)) ) {
    return FALSE;
  }

  THIS->enabled = TRUE;

  if (THIS->glRenderAction)
    THIS->glRenderAction->addPreRenderCallback(THIS->prerendercb, THIS);
  return TRUE;
}

void SoAudioDevice::disable()
{
  if (!THIS->enabled)
    return; // allready disabled
  
  THIS->enabled = FALSE;

  if (THIS->glRenderAction)
    THIS->glRenderAction->removePreRenderCallback(THIS->prerendercb, THIS);
}

#ifndef DOXYGEN_SKIP_THIS

void SoAudioDeviceP::prerendercb(void * userdata, SoGLRenderAction * action)
{
  SoAudioDeviceP *thisp = (SoAudioDeviceP *) userdata;
  thisp->audioRenderAction->apply(thisp->root);
}

#endif // !DOXYGEN_SKIP_THIS
