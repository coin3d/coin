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

#ifndef COIN_SOGLAMBIENTCOLORELEMENT_H
#define COIN_SOGLAMBIENTCOLORELEMENT_H

#include <Inventor/elements/SoAmbientColorElement.h>
#include <Inventor/SbColor.h>


class COIN_DLL_API SoGLAmbientColorElement : public SoAmbientColorElement {
  typedef SoAmbientColorElement inherited;

  SO_ELEMENT_HEADER(SoGLAmbientColorElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLAmbientColorElement();

public:
  virtual void init(SoState *state);
  virtual void push(SoState *state);
  virtual void pop(SoState *state, const SoElement *prevTopElement);

  void send(const int index);

protected:
  virtual void setElt(const int32_t numColors, const SbColor * const colors);

private:
  void updategl();
  SbColor current;

};

#endif // !COIN_SOGLAMBIENTCOLORELEMENT_H
