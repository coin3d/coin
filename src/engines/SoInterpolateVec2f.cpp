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
  \class SoInterpolateVec2f SoInterpolateVec2f.h Inventor/engines/SoInterpolateVec2f.h
  \brief The SoInterpolateVec2f class is used to interpolate between two 2D vectors.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoInterpolateVec2f.h>

SO_INTERPOLATE_SOURCE(SoInterpolateVec2f, SoMFVec2f, SbVec2f, (0.0f,0.0f),
                      (0.0f,0.0f), (v1-v0)*a+v0);
SO_INTERPOLATE_INTERNAL_INIT_CLASS(SoInterpolateVec2f);
