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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include "SoOffscreenCGData.h"

#ifdef COIN_MACOS_10_3
#include <ApplicationServices/ApplicationServices.h>
#endif

// Pixels-pr-mm.
SbVec2f
SoOffscreenCGData::getResolution(void)
{
#ifdef COIN_MACOS_10_3
  CGDirectDisplayID display = CGMainDisplayID();
  CGSize size = CGDisplayScreenSize(display);
  return SbVec2f(CGDisplayPixelsWide(display)/size.width,
                 CGDisplayPixelsHigh(display)/size.height)l
#else
  return SbVec2f(72.0f / 25.4f, 72.0f / 25.4f); // fall back to 72dpi
#endif
}
