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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOGLNORMALIZEELEMENT_H
#define COIN_SOGLNORMALIZEELEMENT_H

#include <Inventor/elements/SoSubElement.h>


class COIN_DLL_EXPORT SoGLNormalizeElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoGLNormalizeElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLNormalizeElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo() const;

  static void setMatrixState(SoState * const state,
                             const SbBool valid);
  static void setUnitNormals(SoState * const state,
                             const SbBool unitNormals);

  void evaluate() const;
  void forceSend(const SbBool normalsAreUnitLength) const;

private:

  SbBool okMatrix;
  SbBool unitNormals;
  SbBool glnormalize;

  void updategl(const SbBool normalize);
};

#endif // !COIN_SOGLNORMALIZEELEMENT_H
