#ifndef COIN_SODEPTHBUFFERELEMENT_H
#define COIN_SODEPTHBUFFERELEMENT_H

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

#include <Inventor/elements/SoElement.h>
#include <Inventor/elements/SoSubElement.h>
#include <Inventor/SbVec2f.h>

class COIN_DLL_API SoDepthBufferElement : public SoElement {
  typedef SoElement inherited;
  SO_ELEMENT_HEADER(SoDepthBufferElement);

public:
  static void initClass(void);

  enum DepthWriteFunction {
    NEVER,
    ALWAYS,
    LESS,
    LEQUAL,
    EQUAL,
    GEQUAL,
    GREATER,
    NOTEQUAL
  };

  static void set(SoState * state, SbBool test, SbBool write,
                  DepthWriteFunction function, SbVec2f range);

  static void get(SoState * state, SbBool & test_out, SbBool & write_out,
                  DepthWriteFunction & function_out, SbVec2f & range_out);

  static SbBool getTestEnable(SoState * state);
  static SbBool getWriteEnable(SoState * state);
  static DepthWriteFunction getFunction(SoState * state);
  static SbVec2f getRange(SoState * state);

  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

protected:
  virtual ~SoDepthBufferElement();

  virtual void setElt(SbBool test, SbBool write,
                      DepthWriteFunction function, SbVec2f range);

  SbBool test;
  SbBool write;
  DepthWriteFunction function;
  SbVec2f range;

}; // SoDepthBufferElement

#endif // !COIN_SODEPTHBUFFERELEMENT_H
