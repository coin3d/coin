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

#ifndef COIN_SOCOMPOSE_H
#define COIN_SOCOMPOSE_H

#ifdef COIN_INTERNAL
/* Detect erroneous include statements in library code. Application
   programmers can ignore this. */
#error "Do not include SbCompose.h in internal library code."
#endif // COIN_INTERNAL

/*
 * Note to application programmers: include this header file instead
 * of the specific class header files below if you want to be
 * sourcecode compatible with the SGI and TGS Inventor
 * implementations.
 *
 * (All the class definitions below are placed in the SoCompose.h file
 * in SGI and TGS Inventor, and the following header files are not
 * part of the SDK.)
 */

#include <Inventor/engines/SoComposeVec2f.h>
#include <Inventor/engines/SoComposeVec3f.h>
#include <Inventor/engines/SoComposeVec4f.h>
#include <Inventor/engines/SoDecomposeVec2f.h>
#include <Inventor/engines/SoDecomposeVec3f.h>
#include <Inventor/engines/SoDecomposeVec4f.h>
#include <Inventor/engines/SoComposeRotation.h>
#include <Inventor/engines/SoDecomposeRotation.h>
#include <Inventor/engines/SoComposeMatrix.h>
#include <Inventor/engines/SoDecomposeMatrix.h>
#include <Inventor/engines/SoComposeRotationFromTo.h>

#endif // !COIN_SOCOMPOSE_H
