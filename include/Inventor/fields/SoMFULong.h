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

#ifndef __SOMFULONG_H__
#define __SOMFULONG_H__

#if defined(IV_STRICT)
#error "SoMFULong has been obsoleted. Use SoMFUInt32 instead."
#endif // IV_STRICT

#include <Inventor/fields/SoMFUInt32.h>
typedef SoMFUInt32 SoMFULong;

#endif // !__SOMFULONG_H__
