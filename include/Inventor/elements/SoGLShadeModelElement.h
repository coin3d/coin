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

#ifndef COIN_SOGLSHADEMODELELEMENT_H
#define COIN_SOGLSHADEMODELELEMENT_H

#include <Inventor/elements/SoSubElement.h>


class COIN_DLL_API SoGLShadeModelElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoGLShadeModelElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLShadeModelElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo() const;

  static void set(SoState * state, const SbBool flat);

  virtual void lazyEvaluate(void) const;
  virtual SbBool isLazy(void) const;
  void forceSend(const SbBool flat) const;

  static const SoGLShadeModelElement * getInstance(SoState * state);

private:
  SbBool flat;
  SbBool glflat;
  void updategl(const SbBool flatshade);
};

#endif // !COIN_SOGLSHADEMODELELEMENT_H
