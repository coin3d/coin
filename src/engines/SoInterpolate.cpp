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

/*!
  \var SoSFFloat SoInterpolate::alpha

  The value which says how much we've should interpolate from first
  value to second value. A value equal to 0 will give an output equal
  to the first value, alpha equal to 1 gives the second value, any
  value in between gives a "weighted" interpolation between the two
  values.
*/
/*!
  \var SoEngineOutput SoInterpolate::output

  Interpolated values from the input fields. The type of the output
  will of course be the same as the type of the input fields of each
  non-abstract subclass inheriting SoInterpolate.
*/

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

// Documented in superclass.
void
SoInterpolate::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(SoInterpolate);
}

/*!
  This method is provided only for API compatibility, and does nothing
  in Coin.
*/
void 
SoInterpolate::initClasses(void)
{
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
