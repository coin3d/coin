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

#ifndef COIN_SBBOX_H
#define COIN_SBBOX_H

#if defined(COIN_INTERNAL)
#error "Do not include SbBox.h internally."
#endif // COIN_INTERNAL

#define COIN_INCLUDED_FROM_SBBOX_H
#include <Inventor/SbBox2f.h>
#include <Inventor/SbBox2s.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbXfBox3f.h>
#undef  COIN_INCLUDED_FROM_SBBOX_H

#endif // !COIN_SBBOX_H
