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

#ifndef __COIN_KOSHER_H__
#define __COIN_KOSHER_H__

// *************************************************************************
/*
  The intention for this file is to make all the various operating systems
  and compilers "kosher" for compiling Coin.

  The contents of this file has varied during Coin development and is now at
  one of its minimums.
*/

#if defined(__GNUC__) && (__GNUC__ == 2 && __GNUC_MINOR__ <= 7)
// gcc v2.7 or older
#define NEED_TEMPLATE_DEFINITION (1)
#else
// to make sure this file has been parsed at all...
#define DONT_NEED_TEMPLATE_DEFINITION (1)
#endif

#endif // !__COIN_KOSHER_H__
