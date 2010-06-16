#ifndef COIN_SONORMALBINDINGELEMENT_H
#define COIN_SONORMALBINDINGELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

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
