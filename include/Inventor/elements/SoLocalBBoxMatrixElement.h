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

#ifndef COIN_SOLOCALBBOXMATRIXELEMENT_H
#define COIN_SOLOCALBBOXMATRIXELEMENT_H

#include <Inventor/elements/SoModelMatrixElement.h>


class COIN_DLL_EXPORT SoLocalBBoxMatrixElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoLocalBBoxMatrixElement);
public:
  static void initClass(void);
protected:
  virtual ~SoLocalBBoxMatrixElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);

  static void makeIdentity(SoState * const state);

  static void set(SoState * const state, const SbMatrix & matrix);

  static void mult(SoState * const state, const SbMatrix & matrix);
  static void translateBy(SoState * const state,
                           const SbVec3f & translation);
  static void rotateBy(SoState * const state, const SbRotation & rotation);
  static void scaleBy(SoState * const state, const SbVec3f & scaleFactor);

  static SbMatrix pushMatrix(SoState * const state);
  static void popMatrix(SoState * const state, const SbMatrix & matrix);

  static void resetAll(SoState * const state);

  static const SbMatrix & get(SoState * const state);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  virtual void print(FILE * file) const;

protected:
  SbMatrix localMatrix;

private:
  SbMatrix modelInverseMatrix;

};

#endif // !COIN_SOLOCALBBOXMATRIXELEMENT_H
