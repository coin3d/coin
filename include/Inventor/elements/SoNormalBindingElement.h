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

#ifndef COIN_SONORMALBINDINGELEMENT_H
#define COIN_SONORMALBINDINGELEMENT_H

#include <Inventor/elements/SoInt32Element.h>


class COIN_DLL_API SoNormalBindingElement : public SoInt32Element {
  typedef SoInt32Element inherited;

  SO_ELEMENT_HEADER(SoNormalBindingElement);
public:
  static void initClass(void);
protected:
  virtual ~SoNormalBindingElement();

public:
// constants: (numbers are adjusted for Inventor 2.0 binary format files)
  enum Binding {
    OVERALL = 2,
    PER_PART,
    PER_PART_INDEXED,
    PER_FACE,
    PER_FACE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED,
    DEFAULT = PER_VERTEX_INDEXED,
    NONE = OVERALL
  }; // enum Binding

  virtual void init(SoState * state);
  static  void set(SoState * const state, SoNode * const node,
                     const Binding binding);
  static  void set(SoState * const state, const Binding binding);
  static  Binding get(SoState * const state);
  static  Binding getDefault();

};

#endif // !COIN_SONORMALBINDINGELEMENT_H
