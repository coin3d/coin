#ifndef COIN_SOGLTEXTUREENABLEDELEMENT_H
#define COIN_SOGLTEXTUREENABLEDELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/elements/SoTextureEnabledElement.h>

class COIN_DLL_API SoGLTextureEnabledElement : public SoTextureEnabledElement {
  typedef SoTextureEnabledElement inherited;

  SO_ELEMENT_HEADER(SoGLTextureEnabledElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLTextureEnabledElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  static void set(SoState * const state, SoNode * const node,
                  const SbBool enabled);
  static void set(SoState * const state, const SbBool enabled);

  static SbBool get(SoState * const state);
  static SbBool getDefault(void);

  virtual void setElt(int32_t value);

private:
  void updategl(void);

};

#endif // !COIN_SOGLTEXTUREENABLEDELEMENT_H
