/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_SOLIGHTELEMENT_H
#define COIN_SOLIGHTELEMENT_H

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/lists/SoNodeList.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbMatrix.h>

class SoLight;

class COIN_DLL_API SoLightElement : public SoAccumulatedElement {
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
