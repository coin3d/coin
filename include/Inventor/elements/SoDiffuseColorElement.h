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

#ifndef COIN_SODIFFUSECOLORELEMENT_H
#define COIN_SODIFFUSECOLORELEMENT_H

#include <Inventor/elements/SoReplacedElement.h>

class SbColor;


class COIN_DLL_API SoDiffuseColorElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoDiffuseColorElement);
public:
  static void initClass(void);
protected:
  virtual ~SoDiffuseColorElement();

public:
  virtual void init(SoState * state);

  static void set(SoState * const state, SoNode * const node,
                  const int32_t numColors, const SbColor * const colors);

  static void set(SoState * const state, SoNode * const node,
                  const int32_t numColors, const uint32_t * const colors,
                  const SbBool packedtransparency = FALSE);

  static const SoDiffuseColorElement * getInstance(SoState *state);

  int32_t getNum(void) const;
  const SbColor &get(const int index) const;

  SbBool isPacked(void) const;
  SbBool hasPackedTransparency(void) const;

  const SbColor *getColorArrayPtr() const;
  const uint32_t *getPackedArrayPtr() const;

protected:
  friend class SoMaterialBundle;
  virtual void setElt(const int32_t numColors, const SbColor * const colors);
  virtual void setElt(const int32_t numColors,
                      const uint32_t * const packedcolors,
                      const SbBool packedtransparency);

  int numColors;
  const SbColor *colors;
  const uint32_t *packedColors;
  SbBool packedTransparency;
};

#endif // !COIN_SODIFFUSECOLORELEMENT_H
