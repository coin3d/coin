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

#ifndef COIN_SOLIGHTELEMENT_H
#define COIN_SOLIGHTELEMENT_H

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/lists/SoNodeList.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbMatrix.h>

class SoLight;

class COIN_DLL_EXPORT SoLightElement : public SoAccumulatedElement {
  typedef SoAccumulatedElement inherited;

  SO_ELEMENT_HEADER(SoLightElement);
public:
  static void initClass(void);
protected:
  virtual ~SoLightElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  static  void add(SoState * const state, SoLight * const light,
                   const SbMatrix & matrix);
  static  const SoNodeList & getLights(SoState * const state);
  static  const SbMatrix & getMatrix(SoState * const state,
                                     const int index);

protected:
  SoNodeList lights;
  SbList <SbMatrix> * matrixlist;

private:

  // dummy class needed to initialize didalloc when constructed.
  class so_light_elem_flag {
  public:
    so_light_elem_flag(void) {
      this->state = FALSE;
    }
    SbBool state;
  };
  so_light_elem_flag didalloc;
};

#endif // !COIN_SOLIGHTELEMENT_H
