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

#ifndef COIN_SOTEXTUREOVERRIDEELEMENT_H
#define COIN_SOTEXTUREOVERRIDEELEMENT_H

#include <Inventor/elements/SoSubElement.h>

class COIN_DLL_EXPORT SoTextureOverrideElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoTextureOverrideElement);

public:
  static void initClass(void);
protected:
  virtual ~SoTextureOverrideElement();

  enum {
    TEXTURE_QUALITY = 0x1,
    TEXTURE_IMAGE   = 0x2
  };
public:
  virtual SbBool matches(const SoElement *element) const;
  virtual SoElement *copyMatchInfo() const;

  virtual void init(SoState *state);
  virtual void push(SoState *state);
  static SbBool getQualityOverride(SoState *state);
  static SbBool getImageOverride(SoState *state);

  static void setQualityOverride(SoState *state, const SbBool value);
  static void setImageOverride(SoState *state, const SbBool value);

  virtual void print(FILE *fp) const;

private:
  uint32_t flags;
};

#endif // COIN_SOTEXTUREOVERRIDEELEMENT_H
