#ifndef COIN_SOAMBIENTCOLORELEMENT_H
#define COIN_SOAMBIENTCOLORELEMENT_H

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

#include <Inventor/elements/SoReplacedElement.h>

class SbColor;

class COIN_DLL_API SoAmbientColorElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoAmbientColorElement);
public:
  static void initClass(void);
protected:
  virtual ~SoAmbientColorElement();

public:
  virtual void init(SoState * state);
  static void set(SoState * const state, SoNode * const node,
                  const int32_t numcolors, const SbColor * const colors);

  static const SoAmbientColorElement * getInstance(SoState * state);

  int32_t getNum(void) const;
  const SbColor & get(const int index) const;
  const SbColor * getArrayPtr(void) const;

private:
  SoState * state;
};

#endif // !COIN_SOAMBIENTCOLORELEMENT_H
