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

#ifndef COIN_SOTEXTUREMATRIXELEMENT_H
#define COIN_SOTEXTUREMATRIXELEMENT_H

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/SbMatrix.h>


class COIN_DLL_EXPORT SoTextureMatrixElement : public SoAccumulatedElement {
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
  virtual void multElt(const SbMatrix & matrix);
  virtual void translateEltBy(const SbVec3f & translation);
  virtual void rotateEltBy(const SbRotation & rotation);
  virtual void scaleEltBy(const SbVec3f & scaleFactor);
  virtual const SbMatrix & getElt(void) const;

  SbMatrix textureMatrix;
};

#endif // !COIN_SOTEXTUREMATRIXELEMENT_H
