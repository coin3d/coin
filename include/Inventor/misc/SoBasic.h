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
