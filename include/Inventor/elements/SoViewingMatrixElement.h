#ifndef COIN_SOVIEWINGMATRIXELEMENT_H
#define COIN_SOVIEWINGMATRIXELEMENT_H

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

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbMatrix.h>

class COIN_DLL_API SoViewingMatrixElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoViewingMatrixElement);
public:
  static void initClass(void);
protected:
  virtual ~SoViewingMatrixElement();

public:
  virtual void init(SoState * state);
  static  void set(SoState * const state, SoNode * const node,
                   const SbMatrix & ViewingMatrix);
  static  const SbMatrix &get(SoState * const state);

protected:
  virtual void setElt(const SbMatrix &matrix);
  SbMatrix viewingMatrix;

};

#endif // !COIN_SOVIEWINGMATRIXELEMENT_H
