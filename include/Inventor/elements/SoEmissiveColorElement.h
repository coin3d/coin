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

#ifndef COIN_SOEMISSIVECOLORELEMENT_H
#define COIN_SOEMISSIVECOLORELEMENT_H

#include <Inventor/elements/SoReplacedElement.h>

class SbColor;


class SoEmissiveColorElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoEmissiveColorElement);
public:
  static void initClass(void);
protected:
  virtual ~SoEmissiveColorElement();

public:
  virtual void init(SoState * state);
  static void set(SoState * const state, SoNode * const node,
                  const int32_t numColors, const SbColor * const colors);

  static const SoEmissiveColorElement * getInstance(SoState *state);

  int32_t getNum() const;
  const SbColor &get(const int index) const;
  const SbColor *getArrayPtr() const;

protected:
  virtual void setElt(const int32_t numColors, const SbColor * const colors);
  int numColors;
  const SbColor *colors;

};

#endif // !COIN_SOEMISSIVECOLORELEMENT_H
