#ifndef COIN_SBBOX_H
#define COIN_SBBOX_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#if defined(COIN_INTERNAL)
#error "Do not include SbBox.h internally."
#endif // COIN_INTERNAL

// These are here for compile-time compatibility with SGI / TGS
// Inventor.
#include <Inventor/SbBasic.h>
#include <Inventor/SbLinear.h>

#include <Inventor/SbBox2f.h>
#include <Inventor/SbBox2s.h>
#include <Inventor/SbBox3s.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbXfBox3f.h>

#endif // !COIN_SBBOX_H
