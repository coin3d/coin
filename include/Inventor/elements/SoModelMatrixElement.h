#ifndef COIN_SOMODELMATRIXELEMENT_H
#define COIN_SOMODELMATRIXELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/lists/SbPList.h>

class COIN_DLL_API SoModelMatrixElement : public SoAccumulatedElement {
  typedef SoAccumulatedElement inherited;

  SO_ELEMENT_HEADER(SoModelMatrixElement);
public:
  static void initClass(void);
protected:
  virtual ~SoModelMatrixElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual SbBool matches(const SoElement * element) const;
  static void makeIdentity(SoState * const state, SoNode * const node);
  static void set(SoState * const state, SoNode * const node,
                  const SbMatrix & matrix);
  static void setCullMatrix(SoState * state, SoNode * node,
                            const SbMatrix & matrix);
  static void mult(SoState * const state, SoNode * const node,
                   const SbMatrix & matrix);
  static void translateBy(SoState * const state, SoNode * const node,
                          const SbVec3f & translation);
  static void rotateBy(SoState * const state, SoNode * const node,
                       const SbRotation & rotation);
  static void scaleBy(SoState * const state, SoNode * const node,
                      const SbVec3f & scaleFactor);

  static SbMatrix pushMatrix(SoState * const state);
  static void popMatrix(SoState * const state, const SbMatrix & matrix);

  static const SbMatrix & getCombinedCullMatrix(SoState * const state);
  static const SbMatrix & get(SoState * const state);
  static const SbMatrix & get(SoState * const state, SbBool & isIdentity);

  const SbMatrix & getModelMatrix(void) const;

protected:
  virtual void makeEltIdentity(void);
  virtual void setElt(const SbMatrix & matrix);
  virtual void multElt(const SbMatrix & matrix);
  virtual void translateEltBy(const SbVec3f & translation);
  virtual void rotateEltBy(const SbRotation & rotation);
  virtual void scaleEltBy(const SbVec3f & scaleFactor);
  virtual SbMatrix pushMatrixElt(void);
  virtual void popMatrixElt(const SbMatrix & matrix);

protected:
  SbMatrix modelMatrix;
  SbMatrix cullMatrix;
  SbMatrix combinedMatrix;
  uint32_t flags;

};

#endif // !COIN_SOMODELMATRIXELEMENT_H
