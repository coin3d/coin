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
#include <Inventor/SbString.h>
#include <Inventor/SbVec2s.h>

class SoImageInterface;


class SoGLImage {
public:

  static SoGLImage *findOrCreateGLImage(SoImageInterface * const image,
                                        const SbBool clamps,
                                        const SbBool clampt,
                                        const float quality,
                                        void * const context);

  void unref(); // use this to delete

  SbBool matches(const SbBool clamps, const SbBool clampt,
                 const float quality) const;

  void apply() const;
  int getHandle() const;
  SbBool hasTransparency() const;
  const SoImageInterface *getImage() const;
  SbBool shouldClampS() const;
  SbBool shouldClampT() const;
  float getQuality() const;

private:
  friend class dummyClass; // avoid warnings on stupid compilers

  SoGLImage(SoImageInterface * const img,
            const SbBool clamps,
            const SbBool clampt,
            const float quality,
            void * const context);
  ~SoGLImage();

  static void unrefGLImage(SoGLImage * const image);
  void checkResize();
  SbBool GLinit();

  SoImageInterface *image;

  SbBool clampS;
  SbBool clampT;
  void *context;
  int refCount;
  int handle;
  float quality;
};

#endif // !COIN_SOGLIMAGE_H
