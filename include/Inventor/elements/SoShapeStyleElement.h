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

#ifndef COIN_SOSHAPESTYLEELEMENT_H
#define COIN_SOSHAPESTYLEELEMENT_H

#include <Inventor/elements/SoSubElement.h>


class COIN_DLL_API SoShapeStyleElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoShapeStyleElement);
public:
  static void initClass(void);
protected:
  virtual ~SoShapeStyleElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  static const SoShapeStyleElement * get(SoState * const state);

  SbBool mightNotRender(void) const;
  SbBool needNormals(void) const;
  SbBool needTexCoords(void) const;
  int getRenderCaseMask(void) const;

  static void setDrawStyle(SoState * const state, const int32_t value);
  static void setComplexityType(SoState * const state,
                                const int32_t value);
  static void setTransparencyType(SoState * const state,
                                  const int32_t value);
  static void setTextureEnabled(SoState * const state, const SbBool value);
  static void setTextureFunction(SoState * const state,
                                 const SbBool value);
  static void setLightModel(SoState * const state, const int32_t value);
  static void setOverrides(SoState * const state, const SbBool value);

  static SbBool isScreenDoor(SoState * const state);
  static int getTransparencyType(SoState * const state);
  SbBool isTextureFunction(void) const;

private:

  static SoShapeStyleElement * getElement(SoState * const state);
  static const SoShapeStyleElement * getConstElement(SoState * const state);

  unsigned int flags;
};

#endif // !COIN_SOSHAPESTYLEELEMENT_H
