#ifndef COIN_SOUNITSELEMENT_H
#define COIN_SOUNITSELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/elements/SoInt32Element.h>

class COIN_DLL_API SoUnitsElement : public SoInt32Element {
  typedef SoInt32Element inherited;

  SO_ELEMENT_HEADER(SoUnitsElement);
public:
  static void initClass(void);
protected:
  virtual ~SoUnitsElement();

public:
// constants:
  enum Units {
    // We have to start at 0, or code in nodes/SoUnits.cpp will crash
    // and burn. --mortene
    METERS = 0,
    CENTIMETERS,
    MILLIMETERS,
    MICROMETERS,
    MICRONS,
    NANOMETERS,
    ANGSTROMS,
    KILOMETERS,
    FEET,
    INCHES,
    POINTS,
    YARDS,
    MILES,
    NAUTICAL_MILES
  };

  virtual void init(SoState * state);

  static  void set(SoState * const state, SoNode * const node,
                   const Units units);
  static  void set(SoState * const state, const Units units);
  static Units get(SoState * const state);
  static Units getDefault();

};

#endif // !COIN_SOUNITSELEMENT_H
