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
  \class SoInterpolateVec2f SoInterpolateVec2f.h Inventor/engines/SoInterpolateVec2f.h
  \brief The SoInterpolateVec2f class is used to interpolate between pairs of 2D vectors.
  \ingroup engines
*/

#include <Inventor/engines/SoInterpolateVec2f.h>
#include <Inventor/engines/SoSubEngineP.h>

/*!
  \var SoMFVec2f SoInterpolateVec2f::input0
  First set of input values.
*/
/*!
  \var SoMFVec2f SoInterpolateVec2f::input1
  Second set of input values.
*/

SO_INTERPOLATE_INTERNAL_INIT_CLASS(SoInterpolateVec2f);

SO_INTERPOLATE_INTERNAL_SOURCE(SoInterpolateVec2f,
                               SoMFVec2f,
                               SbVec2f,
                               (0.0f,0.0f),
                               (0.0f,0.0f),
                               (v1-v0)*a+v0);
