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

#ifndef COIN_SOPROJECTIONMATRIXELEMENT_H
#define COIN_SOPROJECTIONMATRIXELEMENT_H

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbMatrix.h>


class COIN_DLL_API SoProjectionMatrixElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoProjectionMatrixElement);
public:
  static void initClass(void);
protected:
  virtual ~SoProjectionMatrixElement();

public:
  virtual void init(SoState * state);

  static void set(SoState * const state, SoNode * const node,
                  const SbMatrix & matrix);
  static const SbMatrix &get(SoState * const state);

protected:
  SbMatrix projectionMatrix;
  virtual void setElt(SbMatrix const & matrix);

};

#endif // !COIN_SOPROJECTIONMATRIXELEMENT_H
