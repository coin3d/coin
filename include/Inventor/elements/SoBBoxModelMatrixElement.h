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

#ifndef COIN_SOBBOXMODELMATRIXELEMENT_H
#define COIN_SOBBOXMODELMATRIXELEMENT_H

#include <Inventor/elements/SoModelMatrixElement.h>

class SoLocalBBoxMatrixElement;


class SoBBoxModelMatrixElement : public SoModelMatrixElement {
  typedef SoModelMatrixElement inherited;

  SO_ELEMENT_HEADER(SoBBoxModelMatrixElement);
public:
  static void initClass(void);
protected:
  virtual ~SoBBoxModelMatrixElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);

  static  void reset(SoState * const state, SoNode * const node);
  static  void pushMatrix(SoState * const state, SbMatrix & matrix,
                          SbMatrix & localmatrix);
  static  void popMatrix(SoState * const state, const SbMatrix & matrix,
                         const SbMatrix & localmatrix);

protected:
  virtual void makeEltIdentity(void);
  virtual void setElt(const SbMatrix & matrix);
  virtual void multElt(const SbMatrix & matrix);
  virtual void translateEltBy(const SbVec3f & translation);
  virtual void rotateEltBy(const SbRotation & translation);
  virtual void scaleEltBy(const SbVec3f & scaleFactor);

// the following two functions should not be called - only for debug usage
  virtual SbMatrix pushMatrixElt();
  virtual void popMatrixElt(const SbMatrix & matrix);

private:
  SoLocalBBoxMatrixElement *getLocal();

  SoState *state;
};

#endif // !COIN_SOBBOXMODELMATRIXELEMENT_H
