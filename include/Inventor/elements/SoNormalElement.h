#ifndef COIN_SONORMALELEMENT_H
#define COIN_SONORMALELEMENT_H

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

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbVec3f.h>
#include <assert.h>

class COIN_DLL_API SoNormalElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoNormalElement);
public:
  static void initClass(void);
protected:
  virtual ~SoNormalElement();

public:
  virtual void init(SoState * state);

  static void set(SoState * const state, SoNode * const node,
                  const int32_t numNormals, const SbVec3f * const normals,
                  const SbBool normalsAreUnitLength = FALSE);

  static const SoNormalElement * getInstance(SoState * const state);
  int32_t getNum(void) const;
  const SbVec3f & get(const int index) const;
  const SbVec3f *getArrayPtr(void) const;

  SbBool areNormalsUnitLength(void) const;

protected:
  int32_t numNormals;
  const SbVec3f * normals;
  SbBool unitLength;
};

#endif // !COIN_SONORMALELEMENT_H
