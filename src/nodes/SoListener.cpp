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
#endif

#include <Inventor/nodes/SoListener.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoAudioRenderAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoAudioDevice.h>
#include <Inventor/elements/SoListenerPositionElement.h>
#include <Inventor/elements/SoListenerOrientationElement.h>
#include <Inventor/elements/SoListenerVelocityElement.h>
#include <Inventor/elements/SoListenerGainElement.h>

SO_NODE_SOURCE(SoListener);

void SoListener::initClass()
{
  SO_NODE_INTERNAL_INIT_CLASS(SoListener, SO_FROM_COIN_2_0);

  SO_ENABLE(SoAudioRenderAction, SoListenerPositionElement);
  SO_ENABLE(SoAudioRenderAction, SoListenerOrientationElement);
  SO_ENABLE(SoAudioRenderAction, SoListenerVelocityElement);
  SO_ENABLE(SoAudioRenderAction, SoListenerGainElement);
}

SoListener::SoListener()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoListener);
  SO_NODE_ADD_FIELD(position, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(orientation, 
                    (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(velocity, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(gain, (1.0f));
}

SoListener::~SoListener()
{
}


/*!  
  Sets the doppler velocity relative to the global coordinate
  system. Not implemented yet.  
*/

void 
SoListener::setDopplerVelocity(float velocity)
{
  // FIXME: as of yet unimplemented. 2003-02-26 thammer.
  SoDebugError::postWarning("SoListener::setDopplerVelocity",
                            "Not yet implemented for Coin. "
                            "Get in touch if you need this functionality.");
}

/*!  
  Returns the doppler velocity relative to the global coordinate
  system. Not implemented yet.  
*/

float 
SoListener::getDopplerVelocity()
{
  // FIXME: as of yet unimplemented. 2003-02-26 thammer.
  SoDebugError::postWarning("SoListener::getDopplerVelocity",
                            "Not yet implemented for Coin. "
                            "Get in touch if you need this functionality.");
  return 0.0f;
}

/*!  
  Sets the doppler factor. Not implemented yet.  
*/

void 
SoListener::setDopplerFactor(float factor)
{
  // FIXME: as of yet unimplemented. 2003-02-26 thammer.
  SoDebugError::postWarning("SoListener::setDopplerFactor",
                            "Not yet implemented for Coin. "
                            "Get in touch if you need this functionality.");
}

/*!  
  Returns the doppler factor. Not implemented yet.  
*/

float 
SoListener::getDopplerFactor()
{
  // FIXME: as of yet unimplemented. 2003-02-26 thammer.
  SoDebugError::postWarning("SoListener::getDopplerFactor",
                            "Not yet implemented for Coin. "
                            "Get in touch if you need this functionality.");
  return 0.0f;
}

void
SoListener::audioRender(SoAudioRenderAction *action)
{
  SoState * state = action->getState();

  if (! this->position.isIgnored()) {
    SbVec3f pos, worldpos;
    pos = this->position.getValue();
    SoModelMatrixElement::get(state).multVecMatrix(pos, worldpos); 
    SoListenerPositionElement::set(state, this, worldpos, TRUE);
#if COIN_DEBUG && 0
    float x, y, z;
    worldpos.getValue(x, y, z);
    SoDebugError::postInfo("SoListener::audioRender", "listenerpos "
            "(%0.2f, %0.2f, %0.2f)", x, y, z);
#endif // debug
  }
  if (! this->orientation.isIgnored()) {
    SbVec3f t;
    SbRotation r;
    SbVec3f s;
    SbRotation so;
    SoModelMatrixElement::get(state).getTransform(t, r, s, so);
    r *= this->orientation.getValue();
    SoListenerOrientationElement::set(state, this, r, TRUE);
  }
  if (! this->velocity.isIgnored())
    SoListenerVelocityElement::set(state, this, this->velocity.getValue());
  if (! this->gain.isIgnored())
    SoListenerGainElement::set(state, this, this->gain.getValue());
}

