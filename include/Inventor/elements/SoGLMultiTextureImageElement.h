#ifndef COIN_SOGLMULTITEXTUREIMAGEELEMENT_H
#define COIN_SOGLMULTITEXTUREIMAGEELEMENT_H

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

#include <Inventor/elements/SoMultiTextureImageElement.h>

class SoGLImage;
class SoGLDisplayList;
class SoGLMultiTextureImageElementP;

class COIN_DLL_API SoGLMultiTextureImageElement : public SoMultiTextureImageElement {
  typedef SoMultiTextureImageElement inherited;

  SO_ELEMENT_HEADER(SoGLMultiTextureImageElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLMultiTextureImageElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);
  
  static void set(SoState * const state, SoNode * const node,
                  const int unit,
                  SoGLImage * image, const SoTextureImageElement::Model model,
                  const SbColor & blendColor);

  static void restore(SoState * state, const int unit);
  
  static SoGLImage * get(SoState * state, 
                         const int unit,
                         SoTextureImageElement::Model & model,
                         SbColor & blendcolor);
  
  class GLUnitData {
  public:
    SoGLImage * glimage;
  };
  
protected:
  virtual SbBool hasTransparency(const int unit) const;

private:
  void updateGL(const int unit);
  SoGLMultiTextureImageElementP * pimpl;
};

#endif // !COIN_SOGLMULTITEXTUREIMAGEELEMENT_H

