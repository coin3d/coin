/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOTEXTUREMATRIXELEMENT_H__
#define __SOTEXTUREMATRIXELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOTEXTUREMATRIXELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOTEXTUREMATRIXELEMENT

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/SbMatrix.h>

class SoTextureMatrixElement : public SoAccumulatedElement {
  typedef SoAccumulatedElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoTextureMatrixElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoTextureMatrixElement(void);

  static void initClass(void);

protected:
  SoTextureMatrixElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

  static  void makeIdentity(SoState * const state, SoNode * const node);

  static  void mult(SoState * const state, SoNode * const node,
              const SbMatrix & matrix);

  static  void translateBy(SoState * const state, SoNode * const node,
              const SbVec3f & translation);
  static  void rotateBy(SoState * const state, SoNode * const node,
                const SbRotation & rotation);
  static  void scaleBy(SoState * const state, SoNode * const node,
               const SbVec3f & scaleFactor);

  static  const SbMatrix & get(SoState * const state);

  virtual void print(FILE * file) const;
  static  void emptyMatrix(SoState * const state);

protected:
  virtual void makeEltIdentity(void);
  virtual void multElt(const SbMatrix & matrix);
  virtual void translateEltBy(const SbVec3f & translation);
  virtual void rotateEltBy(const SbRotation & rotation);
  virtual void scaleEltBy(const SbVec3f & scaleFactor);
  virtual const SbMatrix & getElt(void) const;

  SbMatrix textureMatrix;

};

#endif // !__SOTEXTUREMATRIXELEMENT_H__
