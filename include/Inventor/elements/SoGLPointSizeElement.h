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

#ifndef COIN_SOGLPOINTSIZEELEMENT_H
#define COIN_SOGLPOINTSIZEELEMENT_H

#include <Inventor/SbVec2f.h>
#include <Inventor/elements/SoPointSizeElement.h>


class COIN_DLL_API SoGLPointSizeElement : public SoPointSizeElement {
  typedef SoPointSizeElement inherited;

  SO_ELEMENT_HEADER(SoGLPointSizeElement);

public:
  static void initClass(void);
protected:
  virtual ~SoGLPointSizeElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

  virtual void lazyEvaluate(void) const;
  virtual SbBool isLazy(void) const;

  static SbVec2f getValidRange(void);

protected:
  virtual void setElt(float size);

private:
  // FIXME: use native types, in this case an SbVec2f. Can't be fixed
  // without breaking ABI compatibility, though, so wait until Coin
  // version 2.  20010823 mortene.
  static float sizerange[2];

  float current;
  void updategl();
};

#endif // !COIN_SOGLPOINTSIZEELEMENT_H
