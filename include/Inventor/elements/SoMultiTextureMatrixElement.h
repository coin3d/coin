#ifndef COIN_SOMULTITEXTUREMATRIXELEMENT_H
#define COIN_SOMULTITEXTUREMATRIXELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/SbMatrix.h>

class SoMultiTextureMatrixElementP;

class COIN_DLL_API SoMultiTextureMatrixElement : public SoAccumulatedElement {
  typedef SoAccumulatedElement inherited;

  SO_ELEMENT_HEADER(SoMultiTextureMatrixElement);
public:
  static void initClass(void);
protected:
  virtual ~SoMultiTextureMatrixElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  static void mult(SoState * const state, SoNode * const node,
                   const int unit,
                   const SbMatrix & matrix);
  static const SbMatrix & get(SoState * const state, const int unit);
  
  class UnitData {
  public:
    SbMatrix textureMatrix;
  };

protected:
  UnitData & getUnitData(const int unit);
  const UnitData & getUnitData(const int unit) const;
  
  virtual void multElt(const int unit, const SbMatrix & matrix);
  virtual const SbMatrix & getElt(const int unit) const;

private:
  SoMultiTextureMatrixElementP * pimpl;
};

#endif // !COIN_SOTEXTUREMATRIXELEMENT_H
