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
  // Don't use standard SO_ENGINE_CONSTRUCTOR.

  // Catch attempts to use an engine class which has not been
  // initialized.
  assert(SoInterpolate::classTypeId != SoType::badType());
  // Initialize a inputdata container for the class only once.
  if (!SoInterpolate::inputdata) {
    // FIXME: is this really necessary for SoInterpolate? 20000505 mortene.
    SoInterpolate::inputdata =
      new SoFieldData(SoInterpolate::parentinputdata ?
                      *SoInterpolate::parentinputdata : NULL);
    SoInterpolate::outputdata =
      new SoEngineOutputData(SoInterpolate::parentoutputdata ?
                             *SoInterpolate::parentoutputdata : NULL);
  }

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
