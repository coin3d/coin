/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_AGL

#include "SoOffscreenAGLData.h"


SoOffscreenAGLData::SoOffscreenAGLData(void)
{
  this->buffer = NULL;
  this->context = NULL;
}

SoOffscreenAGLData::~SoOffscreenAGLData() 
{
  if (this->context) cc_glglue_context_destruct(this->context);
  delete[] this->buffer;
}


// Pixels-pr-mm.
SbVec2f
SoOffscreenAGLData::getResolution(void)
{
  SInt16 hr, vr;
  ScreenRes(&hr, &vr); 
  return SbVec2f((float)hr / 25.4f, (float)vr / 25.4f);
}

void 
SoOffscreenAGLData::setBufferSize(const SbVec2s & size) 
{
  // Avoid costly operations below if not really necessary.
  if (this->buffersize == size) { return; }

  SoOffscreenInternalData::setBufferSize(size);

  delete[] this->buffer;
  this->buffer =
    new unsigned char[this->buffersize[0] * this->buffersize[1] * 4];

  if (this->context) cc_glglue_context_destruct(this->context);
  this->context = cc_glglue_context_create_offscreen(size[0], size[1]);
}

SbBool 
SoOffscreenAGLData::makeContextCurrent(uint32_t contextid) 
{
  assert(this->buffer);
  assert(this->context);
  return cc_glglue_context_make_current(this->context);
}

unsigned char * 
SoOffscreenAGLData::getBuffer(void) 
{
  return this->buffer;
}

void
SoOffscreenAGLData::unmakeContextCurrent(void)
{   
  cc_glglue_context_reinstate_previous(this->context);
}

#endif // HAVE_AGL
