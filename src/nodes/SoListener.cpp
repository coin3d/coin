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

#include <Inventor/nodes/SoListener.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoAudioRenderAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoAudioDevice.h>

#include "../misc/AudioTools.h"

#ifdef HAVE_SOUND
#include <AL/al.h>
#include <AL/altypes.h>
#endif // HAVE_SOUND

SO_NODE_SOURCE(SoListener);

void SoListener::initClass()
{
  SO_NODE_INTERNAL_INIT_CLASS(SoListener, SO_FROM_COIN_2_0);
}

SoListener::SoListener()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoListener);
  SO_NODE_ADD_FIELD(position, (0.0f, 0.0f, 1.0f));
  SO_NODE_ADD_FIELD(orientation, 
                    (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(velocity, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(gain, (1.0f));
}

SoListener::~SoListener()
{
}

#ifdef HAVE_SOUND
inline void
SbVec3f2ALfloat3(ALfloat *dest, const SbVec3f &source)
{
  source.getValue(dest[0], dest[1], dest[2]);
}
#endif // HAVE_SOUND

void SoListener::audioRender(SoAudioRenderAction *action)
{
#ifdef HAVE_SOUND
  if (!SoAudioDevice::instance()->haveSound())
    return;
  ALint error;
  SbVec3f pos, worldpos;
  SbVec3f viewdir;
  SbVec3f viewup;
  ALfloat alfloat3[3];
  ALfloat alfloat6[6];

  pos = position.getValue();
  SoModelMatrixElement::get(action->getState()).multVecMatrix(pos, worldpos); 

#if COIN_DEBUG && 0
  // 20011206 thammer, kept for debugging purposes
//  float x, y, z;
//  worldpos.getValue(x, y, z);
//  printf("(%0.2f, %0.2f, %0.2f)\n", x, y, z);
#endif // debug

  SbVec3f2ALfloat3(alfloat3, worldpos);

  // Position ...
  alListenerfv(AL_POSITION, alfloat3);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoListener::audioRender",
                              "alListenerfv(AL_POSITION,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }

  // Velocity ...
  SbVec3f2ALfloat3(alfloat3, velocity.getValue());

  alListenerfv(AL_VELOCITY, alfloat3);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoListener::audioRender",
                              "alListenerfv(AL_VELOCITY,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }

  // Orientation ...
  this->orientation.getValue().multVec(SbVec3f(0,0,-1), viewdir);
  SbVec3f2ALfloat3(alfloat6, viewdir);

  this->orientation.getValue().multVec(SbVec3f(0,1,0), viewup);
  SbVec3f2ALfloat3(alfloat6+3, viewup);

  alListenerfv(AL_ORIENTATION,alfloat6);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoListener::audioRender",
                              "alListenerfv(AL_ORIENTATION,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }

  // Gain
  float gain = this->gain.getValue();
  gain = gain<0.0f ? 0.0f : ( gain>1.0f ? 1.0f : gain );  // clamp to [0.0f, 1.0f];
  alListenerf(AL_GAIN, gain);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoListener::audioRender",
                              "alListenerf(AL_GAIN,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }
#endif // HAVE_SOUND
}

