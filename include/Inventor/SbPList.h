/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SBPLIST_H
#define COIN_SBPLIST_H

#ifndef COIN_INTERNAL
 // The next two includes are for Open Inventor compatibility.
 #include <Inventor/SbBasic.h>
 #include <Inventor/SbLinear.h>
 // Here's the class definition of SbPList in Coin.
 #include <Inventor/lists/SbPList.h>
#else // COIN_INTERNAL
 #error "Do not include Inventor/SbPList.h internally (use Inventor/lists/SbPList.h)."
#endif // COIN_INTERNAL

#endif // !COIN_SBPLIST_H
