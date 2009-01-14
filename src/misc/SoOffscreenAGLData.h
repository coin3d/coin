#ifndef COIN_SOOFFSCREENAGLDATA_H
#define COIN_SOOFFSCREENAGLDATA_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

// *************************************************************************

// Stop Carbon from trying to use the CarbonSound.h header that was removed 
// in QuickTime 7.0. We are not using CarbonSound, so we do not need that 
// anyway. See http://www.cocoadev.com/index.pl?CarbonSound
#define __CARBONSOUND__
#include <Carbon/Carbon.h>
#include <Inventor/SbVec2f.h>

// *************************************************************************

class SoOffscreenAGLData {
public:
  static SbVec2f getResolution(void);
};

// *************************************************************************

#endif // ! COIN_SOOFFSCREENAGLDATA_H
