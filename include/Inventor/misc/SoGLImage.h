/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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
               void * context);

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
