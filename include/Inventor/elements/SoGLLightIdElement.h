#ifndef COIN_SOGLLIGHTIDELEMENT_H
#define COIN_SOGLLIGHTIDELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/elements/SoInt32Element.h>

class COIN_DLL_API SoGLLightIdElement : public SoInt32Element {
  typedef SoInt32Element inherited;

  SO_ELEMENT_HEADER(SoGLLightIdElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLLightIdElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  static int32_t increment(SoState * const state, SoNode * const node);
  static int32_t increment(SoState * const state);

  static int32_t get(SoState * const state);
  static int32_t getDefault();
  static int32_t getMaxGLSources();

protected:
};

#endif // !COIN_SOGLLIGHTIDELEMENT_H
