/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_WGL

#include "SoOffscreenWGLData.h"


SoOffscreenWGLData::SoOffscreenWGLData(void)
{
  this->buffer = NULL;
  this->context = NULL;
}

SoOffscreenWGLData::~SoOffscreenWGLData() 
{
  if (this->context) cc_glglue_context_destruct(this->context);
  delete[] this->buffer;
}

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

void 
SoOffscreenWGLData::setBufferSize(const SbVec2s & size) 
{
  // Avoid costly operations below if not really necessary.
  if (this->buffersize == size) { return; }

  SoOffscreenInternalData::setBufferSize(size);
  
  delete[] this->buffer;
  this->buffer =
    new unsigned char[this->buffersize[0] * this->buffersize[1] * 4];
  
  // just delete the old context. Don't create a new one yet.
  if (this->context) {
    cc_glglue_context_destruct(this->context);
    this->context = NULL;
  }
}

SbBool 
SoOffscreenWGLData::makeContextCurrent(uint32_t contextid) 
{
  assert(this->buffer);
  
  if (this->context == NULL) {
    this->context = cc_glglue_context_create_offscreen(this->buffersize[0], this->buffersize[1]);
  }
  if (this->context) {
    return cc_glglue_context_make_current(this->context);
  }
  return FALSE;
}

unsigned char * 
SoOffscreenWGLData::getBuffer(void) 
{
  return this->buffer;
}

void
SoOffscreenWGLData::unmakeContextCurrent(void)
{
  if (this->context) {
    cc_glglue_context_reinstate_previous(this->context);
  }
}

#endif // HAVE_WGL
