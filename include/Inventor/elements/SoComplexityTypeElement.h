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

#ifndef COIN_SOCOMPLEXITYTYPEELEMENT_H
#define COIN_SOCOMPLEXITYTYPEELEMENT_H

#include <Inventor/elements/SoInt32Element.h>


class COIN_DLL_API SoComplexityTypeElement : public SoInt32Element {
  typedef SoInt32Element inherited;

  SO_ELEMENT_HEADER(SoComplexityTypeElement);
public:
  static void initClass(void);
protected:
  virtual ~SoComplexityTypeElement();

public:
// constants:
  enum Type {
    OBJECT_SPACE,
    SCREEN_SPACE,
    BOUNDING_BOX
  };

  virtual void init(SoState * state);

  static  void set(SoState * const state, SoNode * const node,
                   const Type type);
  static  void set(SoState * const state, const Type type);
  static  Type get(SoState * const state);
  static  Type getDefault();

};

#endif // !COIN_SOCOMPLEXITYTYPEELEMENT_H
