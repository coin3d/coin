#ifndef COIN_SOOFFSCREENGLXDATA_H
#define COIN_SOOFFSCREENGLXDATA_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include "SoOffscreenInternalData.h"
#include <X11/Xlib.h>
#include <Inventor/SbVec2f.h>


class SoOffscreenGLXData : public SoOffscreenInternalData {
public:
  SoOffscreenGLXData(void);
  virtual ~SoOffscreenGLXData();
  virtual void setBufferSize(const SbVec2s & size);
  virtual SbBool makeContextCurrent(uint32_t contextid);
  virtual void unmakeContextCurrent(void);
  virtual unsigned char * getBuffer(void);
  static SbVec2f getResolution(void);

private:
  static Display * getDisplay(void);

  unsigned char * buffer;
  static Display * display;

  void * context;
};

#endif // !COIN_SOOFFSCREENGLXDATA_H
