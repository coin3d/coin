#ifndef COIN_SOGLMODELMATRIXELEMENT_H
#define COIN_SOGLMODELMATRIXELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/elements/SoModelMatrixElement.h>

class COIN_DLL_API SoGLModelMatrixElement : public SoModelMatrixElement {
  typedef SoModelMatrixElement inherited;

  SO_ELEMENT_HEADER(SoGLModelMatrixElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLModelMatrixElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

protected:
  virtual void makeEltIdentity();
  virtual void setElt(const SbMatrix & matrix);
  virtual void multElt(const SbMatrix & matrix);

  virtual void translateEltBy(const SbVec3f & translation);
  virtual void rotateEltBy(const SbRotation & translation);
  virtual void scaleEltBy(const SbVec3f & scaleFactor);
  virtual SbMatrix pushMatrixElt();
  virtual void popMatrixElt(const SbMatrix & matrix);

private:
  SoState * state;
  uint32_t viewEltNodeId;
  SbBool stackoverflow;
};

#endif // !COIN_SOGLMODELMATRIXELEMENT_H
