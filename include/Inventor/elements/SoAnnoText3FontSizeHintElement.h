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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOANNOTEXT3FONTSIZEHINTELEMENT_H
#define COIN_SOANNOTEXT3FONTSIZEHINTELEMENT_H

#include <Inventor/elements/SoInt32Element.h>


class COIN_DLL_EXPORT SoAnnoText3FontSizeHintElement : public SoInt32Element {
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
