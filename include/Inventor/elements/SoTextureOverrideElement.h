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

#ifndef COIN_SOTEXTUREOVERRIDEELEMENT_H
#define COIN_SOTEXTUREOVERRIDEELEMENT_H

#include <Inventor/elements/SoSubElement.h>

class SoTextureOverrideElement : public SoElement {
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
