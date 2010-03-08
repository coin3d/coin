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

#ifdef HAVE_WGL

#include <windows.h>
#include <Inventor/errors/SoDebugError.h>
#include "SoOffscreenWGLData.h"

// Pixels-pr-mm.
SbVec2f
SoOffscreenWGLData::getResolution(void)
{
  HDC devctx = CreateCompatibleDC(NULL);
  if (devctx == NULL) {
    SoDebugError::postWarning("SoOffscreenWGLData::getResolution",
                              "Couldn't create a device context.");
    return SbVec2f(72.0f / 25.4f, 72.0f / 25.4f); // fall back to 72dpi
  }
  float resx = (float)GetDeviceCaps(devctx, LOGPIXELSX);
  float resy = (float)GetDeviceCaps(devctx, LOGPIXELSY);
  (void)DeleteDC(devctx);

  return SbVec2f(resx / 25.4f, resy / 25.4f);
}

#endif // HAVE_WGL
