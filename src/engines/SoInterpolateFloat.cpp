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
  \class SoInterpolateFloat SoInterpolateFloat.h Inventor/engines/SoInterpolateFloat.h
  \brief The SoInterpolateFloat class is used to interpolate two floating point values.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoInterpolateFloat.h>

SO_INTERPOLATE_SOURCE(SoInterpolateFloat, SoMFFloat, float, (0.0f), (1.0f),
                      (v1-v0)*a+v0);
SO_INTERPOLATE_INTERNAL_INIT_CLASS(SoInterpolateFloat);
