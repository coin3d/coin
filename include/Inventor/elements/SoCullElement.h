/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SOCULLELEMENT_H
#define COIN_SOCULLELEMENT_H

#include <Inventor/elements/SoSubElement.h>
#include <Inventor/SbPlane.h>

class SbBox3f;

class COIN_DLL_EXPORT SoCullElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoCullElement);
public:
  static void initClass(void);
protected:
  virtual ~SoCullElement();

public:

  virtual void init(SoState * state);
  virtual void push(SoState * state);

  static void addPlanes(SoState * state, const SbPlane * planes, const int numplanes);
  static SbBool cullBox(SoState * state, const SbBox3f & box, const SbBool transform = TRUE);
  static SbBool cullTest(SoState * state, const SbBox3f & box, const SbBool transform = TRUE);
  static SbBool completelyInside(SoState * state);

  virtual SbBool matches(const SoElement * elt) const;
  virtual SoElement * copyMatchInfo(void) const;

private:

  static SbBool docull(SoState * state, const SbBox3f & box, const SbBool transform,
                       const SbBool updateelem);

  SbPlane plane[32];
  int numplanes;
  unsigned int flags;
};

#endif // !COIN_SOCULLELEMENT_H
