#ifndef COIN_SBDPLINEAR_H
#define COIN_SBDPLINEAR_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifdef COIN_INTERNAL
/* Detect erroneous include statements in library code. Application
   programmers can ignore this. */
#error "Do not include SbLinear.h in internal library code."
#endif // COIN_INTERNAL

/*
 * Note to application programmers: include this header file in your
 * application code instead of the specific class header files below
 * if you want to be sourcecode compatible with the SGI and TGS
 * Inventor implementations.
 *
 * (All the class definitions below are placed in the SbLinear.h file
 * in SGI and TGS Inventor, and the following header files are not
 * part of the SDK.)
 */

#include <Inventor/SbVec2d.h>
#include <Inventor/SbVec3d.h>
#include <Inventor/SbVec4d.h>
// #include <Inventor/SbSphere.h>
#include <Inventor/SbDPRotation.h>
// #include <Inventor/SbLine.h>
// #include <Inventor/SbCylinder.h>
// #include <Inventor/SbPlane.h>
#include <Inventor/SbDPMatrix.h>
#include <Inventor/SbDPRotation.h>
// #include <Inventor/SbPlane.h>
// #include <Inventor/SbViewVolume.h>

#endif // !COIN_SBDPLINEAR_H
