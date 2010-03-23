#ifndef COIN_SOCACHEHINTELEMENT_H
#define COIN_SOCACHEHINTELEMENT_H

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

#include <Inventor/elements/SoSubElement.h>

class SoCacheHintElementP;

// Please note that this is an experimental class. The API might
// change a lot before/if it's included in any official Coin
// release. pederb, 2004-07-02
class COIN_DLL_API SoCacheHintElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoCacheHintElement);
public:
  static void initClass(void);
protected:
  virtual ~SoCacheHintElement();

public:

  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevtopelement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  static void set(SoState * state, SoNode * node,
                  const float memvalue,
                  const float gfxvalue);
  static void get(SoState * state, float & memvalue, float & gfxvalue);
  
protected:

  SoCacheHintElementP * pimpl;
};

#endif // !COIN_SOCACHEHINTELEMENT_H
