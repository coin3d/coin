#ifndef COIN_SOTEXTOUTLINEENABLEDELEMENT_H
#define COIN_SOTEXTOUTLINEENABLEDELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/elements/SoInt32Element.h>

class COIN_DLL_API SoTextOutlineEnabledElement : public SoInt32Element {
  typedef SoInt32Element inherited;

  SO_ELEMENT_HEADER(SoTextOutlineEnabledElement);
public:
  static void initClass(void);
protected:
  virtual ~SoTextOutlineEnabledElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

  static void set(SoState * const state, SoNode * const node,
                  const SbBool enabled);
  static void set(SoState * const state, const SbBool enabled);
  static SbBool get(SoState * const state);
  static SbBool getDefault(void);
};

#endif // !COIN_SOTEXTOUTLINEENABLEDELEMENT_H
