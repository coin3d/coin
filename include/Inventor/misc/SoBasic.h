#ifndef COIN_SOBASIC_H
#define COIN_SOBASIC_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef COIN_INTERNAL
#error Do not include Inventor/misc/SoBasic.h internally.
#endif // COIN_INTERNAL

// This file is present just for compatibility with Open Inventor. The
// macros originally provided here (SO__QUOTE and SO__CONCAT), has
// been moved to SbBasic.h, so we more comfortably can tune them to
// the findings of the configure process.  -mortene.

#include <Inventor/SbBasic.h>

#endif // !COIN_SOBASIC_H
