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

#ifndef COIN_SOLIGHTELEMENT_H
#define COIN_SOLIGHTELEMENT_H

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/lists/SoNodeList.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbMatrix.h>

class SoLight;

class SoLightElement : public SoAccumulatedElement {
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
