#ifndef COIN_SBBOX_H
#define COIN_SBBOX_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#if defined(COIN_INTERNAL)
#error "Do not include SbBox.h internally."
#endif // COIN_INTERNAL

#define COIN_INCLUDED_FROM_SBBOX_H
#include <Inventor/SbBox2f.h>
#include <Inventor/SbBox2s.h>
#include <Inventor/SbBox3s.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbXfBox3f.h>
#undef  COIN_INCLUDED_FROM_SBBOX_H

#endif // !COIN_SBBOX_H
