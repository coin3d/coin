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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoInterpolateVec4f SoInterpolateVec4f.h Inventor/engines/SoInterpolateVec4f.h
  \brief The SoInterpolateVec4f class is used to interpolate between two 4D vectors.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoInterpolateVec4f.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_INTERPOLATE_INTERNAL_INIT_CLASS(SoInterpolateVec4f);

SO_INTERPOLATE_INTERNAL_SOURCE(SoInterpolateVec4f,
                               SoMFVec4f,
                               SbVec4f,
                               (0.0f,0.0f,0.0f,0.0f),
                               (0.0f,0.0f,0.0f,0.0f),
                               (v1-v0)*a+v0);
