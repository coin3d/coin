/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOFLOATELEMENT_H
#define COIN_SOFLOATELEMENT_H

#include <Inventor/elements/SoSubElement.h>


class COIN_DLL_API SoFloatElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_ABSTRACT_HEADER(SoFloatElement);
public:
  static void initClass(void);
protected:
  virtual ~SoFloatElement();

public:
  virtual void init(SoState * state);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  virtual void print(FILE * file) const;

  static void set(const int stackIndex, SoState * const state,
                  SoNode * const node, const float value);
  static void set(const int stackIndex, SoState * const state,
                  const float value);

  static float get(const int stackIndex, SoState * const state);

  virtual void setElt(float value);

protected:
  float data;
};

#endif // !COIN_SOFLOATELEMENT_H
