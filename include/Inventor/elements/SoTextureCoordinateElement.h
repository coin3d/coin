#ifndef COIN_SOTEXTURECOORDINATEELEMENT_H
#define COIN_SOTEXTURECOORDINATEELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec4f.h>

/*
 * TODO
 * - conversion between 2D and 4D (like Open Inventor)
 */

typedef const SbVec4f & SoTextureCoordinateFunctionCB(void * userdata,
                                                      const SbVec3f & point,
                                                      const SbVec3f & normal);

class COIN_DLL_API SoTextureCoordinateElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoTextureCoordinateElement);
public:
  static void initClass(void);
protected:
  virtual ~SoTextureCoordinateElement();

public:

  enum CoordType {
    NONE = 0,
    TEXGEN = 0,
    EXPLICIT = 1,
    FUNCTION = 2,
    DEFAULT = 3
  };

  virtual void init(SoState * state);

  static void setDefault(SoState * const state, SoNode * const node);
  static void setFunction(SoState * const state, SoNode * const node,
                          SoTextureCoordinateFunctionCB * const func,
                          void * const userdata);

  static void set2(SoState * const state, SoNode * const node,
                   const int32_t numCoords, const SbVec2f * const coords);
  static void set4(SoState * const state, SoNode * const node,
                   const int32_t numCoords, const SbVec4f * const coords);

  static CoordType getType(SoState * const state);
  virtual CoordType getType() const;

  static const SoTextureCoordinateElement *getInstance(SoState * const state);

  const SbVec4f &get(const SbVec3f & point,
                     const SbVec3f & normal) const;
  int32_t getNum() const;
  SbBool is2D() const;

  const SbVec2f &get2(const int index) const;
  const SbVec4f &get4(const int index) const;

  const SbVec2f *getArrayPtr2() const;
  const SbVec4f *getArrayPtr4() const;

protected:
  CoordType whatKind;
  SoTextureCoordinateFunctionCB *funcCB;
  void *funcCBData;
  int32_t numCoords;
  const SbVec2f *coords2;
  const SbVec4f *coords4;
  SbBool coordsAre2D;
  SbVec2f convert2;
  SbVec4f convert4;

};

#endif // !COIN_SOTEXTURECOORDINATEELEMENT_H
