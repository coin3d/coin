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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOVIEWPORTREGIONELEMENT_H
#define COIN_SOVIEWPORTREGIONELEMENT_H

#include <Inventor/elements/SoSubElement.h>
#include <Inventor/SbViewportRegion.h>


class COIN_DLL_EXPORT SoViewportRegionElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoViewportRegionElement);
public:
  static void initClass(void);
protected:
  virtual ~SoViewportRegionElement();

public:
  virtual void init(SoState * state);
  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement *copyMatchInfo() const;

  static void set(SoState * const state,
                  const SbViewportRegion & viewportRegion);
  static const SbViewportRegion &get(SoState * const state);

  virtual void print(FILE * file) const;

protected:
  virtual void setElt(const SbViewportRegion & viewportRegion);
  SbViewportRegion viewportRegion;

private:
  static  SbViewportRegion *emptyViewportRegion;

};

#endif // !COIN_SOVIEWPORTREGIONELEMENT_H
