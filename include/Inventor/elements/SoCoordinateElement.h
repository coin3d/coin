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

#ifndef __SOCOORDINATEELEMENT_H__
#define __SOCOORDINATEELEMENT_H__

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>


class SoCoordinateElement : public SoReplacedElement {
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
  SbVec3f dummy3D;
  SbVec4f dummy4D;
  static SbVec3f initialdefaultcoords;

};

#endif // !__SOCOORDINATEELEMENT_H__
