/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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

  this->calculator->expression = "oa = (1.0 - cos(fmod(a*b*M_PI*2, M_PI*2))) * 0.5";
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
  SO_NODE_INTERNAL_INIT_CLASS(SoShuttle);
}

/*!
  Overloaded to not write out internal engine connections.
*/
void
SoShuttle::write(SoWriteAction * action)
{
  // Decouple connections to/from internal engine to avoid it being
  // written.
  this->timer->on.disconnect(&this->on);
  this->timer->on = FALSE;
  this->calculator->b.disconnect(&this->speed);
  this->interpolator->input0.disconnect(&this->translation0);
  this->interpolator->input1.disconnect(&this->translation1);
  this->translation.disconnect(&this->interpolator->output);

  inherited::write(action);

  // Reenable all connections to/from internal engine.
  this->timer->on.connectFrom(&this->on);
  this->calculator->b.connectFrom(&this->speed);
  this->interpolator->input0.connectFrom(&this->translation0);
  this->interpolator->input1.connectFrom(&this->translation1);
  this->translation.connectFrom(&this->interpolator->output);

  // Make sure "on" field of the timer engine get synchronized with
  // the "on" field of the shuttle.
  this->on.touch();
}
