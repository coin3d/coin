#ifndef COIN_SOLOCALBBOXMATRIXELEMENT_H
#define COIN_SOLOCALBBOXMATRIXELEMENT_H

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

#include <Inventor/elements/SoModelMatrixElement.h>

class COIN_DLL_API SoLocalBBoxMatrixElement : public SoElement {
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

protected:
  SbMatrix localMatrix;

private:
  SbMatrix modelInverseMatrix;

};

#endif // !COIN_SOLOCALBBOXMATRIXELEMENT_H
