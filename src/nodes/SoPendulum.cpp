/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoPendulum SoPendulum.h Inventor/nodes/SoPendulum.h
  \brief The SoPendulum class is used to create oscillating rotations.
  \ingroup nodes

  A smooth transition between rotation0 and rotation1 is created using
  a cosine function. In the beginning of the cycle, rotation0 is
  used. Halfway through the cycle, the resulting rotation equals
  rotation1, and at the end of the cycle, we're at rotation0 again.
*/

#include <Inventor/nodes/SoPendulum.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/engines/SoInterpolateRotation.h>

/*!
  \var SoSFRotation SoPendulum::rotation0
  The first rotation limit of the interpolation.
*/
/*!
  \var SoSFRotation SoPendulum::rotation1
  The other rotation limit of the interpolation.
*/
/*!
  \var SoSFFloat SoPendulum::speed
  Speed in cycles per second. Defaults to 1.
*/
/*!
  \var SoSFBool SoPendulum::on
  Toggles animation on or off. Defaults to being \c on.
*/

// *************************************************************************

SO_NODE_SOURCE(SoPendulum);

/*!
  Constructor.
*/
SoPendulum::SoPendulum(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPendulum);

  SO_NODE_ADD_FIELD(rotation0, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(rotation1, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(speed, (1.0f));
  SO_NODE_ADD_FIELD(on, (TRUE));

  this->interpolator = new SoInterpolateRotation;
  this->interpolator->ref();
  this->calculator = new SoCalculator;
  this->calculator->ref();
  this->timer = new SoElapsedTime;
  this->timer->ref();

  this->calculator->expression = "oa = (1.0 - cos(fmod(a*b*M_PI*2, M_PI*2))) * 0.5";
  this->calculator->a.connectFrom(&this->timer->timeOut);
  this->timer->on.connectFrom(&this->on);
  this->calculator->b.connectFrom(&this->speed);
  this->interpolator->input0.connectFrom(&this->rotation0);
  this->interpolator->input1.connectFrom(&this->rotation1);
  this->interpolator->alpha.connectFrom(&this->calculator->oa);
  this->rotation.connectFrom(&this->interpolator->output);
}

/*!
  Destructor.
*/
SoPendulum::~SoPendulum()
{
  this->interpolator->unref();
  this->calculator->unref();
  this->timer->unref();
}

// Doc from superclass.
void
SoPendulum::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPendulum, SO_FROM_INVENTOR_1);
}

// Documented in superclass.
void
SoPendulum::write(SoWriteAction * action)
{
  // Overridden to not write out internal engine connections.

  SoOutput * out = action->getOutput();

  // Decouple connections to/from internal engines to avoid them being
  // written. (Only done at first pass.)
  if (out->getStage() == SoOutput::COUNT_REFS)
    this->deconnectInternalEngine();

  inherited::write(action);

  // Reenable all connections to/from internal engine. (Only done at
  // last pass.)
  if (out->getStage() == SoOutput::WRITE)
    this->reconnectInternalEngine();
}

// FIXME: I _think_ we made a mistake when overriding SoNode::copy()
// and making it virtual. See FIXME-comment above
// SoBlinker::copy(). 20011220 mortene.

// Overridden to decouple and reconnect engine around copy operation.
SoNode *
SoPendulum::copy(SbBool copyconnections) const
{
  // Decouple connections to/from internal engines to avoid them being
  // copied.
  ((SoPendulum *)this)->deconnectInternalEngine();

  SoPendulum * cp = (SoPendulum *)inherited::copy(copyconnections);

  // Reenable all connections to/from internal engines.
  ((SoPendulum *)this)->reconnectInternalEngine();

  return cp;
}

// Remove connections to and from internal engine.
void
SoPendulum::deconnectInternalEngine(void)
{
  // Decouple connections to/from internal engine to avoid them being
  // written.
  this->timer->on.disconnect(&this->on);
  this->timer->on = FALSE;
  this->calculator->b.disconnect(&this->speed);
  this->interpolator->input0.disconnect(&this->rotation0);
  this->interpolator->input1.disconnect(&this->rotation1);
  this->rotation.disconnect(&this->interpolator->output);
}


// Reset connections to and from internal engine.
void
SoPendulum::reconnectInternalEngine(void)
{
  // Reenable all connections to/from internal engine.
  this->timer->on.connectFrom(&this->on);
  this->calculator->b.connectFrom(&this->speed);
  this->interpolator->input0.connectFrom(&this->rotation0);
  this->interpolator->input1.connectFrom(&this->rotation1);
  this->interpolator->alpha.connectFrom(&this->calculator->oa);
  this->rotation.connectFrom(&this->interpolator->output);
}
