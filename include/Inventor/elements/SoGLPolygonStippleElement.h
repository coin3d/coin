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

#ifndef __SOGLPOLYGONSTIPPLEELEMENT_H__
#define __SOGLPOLYGONSTIPPLEELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT

#include <Inventor/elements/SoElement.h>

class SoGLPolygonStippleElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoGLPolygonStippleElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLPolygonStippleElement();

public:
  virtual void init(SoState *state);

  static void set(SoState * const state, const SbBool onoff);

  static void setTransparency(SoState * const state,
                              const float transparency);

  static SbBool get(SoState * const state);

  virtual void push(SoState *state);
  virtual void pop(SoState *state, const SoElement *prevTopElement);

  virtual SbBool matches(const SoElement *elt) const;
  virtual SoElement *copyMatchInfo() const;

  virtual void print(FILE *fp) const;

  void evaluate() const;

private:
  static unsigned char patterns[64 + 1][32 * 4];

  void updategl();
  SbBool isEnabled;
  int pattern;

  SbBool currentEnabled;
  int currentPattern;

};

#endif // !__SOGLPOLYGONSTIPPLEELEMENT_H__
