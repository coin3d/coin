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

#ifndef COIN_SOTEXTUREMATRIXELEMENT_H
#define COIN_SOTEXTUREMATRIXELEMENT_H

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/SbMatrix.h>


class COIN_DLL_API SoTextureMatrixElement : public SoAccumulatedElement {
  typedef SoAccumulatedElement inherited;

  SO_ELEMENT_HEADER(SoTextureMatrixElement);
public:
  static void initClass(void);
protected:
  virtual ~SoTextureMatrixElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  static void makeIdentity(SoState * const state, SoNode * const node);

  static void set(SoState * const state, SoNode * const node,
                  const SbMatrix & matrix);
  static void mult(SoState * const state, SoNode * const node,
                   const SbMatrix & matrix);

  static void translateBy(SoState * const state, SoNode * const node,
                           const SbVec3f & translation);
  static void rotateBy(SoState * const state, SoNode * const node,
                        const SbRotation & rotation);
  static void scaleBy(SoState * const state, SoNode * const node,
                      const SbVec3f & scaleFactor);

  static const SbMatrix & get(SoState * const state);
  static void emptyMatrix(SoState * const state);

protected:
  virtual void makeEltIdentity(void);
  virtual void setElt(const SbMatrix & matrix);
  virtual void multElt(const SbMatrix & matrix);
  virtual void translateEltBy(const SbVec3f & translation);
  virtual void rotateEltBy(const SbRotation & rotation);
  virtual void scaleEltBy(const SbVec3f & scaleFactor);
  virtual const SbMatrix & getElt(void) const;

  SbMatrix textureMatrix;
};

#endif // !COIN_SOTEXTUREMATRIXELEMENT_H
