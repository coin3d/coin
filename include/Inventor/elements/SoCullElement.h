#ifndef COIN_SOCULLELEMENT_H
#define COIN_SOCULLELEMENT_H

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

#include <Inventor/elements/SoSubElement.h>
#include <Inventor/SbPlane.h>

class SbBox3f;
class SbViewVolume;

class COIN_DLL_API SoCullElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoCullElement);
public:
  static void initClass(void);
protected:
  virtual ~SoCullElement();

public:

  virtual void init(SoState * state);
  virtual void push(SoState * state);
  
  static void setViewVolume(SoState * state, const SbViewVolume & vv);
  static void addPlane(SoState * state, const SbPlane & newplane);
  static SbBool cullBox(SoState * state, const SbBox3f & box, const SbBool transform = TRUE);
  static SbBool cullTest(SoState * state, const SbBox3f & box, const SbBool transform = TRUE);
  static SbBool completelyInside(SoState * state);

  virtual SbBool matches(const SoElement * elt) const;
  virtual SoElement * copyMatchInfo(void) const;

private:

  enum { MAXPLANES = 32 };

  static SbBool docull(SoState * state, const SbBox3f & box, const SbBool transform,
                       const SbBool updateelem);

  SbPlane plane[MAXPLANES];
  int numplanes;
  unsigned int flags;
  int vvindex;
};

#endif // !COIN_SOCULLELEMENT_H
