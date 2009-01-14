#ifndef COIN_SOGLTEXTUREMATRIXELEMENT_H
#define COIN_SOGLTEXTUREMATRIXELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/elements/SoTextureMatrixElement.h>

class COIN_DLL_API SoGLTextureMatrixElement : public SoTextureMatrixElement {
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
