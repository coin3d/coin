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

#ifndef COIN_SOVIEWINGMATRIXELEMENT_H
#define COIN_SOVIEWINGMATRIXELEMENT_H

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
