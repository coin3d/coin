/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOGLIMAGE_H
#define COIN_SOGLIMAGE_H

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2s.h>

class COIN_DLL_EXPORT SoGLImage {
public:

  SoGLImage();
  ~SoGLImage();

  void setData(const unsigned char * bytes,
               const SbVec2s size,
               const int nc,
               const SbBool clamps,
               const SbBool clampt,
               const float quality,
               void * context,
               const SbBool createhandlenow = FALSE);

  const unsigned char * getDataPtr(void) const;
  SbVec2s getSize(void) const;
  int getNumComponents(void) const;

  void apply(const float quality);
  SbBool hasTransparency(void) const;
  SbBool needAlphaTest(void) const;
  SbBool shouldClampS() const;
  SbBool shouldClampT() const;
  float getQuality() const;
  SbBool isValid(void) const;

private:
  int createHandle(void);
  void checkTransparency(void);

  const unsigned char * bytes;
  SbVec2s size;
  int numcomponents;
  unsigned int flags;
  void * context;
  int handle;
  float quality;
};

#endif // !COIN_SOGLIMAGE_H
