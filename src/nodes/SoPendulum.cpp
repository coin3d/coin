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
  \class SoPendulum SoPendulum.h Inventor/nodes/SoPendulum.h
  \brief The SoPendulum class is used to create oscillating rotations.
  \ingroup nodes

  A smooth transition between rotation0 and rotation1 is created using
  a cosine function. In the beginning of the cycle, rotation0 is
  used. Halfway through the cycle, the resulting rotation equals
  rotation1, and at the end of the cycle, we're at rotation0 again.
*/

#include <Inventor/nodes/SoPendulum.h>
#include <Inventor/SbVec3f.h>
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
  SO_NODE_INTERNAL_INIT_CLASS(SoPendulum);
}
