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
  \class SoInterpolate SoInterpolate.h Inventor/engines/SoInterpolate.h
  \brief The SoInterpolate class is the base class for all interpolator engines.
  \ingroup engines

  Interpolators are used to linearly interpolate between two values.

  In Coin, we've chosen to implement all interpolators in separate
  files. If you want to be OIV compatible when programming, you should
  include the SoInterpolate.h, and not the interpolator file(s) you
  need.
*/

#include <Inventor/engines/SoInterpolate.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_ENGINE_ABSTRACT_SOURCE(SoInterpolate);

/*!
  Default constructor.
*/
SoInterpolate::SoInterpolate(void)
{
  SO_ENGINE_CONSTRUCTOR(SoInterpolate);

  SO_ENGINE_ADD_INPUT(alpha, (0.0f));
}

// overloaded from parent
void
SoInterpolate::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(SoInterpolate);
}

/*!
  Destructor.
*/
SoInterpolate::~SoInterpolate()
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoInterpolate::~SoInterpolate", "%p", this);
#endif // debug
}
