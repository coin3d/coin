/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOINTERPOLATE_H__
#define __SOINTERPOLATE_H__

#if defined(__SOLIB_INTERNAL__)
#error "Do not include SbInterpolate.h internally."
#endif // __SOLIB_INTERNAL__

#define __INCLUDED_FROM_SOINTERPOLATE_H__
#include <Inventor/engines/SoInterpolateFloat.h>
#include <Inventor/engines/SoInterpolateVec2f.h>
#include <Inventor/engines/SoInterpolateVec3f.h>
#include <Inventor/engines/SoInterpolateVec4f.h>
#include <Inventor/engines/SoInterpolateRotation.h>
#undef  __INCLUDED_FROM_SOINTERPOLATE_H__

#endif // !__SOINTERPOLATE_H__
