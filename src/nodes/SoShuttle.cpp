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
  \class SoShuttle SoShuttle.h Inventor/nodes/SoShuttle.h
  \brief The SoShuttle class is used to oscillate between two translations.
  \ingroup nodes

  A smooth transition between translation0 and translation1 is created
  using a cosine function. In the beginning of the cycle, translation0
  is used. Halfway through the cycle, the resulting translation equals
  translation1, and at the end of the cycle, we're at translation0
  again.
*/

#include <Inventor/nodes/SoShuttle.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/engines/SoInterpolateVec3f.h>


/*!
  \var SoSFVec3f SoShuttle::translation0

  Translation at the start and end of the cycle. Default value is (0,
  0, 0).
*/
/*!
  \var SoSFVec3f SoShuttle::translation1

  Translation at the middle of the cycle. Default value is (0, 0, 0).
*/
/*!
  \var SoSFFloat SoShuttle::speed
  Number of cycles per second. Default value is 1.
*/
/*!
  \var SoSFBool SoShuttle::on
  Toggles animation on or off. Defauls to \c TRUE.
*/

SO_NODE_SOURCE(SoShuttle);

/*!
  Constructor.
*/
SoShuttle::SoShuttle(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShuttle);

  SO_NODE_ADD_FIELD(translation0, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_NODE_ADD_FIELD(translation1, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_NODE_ADD_FIELD(speed, (1.0f));
  SO_NODE_ADD_FIELD(on, (TRUE));

  this->interpolator = new SoInterpolateVec3f;
  this->interpolator->ref();
  this->calculator = new SoCalculator;
  this->calculator->ref();
  this->timer = new SoElapsedTime;
  this->timer->ref();

  this->calculator->expression = "oa = (1.0 - cos(a*b*2*M_PI)) * 0.5";
  this->calculator->a.connectFrom(&this->timer->timeOut);
  this->timer->on.connectFrom(&this->on);
  this->calculator->b.connectFrom(&this->speed);
  this->interpolator->input0.connectFrom(&this->translation0);
  this->interpolator->input1.connectFrom(&this->translation1);
  this->interpolator->alpha.connectFrom(&this->calculator->oa);
  this->translation.connectFrom(&this->interpolator->output);
}

/*!
  Destructor.
*/
SoShuttle::~SoShuttle()
{
  this->interpolator->unref();
  this->calculator->unref();
  this->timer->unref();
}

// doc in parent
void
SoShuttle::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShuttle, SO_FROM_INVENTOR_1);
}

// Documented in superclass.
void
SoShuttle::write(SoWriteAction * action)
{
  // Overridden to not write out internal engine connections.

  SoOutput * out = action->getOutput();

  // Decouple connections to/from internal engines to avoid them being
  // written. (Only done at first pass.)
  if (out->getStage() == SoOutput::COUNT_REFS)
    this->deconnectInternalEngines();

  inherited::write(action);

  // Reenable all connections to/from internal engine. (Only done at
  // last pass.)
  if (out->getStage() == SoOutput::WRITE)
    this->reconnectInternalEngines();
}

// FIXME: I _think_ we made a mistake when overriding SoNode::copy()
// and making it virtual. See FIXME-comment above
// SoBlinker::copy(). 20011220 mortene.

// Overridden to decouple and reconnect engine around copy operation.
SoNode *
SoShuttle::copy(SbBool copyconnections) const
{
  // Decouple connections to/from internal engines to avoid them being
  // copied.
  ((SoShuttle *)this)->deconnectInternalEngines();

  SoShuttle * cp = (SoShuttle *)inherited::copy(copyconnections);

  // Reenable all connections to/from internal engines.
  ((SoShuttle *)this)->reconnectInternalEngines();

  return cp;
}

// Remove connections to and from internal engines.
void
SoShuttle::deconnectInternalEngines(void)
{
  // Decouple connections to/from internal engines to avoid them being
  // written.
  this->timer->on.disconnect(&this->on);
  this->timer->on = FALSE;
  this->calculator->b.disconnect(&this->speed);
  this->interpolator->input0.disconnect(&this->translation0);
  this->interpolator->input1.disconnect(&this->translation1);
  this->translation.disconnect(&this->interpolator->output);
}


// Reset connections to and from internal engines.
void
SoShuttle::reconnectInternalEngines(void)
{
  // Reenable all connections to/from internal engines.
  this->timer->on.connectFrom(&this->on);
  this->calculator->b.connectFrom(&this->speed);
  this->interpolator->input0.connectFrom(&this->translation0);
  this->interpolator->input1.connectFrom(&this->translation1);
  this->translation.connectFrom(&this->interpolator->output);
}
