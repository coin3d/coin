/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOPROFILECOORDINATEELEMENT_H
#define COIN_SOPROFILECOORDINATEELEMENT_H

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <assert.h>

/*
 * TODO
 * - conversion between coord2 and coord3 (done in Inventor)
 */


class COIN_DLL_API SoProfileCoordinateElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoProfileCoordinateElement);
public:
  static void initClass(void);
protected:
  virtual ~SoProfileCoordinateElement();

public:
  virtual void init(SoState * state);
  static void set2(SoState * const state, SoNode * const node,
                   const int32_t numCoords, const SbVec2f * const coords);
  static void set3(SoState * const state, SoNode * const node,
                   const int32_t numCoords, const SbVec3f * const coords);
  static const SoProfileCoordinateElement * getInstance(SoState * const state);
  int32_t getNum(void) const;
  const SbVec2f & get2(const int index) const;
  const SbVec3f & get3(const int index) const;

  SbBool is2D(void) const;

  static SbVec2f getDefault2(void);
  static SbVec3f getDefault3(void);

  const SbVec2f * getArrayPtr2(void) const;
  const SbVec3f * getArrayPtr3(void) const;

protected:
  int32_t numCoords;
  const SbVec2f * coords2;
  const SbVec3f * coords3;
  SbBool coordsAre2D;

private:
  static void clean(void);
  static SbVec2f * initdefaultcoords;
};

#endif // !COIN_SOPROFILECOORDINATEELEMENT_H
