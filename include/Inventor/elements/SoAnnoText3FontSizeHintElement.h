#ifndef COIN_SOANNOTEXT3FONTSIZEHINTELEMENT_H
#define COIN_SOANNOTEXT3FONTSIZEHINTELEMENT_H

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

class COIN_DLL_API SoAnnoText3FontSizeHintElement : public SoInt32Element {
  typedef SoInt32Element inherited;

  SO_ELEMENT_HEADER(SoAnnoText3FontSizeHintElement);
public:
  static void initClass(void);
protected:
  virtual ~SoAnnoText3FontSizeHintElement();

public:
  enum FontSizeHint {
    ANNOTATION, FIT_TEXT_VECTOR
  };

  virtual void init(SoState * state);
  static void set(SoState * const state, const FontSizeHint hint);
  static FontSizeHint get(SoState * const state);
  static FontSizeHint getDefault(void);

  static void set(SoState * const state, SoNode * const node,
                  const FontSizeHint hint);
};

#endif // !COIN_SOANNOTEXT3FONTSIZEHINTELEMENT_H
