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

#ifndef COIN_SOGLTEXTUREIMAGEELEMENT_H
#define COIN_SOGLTEXTUREIMAGEELEMENT_H

#include <Inventor/elements/SoTextureImageElement.h>

class SoGLImage;
class SoGLDisplayList;

class COIN_DLL_EXPORT SoGLTextureImageElement : public SoTextureImageElement {
  typedef SoTextureImageElement inherited;

  SO_ELEMENT_HEADER(SoGLTextureImageElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLTextureImageElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  static void set(SoState * const state, SoNode * const node,
                  SoGLImage *image, const Model model,
                  const SbColor &blendColor);

  virtual void evaluate(const SbBool enabled, const SbBool transparency) const;
  static int32_t getMaxGLTextureSize(void);

protected:
  virtual SbBool hasTransparency(void) const;

private:
  SoGLImage * image;
  SoGLDisplayList * dlist;
  float quality;
  SbBool alphatest;
  SoState * state;
  int glmodel;
  SbBool glalphatest;
  SbColor glblendcolor;
  SbBool didapply;
};

#endif // !COIN_SOGLTEXTUREIMAGEELEMENT_H
