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

#ifndef COIN_SOGLPOLYGONOFFSETELEMENT_H
#define COIN_SOGLPOLYGONOFFSETELEMENT_H

#include <Inventor/elements/SoPolygonOffsetElement.h>


class COIN_DLL_API SoGLPolygonOffsetElement : public SoPolygonOffsetElement {
  typedef SoPolygonOffsetElement inherited;

  SO_ELEMENT_HEADER(SoGLPolygonOffsetElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLPolygonOffsetElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

  virtual void lazyEvaluate(void) const;
  virtual SbBool isLazy(void) const;

protected:
  virtual void setElt(float factor, float units, Style styles, SbBool on);

private:

  Style currentstyles;
  float currentoffsetfactor;
  float currentoffsetunits;
  SoState * state; // needed to test for OpenGL extension

  void updategl(void);
};

#endif // !COIN_SOGLPOLYGONOFFSETELEMENT_H
