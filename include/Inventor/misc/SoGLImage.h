/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOGLIMAGE_H__
#define __SOGLIMAGE_H__

// FIXME: couldn't some of this code be used independently of OpenGL?
// And shouldn't its inclusion be based on the presence of texture
// handling instead?  19990408 mortene.
#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLRENDERACTION)
#error This file is excluded from Coin through the configuration system!
#endif // COIN_EXCLUDE_SOGLRENDERACTION

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec2s.h>

class SoImageInterface;

class SoGLImage {
public:

  static SoGLImage *findOrCreateGLImage(SoImageInterface * const image,
                                        void * const context);

  void unref(); // use this to delete

  SbBool init(const SbBool clamps, const SbBool clampt);
  SbBool isInitialized() const;
  void apply() const;
  int getHandle() const;
  SbBool hasAlphaComponent() const;
  const SoImageInterface *getImage() const;
  SbBool shouldClampS() const;
  SbBool shouldClampT() const;

private:
  friend class dummyClass; // avoid warnings on stupid compilers

  SoGLImage(SoImageInterface * const img,
            void * const context);
  ~SoGLImage();

  static void unrefGLImage(SoGLImage * const image);
  void checkResize();

  SoImageInterface *image;

  unsigned int alpha : 1;
  unsigned int clampS : 1;
  unsigned int clampT : 1;

  void *context;
  int refCount;
  int handle;
};

#endif // !__SOGLIMAGE_H__
