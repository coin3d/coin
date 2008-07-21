#ifndef COIN_SOVERTEXATTRIBUTEBINDINGELEMENT_H
#define COIN_SOVERTEXATTRIBUTEBINDINGELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/elements/SoInt32Element.h>

class COIN_DLL_API SoVertexAttributeBindingElement : public SoInt32Element {
  typedef SoInt32Element inherited;

  SO_ELEMENT_HEADER(SoVertexAttributeBindingElement);
public:
  static void initClass(void);
protected:
  virtual ~SoVertexAttributeBindingElement();

public:
  enum Binding {
    OVERALL = 0,
    // PER_FACE = 1,
    // PER_FACE_INDEXED = 2,
    PER_VERTEX = 3,
    PER_VERTEX_INDEXED = 4,
    DEFAULT = PER_VERTEX_INDEXED
  }; // enum Binding

  virtual void init(SoState * state);

  static  void set(SoState * const state, SoNode * const node,
                   const Binding binding);
  static  void set(SoState * const state, const Binding binding);
  static  Binding get(SoState * const state);
  static  Binding getDefault();

};

#endif // !COIN_SOVERTEXATTRIBUTEBINDINGELEMENT_H
