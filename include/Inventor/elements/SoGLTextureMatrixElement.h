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

#ifndef COIN_SOGLTEXTUREMATRIXELEMENT_H
#define COIN_SOGLTEXTUREMATRIXELEMENT_H

#include <Inventor/elements/SoTextureMatrixElement.h>


class COIN_DLL_EXPORT SoGLTextureMatrixElement : public SoTextureMatrixElement {
  typedef SoTextureMatrixElement inherited;

  SO_ELEMENT_HEADER(SoGLTextureMatrixElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLTextureMatrixElement();

public:
  virtual void init(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  static void setEltIdentity(void);
  virtual void setElt(const SbMatrix & matrix);
  virtual void makeEltIdentity(void);

  virtual void multElt(const SbMatrix & matrix);

  virtual void translateEltBy(const SbVec3f & translation);
  virtual void rotateEltBy(const SbRotation & translation);
  virtual void scaleEltBy(const SbVec3f & scaleFactor);

private:
  void updategl(void) const;

};

#endif // !COIN_SOGLTEXTUREMATRIXELEMENT_H
