/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOGLBIGIMAGE_H
#define COIN_SOGLBIGIMAGE_H

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/misc/SoGLImage.h>

class COIN_DLL_API SoGLBigImage : public SoGLImage {
  typedef SoGLImage inherited;

public:

  SoGLBigImage();
  virtual void unref(SoState * state = NULL);

  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const ;

  virtual void setData(const unsigned char * bytes,
                       const SbVec2s size,
                       const int nc,
                       const Wrap wraps,
                       const Wrap wrapt,
                       const float quality = 0.5f,
                       const int border = 0,
                       SoState * createinstate = NULL);
  
  int initSubImages(SoState * state, const SbVec2s & subimagesize) const;
  void handleSubImage(const int idx, SbVec2f & start, SbVec2f & end,
                      SbVec2f & tcmul);
  void applySubImage(SoState * state, const int idx, const float quality,
                     const SbVec2s & projsize);

  // will return NULL to avoid that SoGLTextureImageElement will
  // update the texture state.
  virtual SoGLDisplayList * getGLDisplayList(SoState * state);

private:
  virtual ~SoGLBigImage();

  void copySubImage(const int idx, unsigned char * dst);

  class SoGLBigImageP * pimpl;
  friend class SoGLBigImageP;
};

#endif // !COIN_SOGLBIGIMAGE_H
