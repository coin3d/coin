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

#ifndef COIN_SONORMALELEMENT_H
#define COIN_SONORMALELEMENT_H

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
