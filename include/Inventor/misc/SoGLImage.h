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
#include <Inventor/lists/SbList.h>

class SoGLDisplayList;
class SoState;

class COIN_DLL_EXPORT SoGLImage {
public:

  SoGLImage();
  void unref(SoState * state = NULL);

  enum Wrap {
    REPEAT = 0,
    CLAMP,
    CLAMP_TO_EDGE
  };

  void setData(const unsigned char * bytes,
               const SbVec2s size,
               const int nc,
               const Wrap wraps,
               const Wrap wrapt,
               const float quality = 0.5f,
               const int border = 0,
               SoState * createinstate = NULL);

  const unsigned char * getDataPtr(void) const;
  SbVec2s getSize(void) const;
  int getNumComponents(void) const;

  static void apply(SoState * state, SoGLDisplayList * dl, const float quality);
  SoGLDisplayList * getGLDisplayList(SoState * state, const float quality);
  SbBool hasTransparency(void) const;
  SbBool needAlphaTest(void) const;
  Wrap getWrapS(void) const;
  Wrap getWrapT(void) const;

private:
  friend class my_dummy_class; // avoid warnings because of private destructor
  ~SoGLImage();
  SoGLDisplayList * createGLDisplayList(SoState * state, const float quality);
  void checkTransparency(void);
  void unrefDLists(SoState * state);
  void reallyCreateTexture(SoState * state,
                           const unsigned char * const texture,
                           const int numComponents,
                           const int w, const int h,
                           const SbBool dlist,
                           const SbBool mipmap,
                           const int border);
  const unsigned char * bytes;
  SbVec2s size;
  int numcomponents;
  unsigned int flags;
  Wrap wraps;
  Wrap wrapt;
  int border;
  SbList <SoGLDisplayList*> dlists;
  static int translateWrap(const Wrap wrap);
};

#endif // !COIN_SOGLIMAGE_H
