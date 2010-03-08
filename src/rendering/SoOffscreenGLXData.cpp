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

#ifdef HAVE_GLX

#include "SoOffscreenGLXData.h"

#include <Inventor/errors/SoDebugError.h>

#include "tidbitsp.h"

// *************************************************************************

Display * SoOffscreenGLXData::display = NULL;
static SbBool display_initialized = FALSE;

void
SoOffscreenGLXData::cleanup(void)
{
  display_initialized = FALSE;
  // FIXME: Disabled since this might cause problems according to the
  // comment below (see getDisplay). Not sure if *not* closing the
  // display cannot also lead to problems though... 20060208 kyrah
#if 0
  XCloseDisplay(display);
  display = NULL;
#endif
}

Display *
SoOffscreenGLXData::getDisplay(void)
{
  if (!display_initialized) {
    display_initialized = TRUE;
    coin_atexit((coin_atexit_f*)SoOffscreenGLXData::cleanup, CC_ATEXIT_NORMAL);

    // Keep a single static display-ptr.
    //
    // This resource is never deallocated explicitly (but of course
    // implicitly by the system on application close-down). This to
    // work around some strange problems with the NVidia-driver 29.60
    // on XFree86 v4 when using XCloseDisplay() -- like doublebuffered
    // visuals not working correctly.
    //
    // Also, XCloseDisplay() has been known to cause crashes when
    // running remotely from some old Mesa version on Red Hat Linux
    // 6.2 onto an IRIX6.5 display. It seems likely that this was
    // caused by a bug in that particular old Mesa version.
    //
    // mortene@sim.no

    SoOffscreenGLXData::display = XOpenDisplay(NULL);

    if (!SoOffscreenGLXData::display) {
      SoDebugError::post("SoOffscreenGLXData::SoOffscreenGLXData",
                         "Couldn't connect to X11 DISPLAY.");
      // FIXME: will probably cause a crash later? If so, should be
      // more robust. 20020802 mortene.
      return NULL;
    }
  }

  return SoOffscreenGLXData::display;
}

// Pixels-pr-mm.
SbVec2f
SoOffscreenGLXData::getResolution(void)
{
  Display * d = SoOffscreenGLXData::getDisplay();
  if (!d) {
    return SbVec2f(72.0f / 25.4f, 72.0f / 25.4f); // fall back to 72dpi
  }

  int s = DefaultScreen(d);
  SbVec2f r(((float)DisplayWidth(d, s)) /  ((float)DisplayWidthMM(d, s)),
            ((float)DisplayHeight(d, s)) / ((float)DisplayHeightMM(d, s)));

  return r;
}

#endif // HAVE_GLX
