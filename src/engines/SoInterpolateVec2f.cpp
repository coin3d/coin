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
  \class SoInterpolateVec2f SoInterpolateVec2f.h Inventor/engines/SoInterpolateVec2f.h
  \brief The SoInterpolateVec2f class is used to interpolate between two 2D vectors.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoInterpolateVec2f.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_INTERPOLATE_INTERNAL_INIT_CLASS(SoInterpolateVec2f);

SO_INTERPOLATE_INTERNAL_SOURCE(SoInterpolateVec2f,
                               SoMFVec2f,
                               SbVec2f,
                               (0.0f,0.0f),
                               (0.0f,0.0f),
                               (v1-v0)*a+v0);
