/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOSHAPESTYLEELEMENT_H__
#define __SOSHAPESTYLEELEMENT_H__

#include <Inventor/elements/SoElement.h>

#if defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOSHAPESTYLEELEMENT


class SoShapeStyleElement : public SoElement {
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

  virtual void print(FILE * file) const;

  SbBool isTextureFunction(void) const;

private:

  static SoShapeStyleElement * getElement(SoState * const state);
  static const SoShapeStyleElement * getConstElement(SoState * const state);

  unsigned int flags;
};

#endif // !__SOSHAPESTYLEELEMENT_H__
