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

#ifndef COIN_SOBASIC_H
#define COIN_SOBASIC_H

#ifdef COIN_INTERNAL
#error Do not include Inventor/misc/SoBasic.h internally.
#endif // COIN_INTERNAL


// This file is present just for compatibility with Open Inventor. The
// macros originally provided here (SO__QUOTE and SO__CONCAT), has
// been moved to SbBasic.h, so we more comfortably can tune them to
// the findings of the configure process.  -mortene.

#include <Inventor/SbBasic.h>


#endif // !COIN_SOBASIC_H
