#ifndef COIN_SOTEXTUREOVERRIDEELEMENT_H
#define COIN_SOTEXTUREOVERRIDEELEMENT_H

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

#include <Inventor/elements/SoSubElement.h>

class COIN_DLL_API SoTextureOverrideElement : public SoElement {
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

#endif // !COIN_SOTEXTUREOVERRIDEELEMENT_H
