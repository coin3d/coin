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

#ifndef COIN_SOGLSHININESSELEMENT_H
#define COIN_SOGLSHININESSELEMENT_H

#include <Inventor/elements/SoShininessElement.h>


class COIN_DLL_EXPORT SoGLShininessElement : public SoShininessElement {
  typedef SoShininessElement inherited;

  SO_ELEMENT_HEADER(SoGLShininessElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLShininessElement();

public:
  virtual void init(SoState *state);
  virtual void push(SoState *state);
  virtual void pop(SoState *state, const SoElement *prevTopElement);

  void send(const int index);

protected:
  virtual void setElt(const int32_t numValues, const float * const values);

private:
  void updategl();
  float current;

};

#endif // !COIN_SOGLSHININESSELEMENT_H
