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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSWITCHELEMENT_H
#define COIN_SOSWITCHELEMENT_H

#include <Inventor/elements/SoInt32Element.h>


class COIN_DLL_EXPORT SoSwitchElement : public SoInt32Element {
  typedef SoInt32Element inherited;

  SO_ELEMENT_HEADER(SoSwitchElement);
public:
  static void initClass(void);
protected:
  virtual ~SoSwitchElement();

public:
  virtual void init(SoState * state);

  static void set(SoState * const state, SoNode * const node,
                  const int32_t index);
  static void set(SoState * const state, const int32_t index);
  static int32_t get(SoState * const state);
  static int32_t getDefault();
};

#endif // !COIN_SOSWITCHELEMENT_H
