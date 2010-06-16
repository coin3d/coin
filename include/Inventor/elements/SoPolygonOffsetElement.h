#ifndef COIN_SOPOLYGONOFFSETELEMENT_H
#define COIN_SOPOLYGONOFFSETELEMENT_H

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

#include <Inventor/elements/SoReplacedElement.h>

class COIN_DLL_API SoPolygonOffsetElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoPolygonOffsetElement);
public:
  static void initClass(void);
protected:
  virtual ~SoPolygonOffsetElement();

public:
  enum Style {
    FILLED = 0x01,
    LINES = 0x02,
    POINTS = 0x04
  };

  virtual void init(SoState * state);
  static void set(SoState * state, SoNode * node, float factor, float units,
                  Style styles, SbBool on);
  static void get(SoState * state, float & factor, float & units,
                  Style & styles, SbBool & on);
  static void getDefault(float & factor, float & units, Style & styles,
                         SbBool & on);

protected:
  Style style;
  SbBool active;
  float offsetfactor;
  float offsetunits;

  virtual void setElt(float factor, float units, Style styles, SbBool on);
};

#endif // !COIN_SOPOLYGONOFFSETELEMENT_H
