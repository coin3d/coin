#ifndef COIN_SOCOORDINATEELEMENT_H
#define COIN_SOCOORDINATEELEMENT_H

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
#include <Inventor/SbVec4f.h>

class COIN_DLL_API SoCoordinateElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoCoordinateElement);
public:
  static void initClass(void);
protected:
  virtual ~SoCoordinateElement();

public:
  virtual void init(SoState * state);
  static void set3(SoState * const state, SoNode * const node,
                   const int32_t numCoords, const SbVec3f * const coords);
  static void set4(SoState * const state, SoNode * const node,
                   const int32_t numCoords, const SbVec4f * const coords);

  static const SoCoordinateElement *getInstance(SoState * const state);
  int32_t getNum() const;
  SbBool is3D() const;
  const SbVec3f & get3(const int index) const;
  const SbVec4f & get4(const int index) const;
  const SbVec3f *getArrayPtr3() const;
  const SbVec4f *getArrayPtr4() const;

  static SbVec3f getDefault3();
  static SbVec4f getDefault4();


  virtual void print(FILE * file) const;

protected:
  int32_t numCoords;
  const SbVec3f *coords3D;
  const SbVec4f *coords4D;
  SbBool areCoords3D;

private:
  static void clean(void);

  SbVec3f dummy3D;
  SbVec4f dummy4D;
  static SbVec3f * initialdefaultcoords;
};

#endif // !COIN_SOCOORDINATEELEMENT_H
