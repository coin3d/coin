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

#ifndef COIN_SOGLDIFFUSECOLORELEMENT_H
#define COIN_SOGLDIFFUSECOLORELEMENT_H

#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/SbVec4f.h>


class COIN_DLL_API SoGLDiffuseColorElement : public SoDiffuseColorElement {
  typedef SoDiffuseColorElement inherited;

  SO_ELEMENT_HEADER(SoGLDiffuseColorElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLDiffuseColorElement();

public:
  virtual void init(SoState *state);
  virtual void push(SoState *state);
  virtual void pop(SoState *state, const SoElement *prevTopElement);

  void send(const int index, const float alpha);
  void send(const int index);
  void sendOnePacked(const uint32_t packedcol);
  void sendOneColor(const SbVec4f & color);

protected:

private:
  void updategl(const uint32_t col);
  void updategl(const SbVec4f & col);
  SbVec4f current;
  uint32_t currentpacked;
  SbBool currentispacked;
};

#endif // !COIN_SOGLDIFFUSECOLORELEMENT_H
