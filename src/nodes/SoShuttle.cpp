/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoShuttle SoShuttle.h Inventor/nodes/SoShuttle.h
  \brief The SoShuttle class is used to oscillate between two translations.
  \ingroup nodes
  
  A smooth transition between translation0 and translation1 is created using
  a cosine function. In the beginning of the cycle, translation0 is 
  used. Halfway through the cycle, the resulting translation equals
  translation1, and at the end of the cycle, we're at translation0 again.

*/

#include <Inventor/nodes/SoShuttle.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/engines/SoInterpolateVec3f.h>


/*!
  \var SoSFVec3f SoShuttle::translation0
  The first translation.
*/
/*!
  \var SoSFVec3f SoShuttle::translation1
  The second translation.
*/
/*!
  \var SoSFFloat SoShuttle::speed
  Number of cycles per second.
*/
/*!
  \var SoSFBool SoShuttle::on
  Toggles animation on or off.  
*/

SO_NODE_SOURCE(SoShuttle);

/*!
  Constructor.
*/
SoShuttle::SoShuttle()
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
