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

#ifndef __SOGLTEXTUREMATRIXELEMENT_H__
#define __SOGLTEXTUREMATRIXELEMENT_H__

#include <Inventor/elements/SoTextureMatrixElement.h>


class SoGLTextureMatrixElement : public SoTextureMatrixElement {
  typedef SoTextureMatrixElement inherited;

  SO_ELEMENT_HEADER(SoGLTextureMatrixElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLTextureMatrixElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
              const SoElement * prevTopElement);

  static  void setEltIdentity(void);
  virtual void makeEltIdentity(void);

  virtual void multElt(const SbMatrix & matrix);

  virtual void translateEltBy(const SbVec3f & translation);
  virtual void rotateEltBy(const SbRotation & translation);
  virtual void scaleEltBy(const SbVec3f & scaleFactor);

private:
  void updategl() const;

};

#endif // !__SOGLTEXTUREMATRIXELEMENT_H__
